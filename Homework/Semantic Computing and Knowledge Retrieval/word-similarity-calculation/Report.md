# Word Similarity

[TOC]

## 一、作业要求

- 词汇相似度计算
- 实现3种词汇相似度计算方法，尽量保证方法的多样性
- 基于Mturk-771进行实验和分析(开放式)
  - http://www2.mta.ac.il/~gideon/mturk771.html

## 二、数据集和工具

Mturk-771数据集包含两部分，一部分是词组，包含了两个用于计算相似度的词语，另一部分是人工打分的相似度，用于测试实验结果。

本次实验共使用了三种计算方法，分别是：基于WordNet进行计算、基于Word2Vec进行计算以及基于检索页面数量的方法。其中WordNet基于[NLTK（Natural Language Toolkit）](https://www.nltk.org/)实现，Word2Vec基于[Gensim](https://radimrehurek.com/gensim/)实现，基于检索页面的方法中对Google Search搜索结果数量的抓取则是通过[Requests](http://docs.python-requests.org/)实现。

对于实验结果的验证，为了计算实验得到相似度和人工打分的相似度的之间的关系，使用[斯皮尔曼等级相关系数（Spearman's Rank Correlation Coefficient）](https://en.wikipedia.org/wiki/Spearman%27s_rank_correlation_coefficient)进行计算，如果相关系数越高，说明实验方法得到的相似度和人工打分的相似度越接近，也就是说实验效果越好。计算斯皮尔曼等级相关系数使用的是[Scipy](https://docs.scipy.org/doc/scipy-0.18.1/reference/generated/scipy.stats.mstats.spearmanr.html)科学计算库。

在基于WordNet计算的实验中，Resnik Similarity、Jiang-Conrath Similarity和Lin Similarity三种计算方法都需要概率$P(C)$，即基于语料库计算从一个语料库中随机挑选一个词，该词语属于概念$C$的概率。因此在这三种计算方法中涉及的语料库均来自[WordNet-InfoContent](https://github.com/teropa/nlp/tree/master/resources/corpora/wordnet_ic)，也可以在NLTK Downloader中Corpora目录下的`wordnet_ic`中下载到。语料库包括：

| 语料库   | 名称                          |
| -------- | ----------------------------- |
| bnc      | British National Corpus       |
| brown    | Brown corpus                  |
| treebank | Penn Treebank                 |
| semcor   | SemCor                        |
| shaks    | Complete Works of Shakespeare |

基于Word2Vec计算的实验中，Word2Vec需要大量语料进行训练才能生成Word2Vec模型并达到较好的效果。目前主流的预训练模型有：[fasttext-wiki-news-subwords-300](https://fasttext.cc/docs/en/english-vectors.html)、[glove-twitter-200](https://nlp.stanford.edu/projects/glove/)、[word2vec-google-news-300](https://code.google.com/archive/p/word2vec/)等等。在本次实验中使用了Google提供的预训练模型Google News Vector Negative 300和Facebook提供的预训练模型Fasttext Wiki News Subwords 300，载入word2vec模型后直接用来计算词汇相似度。

## 三、实验方法

### 1. 基于WordNet的实现

WordNet是一个词义数据库，以同义集合（Synset）作为基本单元，其中同义集合间的关系（Relation）和架构（Hierarchy）可以用于计算两个词的相似度。基于WordNet实现的计算词义相似度的方法有：

| Method                       |                                                              |
| ---------------------------- | ------------------------------------------------------------ |
| Path Similarity              | $sim_{path}(c_1, c_2) = -\log{pathlen(c_1, c_2)}$            |
| Leacock-Chodorow Similarity: | $sim_{lch}(c_1, c_2) = max(\log{2D} - \log{pathlen(c_1, c_2)})$ |
| Wu-Palmer Similarity         | $sim_{wup}(c_1, c_2) = \frac{2 \times depth(LCS(c_1, c_2))}{depth(c_1) + depth(c_2)}$ |
| Resnik Similarity            | $sim_{res}(c_1, c_2) = -\log{P(LCS(c_1, c_2))}$              |
| Jiang-Conrath Similarity     | $sim_{jc}(c_1, c_2)=\frac{1}{2 \times \log{P(LCS(c_1, c_2))-(\log{P(c_1) + logP(c_2))}}}$ |
| Lin Similarity               | $sim_{lin}(c_1, c_2)=\frac{2\times\log{P(LCS(c_1, c_2))}}{\log{P(c_1)+\log{P(c_2)}}}$ |

其中，$pathlen(c_1, c_2)$是指词义节点$c_1$和$c_2$之间最短路径上边的数量；$P(C)$是指从一个语料库中随机选择一个词，这个词属于词义$C$的概率；信息量（Information Content）$IC(C) = -\log{P(C)}$；$LCS(c_1, c_2)$是指离$c_1$和$c_2$两个节点最近的父节点。

具体在代码中的实现方式为，对于训练集中每一个词对中两个词的词义，分别计算词义相似度，然后求得所有词义相似度的最大值，即可认为是这两个词的相似度。计算过程中如果两个词义之间不存在$pathlen(c_1, c_2)$，则直接跳过。对于Resnik、Jiang-Conrath和Lin三种方法来说，$P(C)$需要基于语料库进行计算，因此需要在计算前读取语料库。

### 2. 基于Word2Vec的实现

Word2vec是一个用于处理文本的双层神经网络。它的输入是文本语料，输出则是一组向量：该语料中词语的特征向量。Word2Vec可以在向量空间内将词的向量按相似性进行分组。它能够识别出数学上的相似性。Word2vec衡量词的余弦相似度，无相似度表示为90度角，而相似度为1的完全相似则表示为0度角。

具体在代码中的实现是，将预处理的模型载入，然后计算每个词对之间的相似度。

### 3. 基于检索页面数量的实现

基于检索页面数量的实现，即获取两个词单独的搜索结果数量，然后再获取两个词合在一起时的搜索结果数量。后者越大，说明两个词同时出现的概率越大，也就可以间接说明两个词的相似度较高。

首先要考虑如何抓取检索页面数量，实验中以Google为目标搜索引擎，使用Requests进行抓取。由于Google反爬虫机制较为成熟，因此抓取时共考虑了以下对抗机制：

1. 随机更换User Agent，并模拟正常用户访问的Request-Headers
2. 随机延迟抓取间隔，设置在5s-15s左右，避免被识别
3. 由于一些地区的Google针对英语的搜索结果数量是一致的，因此对多个地区的Google进行抓取
4. 对于抓取超时的词语，任务完成后重新抓取

在抓取到页面后再使用BeautifulSoup对页面进行处理，提取搜索结果数量并记录下来。

在抓取到所有词语以及其组合的检索页面数量后，根据页面数量，使用以下方式计算两个词语的相似度：
$$
Similarity Score = 
\begin{cases}
0, & \text{if $H(P \bigcap Q) \leq c$} \\[2ex]
score, & \text{otherwise}
\end{cases}
$$

| Method             | Score                                                        |
| ------------------ | ------------------------------------------------------------ |
| $WebJaccard(P, Q)$ | $score = \frac{H(P\bigcap Q)}{H(P) + H(Q) - H(P \bigcap Q)}$ |
| $WebOverlap(P, Q)$ | $score = \frac{H(P \bigcap Q)}{min(H(P), H(Q))}$             |
| $WebDice(P, Q)$    | $score = \frac{2H(P\bigcap Q)}{H(P) + H(Q)}$                 |
| $WebPMI(P, Q)$     | $score=\log_2{(\frac{\frac{H(P\bigcap Q)}{N}}{\frac{H(P)}{N}\frac{H(Q)}{N}})}$ |
| $NGD(P, Q)$        | $score = \frac{max\{\log{f(x)}, \log{f(y)}\}-\log{f(x, y)}}{\log{N}-min\{\log{f(x), \log{f(y)}}\}}$ |

## 四、实验结果与分析

|               | Method                | Info-Content | Similarity Score     |
| ------------- | --------------------- | ------------ | -------------------- |
| WordNet       | path                  |              | 0.49849248829295956  |
|               | lch                   |              | 0.49604190501569656  |
|               | wup                   |              | 0.45500538456371514  |
|               | res                   | bnc          | 0.42155573374440486  |
|               |                       | semcor       | 0.3620145806699351   |
|               |                       | shaks        | 0.40107438992515704  |
|               |                       | brown        | 0.41680242437128656  |
|               |                       | treebank     | 0.4151692821109224   |
|               | jcn                   | bnc          | 0.4894230408532388   |
|               |                       | semcor       | 0.4940176493601078   |
|               |                       | shaks        | 0.40823670976275295  |
|               |                       | brown        | 0.48231918119098216  |
|               |                       | treebank     | 0.46600609845544283  |
|               | lin                   | bnc          | 0.5006613071745352   |
|               |                       | semcor       | 0.5022365681958145   |
|               |                       | shaks        | 0.4212091118688091   |
|               |                       | brown        | 0.49316716989436044  |
|               |                       | treebank     | 0.48196149589125725  |
|               | **Pre-trained Model** |              | **Similarity Score** |
| Word2Vec      | google_news           |              | 0.6713097016879208   |
|               | wiki-freetext         |              | 0.7279245740173242   |
|               | **Method**            |              | **Similarity Score** |
| Google Search | jaccard               |              | 0.192150115381344    |
|               | overlap               |              | 0.1522908944829982   |
|               | dice                  |              | 0.19467352767275206  |
|               | pmi                   |              | 0.1710752435967949   |
|               | ngd                   |              | -0.20024528747034684 |

根据分析可知，在所有求相似度的方法中，基于word2vec的方式与人工打分的匹配度最高，达到了0.728。可以看到Word2Vec将文本语料转换成文本向量计算语义相似度的方式相比于基于WordNet和基于检索页面数量来说更为准确，更能够反映词义之间的相似度。

与此同时可以看到，对于Word2Vec来说，模型对相似度计算的影响较大，基于FreeText方式和Wiki语料生成的预训练模型要比基于Google News生成的预训练模型效果要好一些，准确度增加了0.05。而在基于WordNet的方法中，基于Lin‘s Similarity方法和Semcor语料得到的准确度更高，达到了0.502。不过对于基于WordNet的方法来说，除了Reslik的方法普遍较差以外，不同方法以及语料得到的结果会有差别，不过差别并不是特别大。WordNet结果不如Word2Vec的可能原因一是一些词在WordNet语义词典中可能没有被收录，抑或是没有出现在语料中，因而会出现一些偏差；二是一些词义之间不存在$pathlen$和$LCS$，对于这些词义在实验中直接进行了忽略，可能赋以一个非常大的值可以对方法加以改善；三是WordNet依赖上下位的层次关系，对名词较准确，对形容词和动词来说没有名词准确，同时对于名词-形容词这样的词语对（例如bird-solid）来说实验效果也会较差。

而基于检索页面数量的实现效果最差，一方面Google Search对于检索页面的数量统计较为粗略，其有效位数较少，会部分影响实验结果；另一方面，这一方法单纯只考虑词语在搜索引擎中的搜索结果，而没有考虑词语之间词义的关联，而且实验在具体公式实现上可能存在一定偏差，因此实验效果整体偏差。

总而言之，本次实验的实验结果说明，Word2Vec实验效果最好，预训练模型对实验效果影响较大；WordNet其次，计算方法和语料对实验结果有一定影响；基于检索页面数量的实现最差。