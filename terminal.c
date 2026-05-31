#include "terminal.h"

static int terminal_row = 0;
static int terminal_col = 0;
static char terminal_color;
static char* terminal_buffer;

char make_color(char fg, char bg) {
    return fg | (bg << 4);
}

void terminal_putchar_at(char c, char color, int x, int y) {
    int index = y * VGA_WIDTH + x;
    terminal_buffer[index * 2] = c;
    terminal_buffer[index * 2 + 1] = color;
}

static void terminal_scroll(void) {
    for (int y = 1; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            char c = terminal_buffer[(y * VGA_WIDTH + x) * 2];
            char color = terminal_buffer[(y * VGA_WIDTH + x) * 2 + 1];
            terminal_putchar_at(c, color, x, y - 1);
        }
    }
    
    for (int x = 0; x < VGA_WIDTH; x++) {
        terminal_putchar_at(' ', terminal_color, x, VGA_HEIGHT - 1);
    }
}

void terminal_initialize(void) {
    terminal_color = make_color(COLOR_GREEN, COLOR_BLACK);
    terminal_buffer = VGA_MEMORY;
    terminal_row = 0;
    terminal_col = 0;
    terminal_clear();
}

void terminal_clear(void) {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            terminal_putchar_at(' ', terminal_color, x, y);
        }
    }
    terminal_row = 0;
    terminal_col = 0;
}

void terminal_setcolor(char color) {
    terminal_color = color;
}

void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_col = 0;
        terminal_row++;
        if (terminal_row >= VGA_HEIGHT) {
            terminal_scroll();
            terminal_row = VGA_HEIGHT - 1;
        }
        return;
    }
    
    terminal_putchar_at(c, terminal_color, terminal_col, terminal_row);
    terminal_col++;
    
    if (terminal_col >= VGA_WIDTH) {
        terminal_col = 0;
        terminal_row++;
        if (terminal_row >= VGA_HEIGHT) {
            terminal_scroll();
            terminal_row = VGA_HEIGHT - 1;
        }
    }
}

void terminal_print(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        terminal_putchar(str[i]);
    }
}

// Cursor and color accessors
int terminal_get_cursor_col(void) {
    return terminal_col;
}

int terminal_get_cursor_row(void) {
    return terminal_row;
}

void terminal_set_cursor_pos(int col, int row) {
    terminal_col = col;
    terminal_row = row;
}

char terminal_get_current_color(void) {
    return terminal_color;
}
