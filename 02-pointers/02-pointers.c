#include <stdio.h>

void double_it(int *n) {
  *n = *n * 2;
}

int main () {
  int x = 5;
  double_it(&x);
  printf("x = %d\n", x);
  return 0;
}
