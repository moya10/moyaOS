[org 0x7c00]                        
KERNEL_LOCATION equ 0x1000
                                    

mov [BOOT_DISK], dl                 

                                    
xor ax, ax                          
mov es, ax
mov ds, ax
mov bp, 0x8000
mov sp, bp

mov bx, KERNEL_LOCATION
mov dh, 20              

mov ah, 0x02
mov al, dh 
mov ch, 0x00
mov dh, 0x00
mov cl, 0x02
mov dl, [BOOT_DISK]
int 0x13                ; no error management, do your homework!

                                    
mov ah, 0x0
mov al, 0x3
int 0x10                ; text mode


CODE_SEG equ GDT_code - GDT_start
DATA_SEG equ GDT_data - GDT_start

cli                     ; disable all interrupts
lgdt [GDT_descriptor]   ; call the GDT
mov eax, cr0            ; change the bit of cr0 to switch
or eax, 1               ; to protected mode, using the 
mov cr0, eax            ; GP registers
jmp CODE_SEG:start_protected_mode   ; jump to other segment

jmp $
                                    
BOOT_DISK: db 0

GDT_start:
    GDT_null:               ; empty descriptor
        dd 0x0
        dd 0x0

    GDT_code:
        dw 0xffff           ; 16 bits of the limits
        dw 0x0              ; 24 bits of the base
        db 0x0
        db 0b10011010       ; type flag properties
        db 0b11001111
        db 0x0

    GDT_data:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10010010
        db 0b11001111
        db 0x0

GDT_end:

GDT_descriptor:
    dw GDT_end - GDT_start - 1  ; size of the GDT
    dd GDT_start                ; start of the GDT


[bits 32]
start_protected_mode:       ; write to videoMemory directly
    mov ax, DATA_SEG        ; set up segment registers and stack
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	mov ebp, 0x90000		; 32 bit stack base pointer
	mov esp, ebp

    jmp KERNEL_LOCATION     ; jump to kernel location

                                     
 
times 510-($-$$) db 0              
dw 0xaa55
