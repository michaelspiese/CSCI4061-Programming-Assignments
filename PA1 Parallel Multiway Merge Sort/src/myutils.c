/* test machine: csel-vole-35
 * group number: G41
 * name: Michael Spiese, Jack Vehmeier, Isaac Hein
 * x500: spies046, vehme003, hein0540
 */

#include "myutils.h"

void printArray(int arr[], int size) {
    int i;
    for (i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void writeSortedResultToFile(char* myID, int arr[], int size) {
    char line[WriteBufferSize];
    char filepath[MaxFileNameLength];
    memset(line, '\0', WriteBufferSize);
    memset(filepath, '\0', MaxFileNameLength);
    sprintf(filepath, "output/%s.out", myID);

    int i;
    int idx = 0;
    idx += sprintf(&line[idx], "%d\n", size);
    for (i = 0; i < size; i++) {
        idx += sprintf(&line[idx], "%d\n", arr[i]);
    }
    writeLineToFile(filepath, line);                // Write data in the write buffer to the file
}

// Insertion sort for sorting leaf nodes
void insertionSort(char* myID, int arrSort[], int size) {
    int key, j;
    for (int i = 1; i<size; i++) {
    	key = arrSort[i];
    	for (j = i-1; arrSort[j]>key; j--) {
    	    arrSort[j+1] = arrSort[j];
    	}
    	arrSort[j+1] = key;
    }
    
    // Print required terminal output of leaf node sorting success
    printf("Process [%s] - Insertion Sort - Done\n", myID);
}


// Multiway Merge Sort with multiple data streams from intermediate files
void merge(char* myID, int nChild) {	
	char *line = (char *)malloc(sizeof(char) * LineBufferSize);         // Line buffer where a new line is stored
    size_t len = LineBufferSize;               // Maximum length of line buffer
    ssize_t nread;         // Bytes read by reading line from file
	
	// Create array and initialize all array values to number larger than maximum allowable data value
	int numsIn[MaxDegree][MaxDataSize];
	for (int i=0; i<MaxDegree; i++) {
		for (int j=0; j<MaxDataSize; j++) {
			numsIn[i][j] = 10001; 
		}
	}
	
	// Create array to save data length of each file
	int *nData = (int *)malloc(sizeof(int) * nChild);
	
	// Create and open current process's output filepath 
	FILE *curOut;
	char *curPath = (char *)malloc(sizeof(char) * MaxFileNameLength);
	sprintf(curPath, "output/%s.out", myID);
	curOut = fopen(curPath, "w");
	
	// Create array of file pointers and buffer initialized to NULL to hold filepaths
    FILE ** curIn = (FILE **)malloc(sizeof(FILE *) * nChild);
    char *tempChildPath = (char *)malloc(sizeof(char) * MaxFileNameLength);
    memset(tempChildPath, '\0', MaxFileNameLength);
    
    // Open all childrens' intermediate files and save inputs into memory
    for (int i=0; i<nChild; i++) {
    	// Format child filepath from ID of current node
    	if (strcmp(myID, "master") == 0) {
    	    sprintf(tempChildPath, "output/%d.out", i+1);
    	}
    	else {
    	    sprintf(tempChildPath, "output/%s%d.out", myID, i+1);
    	}
    	
    	// Open single intermediate file
    	if ((curIn[i] = getFilePointer(tempChildPath)) == NULL) { 
    	    printf("Error opening file with path \"%s\". Exiting...\n", tempChildPath);
        	exit(EXIT_FAILURE);
        }
            
        // Read first line of single intermediate file to grab data length
    	if((nread = getLineFromFile(curIn[i], line, len)) != -1) {  
            sscanf(line, "%d\n", &nData[i]);
        }
        
        // Grab all data from single intermediate file
        int aNumber;
        int idxInput = 0;
        while((nread = getLineFromFile(curIn[i], line, len)) != -1) {
                sscanf(line, "%d\n", &aNumber);
                numsIn[i][idxInput++] = aNumber;
        }
    }
    
    // Close all input files and free line and child path buffers from memory after last use	
    for (int i=0; i<nChild; i++) {
    	fclose(curIn[i]);
    }
    free(line);
    free(tempChildPath);
    	
    // Find the total number of data values to merge from all files
    int nTot = 0;
    for (int i = 0; i<nChild; i++) {
    	nTot += nData[i];
    }
    
    // Free data length array from memory after last use
    free(nData);
    	
    int *merged = (int *)malloc(sizeof(int) * nTot);      // Integer array to store final merged array
    int *counter = (int *)malloc(sizeof(int) * nChild);   // Integer array to store dynamic number of counters for merge operation
    memset(counter, 0, sizeof(int)*nChild);               // Initialize all subarray counters to 0
    	
    // k-dimensional merge from all childrens' data subarrays
    for (int mergeIdx = 0; mergeIdx<nTot; mergeIdx++) {
    	int smallest = 10001;
    	int smIdx = -1;
    	
    	// Find value and index of smallest element between all arrays
    	for (int j=0; j<nChild; j++) {
    		if (numsIn[j][counter[j]] < smallest) {    
    			smallest = numsIn[j][counter[j]];
    			smIdx = j;
    		}
    	}
    	
    	// Append next smallest value to merged array and increase associated counter
    	merged[mergeIdx] = smallest;     
    	counter[smIdx]++;        
    }
    free(counter); // Free memory used by counter array
		
	// Write final merged array to the output file and print required terminal output of merge success
	writeSortedResultToFile(myID, merged, nTot);
	printf("Process [%s] - Merge Sort - Done\n", myID);
	
	// Finished printing, free merged array and close output file
	free(merged);
    fclose(curOut);
}
