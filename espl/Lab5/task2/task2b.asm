%include "cmpstr.asm"
%include "task1b.asm"

section .data
    num_lines_str: db 'Number of lines: ', 0
    num_lines_str_len: equ  $ - num_lines_str        ; calc string size
    l_opt: db '-l', 0
    endl: db 10, 0

    open_file_err: db 'Failed to open the file', 10, 0
    open_file_err_len: equ  $ - open_file_err

    close_file_err: db 'Failed to close the file', 10, 0
    close_file_err_len: equ  $ - close_file_err

    read_from_file_err: db 'Failed to read from file', 10, 0
    read_from_file_err_len: equ  $ - read_from_file_err

    write_to_File_err: db 'Failed to write to file', 10, 0
    write_to_File_err_len: equ  $ -  write_to_File_er

section .bss
    int_str_repr: resb 11                            ; save place for convertion of int to str

section .text
    global _start

_start:

    mov eax, [esp]                                   ; argc
    mov ebx, esp                                     ; argv @esp+4
    add ebx, 4h                                      ;4h means write - to ask Tali why we're adding it

    push ebx
    push eax

    call	main
    jmp .exit



.exit:
    mov ebx,eax
    mov	eax,1
    int 0x80

%define ARGC dword [ebp + 8]
%define ARGV dword [ebp + 12]
%define BUFFER_SIZE dword 50
%define O_RDONLY dword 0
%define LINE_NUM dword [ebp - 58]
%define IS_COUNT_LINES byte [ebp - 59]
%define PATH_TO_F dword [ebp - 53]

main: 
    enter 63, 0                 ; reserve 50 bytes fro buffer starting at ebp-50
                                ; 4 to save fd, 4 to save num of lines, 
                                ; 1 for is '-w' option flag, and 4 for path to file pointer

    mov LINE_NUM, 0                                  ; put 0 in stack in case we need to count lines
    mov IS_COUNT_LINES, 0                            ; flag for -w option

parse_args:
    mov ecx, ARGC                                       
    sub ecx, 1                                       ; now ecx number of cmd args

    mov ebx, ARGV

.move_ebx:
    add ebx, 4                                       ; now ebx points at argv[1] - which is the file arg

    push l_opt                                       ; push l as first arg to cmpstr
    push dword [ebx]                                 ; push [ebx] as second arg to cmpstr

    call cmpstr

    pop edx
    pop edx                                          ; clear arguments

    cmp eax, 0
    je .found_l
    jmp .fount_not_l