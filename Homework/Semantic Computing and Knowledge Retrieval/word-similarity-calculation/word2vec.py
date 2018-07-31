from gensim.models import KeyedVectors
from wordpair import WordPair
from scipy import stats


class Word2VecSimilarity:

    def __init__(self, word_pair: WordPair, model_path: str, binary: bool):
        # ./model/GoogleNews-vectors-negative300.bin
        self.model = KeyedVectors.load_word2vec_format(model_path, binary=binary)
        self.word_pair = word_pair

    def similarity_score(self):
        score = []
        for word_pair in self.word_pair.pair:
            score.append(self.model.similarity(word_pair[0], word_pair[1]))
        return score


if __name__ == '__main__':
    wp = WordPair('./Data/MTURK-771.csv')
    wvs = Word2VecSimilarity(word_pair=wp, model_path='./model/GoogleNews-vectors-negative300.bin', binary=False)
    print(stats.spearmanr(wp.true_score(), wvs.similarity_score()))
