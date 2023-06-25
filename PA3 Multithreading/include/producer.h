#ifndef PRODUCER_H
#define PRODUCER_H

#include "utils.h"

/* buffer functions */
/**
 * Adds a new node to the shared packet queue linked-list
 * @params:
 *		line : An entire, single line from the input file
 *		lineNum : An integer corresponding to the line's number
 * @return none
 */
void addNode(char *line, int lineNum);

/**
 * Prints all nodes' lines to the terminal. Used for debugging
 * @param none
 * @return none
 */
void printNodes();

/* thread function */
/**
 * Opens the input filepath passed in and appends packets to the shared queue based on the input file
 * @param filename A pointer to a string containing the input filepath
 * @return none
 */
void *producer(void *filename);

#endif
