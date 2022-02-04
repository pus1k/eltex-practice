#include "moves.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

void move_up(window_options* opt, char** lines)
{
	if (opt->y == 0)
	{
		if (opt->up_border != 0)
		{
			opt->up_border--;
			opt->bottom_border--;
			clear();
		}
	}
	else
	{
		opt->y--;
		if (opt->x == strlen(lines[opt->y + 1]))
		{
			opt->x = strlen(lines[opt->y]) - 1;
		}
	}
}
void move_down(window_options* opt, char** lines, int count)
{

	if (opt->y == opt->size.ws_row - 1)
	{
		if (opt->bottom_border != count)
		{
			opt->up_border++;
			opt->bottom_border++;
			clear();
		}
	}
	else
	{
		opt->y++;
		if (opt->x > strlen(lines[opt->y]))
		{
			opt->x = strlen(lines[opt->y]) - 1;
		}
	}
}
void move_right(window_options* opt, char** lines, int count)
{
	if (opt->x + 1 < strlen(lines[opt->y]))
	{
		opt->x++;
	}
	else
	{
		opt->x = 0;
		move_down(opt, lines, count);
	}
}
void move_left(window_options* opt, char** lines)
{
	if (opt->x > 0)
	{
		opt->x--;
	}
	else
	{
		opt->x = strlen(lines[opt->y]);
		move_up(opt, lines);
	}
}
void move_backspace(window_options* opt, char** lines, int* count)
{
	if (opt->x == 0)
	{
		opt->x = strlen(lines[opt->y - 1]) - 1;
		(*count) -= 1;
		for (int i = opt->y; i < (*count); i++)
		{
			if (strlen(lines[i]) != strlen(lines[i + 1]))
			{
				lines[i] = realloc(lines[i], strlen(lines[i + 1]));
			}
			strcpy(lines[i], lines[i + 1]);
		}
	}
	else
	{
		for (int i = --opt->x; i < strlen(lines[opt->y]) - 1; i++)
		{
			lines[opt->y][i] = lines[opt->y][i + 1];
		}
	}
	clear();
}