#ifndef TERMINAL_H
#define TERMINAL_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY (char*)0xB8000

// Colors
#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 11
#define COLOR_WHITE 15

// Function declarations
char make_color(char fg, char bg);
void terminal_initialize(void);
void terminal_clear(void);
void terminal_putchar(char c);
void terminal_putchar_at(char c, char color, int x, int y);
void terminal_print(const char* str);
void terminal_setcolor(char color);

// Cursor and color accessors
int terminal_get_cursor_col(void);
int terminal_get_cursor_row(void);
void terminal_set_cursor_pos(int col, int row);
char terminal_get_current_color(void);

#endif
