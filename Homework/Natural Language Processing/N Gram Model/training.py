# -*- coding: utf-8 -*-
import re

dict = {'keyword': 0} # use train.txt to train the dictionary and save it to dict.txt
time1=0.0
dict2 = {'keyword': 0} # put one word, two consecutive words, three consecutive words and four consecutive words into corresponding dict 
time2=0.0
dict3 = {'keyword': 0}
time3=0.0
dict4 = {'keyword': 0}
time4=0.0
with open("training.txt", 'r', encoding='utf-8') as f:
    for line in f.readlines():
        a = re.split(r'\s+', line)
        a.insert(0, "START")
        for i in range(0, len(a) - 1):
            strTemp = a[i] # read every word
            if a[i] in dict:
                dict[a[i]] += 1
            else:
                dict[a[i]] = 1
            time1+=1
            if i + 1 < len(a) - 1: # read one more word
                strTemp += '/' + a[i + 1]
                if strTemp in dict2:
                    dict2[strTemp] += 1
                else:
                    dict2[strTemp] = 1
                time2+=1
                if i + 2 < len(a) - 1: # read three words
                    strTemp += '/' + a[i + 2]
                    if strTemp in dict3:
                        dict3[strTemp] += 1
                    else:
                        dict3[strTemp] = 1
                    time3+=1
                    if i + 3 < len(a) - 1: # four words
                        strTemp += '/' + a[i + 3]
                        if strTemp in dict4:
                            dict4[strTemp] += 1
                        else:
                            dict4[strTemp] = 1
                        time4+=1
                    else:
                        strTemp += '/STOP'
                        if strTemp in dict4:
                            dict4[strTemp] += 1
                        else:
                            dict4[strTemp] = 1
                else:
                    strTemp += '/STOP'
                    if strTemp in dict3:
                        dict3[strTemp] += 1
                    else:
                        dict3[strTemp] = 1
            else:
                strTemp += '/STOP'
                if strTemp in dict2:
                    dict2[strTemp] += 1
                else:
                    dict2[strTemp] = 1
with open("dict.txt", 'w', encoding='utf-8') as f2: # store dicts into dict.txt
    items = dict.items()
    items = sorted(items)
    for key, value in items:
        f2.write(key + ' ' + str(value) + '\n')
    print(str(time1))
with open("dict2.txt", 'w', encoding='utf-8') as f3:
    items = dict2.items()
    items = sorted(items)
    for key, value in items:
        f3.write(key + ' ' + str(value) + '\n')
    print(str(time2))
with open("dict3.txt", 'w', encoding='utf-8') as f4:
    items = dict3.items()
    items = sorted(items)
    for key, value in items:
        f4.write(key + ' ' + str(value) + '\n')
        time3+=value
    print(str(time3))
with open("dict4.txt", 'w', encoding='utf-8') as f5:
    items = dict4.items()
    items = sorted(items)
    for key, value in items:
        f5.write(key + ' ' + str(value) + '\n')
    print(str(time4))