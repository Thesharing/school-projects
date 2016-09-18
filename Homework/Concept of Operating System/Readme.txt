实验2 子进程中计算斐波那契数列
需要使用命令行打开，（在Win8/Win10系统中）点击右上角选择“打开命令提示符”，输入"SE2 （斐波那契数列长度）"运行。例如，输入"SE2 10"，显示"0 1 1 2 3 5 8 13 21 34 55"，共11项（从第0项开始）。

实验3 模拟FIFO和LRU页面置换算法
直接运行，在提示"Input the total number of frames:"后输入帧数（题目要求限制在1-7），在提示"Input the count of reference (Negative Number to Quit):"后输入有多少次页面访问。将随机生成一个页面号0-9的序列，然后按照FIFO和LRU进行计算。输入'q'将结束程序。

例如：
Input the total number of frames:5
Input the count of reference (Negative Number to Quit):15
FIFO
1               5       -1      -1      -1      -1              Now Referring:5
2               5       4       -1      -1      -1              Now Referring:4
3               5       4       1       -1      -1              Now Referring:1
4               5       4       1       3       -1              Now Referring:3
5               5       4       1       3       -1              Now Referring:5
6               5       4       1       3       -1              Now Referring:4
7               5       4       1       3       9               Now Referring:9
8               7       4       1       3       9               Now Referring:7
9               7       0       1       3       9               Now Referring:0
10              7       0       1       3       9               Now Referring:3
11              7       0       2       3       9               Now Referring:2
12              7       0       2       1       9               Now Referring:1
13              7       0       2       1       3               Now Referring:3
14              7       0       2       1       3               Now Referring:3
15              7       0       2       1       3               Now Referring:7
Total Faults: 10

LRU
1               5       -1      -1      -1      -1              Now Referring:5
2               5       4       -1      -1      -1              Now Referring:4
3               5       4       1       -1      -1              Now Referring:1
4               5       4       1       3       -1              Now Referring:3
5               5       4       1       3       -1              Now Referring:5
6               5       4       1       3       -1              Now Referring:4
7               5       4       1       3       9               Now Referring:9
8               5       4       7       3       9               Now Referring:7
9               5       4       7       0       9               Now Referring:0
10              3       4       7       0       9               Now Referring:3
11              3       2       7       0       9               Now Referring:2
12              3       2       7       0       1               Now Referring:1
13              3       2       7       0       1               Now Referring:3
14              3       2       7       0       1               Now Referring:3
15              3       2       7       0       1               Now Referring:7
Total Faults: 10
