import json
from wordpair import WordPair
from wordnet import WordNetSimilarity
from word2vec import Word2VecSimilarity
from google_search import GoogleSearchSimilarity
from scipy import stats

if __name__ == '__main__':
    wp = WordPair('./Data/MTURK-771.csv')
    score = {'wordnet': {}, 'google_search': {}, 'word2vec': {}}
    wns = WordNetSimilarity(wp)
    for m in wns.method_list:
        if m in ['res', 'jcn', 'lin']:
            score['wordnet'][m] = {}
            for ic in wns.ic_list:
                score['wordnet'][m][ic] = stats.spearmanr(wp.true_score(), wns.similarity_score(m, ic)).correlation
        else:
            score['wordnet'][m] = stats.spearmanr(wp.true_score(), wns.similarity_score(m)).correlation
    wvs_param_list = [
        ('GoogleNews', './model/GoogleNews-vectors-negative300.bin', True),
        ('Wiki', './model/wiki-news-300d-1M-subword.vec', False)
    ]
    for param in wvs_param_list:
        wvs = Word2VecSimilarity(word_pair=wp, model_path=param[1], binary=param[2])
        score['word2vec'][param[0]] = stats.spearmanr(wp.true_score(), wvs.similarity_score()).correlation
    gss = GoogleSearchSimilarity(wp)
    for m in gss.method_list:
        score['google_search'][m] = stats.spearmanr(wp.true_score(), gss.similarity_score(m)).correlation
    with open('./data/experiment_result.txt', 'w', encoding='utf-8') as f:
        json.dump(score, f, indent=4)
