            .syntax     unified
            .cpu        cortex-m4
            .text
            .thumb_func
            .align       2

            //returns unsigned 32 bit int with value 10
            .global Ten32
Ten32:      LDR R0,=10
            BX LR

            //returns unsigned 63 bit int with value 10
            .global Ten64
Ten64:      LDR R0,=10
            LDR R1,=0 //fills most significant bits with 0
            BX LR

            //increments and returns unsigned 32 bit int
            .global Incr
Incr:       LDR R1,=1
            ADD R0,R0,R1 //add 1 to value and store in R0
            BX LR

            //calls rand and increments by one
            .global Nested1
Nested1:    PUSH {LR}
            BL rand //rand func call
            LDR R1,=1
            ADD R0,R0,R1 //increments by one
            POP {PC}

            //adds two random unsigned 32 bit ints
            .global Nested2
Nested2:    PUSH {LR,R4}
            BL rand //rand func call
            MOV R4, R0
            BL rand
            ADD R0,R0,R4 //add the two rands
            POP {LR,R4}
            BX LR

            //prints input value and value + 1
            .global PrintTwo
PrintTwo:   PUSH {LR,R4,R5}
            MOV R4,R0 //save input
            MOV R5,R1 //save input

            BL printf

            MOV R0,R4
            MOV R1,R5
            LDR R2,=1
            ADD R1,R1,R2 //add one to value

            BL printf

            POP {LR,R4,R5}
            BX LR

            .end

