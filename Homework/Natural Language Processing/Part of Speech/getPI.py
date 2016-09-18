# -*- coding: utf-8 -*-
import re

dict = {'keyword': 0}
all = 0
with open("unprepared-training.txt", 'r', encoding='utf-8') as f:
    for line in f.readlines():
        if len(line) > 23:
            a = re.split(r'\s+', line[23:])
            b = re.split('/', a[0])
            if b[1] in dict:
                dict[b[1]] += 1
            else:
                dict[b[1]] = 1
with open("pi.txt", 'w', encoding='utf-8') as f2:
    items = dict.items()
    items = sorted(items)
    for key,value in items:
        f2.write(key+' '+str(value)+'\n')
        all+=value
    f2.write('ALL '+str(all)+'\n')
print("Preparation Finished.")