# -*- coding: utf-8 -*-
import re
import math

start = {'keyword': 0}
transit = {'keyword': {'keyword': 0}}
launch = {'keyword': {'keyword': 0}}
type = []
with open("pi.txt", 'r', encoding='utf-8') as f:
    for line in f.readlines():
        a = re.split(r'\s+', line)
        start[a[0]] = int(a[1])
    start.pop('keyword')
with open("a.txt", 'r', encoding='utf-8') as f:
    for line in f.readlines():
        a = re.split(r'\s+', line)
        b = re.split('/', a[0])
        if b[0] in transit:
            transit[b[0]][b[1]] = int(a[1])
        else:
            transit[b[0]] = {'keyword': 0}
            transit[b[0]][b[1]] = int(a[1])
            transit[b[0]].pop('keyword')
    transit.pop('keyword')
with open("b.txt", 'r', encoding='utf-8') as f:
    for line in f.readlines():
        a = re.split('/', line)
        if len(a) > 2:
            if a[0] in launch:
                launch[a[0]][a[1]] = int(a[2])
            else:
                launch[a[0]] = {'keyword': 0}
                launch[a[0]][a[1]] = int(a[2])
                launch[a[0]].pop('keyword')
                type.append(a[0])
    launch.pop('keyword')

question = []
answer = []
punctuation = ['，', '。', '？', '！', '；', '……']
lastState = []
lastPossibility = []
nowState = []
nowPossibility = []
footprint = []
right = 0
wrong = 0
with open("output.txt", 'w', encoding='utf-8') as output:
    with open("test.txt", 'r', encoding='utf-8') as f:
        for line in f.readlines():
            a = re.split(r'\s+', line[23:])
            b = []
            for eachA in range(0, len(a)):
                b = re.split('/', a[eachA])
                if len(b) > 1:
                    question.append(b[0])
                    answer.append(b[1])
                    # matrix = [[] for i in range(len(type))]
                if len(a) > 1 and len(question) > 1:
                    if (b[0] in punctuation) or (eachA == len(a) - 1):
                        for j in range(0, len(type)):
                            matrixValue = start.get(type[j], 0)
                            matrixValue /= start['ALL']
                            matrixValue *= launch[type[j]].get(question[0], 0)
                            matrixValue /= launch[type[j]]['ALL']
                            if matrixValue > 0:
                                lastState.append(j)
                                lastPossibility.append(math.log2(matrixValue))
                        if len(lastState) == 0:
                            lastState.append(type.index('m'))
                            lastPossibility.append(0)
                            footprint.append(type.index('m'))
                        else:
                            maxPossibility = -100000
                            maxState = 0
                            for j in range(0, len(lastState)):
                                if maxPossibility < lastPossibility[j]:
                                    maxPossibility = lastPossibility[j]
                                    maxState = lastState[j]
                            footprint.append(maxState)
                        for i in range(1, len(question)):
                            for j in range(0, len(type)):
                                max = -100000
                                for l in range(0, len(lastState)):
                                    matrixValue = transit[type[lastState[l]]].get(type[j], 0)
                                    matrixValue /= transit[type[lastState[l]]]['ALL']
                                    matrixValue *= launch[type[j]].get(question[i], 0)
                                    matrixValue /= launch[type[j]]['ALL']
                                    if matrixValue > 0:
                                        now = lastPossibility[l] + math.log2(matrixValue)
                                        if max < now:
                                            max = now
                                if max > -100000:
                                    nowState.append(j)
                                    nowPossibility.append(max)
                            if len(nowState) == 0:
                                nowState.append(type.index('m'))
                                nowPossibility.append(0)
                            max = -100000
                            maxFoot = 0
                            for j in range(0, len(nowState)):
                                if nowPossibility[j] > max:
                                    max = nowPossibility[j]
                                    maxFoot = nowState[j]
                            footprint.append(maxFoot)
                            lastState = nowState
                            lastPossibility = nowPossibility
                            nowState = []
                            nowPossibility = []
                        for i in range(0, len(footprint)):
                            print(question[i] + ' ' + type[footprint[i]])
                            output.write(question[i] + ' ' + type[footprint[i]])
                            if (type[footprint[i]] == answer[i]):
                                right += 1
                            else:
                                wrong += 1
                        question.clear()
                        answer.clear()
                        lastPossibility.clear()
                        lastState.clear()
                        nowState.clear()
                        nowPossibility.clear()
                        footprint.clear()
                        output.write("\n")
print("Right:" + str(right) + " Wrong:" + str(wrong))
print("Rate:" + str(right / (wrong + right) * 100))
