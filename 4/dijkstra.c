/*********************************************************
 *                      dijkstra.c                       *
 *********************************************************/

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
Weight d[N] = {M};  /* 重みの累積値を格納する行列 */
int from[N] = {0}; // どの頂点から辿られたのか格納する行列

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

void add(int p)
{
	S[p] = TRUE;
	++Scount;
}

Bool remain(void)
{
	return (Scount < N);
}

int select_min(void)
{
	int i, p, min = M;
	for(i=0; i<N; ++i) if (!S[i]) {
		if(less(d[i], min)) {
			min = d[i];
			p = i;
		}
	}
	return p;
}

Bool member(int u, int x)
{
	return (w[u][x] != M);
}

void Dijkstra(int p)
{
	int i, u, x, k;

	add(p);
	for (i = 0; i < N; i++) {
		d[i] = w[p][i];
		from[i] = p;
	}
	while (remain()) {
		u = select_min();
		add(u);
		for (x = 0; x < N; x++)
			if (member(u, x)) {
				k = append(d[u], w[u][x]);
				if (less(k, d[x])) {
					d[x] = k;
					from[x] = u;
				}
			}
	}
}

// 2頂点mからnの最短経路を再帰的に表示する
void show_path(int p, int i)
{
	if(p == i) return;
	show_path(p, from[i]);
	printf("%d => ", from[i]);
}

int main(int argc, char *argv[])
{
	if(argc != 2) {
		fprintf(stderr, "Usage: %s nodeid\n", argv[0]);
		exit(1);
	}

	int p = atoi(argv[1]);
	Dijkstra(p);

	int i,x;
	for(i=0; i<N; ++i)
		if(d[i] == M) {
			printf("there is no path! (%d => %d)\n", p, i);
		} else {
			show_path(p, i);
			printf("%d (%d)\n", i, d[i]);
		}

	return 0;
}
