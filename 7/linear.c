/*
 * linear.c: 線形走査法の実装
 */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "hash.h"

/* 定数の定義 */
const int d = 5;		/* ハッシュ増分 */

/*
 * linear_store: キーをハッシュ表に登録する
 */
void linear_store(char *key)
{
  int h;

  if (vflag)
    printf("insertion of \"%s\"\n", key);

  h = (*hashfun)(key, M);	/* ハッシュ番地 (h_0) を計算 */

  while (htable[h] != NULL) {	/* 衝突: すでにキーが格納されていた */
    if (vflag)
      printf("  h = %d: collision with \"%s\"\n", h, htable[h]);
    h = (h + d) % M;		/* 次のハッシュ番地を計算 */
  }

  /* ハッシュ番地 h に key を格納のため文字列の領域を割当て */
  htable[h] = (char *)malloc(strlen(key) + 1);
  strcpy(htable[h], key);	/* key をハッシュ表の該当番地にコピー */

  if (vflag)
    printf("  h = %d: insertion is done\n", h);
}

/*
 * linear_search: 与えられたキーについてハッシュ表を探索する
 */
Sstat linear_search(char *key)
{
  Sstat stat;			/* 探索結果 */
  int h, i;

  h = (*hashfun)(key, M);	/* ハッシュ番地を計算 */
  stat.colcount = 0;		/* 衝突回数を初期化 */
  stat.found = -1;		/* 返り値の初期値 (-1) を設定 */

  for (i = 0; i < M; i++)
    if (htable[h] == NULL)	/* 探索は不成功だった */
      return stat;
    else if (strcmp(htable[h], key) == 0) { /* キーが見つかった */
      if (vflag)
        printf("h = %d: found\n", h);
      stat.found = h;
      return stat;
    } else {			/* 衝突: ハッシュ番地には別のキーが存在 */
      if (vflag)
        printf("h = %d: collision with \"%s\"\n", h, htable[h]);
      stat.colcount++;
      h = (h + d) % M;		/* 次のハッシュ番地を計算 */
    }

  /* ここに到達するのは以下の二つの条件がともに成立するとき
   *   1. ハッシュ表が満杯
   *   2. ハッシュ表中に与えられたキーが存在しない */
  return stat;
}

/*
 * linear_failsearch_len: ハッシュ番地 h から開始した場合の
 * 不成功探索の探査長（探査回数）を返す．
 */
unsigned linear_failsearch_len(int h)
{
  unsigned len = 0;
  int i;

  for (i = 0; i < M; i++) {
    len++;
    if (htable[h] == NULL)
      return len;
    else
      h = (h + d) % M;		/* 次のハッシュ番地を計算 */
  }
}
