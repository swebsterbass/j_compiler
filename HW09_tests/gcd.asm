.CODE
.FALIGN
gcd
ADD R6 R6 #-3
STR R7 R6 #1
STR R5 R6 #0
ADD R5 R6 #0
LDR R1 R5 #4
ADD R6 R6 #-1
STR R1 R6 #0
ADD R6 R6 #-1
CONST R1 #0
HICONST R1 #0
STR R1 R6 #0
LDR R1 R6 #0
LDR R2 R6 #1
ADD R6 R6 #1
ADD R6 R6 #1
CMP R1 R2
BRz TRUE_eq0
ADD R6 R6 #-1
CONST R1 #0
HICONST R1 #0
STR R1 R6 #0
JMP END_eq0
TRUE_eq0
ADD R6 R6 #-1
CONST R1 #1
HICONST R1 #0
STR R1 R6 #0
END_eq0
LDR R1 R6 #0
ADD R6 R6 #1
CMPI R1 #0
BRz FALSE_if0
LDR R1 R5 #3
ADD R6 R6 #-1
STR R1 R6 #0
JMP END_if0
FALSE_if0
LDR R1 R5 #4
ADD R6 R6 #-1
STR R1 R6 #0
LDR R1 R5 #3
ADD R6 R6 #-1
STR R1 R6 #0
LDR R1 R6 #0
LDR R2 R6 #1
MOD R1 R1 R2
ADD R6 R6 #1
STR R1 R6 #0
LDR R1 R5 #4
ADD R6 R6 #-1
STR R1 R6 #0
JSR gcd
END_if0
LDR R1 R6 #0
STR R1 R5 #2
ADD R6 R5 #0
LDR R5 R6 #0
LDR R7 R6 #1
ADD R6 R6 #2
RET
.FALIGN
main
ADD R6 R6 #-3
STR R7 R6 #1
STR R5 R6 #0
ADD R5 R6 #0
ADD R6 R6 #-1
CONST R1 #206
HICONST R1 #1
STR R1 R6 #0
ADD R6 R6 #-1
CONST R1 #47
HICONST R1 #4
STR R1 R6 #0
JSR gcd
JSR printnum
JSR endl
LDR R1 R6 #0
STR R1 R5 #2
ADD R6 R5 #0
LDR R5 R6 #0
LDR R7 R6 #1
ADD R6 R6 #2
RET
