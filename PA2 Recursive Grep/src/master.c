/* test machine: csel-kh4250-49
 * group number: G41
 * name: Michael Spiese, Jack Vehmeier, Isaac Hein
 * x500: spies046, vehme003, hein0540
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"



int main(int argc, char** argv){
	// Catch error of incorrect number of command-line arguments passed in
	if(argc != 3) {
		fprintf(stderr, "ERROR: Recieved %d arguments, expected 3\n", argc);
		fprintf(stderr,"Usage ./a.out [Path to Directory] [Pattern to search] \n");
		exit(EXIT_FAILURE);
	}
	
	// Store command-line arguments in more convenient character arrays
	char* path = argv[1];
	char* pattern = argv[2];
	
	// Variables to keep track of precess creation and termination
	pid_t pid;
	pid_t terminated_pid;
	int *procStat;
	
	// Variables to handle pipe creation and reading from the pipe
	int fds[2];
	int curPipe;
	int pipesRead = 0;
	int pipeArr[MAX_ROOT_SUBDIRS];
	char buffer[MAX_READ_LENGTH];
	
	//Open root directory specified in command line
	DIR *dir;
	if ((dir = opendir(path)) == NULL) {
		fprintf(stderr, "ERROR: Directory %s could not be opened.\n", path);
		exit(EXIT_FAILURE);
	}
	
	// Iterate through root dir process each file/directory
	char *curPath = (char *)malloc(sizeof(char) * MAX_PATH_LENGTH);
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
		// If file is a directory, create child and set up pipe to communicate
		if (entry->d_type == DT_DIR) {
			// Do not consider current or previous directories, skip and continue through directory
			if (!strcmp(entry->d_name, "..") || !strcmp(entry->d_name, ".")) continue;
			
			// Format filepath of child's directory (root's subdirectory)
			sprintf(curPath, "%s/%s", path, entry->d_name);
			
			//Create pipe for subdirectory in root and catch pipe creation error
			if ((curPipe = pipe(fds)) < 0) {
				fprintf(stderr, "ERROR: Failed to open pipe for subdirectory %s.\n", curPath);
				exit(EXIT_FAILURE);
			}
			
			// Fork off master process
			pid = fork();
			
			// If process is the child, close pipe read end and redirect stdin to write end
			if (pid == 0) {
				close(fds[0]);
				dup2(fds[1], STDOUT_FILENO);
				close(fds[1]);
				
				// Execute child program and catch error if program does not execute
				execl("child", "child", curPath, pattern, NULL);
				fprintf(stderr, "ERROR: Failed to execute child program\n");
				exit(EXIT_FAILURE);
			}
			
			// If process is the parent, close pipe write end and save read end's file descriptor in array
			else if (pid > 0) {
				close(fds[1]);
				pipeArr[pipesRead++] = fds[0];
			}
			
			// Catch error of failure to create child process
			else {
				fprintf(stderr, "ERROR: Failed to spawn child process\n");
				exit(EXIT_FAILURE);
			}
		}
		
		// Search for pattern in regular file located in root directory
		else if (entry->d_type == DT_REG) {
			sprintf(curPath, "%s/%s", path, entry->d_name);
			searchPatternInFile(curPath, pattern);
		}
	}
	// Parsing root completed, filepath no longer necessary
	free(curPath);
	
	//Wait for all children to complete and catch if any processes ended with failure
	while((terminated_pid = wait(procStat)) > 0) {
		if(terminated_pid == EXIT_FAILURE) {
			fprintf(stderr, "ERROR: Child process failed.\n");
			exit(EXIT_FAILURE);
		}
	}
	
	//Read pipes of all children and print to stdout
	//Assumption : Pipe never gets full
	int i = 0;
	int bytes_read = 0;
	while(i<pipesRead) {
		if ((bytes_read = read(pipeArr[i], buffer, MAX_READ_LENGTH)) != 0) {
			// Print contents of pipe if nonzero amount of bytes read
			printf("%s", buffer);
		}
		else {
			// Close each pipe after reading and move to next pipe
			close(pipeArr[i++]);
		}
	}
	
	return EXIT_SUCCESS;
}
