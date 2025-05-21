# RONEL OS

A simple operating system written in C++ that boots up and runs a calculator program.
You can build it on Linux or Ubuntu
## Demo
![Screencast from 05-21-2025 10_27_53 AM](https://github.com/user-attachments/assets/f450ba85-f30f-405a-8779-ebad34356884)
## Features
- Custom bootloader
- Simple kernel
- Calculator program
- Custom OS name display during boot

## Building
To build RONEL OS, you'll need:
- NASM (for assembly files)
- GCC/G++ (for C/C++ files)
- Make
- QEMU (for testing)

### Build Instructions
1. Make sure you have all required tools installed
2. Run `make` to build the OS
3. Run `make run` to start the OS in QEMU

## Project Structure
- `boot.asm` - Bootloader code
- `kernel.cpp` - Main kernel code
- `calculator.cpp` - Calculator program
- `linker.ld` - Linker script
- `Makefile` - Build configuration

## Testing
The OS can be tested using QEMU:
```bash
make run
```
## Author
RONEL TCHOULAYEU
- C++/QT DEVELOPER, EMBEDDED SYSTEMS
- contact: jordanprog@yahoo.fr
## License
This project is open source and available under the MIT License. 
