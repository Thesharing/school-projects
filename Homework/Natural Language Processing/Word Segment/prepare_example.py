# -*- coding: utf-8 -*-
import re

with open("unprepared.txt", 'r', encoding='utf-8') as f: # to prepare the unsegemented text and segmented answer
    with open("example.txt", 'w', encoding='utf-8') as g:
        with open("answer.txt", 'w', encoding='utf-8') as h:
            for line in f.readlines():
                if len(line) > 23:
                    a = re.split(r'\s+', line[23:])
                    for i in range(0, len(a)):
                        b = re.split('/', a[i])
                        g.write(b[0])
                        h.write(b[0] + " ")
                    g.write("\n")
                    h.write("\n")
print("Example Preparation Completed.")
