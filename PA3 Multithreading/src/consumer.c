/* test machine: csel-kh4250-49
 * group number: G41
 * name: Michael Spiese, Jack Vehmeier, Isaac Hein
 * x500: spies046, vehme003, hein0540
 */

#include "consumer.h"
#include <ctype.h>

/**
 * Takes a string of the correct csv line format and updates the balance array of a customer's 
 * index with the contents of the line.
 */
int parse(char line[]){
    char *numstr;
    double sum = 0;
    long int idx;
    
    // If line to parse is NULL, special packet has been received
    if (line == NULL) {
    	return 1;
    }
    
    // Wait on/decrement balance array semaphore counter before critical section (strtok calls not thread safe, so they are included in critical section)
    sem_wait(&balanceSem);
    
    // Find index of account to update (number before first comma)
    numstr = strtok(line,",");
    idx = strtol(numstr, NULL, 10);
    numstr = strtok(NULL,",");      // Advance to next comma
    
    // Sum up transactions by using number before each comma in the string
    while (numstr != NULL) {        
      sum += strtod(numstr, NULL);  
      numstr = strtok(NULL, ",");  
    }
    
    // Update the global array at specified index with the line's sum
    balance[idx] += sum;
    
    // Signal/increment balance semaphore counter after critical section
    sem_post(&balanceSem);
    
    // Return 0 if EOF not reached yet
    return 0;
}


/**
 * Grabs the packet at the head of the shared linked-list queue and moves head forward one node
 */
struct Packet *takeFromQueue() {
	struct Packet *grabbed = head;
	head = head->next;
	return grabbed;
}

/** 
 * Grabs the head node from the shared queue and parses it's line to update the balance array
 */
void *consumer(void *cIdx) {
	int consNum = *((int *) cIdx); // Convert consumer index to an integer
    
    // Loop and "consume" packets from the shared queue until end of file
    while(1){
    	sem_wait(&bCountSem); // Wait if shared queue is empty, decrement semaphore counter otherwise
    	sem_wait(&bufferSem); // Wait on/decrement semaphore counter before critical section of adding packets to shared queue
    	
    	// Grab head node from the shared queue
        struct Packet *current = takeFromQueue();
        
        // Write log output "consumer [consumer #]: line [line #]" if flag is given
        if (doLog == 1)
        	fprintf(flog, "consumer %d: line %d\n", consNum, current->lineNum);
        	
        sem_post(&bufferSem); // Signal/increment semaphore counter after consumer critical section
        if (bound == 1)
        	sem_post(&boundSem); // Signal/increment semaphore counter for shared queue bound if flag is given
        
        // parse() returns 1 if special packet is received at EOF in producer, Break out of consumer loop
        if (parse(current->line) == 1) {
        	break;
        }
        
        // Free the grabbed packet's line and structure from the heap after use
        free(current->line);
        free(current);
    }
    
    return NULL; 
}


