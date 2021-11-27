section .text
    global atou_s

atou_s:
    enter 4,0       ;sets a stack frame with 8 bytes

    push ebx               ;save register
    push edx               ;save register
    push ecx               ;save register

    mov [ebp - 4], dword 1 ; divisor of base 10

    mov ebx, [ebp + 8]     ;get argument string
    mov edx, ebx
    mov ecx, 1
    mov eax, 0             ;set eax to be 0
    call find_end

.adder:
    dec edx
    cmp edx, ebx
    je .finish
    add [edx], 0
    mul [edx], ecx
    add eax, [edx]
    mul ecx, 10
    jmp .adder


.find_end:
    mov dl, [edx]
    test dl, dl            ;check if \0
    je ret
    inc edx
    jmp .find_end


.finish:
    pop ecx                ;restore registers
    pop edx
    pop ebx

    leave
