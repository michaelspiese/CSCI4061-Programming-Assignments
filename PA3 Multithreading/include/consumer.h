#ifndef CONSUMER_H
#define CONSUMER_H

#include "utils.h"

/* buffer functions */
/**
 * Takes a string of the correct csv line format and updates the balance array of a customer's 
 * index with the contents of the line.
 * @param line A string of the form "[customer#],[t1],[t2]...[tn]\n"
 * @return 0 if the line contains customer data, 1 if a special packet has been received and the line is NULL
 */
int parse(char line[]);

/**
 * Grabs the packet at the head of the shared linked-list queue and moves head forward one node
 * @param none
 * @return The packet node from the head of the linked-list queue
 */
struct Packet *takeFromQueue();

/* thread function */
/**
 * Grabs the head node from the shared queue and parses it's line to update the balance array
 * @param filename A pointer to a string containing the input filepath
 * @return none
 */
void *consumer(void *cIdx);

#endif
