/*
 * compiler.h
 */

#include "token.h"

#include <stdio.h>

// stack element
typedef struct sElt_tag {
	int val;
	struct sElt_tag *prev;
} sElt;

void quit(sElt *head);

void write_assembly (token *theToken, FILE *iFile, FILE *oFile);

int compile (FILE *iFile, FILE *oFile);