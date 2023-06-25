/* test machine: csel-vole-35
 * group number: G41
 * name: Michael Spiese, Jack Vehmeier, Isaac Hein
 * x500: spies046, vehme003, hein0540
 */

#include "myutils.h"

int main(int argc, char *argv[]) {

	// Check if number of arguments passed into program is correct
    if (argc < 9) {
        printf("Less number of arguments.\n");
        printf("./childProgram myDepth ID parID startIdx endIdx myDataLen InputFileName maxDepth\n");
        fflush(stdout);
        exit(EXIT_FAILURE);
    }

    // Convert command line arguments to more usable strings and integers
    int myDepth = strtol(argv[1], NULL, 10);
    int myID = strtol(argv[2], NULL, 10);
    char* strMyID = argv[2];
    int myStartIdx = strtol(argv[4], NULL, 10);
    int myEndIdx = strtol(argv[5], NULL, 10);
    int myDataLen = strtol(argv[6], NULL, 10);
    char* inputFileName = argv[7];
    int maxDepth = strtol(argv[8], NULL, 10);

    // Open the input file [input#].file
    FILE *fpi;
    if ((fpi = getFilePointer(inputFileName)) == NULL) {             // Open a file and return file pointer to the file
    	printf("Error opening input file. Exiting...\n");
        exit(EXIT_FAILURE);
    }
    
    // Open the output file [currentProcessID].out
    FILE *fpo;
    char path[25];
    sprintf(path, "output/%d.out", myID);
    fpo = fopen(path, "w");
    
    char *line = (char *)malloc(sizeof(char) * LineBufferSize);         // Line buffer where a new line is stored
    size_t len = LineBufferSize;                                        // Size of the line buffer
    ssize_t nread;                                                      // Number of bytes read from file
    
    // Read the number of data and depth
    int nData;
    int depth = 0;
    if((nread = getLineFromFile(fpi, line, len)) != -1) {            // Read next line and write it to line buffer
        sscanf(line, "%d %d\n", &nData, &depth);
    }
    
    // Read degrees of each level
    int * nDeg = (int *)malloc(sizeof(int) * depth);
    if ((nread = getLineFromFile(fpi, line, len)) != -1) {
    	sscanf(line, "%d %d %d %d %d %d %d %d %d", &nDeg[0], &nDeg[1], &nDeg[2], &nDeg[3], &nDeg[4], &nDeg[5], &nDeg[6], &nDeg[7], &nDeg[8]);
    }
    
    // Read input data from file
    int * input = (int *)malloc(sizeof(int) * nData);
    int aNumber;
    int idxInput = 0;
    while((nread = getLineFromFile(fpi, line, len)) != -1) {
        sscanf(line, "%d\n", &aNumber);
        input[idxInput++] = aNumber;
    }
    
    free(line);  // Free memory taken up by line input buffer
    fclose(fpi);    // Close the input file
    
    // If current process is a leaf, create subarray from indexes and perform insertionSort
    if (myDepth == maxDepth) {
        int * subArray = (int *)malloc(sizeof(int) * myDataLen);
    	for (int i = myStartIdx; i<=myEndIdx; i++) {
    	    subArray[i-myStartIdx] = input[i];
    	}
    	
    	insertionSort(strMyID, subArray, myDataLen);
    	
    	// Write sorted subarray to output file
    	writeSortedResultToFile(strMyID, subArray, myDataLen);
    	free(subArray);
    }
    // If not leaf, merge childrens' file data
    else {
	merge(strMyID, nDeg[myDepth]);
    }
    
    // Release full input and degree arrays from memory
    free(input);
    free(nDeg);
    
    // Close output file
    fclose(fpo);

    return EXIT_SUCCESS;
}
