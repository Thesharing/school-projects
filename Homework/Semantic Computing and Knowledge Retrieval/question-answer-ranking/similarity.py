from gensim.models import KeyedVectors


class Similarity:
    pass


class Word2VecSimilarity(Similarity):

    def __init__(self, path):
        self.word2vec = KeyedVectors.load_word2vec_format(path, binary=False)

    def similarity(self, a: str, b: str) -> float:
        try:
            return self.word2vec.similarity(a, b)
        except KeyError:
            return CharacterSimilarity.similarity(a, b)


class CharacterSimilarity(Similarity):

    @staticmethod
    def similarity(a: str, b: str) -> float:
        if a == b:
            score = 1.0
        else:
            score = 0.0
            for ch in a:
                if ch in b:
                    score += 1.0
            score /= len(a)
        return score
