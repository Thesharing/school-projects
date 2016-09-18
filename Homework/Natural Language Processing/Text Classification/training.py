# -*- coding: utf-8 -*-

import os
import re

dictionary = {'type': {'word': 0}}
stopWord = []
with open('EnglishStopWords.txt', 'r', encoding='utf-8') as f:
    for line in f.readlines():
        stopWord.append(line)
folderName = 'G:\\Download\\20_newsgroups'
folder = os.listdir(folderName)
for eachFolder in folder:
    dictionary[eachFolder] = {'keyword': 0}
    dictionary[eachFolder].pop('keyword')
    files = os.listdir(folderName + '\\' + eachFolder)
    for file in files:
        with open(folderName + '\\' + eachFolder + '\\' + file, 'r', encoding='Windows 1252') as f:
            for line in f.readlines():
                words = re.split(r'\s+', line)
                for word in words:
                    if not (word in stopWord):
                        if word in dictionary[eachFolder]:
                            dictionary[eachFolder][word] += 1
                        else:
                            dictionary[eachFolder][word] = 1
with open('dict.txt', 'w', encoding='utf-8') as f:
    for key, value in dictionary.items():
        for key1, value1 in value.items():
            f.write(key + ' ' + key1 + ' ' + str(value1) + '\n')
