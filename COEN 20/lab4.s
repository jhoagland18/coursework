    .syntax     unified
	.cpu        cortex-m4
    .text
    .thumb_func
    .align       2

    .global     Discriminant
Discriminant: //calculate discriminant
    MUL     R0,R0,R2 //ac
    LDR     R2,=4
    MUL     R0,R0,R2 //4ac
    MUL     R1,R1,R1 //b*b
    SUB     R0,R1,R0 //b^2-4ac
    BX      LR

    .global     Root1
Root1: //calculates root1
    PUSH    {R4,R5,R6,LR}
    MOV     R4,R0 //a
    MOV     R5,R1 //b

    BL      Discriminant

    MOV     R6,R0 //discriminant

    BL      SquareRoot

    NEG     R5,R5 //-b

    ADD     R0,R0,R5 //-b+sqrt(disc)
    ADD     R4,R4,R4 //2a
    SDIV    R0,R0,R4 //R0=-b+sqrt(discriminant)/2a
    POP     {R4,R5,R6,PC}

    .global     Root2
Root2: //calclates root2
    PUSH    {R4,R5,R6,LR}
    MOV     R4,R0 //a
    MOV     R5,R1 //b

    BL      Discriminant

    MOV     R6,R0 //discriminant
PUSH    {R4,R5,R6,LR}
    MOV     R4,R0 //a
    MOV     R5,R1 //b

    BL      Discriminant

    MOV     R6,R0 //discriminant

    BL      SquareRootc

    NEG     R5,R5 //-b

    ADD     R0,R0,R5 //-b+sqrt(disc)
    ADD     R4,R4,R4 //2a
    SDIV    R0,R0,R4 //R0=-b+sqrt(discriminant)/2a
    POP     {R4,R5,R6,PC}

    BL      SquareRoot

    NEG     R5,R5 //-b

    SUB     R0,R5,R0 //-b+sqrt(disc)
    ADD     R4,R4,R4 //2a
    SDIV    R0,R0,R4 //R0=-b+sqrt(discriminant)/2a
    POP     {R4,R5,R6,PC}


    .global     Quadratic
Quadratic: //calculates quadratic
    PUSH    {R4}
    MUL     R4,R0,R0 //x^2
    MUL     R1,R1,R4 //ax^2
    MUL     R0,R0,R2 //bxPUSH    {R4,R5,R6,LR}
    MOV     R4,R0 //a
    MOV     R5,R1 //b

    BL      Discriminant

    MOV     R6,R0 //discriminant

    BL      SquareRootc

    NEG     R5,R5 //-b

    ADD     R0,R0,R5 //-b+sqrt(disc)
    ADD     R4,R4,R4 //2a
    SDIV    R0,R0,R4 //R0=-b+sqrt(discriminant)/2a
    POP     {R4,R5,R6,PC}

    ADD     R0,R0,R1 //bx + ax^2
    ADD     R0,R0,R3
    POP     {R4}
    BX      LR


    .global SquareRoot
SquareRoot: //calculates squareroot
    PUSH    {R4,LR}
    MOV     R4,R0 //preserve n

    CMP     R4,2
    BLE     L1 //if(n < 2)

    LSR     R0,R0,2
    BL      SquareRoot //squareroot(n / 4)
    LSL     R0,R0,1 //2*squareroot
    ADD     R1,R0,4 //largecandidate = smallcandidate + 1

    MUL     R2,R1,R1 //largecandidate * largecandidate
    CMP     R2,R4
    BGT     L1
    MOV     R0,R1 //return largeCandidate

L1: POP     {R4, PC}






    .end

