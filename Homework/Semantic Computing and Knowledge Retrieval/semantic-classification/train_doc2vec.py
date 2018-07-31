from corpse import Corpse
from doc2vec import ModelLoader, ModelTrainer

if __name__ == '__main__':
    model = ModelTrainer()
    corpse = Corpse.merge(Corpse('./data/train'), Corpse('./data/test'))
    model.train(corpse)
    model.save('./model/doc2vec.bin')
