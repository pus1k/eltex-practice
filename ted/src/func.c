#include "func.h"

void logger(const char* problem_name, const char* optional, bool status)
{
	int bugd = open("log", O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
	char report[500];
	memset(report, 0, 500);
	if (status) {
		sprintf(report, "Success: %s %s\n", problem_name, optional);
	} else {
		sprintf(report, "Failure: %s %s\n", problem_name, optional);
	}
	write(bugd, report, strlen(report));
	close(bugd);
}

void slice_str(const char* str, char* buffer, size_t start, size_t end)
{
	memcpy(buffer, str + start, end - start + 1);
	buffer[end - start + 1] = '\0';
}

size_t get_line_length()
{
	if (data._line_index + 1 == data._line_size)
		return strlen(data._text) - data._lines_starts[data._line_index];
	else
		return data._lines_starts[data._line_index + 1] - data._lines_starts[data._line_index] - 1;
}

void move_up()
{
	if (data._line_index > 0) {
		data._line_index--;
		if (data._curs_y > 0) {		
			data._curs_y--;
		} else if (data._up > 0) {
			data._up--, data._bottom--;
		}
	}
	
	size_t line_len = get_line_length();
	if (data._curs_x > line_len)
		data._curs_x = line_len;

	data._text_index = data._lines_starts[data._line_index] + data._curs_x;
}

void move_down()
{
	if (data._line_index + 1 < data._line_size) {
		data._line_index++;
		if (data._curs_y + 3 < (size_t)(data._winsize.ws_row)) {		
			data._curs_y++;
		} else if (data._bottom < data._line_size) {
			data._up++, data._bottom++;
		}
	}

	size_t line_len = get_line_length();
	if (data._curs_x > line_len)
		data._curs_x = line_len;

	data._text_index = data._lines_starts[data._line_index] + data._curs_x;
}

void move_right()
{
	if (data._curs_x >= get_line_length()) {
		if (data._line_index + 1 < data._line_size) {
			data._curs_x = 0;
			move_down();
		} 
	} else {
		data._curs_x++;
		data._text_index++;
	}
}

void move_left()
{
	if (data._curs_x > 0) {
		data._curs_x--;
		data._text_index--;
	} else {
		if (data._line_index > 0) {
			data._line_index--;
			data._curs_x = get_line_length();
			data._line_index++;
			move_up();
		}
	}
}
void write_in_text(const int ch)
{
	if (data._text_capacity == strlen(data._text)) {
		data._text_capacity *= 2;
		data._text = realloc(data._text, data._text_capacity);
	}
	if (data._text_index == 0) {
		char save[strlen(data._text)]; 
		strcpy(save, data._text);
		strcpy(data._text + 1, save);
	} else if (data._text_index < strlen(data._text)) {
		char first[data._text_index], second[strlen(data._text) - data._text_index];
		slice_str(data._text, first, 0, data._text_index);
		slice_str(data._text, second, data._text_index, strlen(data._text));
		strcpy(data._text, first);
		strcpy(data._text + data._text_index + 1, second);
	}
	data._text[data._text_index] = (char)ch;

	if (ch == 10) {
		if (data._line_capacity == data._line_size) {
			data._line_capacity *= 2;
			data._lines_starts = realloc(data._lines_starts, sizeof(size_t) * data._line_capacity);
		}
		data._line_index++;
		memcpy(&data._lines_starts[data._line_index + 1], &data._lines_starts[data._line_index], sizeof(size_t) * (data._line_size - data._line_index));
		data._lines_starts[data._line_index] = data._text_index + 1;
		data._line_size++;
		if (data._bottom != (size_t)(data._winsize.ws_row - 2)) data._bottom++;
	}
	for (size_t i = data._line_index + 1; i < data._line_size; i++) data._lines_starts[i]++;
	if (ch == 10) data._line_index--;
	move_right();
}
void save()
{
	int fd = open(data._name, O_CREAT | O_WRONLY, S_IRWXU);
	if (fd < 0) {
		logger("save", data._name, false);
		exit(EXIT_FAILURE);
	}
	write(fd, data._text, strlen(data._text));
	close(fd);
}
void backspace()
{
	if (data._text_index > 0) {
		move_left();
		if (data._text[data._text_index] == '\n') {
			for (size_t i = 0, j = 0; i < data._line_size; i++) {
				if (i != data._line_index + 1) {
					data._lines_starts[j++] = data._lines_starts[i];
				}
			}
			data._line_size--;
		}
		memcpy(&data._text[data._text_index], &data._text[data._text_index + 1], strlen(data._text) - data._text_index - 1);
		data._text[strlen(data._text) - 1] = '\0';
		for (size_t i = data._line_index + 1; i < data._line_size; i++)
			data._lines_starts[i]--;
	}
}