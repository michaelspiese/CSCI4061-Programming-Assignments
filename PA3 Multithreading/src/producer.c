/* test machine: csel-kh4250-49
 * group number: G41
 * name: Michael Spiese, Jack Vehmeier, Isaac Hein
 * x500: spies046, vehme003, hein0540
 */

#include "producer.h"

/**
 * Adds a new node to the shared packet queue linked-list
 */
void addNode(char *line, int lineNum) {
	if (bound == 1)
		sem_wait(&boundSem); // Wait on/decrement semaphore counter of queue bounds if flag is given
    sem_wait(&bufferSem); // Wait on/decrement semaphore counter before accessing shared queue critical section
    	
    // Create and initialize new packet node on the heap with file line and line number
	struct Packet *newNode = (struct Packet*)malloc(sizeof(struct Packet));
    newNode->line = line;
    newNode->lineNum = lineNum;
    newNode->next = NULL;
 
    // If node is first added to shared queue, update head pointer to new packet node
    if (head == NULL) {
    	head = newNode;
	}
	// If packet node is subsequent node, set current tail's next pointer to the new node
	else {
		tail->next = newNode;
	}
	tail = newNode; // Update tail node pointer to the new node
	
	// Write log output "producer: line [line #]" if flag is given
    if (doLog == 1) 
    	fprintf(flog, "producer: line %d\n", lineNum);
	
    sem_post(&bufferSem); // Signal/increment semaphore counter after accessing shared queue critical section
    sem_post(&bCountSem); // Signal/increment semaphore counter of number of nodes in shared queue
}
 
/**
 * Prints all nodes' lines to the terminal. Used for debugging
 */
void printNodes() {
	// Iterate through and print packet lines starting from head
	struct Packet * iter = head;
	while (iter != NULL) {
		printf("%s", iter->line);
		iter = iter->next;
	}
}


/**
 * Opens the input filepath passed in and appends packets to the shared queue based on the input file
 */
void *producer(void *filename){
	char *inputFileName = filename;
    
	FILE * fin;                                                      // File pointer to input file
    char *line = (char *)malloc(sizeof(char) * chunkSize);         // Line buffer where a new line is stored
    size_t len = chunkSize;                                        // The size of the line buffer
    ssize_t nread;                                                // The number of bytes read from the input file
	
	// Check if the input file exists and open it if it does
	if ((fin = getFilePointer(inputFileName)) == NULL) {  
    	printf("Error opening input file \"%s\". Exiting...\n", inputFileName);
        exit(EXIT_FAILURE);          
    }

	// Read the content if the input file line by line
	int lineNum = 0;
    while((nread = getLineFromFile(fin, line, len)) != -1) {
    	// Create a new buffer in the heap as to not overwrite line buffer with packet use and operations
        char *packetLine = (char *)malloc(sizeof(char)*chunkSize);
    	memset(packetLine, '\0', chunkSize);
    	sprintf(packetLine, "%s", line);
    	
    	// Add a packet containing the line and the line's number to the shared queue
    	addNode(packetLine, lineNum++);
    }
    
    // End of file reached, add n special packets to the shared queue
    for (int i=0;i<numCons;i++) {
    	addNode(NULL, -1);
    }
    
    // Close the input file
    fclose(fin);
    
    // cleanup and exit
    free(line);
    return NULL; 
}
