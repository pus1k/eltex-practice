#include "substraction.h"
#include <stdio.h>

double substraction(const double a, const double b)
{
	return a - b;
}
void make(const double a, const double b)
{
	printf("\n%.3lf - %.3lf = %.3lf\n\n", a, b, substraction(a, b));
}
const char* ui(void)
{
	return "Substraction";
}