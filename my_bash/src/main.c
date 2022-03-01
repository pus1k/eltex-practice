#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// STDIN_FILENO 0
// STDOUT_FILENO 1

void free_tokens_list(char** tokens_list, int tokens_count)
{
	for (int i = 0; i < tokens_count; i++)
		free(tokens_list[i]);
	free(tokens_list);
}
char** get_tokens(char* str, int* tokens_count)
{
	int number_of_malloc_tokens = 3;
	char* token = malloc(sizeof(*token) * 255);
	char** tmp = malloc(sizeof(*tmp) * number_of_malloc_tokens);

	for (size_t i = 0, token_len = 0; i < strlen(str); i++)
	{
		if ((str[i] == '\n' || str[i] == '\t' || str[i] == ' ') && token_len > 0)
		{
			token[token_len] = '\0';
			if ((*tokens_count) == number_of_malloc_tokens)
			{
				number_of_malloc_tokens += 3;
				tmp = realloc(tmp, number_of_malloc_tokens * sizeof(*tmp));
			}
			tmp[(*tokens_count)] = malloc(sizeof(**tmp) * token_len);
			strcpy(tmp[(*tokens_count)], token);
			(*tokens_count) += 1, token_len = 0;
		}
		else if (str[i] != '\n' && str[i] != '\t' && str[i] != ' ')
			token[token_len++] = str[i];
	}
	free(token);
	return tmp;
}
char** find_next_command(char** tokens_list, int tokens_count, int start, int* count)
{
	char** tmp = malloc(sizeof(*tmp) * (tokens_count - start));
	for (int i = start, j = 0; i < tokens_count && strcmp(tokens_list[i], "|") != 0; i++, j++, (*count) += 1)
	{
		tmp[j] = tokens_list[i];
	}
	tmp[(*count)] = NULL;
	return tmp;
}
void execute_commands(char*** tokens_list, int count)
{
	int status;
	pid_t pid;

	if (count > 1)
	{
		pid_t ppid = fork();
		if (ppid == 0)
		{
			int fd[count][2];
			for (int i = 0; i < count; i++)
				if (pipe(fd[i]) < 0)
					perror("pipe"), exit(EXIT_FAILURE);

			for (int i = 0; i < count; i++)
			{
				pid = fork();
				if (pid == 0)
				{
					dup2(fd[i][STDIN_FILENO], STDIN_FILENO);
					close(fd[i][STDOUT_FILENO]);
					if (i + 1 != count)
					{
						dup2(fd[i + 1][STDOUT_FILENO], STDOUT_FILENO);
						close(fd[i + 1][STDIN_FILENO]);
					}
					execvp(tokens_list[i][0], tokens_list[i]);
					perror("execvp"), exit(EXIT_FAILURE);
				}
				close(fd[i + 1][STDOUT_FILENO]);
				do
				{
					waitpid(pid, &status, WUNTRACED);
				} while (!WIFEXITED(status) && !WIFSIGNALED(status));
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			int sp;
			do
			{
				waitpid(ppid, &sp, WUNTRACED);
			} while (!WIFEXITED(sp) && !WIFSIGNALED(sp));
		}
	}
	else
	{
		if (!strcmp(tokens_list[0][0], "cd"))
		{
			if (chdir(tokens_list[0][1]) != 0)
			{
				perror("cd");
			}
		}
		else if (!strcmp(tokens_list[0][0], "exit"))
		{
			exit(EXIT_SUCCESS);
		}
		else
		{
			pid = fork();
			if (pid == 0)
			{
				execvp(tokens_list[0][0], tokens_list[0]);
				perror("execvp"), exit(EXIT_FAILURE);
			}
			do
			{
				waitpid(pid, &status, WUNTRACED);
			} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		}
	}
}

int main(void)
{
	printf("\nYou run my_bash and this is so good!");
	printf("\nIf you wanna go out just enter 'quit' or 'q'.\nPlease enter your commands:\n\n");

	int tokens_count = 0, start = 0, count = 0, iterations = 0;
	char* str = malloc(sizeof(*str) * 255);

	for (;;)
	{
		tokens_count = 0, start = 0, count = 0, iterations = 0;

		printf("> ");
		fgets(str, 256, stdin);

		char** tokens_list = get_tokens(str, &tokens_count);

		if (tokens_count == 0)
		{
			free_tokens_list(tokens_list, tokens_count);
			continue;
		}

		if (!strcmp(tokens_list[0], "quit") || !strcmp(tokens_list[0], "q"))
		{
			free_tokens_list(tokens_list, tokens_count);
			break;
		}

		char*** rezult = malloc(sizeof(**rezult) * tokens_count);

		if (rezult == NULL)
		{
			free_tokens_list(tokens_list, tokens_count);
			free(str);
			perror("malloc rezult"), exit(EXIT_FAILURE);
		}

		do
		{
			rezult[iterations++] = find_next_command(tokens_list, tokens_count, start, &count);
			start += count + 1, count = 0;
		} while (start < tokens_count);

		execute_commands(rezult, iterations);
		free_tokens_list(tokens_list, tokens_count);

		for (int i = 0; i < iterations; i++)
		{
			free(rezult[i]);
		}
		free(rezult);
	}
	free(str);
	printf("\n\nThank you for your time!\nSee you later!\n\n");
	return 0;
}