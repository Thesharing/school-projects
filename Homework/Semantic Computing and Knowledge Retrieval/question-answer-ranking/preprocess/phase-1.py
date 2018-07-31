from dataset import Answer, Question, Data
import math
import os

if __name__ == '__main__':
    phase = 'contest'  # 'test'
    path = '../data/'
    data = Data(path + 'nlpcc-iccpol-2016.dbqa.' + phase + 'ing-data')
    if not os.path.exists(path + phase):
        os.mkdir(path + phase)
    if not os.path.exists(path + phase + '/question'):
        os.mkdir(path + phase + '/question')
    for i in range(int(math.ceil(len(data.questions) / 100))):
        with open(path + phase + '/question/question-' + str(i) + '.txt', 'w', encoding='utf-8') as q:
            for idx in range(i * 100, min(i * 100 + 100, len(data.questions))):
                question = data.questions[idx].question.strip()
                question = question.replace('?', '').replace('？', '').replace('。', '，')
                if question[-1] == '、':
                    question = question[:-1]
                data.questions[idx].question = question + '？'
                print(data.questions[idx].question, file=q)
    if not os.path.exists(path + phase + '/processed'):
        os.mkdir(path + phase + '/processed')
