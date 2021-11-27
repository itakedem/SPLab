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
%define BUFFER_SIZE dword 100
main:
    enter 104, 0            ; reserve 100 bytes for buffer starting at ebp-100

    mov ebx, ARGV
    add ebx, 4              ; now ebx points at argv[1] - which is the file arg

.open_the_file:
    mov ebx, [ebx]          ; pointer to str
    push O_RDONLY
    push ebx

    call open               ; now if all good we have the fd at eax

    pop ebx
    pop ebx                 ; clean the stack

    mov [ebp - 104], eax    ; save fd on stack

    cmp eax, 0
    jle .bad_ret            ; if eax <= 0 something bad happened