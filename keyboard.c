#include "keyboard.h"
#include "terminal.h"
#include "commands.h"

static char input_buffer[256];
static int input_pos = 0;

static unsigned char inb(unsigned short port) {
    unsigned char result;
    __asm__ __volatile__("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

static char scancode_to_char(unsigned char scancode) {
    if (scancode >= 0x80) return 0;
    
    switch(scancode) {
        case 0x0E: return '\b';
        case 0x1C: return '\n';
        case 0x39: return ' ';
        case 0x01: return 0;  // Escape
    }
    
    if (scancode >= 0x02 && scancode <= 0x0D) {
        const char chars[] = "1234567890-=";
        return chars[scancode - 0x02];
    }
    
    if (scancode >= 0x10 && scancode <= 0x1B) {
        const char chars[] = "qwertyuiop[]";
        return chars[scancode - 0x10];
    }
    
    if (scancode >= 0x1E && scancode <= 0x26) {
        const char chars[] = "asdfghjkl;'";
        return chars[scancode - 0x1E];
    }
    
    if (scancode >= 0x2C && scancode <= 0x32) {
        const char chars[] = "zxcvbnm,./";
        return chars[scancode - 0x2C];
    }
    
    return 0;
}

static void process_keyboard_input(char c) {
    if (c == '\n') {
        terminal_putchar('\n');
        input_buffer[input_pos] = '\0';
        
        if (input_pos > 0) {
            handle_command(input_buffer);
        } else {
            terminal_print("> ");
        }
        
        input_pos = 0;
    }
    else if (c == '\b') {
        if (input_pos > 0) {
            input_pos--;
            
            int col = terminal_get_cursor_col();
            int row = terminal_get_cursor_row();
            
            if (col > 0) {
                terminal_set_cursor_pos(col - 1, row);
            }
            
            terminal_putchar_at(' ', terminal_get_current_color(), 
                              terminal_get_cursor_col(), terminal_get_cursor_row());
        }
    }
    else if (c >= ' ' && c <= '~') {
        if (input_pos < 255) {
            input_buffer[input_pos++] = c;
            terminal_putchar(c);
        }
    }
}

void keyboard_init(void) {
    // Nothing to initialize for polling mode
}

void keyboard_poll(void) {
    if (inb(0x64) & 0x01) {
        unsigned char scancode = inb(0x60);
        char ascii = scancode_to_char(scancode);
        if (ascii != 0) {
            process_keyboard_input(ascii);
        }
    }
}
