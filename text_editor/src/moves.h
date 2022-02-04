#ifndef MOVES
#define MOVES

#include <sys/ioctl.h>

typedef struct window_options
{
	int y, x, offset_numbering;
	int up_border, bottom_border;
	struct winsize size;
} window_options;

void move_up(window_options* opt, char** lines);
void move_down(window_options* opt, char** lines, int count);
void move_right(window_options* opt, char** lines, int count);
void move_left(window_options* opt, char** lines);
void move_backspace(window_options* opt, char** lines, int* count);

#endif