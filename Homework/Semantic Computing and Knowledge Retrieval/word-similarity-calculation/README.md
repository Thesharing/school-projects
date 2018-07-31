# Word Similarity

### 作业要求

- 词汇相似度计算
- 实现3种词汇相似度计算方法，尽量保证方法的多样性
- 基于Mturk-771进行实验和分析(开放式)
  - http://www2.mta.ac.il/~gideon/mturk771.html

### 数据集和工具

#### 数据集

[Mturk-771数据集](http://www2.mta.ac.il/~gideon/mturk771.html)包含两部分，一部分是词组，包含了两个用于计算相似度的词语，另一部分是人工打分的相似度，用于测试实验结果。

路径 `./data/MTURK-771.csv`

#### 工具

WordNet基于[NLTK（Natural Language Toolkit）](https://www.nltk.org/)实现

Word2Vec基于[Gensim](https://radimrehurek.com/gensim/)实现

基于检索页面的方法中对Google Search搜索结果数量的抓取则是通过[Requests](http://docs.python-requests.org/)实现。

对于实验结果的验证，为了计算实验得到相似度和人工打分的相似度的之间的关系，使用[斯皮尔曼等级相关系数（Spearman's Rank Correlation Coefficient）](https://en.wikipedia.org/wiki/Spearman%27s_rank_correlation_coefficient)进行计算，如果相关系数越高，说明实验方法得到的相似度和人工打分的相似度越接近，也就是说实验效果越好。计算斯皮尔曼等级相关系数使用的是[Scipy](https://docs.scipy.org/doc/scipy-0.18.1/reference/generated/scipy.stats.mstats.spearmanr.html)科学计算库。

在基于WordNet计算的实验中，Resnik Similarity、Jiang-Conrath Similarity和Lin Similarity三种计算方法都需要概率$P(C)$，即基于语料库计算从一个语料库中随机挑选一个词，该词语属于概念$C$的概率。因此在这三种计算方法中涉及的语料库均来自[WordNet-InfoContent](https://github.com/teropa/nlp/tree/master/resources/corpora/wordnet_ic)，也可以在NLTK Downloader中Corpora目录下的`wordnet_ic`中下载到。语料库包括：

| 语料库   | 名称                          |
| -------- | ----------------------------- |
| bnc      | British National Corpus       |
| brown    | Brown corpus                  |
| treebank | Penn Treebank                 |
| semcor   | SemCor                        |
| shaks    | Complete Works of Shakespeare |

NLTK Downloader下载的语料库默认放在：`%nltk_data/corpora/wordnet_ic/`中。

自定义的语料库路径：`./ic/`

基于Word2Vec计算的实验中，Word2Vec需要大量语料进行训练才能生成Word2Vec模型并达到较好的效果。目前主流的预训练模型有：[fasttext-wiki-news-subwords-300](https://fasttext.cc/docs/en/english-vectors.html)、[glove-twitter-200](https://nlp.stanford.edu/projects/glove/)、[word2vec-google-news-300](https://code.google.com/archive/p/word2vec/)等等。在本次实验中使用了Google提供的预训练模型GoogleNews Vector Negative 300，载入word2vec模型后直接用来计算词汇相似度。

预训练模型的路径：`./model/`

### 参考资料

[RaRe-Technologies/gensim-data](https://github.com/RaRe-Technologies)（Gensim可使用的所有语料库和预训练模型）

[WordNet::Similarity](http://wn-similarity.sourceforge.net/)（提供 WordNet Information Content）

[Google's trained Word2Vec model in Python](http://mccormickml.com/2016/04/12/googles-pretrained-word2vec-model-in-python/)

[词语相似度计算：2、使用NLTK和WordNet计算词语相似度](http://www.voidcn.com/article/p-xaltbdyr-zv.html)

[WordSimilarity 词汇相似度计算](https://blog.csdn.net/Garfy_/article/details/68485604)

[词语相似度计算：6、实验报告](https://blog.csdn.net/mmc2015/article/details/50943163)

[词汇相似度计算](https://blog.csdn.net/lrita/article/details/51149798)

