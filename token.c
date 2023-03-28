/*
 * token.c
 */

#include "token.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// line_number : You can use this to keep track of the current line number which can be useful in reporting syntax errors.
int line_number = 1;

// examines the string and classifies based on if it is a literal or keyword
void classify (token *theToken, FILE *iFile) {
	char *s = theToken->str;
	if (!strncmp(s, "defun", MAX_TOKEN_LENGTH)) {
		theToken->type = DEFUN;
		char c = fgetc(iFile);

		// continue reading from the file until we hit a non-whiterspace or newline char
		// this should eliminate all whitespace and newline characters
		while (c == ' ' || c == '\n' || c == ';' || c == 9) {
			if (c == ';') {
				while (c != '\n') {
					c = fgetc(iFile);
				}
				if (c == EOF) return;
			}
			if (c == '\n') line_number++;
			c = fgetc(iFile);
		}

		if (c == EOF) return;
		
		// now we are checking for the name of the newly defined function
		if (!isalpha(c)) {
			theToken->type = BROKEN_TOKEN;
			return;
		}
		
		int i;
		for (i = 0; i < MAX_TOKEN_LENGTH && c != EOF && c != ' ' && c != '\n' && c != 9; i++) {
			if (!(isalnum(c) || c != '_')) {
				theToken->type = BROKEN_TOKEN;
				return;
			}
			s[i] = c;
			c = fgetc(iFile);
		}
		s[i] = '\0';
	} else if (!strncmp(s, "return", MAX_TOKEN_LENGTH)) {
		theToken->type = RETURN;
	} else if (!strncmp(s, "+", MAX_TOKEN_LENGTH)) {
		theToken->type = PLUS;
	} else if (!strncmp(s, "-", MAX_TOKEN_LENGTH)) {
		theToken->type = MINUS;
	} else if (!strncmp(s, "*", MAX_TOKEN_LENGTH)) {
		theToken->type = MUL;
	} else if (!strncmp(s, "/", MAX_TOKEN_LENGTH)) {
		theToken->type = DIV;
	} else if (!strncmp(s, "%", MAX_TOKEN_LENGTH)) {
		theToken->type = MOD;
	} else if (!strncmp(s, "and", MAX_TOKEN_LENGTH)) {
		theToken->type = AND;
	} else if (!strncmp(s, "or", MAX_TOKEN_LENGTH)) {
		theToken->type = OR;
	} else if (!strncmp(s, "not", MAX_TOKEN_LENGTH)) {
		theToken->type = NOT;
	} else if (!strncmp(s, "lt", MAX_TOKEN_LENGTH)) {
		theToken->type = LT;
	} else if (!strncmp(s, "le", MAX_TOKEN_LENGTH)) {
		theToken->type = LE;
	} else if (!strncmp(s, "eq", MAX_TOKEN_LENGTH)) {
		theToken->type = EQ;
	} else if (!strncmp(s, "ge", MAX_TOKEN_LENGTH)) {
		theToken->type = GE;
	} else if (!strncmp(s, "gt", MAX_TOKEN_LENGTH)) {
		theToken->type = GT;
	} else if (!strncmp(s, "if", MAX_TOKEN_LENGTH)) {
		theToken->type = IF;
	} else if (!strncmp(s, "else", MAX_TOKEN_LENGTH)) {
		theToken->type = ELSE;
	} else if (!strncmp(s, "endif", MAX_TOKEN_LENGTH)) {
		theToken->type = ENDIF;
	} else if (!strncmp(s, "drop", MAX_TOKEN_LENGTH)) {
		theToken->type = DROP;
	} else if (!strncmp(s, "dup", MAX_TOKEN_LENGTH)) {
		theToken->type = DUP;
	} else if (!strncmp(s, "swap", MAX_TOKEN_LENGTH)) {
		theToken->type = SWAP;
	} else if (!strncmp(s, "rot", MAX_TOKEN_LENGTH)) {
		theToken->type = ROT;
	} else {
		// this case will handle LITERAL, ARG, IDENT, and BROKEN_TOKEN
		int lit;
		unsigned int hex;
		char *str = malloc(200*sizeof(char));
		if (1 == sscanf(s, "%d%s", &lit, str)) {
			theToken->type = LITERAL;
			theToken->literal_value = lit;
		} else if (1 == sscanf(s, "0x%X%s", &hex, str)) {
			theToken->type = LITERAL;
			theToken->literal_value = hex;
		} else if (1 == sscanf(s, "arg%d%s", &lit, str)) {
			if (lit >= 1 || lit <= 20) {
				theToken->arg_no = lit;
				theToken->type = ARG;
			} else theToken->type = BROKEN_TOKEN;
		} else if (1 == sscanf(s, "%s", str)) {
			theToken->type = IDENT;
			if (!isalpha(str[0])) theToken->type = BROKEN_TOKEN;
			for (int i = 1; str[i] != '\0'; i++) {
				if (!(isalnum(str[i]) || str[i] == '_')) {
					theToken->type = BROKEN_TOKEN;
					free(str);
					return;
				}
			}
		} else {
			theToken->type = BROKEN_TOKEN;
		}
		free(str);
	}
}

/*
 * Return:
 * 0 -> Successful
 * 1 -> Reached the end of the file before reaching another token
 */
int read_token (token *theToken, FILE *iFile) {

	theToken->literal_value = 0;
	theToken->arg_no = 0;

	char c = fgetc(iFile);

	// continue reading from the file until we hit a non-whiterspace or newline char
	// this should eliminate all whitespace and newline characters
	while (c == ' ' || c == '\n' || c == ';' || c == 9) {
		if (c == ';') {
			while (c != '\n') {
				c = fgetc(iFile);
			}
			if (c == EOF) return 1;
		}
		if (c == '\n') line_number++;
		c = fgetc(iFile);
	}

	if (c == EOF) return 1;
	
	int i;
	for (i = 0; i < MAX_TOKEN_LENGTH && c != EOF && c != ' ' && c != '\n' && c != 9; i++) {
		theToken->str[i] = c;
		c = fgetc(iFile);
	}
	theToken->str[i] = '\0';

	classify(theToken, iFile);

	// if (c == EOF) return 1;

	return 0;
}

// Extra functions which you may wish to define and use , or not
const char *token_type_to_string (int type);

void print_token (token *theToken) {
	printf("enum: %d | literal_value: %d | arg_no: %d | str: %s\n", theToken->type, theToken->literal_value, theToken->arg_no, theToken->str);
}