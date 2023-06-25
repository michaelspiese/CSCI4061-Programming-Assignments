#ifndef UTILS_H
#define UTILS_H

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define chunkSize 1024
#define acctsNum 1000

/* shared queue linked-list structure definition */
struct Packet {
	char *line;
	int lineNum;
	struct Packet* next;
};

/* definitions of the head and tail nodes of the shared queue */
extern struct Packet *head;
extern struct Packet *tail;

/* semaphore definitions */
extern sem_t bufferSem;
extern sem_t balanceSem;
extern sem_t bCountSem;
extern sem_t boundSem;

/* global integer to store number of consumer threads */
extern int numCons;

/* global integers to keep track of execution flags */
extern int doLog;
extern int bound;

/* definition of the log file pointer */
FILE * flog;

/* shared array definition */
extern double balance[acctsNum];

/* file I/O */
/**
 * Get a pointer to a opened file based on the file name
 * @param *inputFileName  the file path
 * @return a file pointer pointing to the file
 */
FILE * getFilePointer(char *inputFileName);

/**
 * Read an entire line from a file
 * @param  *fp    the file to be read
 * @param  *line  contain the line content
 * @param  len    the size of the line
 * @return the number of character reads (including the newline \n, but not including terminator)
           -1 when reaching the end of file or error occurs
 */
ssize_t getLineFromFile(FILE *fp, char *line, size_t len);

/**
 * Open a file, and write a line to the file
 * @param *filepath  the file path
 * @param *line      the line content
 */
void writeLineToFile(char *filepath, char *line);

/* directory */
void bookeepingCode();

/* other function declaration */


#endif
