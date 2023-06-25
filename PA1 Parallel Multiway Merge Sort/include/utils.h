#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <math.h>
#include <sys/wait.h>

#define LineBufferSize 20
#define WriteBufferSize 5000
#define MaxDataSize 1000
#define MaxFileNameLength 50
#define MaxDepth 9
#define MaxDegree 9

/**
 * open a file with Read and Write mode (0666).
 *
 * @param filepath filepath of a file to be opened
 * @return a file pointer to the file opened.
 */
FILE * getFilePointer(char *filepath);

/**
 * Read an entire line from file
 *
 * @param fp    a file pointer of the file from which data is read
 * @param line  pointer to a read buffer where the read data is stored
 * @param len   size of the read buffer
 * @return the number of characters read on success, -1 on failure
 */
ssize_t getLineFromFile(FILE *fp, char *line, size_t len);

/**
 * Write all data in the line (write buffer) to the file
 *
 * @param filepath  filepath of a file where data in the write buffer is written
 * @param line      pointer to a line (write buffer) where data to be written is stored
 * @return None
 */
void writeLineToFile(char *filepath, char *line);

/**
 * Initialize "output" folder
 *
 * @param None
 * @return None
 * @note Keep this function at the beginning of main()
 */
void bookeepingCode();

#endif
