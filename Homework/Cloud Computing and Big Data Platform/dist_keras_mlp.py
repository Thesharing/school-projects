"""MNIST classification using Distributed Keras.

ATTENTION:
Before running this example, make sure you put the MNIST dataset
on HDFS.
1. unzip mnist.zip
2. hdfs dfs -mkdir data
3. hdfs dfs -copyFromLocal mnist_train.csv data/mnist_train.csv
4. hdfs dfs -copyFromLocal mnist_test.csv data/mnist_test.csv
"""

from distkeras.evaluators import *
from distkeras.predictors import *
from distkeras.trainers import *
from distkeras.transformers import *
from distkeras.utils import *

from keras.layers.convolutional import *
from keras.layers.core import *
from keras.models import Sequential
from keras.optimizers import *

from pyspark import SparkConf
from pyspark import SparkContext
from pyspark.sql import SparkSession

from pyspark.ml.evaluation import MulticlassClassificationEvaluator
from pyspark.ml.feature import OneHotEncoder
from pyspark.ml.feature import StandardScaler
from pyspark.ml.feature import StringIndexer
from pyspark.ml.feature import VectorAssembler

import pwd
import os

# First, setup the Spark variables. You can modify them to your needs.
application_name = "Distributed Keras MNIST Notebook"
using_spark_2 = True
local = True
path_train = "/data/mnist_train.csv"
path_test = "/data/mnist_test.csv"
if local:
    # Tell master to use local resources.
    master = "local[8]"
    num_processes = 8
    num_executors = 1
else:
    # Tell master to use YARN.
    master = "yarn"
    # master = "spark://172.17.0.2:7077"
    num_executors = 1
    num_processes = 4

# This variable is derived from the number of cores and executors, and will be used to assign the number of model trainers.
num_workers = num_executors * num_processes

print("Number of desired executors: " + repr(num_executors))
print("Number of desired processes / executor: " + repr(num_processes))
print("Total number of workers: " + repr(num_workers))

# Use the DataBricks CSV reader, this has some nice functionality regarding invalid values.
os.environ['PYSPARK_SUBMIT_ARGS'] = '--packages com.databricks:spark-csv_2.10:1.4.0 pyspark-shell'

conf = SparkConf()
conf.set("spark.app.name", application_name)
conf.set("spark.master", master)
conf.set("spark.executor.cores", repr(num_processes))
conf.set("spark.executor.instances", repr(num_executors))
conf.set("spark.driver.memory", "4g")
conf.set("spark.executor.memory", "4g")
conf.set("spark.worker.memory", "4g")
conf.set("spark.python.worker.memory", "2g")
conf.set("spark.locality.wait", "0")
conf.set("spark.serializer", "org.apache.spark.serializer.KryoSerializer");
conf.set("spark.local.dir", "/tmp/" + get_os_username() + "/dist-keras");
# conf.set("spark.yarn.scheduler.heartbeat.interval-ms", 36000000);
# conf.set("spark.executor.heartbeatInterval", 36000000);
# conf.set("spark.network.timeout", 36000000);

# Check if the user is running Spark 2.0 +
if using_spark_2:
    sc = SparkSession.builder.config(conf=conf) \
        .appName(application_name) \
        .getOrCreate()
else:
    # Create the Spark context.
    sc = SparkContext(conf=conf)
    # Add the missing imports
    from pyspark import SQLContext

    sqlContext = SQLContext(sc)
# Check if we are using Spark 2.0
if using_spark_2:
    reader = sc
else:
    reader = sqlContext
# Read the training dataset.
raw_dataset_train = reader.read.format('com.databricks.spark.csv') \
    .options(header='true', inferSchema='true') \
    .load(path_train)
# Read the testing dataset.
raw_dataset_test = reader.read.format('com.databricks.spark.csv') \
    .options(header='true', inferSchema='true') \
    .load(path_test)
# First, we would like to extract the desired features from the raw dataset.
# We do this by constructing a list with all desired columns.
# This is identical for the test set.
features = raw_dataset_train.columns
features.remove('label')

# Next, we use Spark's VectorAssembler to "assemble" (create) a vector of all desired features.
# http://spark.apache.org/docs/latest/ml-features.html#vectorassembler
vector_assembler = VectorAssembler(inputCols=features, outputCol="features")
# This transformer will take all columns specified in features, and create an additional column
# "features" which will contain all the desired features aggregated into a single vector.
dataset_train = vector_assembler.transform(raw_dataset_train)
dataset_test = vector_assembler.transform(raw_dataset_test)

# Define the number of output classes.
nb_classes = 10
encoder = OneHotTransformer(nb_classes, input_col="label", output_col="label_encoded")
dataset_train = encoder.transform(dataset_train)
dataset_test = encoder.transform(dataset_test)

# Allocate a MinMaxTransformer from Distributed Keras to normalize the features..
# o_min -> original_minimum
# n_min -> new_minimum
transformer = MinMaxTransformer(n_min=0.0, n_max=1.0, \
                                o_min=0.0, o_max=250.0, \
                                input_col="features", \
                                output_col="features_normalized")
# Transform the dataset.
dataset_train = transformer.transform(dataset_train)
dataset_test = transformer.transform(dataset_test)

# Keras expects the vectors to be in a particular shape, we can reshape the
# vectors using Spark.
reshape_transformer = ReshapeTransformer("features_normalized", "matrix", (784, ))
dataset_train = reshape_transformer.transform(dataset_train)
dataset_test = reshape_transformer.transform(dataset_test)

# Now, create a Keras model.
# Taken from Keras MNIST example.

# Declare model parameters.
img_rows, img_cols = 28, 28
# number of convolutional filters to use
nb_filters = 32
# size of pooling area for max pooling
pool_size = (2, 2)
# convolution kernel size
kernel_size = (3, 3)
input_shape = (img_rows, img_cols, 1)

# Construct the model.
convnet = Sequential()
convnet.add(Dense(512, activation='relu', input_shape=(784,)))
convnet.add(Dropout(0.2))
convnet.add(Dense(512, activation='relu'))
convnet.add(Dropout(0.2))
convnet.add(Dense(nb_classes, activation='softmax'))

# Define the optimizer and the loss.
optimizer_convnet = 'adam'
loss_convnet = 'categorical_crossentropy'

# Print the summary.
convnet.summary()


# We can also evaluate the dataset in a distributed manner.
# However, for this we need to specify a procedure how to do this.
def evaluate_accuracy(model, test_set, features="matrix"):
    evaluator = AccuracyEvaluator(prediction_col="prediction_index", label_col="label")
    predictor = ModelPredictor(keras_model=model, features_col=features)
    transformer = LabelIndexTransformer(output_dim=nb_classes)
    test_set = test_set.select(features, "label")
    test_set = predictor.predict(test_set)
    test_set = transformer.transform(test_set)
    score = evaluator.evaluate(test_set)

    return score


# Select the desired columns, this will reduce network usage.
dataset_train = dataset_train.select("features_normalized", "matrix", "label", "label_encoded")
dataset_test = dataset_test.select("features_normalized", "matrix", "label", "label_encoded")
# Keras expects DenseVectors.
dense_transformer = DenseTransformer(input_col="features_normalized", output_col="features_normalized_dense")
dataset_train = dense_transformer.transform(dataset_train)
dataset_test = dense_transformer.transform(dataset_test)
dataset_train.repartition(num_workers)
dataset_test.repartition(num_workers)
# Assing the training and test set.
training_set = dataset_train.repartition(num_workers)
test_set = dataset_test.repartition(num_workers)
# Cache them.
training_set.cache()
test_set.cache()

# Precache the trainingset on the nodes using a simple count.
print(training_set.count())

# Use the ADAG optimizer. You can also use a SingleWorker for testing purposes -> traditional
# non-distributed gradient descent.
trainer = ADAG(keras_model=convnet, worker_optimizer=optimizer_convnet, loss=loss_convnet,
               num_workers=num_workers, batch_size=16, communication_window=5, num_epoch=5,
               features_col="matrix", label_col="label_encoded")
trained_model = trainer.train(training_set)

print("Training time: " + str(trainer.get_training_time()))
print("Accuracy: " + str(evaluate_accuracy(trained_model, test_set)))
print("Number of parameter server updates: " + str(trainer.parameter_server.num_updates))
