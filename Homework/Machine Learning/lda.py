from nltk.tokenize import RegexpTokenizer
from stop_words import get_stop_words
from nltk.stem.porter import PorterStemmer
import warnings
warnings.filterwarnings(action='ignore', category=UserWarning, module='gensim')
from gensim import corpora, models
import gensim


class LdaModel:

    def __init__(self):
        self.model = None
        self.dictionary = None
        self.tokenizer = RegexpTokenizer(r'\w+')
        self.stop_words = get_stop_words('en')
        self.stop_words.append('via')
        self.p_stemmer = PorterStemmer()

    def fit(self, doc_set, num_topics: int = 10, passes: int = 20):
        texts = []
        # loop through document list
        for sentence in doc_set:
            # clean and tokenize document string
            tokens = self.tokenizer.tokenize(sentence.lower())
            # remove stop words from tokens
            stopped_tokens = [i for i in tokens if i not in self.stop_words]
            # stem tokens
            stemmed_tokens = [self.p_stemmer.stem(i) for i in stopped_tokens]
            # add tokens to list
            texts.append(stemmed_tokens)
        # turn our tokenized documents into a id <-> term dictionary
        self.dictionary = corpora.Dictionary(texts)
        # convert tokenized documents into a document-term matrix
        corpus = [self.dictionary.doc2bow(text) for text in texts]
        # generate LDA model
        self.model = gensim.models.ldamodel.LdaModel(corpus, num_topics=num_topics, id2word=self.dictionary,
                                                     passes=passes)

    def predict(self, sentence):
        tokens = self.tokenizer.tokenize(sentence.lower())
        stopped_tokens = [i for i in tokens if i not in self.stop_words]
        stemmed_tokens = [self.p_stemmer.stem(i) for i in stopped_tokens]
        # convert tokenized documents into a document-term matrix
        corpus = self.dictionary.doc2bow(stemmed_tokens)
        result_vec = self.model[corpus]
        best_topic = max(result_vec, key=lambda item: item[1])
        return LdaResult(self.model.print_topic(best_topic[0]), best_topic[1], result_vec,
                         list(self.model.print_topic(i[0]) for i in result_vec))


class LdaResult:

    def __init__(self, best_topic, best_topic_prob, prob_list, topic_list):
        self.topic = best_topic
        self.prob = best_topic_prob
        self.prob_list = prob_list
        self.topic_list = topic_list


if __name__ == '__main__':
    from dataset import DBLP

    dataset = DBLP('./dataset/FilteredDBLP.txt')
    lda = LdaModel()
    lda.fit(dataset.get_title_by_conference('KDD'))
