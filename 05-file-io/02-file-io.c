#include <stdio.h>

int main() {
  int arr[5] = {10, 20, 30, 40, 50};
  FILE *f = fopen("numbers.bin", "wb"); 
  fwrite(arr, sizeof(int), 5, f);
  fclose(f);

  f = fopen("numbers.bin", "rb");

  int result[5];

  fread(result, sizeof(int), 5, f);

  for (int i = 0; i < 5; i++) {
    printf("%d\n", result[i]);
  }
  fclose(f);

  return 0;
}
