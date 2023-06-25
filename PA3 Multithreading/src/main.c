/* test machine: csel-kh4250-49
 * group number: G41
 * name: Michael Spiese, Jack Vehmeier, Isaac Hein
 * x500: spies046, vehme003, hein0540
 */

#include "header.h"

/**
 * Write final balance to a single file with filepath "output/result.txt"
 */
void writeBalanceToFiles(void) {
    // Open output file, exit with failure if file open fails
    FILE * fout;
    if ((fout = fopen(finalDir, "w")) == NULL) {
    	printf("Error opening input file \"output/result.txt\". Exiting...\n");
        exit(EXIT_FAILURE);          
    }
    
    // Write balance for each customer and calculate total balance change
    double tot;
    for (int i=0;i<acctsNum;i++) {
    	fprintf(fout, "%d\t%lf\n", i, balance[i]);
    	tot += balance[i];
    }
    
    // Write the total balance change
    fprintf(fout, "All: \t%lf\n", tot);
    
    // Close the output file
    fclose(fout);
}

int main(int argc, char *argv[]){
    // Check if the given command-line arguments are in the correct format
    if ((argc < 3) || (argc > 5)) {
    	fprintf(stderr, "ERROR: %d arguments received, expected 3\n", argc);
    	fprintf(stderr, "Input format: ./bank #Consumers fileName [Flag] [queueSize]\n");
    	exit(EXIT_FAILURE);
    }
    
    numCons = strtol(argv[1], NULL, 10); // The number of consumers to generate
    char *inputFileName = argv[2];       // The input filepath
    int queueSize = 0;                   // If given flag given, will store the size of the bounded queue
    
    // Check validity of optional arguments if given
    bound = 0;
    doLog = 0;
    if (argc >= 4) {
    	// Check if optional flag is defined. Catch unknown flag error.
    	if (strcmp(argv[3], "-p") == 0) {
    		doLog = 1;
    	}
    	else if (strcmp(argv[3], "-b") == 0) {
    		bound = 1;
    	}
    	else if (strcmp(argv[3], "-bp") == 0) {
    		doLog = 1;
    		bound = 1;
    	}
    	else {
    		fprintf(stderr, "Unknown flag \"%s\" given. Expected \"-p\", \"-b\", or \"-bp\".\n", argv[3]);
			exit(EXIT_FAILURE);
    	}
			
		// Check if queue size is defined correctly. Catch invalid queue size error.
		if (argc == 5) {
			queueSize = strtol(argv[4], NULL, 10);
			if (queueSize <= 0) {
				fprintf(stderr, "Incorrect queue size \"%s\" given. Expected a positive integer.\n", argv[4]);
				exit(EXIT_FAILURE);
			}
		}
	}
	
	// Initialize output directory
    bookeepingCode();
	
	// Open the file "output/log.txt" if flag is given as argument. Catch file open error.
	if (doLog == 1) {
		if ((flog = fopen(logDir, "w")) == NULL) {   
    		printf("Error opening input file \"output/log.txt\". Exiting...\n");
        	exit(EXIT_FAILURE);          
    	}
	}

    
    // Initialize global variables, like shared queue and semaphores
    head = NULL; // Initialize head and tail of empty shared queue to NULL
    tail = NULL;
    sem_init(&bufferSem, 0, 1); // Semaphore used for shared queue exclusive access
    sem_init(&balanceSem, 0, 1); // Semaphore used for balance array exclusive access
    sem_init(&bCountSem, 0, 0); // Semaphore used for counting packets in the shared queue
    if (bound == 1)
    	sem_init(&boundSem, 0, queueSize); // Semaphore used for bounded buffer upper bound
    pthread_t prodTid;                  // Thread id of the single producer thread
    pthread_t consTid[numCons];      // Array of consumer thread ids
    
	// Create producer thread, send input filepath as argument
	if(pthread_create(&prodTid, NULL, producer, (void *) inputFileName) != 0) {
            printf("Producer thread failed to create\n");
            exit(EXIT_FAILURE);
    }
    if (doLog == 1)
    	fprintf(flog, "producer\n"); // Print "producer" log output if flag is set
    
    //Create consumer threads, send consumer index as argument
    int idx[numCons];
	for (int i = 0; i < numCons; i++) {
		idx[i] = i; // Array is used to prevent race condition on loop index changing before use in thread
		if(pthread_create(&consTid[i], NULL, consumer, (void *) &idx[i]) != 0) {
            printf("Consumer thread %d failed to create\n", i);
            exit(EXIT_FAILURE);
        }
		if (doLog == 1) 
			fprintf(flog, "consumer %d\n", i); // Print "consumer [consumer #]" log output if flag is set
	}

    // Join and wait for all threads to complete execution
    pthread_join(prodTid, NULL);
    
    for(int i = 0; i < numCons; i++) {
        pthread_join(consTid[i], NULL);
    }
    
    //Write the final output
    writeBalanceToFiles();
    
    return EXIT_SUCCESS; 
}
