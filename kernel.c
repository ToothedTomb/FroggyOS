// kernel.c - A simple C OS with terminal output and keyboard support

// VGA text mode constants
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY (char*)0xB8000

// Colors
#define COLOR_BLACK 0
#define COLOR_GREEN 2
#define COLOR_WHITE 15
#define COLOR_CYAN 11

// Terminal state
int terminal_row = 0;
int terminal_col = 0;
char terminal_color;
char* terminal_buffer;

// Input buffer
char input_buffer[256];
int input_pos = 0;

// I/O Port functions
unsigned char inb(unsigned short port) {
    unsigned char result;
    __asm__ __volatile__("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

// Make a VGA color pair
char make_color(char fg, char bg) {
    return fg | (bg << 4);
}

// Put a character at specific position
void terminal_putchar_at(char c, char color, int x, int y) {
    int index = y * VGA_WIDTH + x;
    terminal_buffer[index * 2] = c;
    terminal_buffer[index * 2 + 1] = color;
}

// Scroll the screen up one line
void terminal_scroll() {
    for (int y = 1; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            char c = terminal_buffer[(y * VGA_WIDTH + x) * 2];
            char color = terminal_buffer[(y * VGA_WIDTH + x) * 2 + 1];
            terminal_putchar_at(c, color, x, y - 1);
        }
    }
    
    // Clear last line
    for (int x = 0; x < VGA_WIDTH; x++) {
        terminal_putchar_at(' ', terminal_color, x, VGA_HEIGHT - 1);
    }
}

// Put a character at current cursor position
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

// Print a string
void terminal_print(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        terminal_putchar(str[i]);
    }
}

// Clear the entire screen
void terminal_clear() {
    terminal_color = make_color(COLOR_GREEN, COLOR_BLACK);
    terminal_buffer = VGA_MEMORY;
    
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            terminal_putchar_at(' ', terminal_color, x, y);
        }
    }
    
    terminal_row = 0;
    terminal_col = 0;
}

// Simple command handler
void handle_command(const char* input) {
    if (input[0] == '\0') {
        return;
    }
    
    if (input[0] == 'h' && input[1] == 'e' && input[2] == 'l' && input[3] == 'p') {
        terminal_print("\nAvailable commands:\n");
        terminal_print("  help  - Show this help\n");
        terminal_print("  clear - Clear the screen\n");
        terminal_print("  hello - Say hello\n");
        terminal_print("  info  - Show system info\n");
        terminal_print("\n> ");
    }
    else if (input[0] == 'c' && input[1] == 'l' && input[2] == 'e' && input[3] == 'a' && input[4] == 'r') {
        terminal_clear();
        terminal_print("> ");
    }
    else if (input[0] == 'h' && input[1] == 'e' && input[2] == 'l' && input[3] == 'l' && input[4] == 'o') {
        terminal_print("\nHello from FroggyOS!\n\n> ");
    }
    else if (input[0] == 'i' && input[1] == 'n' && input[2] == 'f' && input[3] == 'o') {
        terminal_print("\nFroggyOS - Simple C OS\n");
        terminal_print("Running in 32-bit protected mode\n");
        terminal_print("VGA Text Mode: 80x25\n\n> ");
    }
    else {
        terminal_print("\nUnknown command. Type 'help' for available commands.\n\n> ");
    }
}

// Convert scancode to ASCII character
char scancode_to_char(unsigned char scancode) {
    // Only handle key presses (scancode < 0x80 means press, >= 0x80 means release)
    if (scancode >= 0x80) return 0;
    
    switch(scancode) {
        case 0x01: return 0;      // Escape
        case 0x0E: return '\b';    // Backspace
        case 0x1C: return '\n';    // Enter
        case 0x39: return ' ';     // Space
    }
    
    // Numbers row (2-13)
    if (scancode >= 0x02 && scancode <= 0x0D) {
        const char chars[] = "1234567890-=";
        return chars[scancode - 0x02];
    }
    
    // QWERTYUIOP row (16-27)
    if (scancode >= 0x10 && scancode <= 0x1B) {
        const char chars[] = "qwertyuiop[]";
        return chars[scancode - 0x10];
    }
    
    // ASDFGHJKL row (30-38)
    if (scancode >= 0x1E && scancode <= 0x26) {
        const char chars[] = "asdfghjkl;'";
        return chars[scancode - 0x1E];
    }
    
    // ZXCVBNM row (44-50)
    if (scancode >= 0x2C && scancode <= 0x32) {
        const char chars[] = "zxcvbnm,./";
        return chars[scancode - 0x2C];
    }
    
    return 0;
}

// Process keyboard input - FIXED backspace without glitches
void process_keyboard_input(char c) {
    if (c == '\n') {  // Enter key
        terminal_putchar('\n');
        input_buffer[input_pos] = '\0';
        
        if (input_pos > 0) {
            handle_command(input_buffer);
        } else {
            terminal_print("> ");
        }
        
        input_pos = 0;
    }
    else if (c == '\b') {  // Backspace key
        if (input_pos > 0) {
            input_pos--;
            
            // Move cursor left
            if (terminal_col > 0) {
                terminal_col--;
            }
            
            // Write space to erase the character
            terminal_putchar_at(' ', terminal_color, terminal_col, terminal_row);
            
            // Cursor is now at the erased position, ready for next input
        }
    }
    else if (c >= ' ' && c <= '~') {  // Printable characters
        if (input_pos < 255) {
            input_buffer[input_pos++] = c;
            terminal_putchar(c);
        }
    }
}

// Main kernel entry point
void kmain() {
    // Initialize the terminal
    terminal_clear();
    
    // Print welcome message
    terminal_print("========================================\n");
    terminal_print("     Welcome to FroggyOS!\n");
    terminal_print("     A Simple C Operating System\n");
    terminal_print("========================================\n\n");
    terminal_print("Type 'help' for available commands.\n\n");
    terminal_print("> ");
    
    // Main keyboard polling loop
    while (1) {
        // Check if keyboard data is available (bit 0 of port 0x64)
        if (inb(0x64) & 0x01) {
            unsigned char scancode = inb(0x60);
            char ascii = scancode_to_char(scancode);
            if (ascii != 0) {
                process_keyboard_input(ascii);
            }
        }
        
        // Small delay to prevent CPU hogging
        for (volatile int i = 0; i < 100; i++);
    }
}
