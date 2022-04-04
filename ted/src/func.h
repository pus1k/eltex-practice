#ifndef FUNC
#define FUNC

#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>

struct main_data
{
	struct winsize _winsize;
	size_t _up, _bottom, _curs_x, _curs_y;
	size_t _text_index, _text_capacity;
	size_t _line_index, _line_capacity, _line_size;
	size_t* _lines_starts;
	char* _text;
	char _name[256];
} data;

void logger(const char* problem_name, const char* optional, bool status);
void slice_str(const char* str, char* buffer, size_t start, size_t end);
size_t get_line_length();

void move_up();
void move_down();
void move_right();
void move_left();

void write_in_text(const int ch);
void backspace();
void save();
#endif