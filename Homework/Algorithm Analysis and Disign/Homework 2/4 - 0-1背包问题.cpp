#include <iostream>
#include <cmath> 
#include "libxl.h" //用于读取excel文件

using namespace libxl; //用于读取excel文件
using namespace std;

#define NUM1 21 //第一个文件是凸21边形，即21个顶点 
#define NUM2 29 //第二个文件是凸29边形，即29个顶点 
#define RADIUM 6378137 //半径
const double PI = acos(-1.0); //常数PI

struct baseData{ //定义基站数据的结构
	int num; //序号 
	int enodebid; //基站编号
	double longitude,latitude;  //精度和纬度
};

//数据和文件处理
int readData(Book* book, struct baseData data[], wchar_t loadFileName[], int n);
//求凸多边形的最优三角剖分对应的各边的权值 
void minWeightTriangulation(int n, double t[][30], int s[][30], struct baseData * data);
double dist(struct baseData a, struct baseData b); // dist用于计算Va和Vb的距离 
double w(struct baseData * data, int a, int b, int c); //w用于计算Va,Vb和Vc构成的三角形的权值 
//通过s[][]来构造最优三角剖分中的子三角形 
void Traceback(int i, int j, int s[][30]);

int main(void){
	//从excel文件中读取数据 
	Book* book = xlCreateBook();
	if (!book){
		cout << "Error when init book." << endl;
		return -1;
	}
	struct baseData data1[30];
	wchar_t loadFileName1[] = L"附件3-1.21个基站凸多边形数据.xls";
	if (readData(book, data1, loadFileName1, NUM1) <= 0)
		return -2;
		
	//定义t[][]和s[][]并初始化，t[i][j]记录了从Vi-1…Vk…到Vj组成的凸多边形的最优三角剖分的各边的权值
	//而s[i][j]则记录了与Vi-1和Vj一起构成三角形的顶点 
	double t[30][30];
	int s[30][30];
	for (int i = 0; i < 30; i++)
		for (int j = 0; j < 30; j++){
			t[i][j] = 0;
			s[i][j] = 0;
		}
	//计算凸多边形的最优三角剖分所对应的权函数值 
	minWeightTriangulation(NUM1 - 1, t, s, data1);
	cout << endl << t[1][NUM1-1] << endl;
	//根据s[][]来构造最优三角剖分的解 
	Traceback(1, NUM1 - 1, s);
	
	//读取第二份数据“29个基站凸多边形数据”并重新初始化s[][]和t[][] 
	Book* book2 = xlCreateBook();
	if (!book2){
		cout << "Error when init book." << endl;
		return -1;
	}
	struct baseData data2[30];
	wchar_t loadFileName2[] = L"附件3-2.29个基站凸多边形数据.xls";
	if (readData(book2, data2, loadFileName2, NUM2) <= 0)
		return -2;
	for (int i = 0; i < 30; i++)
		for (int j = 0; j < 30; j++){
			s[i][j] = 0;
			t[i][j] = 0;
		}
		
	
	minWeightTriangulation(NUM2 - 1, t, s, data2);
	cout << endl << t[1][NUM2 - 1] << endl;
	Traceback(1, NUM2 - 1, s);
	system("PAUSE");
	return 0;
}

void minWeightTriangulation(int n, double t[][30], int s[][30],struct baseData * data){
	//求凸多边形的最优三角剖分对应的各边的权值
	for (int i = 0; i <= n; i++) //当i=j时，t[i][j]=0
		t[i][i] = 0;
	for (int r = 2; r <= n; r++)
		for (int i = 1; i <= n - r + 1; i++){
			int j = i + r - 1;
			t[i][j] = t[i + 1][j] + w(data, i - 1, i, j); //当i<j时t[i][j]=t[i+1][j]+w(i-1,i,j)，即凸多边形Vi...Vj=(凸多边形Vi+1...Vj)+三角形Vi-1ViVj 
			s[i][j] = i;
			for (int k = i + 1; k < i + r - 1; k++){ //k从i遍历到j，当存在t[i][k]<t[i][j]时 更新t[i][j]和s[i][j] 
				double u = t[i][k] + t[k + 1][j] + w(data, i - 1, k, j);
				if (u < t[i][j]){
					t[i][j] = u;
					s[i][j] = k;
				}
			}
		}
}

double w(struct baseData * data, int a, int b, int c){ //计算三角形VaVbVc的三边之和 
	return (dist(data[a], data[b]) + dist(data[a], data[c]) + dist(data[b], data[c]));
}

double dist(struct baseData a, struct baseData b){ //已知经度和纬度求距离
	return RADIUM*acos(cos(a.latitude*PI / 180)*cos(b.latitude*PI / 180)*cos(a.longitude*PI / 180 - b.longitude*PI / 180) + sin(a.latitude*PI / 180)*sin(b.latitude*PI / 180));
}

void Traceback(int i, int j, int s[][30]){ //根据s[][]来推得最优三角剖分的解 
	if (i == j) return;
	cout << i - 1 << "," << s[i][j] << "," << j << endl;
	Traceback(i, s[i][j], s);
	Traceback(s[i][j] + 1, j, s);
}

//数据与文件处理
int readData(Book* book, struct baseData data[], wchar_t loadFileName[], int n){ //将数据从excel文件中读出
	if (book->load(loadFileName)){ //读取book
		cout << "已读取文件。" << endl;
	}
	else{
		cout << "读取文件时错误。" << endl;
		return -2;
	}
	Sheet* sheet = book->getSheet(1);//读取excel文件中的sheet2 
	if (sheet){ //将sheet中的数据复制到结构数组中
		for (int i = 0; i < n; i++){
			data[i].num = i + 1;
			data[i].enodebid = (int)sheet->readNum(i+1, 0);
			data[i].longitude = sheet->readNum(i+1, 1);
			data[i].latitude = sheet->readNum(i+1, 2);
		}
	}
	book->release();
	return 1;
}
