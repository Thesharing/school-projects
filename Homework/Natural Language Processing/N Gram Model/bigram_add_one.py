# -*- coding: utf-8 -*-
import re
import math

dict1 = {'keyword': 0}
dict2 = {'keyword': 0}
v = 384720
t = 0.0
n = 0
with open("dict.txt", 'r', encoding='utf-8') as f:
    for line in f.readlines():
        a = re.split(r'\s+', line)
        dict1[a[0]] = int(a[1])
with open("dict2.txt", 'r', encoding='utf-8') as f2:
    for line in f2.readlines():
        a = re.split(r'\s+', line)
        dict2[a[0]] = int(a[1])
with open("test.txt", 'r', encoding='utf-8') as ft:
    for line in ft.readlines():
        a = re.split(r'\s+', line)
        a.pop()
        n += len(a)
        a.insert(0, "START")
        a.append("STOP")
        s = 0
        for i in range(0, len(a) - 1):
            l = (dict2.get(a[i] + '/' + a[i + 1], 0) + 0.001) / (dict1.get(a[i], 0) - dict2.get(a[i] + '/STOP', 0) + 0.001*v)
            t += math.log(l, 2)
    t = math.pow(2, -t / n)
    print(str(t))
