#include "commands.h"
#include "terminal.h"

void handle_command(const char* input) {
    if (input[0] == '\0') {
        return;
    }
    
    // help command
    if (input[0] == 'h' && input[1] == 'e' && input[2] == 'l' && input[3] == 'p') {
        terminal_print("\nAvailable commands:\n");
        terminal_print("  help  - Show this help\n");
        terminal_print("  clear - Clear the screen\n");
        terminal_print("  hello - Say hello\n");
        terminal_print("  info  - Show system info\n");
        terminal_print("  echo  - Print text to the console\n");
        terminal_print("\n> ");
    }
    
    // clear command
    else if (input[0] == 'c' && input[1] == 'l' && input[2] == 'e' && input[3] == 'a' && input[4] == 'r') {
        terminal_clear();
        terminal_print("> ");
    }
    
    // hello command
    else if (input[0] == 'h' && input[1] == 'e' && input[2] == 'l' && input[3] == 'l' && input[4] == 'o') {
        terminal_print("\nHello from FroggyOS!\n\n> ");
    }
    
    // info command
    else if (input[0] == 'i' && input[1] == 'n' && input[2] == 'f' && input[3] == 'o') {
        terminal_print("\nFroggyOS - Alpha v1.2\n");
        terminal_print("VGA Text Mode: 80x25\n\n> ");
    }
    
    // echo command
    else if (input[0] == 'e' && input[1] == 'c' && input[2] == 'h' && input[3] == 'o') {
        int i = 4;
        while (input[i] == ' ') {
            i++;
        }
        
        if (input[i] == '\0') {
            terminal_print("\nUsage: echo <message>\n");
            terminal_print("Example: echo Hello World!\n\n> ");
        } else {
            terminal_print("\n");
            while (input[i] != '\0') {
                terminal_putchar(input[i]);
                i++;
            }
            terminal_print("\n\n> ");
        }
    }
    
    // unknown command
    else {
        terminal_print("\nUnknown command. Type 'help' for available commands.\n\n> ");
    }
}
