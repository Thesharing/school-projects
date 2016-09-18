#include <iostream>
#include <cstdio>
#include <cmath> 
#include "libxl.h" //用于读取excel文件

using namespace libxl; //用于读取excel文件
using namespace std;

#define NUM1 22 
#define NUM2 42

struct closeEdgeNode{
	int adjvex;
	double lowcost;
};

//数据和文件处理
int readData(Book* book, int id[], double arc[][50], wchar_t loadFileName[], int sheetNum, int n);
//普里姆算法求最小生成树
void MiniSpanTree_PRIM(double arc[][50], int id[], int num);

int main(void){
	//从excel文件中读取数据 
	Book* book = xlCreateBook();
	if (!book){
		cout << "Error when init book." << endl;
		return -1;
	}
	int id[50];
	double arc[50][50];
	wchar_t loadFileName1[] = L"附件1-1.基站图的邻接矩阵-v1.xls";
	if (readData(book, id, arc, loadFileName1, 1, NUM1) <= 0)
		return -2;
	//普里姆算法求22个基站的最小生成树
	MiniSpanTree_PRIM(arc, id, NUM1);

	Book* book2 = xlCreateBook();
	if (!book2){
		cout << "Error when init book." << endl;
		return -1;
	}
	if (readData(book2, id, arc, loadFileName1, 0, NUM2) <= 0)
		return -2;
	//普里姆算法求42个基站的最小生成树
	MiniSpanTree_PRIM(arc, id, NUM2);
	system("PAUSE");
	return 0;
}

void MiniSpanTree_PRIM(double arc[][50], int id[], int num){
	//普里姆算法求最小生成树
	int u;
	double sum = 0;
	printf("Please input the starting vertex of minimum spanning tree:");
	cin >> u;
	struct closeEdgeNode closedge[50];
	//用点u来初始化各个节点的closedge
	int k = u - 1;
	for (int j = 0; j < num; j++)
	{
		if (j != k){
			closedge[j].adjvex = k;// id[k];
			closedge[j].lowcost = arc[k][j];
		}
	}
	closedge[k].lowcost = 0;

	//求出closedge中的最小值，将此点加入最小生成树S中，然后用该点到其他点的值更新closedge
	for (int i = 1; i < num; i++){
		int b = 1;
		k = 0;
		for (double min = closedge[0].lowcost; b < num; b++){
			if (min == 0 && closedge[b].lowcost != 0){
				min = closedge[b].lowcost;
				k = b;
			}
			else if (closedge[b].lowcost != 0 && closedge[b].lowcost<min){
				min = closedge[b].lowcost;
				k = b;
			}
		}
		printf("(%d,%d)\n", closedge[k].adjvex + 1, k + 1);
		sum += arc[closedge[k].adjvex][k];
		closedge[k].lowcost = 0;
		for (int j = 0; j < num; j++)
			if (arc[k][j] < closedge[j].lowcost)
			{
				closedge[j].adjvex = k;
				closedge[j].lowcost = arc[k][j];
			}

	}
	cout << "The sum is " << sum << endl;
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
	Sheet* sheet = book->getSheet(sheetNum);//读取excel文件中的sheet
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
