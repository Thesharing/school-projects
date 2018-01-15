# author = Zhilong Hong

import numpy as np
from sklearn.metrics import accuracy_score
from tqdm import tqdm


class ClassifierEvaluator:
    def __init__(self, estimator, k_fold=3, iteration=10):
        self.estimator = estimator
        self.k_fold = k_fold
        self.iteration = iteration
        self._bias = 0.
        self._var = 0.

    def fit(self, X, y):
        error_rate = np.zeros(self.iteration)
        for i in tqdm(range(self.iteration)):
            index = np.random.randint(0, self.k_fold, size=y.shape)
            for j in range(self.k_fold):
                X_train = X[index != j]
                y_train = y[index != j]
                X_val = X[index == j]
                y_val = y[index == j]
                self.estimator.fit(X_train, y_train)
                error_rate[i] += (1 - accuracy_score(y_val, self.estimator.predict(X_val)))
        error_rate /= self.k_fold
        self._bias = error_rate.mean()
        self._var = error_rate.var()

    def bias(self):
        return self._bias

    def var(self):
        return self._var
