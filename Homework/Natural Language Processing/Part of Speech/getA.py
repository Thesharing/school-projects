# -*- coding: utf-8 -*-
import re

dict = {'keyword': 0}
with open("unprepared-training.txt", 'r', encoding='utf-8') as f:
    for line in f.readlines():
        a = re.split(r'\s+', line[23:])
        for i in range(0, len(a) - 1):
            b = re.split('/', a[i])
            c = re.split('/',a[i+1])
            if(len(b)>1 and len(c)>1):
                strTemp = b[1]+'/'+c[1]
            if strTemp in dict:
                dict[strTemp] += 1
            else:
                dict[strTemp] = 1
            if b[1]+'/ALL' in dict:
                dict[b[1]+'/ALL'] +=1
            else:
                dict[b[1]+'/ALL'] = 1
with open("a.txt", 'w', encoding='utf-8') as f2:
    items = dict.items()
    items = sorted(items)
    for key,value in items:
        f2.write(key+' '+str(value)+'\n')