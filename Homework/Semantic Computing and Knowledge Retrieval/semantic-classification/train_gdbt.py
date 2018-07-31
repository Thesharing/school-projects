from corpse import Corpse
from doc2vec import ModelLoader, ModelTrainer
from sklearn.ensemble import GradientBoostingClassifier
from sklearn.metrics import classification_report, accuracy_score

if __name__ == '__main__':
    model = ModelLoader('./model/doc2vec.bin').model
    train_corpse = Corpse('./data/train/')
    train_vector = train_corpse.vector(model)
    test_corpse = Corpse('./data/test')
    test_vector = test_corpse.vector(model)

    cls = GradientBoostingClassifier(subsample=0.8, random_state=100, n_estimators=400)
    cls.fit(train_vector, train_corpse.labels())
    res = cls.predict(test_vector)
    print(classification_report(test_corpse.labels(), res))
    print(accuracy_score(test_corpse.labels(), res))
