section .text
global branch_experiment

branch_experiment:
    push rbx
    push r12
    push r13
    push r14
    push r15

    ; Save pointer arguments
    mov r12, rdi  ; First argument (pointer to taken_cycles)
    mov r13, rsi  ; Second argument (pointer to fall_through_cycles)

    ; Measure taken branch
    rdtsc
    shl rdx, 32
    or rax, rdx
    mov r14, rax  ; Store initial time in r14

    mov ecx, 100000000  ; Number of iterations
.loop_taken:
    xor eax, eax
    test eax, eax
    jz .taken
    ; This should never execute
    int3
.taken:
    dec ecx
    jnz .loop_taken

    rdtsc
    shl rdx, 32
    or rax, rdx
    sub rax, r14
    mov [r12], rax  ; Store taken branch cycles directly to memory

    ; Measure fall-through branch
    rdtsc
    shl rdx, 32
    or rax, rdx
    mov r14, rax  ; Store initial time in r14

    mov ecx, 100000000  ; Number of iterations
.loop_fall:
    mov eax, 1
    test eax, eax
    jz .not_taken
    ; This should always execute (fall-through)
    nop
.not_taken:
    dec ecx
    jnz .loop_fall

    rdtsc
    shl rdx, 32
    or rax, rdx
    sub rax, r14
    mov [r13], rax  ; Store fall-through branch cycles directly to memory

    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    ret
