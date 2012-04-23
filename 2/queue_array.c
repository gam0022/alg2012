#include "stdio.h"
#include "stdlib.h"

#define M 4 /* 最大３個の要素が入る待ち行列*/
int x[M]; /* 待ち行列用の配列の宣言 (11/04/25更新: 余計な-1がついていました) */
int f,r; // 先頭のデータの添字f, 次に新規のデータを入れるべき添字r

void init_queue(void) /* 待ち行列の初期化 */
{
	f = r = 0;
}

void enqueue(int y) /* 配列を使ったenqueue処理の実装 */
{
	if (f-r == 1 || f-r+M == 1) {
		perror("Overflow\n");
		exit(1);
	} else {
		x[r] = y;
		if(++r == M) r = 0;
		printf("enqueue: %d\n", y);
	}
}

int dequeue(void) /* 配列を使ったdequeue処理の実装 */
{
	if (f == r) {
		perror("Underflow\n");
		exit(1);
	} else {
		int y = x[f++];
		if (f == M) f = 0;
		printf("dequeue: %d\n", y);
		return y;
	}
}

void print_queue(void) /* 待ち行列の内容の表示 */
{
	int i;
	printf("queue contains:");
	for(i = f; i != r; i = ++i % M)
		printf(" %d", x[i]);

	printf("\n");
}

main(void)
{
	int n;

	init_queue();

	enqueue(1);
	print_queue();

	enqueue(2);
	print_queue();

	enqueue(3);
	print_queue();

	n = dequeue();
	print_queue();

	enqueue(4);
	print_queue();
}

