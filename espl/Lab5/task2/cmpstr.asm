section .text
    global cmpstr

; Compare 2 strings in a lexicographical way
; Assumption: both strings end with \0
cmpstr:
    push ebp                     ; save caller's ebp
    push ebx                     ; save caller's ebx
    push ecx                     ; save caller's ecx

    mov	ebp, esp                 ; let ebp hold the stack pointer address
    mov	eax,-1                   ; init eax with -1


; iter over both string until finish or find diff
.do_comp:
    add eax, 1                   ; move string offset

    mov ebx, eax                 ; add offset to ebx - for first string
    mov ecx, eax                 ; add offset to ecx - for second string

    add ebx, [ebp + 16]          ; get current 4 chars from first string
    add ecx, [ebp + 20]          ; get current 4 chars from second string

    movzx	ebx, BYTE [ebx]      ; extract only first char from ebx
    movzx	ecx, BYTE [ecx]      ; extract only first char from ecx

    test bl , bl                 ; check if current char in str1 is '\0'
    je first_string_ended        ; if the char is \0 handle end of string1

    test cl , cl                 ; check if current char in str2 is '\0'
    je second_only_string_ended  ; if the char is \0 handle end of string2 (but not end of str1)

    cmp ebx, ecx                 ; comp current char
    jg first_bigger
    jl second_bigger

    jmp .do_comp                 ; continue iter

; ----------------------------- string end handle -----------------------------
first_string_ended:
    test cl , cl                 ; check if current char in str2 is '\0'
    je both_strings_ended        ; if the char is \0 both strings are equal
    jmp second_bigger

second_only_string_ended:
    jmp first_bigger

both_strings_ended:
    mov eax, 0                   ; set return value to be - 0 (equal)
    jmp FINALIZE

; ---------------------------- string bigger handle ---------------------------
second_bigger:
    mov eax, 2                   ; set return value to be - 2 (second string is bigger)
    jmp FINALIZE

first_bigger:
    mov eax, 1                   ; set return value to be - 1 (first string is bigger)
    jmp FINALIZE

FINALIZE:
    mov esp, ebp                 ; move stack pointer to where it was before exe cmpstr

    pop ecx                      ; return ecx to caller's state
    pop ebx                      ; return ebx to caller's state
    pop ebp                      ; return ebp to caller's state

    ret                          ; return to caller