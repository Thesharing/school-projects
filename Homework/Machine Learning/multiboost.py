__author__ = 'Thesharing'

import math
import random
import numpy as np
import pandas
from sklearn.base import clone, ClassifierMixin, RegressorMixin
from sklearn.tree import DecisionTreeClassifier
from sklearn.tree import DecisionTreeRegressor
from sklearn.ensemble import AdaBoostRegressor
from sklearn.metrics import accuracy_score


class MultiBoostClassifier:
    def __init__(self, base_estimator=DecisionTreeClassifier(), n_estimators=10):

        pandas.options.mode.chained_assignment = None

        self.base_estimator = base_estimator
        self.n_estimators = n_estimators
        self.beta = np.empty(0)
        self.error_rate = np.empty(0)
        self.data_size = 0
        self.y_range = np.empty(0)
        self._estimators = np.empty(0)
        self._class_num = 0

    def fit(self, X: pandas.DataFrame, y):
        # Initialization
        self._estimators = np.empty(self.n_estimators, dtype=ClassifierMixin)
        self.beta = np.empty([self.n_estimators])
        self.error_rate = np.empty([self.n_estimators])
        self.data_size = X.shape[0]
        if isinstance(y, pandas.DataFrame):
            y = y.values.reshape((-1))
        self._get_y_dict(y)
        y = self._transform_y(y)
        self.y_range = np.unique(y)
        # S' = S with instance weights assigned to be 1.
        sample_X, sample_y, sample_weight = self._reset_sample_and_standardize(X, y, mode='uniform')
        # set k = 1
        k = 1
        # for t = 1 to T {
        for t in range(self.n_estimators):
            # if Ik = t then
            if self._get_iteration_number(k) == t:
                # reset S' to random weights drawn from the continuous Poisson distribution.
                # standardize S' to sum to n.
                sample_X, sample_y, sample_weight = self._reset_sample_and_standardize(X, y)
                # increment k
                k = k + 1
            # Ct = BaseLearn(S')
            temp = clone(self.base_estimator).fit(sample_X, sample_y, sample_weight)
            self._estimators[t] = temp
            estimate_y = temp.predict(sample_X)
            self.error_rate[t] = 1 - accuracy_score(sample_y, estimate_y, sample_weight=sample_weight)
            while self.error_rate[t] > 1 - 1. / self._class_num:
                # reset S' to random weights drawn from the continuous Poisson distribution.
                # standardize S' to sum to n.
                sample_X, sample_y, sample_weight = self._reset_sample_and_standardize(X, y)
                # increment k
                k = k + 1
                # go to step 8
                temp.fit(sample_X, sample_y, sample_weight)
                self._estimators[t] = temp
                estimate_y = temp.predict(sample_X)
                self.error_rate[t] = 1 - accuracy_score(sample_y, estimate_y, sample_weight=sample_weight)
            # otherwise if et = 0 then
            if self.error_rate[t] == 0:
                # set beta t to 10e-10
                self.beta[t] = 1e-10
                # reset S' to random weights drawn from the continuous Poisson distribution.
                # standardize S' to sum to n.
                sample_X, sample_y, sample_weight = self._reset_sample_and_standardize(X, y)
                # increment k
                k = k + 1
            # otherwise
            else:
                self.beta[t] = self.error_rate[t] / (1 - self.error_rate[t])
                sample_weight[estimate_y != sample_y] /= (2 * self.error_rate[t])
                sample_weight[estimate_y == sample_y] /= (2 * (1 - self.error_rate[t]))
                sample_weight[sample_weight < 1e-8] = 1e-8

    def predict(self, X: pandas.DataFrame):
        res = np.zeros((X.shape[0], len(self.y_range)))
        for t in range(self.n_estimators):
            y = self._estimators[t].predict(X)
            temp = np.zeros((y.shape[0], self._class_num))
            temp[np.arange(y.shape[0]), y] = 1
            res -= temp * np.log(self.beta[t])
        res = np.argmax(res, axis=1)
        return self._transform_y_(res)

    def _get_y_dict(self, y):
        self._class_num = len(np.unique(y))
        self.y2y_ = dict(zip(np.unique(y), range(self._class_num)))
        self.y_2y = dict(zip(range(self._class_num), np.unique(y)))

    def _transform_y(self, y):
        return np.array(list(map(lambda x: self.y2y_[x], list(y))))

    def _transform_y_(self, y_):
        return np.array(list(map(lambda x: self.y_2y[x], list(y_))))

    def _get_iteration_number(self, i):
        n = math.floor(math.sqrt(self.n_estimators))
        if i < n:
            return math.ceil(i * self.n_estimators / n)
        else:
            return self.n_estimators

    def _reset_sample_and_standardize(self, X: pandas.DataFrame, y, mode='poisson'):
        # reset S' to random weights drawn from the continuous Poisson distribution.
        # sample_weight = np.zeros(X.shape[0], dtype=np.float64)
        if mode == 'poisson':
            sample_weight = -np.log(np.random.randint(1, 1000, size=y.shape) / 1000.)
        else:
            sample_weight = np.ones(X.shape[0], dtype=np.float64)
        sample_weight /= sample_weight.sum()
        idxs = np.random.choice(np.arange(X.shape[0]), X.shape[0], replace=True, p=sample_weight)
        sample_X = X.iloc[idxs, :]
        sample_weight = sample_weight[idxs]
        sample_y = y[idxs]
        # standardize S' to sum to n.
        sample_weight = sample_weight / sample_weight.sum() * sample_weight.shape[0]
        return sample_X, sample_y, sample_weight

    def _poisson(self):
        return -math.log(random.randint(1, 999) / 1000)
