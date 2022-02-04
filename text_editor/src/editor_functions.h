#ifndef EDITOR_PROMAX
#define EDITOR_PROMAX

#include "moves.h"

void sig_winch(int signo);
void init_opt(window_options* opt);
void draw(window_options* opt, int count, char** lines);
int get_lines_of_file(char* name, char*** lines);
void main_loop(char*);

#endif