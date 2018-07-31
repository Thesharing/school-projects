import json
from sklearn.externals import joblib

from dataset import Data, Question
from semantic import ParserTree, DependencyTriple


def convert_bool(src: str):
    if src == 'None':
        return None
    else:
        return src == 'True'


class Persistence:

    @staticmethod
    def save_data(data: Data, path: str):
        with open(path, 'w', encoding='utf-8') as f:
            for question in data.questions:
                print(json.dumps(question.question, ensure_ascii=False), file=f)
                print(str(question.parse), file=f)
                print(json.dumps(question.dep, ensure_ascii=False), file=f)
                print(json.dumps(question.ner, ensure_ascii=False), file=f)
                print(len(question.answers), file=f)
                for answer in question.answers:
                    print('%s\t%s' % (json.dumps(answer.answer, ensure_ascii=False), answer.label), file=f)

    @staticmethod
    def load_data(path: str):
        data = Data()
        with open(path, 'r', encoding='utf-8') as f:
            while True:
                q = f.readline().strip('\n')
                if len(q) <= 0:
                    break
                question = Question(json.loads(q))
                question.parse = ParserTree(f.readline().strip('\n'))
                question.dep = list(DependencyTriple(i[0], i[1], i[2]) for i in json.loads(f.readline().strip('\n')))
                question.ner = json.loads(f.readline().strip('\n'))
                ans_num = int(f.readline().strip('\n'))
                for i in range(ans_num):
                    item = f.readline().strip('\n').split('\t')
                    if len(item) > 1:
                        question.add_answer(json.loads(item[0]), convert_bool(item[1]))
                    else:
                        question.add_answer(json.loads(item[0]))
                data.questions.append(question)
        return data

    @staticmethod
    def save_result(result: list, path: str):
        with open(path, 'w', encoding='utf-8') as f:
            for res in result:
                print(res, file=f)

    @staticmethod
    def save_model(model, path: str):
        joblib.dump(model, path)

    @staticmethod
    def load_model(path: str):
        return joblib.load(path)


if __name__ == '__main__':
    d = Persistence.load_data('./train.txt')
