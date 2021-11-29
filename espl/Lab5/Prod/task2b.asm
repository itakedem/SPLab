%include "cmpstr.asm"
%include "utoa_s.asm"

section .data
    lineSpace: db 10, 0
    flagW: db '-w', 0
    flagWs: db '-ws', 0
    wordGloble: db 0
    endOfWord: db 0, 0

    open_file_err: db 'Failed to open the file', 10, 0
    open_file_err_len: equ  $ - open_file_err

    close_file_err: db 'Failed to close the file', 10, 0
    close_file_err_len: equ  $ - close_file_err

    read_from_file_err: db 'Failed to read from file', 10, 0
    read_from_file_err_len: equ  $ - read_from_file_err

    write_to_File_err: db 'Failed to write to file', 10, 0
    write_to_File_err_len: equ  $ -  write_to_File_err

section .bss
    int_str_repr: resb 11                            ; save place for convertion of int to str

section .text
    global _start
    global read
    global write
    global open
    global close
    global strlen
    

_start:

    mov eax, [esp]                                   ; argc
    mov ebx, esp                                     ; argv @esp+4
    add ebx, 4h                                      ;4h means write - to ask Tali why we're adding it

    push ebx
    push eax

    call	main

.exit:
    mov ebx,eax
    mov	eax,1
    int 0x80

%define ARGC dword [ebp + 8]
%define ARGV dword [ebp + 12]
%define BUFFER_SIZE dword 50
%define flag byte [ebp-55]
%define fileName dword [ebp-59]
%define wordsCounter dword [ebp-63]
%define wordToCount dword [ebp-67]

main: 
    enter 67, 0         ; 50 bytes for buffer starting at ebp-50 
                        ; 4 to file fd
                        ; 1 flag
                        ; 4 fileName
                        ; 4 wordsCounter
                        ; 4 wordToCount

    mov ebx, ARGV
    add ebx, 4              ; move ebx to point on argv[1] 
    mov flag, 0             ; default for no flags
    mov wordsCounter, 1     ; init counter

    push flagW
    push dword [ebx]  
    call cmpstr        
    pop ecx
    pop ecx 
    cmp eax, 0
    je .rememberFlagW

    push flagWs
    push dword [ebx]  
    call cmpstr        
    pop ecx
    pop ecx 
    cmp eax, 0
    je .rememberFlagWs

    jmp .openFile

.rememberFlagW:
    add ebx, 4              ; ebx points to file name
    mov flag, 1             ; remember flag -w
    jmp .openFile

.rememberFlagWs:
    mov flag, 2             ; remember flag -ws

    add ebx, 4              ; move ebx to point on wordToCount
    mov edx, [ebx]          ; put the address of the word in edx
    mov [wordGloble], edx
    add ebx, 4              ; ebx points to file name

.openFile:
    mov edx, [ebx]          ; now ebx contains the file name
    mov fileName, edx       ; store file name
    push 0                  ;read only flag
    push edx
    call open               ; fd at eax if succeed
    pop ebx
    pop ebx                 ; clean stack

    mov [ebp - 54], eax    ; save fd on stack
    cmp eax, 0
    jle .finishError            ; if eax <= 0 finish with error

.readFile:
    push BUFFER_SIZE        ; arg1 for read - size to read
    mov ebx, ebp
    sub ebx, 50             ; ebx points buffer start  
    push ebx                ; arg2 for read - buffer
    push dword [ebp - 54]   ; arg3 for read - fd
    call read
    pop ebx
    pop ebx
    pop ebx                 ; clean stack

    cmp eax, -1             ; check if error
    jle .finishError

    cmp eax, 0               ; check if read something
    jne .act

.finishRead:
    push dword [ebp - 54]  ; arg1 to close - fd
    call close
    pop ebx                 ; clean stack
    cmp flag, 1
    jge .printCounter
    jmp .finishSuccess

.act:
    cmp flag, 1
    je .countWords         
    cmp flag, 2
    je .countWordToCount 

;else - no flag so printing file
.printBuffer:
    push eax                ; arg1 to write - num of bytes (got from read)
    mov ebx, ebp
    sub ebx, 50             ;ebx points at buffer start
    push ebx                ; arg2 to write - buffer
    push 1                  ; arg3 to write - stdout fd 
    call write
    pop ebx
    pop ebx
    pop ebx                 ; clean stack

    cmp eax, 0              ; check if wrote something
    jle .finishError

    jmp .readFile           ; continue reading

.countWords:
    push eax                 ; num of bytes (got from read)
    mov ebx, ebp
    sub ebx, 50              ;beginning of the buffer
    push ebx                  ; buffer address
    mov ebx, ebp
    sub ebx, 63                 ;to get the wordsCounter
    push ebx                   ; counter address
    call countWords
    pop ebx
    pop ebx
    pop ebx                                          ; clean stack from args

    jmp .readFile

.countWordToCount:
    mov ebx, ebp
    sub ebx, 67                 ;wordToCount pointer
    push ebx
    push eax                 ; num of bytes (got from read)
    mov ebx, ebp
    sub ebx, 50
    push ebx                  ; buffer address
    mov ebx, ebp
    sub ebx, 63
    push ebx                   ; counter address
    call countWordToCount
    pop ebx
    pop ebx
    pop ebx
    pop ebx                                          ; clean stack from args

    jmp .readFile

.finishError:
    mov eax, 1              ; default error return
    jmp _start.exit             ; return

.printCounter:
    mov eax, wordsCounter
    mov ebx, int_str_repr
    push ebx
    push eax                ; arg1 to otua_s - counter
    call utoa_s             ; calling utoa to get string of our counter
    pop ecx                 ; stack clean
    pop ecx

    ;counts number of digits in counter as string
    push int_str_repr
    call strlen
    pop edx

    push eax                ; arg1 to write - num of bytes 
    push dword int_str_repr                ; arg2 to write - counter as string
    push 1                  ; arg3 to write - stdout fd 
    call write
    pop ebx
    pop ebx
    pop ebx                 ; clean stack

    cmp eax, 0              ; check if wrote something
    jle .finishError

    .finishSuccess:
    ; print line space
    push dword 1                ; arg1 to write - num of bytes 
    push dword lineSpace                ; arg2 to write - counter as string
    push 1                  ; arg3 to write - stdout fd 
    call write
    pop ebx
    pop ebx
    pop ebx                 ; clean stack
    
    mov eax, 0              ; default success return

.return:
    leave
    ret

read:
	push ebp 	;stack maintenance
	push ebx 	;keeping prev values
	push ecx 	;keeping prev values
	push edx 	;keeping prev values
	mov ebp, esp	 ;stack maintenance
	mov eax, 3			;read system call num
	mov ebx, [ebp + 20]	;file descriptor arg
	mov ecx, [ebp + 24]	;input buffer arg
	mov edx, [ebp + 28]	;buffer size arg
	int 0x80			;call kernel
    cmp eax, -1
    jle .readFailed

.FINISH_READ:
	mov esp, ebp	 ;stack maintenance
	pop edx			;return prev scope values
	pop ecx			;return prev scope values
	pop ebx			;return prev scope values
	pop ebp 		;stack maintenance
	ret 			;stack maintenance

.readFailed:
    push dword  read_from_file_err_len               ; buffer pointer
    push dword  read_from_file_err                   ; number of bytes read to buffer
    push 1                                           ; fd to stdout
    call write
    pop ebx 
    pop ebx 
    pop ebx                                          ; clean stack of arguments
    jmp main.finishError

write:
	push ebp 	;stack maintenance
	push ebx 	;keeping prev values
	push ecx 	;keeping prev values
	push edx 	;keeping prev values
	mov ebp, esp	 ;stack maintenance
	mov eax, 4			;write system call num
	mov ebx, [ebp + 20]	;file descriptor arg
	mov ecx, [ebp + 24]	;output buffer arg
	mov edx, [ebp + 28]	;count of bytes to sent arg
	int 0x80			;call kernel
    cmp eax, -1
    jle .writeFailed

.FINISH_WRITE:
	mov esp, ebp	 ;stack maintenance
	pop edx			;return prev scope values
	pop ecx			;return prev scope values
	pop ebx			;return prev scope values
	pop ebp 		;stack maintenance
	ret 			;stack maintenance

.writeFailed:
    push dword  write_to_File_err_len                ; buffer pointer
    push dword  write_to_File_err                    ; number of bytes read to buffer
    push 1                                           ; fd to stdout
    call write
    pop ebx 
    pop ebx 
    pop ebx                                          ; clean stack of arguments
    jmp main.finishError

open:
    enter 0, 0
    push ebx           ; save regs
    push ecx           ; save regs
    mov eax, 5h        ; setup system call
    mov ebx, [ebp + 8] ; setup system call
    mov ecx, [ebp + 12] ; setup system call
    int 0x80           ; sys call
    cmp eax, -1
    jle .openFailed
    pop ecx            ; restore regs
    pop ebx            ; restore regs
    leave
    ret

.openFailed:
    mov ebx, open_file_err
    push dword  open_file_err_len                    ; buffer pointer
    push dword  open_file_err                        ; number of bytes read to buffer
    push 1                                           ; fd to stdout
    call write
    pop ebx 
    pop ebx 
    pop ebx                                          ; clean stack of arguments
    jmp main.finishError

close:
    enter 0, 0
    push ebx           ; save regs
    mov eax, 6h        ; setup system call
    mov ebx, [ebp + 8]
    int 0x80           ; sys call
    cmp eax, -1
    jle .closeFailed
    pop ebx            ; restore regs
    leave
    ret                ; clear args from stack and return

.closeFailed:
    push dword  close_file_err_len                   ; buffer pointer
    push dword  close_file_err                       ; number of bytes read to buffer
    push 1                                           ; fd to stdout
    call write
    pop ebx 
    pop ebx 
    pop ebx                                          ; clean stack of arguments
    jmp main.finishError

strlen:
    enter 0, 0
    push ebx
    mov eax, -1

strlen_body:
    inc eax            ; calc offset
    mov ebx, eax
    add ebx, [ebp+8]   ; take str from stack
    mov bl, BYTE [ebx] ; get byte at adress of ebx
    test bl, bl        ; check if byte is \0
    jne strlen_body      ; if not \0 cont to next byte
    pop ebx
    leave
    ret

%define COUNTER dword [ebp + 8]
%define BUFFER_P dword [ebp + 12]
%define BUF_SIZE dword [ebp + 16]
countWords:
    enter 0,0
    push ecx
    push ebx
    push eax

    mov ecx, BUF_SIZE
    mov ebx, BUFFER_P

.countLoop:
    call .incrementIfNeeded
    inc ebx                                          ; move pointer
    loop .countLoop
    jmp .finish

.incrementIfNeeded:
    cmp [ebx], byte 10                               ; 10 in ascii means new line
    je .increment                                    
    cmp [ebx], byte 32                               ; 32 in ascii means space, new word
    je .increment
    
    ret

.increment:
    mov eax, COUNTER                                 ; get counter adress
    inc dword [eax]
    ret

.finish:
    pop eax
    pop ebx
    pop ecx
    leave
    ret

%define COUNTER2 dword [ebp + 8]
%define BUFFER_P2 dword [ebp + 12]
%define BUF_SIZE2 dword [ebp + 16]
%define WORD_TO_COUNT dword [ebp + 20]

countWordToCount:
    enter 0,0
    push edx
    push ecx
    push ebx
    push eax

    mov ecx, BUF_SIZE2
    mov ebx, BUFFER_P2
    mov [edx], ebx            ; save start of word to compare

.countWordToCountLoop:
    call .dealNextChar
    loop .countWordToCountLoop
    jmp .finishWordToCount

.dealNextChar:
    cmp [ebx], byte 32
    je .incrementIfNeededWordToCount
    inc ebx                     ; next char if not space
    ret

.incrementIfNeededWordToCount:
                                ;padding with \0 both wordToCount and wordToCompare

    mov eax, [wordGloble]

    

    push eax                
    push dword [edx]       ; where does it end ?
    call cmpstr             ; ?????????????????????????????????????????
    pop edx
    pop edx 
    
    inc ebx                     ; next char if last one was space
    mov [edx], ebx            ; move edx to point the start of the next word

    cmp eax, 0
    je .incrementWordToCount
    ret



.incrementWordToCount:
    mov eax, COUNTER2                                 ; get counter adress
    inc dword [eax]
    ret

.finishWordToCount:
    pop eax
    pop ebx
    pop ecx
    pop edx
    leave
    ret