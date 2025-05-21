#include <stdint.h>

// Forward declarations of kernel functions we'll use
extern "C" {
    void print(const char* str);
    void print_int(int num);
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