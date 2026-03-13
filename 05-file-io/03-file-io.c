#include <stdio.h>

int main() {
  FILE *f = fopen("ghost", "r"); 
  if (f == NULL) {
    perror("fopen");
    return 1;
  }

  char buffer[128];
  size_t n = fread(buffer, 1, 128, f);
  printf("read %zu bytes\n", n);
  fclose(f);


  return 0;
}
