    .syntax     unified
	.cpu        cortex-m4
    .text
    .thumb_func
    .align       2

// uint32_t  Add1(uint32_t x) ;

        .global     DeleteItem
DeleteItem:
        PUSH    {R4,LR}
        CPY     R3,R2 //copy index to iterator (R3)
        SUB     R1,R1,1

loopdelete:
        CMP     R3,R1 //check if iterator greater than || equal to items length
        BGE     endloopdelete //if iterator is greater || equal, branch to endloop
        ADD     R3,R3,1
        LDR     R4,[R0,R3,LSL 2] //next value of array (arr index + iterator)
        SUB     R3,R3,1
        STR     R4,[R0,R3, LSL 2] //store next value of array at current index
        ADD     R3,R3,1 //increment iterator
        B       loopdelete

endloopdelete:
        POP     {R4,PC}


        .global     InsertItem
InsertItem:
        PUSH    {R4,R5,R6,LR}
        CPY     R4,R1 //iterator = end of array index

loopinsert:
        CMP     R4,R2 //compare iterator to index
        BLE     endloopinsert //if iterator is less than or equal to index

        SUB     R5,R4,1 //iterator-1
        LDR     R6,[R0,R5,LSL 2] //load value at iterator -1
        STR     R6,[R0,R4,LSL 2] //store value to iterator
        SUB     R4,R4,1 //decrement iterator
        B       loopinsert

endloopinsert:
        STR     R3,[R0,R4,LSL 2] //store new value to iterator (should = index)
        POP     {R4,R5,R6,PC}
        .end
