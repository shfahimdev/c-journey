#include <stdio.h>

int add(int a, int b) {
  return a + b;
}

int multiply(int a, int b) {
  return a * b;
}

void greet(char a[]) {
  printf("%s \n", a);
}

int main () {
  greet("Fahim");
  printf("Sum is %d\n", add(10, 40));
  printf("Product is %d\n", multiply(5, 6));

  return 0;
}
