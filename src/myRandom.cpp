#include "myRandom.h"
#define _CRT_RAND_S

#include <stdlib.h>

int randomTo(int max) {
  unsigned int n = rand();
  return (int)((double)n / RAND_MAX * max);
}
double biunitRandom() {
  unsigned int n = rand();
  return (((double)n) / RAND_MAX) * 2 - 1;
}
double unitRandom() {
  unsigned int n = rand();
  return ((double)n) / RAND_MAX;
}
