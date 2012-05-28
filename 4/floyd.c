#include <stdio.h>
#include <stdlib.h>

typedef enum {FALSE, TRUE} Bool;
typedef int Weight;

#define M (-1)		/* 無限大の重みを表す */

/* 対象のグラフの定義 */
#define N 6
int w[N][N] = 
	{{ 0, M,  M, 8, 15, M},
	{ 10, 0, 24, M,  8, M},
	{  M, M,  0, M,  M, 6},
	{  M, M,  M, 0,  5, M},
	{  M, M, 12, M,  0, 7},
	{  M, M,  3, M,  M, 0}};

Bool S[N] = {FALSE};		/* 処理済みの頂点の集合 S */
int Scount=0;	  /* 集合 S の要素数 */
Weight d[N][N];  /* 重みの累積値を格納する行列 */
int p[N][N];// どの頂点から辿られたのか格納する行列

// Mを考慮したWeightの比較
Bool less(Weight a, Weight b)
{
	return ( a != M && ( a < b || b == M ) );
}

// Mを考慮したWeightの加算
Weight append(Weight a, Weight b)
{
	return (a == M || b == M) ? M : a+b;
}

void Floyd()
{
	int i,j,k,can;
	for(i=0; i<N; ++i)
		for(j=0; j<N; ++j) {
			d[i][j] = w[i][j];
			p[i][j] = i;
		}
	for(k=0; k<N; ++k)
		for(i=0; i<N; ++i)
			for(j=0; j<N; ++j) {
				can = append(d[i][k], d[k][j]);
				if(less(can, d[i][j])) {
					d[i][j] = can;
					p[i][j] = p[k][j];
				}
			}
}

// 2頂点mからnの最短経路を再帰的に表示する
void show_path(int m, int n)
{
	if(m == n) return;
	show_path(m, p[m][n]);
	printf("%d => ", p[m][n]);
}

// 2頂点mからnの最短距離とその経路を表示する
void shortest_path(int m, int n)
{
	if (d[m][n] == M)
		printf("there is no path! (%d => %d)\n", m, n);
	else {
		show_path(m,n);
		printf("%d (%d)\n", n, d[m][n]);
	}
}

int main()
{
	Floyd();

	int i,j;
	for(i=0; i<N; ++i)
		for(j=0; j<N; ++j)
			shortest_path(i,j);
	return 0;
}
