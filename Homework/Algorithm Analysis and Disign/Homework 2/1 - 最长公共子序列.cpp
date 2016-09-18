#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

int c[600][600], b[600][600]; //c[i][j]存储x[i]到y[j]的最长子序列的长度，而b[i][j]则存储c[i][j]是由哪一个子问题的解得到的 
//b[][]中有三种取值，1、2、3分别对应Zk=Xm=Yn，Xm!=Yn&Zk!=Xm，Xm!=Yn&Zk!=Yn三种情况  

void lcsLength(char x[], char y[], int c[][600], int b[][600]); //lcsLength用于求源序列X和Y的最长公共子序列的长度 
void lcs(int i, int j, char x[], int b[][600]); //通过b[][]可以构造出最长公共子序列，b[][]记录了原问题是由哪一种子问题的解得到的

int main(void){
	//求解最长公共子序列
	//从文件中读取序列a1、a2、a3、a4 
	FILE * fileOne; 
	fileOne = fopen("1.txt", "r");
	char a1[600], a2[600], a3[600], a4[600];
	char trash[600];
	fgets(trash, 600, fileOne);
	fgets(a1, 600, fileOne);
	fgets(trash, 600, fileOne);
	fgets(a2, 600, fileOne);
	fgets(trash, 600, fileOne);
	fgets(a3, 600, fileOne);
	fgets(trash, 600, fileOne);
	fgets(a4, 600, fileOne);
	
	//对序列a1、a2求最长公共子序列并构造结果 
	lcsLength(a1, a2, c, b);
	cout << "The longest common substring of A and B is:" << endl;
	lcs(strlen(a1), strlen(a2), a, y);
	
	//对序列a3、a4求最长公共子序列并构造结果 
	lcsLength(a3, a4, x, y);
	cout << "The longest common substring of C and D is:" << endl;
	lcs(strlen(a3), strlen(a4, c, y);
	
	//对序列a1、a4求最长公共子序列并构造结果 
	lcsLength(a1, a4, x, y);
	cout << "The longest common substring of A and D is:" << endl;
	lcs(strlen(a1), strlen(a4), a, y);
	
	//end 
	fclose(fileOne);
	system("PAUSE");
	return 0;
}

void lcsLength(char x[], char y[], int c[][600], int b[][600]){
	//先求x和y的长度 
	int m = strlen(x);
	int n = strlen(y) - 1;
	//定义临时变量 
	int i = 0, j = 0;
	
	//当i=0或j=0时，c[i][j]=0
	for (i = 0; i <= m; i++)
		c[i][0] = 0;
	for (i = 0; i <= n; i++)
		c[0][i] = 0;
	
	for (i = 1; i <= m; i++)
		for (j = 1; j <= n; j++){
			//当x[i]=y[j]时，c[i][j] = c[i - 1][j - 1] + 1，对应第一种情况 
			if (x[i-1] == y[j-1]){
				c[i][j] = c[i - 1][j - 1] + 1;
				b[i][j] = 1;
			}
			//当x[i]!=y[j]时，c[i][j]=max(c[i-1][j], c[i][j-1])，分别对应第二种情况和第三种情况 
			else if (c[i - 1][j] >= c[i][j - 1]){
				c[i][j] = c[i - 1][j];
				b[i][j] = 2;
			}
			else{
				c[i][j] = c[i][j - 1];
				b[i][j] = 3;
			}
		}
}

void lcs(int i, int j, char x[], int b[][600]){ //根据b[][]来构造最长子序列 
	if (i == 0 || j == 0)
		return;
	if (b[i][j] == 1){ //第一种情况 ，x[i]=y[j]，Zk = Zk-1 + z[k]
		lcs(i - 1, j - 1, x, b);
		cout << x[i-1];
	}
	else if (b[i][j] == 2){ //第二种情况， x[i]!=y[j]，在X[i-1]和Y[j]中继续构造 
		lcs(i - 1, j, x, b);
	}
	else{ //第三种情况， x[i]!=y[j]，在X[i]和Y[j-1]中继续构造
		lcs(i, j - 1, x, b);
	}
}
