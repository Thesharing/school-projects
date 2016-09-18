# -*- coding: utf-8 -*-

import re

possibilityAll = 0
typeWord = 'comp.sys.mac.hardware'
with open('dict.txt', 'r', encoding='utf-8') as f:
    with open('dictAll.txt', 'w', encoding='utf-8') as f1:
        for line in f.readlines():
            words = re.split(r'\s+', line)
            if not (words[0] == typeWord):
                f1.write(typeWord + ' ' + str(possibilityAll) + '\n')
                typeWord = words[0]
                try:
                    possibilityAll += int(words[2])
                except ValueError:
                    print(words[0]+' '+words[1]+' '+words[2])
                    pass
            else:
                try:
                    possibilityAll += int(words[2])
                except ValueError:
                    print(words[0]+' '+words[1]+' '+words[2])
                    pass
        f1.write(typeWord + ' ' + str(possibilityAll) + '\n')