    .syntax     unified
	.cpu        cortex-m4
    .text
    .thumb_func
    .align       2

    .global     CallReturnOverhead
CallReturnOverhead: //run time performance comparison
    BX      LR

    .global         ReverseBits
ReverseBits: //reverse 32 bit datatype
    .rept   32
    LSLS    R0,R0,1 //pop
    RRX     R1,R1 //push
    .endr
    MOV     R0,R1

    BX      LR

    .global    ReverseBytes
ReverseBytes: //reverse bytes of a 4 byte datatype
    PUSH    {R4,LR}
    LSL     R1,R0,24 //1st byte

    LSR     R2,R0,8 //2nd byte
    LSL     R2,R2,24
    LSR     R2,R2,8

    LSR     R3,R0,16 //3rd byte
    LSL     R3,R3,24
    LSR     R3,R3,16

    LSR     R4,R0,24 //4th byte

    LDR     R0,=0 //add them all together
    ADD     R0,R0,R1
    ADD     R0,R0,R2
    ADD     R0,R0,R3
    ADD     R0,R0,R4

    POP     {R4,PC}
    .end
