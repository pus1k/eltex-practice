#include "division.h"
#include <stdio.h>

double division(const double a, const double b)
{
	if (b == 0)
	{
		printf("Zero delition");
		return 0;
	}
	return a / b;
}
void make(const double a, const double b)
{
	printf("\n%.3lf / %.3lf = %.3lf\n\n", a, b, division(a, b));
}
const char* ui(void)
{
	return "Division";
}