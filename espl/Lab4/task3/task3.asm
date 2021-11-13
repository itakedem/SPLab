%define X 'A'
%define STR string1

section .rodata
    print_format db '"%c" appears in "%s" %d times', 10, 0
    string1 db 'ABBA', 0
    string2 db 'BBA', 0
    string3 db 'BB', 0
    string4 db '', 0

section .text 
	global task3
    extern printf

task3:
    mov ebx, string1
    call printer

    mov ebx, string2
    call printer

    mov ebx, string3
    call printer

    mov ebx, string4
    call printer

    jmp finish

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
    mov ebp, esp    ;set a new activation frame

    mov ecx,-1      ;ecx counts the location in the string
    mov eax, 0      ;counts the number of times X in the string


.count:
    add ecx, 1          ;inc string position counter
    mov edx, ecx        ;set edx to the current location of the string

    add edx, ebx        ;get the position of the char

    movzx edx, BYTE [edx]   ;get a byte from the current position

    test dl dl
    je finishedString

    cmp edx, X
    je incCounter
    jmp .count



incCounter:
    add eax, 1
    jmp .count

finishedString:
    mov esp, ebp    ;restore pointers
    pop ecx         ;restore registers values
    pop edx         ;restore registers values
    pop ebp         ;restore pointers
    ret


printer:
    push ebp            ;backup ebp
    mov ebp, esp        ;set EBP to Func activation frame
    call counter

    push eax            ;arg 4 to printf - %d counter
    push ebx           ; argument 3 to printf - %s
    push X         ; argument 2 to printf - %c
    push print_format   ; argument 1 to printf
    call printf

    mov esp, ebp        ;restore pointers
    pop ebp             ;restore pointers
    ret

finish:
    mov eax, 1
    mov ebx, 0
    int 80h