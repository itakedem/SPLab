%define X 'A'
%define STR string1

section .rodata
    print_format db '"%c" appears in "%s" %d times', 10, 0
    string1 db 'ABBA', 0
    string2 db 'BBA', 0
    string3 db 'BB', 0
    string4 db '', 0

section .text 
	global _start
    extern printf

_start:
    mov ebx, string1
    call printer

    mov ebx, string2
    call printer

    mov ebx, string3
    call printer

    mov ebx, string4
    call printer

    jmp _exit

printer:
    push ebp        ;save previous stack state
    mov ebp, esp    ;set a new activation frame

    call counter
    push eax            ;arg 4 to printf - %d counter
    push ebx            ; argument 3 to printf - %s
    push X              ; argument 2 to printf - %c
    push print_format   ; argument 1 to printf
    call printf

    mov esp, ebp        ;restore pointers
    pop ebp             ;restore pointers
    ret


counter:
    push ebp        ;save previous stack state
    push edx
    push ecx
    mov ebp, esp
    mov edx,-1      ;ecx counts the location in the string
    mov eax, 0      ;counts the number of times X in the string


count:
    add edx, 1          ;inc string position counter
    mov ecx, edx        ;set edx to the current location of the string

    add ecx, ebx        ;get the position of the char

    movzx ecx, BYTE [ecx]   ;get a byte from the current position

    test cl, cl
    je finishedString

    cmp ecx, X
    je incCounter

    jmp count


incCounter:
    add eax, 1
    jmp count

finishedString:
    mov esp, ebp    ;restore pointers
    pop ecx         ;restore registers values
    pop edx         ;restore registers values
    pop ebp         ;restore pointers
    ret

_exit:
    mov eax, 1
    int 80h