#include "../include/functional.h"
#include <stdio.h>

int addition(const int a, const int b)
{
    return a + b;
}
int substraction(const int a, const int b)
{
    return a - b;
}
int multiplication(const int a, const int b)
{
    return a * b;
}
double division(const int a, const int b)
{
    if (b == 0) {
        printf("Zero delition");
        return 0;
    }
    return a / b;
}