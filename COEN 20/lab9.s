    .syntax     unified
	.cpu        cortex-m4
    .text
    .thumb_func
    .align       2

// uint32_t  Add1(uint32_t x) ;

        .global     SIMD_USatAdd
SIMD_USatAdd:
        PUSH    {R4-R12}
        BFI     R2,R2,8,8
        BFI     R2,R2,16,16

loop:   CMP     R1,40
        BLT      cleanup
        //LDR     R3,[R0]
        LDMIA   R0,{R3-R12} //get next 40 bytes

        //add (saturate) 40 bytes
        UQADD8  R3,R3,R2
        UQADD8  R4,R4,R2
        UQADD8  R5,R5,R2
        UQADD8  R6,R6,R2
        UQADD8  R7,R7,R2
        UQADD8  R8,R8,R2
        UQADD8  R9,R9,R2
        UQADD8  R10,R10,R2
        UQADD8  R11,R11,R2
        UQADD8  R12,R12,R2

        //store 40 bytes
        STMIA   R0!,{R3-R12}
        SUB     R1,R1,40
        B       loop

cleanup:
        CBZ     R1,done
        LDR     R3,[R0] //load 4 bytes
        UQADD8  R3,R3,R2   //add (saturate) 4 bytes
        STR     R3,[R0],4 //store 4 bytes
        SUB     R1,R1,4 //decrement index
        B       cleanup

done:   POP     {R4-R12}
        BX      LR

        .end

