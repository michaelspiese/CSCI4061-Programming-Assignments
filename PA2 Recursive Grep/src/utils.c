/* test machine: csel-kh4250-49
 * group number: G41
 * name: Michael Spiese, Jack Vehmeier, Isaac Hein
 * x500: spies046, vehme003, hein0540
 */

#include<stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include<string.h>
#include "utils.h"

/*
 * Searches for pattern in file and writes the entire line containing the pattern to STDOUT
 * Format for print: [path to file] : Line in the file
 * @Params:
 *		path - path to the file
 *		pattern : pattern to be searched
 */
void searchPatternInFile(char* path, char* pattern){
	// Specify variables to use in parsing file for given pattern
	FILE * fp;
	char buffer[MAX_WRITE_LENGTH];
	char line[1000];

	// Open file and catch error of incorrectly opened file
	if ((fp = fopen(path, "r")) == NULL){
		fprintf(stderr,"Error opening file: %s\n", path);
		return;
	}
	
	//Read file line by line and check if pattern exists in the line
	while(fgets(line, sizeof(line), fp) != NULL){
		// If line contains specified pattern, write "[Path to file]: [Line containing pattern]" to STDOUT
		if(strstr(line, pattern) != NULL){
			sprintf(buffer, "%s: %s", path, line);
			write(STDOUT_FILENO, buffer, strlen(buffer));
		}
	}
	
	//Close input file
	fclose(fp);
}
