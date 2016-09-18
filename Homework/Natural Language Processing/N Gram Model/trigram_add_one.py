# -*- coding: utf-8 -*-
import re
import math

dict2 = {'keyword': 0}
dict3 = {'keyword': 0}
v = 701416
start = 15229
t = 0.0
n = 0
with open("dict2.txt", 'r', encoding='utf-8') as f:
    for line in f.readlines():
        a = re.split(r'\s+', line)
        dict2[a[0]] = int(a[1])
with open("dict3.txt", 'r', encoding='utf-8') as f2:
    for line in f2.readlines():
        a = re.split(r'\s+', line)
        dict3[a[0]] = int(a[1])
max = 0
for item in dict2:
    if max < dict2[item]:
        max = dict2[item]
with open("test.txt", 'r', encoding='utf-8') as ft:
    for line in ft.readlines():
        a = re.split(r'\s+', line)
        a.pop()
        n += len(a)
        a.insert(0, "START")
        a.append("STOP")
        s = 0
        l = (dict2.get(a[0] + '/' + a[1], 0) + 0.001) / (start + 0.001)
        t += math.log(l, 2)
        for i in range(0, len(a) - 2):
            if (dict3.get(a[i] + '/' + a[i + 1] + '/' + a[i + 2], 0)) == 0 or dict2.get(a[i] + '/' + a[i + 1], 0) == 0:
                l = 1 / max
            else:
                l = (dict3.get(a[i] + '/' + a[i + 1] + '/' + a[i + 2], 0)) / dict2.get(a[i] + '/' + a[i + 1], 0)
            t += math.log(l, 2)
    t = math.pow(2, -t / n)
    print(str(t))
