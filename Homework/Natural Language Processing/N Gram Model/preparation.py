# -*- coding: utf-8 -*-
import re

dict = {'keyword': 0}
with open("unprepared-training.txt", 'r', encoding='utf-8') as f:
    with open("training.txt", 'w', encoding='utf-8') as f2:
        for line in f.readlines():
            if len(line) > 23:
                a = re.split(r'\s+', line[23:])
                for i in range(0, len(a)):
                    b = re.split('/', a[i])
                    f2.write(b[0]+" ")
                f2.write("\n")
print("Preparation Finished.")
