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
	double longitude, latitude;  //精度和纬度
};

//数据和文件处理
int readData(Book* book, struct baseData data[], wchar_t loadFileName[], int n);
//求凸多边形的最优三角剖分对应的各边的权值 
double minWeightTriangulation(int n, struct baseData data[], int &vex);
double dist(struct baseData a, struct baseData b); // dist用于计算Va和Vb的距离 
double w(struct baseData * data, int a, int b, int c); //w用于计算Va,Vb和Vc构成的三角形的权值
//通过s[][]来构造最优三角剖分中的子三角形 
void Traceback(int n, struct baseData data[], double &sum);

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
	double sum = 0;
	cout << "21 个基站凸多边形最优三角剖分的所有三角形如下:" << endl;
	Traceback(20, data1, sum);//计算最优三角划分并将结果进行输出

	//读取第二份数据“29个基站凸多边形数据”然后计算29个基站的凸多边形最优三角剖分
	Book* book2 = xlCreateBook();
	if (!book2){
		cout << "Error when init book." << endl;
		return -1;
	}
	struct baseData data2[30];
	wchar_t loadFileName2[] = L"附件3-2.29个基站凸多边形数据.xls";
	if (readData(book2, data2, loadFileName2, NUM2) <= 0)
		return -2;
	cout << endl << "29 个基站凸多边形最优三角剖分的所有三角形如下:" <<endl;
	Traceback(28, data2, sum);//计算最优三角划分并将结果输出

	system("PAUSE");
	return 0;
}


double minWeightTriangulation(int n, struct baseData data[], int &vex){ //求出边长最小的相邻三点组成的三角形的边长并将三角形的一个顶点返回
	double min;
	min = w(data, 0, 1, 2); //先将V0,V1,V2组成的三角形组成的三角形设为最小值
	vex = 0;
	for (int i = 1; i <= n - 4; i++){
		if (min > w(data, i, i + 1, i + 2))
		{
			min = w(data, i, i + 1, i + 2);
			vex = i;
		}
	}
	//在遍历完V0V1V2,V1V2V3,...,Vn-3Vn-2Vn-1以后
	if (min > w(data, n - 2, n - 1, 0)){ //看Vn-2Vn-1V0是否是最小值
		min = w(data, n - 2, n - 1, 0);
		vex = n - 2;
	}
	if (min > w(data, n - 1, 0, 1)){ //看Vn-1V0V1是否是最小值
		min = w(data, n - 1, 0, 1);
		vex = n - 1;
	}
	return min;
}

void Traceback(int n, struct baseData data[], double &sum){
	sum = 0;
	int i = 0;
	for (; n > 3; n--){
		sum += minWeightTriangulation(n, data, i); //在求出最小的三角形边长以及对应顶点后
		if (i == n - 2) //如果是第n-2个点则直接输出
			cout << data[n - 2].num << "\t" << data[n - 1].num <<"\t" << data[0].num << endl;
		else if (i == n - 1){ //如果是第n-1个点则在输出后将data[0]删除
			cout << data[n - 1].num << "\t" << data[0].num << "\t"<< data[1].num << endl;
			for (int j = 0; j < n - 1; j++)
				data[j] = data[j + 1];
		}
		else{//其他情况则在输出后将data[i+1]删除
			cout << data[i].num << "\t" << data[i + 1].num << "\t"<< data[i + 2].num << endl;
			for (int j = i + 1; j < n - 1; j++)
				data[j] = data[j + 1];
		}
	}
	sum += w(data, 0, 1, 2); //将最后的V0V1V2加进去
	cout << data[0].num << "\t" << data[1].num << "\t" << data[2].num <<endl; //输出V0V1V2
	cout << "最优三角剖分值为：" << sum << endl << endl; //输出结果
}

double w(struct baseData * data, int a, int b, int c){ //计算三角形VaVbVc的三边之和
		return (dist(data[a], data[b]) + dist(data[a], data[c]) + dist(data[b], data[c]));
}

double dist(struct baseData a, struct baseData b){ //已知经度和纬度求距离
	return RADIUM*acos(cos(a.latitude*PI / 180)*cos(b.latitude*PI /180)*cos(a.longitude*PI / 180 - b.longitude*PI / 180) + sin(a.latitude*PI /180)*sin(b.latitude*PI / 180));
}

//数据与文件处理
int readData(Book* book, struct baseData data[], wchar_t loadFileName[], int

	n){ //将数据从excel文件中读出
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
			data[i].enodebid = (int)sheet->readNum(i + 1, 0);
			data[i].longitude = sheet->readNum(i + 1, 1);
			data[i].latitude = sheet->readNum(i + 1, 2);
		}
	}
	book->release();
	return 1;
}
