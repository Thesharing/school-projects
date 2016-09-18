# -*- coding: utf-8 -*-

import re
import math
import os

dictionary = {'type': {'word': 0}}
with open('dict.txt', 'r', encoding='utf-8') as f:
    for line in f.readlines():
        words = re.split(r'\s+', line)
        if words[0] in dictionary:
            if words[1] in dictionary[words[0]]:
                dictionary[words[0]][words[1]] += 1
            else:
                dictionary[words[0]][words[1]] = 1
        else:
            dictionary[words[0]] = {'type': 0}
            dictionary[words[0]].pop('type')
            dictionary[words[0]][words[1]] = 1
dictionary.pop('type')
with open('dictALL.txt','r',encoding='utf-8') as f:
    for line in f.readlines():
        words = re.split(r'\s+', line)
        dictionary[words[0]]['ALL']=int(words[1])

possibility = {'type': 0}
possibility.pop('type')
testAll = 0
testRight = 0
for type in dictionary.keys():
    possibility[type] = 0
folderName = 'G:\\Download\\Test'
files = os.listdir(folderName)
for file in files:
    for type in dictionary.keys():
        with open(folderName + '\\' + file, 'r', encoding='Windows 1252') as f:
            for line in f.readlines():
                words = re.split(r'\s+', line)
                for i in range(0, len(words)):
                    if words[i] in dictionary[type]:
                        possibility[type] += math.log2(dictionary[type][words[i]] / dictionary[type]['ALL'])
                    else:
                        possibility[type] += -100
    max = possibility['sci.electronics']
    result = 'sci.electronics'
    for type in dictionary.keys():
        if max < possibility[type]:
            max = possibility[type]
            result = type
    print("The type of the text is: " + result + "  Answer:"+file)
    if result in file:
        testRight+=1
    testAll+=1
    for type in dictionary.keys():
        possibility[type] = 0
print("All:"+str(testAll)+" Right:"+str(testRight))