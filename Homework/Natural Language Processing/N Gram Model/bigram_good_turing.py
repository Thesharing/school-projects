# -*- coding: utf-8 -*-
import re
import math

dict1 = {'keyword': 0}
dict2 = {'keyword': 0}
time1 = {0: 0}
time2 = {0: 0}
t = 0.0
n = 0
dict1ALL = 0
with open("dict.txt", 'r', encoding='utf-8') as f:
    for line in f.readlines():
        a = re.split(r'\s+', line)
        dict1[a[0]] = int(a[1])
        dict1ALL += int(a[1])
with open("dict2.txt", 'r', encoding='utf-8') as f2:
    for line in f2.readlines():
        a = re.split(r'\s+', line)
        dict2[a[0]] = int(a[1])

items = dict1.items()
for key, value in items:
    if value in time1:
        time1[value] += 1
    else:
        time1[value] = 1
keySorted = sorted(time1.keys())
for key in keySorted[1:10]:
    time1[key] = (key + 1) * time1[key + 1] / time1[key]
for i in range(11, 100000):
    time1[i] = i

items = dict2.items()
for key, value in items:
    if value in time2:
        time2[value] += 1
    else:
        time2[value] = 1
keySorted = sorted(time2.keys())
for key in keySorted[1:10]:
    time2[key] = (key + 1) * time2[key + 1] / time2[key]
for i in range(11, 100000):
    time2[i] = i

with open("test.txt", 'r', encoding='utf-8') as ft:
    for line in ft.readlines():
        a = re.split(r'\s+', line)
        a.pop()
        n += len(a)
        a.insert(0, "START")
        a.append("STOP")
        s = 0
        for i in range(0, len(a) - 1):
            if dict2.get(a[i] + '/' + a[i + 1], 0) == 0 or dict1.get(a[i], 0) == 0:
                l = time1[dict1.get(a[i], 1)] / (dict1ALL * 10)
            else:
                l = time2[dict2.get(a[i] + '/' + a[i + 1])] / time1[dict1.get(a[i], 0)]
            t += math.log(l, 2)
    t = math.pow(2, -t / n)
    print(str(t))
