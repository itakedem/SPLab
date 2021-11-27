section .text
    global _start
    global read
    global write
    global open
    global close
    global strlen

_start:
    mov eax, [esp]          ; argc
    mov ebx, esp            ; argv @esp+4
    add ebx, 4h             ;4h means write

    push ebx
    push eax

    call	main
    mov     ebx,eax
    mov	eax,1
    int 0x80


%define ARGC dword [ebp + 8]
%define ARGV dword [ebp + 12]
%define BUFFER_SIZE dword 50
main:
    enter 54, 0            ; reserve 50 bytes for buffer starting at ebp-50

    mov ebx, ARGV
    add ebx, 4              ; now ebx points at argv[1] - which is the file arg

.open_the_file:
    mov ebx, [ebx]          ; pointer to str
    push 0                  ; push mode of open file
    push ebx                ; push the file path

    call open               ; now if all good we have the fd at eax

    pop ebx
    pop ebx                 ; clean the stack

    mov [ebp - 54], eax    ; save fd on stack

    cmp eax, 0
    jle .failure            ; if eax <= 0 something bad happened

.read_from_file:
    push BUFFER_SIZE        ; prep arguments for read
    mov ebx, ebp
    sub ebx, 50
    push ebx
    push dword [ebp - 54]   ; 

    call read
    pop ebx
    pop ebx
    pop ebx                 ; clear arguments

    cmp eax, -1             ; check for errors
    jle .failure

    cmp eax, 0
    jg .print_buf ; if read more than 0 bytes print to screen

.finished_reading:
    push dword [ebp - 54]  ; close the file
    call close
    pop ebx
    jmp .return

%define STDOUT dword 1

.print_buf:
    push eax                ; number of bytes read to buffer
    mov ebx, ebp
    sub ebx, 50
    push ebx                ; buffer pointer
    push STDOUT             ; fd for stdout

    call write

    pop ebx
    pop ebx
    pop ebx                 ; clear arguments

    cmp eax, 0              ; check if something was written to stdout
    jle .failure

    jmp .read_from_file

.failure:
    mov eax, 1
    jmp .do_return

.return:
    mov eax, 0

.do_return:
    leave
    ret

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
