#include "func.h"

void init_data()
{
	ioctl(fileno(stdout), TIOCGWINSZ, (char*)&data._winsize);
	data._up = 0, data._bottom = 0;
	data._curs_x = 0, data._curs_y = 0;
	data._text_index = 0, data._line_index = 0;
	data._text_capacity = 0, data._line_capacity = 0;
	data._line_size = 1;
	data._lines_starts = NULL, data._text = NULL;
}

int set_text(char* name)
{
	int fd = open(name, O_CREAT | O_RDONLY, S_IRWXU), ret = 0;
	if (fd > 0) { 
		for (char buf = '\0'; read(fd, &buf, 1) > 0; data._text_capacity++) {
			if (buf == '\n') data._line_size++;
		}
		lseek(fd, 0, SEEK_SET);
		
		if (data._text_capacity < data._winsize.ws_row * data._winsize.ws_col)
			data._text_capacity = data._winsize.ws_row * data._winsize.ws_col * 2;
		else 
			data._text_capacity *= 2;

		if (data._line_size >= (size_t)(data._winsize.ws_row - 2)) {
			data._bottom = (size_t)(data._winsize.ws_row - 2);
			data._line_capacity = data._line_size * 2;
		} else {
			data._bottom = data._line_size;
			data._line_capacity = data._winsize.ws_row;
		}

		data._text = calloc(sizeof(*data._text), data._text_capacity);
		data._lines_starts = calloc(sizeof(*data._lines_starts), data._line_capacity);

		for (size_t i = 0, j = 0; read(fd, &(data._text[i]), 1) != 0; i++) {
			if (data._text[i] == '\n') data._lines_starts[++j] = i + 1;
		}
		ret++;
	}
	close(fd);
	return ret;
}
void draw()
{
	WINDOW* main = newwin(data._winsize.ws_row - 2, data._winsize.ws_col, 0, 0);
	WINDOW* opt = newwin(3, data._winsize.ws_col, data._winsize.ws_row - 2, 0);
	box(opt, 0, 0);
	
	mvwprintw(opt, 1, 1, "F1 OPEN    F2 SAVE    F3 QUIT\t\t%s\t%.0lf%%", data._name, 
		(1 / (double)(strlen(data._text))) * data._text_index * 100);
	if (data._text != NULL) {
		size_t start = data._lines_starts[data._up];
		size_t end = data._lines_starts[data._bottom];

		if (data._bottom + 1 < data._line_size) {
			end += data._lines_starts[data._bottom + 1] - data._lines_starts[data._bottom];
		} else {
			end += strlen(data._text) - data._lines_starts[data._bottom];
		}
		if (data._bottom > (size_t)(data._winsize.ws_row - 2)) {
			char* str = malloc(end - start + 1);
			slice_str(data._text, str, start, end);
			mvwprintw(main, 0, 0, "%s", str);
			free(str);
		} else {
			mvwprintw(main, 0, 0, "%s", data._text);
			
		}
	}
	move(data._curs_y, data._curs_x);
	wrefresh(main);
	wrefresh(opt);
	refresh();
	delwin(main);
	delwin(opt);
}
void set_file()
{
	if (data._text != NULL && data._lines_starts != NULL) {
		free(data._text);
		free(data._lines_starts);
		init_data();
	}
	WINDOW* search_win = newwin(3, data._winsize.ws_col, data._winsize.ws_row - 2, 0);
	box(search_win, 0, 0);
	do {
		mvwprintw(search_win, 1, 1, "ENTER FILE NAME: ");
		wrefresh(search_win);
		wgetstr(search_win, data._name);
	} while (!set_text(data._name));
	delwin(search_win);
	clear();
	
}
int main(int argc, char* argv[])
{
	initscr();
	keypad(stdscr, TRUE);
	init_data();
	if (argc == 2) {
		strcpy(data._name, argv[1]);
		if (!set_text(data._name)) {
			endwin();
			return 0;
		}
	} else {
		set_file();
	}
	draw();
	int ch = 0;
	while (ch != 267)
	{
		draw();
		ch = getch();
		switch (ch) {
			case KEY_RIGHT:
				move_right();
				break;
			case KEY_LEFT:
				move_left();
				break;
			case KEY_UP:
				move_up();
				break;
			case KEY_DOWN:
				move_down();
				break;
			case KEY_BACKSPACE:
				backspace();
				break;
			case 265: // F1 FIND
				set_file();
				break;
			case 266: // F2 SAVE
				save();
				break;
			case 267: // F3 QUIT
				save();
				break;
			default:
				if ((ch > 31 && ch < 127) || ch == 10) write_in_text(ch);
		}
	}
	endwin();
	return 0;
}