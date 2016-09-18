# -*- coding: utf-8 -*-
import re


def strTr(trstring):
    rstring = "" # convert half-width char to full-width char
    for uchar in trstring:
        inside_code = ord(uchar)
        if inside_code != 32:  # space
            if inside_code > 32 and inside_code <= 126:  # char (not space)
                inside_code += 65248
            rstring += chr(inside_code)
    return rstring


dict = {'keyword': '0'} # read the dictionary from dict.txt
with open("dict.txt", 'r', encoding='utf-8') as f:
    for line in f.readlines():
        a = re.split(r'\s+', line)
        dict[a[0]] = a[1] # for eve
print("Dict Reading Completed.")

with open("example.txt", 'r', encoding='utf-8') as g: # example.txt is the text waiting to be segmented
    with open("segmented.txt", 'w', encoding='utf-8') as h: # save segmented text in segmented.txt
        segment = [0]
        segmentWord = []
        for line in g.readlines():
            unRecBool = False # if there exist words not found in dictionary, unRecBool is True
            inputStr = strTr(line[:len(line) - 1])
            inputLen = len(inputStr)
            segment.clear()
            segment.append(0)
            i = 0
            k = 0
            while i < inputLen:
                for j in range(6, 0, -1):
                    if i + j <= inputLen:
                        if inputStr[i:i + j] in dict:
                            if unRecBool: # pop all the words not segmented before
                                segment.append(i - 1)
                                unRecBool = False
                            segment.append(i + j)
                            i += j
                            k = 1
                            break
                if k == 0: # if words not found in the dictionary 
                    unRecBool = True
                    i += 1
                else:
                    k = 0
            if unRecBool:
                segment.append(i)
            if len(segment) > 0: # Write the whole sentence to file
                for i in range(len(segment) - 1):
                    segmentWord.append(inputStr[segment[i]:segment[i + 1]])
            for wordi in segmentWord:
                print(wordi + '/', end="")
                h.write(wordi + ' ')
            segmentWord.clear()
            print()
            h.write("\n")
