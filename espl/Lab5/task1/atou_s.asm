section .text
    global atoa_s

atoa_s:
    enter 4, 0
    push ebx
    push edx
    push ecx
 
    mov ebx, [ebp+8]        ; pointer to the begining of the string
 
    mov ecx, 0              ; storing the number in ecx

.loop1:
    cmp byte [ebx], 0       ; check if the string is empty
    je exit1

    movzx edx, byte [ebx]   ; getting the ascii value
    sub edx, 48             ; converting the ascii value to decimal
    mov [ebp-4], edx        ; storing the decimal value in ebp-4
    mov eax, 10    
    mul cl                  ; multiplying the number by 10   
    add eax, [ebp-4]        ; adding the number to the total
    mov ecx, eax            ; storing the total in ecx

    inc ebx                 ; incrementing the pointer

    jmp .loop1

exit1:
    movzx eax, cl           ; getting the number
 
    pop ebx
 
    leave
    ret