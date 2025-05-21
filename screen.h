#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>

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

// Screen dimensions
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// Function declarations
#ifdef __cplusplus
extern "C" {
#endif

// Basic screen functions
void clear_screen();
void clear_screen_with_color(uint8_t color);
void set_cursor(uint16_t x, uint16_t y);
void set_text_color(uint8_t color);

// Print functions
void putchar(char c);
void print(const char* str);
void print_int(int num);
void print_hex(uint32_t num);
void print_at(const char* str, uint16_t x, uint16_t y);
void print_centered(const char* str, uint16_t y);

// Cursor control
void cursor_enable(uint8_t cursor_start, uint8_t cursor_end);
void cursor_disable();
void cursor_update();

#ifdef __cplusplus
}
#endif

#endif // SCREEN_H 