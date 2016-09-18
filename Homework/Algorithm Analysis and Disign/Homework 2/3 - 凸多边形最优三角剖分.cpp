#include <iostream>
#include <cstdlib>

#define MAX 601

using namespace std;

void knapsack(int v[], int w[], int c, int n);//动态规划计算0-1背包问题的最优值 
void traceback(int w[], int c, int x[], int n); //在得到最优值的情况下反向构造最优解 

int m[MAX][MAX];//m[i][j]包括了0-1背包问题的最优值，其中m[i][j]指可选物品为i,i+1,...,n，背包容量还剩j时的0-1背包问题的最优值
//那么m[1][c]就是整个0-1背包问题的最优值 

int main(void){
	//打开文件并将数据读取到w1[],w2[],v1[],v2[]中，其中wi[]是重量，vi[]是与重量对应的价值 
	FILE * fileFour;
	fileFour = fopen("4.txt", "r");
	int n1 = 50, n2 = 100, i = 0; //n是物品个数，i是临时变量 
	int w1[51], w2[101], v1[51], v2[101], x1[51], x2[101];
	int c1, c2;  //c是最大容量 
	fscanf(fileFour, "%d", &c1);
	for (i = 1; i <= n1; i++)
		fscanf(fileFour, "%d", &w1[i]);
	for (i = 1; i <= n1; i++)
		fscanf(fileFour, "%d", &v1[i]);
	fscanf(fileFour, "%d", &c2);
	for (i = 1; i <= n2; i++)
		fscanf(fileFour, "%d", &w2[i]);
	for (i = 1; i <= n2; i++)
		fscanf(fileFour, "%d", &v2[i]);

	//计算第一个背包的解（50个物品，最大容量为300） 
	knapsack(v1, w1, c1, n1); //计算最优值 
	traceback(w1, c1, x1, n1); //根绝最优值构建最优解 
	cout << "The best value of the pack A is " << m[1][c1] << endl;//输出结果 
	cout << "The items are:" << endl; 
	int weight = 0, value = 0;
	for (int i = 1; i <= n1; i++)
		if (x1[i] == 1){
			cout << "Item " << i << " Weight: " << w1[i] << " Value: " << v1[i] << endl;
			weight += w1[i];
			value += v1[i];
		}
	cout << "The total weight is " << weight << ' ' << value << endl << endl;

	//计算第二个背包的解（100个物品，最大容量600） 
	knapsack(v2, w2, c2, n2);
	traceback(w2, c2, x2, n2);
	cout << "The best value of the pack B is " << m[1][c2] << endl;
	cout << "The items are:" << endl;
	weight = 0;
	value = 0;
	for (int i = 1; i <= n2; i++)
		if (x2[i] == 1){
			cout << "Item " << i << " Weight: " << w2[i] << " Value: " << v2[i] << endl;
			weight += w2[i];
			value += v2[i];
		}
	cout << "The total weight is " << weight << ' ' << value << endl;
	system("PAUSE");
	return 0;
}

void knapsack(int v[], int w[], int c, int n){
	//根据v[]和w[]来计算0-1背包问题的最优值，v[]是价值，w[]是重量，c是最大容量，n是物品个数 
	n++; 
	int jMax = (w[n] - 1 > c) ? c : (w[n] - 1); //jMax=min(w[n]-1,c)，即jmax=第n个物品重量-1和最大容量二者中的较小值 
	
	for (int j = 0; j <= jMax; j++) //令m[n][0],m[n][1],...,m[n][jMax]=0,即仅剩第n个物品，容量还剩0,1,...,jMax时，不能将物品n放入 
		m[n][j] = 0;
	for (int j = w[n - 1]; j <= c; j++) //令m[n][w[n-1]],m[n][w[n]],...,m[n][c]=0,即仅剩第n个物品，容量还剩w[n-1],w[n],...,c时，可以将物品n放入，且这个子背包的价值为v 
		m[n][j] = v[n];

	for (int i = n - 1; i > 1; i--){
		jMax = (w[i] - 1 > c) ? c : (w[i] - 1); //jMax=min(w[i]-1,c) 
		for (int j = 0; j <= jMax; j++) //令m[i][0],m[i][1],...,m[i][jMax]=m[i+1][j],即对于第i个物品，容量还剩0,1,...,jMax时，不能将物品n放入，子背包的价值没加入第i个物品时是相同的 
			m[i][j] = m[i + 1][j];
		for (int j = w[i]; j <= c; j++) //对于m[i][w[i]],m[i][w[i]+1],...,m[i][c]来说，m[i][j]= max( m[i + 1][j] , m[i + 1][j - w[i]] + v[i] ) 
			m[i][j] = (m[i + 1][j] > (m[i + 1][j - w[i]] + v[i])) ? m[i + 1][j] : (m[i + 1][j - w[i]] + v[i]);
	}
	m[1][c] = m[2][c];
	if (c >= w[1]) //如果c大于w[1]的话， m[1][c]= max( m[1][c] , m[2][c - w[1]] + v[1] )
		m[1][c] = (m[1][c] > (m[2][c - w[1]] + v[1])) ? m[1][c] : (m[2][c - w[1]] + v[1]);
}

void traceback(int w[], int c, int x[], int n){ 
	//根据m[][]来反向构造0-1背包问题的最优解，x[i]用以存储物品i是否被放入 
	for (int i = 1; i < n; i++){ 
		if (m[i][c] == m[i + 1][c]) //如果m[i][c]=m[i-1][c]，说明这个物品没被放进去，则x[i]=0 
			x[i] = 0;
		else{ //否则x[i]=1，且由m[i+1][ c-w[i] ]来继续构造最优解 
			x[i] = 1;
			c -= w[i];
		}
	}
	x[n] = (m[n][c] > 0) ? 1 : 0; //如果m[n]c[c]>0说明第n个物品被放入，防止c为负数造成数组访问越界 
}


