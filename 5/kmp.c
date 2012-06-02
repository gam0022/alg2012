/* 単純照合のプログラム simple.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int Ncomp = 0; /* 比較回数をカウントする大域変数 */
int *next;
#define TRACE

int getFileSize(char *fname)
{
	int fd, sz;

	if ((fd = open(fname, O_RDONLY)) == -1) {
		fprintf(stderr, "open");
		exit(1);
	}
	if ((sz = lseek(fd, 0, SEEK_END)) == -1) {
		fprintf(stderr, "lseek");
		exit(1);
	}
	close(fd);

	return sz;
}

char *patget(char *s)
{
	char *pat;

	if (( pat = calloc(strlen(s) + 1, sizeof(char)) ) == NULL) {
		fprintf(stderr, "calloc");
		exit(1);
	}
	strcpy(pat, s);

	if ((next = calloc(strlen(s)+1, sizeof(int))) == NULL) {
		perror("calloc");
		exit(1);
	}

	return pat;
}

char *txtget(char *fname)
{
	int cnt = 0, n;
	char buf[BUFSIZ];
	char *str;
	FILE *fp;

	if ((str = calloc(getFileSize(fname) + 1, sizeof(char))) == NULL) {
		fprintf(stderr, "calloc");
		exit(1);
	}

	if ((fp = fopen(fname, "r")) == NULL) {
		fprintf(stderr, "fopen");
		exit(1);
	}

	for (cnt = 0; (n = fread(buf, sizeof(char), sizeof(buf), fp)) != 0; cnt += n)
		memcpy(&str[cnt], buf, n);

	fclose(fp);

	return str;
}

/* 文字の比較と比較回数の収集を行う*/
int compare(char a, char b)
{
#ifdef TRACE
	printf("compare(%c, %c)\n", a, b);
#endif
	Ncomp++; /* 比較回数を1増やす */
	return a == b;
}

// nextを求める

void compnxt(char *pat, int plen)
{
	int i = 0, s = -1;
	next[0] = -1;
	while(i < plen) {
		while(s>-1 && pat[i] != pat[s])
			s = next[s];

		if(pat[++i] == pat[++s])
			next[i] = next[s];
		else
			next[i] = s;
	}
}

/* KNP照合 */
int kmp_search(char *text, int tlen, char *pat, int plen)
{
	int i=0, j=0;// i:patのindex, j:textのindex
	while(j < tlen) {
		printf("i:%d j:%d\n", i, j);
		while(i > -1 && !compare(pat[i], text[j]))
			i = next[i];

		if(i == plen-1) return j - plen + 1;
		else {++i; ++j;}
	}
	return -1;
}

int main(int argc, char *argv[])
{
	char *text; /* テキストの文字配列 */
	char *pat; /* パターンの文字配列 */
	int tlen; /* テキスト長（文字数） */
	int plen; /* パターン長（文字数） */
	int pos; /* 照合した位置 */

	/* 引数をチェック */
	if (argc != 3) {
		fprintf(stderr, "wrong no. of arguments\n");
		fprintf(stderr, "usage: %s pat fname\n", argv[0]);
		exit(1);
	}

	pat = patget(argv[1]);
	plen = strlen(pat); /* パターンの文字数を求める */

	/* 第2引数で指定されたファイル名のファイルを読み込む */
	text = txtget(argv[2]);
	tlen = strlen(text); /* テキストの文字数を求める */

	/* KMP照合の関数を呼び出す */
	compnxt(pat, plen);

	int i;
	for(i=0; i<plen; i++)
		printf("%d ", next[i]);
	printf("\n");

	/* 単純照合の関数を呼び出す */
	pos = kmp_search(text, tlen, pat, plen);

	if (pos >= 0)
		printf("Found at %d\n", pos);
	else
		printf("Not found\n");
	printf("no. of comparisons = %d\n", Ncomp);

	free(pat); /* patのメモリ領域を解放 */
	free(text); /* textのメモリ領域を解放 */
	free(next);

	return 0;
}
