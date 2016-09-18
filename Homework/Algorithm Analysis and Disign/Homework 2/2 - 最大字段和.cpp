#include <iostream>
#include <cstdlib>

using namespace std;

float maxSum(int n, float a[], int &x, int &y); //求a[]中的最大字段和 

int main(void){
	//求最大字段和 
	//从文件读取a和b两个字段 
	FILE * fileTwo, *fileThree;
	fileTwo = fopen("2.txt", "r");
	fileThree = fopen("3.txt", "r");
	int m = 350, n = 180, i = 0;
	float a[350], b[180]; 
	for (i = 0; i < m; i++)
		fscanf(fileTwo, "%f", &a[i]);
	for (i = 0; i < n; i++)
		fscanf(fileThree, "%f", &b[i]);
	
	int aX, aY, bX, bY; //aX和aY分别对应a[]字段的最大字段和的起始位置和终止位置 
	
	//求出a和b的最大字段和 
	float aSum = maxSum(m, a, aX, aY);
	float bSum = maxSum(n, b, bX, bY);
	cout << "The max sum of subsequence of A is " << aSum << " from " << aX << " to " << aY << endl;
	cout << "The max sum of subsequence of B is " << bSum << " from " << bX << " to " << bY << endl;
	system("PAUSE");
	return 0;
}

float maxSum(int n, float a[], int &x, int &y) { //求出a[]的最大字段和 
	float sum = 0; 
	float b = 0;
	for (int i = 0; i < n; i++){
		if (b >= 0) //当从x到i的字段和为正时，说明上一个字a[i-1]>0 
			b += a[i];
		else{ //如果从x到i的字段和为负，说明上一个字a[i-1]<0，从i开始重新计x和sum 
			x = i + 1;
			b = a[i];
		}
		if (b > sum){ //如果b>sum的话，就令sum=b，且记此时的i为最大字段和的终止位置y 
			sum = b;
			y = i + 1;
		}
	}
	return sum; //返回最大字段和 
}
