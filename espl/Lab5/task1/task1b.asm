section .text
    global utoa_s

utoa_s:
    enter 8,0       ;sets a stack frame with 8 bytes

    push ebx               ;save register
    push edx               ;save register
    push ecx               ;save register

    mov [ebp - 4], dword 10 ; divisor of base 10

    mov eax, [ebp + 8]     ;get argument number
    mov ebx, [ebp + 12]    ;get argument buffer
    mov [ebp - 8], ebx     ;save buf address

    call clear_buffer
    mov ebx, [ebp - 8]      ;restore buffer pointer to start

.divider:
    mov edx, 0
    div dword [ebp - 4]     ;after this -> number will be on eax, and the remainder on edx
    mov cl, dl             ;cl will hold the remainder

    or eax, edx            ;check if finished dividing
    jne .convert
    jmp .finish_convert

.convert:
    add cl, '0'            ;convert to char
    mov [ebx], cl          ;write remainder to the buffer
    inc ebx                ;increments buffer's pointer
    jmp .divider

.finish_convert:
    mov [ebx], byte 0      ;adds \0 to the string, marks the end of it

reverse:
    dec ebx                ;decrements ebx to be one behind \0
    mov ecx, [ebp - 8]     ;sets ecx to the start of the buffer

.check_reverse:
    cmp ecx, ebx
    jge reverse

.replace:
    mov al, [ebx]          ;replace the bytes
    xchg al, [ecx]
    mov [ebx], al

    dec ebx                ;decrement pointer
    inc ecx                ;increment pointer
    jmp .check_reverse

.pop:
    pop ecx                ;restore registers
    pop edx
    pop ebx

    leave
    ret

clear_buffer:
    mov dl, [ebx]
    test dl, dl            ;check if \0
    jne .remove_byte
    jmp .end

.remove_byte:
    mov [ebx], byte 0      ;removes one byte
    inc ebx                ;increments pointer
    jmp clear_buffers


.end:
    ret