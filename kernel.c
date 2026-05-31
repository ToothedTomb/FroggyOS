#include "terminal.h"
#include "keyboard.h"

void kmain(void) {
    // Initialize terminal
    terminal_initialize();
    
    // Print welcome message
    terminal_print("========================================\n");
    terminal_print("     Welcome to FroggyOS v1.2!\n");
    terminal_print("     A Simple C Operating System\n");
    terminal_print("========================================\n\n");
    terminal_print("Type 'help' for available commands.\n\n");
    terminal_print("> ");
    
    // Initialize keyboard
    keyboard_init();
    
    // Main kernel loop
    while (1) {
        keyboard_poll();
        
        // Small delay to prevent CPU hogging
        for (volatile int i = 0; i < 100; i++);
    }
}
