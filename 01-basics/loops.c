#include <stdio.h>

int main() {
  for (int i = 0; i <= 10; i++) {
    printf("i is = %d\n", i);
  }

  int x = 10;
  while (x > 0) {
    printf("x is = %d\n", x);
    x -= 3;
  }
  return 0;
}
