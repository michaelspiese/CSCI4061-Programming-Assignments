/* test machine: csel-vole-35
 * group number: G41
 * name: Michael Spiese, Jack Vehmeier, Isaac Hein
 * x500: spies046, vehme003, hein0540
 */

#include "myutils.h"

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Less number of arguments.\n");
        printf("./master InputFileName\n");
        fflush(stdout);
        exit(EXIT_FAILURE);
    }

    // Please keep this function call in the main.
    // This function initializes "output" folder
    // by deleting the folder and all its files,
    // and then creating a clean "output" folder.
    bookeepingCode();

    // Parse input file
    FILE * fp;                                                      // File pointer to input file
    char *line = (char *)malloc(sizeof(char) * LineBufferSize);         // Line buffer where a new line is stored
    size_t len = LineBufferSize;                                        // The size of the line buffer
    ssize_t nread;                                                  // Length of a new line read

    char inputFileName[MaxFileNameLength];                          // Buffer to store input file name
    memset(inputFileName, '\0', MaxFileNameLength);                 // Initializing all chars in buffer to '\0'
    sprintf(inputFileName, "input/%s", argv[1]);                    // Prepending input file directory

    if ((fp = getFilePointer(inputFileName)) == NULL) {             // Open a file and return file pointer to the file
    	printf("Error opening input file \"%s\". Exiting...\n", inputFileName);
        exit(EXIT_FAILURE);          
    }

    // Read the number of data and depth
    int nData;
    int depth = 0;
    if((nread = getLineFromFile(fp, line, len)) != -1) {            // Read next line and write it to line buffer
        sscanf(line, "%d %d\n", &nData, &depth);
    }
    
    // Handling invalid number of data error
    if (nData == 0) {
    	printf("ERROR: Assumption violation: N. Data (%d) should be between 1 and 1000.\n", nData);
    	exit(EXIT_FAILURE);
    }
    
    // Handling depth out of bounds error
    if (depth > 9) {
    	printf("ERROR: Assumption violation: Depth (%d) is greater than 9.\n", depth);
    	exit(EXIT_FAILURE);
    }
    
    // Read degrees of each level up to maximum allowed depth of 9
    int * nDeg = (int *)malloc(sizeof(int) * depth);
    if ((nread = getLineFromFile(fp, line, len)) != -1) {
    	sscanf(line, "%d %d %d %d %d %d %d %d %d", &nDeg[0], &nDeg[1], &nDeg[2], &nDeg[3], &nDeg[4], &nDeg[5], &nDeg[6], &nDeg[7], &nDeg[8]);
    }
    
    // Calculating number of leaves and handling degree out of bounds error
    int nLeaves = 1;
    for (int i=0; i<depth; i++) {
    	if ((nDeg[i] < 1) || (nDeg[i] > 9)) {
    		printf("ERROR: Assumption violation: Degree (%d) of a depth (%d) should be between 1 and 9.\n", nDeg[i], i);
    		exit(EXIT_FAILURE);
    	}
    	nLeaves = nDeg[i]*nLeaves;
	}
	
	// Handling number of leaf nodes greater than number of data error
	if (nLeaves > nData) {
		printf("ERROR: Assumption violation: N. leaf nodes (%d) is greater than N. Data (%d).\n", nLeaves, nData);
		exit(EXIT_FAILURE);
	}
    
    // Read input data
    int * input = (int *)malloc(sizeof(int) * nData);
    int aNumber;
    int idxInput = 0;
    while((nread = getLineFromFile(fp, line, len)) != -1) {
        sscanf(line, "%d\n", &aNumber);
        input[idxInput++] = aNumber;
    }

    free(line);
    fclose(fp);

    // Spawn child processes and launch childProgram if necessary
    if (depth > 0) {
        pid_t pid;
        pid_t terminated_pid;
    
    	char *id = (char *)malloc(sizeof(char) * MaxDepth); // ID of current process
    	char *parID = (char *)malloc(sizeof(char) * MaxDepth); // ID of the parent's process
    	
    	char curDepth[2]; // Strings to hold current depth, maximum depth, data size, start subarray index and end subarray index
    	char maxDepth[2];
    	char len[5];
    	char startIdx[4];
    	char endIdx[4];
    	
    	int cCheck = 0; // Variable to keep track of if process is not master
    	int *procStat;
    	
    	int offset = 0; // Integers to calculate subarray indexes
    	int s = 0;
    	int e = nData - 1;
    	
    	sprintf(maxDepth, "%d", depth); // Format string of maximum data length
        
        // Create process tree using tree depth and number of degrees at each depth
        for(int i=0; i<depth; i++){ 
    	    for(int j=0; j<nDeg[i]; j++){
    	    
                pid = fork(); // Always fork at beginning of loop
               
                // If process off fork is a child, calculate its parameters
    	        if (pid == 0) {
    	            
    	            // Format strings of current process ID, parent ID, and current process depth
    	            sprintf(parID, "%s", id);   // Parent ID is the same as ID from previous process
    	            sprintf(id, "%s%d", id, j+1);
    	            sprintf(curDepth, "%d", i+1);
    	            
    	            // If current degree is 1
    	            if (nDeg[i] == 1) {
    	            	nData = nData;
    	            	s = s;
    	            	e = e;
    	            }
    	            // Find current process subarray data length and start/end indexes f current node is a grey node and the current degree is not 1
    	            else if (j == nDeg[i]-1) { 
    	                int mod = (nData-1) % nDeg[i];
    	            	nData = nData - (nDeg[i]-1)*floor(nData/nDeg[i]);
    	            	s = e - nData + 1;
    	            	offset = offset + (nData-1) + mod;   // Update offset created by existence of grey nodes in current tree path
    	            } 
    	            // Find process subarray data length and start/end indexes if current node is a white node
    	            else {
    	            	nData = floor(nData/nDeg[i]);
    	            	s = j * nData + offset;
    	            	e = s + nData - 1;
    	            }

                    // Format strings of current process data length, start index, and end index
    	            sprintf(len, "%d", nData);
    	            sprintf(startIdx, "%d", s);         
    	            sprintf(endIdx, "%d", e);
    	            
    	            // Required terminal output
    	            if (strcmp(parID, "") == 0) {
    	            	printf("Parent [master] - Spawn Child [%s, %s, %s, %s, %s]\n", curDepth, id, startIdx, endIdx, len);
    	            }
    	            else printf("Parent [%s] - Spawn Child [%s, %s, %s, %s, %s]\n", parID, curDepth, id, startIdx, endIdx, len);
    	            
    		    cCheck = 1;   // Current is a process in a lower depth than master
    		    break;        // Do not fork off of children until next outer loop iteration
    	        }
    	        
    	        else if (pid < 0) {
    	        	printf("Error creating process. Exiting...\n");
    	        	exit(EXIT_FAILURE);
    	        }
    	    }
    	    
    	    // Break out of inner loop is current process is a parent to stop forking
    	    if (pid > 0) {
    	        break;                 
    	    }
        }
    
    	// Wait for all child processes to current process to terminate
        while((terminated_pid = wait(procStat)) > 0) {
        	if (terminated_pid == EXIT_FAILURE) {
        		exit(EXIT_FAILURE);
        	}
        }
   
        // Execute childProgram if process is not the master program
        if (cCheck == 1) {
    	    execl("childProgram", "childProgram", curDepth, id, parID, startIdx, endIdx, len, inputFileName, maxDepth, NULL);
            printf("Error executing execl()\n");
            exit(0);
        }
    
    // Free ID buffers from memory after calling 
    free(parID);
    free(id);
    }
    
    // If master is not a leaf, merge the intermediate files from the master's children
    if (depth != 0) {
        merge("master", nDeg[0]);
    }
    // Perform insertion sort and write input array to output file if master is itself a leaf
    else {
        insertionSort("master", input, nData);
        writeSortedResultToFile("master", input, nData);
    }
    
    // Free input and degree arrays and master's ID from memory after last use
    free(input);
    free(nDeg);

    return EXIT_SUCCESS;
}
