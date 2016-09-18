trainning.py：训练词典，语料为training.txt，词典为dict.txt
main.py：主程序，读取dict.txt作为词典，读取example.txt作为测试样例，将分词结果写入segmentd.txt
prepare_example.py：准备测试样例，将测试样例中的词性标注删除，同时将所有分词标志（空格）删除
judge.py：读取分词结果segmented.txt和答案answer.txt，比较得到分词F值