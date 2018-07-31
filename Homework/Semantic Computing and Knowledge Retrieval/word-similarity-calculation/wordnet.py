from nltk.corpus import wordnet as wn
from nltk.corpus import wordnet_ic as wnic
from wordpair import WordPair
from nltk.corpus.reader.wordnet import WordNetError
import numpy as np
from scipy import stats
import os


class WordNetSimilarity:
    corpus_path = './information_content'
    method_list = {
        'path': wn.path_similarity,  # Path Similarity
        'lch': wn.lch_similarity,  # Leacock-Chodorow Similarity
        'wup': wn.wup_similarity,  # Wu-Palmer Similarity
        'res': wn.res_similarity,  # Resnik Similarity
        'jcn': wn.jcn_similarity,  # Jiang-Conrath Similarity
        'lin': wn.lin_similarity  # Lin Similarity
    }
    ic_list = [
        'bnc', 'semcor', 'shaks', 'brown', 'treebank'
    ]

    def __init__(self, word_pair: WordPair):
        self.word_pair = word_pair

    def similarity_score(self, method_name='path', ic_name=None):
        ic = None
        if ic_name is not None:
            ic = self.get_corpus(ic_name)
        score_set = []
        for word_pair in self.word_pair.pair:
            score = []
            for synset_1 in wn.synsets(word_pair[0]):
                for synset_2 in wn.synsets(word_pair[1]):
                    if synset_1.pos() == synset_2.pos():
                        try:
                            sim = self.method_list[method_name](synset_1, synset_2) if ic is None else \
                                self.method_list[method_name](synset_1, synset_2, ic)
                        except WordNetError:
                            print("Error for %s and %s" % (synset_1, synset_2))
                        if sim is not None:
                            score.append(sim)
            score_set.append(np.max(score))
        return score_set

    def get_corpus(self, ic_name: str):
        corpus_file_path = os.path.join(os.getenv('APPDATA'), "nltk_data/corpora/wordnet_ic/ic-" + ic_name + '.dat')
        if os.path.isfile(corpus_file_path):
            return wnic.ic(corpus_file_path)
        else:
            corpus_file_path = self.corpus_path + 'ic-' + ic_name + '.dat'
            if os.path.isfile(corpus_file_path):
                return wnic.ic(corpus_file_path)
            else:
                raise FileNotFoundError("Information content file not found.")


if __name__ == '__main__':
    wp = WordPair('./Data/MTURK-771.csv')
    wns = WordNetSimilarity(wp)
    print(stats.spearmanr(wp.true_score(), wns.similarity_score('res', 'treebank')))
