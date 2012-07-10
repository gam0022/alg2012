/*
 * hashhist.c: ハッシュ番地のヒストグラムを生成する
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashfun.h"

main(int argc, char *argv[])
{
  int (*hashfun)(char *, int);	/* ハッシュ関数 */
  int M;			/* ハッシュ表のサイズ */
  char key[256];		/* キーの値 */
  unsigned *hist;		/* ヒストグラム */
  FILE *fp;
  int i; 

  if (argc != 4) {
    fprintf(stderr, "usage: hashhist hashfun input_file tbl_size\n");
    exit(1);
  }

  /* ハッシュ関数の設定 */
  if (strcmp(argv[1], "strhash") == 0)
    hashfun = strhash;
  else if (strcmp(argv[1], "strhash4") == 0)
    hashfun = strhash4;
  else if (strcmp(argv[1], "hashpjw") == 0)
    hashfun = hashpjw;
  else {
    fprintf(stderr, "unknown hash function %s\n", argv[1]);
    exit(1);
  }

  M = atoi(argv[3]);		/* ハッシュ表のサイズを設定 */

  if ((fp = fopen(argv[2], "r")) == NULL) {
    fprintf(stderr, "cannot find file %s\n", argv[2]);
    exit(1);
  }
  /* ヒストグラムのメモリ領域を割当て, 初期化 */
  hist = (unsigned *)malloc((sizeof(unsigned)) * M);
  for (i = 0; i < M; i++)
    hist[i] = 0;

  while(fscanf(fp, "%s", key) != EOF)
    hist[(*hashfun)(key, M)]++;	/* ヒストグラムの更新 */
  fclose(fp);

  /* ヒストグラムの出力 */
  for (i = 0; i < M; i++)
    printf("%d %d\n", i, hist[i]);

  free(hist);			/* ハッシュ表の領域の解放 */
}
