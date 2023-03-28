/* J Complier
 * (written in C)
 * Translates programs in J into Assembly Commands
 *
 * Input: filename of J program to be translated
 * Output: assembly file of trnaslated J program
 *
 * The output assembly file can be tested by the file into PennSim.
 * Make sure to also assemble any other necessary files into PennSim with the output file.
 *
 * Authored by Spencer Webster-Bass
 */

#include "compiler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char **argv) {
	FILE *iFile;
	iFile = fopen(argv[1], "r");

	if (!iFile) {
		printf("Error opening J file.\n");
		exit(1);
	}
	
	// remove the tag from the J file
	char *tag;
	tag = strrchr(argv[1], '.');
	tag[0] = '\0';
	char *ofn = strcat(argv[1], ".asm");

	FILE *oFile;
	oFile = fopen (ofn, "w");

	compile(iFile, oFile);

	fclose(iFile);
	fclose(oFile);
}