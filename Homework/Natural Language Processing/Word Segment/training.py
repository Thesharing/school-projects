# -*- coding: utf-8 -*-
import re

dict = {'keyword': 0}
with open("training.txt", 'r', encoding='utf-8') as f: # train the dictionary by training.txt
    for line in f.readlines():
        if len(line) > 23:
            a = re.split(r'\s+', line[23:])
            for i in range(0, len(a)):
                b = re.split('/', a[i])
                dictKey = b[0]
                if dictKey in dict:
                    dict[dictKey] += 1
                else:
                    dict[dictKey] = 1

with open("dict.txt", 'w', encoding='utf-8') as f2: # save the dictionary to dict.txt
    items = dict.items()
    items = sorted(items)
    for key, value in items:
        if (len(key) > 1) and (key[0] == '['):
            key = key[1:]
        f2.write(key + ' ' + str(value) + '\n')

print("Training Finished.")
