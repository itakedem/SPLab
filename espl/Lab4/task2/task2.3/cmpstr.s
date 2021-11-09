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
    je RETURN_DIFF

    cmp ebx, ecx
    jl RETURN_DIFF
    jg RETURN_DIFF
    
    jmp .find_diff

CHECK_SEC:
    test cl, cl
    je EQUAL
    jmp RETURN_DIFF

EQUAL:
    mov eax, 0
    jmp FINISH

RETURN_DIFF:
    mov eax, ebx
    sub eax, ecx
    jmp FINISH


FINISH:
    mov esp, ebp
    pop ecx
    pop ebx
    pop ebp
    ret

