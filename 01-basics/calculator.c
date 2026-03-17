#include <stdio.h>

int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }
int multiply(int a, int b) { return a * b; }
float divide(float a, float b) { return a / b; }
int power(int a, int b) {
    int result = 1;
    for (int i = 1; i <= b; i++) {
        result = result * a;
    }
    return result;
}

int main() {
    int a = 5, b = 3;

    printf("add         :%d\n", add(a, b));
    printf("subtract   :%d\n", subtract(a, b));
    printf("multiply    :%d\n", multiply(a, b));
    printf("divide      :%.2f\n", divide(a, b));
    printf("power      :%d\n", power(a, b));

    return 0;
}
