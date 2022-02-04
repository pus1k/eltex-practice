#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
	int fd = open("test.txt", O_WRONLY | O_CREAT, S_IRWXU);

	if (fd == -1)
	{
		printf("Cannot open test.txt\n");
	}
	else
	{
		// WRITE TO FILE "Hello world!"
		char str[] = "Hello world!";
		if (write(fd, str, strlen(str)) == -1)
		{
			printf("Cannot write to test.txt\n");
		}
		else
		{
			close(fd);
			// SERIAL OUTPUT
			printf("SERIAL: ");
			fd = open("test.txt", O_RDONLY);
			if (fd == -1)
			{
				printf("Cannot open test.txt\n");
			}
			else
			{
				char buf = '\0';
				while (read(fd, &buf, 1) > 0)
				{
					printf("%c", buf);
				}
				printf("\n");
			}
			close(fd);
			// REVERSE OUTPUT
			printf("REVERSE: ");
			fd = open("test.txt", O_RDONLY);
			if (fd == -1)
			{
				printf("Cannot open test.txt\n");
			}
			else
			{
				struct stat handle;
				stat("test.txt", &handle);
				char buf = '\0';
				off_t offset = -1;
				lseek(fd, offset, SEEK_END);
				while (-1 * (offset + 1) < handle.st_size)
				{
					read(fd, &buf, 1);
					printf("%c", buf), offset--;
					lseek(fd, offset, SEEK_END);
				}
				printf("\n");
			}
		}
	}
	close(fd);

	return 0;
}