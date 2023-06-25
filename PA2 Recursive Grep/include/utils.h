#ifndef UTILS_H
#define UTILS_H


#define MAX_ROOT_SUBDIRS 100
#define MAX_READ_LENGTH 5000
#define MAX_PATH_LENGTH 1024
#define MAX_WRITE_LENGTH 5000 
#define MAX_INODE_LENGTH 128

/*
	Search for pattern in the file given by path
	Writes to STDOUT, in the following format
	[path to file] : [line containing the pattern]
	
	@params:
		path : Path to the file to be searched
		pattern : Pattern to be searched
*/
void searchPatternInFile(char* path, char* pattern);

#endif
