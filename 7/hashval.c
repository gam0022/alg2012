/*
 * hashval.c: 入力中の各キー（文字列）に対するハッシュ値を出力する
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
  FILE *fp;

  if (argc != 4) {
    fprintf(stderr, "usage: hashval hashfun input_file tbl_size\n");
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
  while(fscanf(fp, "%s", key) != EOF)
    /* キー値に対しハッシュ関数を呼出し出力する */
    printf("%d %s\n", (*hashfun)(key, M), key);
  fclose(fp);
}
