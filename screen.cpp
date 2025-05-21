#include "screen.h"
#include <cstddef>
#include <stdint.h>

// VGA text mode buffer
static uint16_t* const VGA_MEMORY = (uint16_t*)0xB8000;

// Current cursor position
static uint16_t cursor_x = 0;
static uint16_t cursor_y = 0;

// Current text color
static uint8_t text_color = (WHITE << 4) | BLACK;

// Function to create a VGA entry
static inline uint16_t vga_entry(unsigned char c, uint8_t color) {
    return (uint16_t)c | (uint16_t)color << 8;
}

// Function to set text color
void set_text_color(uint8_t color) {
    text_color = color;
}

// I/O port functions
static inline void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
} 

// Clear the screen
void clear_screen() {
    clear_screen_with_color(text_color);
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
    cursor_update();
}

// Set cursor position
void set_cursor(uint16_t x, uint16_t y) {
    if (x >= VGA_WIDTH) x = VGA_WIDTH - 1;
    if (y >= VGA_HEIGHT) y = VGA_HEIGHT - 1;
    cursor_x = x;
    cursor_y = y;
    cursor_update();
}

// Update hardware cursor position
void cursor_update() {
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
    
    // Tell the VGA controller we are setting the high cursor byte
    outb(0x3D4, 14);
    outb(0x3D5, pos >> 8);
    
    // Tell the VGA controller we are setting the low cursor byte
    outb(0x3D4, 15);
    outb(0x3D5, pos);
}

// Enable the hardware cursor
void cursor_enable(uint8_t cursor_start, uint8_t cursor_end) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
    
    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

// Disable the hardware cursor
void cursor_disable() {
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
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
    
    cursor_update();
}

// Print a string
/*void print(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        putchar(str[i]);
    }
}*/

// Print a string at specific coordinates
void print_at(const char* str, uint16_t x, uint16_t y) {
    set_cursor(x, y);
    print(str);
}

// Print a centered string at a specific y coordinate
void print_centered(const char* str, uint16_t y) {
    size_t len = 0;
    while (str[len] != '\0') len++;
    uint16_t x = (VGA_WIDTH - len) / 2;
    print_at(str, x, y);
}

// Print a number
/*void print_int(int num) {
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
}*/

// Print a hexadecimal number
void print_hex(uint32_t num) {
    const char* hex_chars = "0123456789ABCDEF";
    
    putchar('0');
    putchar('x');
    
    if (num == 0) {
        putchar('0');
        return;
    }
    
    char buffer[10];
    int i = 0;
    while (num > 0) {
        buffer[i++] = hex_chars[num % 16];
        num /= 16;
    }
    
    while (--i >= 0) {
        putchar(buffer[i]);
    }
}
