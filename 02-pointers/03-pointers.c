#include <stdio.h>

int main () {
  int arr[4] = {10, 20, 30, 40};
  
  printf("arr[0]    = %d\n", arr[0]);
  printf("*arr      = %d\n", *arr);

  int *p = arr;
  
  printf("p[1]      = %d\n", p[1]);
  printf("*(p+2)    = %d\n", *(p+2));

  return 0;
}
