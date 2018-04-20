        .syntax     unified
        .cpu        cortex-m4
        .text
        .thumb_func
        .align       2

one:    .float      1.0
zero:   .float      0.0

        .global     FloatPoly
FloatPoly:

        VLDR        S1,one //x2n
        VLDR        S2,zero //sum

        LDR         R2,=0 //index

loop1:
        CMP         R2,R1 //R0-R1
        BGE         endloop1

        VLDMIA      R0!,{S4}
        VMLA.F32    S2,S4,S1
        VMUL.F32    S1,S1,S0

        ADD         R2,R2,1
        B           loop1

endloop1:
        VMOV        S0,S2
        BX          LR



        .global     FixedPoly
FixedPoly:
        PUSH        {R4,R5,R6,R7,R8}
        LDR         R3,=0b00000000000000010000000000000000 //x2n
        LDR         R4,=0 //sum
        LDR         R5,=0 //index
loop2:
        CMP         R5,R2 //R0-R1
        BGE         endloop2

        LDMIA       R1!,{R6}

//coef[i] * x2n
        SMULL       R7,R8,R3,R6 //R7.R8 = Q32.32 product of x2n and value
        LSRS.N      R7,R7,16 //extract middle 32 bits
        ORR         R7,R7,R8,LSL 16
        ADD         R4,R4,R7

//x2n * x
        SMULL       R7,R8,R0,R3 //R7.R8 = Q32.32 product of x2n and value
        LSRS.N      R7,R7,16 //extract middle 32 bits
        ORR         R7,R7,R8,LSL 16
        MOV         R3,R7

        ADD         R5,R5,1
        B           loop2

endloop2:
        MOV         R0,R4
        POP         {R4,R5,R6,R7,R8}
        BX          LR
        .end

