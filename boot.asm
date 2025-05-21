[BITS 16]           ; 16-bit Real Mode
[ORG 0x7C00]        ; BIOS loads bootloader at this address

; Set up segments
cli                 ; Disable interrupts
mov ax, 0x0000      ; Set up segments
mov ds, ax
mov es, ax
mov ss, ax
mov sp, 0x7C00      ; Set up stack pointer
sti                 ; Enable interrupts

; Set video mode to 80x25 color text
mov ax, 0x0003      ; 80x25 color text mode
int 0x10

; Display splash screen
call display_splash_screen

; Load kernel
mov ah, 0x02        ; BIOS read sector function
mov al, 1           ; Number of sectors to read
mov ch, 0           ; Cylinder number
mov cl, 2           ; Sector number (1 is bootloader)
mov dh, 0           ; Head number
mov dl, 0x80        ; Drive number (0x80 for first hard disk)
mov bx, 0x1000      ; Load address
int 0x13            ; Call BIOS interrupt

; Switch to protected mode
cli                 ; Disable interrupts
lgdt [gdt_descriptor] ; Load GDT

; Enable A20 line
in al, 0x92
or al, 2
out 0x92, al

; Set PE bit in CR0
mov eax, cr0
or eax, 1
mov cr0, eax

; Jump to 32-bit code
jmp 0x08:protected_mode

; Display splash screen function
display_splash_screen:
    ; Clear screen
    mov ax, 0x0600  ; Scroll window up
    mov bh, 0x1F    ; White on blue
    mov cx, 0x0000  ; Upper left corner
    mov dx, 0x184F  ; Lower right corner
    int 0x10

    ; Display logo
    mov ah, 0x02    ; Set cursor position
    mov bh, 0x00    ; Page number
    mov dh, 8       ; Row (above OS name)
    mov dl, 30      ; Column (centered)
    int 0x10

    mov si, logo
    mov bl, 0x1F    ; White on blue
    call print_colored_string

    ; Set cursor position to center for OS name
    mov ah, 0x02    ; Set cursor position
    mov bh, 0x00    ; Page number
    mov dh, 12      ; Row (center of screen)
    mov dl, 35      ; Column (center of screen)
    int 0x10

    ; Display OS name with color
    mov si, os_name
    mov bl, 0x1F    ; White on blue
    call print_colored_string

    ; Display loading message
    mov ah, 0x02    ; Set cursor position
    mov dh, 14      ; Row
    mov dl, 33      ; Column
    int 0x10

    mov si, loading_msg
    mov bl, 0x1F    ; White on blue
    call print_colored_string

    ; Add some delay for visual effect
    mov cx, 0x0F    ; Delay count
.delay_loop:
    push cx
    mov cx, 0xFFFF
.inner_delay:
    loop .inner_delay
    pop cx
    loop .delay_loop

    ret

; Print colored string function
print_colored_string:
    mov ah, 0x0E    ; BIOS teletype function
.loop:
    lodsb           ; Load byte from SI into AL
    test al, al     ; Check if end of string
    jz .done        ; If zero, we're done
    int 0x10        ; Print character
    jmp .loop       ; Repeat for next character
.done:
    ret

; Data
logo db '    ____  ___  _   _  _____  _     ____   ___ ', 0x0D, 0x0A,\
        '   |  _ \|  _ \| \ | ||  _  || |   / ___| / _ \', 0x0D, 0x0A,\
        '   | |_) | |_) |  \| || | | || |  | |    | | | |', 0x0D, 0x0A,\
        '   |  _ <|  __/| |\  || | | || |__| |___ | |_| |', 0x0D, 0x0A,\
        '   |_| \_\_|   |_| \_|\___/ |_____\____| \___/ ', 0

os_name db 'RONEL OS', 0
loading_msg db 'Loading...', 0

; GDT
gdt_start:
    ; Null descriptor
    dd 0x0
    dd 0x0

    ; Code segment descriptor
    dw 0xFFFF       ; Limit (bits 0-15)
    dw 0x0000       ; Base (bits 0-15)
    db 0x00         ; Base (bits 16-23)
    db 10011010b    ; Access byte
    db 11001111b    ; Flags and Limit (bits 16-19)
    db 0x00         ; Base (bits 24-31)

    ; Data segment descriptor
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

[BITS 32]
protected_mode:
    ; Set up segment registers
    mov ax, 0x10    ; Data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Jump to kernel
    jmp 0x08:0x1000

; Boot signature
times 510-($-$$) db 0
dw 0xAA55 