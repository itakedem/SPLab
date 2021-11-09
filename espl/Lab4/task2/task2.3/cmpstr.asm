section .text
	global cmpstr

cmpstr:
    push ebp
    push ebx
    push ecx

    mov ebp, esp
    mov eax,-1

.find_diff:
    add eax,1
    mov ebx, eax
    mov ecx, eax

    add ebx, [ebp + 16]
    add ecx, [ebp + 20]

    movzx ebx, BYTE [ebx]
    movzx ecx, BYTE [ecx]

    test bl, bl
    je CHECK_SEC

    test cl, cl
    je FIR_BIG

    cmp ebx, ecx
    jl SEC_BIG
    jg FIR_BIG
    
    jmp .find_diff

CHECK_SEC:
    test cl, cl
    je EQUAL

EQUAL:
    mov eax, 0
    jmp FINISH

SEC_BIG:
    mov eax, ebx
    sub eax, ecx
    jmp FINISH

FIR_BIG:
    mov eax, ebx
    sub eax, ecx
    jmp FINISH

FINISH:
    mov esp, ebp
    pop ecx
    pop ebx
    pop ebp
    ret

