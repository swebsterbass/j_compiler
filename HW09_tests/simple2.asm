.CODE
.FALIGN
main
ADD R6 R6 #-3
STR R7 R6 #1
STR R5 R6 #0
ADD R5 R6 #0
ADD R6 R6 #-1
CONST R1 #5
HICONST R1 #0
STR R1 R6 #0
ADD R6 R6 #-1
CONST R1 #3
HICONST R1 #0
STR R1 R6 #0
LDR R1 R6 #0
LDR R2 R6 #1
ADD R6 R6 #1
ADD R6 R6 #1
CMP R1 R2
BRp TRUE_gt0
ADD R6 R6 #-1
CONST R1 #0
HICONST R1 #0
STR R1 R6 #0
JMP END_gt0
TRUE_gt0
ADD R6 R6 #-1
CONST R1 #1
HICONST R1 #0
STR R1 R6 #0
END_gt0
JSR printnum
JSR endl
ADD R6 R6 #-1
CONST R1 #88
HICONST R1 #27
STR R1 R6 #0
ADD R6 R6 #-1
CONST R1 #190
HICONST R1 #252
STR R1 R6 #0
LDR R1 R6 #0
LDR R2 R6 #1
ADD R6 R6 #1
ADD R6 R6 #1
CMP R1 R2
BRn TRUE_lt1
ADD R6 R6 #-1
CONST R1 #0
HICONST R1 #0
STR R1 R6 #0
JMP END_lt1
TRUE_lt1
ADD R6 R6 #-1
CONST R1 #1
HICONST R1 #0
STR R1 R6 #0
END_lt1
JSR printnum
JSR endl
LDR R1 R6 #0
STR R1 R5 #2
ADD R6 R5 #0
LDR R5 R6 #0
LDR R7 R6 #1
ADD R6 R6 #2
RET
