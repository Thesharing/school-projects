#include <iostream>
#include <cstdbool>
#include <cstdio>
#include <cmath> 
#include "libxl.h" //用于读取excel文件

using namespace libxl; //用于读取excel文件
using namespace std;

#define NUM1 22 //第一个文件是22个基站的邻接矩阵
#define NUM2 42 //第二个文件是42个基站的邻接矩阵

//数据和文件处理
int readData(Book* book, int id[], double arc[][50], wchar_t loadFileName[], int sheetNum, int n);
//迪杰斯特拉算法
void shortestPath_DIJ(double arc[][50], int v0, bool p[][50], double d[], int num, int prev[]);
//输出最短路径
void printOut(int prev[], int start, int end);

int main(void){
	//从excel文件中读取数据 
	Book* book = xlCreateBook();
	if (!book){
		cout << "Error when init book." << endl;
		return -1;
	}

	int id[50];
	double arc[50][50];

	//读取数据
	wchar_t loadFileName1[] = L"附件1-1.基站图的邻接矩阵-v1.xls";
	if (readData(book, id, arc, loadFileName1, 1, NUM1) <= 0)
		return -2;

	bool p[50][50];
	double d[50];
	int prev[50];
	//迪杰斯特拉算法求22个基站中一点到其他各点的最短路径
	shortestPath_DIJ(arc, 19, p, d, NUM1, prev);
	//输出第20个点（567443）到第1个点（33109）的最短路径
	printOut(prev, 19, 0);

	Book* book2 = xlCreateBook();
	if (!book2){
		cout << "Error when init book." << endl;
		return -1;
	}
	if (readData(book2, id, arc, loadFileName1, 0, NUM2) <= 0)
		return -2;
	//迪杰斯特拉算法求42个基站中一点到其他各点的最短路径
	shortestPath_DIJ(arc, 15, p, d, NUM2, prev);
	//输出第16个点（565845）到第2个点（565667）的最短路径
	printOut(prev, 15, 2);

	system("PAUSE");
	return 0;
}

void shortestPath_DIJ(double arc[][50], int v0, bool p[][50], double d[], int num, int prev[]) {
	//迪杰斯特拉求V0到其余各点的最短路径，arc是边长邻接矩阵，V0是源点，p[v][w]为true是指V0-V的路径上有w点，d[]是最短路径的长度
	//num是点的数量，prev[]是最短路径沿途各点，通过prev可以按序倒推最短路径

	//首先用V0初始化各个参数
	bool final[50];
	for (int i = 0; i < num; i++){
		final[i] = false;
		d[i] = arc[v0][i];
		for (int j = 0; j < num; j++)
			p[i][j] = false;
		if (d[i] < 10000){
			p[i][v0] = true;
			p[i][i] = true;
		}
		if (d[i] == 10000)
			prev[i] = 0;
		else
			prev[i] = v0;
	}
	d[v0] = 0;
	final[v0] = true;

	//然后求出V0到各点中路径最小值的点V，用V0-V来更新各点的最短路径，将结果记录下来
	for (int i = 1; i < num; i++){
		double min = 10000;
		int v = 0;
		for (int w = 0; w < num; w++)
			if (!final[w])
				if (d[w] < min){
					v = w;
					min = d[w];
				}
		final[v] = true;
		for (int w = 0; w < num; w++)
			if (!final[w] && (min + arc[v][w] < d[w])){
				//如果min+arc[v][w]<d[w]，说明V0-V-W为新的较短路径，更新记录并将prev[w]记录为v点
				d[w] = min + arc[v][w];
				for (int i = 0; i < num; i++)
					p[w][i] = p[v][i];
				p[w][w] = true;
				prev[w] = v;
			}
	}

	//输出结果
	for (int i = 0; i < num; i++){
		cout << "Port " << i << endl;
		for (int j = 0; j < num; j++)
			if (p[i][j] == true)
				cout << j << " ";
		cout << endl << d[i] << endl;
	}
	cout << endl;
}

//数据与文件处理
int readData(Book* book, int id[], double arc[][50], wchar_t loadFileName[], int sheetNum, int n){ //将数据从excel文件中读出
	if (book->load(loadFileName)){ //读取book
		cout << "已读取文件。" << endl;
	}
	else{
		cout << "读取文件时错误。" << endl;
		return -2;
	}
	Sheet* sheet = book->getSheet(sheetNum);//读取excel文件中的sheet2 
	if (sheet){ //将sheet中的数据复制到结构数组中
		for (int i = 0; i < n; i++){
			id[i] = (int)sheet->readNum(1, i + 2);
		}
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++){
				arc[i][j] = sheet->readNum(i + 2, j + 2);
				if (arc[i][j] == -1)
					arc[i][j] = 10000;
			}
	}
	book->release();
	return 1;
}

void printOut(int prev[], int start, int end){ //递归倒推出最短路径，从end到start
	cout << end << ' ';
	if (start != end){
		printOut(prev, start, prev[end]);
	}
}