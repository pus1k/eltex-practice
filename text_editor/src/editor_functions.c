#include "editor_functions.h"
#include <fcntl.h>
#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OFFSET_NUMBERING 6
window_options options;

void sig_winch(int signo)
{
	init_opt(&options);
	resizeterm(options.size.ws_row, options.size.ws_col);
}

void init_opt(window_options* opt)
{
	ioctl(fileno(stdout), TIOCGWINSZ, (char*)&opt->size);
	opt->y = 0, opt->x = 0;
	opt->up_border = 0, opt->bottom_border = opt->size.ws_row;
}

void draw(window_options* opt, int count, char** lines)
{
	char str[OFFSET_NUMBERING];
	for (int line = opt->up_border, screen_y = 0; line < opt->bottom_border; line++, screen_y++)
	{
		sprintf(str, "%4d ", line + 1);
		move(screen_y, 0);
		printw(str);
		move(screen_y, OFFSET_NUMBERING);
		printw(lines[line]);
	}
	move(opt->y, opt->x + OFFSET_NUMBERING);
}

int get_lines_of_file(char* name, char*** lines)
{
	int fd = open(name, O_RDONLY);
	int i = 0, line = 0, status = 1, count = 0;
	char buf = '\0';
	while (status > 0)
	{
		status = read(fd, &buf, 1);
		if (buf == '\n' || buf == '\t' || status == 0)
			count++;
	}
	lseek(fd, 0, SEEK_SET);

	char* str = malloc(sizeof(*str) * 256);
	(*lines) = malloc(sizeof(**lines) * count);

	status = 1;
	while (status > 0)
	{
		status = read(fd, &buf, 1);
		str[i++] = buf;
		if (buf == '\n' || buf == '\t' || status == 0)
		{
			(*lines)[line] = malloc(sizeof(char) * i);

			while (i > 0)
			{
				i--, (*lines)[line][i] = str[i];
			}
			line++;
		}
	}
	free(str);
	close(fd);
	return count;
}
void main_loop(char* name)
{
	char** lines = NULL;
	int count = get_lines_of_file(name, &lines);

	initscr();
	signal(SIGWINCH, sig_winch);

	init_opt(&options);

	keypad(stdscr, TRUE);

	for (;;)
	{
		draw(&options, count, lines);
		int ch = getch();
		switch (ch)
		{
		case KEY_RIGHT:
			move_right(&options, lines, count);
			break;
		case KEY_LEFT:
			move_left(&options, lines);
			break;
		case KEY_DOWN:
			move_down(&options, lines, count);
			break;
		case KEY_UP:
			move_up(&options, lines);
			break;
		case KEY_BACKSPACE:
			move_backspace(&options, lines, &count);
			break;
		}
		refresh();
	}
	endwin();
}