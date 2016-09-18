#include <iostream>
#include <cstdbool>
#include <cstdio>
#include <cmath>
#include <ctime>
#include "libxl.h" //用于读取excel文件

using namespace libxl; //用于读取excel文件
using namespace std;

#define NUM1 22 //第一个文件是22个基站的邻接矩阵

double arc[50][50];
int ans[50];
int bestAns[50];
double value;
double bestValue;
int nodeNum;

void tsp(int num);
void backtrack(int i, int num);


//数据和文件处理
int readData(Book* book, double arc[][50], wchar_t loadFileName[], int sheetNum, int n);

int main(void){
	//从excel文件中读取数据 
	Book* book = xlCreateBook();
	if (!book){
		cout << "Error when init book." << endl;
		return -1;
	}

	//读取数据
	wchar_t loadFileName1[] = L"附件1-1.基站图的邻接矩阵-v1.xls";
	if (readData(book, arc, loadFileName1, 1, NUM1) <= 0)
		return -2;
	
	//计时
	clock_t start, finish;
	double duration;

	start = clock();
	tsp(NUM1);
	finish = clock();
	duration = (double)(finish - start);
	
	//输出结果
	cout << "The shortest length is " << bestValue << endl;
	cout << "The time cost is " << duration << endl;
	cout<< "The node travelled: " << nodeNum << endl;
	cout << "The road is: ";
	for (int i = 1; i <= NUM1; i++){
		cout << bestAns[i] << "->";
	}
	cout << endl;

	system("PAUSE");
	return 0;
}

void tsp(int num){ //初始化变量并开始回溯过程
	for (int i = 1; i <= num; i++)
		ans[i] = i;
	bestValue = 10000;
	value = 0;
	nodeNum = 0;
	backtrack(2,num);
}

void backtrack(int i,int num){ //回溯遍历各个节点
	nodeNum++;
	int temp;
	if (i == num){ //如果满足条件，则将这个结果记为最优结果
		if (arc[ans[num - 1]][ans[num]] < 10000 &&
			arc[ans[num]][1] < 10000 &&
			(bestValue == 10000 || value + arc[ans[num - 1]][ans[num]] + arc[ans[num]][1] < bestValue)){
			for (int j = 1; j <= num; j++)
				bestAns[j] = ans[j];
			bestValue = value + arc[ans[num - 1]][ans[num]] + arc[ans[num]][1];
		}
	}
	else{ //如果不满足条件，交换i、j向下一个节点回溯
		for (int j = i; j <= num;j++)
			if (arc[ans[i - 1]][ans[j]] < 10000 && (bestValue == 10000 || value + arc[ans[i - 1]][ans[j]] < bestValue)){
				temp = ans[i];
				ans[i] = ans[j];
				ans[j] = temp;
				
				value += arc[ans[i - 1]][ans[i]];
				backtrack(i + 1,num);
				value -= arc[ans[i - 1]][ans[i]];
				
				temp = ans[i];
				ans[i] = ans[j];
				ans[j] = temp;
			}
	}
}

//数据与文件处理
int readData(Book* book, double arc[][50], wchar_t loadFileName[], int sheetNum, int n){ //将数据从excel文件中读出
	if (book->load(loadFileName)){ //读取book
		cout << "已读取文件。" << endl;
	}
	else{
		cout << "读取文件时错误。" << endl;
		return -2;
	}
	Sheet* sheet = book->getSheet(sheetNum);//读取excel文件中的sheet2 
	if (sheet){ //将sheet中的数据复制到结构数组中
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++){
				arc[i+1][j+1] = sheet->readNum(i + 2, j + 2);
				if (arc[i+1][j+1] == -1)
					arc[i+1][j+1] = 10000;
			}
	}
	book->release();
	return 1;
}
