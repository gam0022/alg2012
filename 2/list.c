/* list.c */
#include <stdio.h>
#include <stdlib.h>

typedef struct cell { /* セルを表す構造体の型定義 */
	int num; /* 整数値を格納 */
	struct cell *next; /* 次のセルを指す */
} cell;
cell *head, *tail; /* リストの先頭・末尾を指す */

/* エラーチェック付きのmalloc */
cell *ensure(void)
{
	cell *p = (cell *)malloc(sizeof(cell));
	if (p == NULL) {
		printf("Out of memory\n");
		exit(1);
	}
	return p;
}

/* リストの初期化 */
void initlist(void)
{
	cell *p = ensure();
	p->num = 0;
	p->next = NULL;
	head = tail = p;
	return;
}

/* リストの末尾に整数要素 i を付け加える */
void append(int i)
{
	cell *p = ensure();
	p->num = i;
	tail->next = p;
	tail = p;
	tail->next = NULL;
	return;
}

/* 与えられた整数 i を保持しているセルを探しそのアドレスを返す．
	 見つからなければ NULL を返す */
cell *search(int i)
{
	cell *p;
	for(p = head->next; p != NULL; p = p->next) {
		if (p->num == i) return p;
	}
	return NULL;
}

/* 整数 i を含むセルを p で指されるセルの直後に挿入する */
void insert(cell *p, int i)
{
	cell *q;
	for(q = head; q != NULL; q = q->next) {
		if (q == p) {
			cell *r = ensure();
			r->num = i;
			r->next = p->next;
			p->next = r;
			return;
		}
	}
}

/* p で指されるセルをリストから削除する */
void delete(cell *p)
{
	cell *q;
	for(q = head; q->next != NULL; q = q->next) {
		if (q->next == p) {
			q->next = p->next;
			free(p);
			return;
		}
	}
}

/* リストの要素を順に出力する */
void printlist(void)
{
	cell *p;
	for(p = head->next; p != NULL; p = p->next) {
		printf("%d ", p->num);
	}
	printf("\n");
}

/* リストの実装をテストするメイン関数 */
main(void)
{  
	cell *p;

	initlist(); /* リストの初期化 */

	append(2); append(5); append(1); /* 2, 5, 1 を追加 */

	printlist();

	if ((p = search(5)) == NULL) { /* 5 を探す */
		fprintf(stderr, "error: not found\n");
		exit(1);
	}

	/* 5 の直後に 3 を入れる */
	insert(p, 3);
	printlist();

	if ((p = search(2)) == NULL) { /* 2 を探す */
		fprintf(stderr, "error: not found");
		exit(1);
	}

	/* 2 を削除する */
	delete(p);
	printlist();
}
