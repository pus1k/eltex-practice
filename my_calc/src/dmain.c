#include <dirent.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_SIZE 10

struct Handle
{
	char** libs_names;
	void** handler_for_libs;
	int libs_count;
};

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
void get_libs(struct Handle* handle)
{
	char lib_path[INPUT_SIZE] = "./lib/";
	DIR* dir = opendir(lib_path);
	if (dir != NULL)
	{
		struct dirent* buf;
		handle->libs_count = 0;
		while ((buf = readdir(dir)) != NULL)
		{
			int length = strlen(buf->d_name);
			if (buf->d_name[length - 3] == '.' && buf->d_name[length - 2] == 's' && buf->d_name[length - 1] == 'o')
			{
				handle->libs_count += 1;
			}
		}
		closedir(dir);
		DIR* dir = opendir(lib_path);
		if (dir != NULL)
		{
			handle->handler_for_libs = malloc(sizeof(*handle->handler_for_libs) * handle->libs_count);
			handle->libs_names = malloc(sizeof(*handle->libs_names) * handle->libs_count);
			int length = 0, i = 0;
			char tmp[256];
			while ((buf = readdir(dir)) != NULL)
			{
				length = strlen(buf->d_name);
				if (length > 3)
				{
					if (buf->d_name[length - 3] == '.' && buf->d_name[length - 2] == 's' && buf->d_name[length - 1] == 'o')
					{
						strcpy(tmp, lib_path);
						strcat(tmp, buf->d_name);
						handle->handler_for_libs[i] = dlopen(tmp, RTLD_LAZY);
						if (!handle->handler_for_libs[i])
						{
							printf("Cannot open dynamic library %s", buf->d_name);
						}
						else
						{
							handle->libs_names[i] = malloc(sizeof(*handle->libs_names[i]) * strlen(buf->d_name));
							strcpy(handle->libs_names[i], buf->d_name);
							i++;
						}
					}
				}
			}
		}
	}
	closedir(dir);
}
int main(void)
{
	printf("\nMY_CALCULATOR v2.0\n\n");

	struct Handle handle;
	get_libs(&handle);

	printf("\nEnter operation number:\n");
	for (int i = 0; i < handle.libs_count; i++)
	{
		const char* (*ui)(void);
		*(const char***)(&ui) = dlsym(handle.handler_for_libs[i], "ui");
		printf("%d) %s\n", i + 1, (*ui)());
	}
	printf("%d) Exit\n\n", handle.libs_count + 1);
	int exit = 0;
	char input[INPUT_SIZE] = "";
	do
	{
		printf("> ");
		fgets(input, INPUT_SIZE, stdin);
		input[strlen(input) - 1] = '\0';

		if (input[0] != '\0')
		{
			if (!is_number(input))
			{
				int action = atoi(input);
				if (action == (handle.libs_count + 1))
				{
					exit = 1;
					continue;
				}
				else if (action < (handle.libs_count + 1) && action > 0)
				{
					int a = 0, b = 0;
					double (*make)(const double, const double);
					*(double**)(&make) = dlsym(handle.handler_for_libs[action - 1], "make");
					get_variables(&a, &b);
					(*make)(a, b);
					continue;
				}
			}
			printf("\nWrong operation number!\n\n");
		}
	} while (!exit);
	printf("\nSee you later!\n\n");
	for (int i = 0; i < handle.libs_count; i++)
	{
		dlclose(handle.handler_for_libs[i]);
		free(handle.libs_names[i]);
	}
	free(handle.libs_names);
	free(handle.handler_for_libs);
	return 0;
}