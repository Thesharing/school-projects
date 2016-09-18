#include <iostream>
#include <cstdbool>
#include <cstdio>
#include <cmath>
#include <ctime>
#include "libxl.h" //用于读取excel文件

using namespace libxl; //用于读取excel文件
using namespace std;

#define NUM1 22 //第一个文件是22个基站的邻接矩阵
#define NUM2 42

int colorNum;
int arc[50][50];
int ans[50];
long int sum;
long long int nodeNum;
bool isFirst;

void backtrack(int t, int num);
void m_coloring(int num);
bool ok(int k);
//数据和文件处理
int readData(Book* book, int arc[][50], wchar_t loadFileName[], int sheetNum, int n);

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
	
	clock_t start, finish;
	double duration;

	for (colorNum = 1; colorNum <= NUM1; colorNum++){
		isFirst = true;
		sum = 0;
		nodeNum = 0;
		start = clock();
		backtrack(1, NUM1);
		finish = clock();
		duration = (double)(finish - start);
		if (sum != 0){
			cout << "The minimum number of color is " << colorNum << endl;
			cout << "The sum is " << sum << ".And the time is " << duration << endl;
			cout << "The nodes travelled:" << nodeNum << endl << endl;
			break;
		}
		else{
			cout << colorNum << ':' << "No Ans." << endl;
			cout << "The time is " << duration << endl;
		}
	}

	Book* book2 = xlCreateBook();
	if (!book2){
		cout << "Error when init book." << endl;
		return -1;
	}
	if (readData(book2, arc, loadFileName1, 0, NUM2) <= 0)
		return -2;

	for (colorNum = 1; colorNum <= NUM2; colorNum++){
		sum = 0;
		nodeNum = 0;
		start = clock();
		backtrack(1, NUM2);
		finish = clock();
		duration = (double)(finish - start);
		if (sum != 0){
			cout << "The minimum number of color is " << colorNum << endl;
			cout << "The sum is " << sum << ".And the time is " << duration << endl;
			cout << "The nodes travelled:" << nodeNum << endl;
			break;
		}
		else{
			cout << colorNum << ':' << "No Ans." << endl;
			cout << "The time is " << duration << endl;
		}
	}

	system("PAUSE");
	return 0;
}

void backtrack(int t,int num){ //对所有节点进行回溯染色
	nodeNum++;
	if (t > num){ //如果t>num，说明所有的点都已经正确染色，此时输出结果
		sum++;
		if (isFirst){
			for (int j = 1; j <= NUM1; j++){
				cout << "The color of node " << j << "is " << ans[j] << endl;
			}
			isFirst = false;
		}
	}
	else{ //此时还有点未被正确着色
		for (int i = 1; i <= colorNum; i++){
			ans[t] = i; //将第t个点染成第i色
			if (ok(t)) //如果不与前t个点冲突，说明可行，继续染下一个点
				backtrack(t + 1, num);
			ans[t] = 0;//将这个点的颜色置空以回溯
		}
	}
}

bool ok(int k){ //判断第k个节点的颜色是否与前k-1个节点冲突
	for (int j = 1; j < k; j++)
		if (arc[k][j] && (ans[j] == ans[k]))
			return false;
	return true;
}

//数据与文件处理
int readData(Book* book, int arc[][50], wchar_t loadFileName[], int sheetNum, int n){ //将数据从excel文件中读出
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
				arc[i+1][j+1] = (int)sheet->readNum(i + 2, j + 2);
				if (arc[i+1][j+1] == -1)
					arc[i+1][j+1] = 0;
				else
					arc[i+1][j+1] = 1;
			}
	}
	book->release();
	return 1;
}
