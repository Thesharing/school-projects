; Copyright @ 2015 Hong Zhilong
; Homework of Microcomputer Principle and Interface Technology
;
;
; DATA STRCUTURE
; STU       STRUC
; NAME      DB 13 DUP (?)
; CLASS     DB 13 DUP (?)
; NO        DB 13 DUP (?)
; NUM       DB ?
; DOT       DB ?
; DEC       DB ?
; STU       ENDS

DATA        SEGMENT
INFO        DB 2000 DUP (0) ;The data.'Cause struct has no array, use a block to "array" it
STU         DW 40 DUP (0)   ;The pointer of data
NUM         DB 0            ;The num of data

LEN         EQU 02AH        ;Constant Define:Length of one data
CLASSPOS    EQU 00DH        ;The relative postion of each part
NOPOS       EQU 01AH
NUMPOS      EQU 027H
DOTPOS      EQU 028H
DECPOS      EQU 029H
MAXLEN      EQU 0AH

CURRENT     DW 0            ;Current Pointer
LOCATION    DW 0            ;LOCATION is a pointer to transport the data into proc
LOCATION1   DW 0
NUMOUT1     DB 0            ;NUMOUT & DOTOUT are used to transport a number in&out proc 
DOTOUT      DB 0
NUMOUT2     DB 0

SUM         DW 0            ;SUM & DECSUM are used to calc the average
DECSUM      DW 0

SEC1        DB 0            ;SECn are used to count sections
SEC2        DB 0
SEC3        DB 0
SEC4        DB 0
SEC5        DB 0

CMP1        DW 0
CMP2        DW 0
CMPRES      DB 0

FUNC_WRD    DB 'Welcome to Student Roll System.$'
FUNC1_WRD   DB '1.Input a Student Info.$'
FUNC2_WRD   DB '2.List All Student Info.$'
FUNC3_WRD   DB '3.Rank the Students by score.$'
FUNC4_WRD   DB '4.Rank the Students by No.$'
FUNC5_WRD   DB '5.Statistic the Average.$'
FUNC6_WRD   DB '6.Statistic the Score Section.$'
FUNC7_WRD   DB '7.Quit.$'
INPUT_WRD   DB 'Input the info of student:$'
INPU1_WRD   DB 'Name:$'
INPU2_WRD   DB 'Class:$'
INPU3_WRD   DB 'No.:$'
INPU4_WRD   DB 'Score:$'
ERRO1_WRD   DB 'Wrong Input.$'
AVE_WRD     DB 'The Average Score is:$'
END_WRD     DB 'Thank you.$'
EMPTY_WRD   DB 'No Records.$'
DEBUG_WRD   DB 'DEBUG HERE.$'
SEC1_WRD    DB '<60:$'
SEC2_WRD    DB '60-70:$'
SEC3_WRD    DB '70-80:$'
SEC4_WRD    DB '80-90:$'
SEC5_WRD    DB '90-100:$'

CHOICE      DW CHOICE1,CHOICE2,CHOICE3,CHOICE4,CHOICE5,CHOICE6,CHOICE7

DATA        ENDS
STACK       SEGMENT STACK
            DW 200 DUP (0)
STACK       ENDS
CODE        SEGMENT
            ASSUME CS:CODE,DS:DATA,SS:STACK
START:      MOV AX,DATA
            MOV DS,AX

            LEA DX,FUNC_WRD  ;Print the main menu
            MOV LOCATION,DX
            CALL PUTS
L1:         LEA DX,FUNC1_WRD
            MOV LOCATION,DX
            CALL PUTS
            LEA DX,FUNC2_WRD
            MOV LOCATION,DX
            CALL PUTS
            LEA DX,FUNC3_WRD
            MOV LOCATION,DX
            CALL PUTS
            LEA DX,FUNC4_WRD
            MOV LOCATION,DX
            CALL PUTS
            LEA DX,FUNC5_WRD
            MOV LOCATION,DX
            CALL PUTS
            LEA DX,FUNC6_WRD
            MOV LOCATION,DX
            CALL PUTS
            LEA DX,FUNC7_WRD
            MOV LOCATION,DX
            CALL PUTS
            
            CALL CHOICE_INPUT ;Input a choice
            
            MOV BL,NUMOUT1
            DEC BL
            MOV BH,0
            SHL BX,1
            JMP CHOICE[BX]
            
CHOICE1:    CALL INPUT            
            JMP L1
            
CHOICE2:    CALL OUTPUTALL
            JMP L1
             
CHOICE3:    CALL RANK
            JMP CHOICE2
            
CHOICE4:    CALL RANKS
            JMP CHOICE2            
            
CHOICE5:    CALL AVERAGE
            LEA DX,AVE_WRD ;Print Average Msg
            MOV AH,09H
            INT 21H
            CALL PUT_NUM
            MOV DL,0DH
            MOV AH,02H
            INT 21H
            MOV DL,0AH
            INT 21H
            JMP L1
            
CHOICE6:    CALL SECTION
            JMP L1
            
CHOICE7:    LEA DX,END_WRD
            MOV LOCATION,DX
            CALL PUTS
            MOV AH,4CH
            INT 21H



;FUNCTION to input a choice and output the choice via numout1
CHOICE_INPUT PROC NEAR
            PUSH AX
            
CHOICE_INPUT_START:
            MOV AH,1 ;Input a char
            INT 21H
            CMP AL,'1'
            JB CHOICE_INPUT_ERR
            CMP AL,'7'
            JA CHOICE_INPUT_ERR
            SUB AL,'0'
            MOV NUMOUT1,AL
            
            MOV DL,0DH  ;Print another line
            MOV AH,02H
            INT 21H
            MOV DL,0AH
            INT 21H

            POP AX
            RET
            
CHOICE_INPUT_ERR: ;Error msg
            CALL ERRPRINT
            JMP CHOICE_INPUT_START
CHOICE_INPUT ENDP                        



;FUNCTION to input an info of a student            
INPUT       PROC NEAR
            PUSH AX
            PUSH BX
            PUSH CX
            PUSH DX
            
            LEA DX,INPUT_WRD ;Display the tip
            MOV AH,09H
            INT 21H
            MOV AH,02H ;Print another line
            MOV DL,0DH
            INT 21H
            MOV DL,0AH
            INT 21H
            MOV AH,09H
            
            LEA DX,INPU1_WRD ;Display the tip
            INT 21H
            
            MOV AL,NUM ;Input the string NAME
            MOV BL,LEN
            MUL BL
            LEA DX,INFO
            ADD DX,AX
            MOV CURRENT,DX
            MOV LOCATION,DX
            CALL GETS 
            
            LEA DX,INPU2_WRD ;Display the tip
            MOV AH,09H
            INT 21H
            
            MOV DX,CURRENT ;Input the string CLASS
            ADD DX,CLASSPOS
            MOV LOCATION,DX
            CALL GETS_NUM
            
            LEA DX,INPU3_WRD ;Display the tip
            MOV AH,09H
            INT 21H
            
            MOV DX,CURRENT ;Input the string No.
            ADD DX,NOPOS
            MOV LOCATION,DX
            CALL GETS_NUM
            
            LEA DX,INPU4_WRD ;Display the tip
            MOV AH,09H
            INT 21H
            
            MOV DX,CURRENT ;Input the string NUM
            ADD DX,NUMPOS
            CALL GETNUM
            MOV BX,DX
            MOV DL,BYTE PTR NUMOUT1 ;Store the NUM into memory and NUMOUT
            MOV [BX],DL
            MOV DL,BYTE PTR DOTOUT
            MOV [BX+1],DL
            MOV DL,BYTE PTR NUMOUT2
            MOV [BX+2],DL
            
            MOV AX,CURRENT ;Output the result
            MOV LOCATION,AX
            CALL OUTPUT
            
            MOV AX,CURRENT ;Store the location to STU pointer
            LEA BX,STU
            ADD BL,NUM
            ADD BL,NUM
            MOV [BX],AX
            INC NUM
            POP DX
            POP CX
            POP BX
            POP AX
            RET
INPUT       ENDP


;Get a String char by char
GETS        PROC NEAR                   
            PUSH AX
            PUSH BX
            PUSH CX
            PUSH DX
            
            MOV BX,LOCATION             ;The string is stored at location
            MOV CX,MAXLEN
GETS_START: MOV AH,01H                  ;Input a char
            INT 21H
            CMP AL,13                   ;If another line
            JZ GETS_END
            MOV BYTE PTR [BX],AL        ;Store the char
            INC BX                      ;Inc the pointer
            DEC CX 
            CMP CX,0                    ;If reaches MAXLEN,END
            JZ GETS_END
            JMP GETS_START              ;Loop
GETS_END:   MOV BYTE PTR [BX],'$'       ;End of string                       
            
            MOV AH,02H                  ;Print another line
            MOV DL,0DH
            INT 21H
            MOV DL,0AH
            INT 21H
            
            POP DX
            POP CX
            POP BX
            POP AX
            RET
GETS        ENDP



;Get a string onetime
GETS_2      PROC NEAR 
            PUSH AX
            PUSH BX
            PUSH DX
            
            MOV DX,LOCATION
            MOV BX,DX ;Put the MAXLEN to head
            MOV BYTE PTR [BX],MAXLEN
            MOV AH,0AH
            INT 21H
            
            MOV BX,DX
            ADD BL,BYTE PTR [BX+1]
            ADD BX,2
            MOV BYTE PTR [BX],'$' ;Put the $ to end
            
            POP DX
            POP BX
            POP AX
            RET
GETS_2      ENDP



;Get a String char by char, if not the char is not a num, error
GETS_NUM    PROC NEAR 
            PUSH AX
            PUSH BX
            PUSH CX
            PUSH DX
            
            MOV BX,LOCATION
            MOV CX,MAXLEN
GETS_NUM_START: 
            MOV AH,01H ;Get Numbers as String
            INT 21H
            CMP AL,13
            JZ GETS_END
            CMP AL,'9' ; if not num,error
            JA GETS_NUM_ERR
            MOV BYTE PTR [BX],AL
            INC BX
            DEC CX
            CMP CX,0
            JZ GETS_END
            JMP GETS_NUM_START
            
GETS_NUM_END:   
            MOV BYTE PTR [BX],'$'                        
            
            MOV AH,02H
            MOV DL,0DH
            INT 21H
            MOV DL,0AH
            INT 21H
            
            POP DX
            POP CX
            POP BX
            POP AX
            RET
            
GETS_NUM_ERR:
            CALL ERRPRINT
            JMP GETS_NUM_START ;Return to start
            
GETS_NUM    ENDP



;Get a number with one decimal number
;The number is stored in NUMOUT1,DOTOUT,NUMOUT2
GETNUM      PROC NEAR 
            PUSH AX ;Only Allows Two Num and One Decimal
            PUSH BX
            PUSH CX
            PUSH DX
            
            MOV AL,0
            MOV DOTOUT,AL
  
GETNUM_L1:  MOV AH,01H  ;The first number
            INT 21H
            CMP AL,'0'
            JB GETNUM_ERR1
            CMP AL,'9'
            JA GETNUM_ERR1
            SUB AL,'0'
            MOV BL,AL
            
GETNUM_L2:  MOV AH,01H ;The second number
            INT 21H
            CMP AL,13
            JZ GETNUM_END
            CMP AL,'.'
            JZ GETNUM_DOT
            CMP AL,'0'
            JB GETNUM_ERR2
            CMP AL,'9'
            JA GETNUM_ERR2
            SUB AL,'0'
            MOV CL,AL
            MOV AL,BL
            MOV DL,10
            MUL DL
            ADD AL,CL
            MOV BL,AL
            
GETNUM_L3:  MOV AH,01H ;The third input
            INT 21H
            CMP AL,13 ;If another line
            JZ GETNUM_END
            CMP AL,'.'
            JZ GETNUM_DOT ;If dot
            JMP GETNUM_ERR3           
                        
GETNUM_DOT: MOV AL,01H ;The decimal part
            MOV DOTOUT,AL ;Only allows one decimal
            MOV AH,01H
            INT 21H
            CMP AL,'0'
            JB GETNUM_ERR4
            CMP AL,'9'
            JA GETNUM_ERR4
            SUB AL,'0'
            MOV CL,AL
            JMP GETNUM_END1
            
GETNUM_END: MOV NUMOUT1,BL ;End with no decimal
            MOV NUMOUT2,0
            MOV AH,02H ;Print another line
            MOV DL,10
            INT 21H
            MOV DL,13
            INT 21H
            
            POP DX
            POP CX
            POP BX
            POP AX
            RET

GETNUM_ERR1:CALL ERRPRINT
            JMP GETNUM_L1
            
GETNUM_END1:MOV NUMOUT1,BL ;End with decimal
            MOV NUMOUT2,CL
            MOV AH,02H ;Print another line
            MOV DL,10
            INT 21H
            MOV DL,13
            INT 21H
            
            POP DX
            POP CX
            POP BX
            POP AX
            RET
            
            ;Errors when input not number

GETNUM_ERR2:CALL ERRPRINT
            JMP GETNUM_L2
            
GETNUM_ERR3:CALL ERRPRINT
            JMP GETNUM_L3
            
GETNUM_ERR4:CALL ERRPRINT
            JMP GETNUM_DOT                                     
            
GETNUM      ENDP 



;Print the String with another line,the string is at LOCATION
PUTS        PROC NEAR 
            PUSH AX
            PUSH DX
            
            MOV DX,LOCATION;Print the string
            MOV AH,09H
            INT 21H
            
            MOV AH,02H ;Print another line
            MOV DL,0DH
            INT 21H
            MOV DL,0AH
            INT 21H
            
            POP DX
            POP AX
            RET
PUTS        ENDP



;Print the String without another line,the string is at LOCATION
PUT         PROC NEAR 
            PUSH AX
            PUSH DX
            
            MOV DX,LOCATION;Print the string
            MOV AH,09H
            INT 21H
            
            POP DX
            POP AX
            RET
PUT         ENDP



;Print the number, the number is at NUMOUT1,DOTOUT,NUMOUT2 
PUT_NUM     PROC NEAR
            PUSH AX
            PUSH BX
            PUSH DX
            
            MOV AL,NUMOUT1 ;Print The Integer
            CBW
            MOV BL,10
            DIV BL
            MOV BX,AX
            ADD BH,'0'
            ADD BL,'0'
            MOV AH,02H
            CMP BH,'0'
            JZ PUT_NUM_L0            
            MOV DL,BL
            INT 21H
PUT_NUM_L0: MOV DL,BH
            INT 21H
            JMP PUT_NUM_L2
            
PUT_NUM_L1: ADD AL,'0'
            CMP AL,9
            JBE PUT_NUM_L1
            MOV DL,AL
            MOV AH,02H
            INT 21H
            
PUT_NUM_L2: MOV AL,DOTOUT ;Whether to print the dot
            CMP AL,1
            JZ  PUT_NUM_DOT
            JMP PUT_NUM_END
            
PUT_NUM_DOT:MOV DL,'.' ;Print the dot and decimal
            MOV AH,02H
            INT 21H
            
            MOV AL,NUMOUT2
            ADD AL,'0'
            MOV DL,AL
            MOV AH,02H
            INT 21H                                    
            
PUT_NUM_END:POP DX
            POP BX
            POP AX
            RET
PUT_NUM     ENDP

 

;Output one info of a student
OUTPUT      PROC NEAR
            PUSH AX
            PUSH BX
            PUSH DX
            
            LEA DX,INPU1_WRD ;Display the tip-NAME
            MOV AH,09H
            INT 21H
            
            MOV DX,LOCATION
            MOV LOCATION1,DX
            CALL PUTS        
            
            LEA DX,INPU2_WRD ;Display the tip-CLASS
            MOV AH,09H
            INT 21H
            
            MOV DX,LOCATION1
            ADD DX,CLASSPOS
            MOV LOCATION,DX
            CALL PUTS
            
            LEA DX,INPU3_WRD ;Display the tip-NO.
            MOV AH,09H
            INT 21H
            
            MOV DX,LOCATION1
            ADD DX,NOPOS
            MOV LOCATION,DX
            CALL PUTS
            
            LEA DX,INPU4_WRD ;Display the tip-SCORE
            MOV AH,09H
            INT 21H
            
            MOV BX,LOCATION1
            ADD BX,NUMPOS
            MOV AL,BYTE PTR [BX]
            MOV NUMOUT1,AL
            MOV AL,BYTE PTR [BX+1]
            MOV DOTOUT,AL
            MOV AL,BYTE PTR [BX+2]
            MOV NUMOUT2,AL
            CALL PUT_NUM
            
            MOV DL,0DH ;Print another line
            MOV AH,02H
            INT 21H
            MOV DL,0AH
            INT 21H
            
            MOV AX,LOCATION1 ;End of output
            MOV LOCATION,AX
            POP DX
            POP BX 
            POP AX
            RET 
OUTPUT      ENDP



;Output info of all students
OUTPUTALL   PROC NEAR
            PUSH AX
            PUSH BX
            PUSH CX
            
            MOV CL,NUM
            CMP CL,0
            JZ OUTPUTALLEND ;If no students, end directly
            MOV CH,0
            MOV BX,0
OUTPUTALL1: MOV AX,STU[BX] ;Output students' info one by one 
            MOV LOCATION,AX
            CALL OUTPUT
            ADD BX,2
            DEC CX
            CMP CX,0
            JNZ OUTPUTALL1
OUTPUTALLEND: 
            POP CX
            POP BX
            POP AX
            RET
OUTPUTALL   ENDP



;Output the error message
ERRPRINT    PROC NEAR
            PUSH AX
            PUSH DX            
            LEA DX,ERRO1_WRD ;Print Error Msg
            MOV AH,09H
            INT 21H
            MOV DL,0DH       ;Print another line
            MOV AH,02H
            INT 21H
            MOV DL,0AH
            INT 21H
            POP DX
            POP AX
            RET
ERRPRINT    ENDP            



;Rank the students by marks
RANK        PROC NEAR
            PUSH AX
            PUSH BX
            PUSH CX
            PUSH DX
            PUSH DI
            PUSH SI
            
            MOV CL,NUM
            MOV CH,0
            CMP CL,0   ;If empty or only one student,end directly
            JZ RANK_EMPTY
            CMP CL,1
            JZ RANK_END
            DEC CL

;Bubble Sort            
LOOP1:      MOV SI,CX  ;Out Loop
            MOV BX,0
LOOP2:      MOV DI,STU[BX]  ;In Loop
            ADD DI,NUMPOS
            MOV AX,[DI]
            MOV DI,STU[BX+2]
            ADD DI,NUMPOS
            MOV DX,[DI]
            CMP AX,DX
            JG CONTI
            JE COMPDEC
CONTU:      MOV AX,STU[BX]  ;If the latter one is bigger than the former one, exchange them
            XCHG STU[BX+2],AX
            XCHG STU[BX],AX
CONTI:      ADD BX,2
            LOOP LOOP2
            MOV CX,SI
            LOOP LOOP1
            JMP RANK_END

COMPDEC:    MOV DI,STU[BX]  ;If their integer is the same, compare the decimal part
            ADD DI,DECPOS
            MOV AX,[DI]
            MOV DI,STU[BX+2]
            ADD DI,DECPOS
            MOV DX,[DI]
            CMP AX,DX
            JL CONTU
            JMP CONTI            
            
RANK_END:   POP SI
            POP DI
            POP DX
            POP CX
            POP BX
            POP AX
            RET

RANK_EMPTY: LEA DX,EMPTY_WRD ;Print Empty Msg
            MOV AH,09H
            INT 21H
            MOV DL,0DH
            MOV AH,02H
            INT 21H
            MOV DL,0AH
            INT 21H
            JMP RANK_END
            
RANK        ENDP



;Rank the students by marks
RANKS       PROC NEAR
            PUSH AX
            PUSH BX
            PUSH CX
            PUSH DX
            PUSH DI
            PUSH SI
            
            MOV CL,NUM
            MOV CH,0
            CMP CL,0   ;If empty or only one student,end directly
            JZ RANK_EMPTY
            CMP CL,1
            JZ RANK_END
            DEC CL

;Bubble Sort            
LOOPS1:     MOV SI,CX  ;Out Loop
            MOV BX,0
LOOPS2:     MOV DI,STU[BX]  ;In Loop
            ADD DI,NOPOS
            MOV CMP1,DI
            MOV DI,STU[BX+2]
            ADD DI,NOPOS
            MOV CMP2,DI
            CALL CMPS
            CMP CMPRES,1
            JZ CONTIS
CONTUS:     MOV AX,STU[BX]  ;If the latter one is bigger than the former one, exchange them
            XCHG STU[BX+2],AX
            XCHG STU[BX],AX
CONTIS:     ADD BX,2
            LOOP LOOPS2
            MOV CX,SI
            LOOP LOOPS1         
            
RANKS_END:  POP SI
            POP DI
            POP DX
            POP CX
            POP BX
            POP AX
            RET

RANKS_EMPTY:
            LEA DX,EMPTY_WRD ;Print Empty Msg
            MOV AH,09H
            INT 21H
            MOV DL,0DH
            MOV AH,02H
            INT 21H
            MOV DL,0AH
            INT 21H
            JMP RANKS_END
            
RANKS       ENDP

CMPS        PROC
            PUSH AX
            PUSH BX
            PUSH CX
            PUSH DX
            PUSH DI
            
            MOV BX,CMP1
            MOV DI,CMP2
            
CMPS_A:     MOV AL,BYTE PTR [BX]
            CMP AL,'$'
            JZ CMPS_B
            INC BX
            MOV AX,[DI]
            CMP AX,'$'
            JZ CMPS_E
            INC DI
            JMP CMPS_A
            
CMPS_B:     MOV AL,BYTE PTR [DI]
            CMP AL,'$'
            JZ CMPS_D
            JMP CMPS_C
            
CMPS_C:     MOV AL,0
            MOV CMPRES,AL
            JMP CMPS_END
            
CMPS_D:     MOV BX,CMP1
            MOV DI,CMP2
CMPS_D2:    MOV AL,BYTE PTR [BX]
            MOV CL,BYTE PTR [DI]
            CMP AL,CL
            JA CMPS_E
            JB CMPS_C
            CMP AL,'$'
            JZ CMPS_C
            INC BX
            INC DI
            JMP CMPS_D2
            
CMPS_E:     MOV AL,1
            MOV CMPRES,AL            
            
CMPS_END:   POP DI
            POP DX
            POP CX
            POP BX
            POP AX
            RET
CMPS        ENDP


;Calculate average score of all students
AVERAGE     PROC NEAR
            PUSH AX
            PUSH BX
            PUSH CX
            PUSH DI
            
            MOV CL,NUM
            CMP CL,0
            JZ AVERAGE_ZERO
            MOV CH,0
            MOV BX,0
            MOV SUM,BX ;SUM is the sum of Integet Part
            MOV DECSUM,BX  ;DECSUM is the sum of Decimal Part
            
AVELOOP1:   MOV DI,STU[BX] ;Take out the number and add it to the sum
            ADD DI,NUMPOS
            MOV AL,BYTE PTR [DI]
            CBW
            ADD SUM,AX
            ADD DI,2
            MOV AL,BYTE PTR [DI]
            CBW
            ADD DECSUM,AX
            ADD BX,2
            LOOP AVELOOP1
            
            MOV AX,SUM ;DIV the sum with num
            MOV CL,NUM
            DIV CL
            MOV NUMOUT1,AL
            MOV BL,10
            MOV AL,AH
            MUL BL
            ADD DECSUM,AX
            MOV AX,DECSUM
            MOV CL,NUM
            DIV CL
            MOV NUMOUT2,AL
            MOV AL,1
            MOV DOTOUT,AL

AVERAGE_END:            
            POP DI
            POP CX
            POP BX
            POP AX
            RET
            
AVERAGE_ZERO: ;If there is no students, average is zero
            MOV AL,0
            MOV NUMOUT1,AL
            MOV DOTOUT,AL
            MOV NUMOUT2,AL
            JMP AVERAGE_END            

AVERAGE     ENDP



;Calculate the number of different section                        
SECTION     PROC NEAR
            PUSH AX
            PUSH BX
            PUSH CX
            PUSH DX
            
            MOV CL,NUM
            MOV CH,0
            MOV BX,0
            MOV SEC1,BL
            MOV SEC2,BL
            MOV SEC3,BL
            MOV SEC4,BL
            MOV SEC5,BL
            CMP CL,0
            JZ SECOUT            
            
SECLOOP:    MOV DI,STU[BX] ;Loop all the info
            ADD DI,NUMPOS
            MOV AL,BYTE PTR [DI]
            CMP AL,60
            JL SECTION1
            CMP AL,70
            JL SECTION2
            CMP AL,80
            JL SECTION3
            CMP AL,90
            JL SECTION4
            JMP SECTION5
SECNEXT:    ADD BX,2
            LOOP SECLOOP
            JMP SECOUT
            
SECTION1:   INC SEC1
            JMP SECNEXT
SECTION2:   INC SEC2
            JMP SECNEXT
SECTION3:   INC SEC3
            JMP SECNEXT
SECTION4:   INC SEC4
            JMP SECNEXT
SECTION5:   INC SEC5
            JMP SECNEXT
                        
SECOUT:     MOV AL,0  ;Print the count of different sections
            MOV DOTOUT,AL
            LEA AX,SEC1_WRD
            MOV LOCATION,AX
            CALL PUT
            MOV AL,SEC1
            MOV NUMOUT1,AL
            CALL PUT_NUM
            MOV DL,0DH ;Print another line
            MOV AH,02H
            INT 21H
            MOV DL,0AH
            INT 21H
            
            LEA AX,SEC2_WRD
            MOV LOCATION,AX
            CALL PUT
            MOV AL,SEC2
            MOV NUMOUT1,AL
            CALL PUT_NUM
            MOV DL,0DH ;Print another line
            MOV AH,02H
            INT 21H
            MOV DL,0AH
            INT 21H
            
            LEA AX,SEC3_WRD
            MOV LOCATION,AX
            CALL PUT
            MOV AL,SEC3
            MOV NUMOUT1,AL
            CALL PUT_NUM
            MOV DL,0DH ;Print another line
            MOV AH,02H
            INT 21H
            MOV DL,0AH
            INT 21H
            
            LEA AX,SEC4_WRD
            MOV LOCATION,AX
            CALL PUT
            MOV AL,SEC4
            MOV NUMOUT1,AL
            CALL PUT_NUM
            MOV DL,0DH ;Print another line
            MOV AH,02H
            INT 21H
            MOV DL,0AH
            INT 21H 
            
            LEA AX,SEC5_WRD
            MOV LOCATION,AX
            CALL PUT
            MOV AL,SEC5
            MOV NUMOUT1,AL
            CALL PUT_NUM
            MOV DL,0DH ;Print another line
            MOV AH,02H
            INT 21H
            MOV DL,0AH
            INT 21H
            
            POP DX
            POP CX
            POP BX
            POP AX
            RET

SECTION     ENDP



;This is designed for debug on Emu8086
DEBUG       PROC NEAR
            PUSH AX
            PUSH DX            
            LEA DX,DEBUG_WRD ;Print Debug Msg
            MOV AH,09H
            INT 21H
            MOV DL,0DH
            MOV AH,02H
            INT 21H
            MOV DL,0AH
            INT 21H
            
            MOV AH,01H
            INT 21H
            POP DX
            POP AX
            RET
DEBUG       ENDP


CODE        ENDS
END         START