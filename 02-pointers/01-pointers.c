#include <stdio.h>

int main() {
  int x = 40;
  int *p = &x;

  printf("The value of X is    : %d\n", x);
  printf("The address of X is  : %p\n", &x);
  printf("P holds              : %p\n", p);
  printf("value at P           : %d\n", *p);
  return 0;
}
