; Checksum.asm
; 64-bit assembly function for custom checksum calculation

; Function prototype
; uint64_t CustomChecksumAsm(const char* data, uint64_t length, uint64_t initialSeed, uint64_t rotateAmount, uint64_t additiveFactor, uint64_t rotateCount)

.code
CustomChecksumAsm PROC
    ; Parameters:
    ; RCX = data pointer (const char* data)
    ; RDX = length (uint64_t length)
    ; R8  = initialSeed (uint64_t initialSeed)
    ; R9  = rotateAmount (uint64_t rotateAmount)
    ; stack + 32 = additiveFactor (uint64_t additiveFactor)
    ; stack + 40 = rotateCount (uint64_t rotateCount)

    ; Load parameters
    mov     r10, R8                ; r10 = initialSeed (checksum)
    mov     r11, R9                ; r11 = rotateAmount
    mov     r12, QWORD PTR [rsp+32] ; r12 = additiveFactor
    mov     r13, QWORD PTR [rsp+40] ; r13 = rotateCount

    ; Loop through each character in data
LoopStart:
    test    RDX, RDX               ; Check if length is 0
    jz      EndC                    ; Exit if no more data

    ; XOR the checksum with the current byte in data
    movzx   rax, BYTE PTR [RCX]    ; Load current byte
    xor     r10, rax               ; checksum ^= byte

    ; Perform multiple rotations based on rotateCount
    mov     rbx, r13               ; Temporary counter for rotations
RotateLoop:
    test    rbx, rbx               ; Check if rotateCount is 0
    jz      SkipRotation           ; Skip if no rotation needed

    rol     r10, cl                ; Rotate left by rotateAmount
    dec     rbx                    ; Decrease rotateCount
    jmp     RotateLoop             ; Repeat until rotateCount reaches 0
SkipRotation:

    ; Add additiveFactor to checksum
    add     r10, r12               ; checksum += additiveFactor

    ; Move to next byte and decrease length
    inc     RCX                    ; Move to next character
    dec     RDX                    ; Decrease length
    jmp     LoopStart              ; Repeat loop

EndC:
    mov     RAX, r10               ; Result in RAX (return value)
    ret
CustomChecksumAsm ENDP
END
