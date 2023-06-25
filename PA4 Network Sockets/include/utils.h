#ifndef UTILS_H
#define UTILS_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h> 
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/socket.h>

#define MAX_STR 64 // The longest possible uame/username
#define MAX_ACC 1023 // The maximum number of user accounts
#define SA struct sockaddr 
#define CASH_AMOUNT 100000 // The amount of cash requested when not enough is present to make transaction
#define START_CASH 10000 // The initial amount of cash in the local cash reserve
#define LOG_SLEEP 5 // The length of time the log thread waits before relogging account info


/*  DO NOT MODIFY */
// The enumerated message types
typedef enum
{
	REGISTER,
	GET_ACCOUNT_INFO,
	TRANSACT,
	GET_BALANCE,
	ACCOUNT_INFO,
	BALANCE,
	REQUEST_CASH,
	CASH,
	ERROR,
	TERMINATE,
	/* extra credit messages */
	REQUEST_HISTORY,
	HISTORY,
}msg_enum;

/**
 * Removes the output directory if it exists, and creates a new empty one
 * 
 * @returns none
 */
void bookeepingCode();

/**
 * Store the next line from a file in the line buffer.
 * 
 * @param fp        the input file's file pointer
 * @param line      the line buffer
 * @param len        length of a line
 * @returns The number of bytes read from the file
 */
ssize_t getLineFromFile(FILE *fp, char *line, size_t len);

#endif

