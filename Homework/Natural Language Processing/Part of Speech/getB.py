# -*- coding: utf-8 -*-
import re

dict = {'keyword': {'type':0}}
with open("unprepared-training.txt", 'r', encoding='utf-8') as f:
    for line in f.readlines():
        a = re.split(r'\s+', line[23:])
        for i in range(0, len(a) - 1):
            b = re.split('/', a[i])
            if b[1] in dict:
                if b[0] in dict [b[1]]:
                    dict[b[1]][b[0]] += 1
                else:
                    dict[b[1]][b[0]] = 1
            else:
                dict[b[1]] = {'ALL':0}
                dict[b[1]][b[0]] = 1
            dict[b[1]]['ALL'] +=1
with open("b.txt", 'w', encoding='utf-8') as f2:
    items = dict.items()
    items = sorted(items)
    for key,value in items:
        items2 = value.items()
        items2 = sorted(items2)
        for key2,value2 in items2:
            f2.write(key+'/'+key2+'/'+str(value2)+'\n')
