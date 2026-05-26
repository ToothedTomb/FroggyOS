[bits 16]
[org 0x7c00]

start:
    ; Set up video mode
    mov ax, 0x0003
    int 0x10
    
    ; Print message
    mov si, message
    call print
    
    ; Hang
    cli
    hlt

print:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0e
    int 0x10
    jmp print
.done:
    ret

message db "Welcome To FroggyOS", 13, 10, 0

; Fill to 510 bytes and add boot signature
times 510 - ($ - $$) db 0
dw 0xaa55
