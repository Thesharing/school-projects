from gensim.models.doc2vec import Doc2Vec
from random import shuffle

from corpse import Corpse


class ModelTrainer:
    def __init__(self, dm: int = 0, size: int = 100, window: int = 10, sample: int = 1e-3, workers: int = 8):
        self.model = Doc2Vec(min_count=1, window=window, size=size, sample=sample, dm=dm, workers=workers)

    def train(self, corpse: Corpse, epoch: int = 10):
        tagged_document = corpse.tagged_document()
        self.model.build_vocab(tagged_document)
        for i in range(epoch):
            shuffle(tagged_document)
            self.model.train(tagged_document, total_examples=len(tagged_document), epochs=self.model.iter)

    def save(self, path: str):
        self.model.save(path)


class ModelLoader:
    def __init__(self, path):
        self.model = Doc2Vec.load(path)
