/*
 * hashfun.c: ハッシュ関数の定義
 */
#include <limits.h>

/* strhash: 文字列のハッシュ関数 (0 から M - 1 の間の整数を返す) */
int strhash(char *s, int M)
{
  unsigned h;

  for (h = 0; *s != '\0'; s++)
    h = ((h << CHAR_BIT) + *s) % M;
  return (int)h;
}

/* strhash4: 文字列のハッシュ関数 (先頭4文字のみ使用) */
int strhash4(char *s, int M)
{
  unsigned h;
  int count = 0;

  for (h = 0; *s != '\0' && count < 4; s++, count++)
    h = ((h << CHAR_BIT) + *s) % M;
  return (int)h;
}

/* hashpjw: 比較的優れているとされる他のハッシュ関数の例 */
int hashpjw(char *s, int M)
{
  char *p;
  unsigned h = 0, g;

  for (p = s; *p; p++) {
    h = (h << 4) + (*p);
    if (g = h & 0xf0000000) {
      h = h ^ (g >> 24);
      h = h ^ g;
    }
  }
  return h % M;
}  
