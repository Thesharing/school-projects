import warnings

warnings.filterwarnings(action='ignore', category=UserWarning, module='gensim')

import math
from typing import Callable, List
from collections import namedtuple
from pandas import DataFrame
from sklearn.ensemble import GradientBoostingClassifier

from dataset import Data, Question
from persistence import Persistence
from similarity import Word2VecSimilarity

Score = namedtuple('Score', ['sobj_score', 'dobj_score', 'ie_score'])


class Model:

    nonsense_gov_words = ['有', '是']
    nonsense_dep_words = ['什么']

    def __init__(self, similarity_function: Callable) -> None:
        self.data = None
        self._model = GradientBoostingClassifier()
        self._similarity = similarity_function

    def _max_similarity(self, word: str, word_list: list) -> float:
        if len(word_list) <= 0:
            return 0.0
        return max(self._similarity(word, w) for w in word_list)

    def _score(self, question: Question) -> List[Score]:

        # Part 1.
        parse = question.parse
        vp_list = parse.find_label('VP')
        leaf_words = []
        for vp in vp_list:
            np = parse.find_nearest_label(vp, 'NP')
            if np:
                for word in parse.get_leaf_word(np):
                    if word not in leaf_words:
                        leaf_words.append(word)

        # Part 2.
        dep_list = question.dep
        dobj_list = []
        for dep in dep_list:
            if dep.dep == 'dobj':
                dobj_list.append(dep)

        # Part 3.
        word_freq = {}
        for answer in question.answers:
            for word in answer.answer:
                if word not in word_freq:
                    word_freq[word] = 1
                else:
                    word_freq[word] += 1

        total = 0
        for word in question.question:
            if word in word_freq:
                total += word_freq[word]

        # TODO: remove NER words

        # Part 4.
        score_list = []
        for answer in question.answers:
            sobj_score = 0.0
            for sobj in leaf_words:
                sobj_score += self._max_similarity(sobj, answer.answer)
            dobj_score = 0.0
            for dep in dobj_list:
                if dep.governor not in self.nonsense_gov_words:
                    if dep.dependent not in self.nonsense_dep_words:
                        dobj_score += (self._max_similarity(dep.governor, answer.answer) +
                                       self._max_similarity(dep.dependent, answer.answer)) / 2
                    else:
                        dobj_score += self._max_similarity(dep.governor, answer.answer)
                else:
                    if dep.dependent not in self.nonsense_dep_words:
                        dobj_score += self._max_similarity(dep.dependent, answer.answer)
            ie_score = 0.0
            if total > 0:
                for word in answer.answer:
                    if word in question.question:
                        ie_score += -math.log(word_freq[word] / total, 2)
            score_list.append(Score(sobj_score, dobj_score, ie_score))
        return score_list

    def fit(self, data: Data) -> None:
        score_list = []
        label_list = []
        for question in data.questions:
            score_list += self._score(question)
            label_list += question.get_answer_labels()
        X = DataFrame(score_list, columns=['SOBJ_Score', 'DOBJ_Score', 'IE_Score'])
        Y = DataFrame(label_list, columns=['Label']).values.ravel()
        self._model.fit(X, Y)

    def predict(self, data: Data) -> List[float]:
        score_list = []
        for question in data.questions:
            score_list += self._score(question)
        X = DataFrame(score_list, columns=['SOBJ_Score', 'DOBJ_Score', 'IE_Score'])
        return list(proba[1] for proba in self._model.predict_proba(X))

    def load(self, path) -> None:
        self._model = Persistence.load_model(path)

    def save(self, path) -> None:
        Persistence.save_model(self._model, path)


if __name__ == '__main__':
    print('Load data.')
    train = Persistence.load_data('./data/train.txt')
    print('Load dict.')
    wv = Word2VecSimilarity('./dict/sgns.zhihu.word')
    model = Model(wv.similarity)
    print('Train model.')
    model.fit(train)
    print('Save model.')
    model.save('./model/gdbt.model')

    print('Load data.')
    test = Persistence.load_data('./data/test.txt')
    # print('Load dict')
    # wv = Word2VecSimilarity('./dict/sgns.zhihu.word')
    # model = Model(wv.similarity)
    # model.load('./model/train.model')
    print('Predict.')
    res = model.predict(test)
    print('Save results.')
    Persistence.save_result(res, './result/res.txt')
