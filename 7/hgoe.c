#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash.h"
#include "hashfun.h"

/* $BBg0hJQ?t$NDj5A(B */
char **htable;			/* $B%O%C%7%eI=(B */
int (*hashfun)(char *, int);	/* $B%O%C%7%e4X?t(B */
int M;				/* $B%O%C%7%eI=$N%5%$%:(B */
int vflag;			/* $B>\:Y%b!<%I$N%U%i%0(B */

/* $B@~7AAv::K!$N4X?t$N@k8@(B ($BK\BN$O(B linear.c $B$K$"$k(B) */
void linear_store(char *);
Sstat linear_search(char *);
unsigned linear_failsearch_len(int);

/* $BFs<!Av::K!$N4X?t$N@k8@(B ($BK\BN$O(B quadratic.c $B$K$"$k(B) */
void quadratic_store(char *);
Sstat quadratic_search(char *);
unsigned quadratic_failsearch_len(int);

/*
 * hashtable_search: $B%f!<%6$K%-!<$rF~NO$7$F$b$i$$!$%-!<$,%O%C%7%eI=(B
 * $B$NCf$K8+$D$+$C$?$+$I$&$+$rJs9p$9$k!%%f!<%6$,(B Ctrl-D $B$rF~NO$9$k$^(B
 * $B$G7+$jJV$7(B, Ctrl-D $B$,F~NO$5$l$?$i=*N;$9$k!%(B
 */
void hashtable_search(Sstat (*search)(char *))
{
  Sstat stat;
  char key[256];

  printf("input key: ");
  while (1) {
    if (scanf("%s", key) == EOF) /* $BF~NO$,=*N;(B (Ctrl-D$B$NF~NO(B) */
      break;			 /* while $B%k!<%W$+$iH4$1=P$9(B         */

    stat = (*search)(key); /* $B%O%C%7%eI=$rC5:w(B */
    if (stat.found >= 0) 
      printf("*found*\n");
    else
      printf("*not found*\n");

    /* $B>WFM2s?t$r=PNO(B */
    printf("no. of collisions: %d\n", stat.colcount);

    /* $BC5::2s?t$r=PNO(B */
    if (stat.found >= 0)
      printf("no. of probes: %d\n", stat.colcount + 1);
    else
      printf("no. of probes: %d\n", stat.colcount);

    printf("input key: ");
  }
}

/*
 * success_test: $B@.8y;~$NJ?6QC5::2s?t$r5a$a$k(B. $B%G!<%?A^F~;~$K;H$C$?(B
 * $BF~NO%U%!%$%k(B (fname) $BCf$N3F%-!<$K$D$$$FC5::$r9T$$(B, $B>WFM2s?t$N=87W(B
 * $B$r$H$k(B. $B:G8e$K$=$l$i$rJ?6Q$7$FE}7W>pJs$r=PNO$9$k!%(B
 */
void success_test(char *fname, Sstat (*search)(char *), int keycount)
{
  Sstat stat;
  unsigned long ncol = 0;	/* $B>WFM$N2s?t(B */
  char key[256];
  FILE *fp;

  fp = fopen(fname, "r");
  while (fscanf(fp, "%s", key) != EOF) {
    stat = (*search)(key);
    if (stat.found < 0) {
      /* $B%W%m%0%i%`$,@5$7$1$l$P$3$N>u67$O5/$3$i$J$$$O$:(B */
      fprintf(stderr, "unexpected error\n");
      exit(1);
    }
    ncol += stat.colcount;
  }
  fclose(fp);

  /* $B%-!<$NAm?t$r%O%C%7%eI=$N%5%$%:(B M $B$G3d$k$HI=@jM-N($,F@$i$l$k(B */
  printf("load factor(alpha) = %1.3f\n", (float)keycount / M);

  /* $BJ?6Q>WFM2s?t$NI=<((B */
  printf("no. collisions = %d (avg: %1.3f)\n",
	 ncol, (float)ncol / keycount);

  /* $BJ?6QC5::2s?t$NI=<((B */
  printf("no. of probes = %d (avg: %1.3f)\n", ncol + keycount,
	 (float)ncol / keycount + 1.0);
}

/*
 * failure_test: $B<:GT;~$NJ?6QC5::2s?t$r5a$a$k(B. $B3F%O%C%7%eHVCO$+$i(B
 * $BIT@.8yC5::$r9T$$(B, $BC5::2s?t$N=87W$r$H$k(B. $B:G8e$K$=$l$i$rJ?6Q$7$F(B
 * $BE}7W>pJs$r=PNO$9$k!%(B
 */
void failure_test(unsigned (*failsearch_len)(int), int keycount)
{
  int h;
  unsigned long nprobes;

  nprobes = 0;
  for (h = 0; h < M; h++)	/* $B3F%O%C%7%eHVCO$K$D$$$F(B */
    nprobes += (*failsearch_len)(h);

  /* $B%-!<$NAm?t$r%O%C%7%eI=$N%5%$%:(B M $B$G3d$k$HI=@jM-N(&A$,F@$i$l$k(B */
  printf("load factor(alpha) = %1.3f\n", (float)keycount / M);

  printf("no. of probes = %d (avg = %1.3f)\n", 
	 nprobes, (float)nprobes / M);
}

main(int argc, char *argv[])
{
  void (*store)(char *);	/* $B%O%C%7%eI=EPO?4X?t(B */
  Sstat (*search)(char *);	/* $B%O%C%7%eMQC5:w4X?t(B */
  unsigned (*failsearch_len)(int); /* $BIT@.8yC5:wD9$rJV$94X?t(B */
  char key[256];		/* $B%-!<(B ($BJ8;zNs(B) $B$NCM(B */
  int keycount = 0;		/* $B%-!<$NAm?t(B */
  char usage[] = "usage: hashtest [-v] method hashfun input_file tbl_size";
  FILE *fp;
  int m, i; 

  if (argc == 1) {
    /* $B0z?t$,M?$($i$l$J$+$C$?>l9g(B: $B%X%k%W$r=PNO$7=*N;(B */
    fprintf(stderr, "%s\n", usage);
    exit(1);
  }

  if (strcmp(argv[1], "-v") == 0) {
    /* $B>\:Y%b!<%I$,;XDj$5$l$?>l9g(B */
    vflag = 1;
    argc--;			/* $B0z?t$N?t$r(B1$B8:$i$9(B */
    argv++;			/* $B0z?t$X$N%]%$%s%?$r(B1$B$D?J$a$k(B */
  } else
    vflag = 0;

  if (argc != 5) {		/* $B0z?t$N?t$N%A%'%C%/(B */
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

  M = atoi(argv[4]);		/* $B%O%C%7%eI=$N%5%$%:$r@_Dj(B */

  /* $B%O%C%7%eI=$NNN0h$N3dEv$F(B */
  htable = malloc((sizeof(char *)) * M);
  for (i = 0; i < M; i++)
    htable[i] = NULL;

  if ((fp = fopen(argv[3], "r")) == NULL) {
    fprintf(stderr, "cannot find file %s\n", argv[3]);
    exit(1);
  }
  keycount = 0;
  while (fscanf(fp, "%s", key) != EOF) {
    (*store)(key);		/* $B3F%-!<CM$r%O%C%7%eI=$KEPO?(B */
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

  /* $B%a%b%j$N2rJ|(B */
  for (i = 0; i < M; i++)
    if (htable[i] != NULL)
      free(htable[i]);
  free(htable);
}
