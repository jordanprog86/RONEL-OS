#include <stdint.h>
#include <array>
#include <cstddef>
// Forward declaration of calculator function
extern "C" void run_calculator();

// VGA text mode colors
enum VGAColor {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LIGHT_GREY = 7,
    DARK_GREY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    LIGHT_BROWN = 14,
    WHITE = 15
};

// VGA text mode buffer
static const uint16_t VGA_WIDTH = 80;
static const uint16_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*)0xB8000;

// Current cursor position
static uint16_t cursor_x = 0;
static uint16_t cursor_y = 0;

// Current text color
static uint8_t text_color = (WHITE << 4) | BLACK;

// Function to set text color
void set_text_color(uint8_t color) {
    text_color = color;
}

// Function to create a VGA entry
static inline uint16_t vga_entry(unsigned char c, uint8_t color) {
    return (uint16_t)c | (uint16_t)color << 8;
}

// Clear the screen with a specific color
void clear_screen_with_color(uint8_t color) {
    uint8_t old_color = text_color;
    text_color = color;
    for (uint16_t y = 0; y < VGA_HEIGHT; y++) {
        for (uint16_t x = 0; x < VGA_WIDTH; x++) {
            const uint16_t index = y * VGA_WIDTH + x;
            VGA_MEMORY[index] = vga_entry(' ', text_color);
        }
    }
    text_color = old_color;
    cursor_x = 0;
    cursor_y = 0;
}
// Set cursor position
void set_cursor(uint16_t x, uint16_t y) {
    cursor_x = x;
    cursor_y = y;
}

// Print a character
void putchar(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        const uint16_t index = cursor_y * VGA_WIDTH + cursor_x;
        VGA_MEMORY[index] = vga_entry(c, text_color);
        cursor_x++;
    }

    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    if (cursor_y >= VGA_HEIGHT) {
        // Scroll screen
        for (uint16_t y = 0; y < VGA_HEIGHT - 1; y++) {
            for (uint16_t x = 0; x < VGA_WIDTH; x++) {
                const uint16_t index = y * VGA_WIDTH + x;
                VGA_MEMORY[index] = VGA_MEMORY[index + VGA_WIDTH];
            }
        }
        cursor_y = VGA_HEIGHT - 1;
    }
}
// Print a string
extern "C" void print(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        putchar(str[i]);
    }
}
// Clear the screen
void clear_screen() {
    for (uint16_t y = 0; y < VGA_HEIGHT; y++) {
        for (uint16_t x = 0; x < VGA_WIDTH; x++) {
            const uint16_t index = y * VGA_WIDTH + x;
            VGA_MEMORY[index] = vga_entry(' ', text_color);
        }
    }
    cursor_x = 0;
    cursor_y = 0;
}

// Print a number
extern "C" void print_int(int num) {
    if (num < 0) {
        putchar('-');
        num = -num;
    }
    
    if (num == 0) {
        putchar('0');
        return;
    }

    char buffer[12];
    int i = 0;
    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }
    
    while (--i >= 0) {
        putchar(buffer[i]);
    }
}


// Display logo function
void display_logo() {
   const char* logo[] = {
        "    ____  ___  _   _  _____  _     ____   ___ ",
        "   |  _ \\|  _ \\| \\ | ||  _  || |   / ___| / _ \\",
        "   | |_) | |_) |  \\| || | | || |  | |    | | | |",
        "   |  _ <|  __/| |\\  || | | || |__| |___ | |_| |",
        "   |_| \\_\\_|   |_| \\_|\\___/ |_____\\____| \\___/ "
    };
    const int logo_height = 5;
    const int logo_width = 45;  // Width of the logo
    
    // Calculate starting position to center the logo
    uint16_t start_x = (VGA_WIDTH - logo_width) / 2;
    uint16_t start_y = 5;  // Start a bit higher on the screen
    
    // Display each line of the logo with different colors
    for (int i = 0; i < logo_height; i++) {
        set_cursor(start_x, start_y + i);
        
        // Use different colors for each line
        switch (i) {
            case 0:
                set_text_color((LIGHT_CYAN << 4) | BLUE);  // Light cyan on blue
                break;
            case 1:
                set_text_color((LIGHT_GREEN << 4) | BLUE); // Light green on blue
                break;
            case 2:
                set_text_color((LIGHT_RED << 4) | BLUE);   // Light red on blue
                break;
            case 3:
                set_text_color((LIGHT_MAGENTA << 4) | BLUE); // Light magenta on blue
                break;
            case 4:
                set_text_color((LIGHT_BROWN << 4) | BLUE); // Light brown on blue
                break;
        }
        
        print(logo[i]);
    }
}

// Display splash screen
void display_splash_screen() {
    // Clear screen with blue background
    clear_screen_with_color((BLUE << 4) | BLACK);
    
    // Display the logo
    display_logo();
    
    // Center the OS name (moved down a bit to make room for logo)
    const char* os_name = "RONEL OS";
    const char* welcome = "Welcome to";
    const char* version = "Version 1.0";
    
    // Calculate center positions
    uint16_t os_x = (VGA_WIDTH - 8) / 2;  // "RONEL OS" is 8 chars
    uint16_t welcome_x = (VGA_WIDTH - 10) / 2;  // "Welcome to" is 10 chars
    uint16_t version_x = (VGA_WIDTH - 9) / 2;   // "Version 1.0" is 9 chars
    
    // Display welcome message (moved down)
    set_cursor(welcome_x, 12);
    set_text_color((WHITE << 4) | BLUE);
    print(welcome);
    
    // Display OS name (moved down)
    set_cursor(os_x, 14);
    set_text_color((LIGHT_CYAN << 4) | BLUE);
    print(os_name);
    
    // Display version (moved down)
    set_cursor(version_x, 16);
    set_text_color((WHITE << 4) | BLUE);
    print(version);
    
    // Add some decorative lines
    set_cursor(20, 18);
    set_text_color((LIGHT_GREY << 4) | BLUE);
    for (int i = 0; i < 40; i++) {
        putchar('-');
    }
    
    // Reset cursor and color
    set_cursor(0, 20);
    set_text_color((WHITE << 4) | BLACK);
}
// Calculator program
void run_calculator() {
    print("\nRONEL OS Calculator\n");
    print("-----------------\n");
    print("Enter first number: ");
    // In a real OS, we would implement input handling here
    int num1 = 10; // Placeholder
    print_int(num1);
    print("\nEnter second number: ");
    int num2 = 5; // Placeholder
    print_int(num2);
    
    print("\n\nResults:\n");
    print_int(num1); print(" + "); print_int(num2); print(" = "); print_int(num1 + num2); print("\n");
    print_int(num1); print(" - "); print_int(num2); print(" = "); print_int(num1 - num2); print("\n");
    print_int(num1); print(" * "); print_int(num2); print(" = "); print_int(num1 * num2); print("\n");
    if (num2 != 0) {
        print_int(num1); print(" / "); print_int(num2); print(" = "); print_int(num1 / num2); print("\n");
    } else {
        print("Division by zero error\n");
    }
}
// Kernel entry point
extern "C" void kernel_main() {
    // Display splash screen
    display_splash_screen();
 
    // Add a small delay
    for (volatile int i = 0; i < 1000000; i++)
    {
      //we display welcome message
    }
    
    // Clear screen and show calculator
    clear_screen();
    print("Welcome to RONEL OS!\n");
    print("-------------------\n");
    run_calculator();
    
    // Halt the CPU
    while (1) {
        __asm__("hlt");
    }
} 
