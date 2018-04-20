    .syntax     unified
    .cpu        cortex-m4
    .text
    .thumb_func
    .align       2

    .global     UseLDRB
UseLDRB: //copies a single byte 512 times
    .rept 512
    LDRB        R2,[R1],1
    STRB        R2,[R0],1
    .endr
    bx          LR

    .global     UseLDRH
UseLDRH: //copies a half-word 256 times
    .rept 256
    LDRH        R2,[R1],2
    STRH        R2,[R0],2
    .endr
    bx          LR

    .global     UseLDR
UseLDR: //copies a word 128 times
    .rept 128
    LDR        R2,[R1],4
    STR        R2,[R0],4
    .endr
    bx          LR

    .global     UseLDRD
UseLDRD: //copies a double word 64 times
    .rept 64
    LDRD        R2,R3,[R1],8
    STRD        R2,R3,[R0],8
    .endr
    bx          LR

    .global     UseLDMIA
UseLDMIA: //copies 512 bytes by running ldmia and stmia 11 times and then copies remaining 28 bytes
    PUSH        {R4-R11}

    .rept       11 //484 bytes
    LDMIA       R1!,{R2-R12}
    STMIA       R0!,{R2-R12}
    .endr

    LDMIA       R1,{R2-R8} //28 bytes
    STMIA       R0,{R2-R8}

    POP         {R4-R11}
    BX          LR

    .end

