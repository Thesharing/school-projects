import os
import re
from numpy import concatenate, asarray
from gensim.models.doc2vec import TaggedDocument, Doc2Vec


class Corpse:

    def __init__(self, path: str = None):

        # pattern
        self.at_pattern = re.compile('@\w+')
        self.punc_pattern = re.compile('[.,*#()?!&:;+=\-"/<>@$]')
        self.http_pattern = re.compile('https?://t\.co/\w+')
        self.white_pattern = re.compile('\s{2,}')
        self.pure_number_pattern = re.compile(r'\b\d{1,15}\b')

        self.data = []

        def process(file_path):
            with open(file_path, 'r', encoding='utf-8') as f:
                for i in f.readlines():
                    if len(i) > 0:
                        data_list = self.process_content(i)
                        if len(data_list) > 1:
                            if len(data_list) == 2 or len(data_list[1]) > 10:
                                data_list = [data_list[0], data_list[1].split()[0], ' '.join(data_list[1].split()[1:])]
                            self.data.append(data_list)

        if path is not None:
            if os.path.isdir(path):
                for file_name in os.listdir(path):
                    process(os.path.join(path, file_name))
            elif os.path.isfile(path):
                process(path)
            else:
                print('No file or dir at specified path.')

    def process_content(self, s):
        return re.sub(self.white_pattern, ' ',
                      re.sub(self.pure_number_pattern, ' ',
                             re.sub(self.punc_pattern, ' ',
                                    re.sub(self.at_pattern, ' ',
                                           re.sub(self.http_pattern, ' urlpattern ',
                                                  s.replace('\n', ' ')))))). \
            replace('\\u2019', '\'').replace('\\u002c', '').replace('\\', '').strip().lower().split('\t')

    def tags(self):
        return list(i[0] for i in self.data)

    def words(self):
        return list(i[2].split for i in self.data)

    def sentences(self):
        return list(i[2] for i in self.data)

    def labels(self):
        return list(i[1] for i in self.data)

    def tagged_document(self):
        return list(TaggedDocument(words=item[2].split(), tags=[item[0]]) for item in self.data)

    def vector(self, model: Doc2Vec):
        return asarray(list(model.docvecs[i] for i in self.tags()))

    def fast_text_corpse(self, f):
        for item in self.data:
            print(' __label__' + item[1] + '    ' + item[2], file=f)

    @classmethod
    def merge(cls, a, b):
        corpse = Corpse()
        corpse.data = concatenate([a.data, b.data])
        return corpse


if __name__ == '__main__':
    p = Corpse('./data/train/')
    with open('./data/fasttext/train.txt', 'w', encoding='utf-8') as f:
        p.fast_text_corpse(f)
    p = Corpse('./data/test/')
    with open('./data/fasttext/test.txt', 'w', encoding='utf-8') as f:
        p.fast_text_corpse(f)