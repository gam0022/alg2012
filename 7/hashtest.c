#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash.h"
#include "hashfun.h"

/* 大域変数の定義 */
char **htable;			/* ハッシュ表 */
int (*hashfun)(char *, int);	/* ハッシュ関数 */
int M;				/* ハッシュ表のサイズ */
int vflag;			/* 詳細モードのフラグ */

/* 線形走査法の関数の宣言 (本体は linear.c にある) */
void linear_store(char *);
Sstat linear_search(char *);
unsigned linear_failsearch_len(int);

/* 二次走査法の関数の宣言 (本体は quadratic.c にある) */
void quadratic_store(char *);
Sstat quadratic_search(char *);
unsigned quadratic_failsearch_len(int);

/*
 * hashtable_search: ユーザにキーを入力してもらい，キーがハッシュ表
 * の中に見つかったかどうかを報告する．ユーザが Ctrl-D を入力するま
 * で繰り返し, Ctrl-D が入力されたら終了する．
 */
void hashtable_search(Sstat (*search)(char *))
{
  Sstat stat;
  char key[256];

  printf("input key: ");
  while (1) {
    if (scanf("%s", key) == EOF) /* 入力が終了 (Ctrl-Dの入力) */
      break;			 /* while ループから抜け出す         */

    stat = (*search)(key); /* ハッシュ表を探索 */
    if (stat.found >= 0) 
      printf("*found*\n");
    else
      printf("*not found*\n");

    /* 衝突回数を出力 */
    printf("no. of collisions: %d\n", stat.colcount);

    /* 探査回数を出力 */
    if (stat.found >= 0)
      printf("no. of probes: %d\n", stat.colcount + 1);
    else
      printf("no. of probes: %d\n", stat.colcount);

    printf("input key: ");
  }
}

/*
 * success_test: 成功時の平均探査回数を求める. データ挿入時に使った
 * 入力ファイル (fname) 中の各キーについて探査を行い, 衝突回数の集計
 * をとる. 最後にそれらを平均して統計情報を出力する．
 */
void success_test(char *fname, Sstat (*search)(char *), int keycount)
{
  Sstat stat;
  unsigned long ncol = 0;	/* 衝突の回数 */
  char key[256];
  FILE *fp;

  fp = fopen(fname, "r");
  while (fscanf(fp, "%s", key) != EOF) {
    stat = (*search)(key);
    if (stat.found < 0) {
      /* プログラムが正しければこの状況は起こらないはず */
      fprintf(stderr, "unexpected error\n");
      exit(1);
    }
    ncol += stat.colcount;
  }
  fclose(fp);

  /* キーの総数をハッシュ表のサイズ M で割ると表占有率が得られる */
  printf("load factor(alpha) = %1.3f\n", (float)keycount / M);

  /* 平均衝突回数の表示 */
  printf("no. collisions = %d (avg: %1.3f)\n",
	 ncol, (float)ncol / keycount);

  /* 平均探査回数の表示 */
  printf("no. of probes = %d (avg: %1.3f)\n", ncol + keycount,
	 (float)ncol / keycount + 1.0);
}

/*
 * failure_test: 失敗時の平均探査回数を求める. 各ハッシュ番地から
 * 不成功探査を行い, 探査回数の集計をとる. 最後にそれらを平均して
 * 統計情報を出力する．
 */
void failure_test(unsigned (*failsearch_len)(int), int keycount)
{
  int h;
  unsigned long nprobes;

  nprobes = 0;
  for (h = 0; h < M; h++)	/* 各ハッシュ番地について */
    nprobes += (*failsearch_len)(h);

  /* キーの総数をハッシュ表のサイズ M で割ると表占有率αが得られる */
  printf("load factor(alpha) = %1.3f\n", (float)keycount / M);

  printf("no. of probes = %d (avg = %1.3f)\n", 
	 nprobes, (float)nprobes / M);
}

main(int argc, char *argv[])
{
  void (*store)(char *);	/* ハッシュ表登録関数 */
  Sstat (*search)(char *);	/* ハッシュ用探索関数 */
  unsigned (*failsearch_len)(int); /* 不成功探索長を返す関数 */
  char key[256];		/* キー (文字列) の値 */
  int keycount = 0;		/* キーの総数 */
  char usage[] = "usage: hashtest [-v] method hashfun input_file tbl_size";
  FILE *fp;
  int m, i; 

  if (argc == 1) {
    /* 引数が与えられなかった場合: ヘルプを出力し終了 */
    fprintf(stderr, "%s\n", usage);
    exit(1);
  }

  if (strcmp(argv[1], "-v") == 0) {
    /* 詳細モードが指定された場合 */
    vflag = 1;
    argc--;			/* 引数の数を1減らす */
    argv++;			/* 引数へのポインタを1つ進める */
  } else
    vflag = 0;

  if (argc != 5) {		/* 引数の数のチェック */
    fprintf(stderr, "%s\n", usage);
    exit(1);
  }

  if (strcmp(argv[1], "linear") == 0) {
    store = linear_store;
    search = linear_search;
    failsearch_len = linear_failsearch_len;
  } else if (strcmp(argv[1], "quadratic") == 0) {
    store = quadratic_store;
    search = quadratic_search;
    failsearch_len = quadratic_failsearch_len;
  } else {
    fprintf(stderr, "unknown method %s\n", argv[1]);
    exit(1);
  }

  if (strcmp(argv[2], "strhash") == 0)
    hashfun = strhash;
  else if (strcmp(argv[2], "strhash4") == 0)
    hashfun = strhash4;
  else if (strcmp(argv[2], "hashpjw") == 0)
    hashfun = hashpjw;
  else {
    fprintf(stderr, "unknown hash function %s\n", argv[2]);
    exit(1);
  }

  M = atoi(argv[4]);		/* ハッシュ表のサイズを設定 */

  /* ハッシュ表の領域の割当て */
  htable = malloc((sizeof(char *)) * M);
  for (i = 0; i < M; i++)
    htable[i] = NULL;

  if ((fp = fopen(argv[3], "r")) == NULL) {
    fprintf(stderr, "cannot find file %s\n", argv[3]);
    exit(1);
  }
  keycount = 0;
  while (fscanf(fp, "%s", key) != EOF) {
    (*store)(key);		/* 各キー値をハッシュ表に登録 */
    keycount++;
  }
  fclose(fp);

  printf("select menu: 1)search, 2)test(success), 3)test(failure), other)quit: "); 
  scanf("%d", &m);
  switch (m) {
  case 1:
    hashtable_search(search);
    break;
  case 2:
    success_test(argv[3], search, keycount);
    break;
  case 3:
    failure_test(failsearch_len, keycount);
    break;
  default:
    break;
  }

  /* メモリの解放 */
  for (i = 0; i < M; i++)
    if (htable[i] != NULL)
      free(htable[i]);
  free(htable);
}
