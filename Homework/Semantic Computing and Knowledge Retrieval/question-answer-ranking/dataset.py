import re
import jieba
from typing import Union, List


class Answer:
    def __init__(self, answer: Union[str, List[str]], label: Union[bool, None] = None):
        self.answer = answer
        self.label = label


class Question:
    def __init__(self, question: Union[str, None, List[str]]):
        self.question = question
        self.parse = None
        self.dep = None
        self.ner = None
        self.answers = list()
        self.right_answers = list()
        self.wrong_answers = list()

    def add_answer(self, answer: Union[str, List[str]], label: Union[bool, None] =None) -> None:
        item = Answer(answer, label)
        self.answers.append(item)
        if label:
            self.right_answers.append(item)
        else:
            self.wrong_answers.append(item)

    def get_answer_labels(self) -> List[bool]:
        return list(answer.label for answer in self.answers)


class Data:
    punc_pattern = re.compile(r'[.,*#()\[\]{}?!&:;+_=\-"\'`\\/<>@$。，？！《》：；“”（）【】·、…]')

    def __init__(self, path=None, segmentation=False, punctuation=False, allow_duplicate=False):
        self.path = path
        self.questions = []

        if path:
            with open(path, 'r', encoding='utf-8') as f:

                last_question = Question(None)
                # Remove the duplicate answers
                answers_in_question = list()
                for raw in f.readlines():
                    line = raw
                    if punctuation:
                        line = self.punc_pattern.sub('', raw)

                    items = line.strip().split('\t')

                    answer_hash = hash(items[1])
                    if answer_hash in answers_in_question and not allow_duplicate:
                        continue
                    else:
                        answers_in_question.append(answer_hash)

                    question = list(word for word in jieba.cut(items[0]) if word != ' ') if segmentation else items[0]
                    answer = list(word for word in jieba.cut(items[1]) if word != ' ') if segmentation else items[1]
                    if len(items) == 3:
                        label = items[2] == '1'
                    else:
                        label = None

                    if question != last_question.question:
                        last_question = Question(question)
                        self.questions.append(last_question)
                        answers_in_question = list()
                    last_question.add_answer(answer, label)


if __name__ == '__main__':
    data = Data('./data/nlpcc-iccpol-2016.dbqa.testing-data')
