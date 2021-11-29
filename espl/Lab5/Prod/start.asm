section .text
	
global _start
global read
global write
global open
global close
global strlen

extern main
_start:

    mov eax, [esp]     ; value of argc
    mov ebx, esp       ; value of argv is esp+4
    add ebx, 4h        ;4h is the sys command to write file

    push ebx            ;sets argv as 2nd param
    push eax            ;sets argc as 1st param

	call main        ;calls main with the params on stack
    mov ebx,eax
	mov	eax,1
	int 0x80

%define DESC dword [ebp + 8]        ;file descriptor
%define POS dword [ebp + 12]        ;buffer position
%define SIZE dword [ebp + 16]       ;buffer size
read:
    enter 0, 0      ;sets a stack frame

    push ebx        ;save current register
    push ecx        ;save current register
    push edx        ;save current register

    mov eax, 3      ;3 is the sys command to read file
    mov ebx, DESC   ;2nd arg is the fd
    mov ecx, POS    ;3rd arg is the buffer pointer
    mov edx, SIZE   ;4th arg is the size of the buffer

    int 0x80        ;tells the kernel to do a system call from the registers

    pop edx         ;free register
    pop ecx         ;free register
    pop ebx         ;free registers

    leave           ;closes the stack frame
    ret             ;clearing args and returns


write:
    enter 0, 0      ;sets a stack frame

    push ebx        ;save current register
    push ecx        ;save current register
    push edx        ;save current register

    mov eax, 4      ;4 is the sys command to write file
    mov ebx, DESC   ;2nd arg is the path to the file
    mov ecx, POS    ;3rd arg is the mode of the opening
    mov edx, SIZE   ;4th arg is the size of the buffer

    int 0x80        ;tells the kernel to do a system call from the registers

    pop edx         ;free register
    pop ecx         ;free register
    pop ebx         ;free register
    

    leave           ;closes the stack frame
    ret             ;clearing args and returns

%define PATH dword [ebp + 8]
%define MODE dword [ebp + 12]
open:
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

%define DESCRIPTOR dword [ebp + 8]
close:
    enter 0, 0

    push ebx        ;save current register

    mov eax, 6h     ;6h is the sys command to close file
    mov ebx, DESCRIPTOR   ;2nd arg is the file descriptor

    int 0x80        ;tells the kernel to do a system call from the registers

    pop ebx

    leave              ;closes the stack frame
    ret                ;clearing args and returns

strlen:
    enter 0, 0
    push ebx

    mov eax, -1         ;setting the counter

.countLen:
    inc eax            ; incrementing the counter
    mov ebx, eax
    add ebx, [ebp+8]   ; ebx will hold the string's address from stack
    mov bl, BYTE [ebx] ; puts a byte(char) from the string in bl

    test bl, bl        ; checks if byte is \0
    jne .countLen      ; if not \0 cont to next byte

    pop ebx
    leave              ;closes the stack frame
    ret                ;clearing args and returns
