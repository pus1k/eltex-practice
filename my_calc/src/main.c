#include "../lib/static/include/functional.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_SIZE 10

void operations()
{
	printf("\nEnter operation number:\n");
	printf("1) Addition\n");
	printf("2) Substraction\n");
	printf("3) Multiplication\n");
	printf("4) Division\n");
	printf("5) Exit\n\n");
}
int is_number(const char* input)
{
	for (int i = 0; input[i] != '\0'; i++)
	{
		if (input[i] > 57 || input[i] < 48)
		{
			return 1;
		}
	}
	return 0;
}
int get_action(const char* input)
{
	if (!strcmp(input, "1"))
	{
		return 1;
	}
	else if (!strcmp(input, "2"))
	{
		return 2;
	}
	else if (!strcmp(input, "3"))
	{
		return 3;
	}
	else if (!strcmp(input, "4"))
	{
		return 4;
	}
	else if (!strcmp(input, "5"))
	{
		return 5;
	}
	else if (input[0] == '\0')
	{
		return 0;
	}
	return -1;
}
void get_variables(int* a, int* b)
{
	char input[INPUT_SIZE] = "";
	do
	{
		printf("# a = ");
		fgets(input, INPUT_SIZE, stdin);
		input[strlen(input) - 1] = '\0';
	} while (is_number(input));
	(*a) = atoi(input);

	do
	{
		printf("# b = ");
		fgets(input, INPUT_SIZE, stdin);
		input[strlen(input) - 1] = '\0';
	} while (is_number(input));
	(*b) = atoi(input);
}
int make_action(int action)
{
	int a = 0, b = 0;
	if (action < 5 && action > 0)
	{
		get_variables(&a, &b);
	}
	switch (action)
	{
	case -1:
		printf("\nWrong operation number!\n\n");
		break;
	case 1:
		printf("\n%d + %d = %d\n\n", a, b, addition(a, b));
		break;
	case 2:
		printf("\n%d - %d = %d\n\n", a, b, substraction(a, b));
		break;
	case 3:
		printf("\n%d * %d = %d\n\n", a, b, multiplication(a, b));
		break;
	case 4:
		printf("\n%d / %d = %.3lf\n\n", a, b, division(a, b));
		break;
	case 5:
		printf("\nSee you later!\n\n");
		return 1;
	}
	return 0;
}
int main(void)
{
	printf("\nMY_CALCULATOR v1.0\n");
	int status = 0;
	char input[INPUT_SIZE] = "";
	operations();
	do
	{
		printf("> ");
		fgets(input, INPUT_SIZE, stdin);
		input[strlen(input) - 1] = '\0';
		status = make_action(get_action(input));
	} while (!status);
	return 0;
}