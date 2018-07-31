import fastText

from corpse import Corpse
from numpy import concatenate
from sklearn.metrics import classification_report, accuracy_score

if __name__ == '__main__':
    p = Corpse('./data/train/')
    with open('./data/fasttext/train.txt', 'w', encoding='utf-8') as f:
        p.fast_text_corpse(f)
    cls = fastText.train_supervised('./data/fasttext/train.txt', label='__label__')
    test_corpse = Corpse('./data/test')
    result = list(i.replace('__label__', '') for i in concatenate(cls.predict(test_corpse.sentences())[0]))
    print(classification_report(test_corpse.labels(), result))
    print(accuracy_score(test_corpse.labels(), result))
