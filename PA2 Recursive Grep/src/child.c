/* test machine: csel-kh4250-49
 * group number: G41
 * name: Michael Spiese, Jack Vehmeier, Isaac Hein
 * x500: spies046, vehme003, hein0540
 */

#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include<string.h>
#include<stdlib.h>
#include "utils.h"

/*
 * Traverse the directory recursively and search for pattern in files.
 * @params:
 *		name - path to the directory
 *		pattern - pattern to be recusrively searched in the directory
 *	
 */
void dirTraverse(const char *name, char * pattern) {
	// Open directory and catch directory opening error
	DIR *dir;
	if ((dir = opendir(name)) == NULL) {
		fprintf(stderr, "ERROR: Directory could not be opened, exiting...\n");
		exit(EXIT_FAILURE);
	}
	
	// Create other variables to help recursively parse files in the directory
	struct dirent *entry;
	char curPath[MAX_PATH_LENGTH];
	
	//Recursively traverse the directory and call searchPatternInFile when neccessary
	while ((entry = readdir(dir)) != NULL) {
		// Ignore current and previous directory entries
		if (!(strcmp(entry->d_name, "..")) || !(strcmp(entry->d_name, "."))) continue;
		
		// Recursively explore sub-directories
		if (entry->d_type == DT_DIR) {
			sprintf(curPath, "%s/%s", name, entry->d_name);
			dirTraverse(curPath, pattern);
		}
		
		// If file is a regular file (not symbolic link) search for pattern in file
		if (entry->d_type == DT_REG) {
			sprintf(curPath, "%s/%s", name, entry->d_name);
			searchPatternInFile(curPath, pattern);
		}
	}
	
	// Close the directory
	closedir(dir);
}

int main(int argc, char** argv) {
	// Catch error if incorrect number of command-line arguments given
	if(argc !=3){
		fprintf(stderr,"ERROR: Child process %d recieved %d arguments, expected 3 \n",getpid(), argc);
		fprintf(stderr,"Usage child.o [Directory Path] [Pattern] \n");
		exit(EXIT_FAILURE);
	}
	
	// Save command-line arguments into character arrays
	char* path = argv[1];
	char* pattern = argv[2];
	
	// No input necessary, close STDIN
	close(STDIN_FILENO);
	
	// Begin recursively parsing the directory passed into through the command-line
	dirTraverse(path,pattern);
	
	// Close the write end of the pipe after finished writing
	close(STDOUT_FILENO);
	
	exit(EXIT_SUCCESS);
}
