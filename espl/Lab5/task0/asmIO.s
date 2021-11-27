section .text
    global open_file
    global close_file

%define PATH dword [ebp + 8]
%define MODE dword [ebp + 12]

open_file:
    enter 0, 0      ;sets a stack frame

    push ebx        ;save current register
    push ecx        ;save current register

    mov eax, 5h     ;5h is the sys command to open file
    mov ebx, PATH   ;2nd arg is the path to the file
    mov ecx, MODE   ;3rd arg is the mode of the opening

    int 0x80        ;tells the kernel to do a system call from the registers

    pop ecx         ;free register
    pop ebx         ;free register

    leave           ;closes the stack frame
    ret             ;clearing args and returns

%define DESC dword [ebp + 8]
close_file:
    enter 0, 0

    push ebx        ;save current register

    mov eax, 6h     ;6h is the sys command to close file
    mov ebx, DESC   ;2nd arg is the file descriptor

    int 0x80        ;tells the kernel to do a system call from the registers

    pop ebx

    leave
    ret