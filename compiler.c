/*
 * compiler.c
 */

#include "compiler.h"

#include <stdio.h>
#include <stdlib.h>

int counter = 0;
int maxCC = 0; // maximum conditional counter
sElt *theStack;
sElt *elseStack;

// YOU NEED TO FINISH THIS AND TRANSFORM IT INTO A HICONST/CONST PAIR EITHER MANUALLY
// OR USING LC BUT YOURE GOING TO NEED A LABEL FOR THIS VALUE AND MAKE SURE ITS UNIQUE
void push (short int val, FILE *oFile) {
	fprintf(oFile, "ADD R6 R6 #-1\n");
	fprintf(oFile, "CONST R1 #%hi\n", (short signed int) (0x00FF & val));
	fprintf(oFile, "HICONST R1 #%hi\n", (short signed int) ((0xFF00 & val)>>8));
	fprintf(oFile, "STR R1 R6 #0\n");
}

void pop (FILE *oFile) {
	fprintf(oFile, "ADD R6 R6 #1\n");
}

void dup (FILE *oFile) {
	fprintf(oFile, "LDR R1 R6 #0\n");
	fprintf(oFile, "ADD R6 R6 #-1\n");
	fprintf(oFile, "STR R1 R6 #0\n");
}

void swap (FILE *oFile) {
	fprintf(oFile, "LDR R2 R6 #1\n");
	fprintf(oFile, "LDR R1 R6 #0\n");
	fprintf(oFile, "STR R1 R6 #1\n");
	fprintf(oFile, "STR R2 R6 #0\n");
}

void rot (FILE *oFile) {
	fprintf(oFile, "LDR R1 R6 #0\n");
	fprintf(oFile, "LDR R2 R6 #1\n");
	fprintf(oFile, "LDR R3 R6 #2\n");
	fprintf(oFile, "STR R1 R6 #1\n");
	fprintf(oFile, "STR R2 R6 #2\n");
	fprintf(oFile, "STR R3 R6 #0\n");
}

sElt *pushS (sElt *head, int num) {
	sElt *elt;
	// Allocate a new heap element with malloc
	elt = malloc (sizeof(*elt));
	// If malloc fails end the program
 	if (elt == NULL) {
    	quit(head);
    	exit(0);
  	}
	elt->val = num;
	elt->prev = head;
	
	return elt;
}

sElt *popS (sElt *head) {
	if(!head) {
		printf("STACK ERROR when popping\n");
		return NULL;
	}
	return head->prev;
}

int peekS (sElt *head) {
	if(!head) {
		printf("STACK ERROR with peeking\n");
		quit(head);
    	exit(0);
	}
	return head->val;
}

void quit(sElt *head) {
	if (head == NULL) {
	} else {
		quit(head->prev);
		free(head);
	}
}

void defun (token *theToken, FILE *oFile) {
	fprintf(oFile, ".FALIGN\n");
	fprintf(oFile, "%s\n", theToken->str);
	// create a frame for the function invocation
	// allocate space on the stack for the RV, RA, and FP
	fprintf(oFile, "ADD R6 R6 #-3\n");
	// save return address of caller
	fprintf(oFile, "STR R7 R6 #1\n");
	// save frame pointer of caller
	fprintf(oFile, "STR R5 R6 #0\n");
	// move FP to where the SP to point at the base of the new frame
	fprintf(oFile, "ADD R5 R6 #0\n");
}

void ident (token *theToken, FILE *oFile) {
	// perform subroutine
	fprintf(oFile, "JSR %s\n", theToken->str);
}

void ret (FILE *oFile) {
	// load the value at the top of stack into R1
	fprintf(oFile, "LDR R1 R6 #0\n");
	// store the value stored in R1 and store it into the RV slot
	fprintf(oFile, "STR R1 R5 #2\n");
	// pop off all of the local variables
	fprintf(oFile, "ADD R6 R5 #0\n");
	// restore caller frame pointer
	fprintf(oFile, "LDR R5 R6 #0\n");
	// restore RA by placing the RA into R7
	fprintf(oFile, "LDR R7 R6 #1\n");
	// pop off the caller's FP and RA
	fprintf(oFile, "ADD R6 R6 #2\n");
	// this is a JMPR R7 in disguise
	fprintf(oFile, "RET\n");
}

void plus (FILE *oFile) {
	fprintf(oFile, "LDR R1 R6 #0\n");
	fprintf(oFile, "LDR R2 R6 #1\n");
	fprintf(oFile, "ADD R1 R1 R2\n");
	pop(oFile);
	fprintf(oFile, "STR R1 R6 #0\n");
}

void minus (FILE *oFile) {
	fprintf(oFile, "LDR R1 R6 #0\n");
	fprintf(oFile, "LDR R2 R6 #1\n");
	fprintf(oFile, "SUB R1 R1 R2\n");
	pop(oFile);
	fprintf(oFile, "STR R1 R6 #0\n");
}

void mul (FILE *oFile) {
	fprintf(oFile, "LDR R1 R6 #0\n");
	fprintf(oFile, "LDR R2 R6 #1\n");
	fprintf(oFile, "MUL R1 R1 R2\n");
	pop(oFile);
	fprintf(oFile, "STR R1 R6 #0\n");
}

void divide (FILE *oFile) {
	fprintf(oFile, "LDR R1 R6 #0\n");
	fprintf(oFile, "LDR R2 R6 #1\n");
	fprintf(oFile, "DIV R1 R1 R2\n");
	pop(oFile);
	fprintf(oFile, "STR R1 R6 #0\n");
}

void mod (FILE *oFile) {
	fprintf(oFile, "LDR R1 R6 #0\n");
	fprintf(oFile, "LDR R2 R6 #1\n");
	fprintf(oFile, "MOD R1 R1 R2\n");
	pop(oFile);
	fprintf(oFile, "STR R1 R6 #0\n");
}

void and (FILE *oFile) {
	fprintf(oFile, "LDR R1 R6 #0\n");
	fprintf(oFile, "LDR R2 R6 #1\n");
	fprintf(oFile, "AND R1 R1 R2\n");
	pop(oFile);
	fprintf(oFile, "STR R1 R6 #0\n");
}

void or (FILE *oFile) {
	fprintf(oFile, "LDR R1 R6 #0\n");
	fprintf(oFile, "LDR R2 R6 #1\n");
	fprintf(oFile, "OR R1 R1 R2\n");
	pop(oFile);
	fprintf(oFile, "STR R1 R6 #0\n");
}

void not (FILE *oFile) {
	fprintf(oFile, "LDR R1 R6 #0\n");
	fprintf(oFile, "NOT R1 R1\n");
	fprintf(oFile, "STR R1 R6 #0\n");
}

void lt (FILE *oFile) {
	fprintf(oFile, "LDR R1 R6 #0\n");
	fprintf(oFile, "LDR R2 R6 #1\n");
	pop(oFile);
	pop(oFile);
	fprintf(oFile, "CMP R1 R2\n");
	fprintf(oFile, "BRn TRUE_lt%d\n", counter);
	push(0, oFile);
	fprintf(oFile, "JMP END_lt%d\n", counter);
	fprintf(oFile, "TRUE_lt%d\n", counter);
	push(1, oFile);
	fprintf(oFile, "END_lt%d\n", counter);
	counter++;
}

void le (FILE *oFile) {
	fprintf(oFile, "LDR R1 R6 #0\n");
	fprintf(oFile, "LDR R2 R6 #1\n");
	pop(oFile);
	pop(oFile);
	fprintf(oFile, "CMP R1 R2\n");
	fprintf(oFile, "BRnz TRUE_le%d\n", counter);
	push(0, oFile);
	fprintf(oFile, "JMP END_le%d\n", counter);
	fprintf(oFile, "TRUE_le%d\n", counter);
	push(1, oFile);
	fprintf(oFile, "END_le%d\n", counter);
	counter++;
}

void eq (FILE *oFile) {
	fprintf(oFile, "LDR R1 R6 #0\n");
	fprintf(oFile, "LDR R2 R6 #1\n");
	pop(oFile);
	pop(oFile);
	fprintf(oFile, "CMP R1 R2\n");
	fprintf(oFile, "BRz TRUE_eq%d\n", counter);
	push(0, oFile);
	fprintf(oFile, "JMP END_eq%d\n", counter);
	fprintf(oFile, "TRUE_eq%d\n", counter);
	push(1, oFile);
	fprintf(oFile, "END_eq%d\n", counter);
	counter++;
}

void ge (FILE *oFile) {
	fprintf(oFile, "LDR R1 R6 #0\n");
	fprintf(oFile, "LDR R2 R6 #1\n");
	pop(oFile);
	pop(oFile);
	fprintf(oFile, "CMP R1 R2\n");
	fprintf(oFile, "BRzp TRUE_ge%d\n", counter);
	push(0, oFile);
	fprintf(oFile, "JMP END_ge%d\n", counter);
	fprintf(oFile, "TRUE_ge%d\n", counter);
	push(1, oFile);
	fprintf(oFile, "END_ge%d\n", counter);
	counter++;
}

void gt (FILE *oFile) {
	fprintf(oFile, "LDR R1 R6 #0\n");
	fprintf(oFile, "LDR R2 R6 #1\n");
	pop(oFile);
	pop(oFile);
	fprintf(oFile, "CMP R1 R2\n");
	fprintf(oFile, "BRp TRUE_gt%d\n", counter);
	push(0, oFile);
	fprintf(oFile, "JMP END_gt%d\n", counter);
	fprintf(oFile, "TRUE_gt%d\n", counter);
	push(1, oFile);
	fprintf(oFile, "END_gt%d\n", counter);
	counter++;
}

void ifF (FILE *iFile, FILE *oFile) {
	theStack = pushS (theStack, maxCC);
	fprintf(oFile, "LDR R1 R6 #0\n");
	pop(oFile);
	fprintf(oFile, "CMPI R1 #0\n");
	fprintf(oFile, "BRz FALSE_if%d\n", peekS(theStack));
	maxCC++;
}

void elseF (FILE *iFile, FILE *oFile) {
	elseStack = pushS(elseStack, peekS(theStack));
	fprintf(oFile, "JMP END_if%d\n", peekS(theStack));
	fprintf(oFile, "FALSE_if%d\n", peekS(theStack));
}

void endifF (FILE *iFile, FILE *oFile) {
	if (peekS(elseStack) != peekS(theStack)) fprintf(oFile, "FALSE_if%d\n", peekS(theStack));
	else elseStack = popS(elseStack);
	fprintf(oFile, "END_if%d\n", peekS(theStack));
	theStack = popS(theStack);
}

void lit (token *theToken, FILE *oFile) {
	push(theToken->literal_value, oFile);
}

void argN (token *theToken, FILE *oFile) {
	int offset = 2 + theToken->arg_no;
	fprintf(oFile, "LDR R1 R5 #%d\n", offset);
	fprintf(oFile, "ADD R6 R6 #-1\n");
	fprintf(oFile, "STR R1 R6 #0\n");
}



// returns the assembly instructions for theToken
void write_assembly (token *theToken, FILE *iFile, FILE *oFile) {
	int t = theToken->type;
	switch (t) {
		case DEFUN :
			defun(theToken, oFile);
			break;
		case IDENT :
			ident(theToken, oFile);
			break;
		case RETURN :
			ret(oFile);
			break;
		case PLUS :
			plus(oFile);
			break;
		case MINUS :
			minus(oFile);
			break;
		case MUL :
			mul(oFile);
			break;
		case DIV :
			divide(oFile);
			break;
		case MOD :
			mod(oFile);
			break;
		case AND :
			and(oFile);
			break;
		case OR :
			or(oFile);
			break;
		case NOT :
			not(oFile);
			break;
		case LT :
			lt(oFile);
			break;
		case LE :
			le(oFile);
			break;
		case EQ :
			eq(oFile);
			break;
		case GE :
			ge(oFile);
			break;
		case GT :
			gt(oFile);
			break;
		case IF :
			ifF(iFile, oFile);
			break;
		case ELSE :
			elseF(iFile, oFile);
			break;
		case ENDIF :
			endifF(iFile, oFile);
			break;
		case DROP :
			pop(oFile);
			break;
		case DUP :
			dup(oFile);
			break;
		case SWAP :
			swap(oFile);
			break;
		case ROT :
			rot(oFile);
			break;
		case ARG :
			argN(theToken, oFile);
			break;
		case LITERAL :
			lit(theToken, oFile);
			break;
		case BROKEN_TOKEN :
			printf("STACK ERROR: BROKEN_TOKEN enum...\n");
			exit(1);
			break;
		default :
			printf("STACK ERROR: undefined enum type...\n	%d\n", t);
			exit(1);
	}
}



// continually reads the input J file and writes to the output file in assembly
int compile (FILE *iFile, FILE *oFile) {
	fprintf(oFile, ".CODE\n");

	token theHead;

	int ret_status = read_token(&theHead, iFile);

	write_assembly(&theHead, iFile, oFile);
	print_token(&theHead);

	while (!ret_status) {
		token theToken;
		ret_status = read_token(&theToken, iFile);
		if (!ret_status) {
			write_assembly(&theToken, iFile, oFile);
			print_token(&theToken);
		}
	}

	return 0;
}