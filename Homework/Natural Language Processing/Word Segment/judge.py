# -*- coding: utf-8 -*-
import re

with open("answer.txt", 'r', encoding='utf-8') as f: # use this to calculate the Precision and Recall Rate of Segmentation
    with open("segmented.txt", 'r', encoding='utf-8') as g:
        right = 0
        wordAnswer = 0
        wordSegment = 0
        for lineAns in f.readlines():
            lineSeg = g.readline()
            a = re.split(r'\s+', lineAns)
            wordAnswer += len(a)
            b = re.split(r'\s+', lineSeg)
            wordSegment += len(b)
            for i in a:
                if i in b:
                    right += 1 # if segmented words is in answer's corresponding sentence
        precision = right / wordSegment
        recall = right / wordAnswer
        f = 2 * precision * recall / (precision + recall)
        print("Segmented:" + str(wordSegment) + ", Answer:" + str(wordAnswer) + ", Right:" + str(right))
        print("Precision:" + str(precision * 100) + "%, Recall:" + str(recall * 100) + "%")
        print("The rate of successful segment is:" + str(f * 100) + "%")
