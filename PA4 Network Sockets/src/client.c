#include "client.h"

void printSyntax(){
    printf("incorrect usage syntax! \n");
    printf("usage: $ ./client input_filename server_addr server_port\n");
}

void register_acct(int sockfd, char *username, char *name, time_t birthday) {
	int amt = 0;
	msg_enum msg_type = REGISTER;
	
	// Send the REGISTER message to the server
	if(write(sockfd, &msg_type, sizeof(msg_enum)) != sizeof(msg_enum)) {
		printf("clientFunc failed to write msg_type.\n");
		printf("It wrote %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	
	// Send the username, name, and birthday of the new account to be registered to the server
	if ((amt = write(sockfd, username, MAX_STR)) != MAX_STR) {
		printf("register failed to write username.\n");
		printf("It read %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	if ((amt = write(sockfd, name, MAX_STR)) != MAX_STR) {
		printf("register failed to write name.\n");
		printf("It read %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	if ((amt = write(sockfd, &birthday, sizeof(time_t))) != sizeof(time_t)) {
		printf("register failed to write birthday.\n");
		printf("It read %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	
	// Read the response from the server after registering a new account
	// Must receive BALANCE message back from server
	msg_enum rsp_type;
	if ((amt = read(sockfd, &rsp_type, sizeof(msg_enum))) != sizeof(msg_enum)) {
		printf("register failed to read rsp_type.\n");
		printf("It read %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	if (rsp_type != BALANCE) {
		printf("incorrect response from server. Expected balance\n");
		exit(EXIT_FAILURE);
	}
	
	// Read the target account's account number from the server
	int acct_number;
	if ((amt = read(sockfd, &acct_number, sizeof(float))) != sizeof(float)) {
		printf("register failed to write acct_number.\n");
		printf("It wrote %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	
	// Read the balance of the new account (should be 0)
	float balance;
	if ((amt = read(sockfd, &balance, sizeof(float))) != sizeof(float)) {
		printf("register failed to read balance.\n");
		printf("It read %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
}

void get_account_info(int sockfd, int acct_number) {
	int amt = 0;
	
	// Send the GET_ACCOUNT_INFO message and the account number to the server
	msg_enum msg_type = GET_ACCOUNT_INFO;
	if(write(sockfd, &msg_type, sizeof(msg_enum)) != sizeof(msg_enum)) {
		printf("clientFunc failed to write msg_type.\n");
		printf("It wrote %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	if ((amt = write(sockfd, &acct_number, sizeof(int))) != sizeof(int)) {
		printf("get_account_info failed to write acct_number.\n");
		printf("It read %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	
	// Read response from server after asking for account info
	// Must receive back ACCOUNT_INFO message from server
	msg_enum rsp_type;
	if ((amt = read(sockfd, &rsp_type, sizeof(msg_enum))) != sizeof(msg_enum)) {
		printf("get_account_info failed to read acct_number.\n");
		printf("It read %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	if (rsp_type != ACCOUNT_INFO) {
		printf("incorrect response from server. Expected account_info\n");
		exit(EXIT_FAILURE);
	}
	
	// Read username, name, and birthday of target account
	char username[MAX_STR];
	char name[MAX_STR];
	time_t birthday;
	if ((amt = read(sockfd, &username, MAX_STR)) != MAX_STR) {
		printf("get_account_info failed to read username.\n");
		printf("It read %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	if ((amt = read(sockfd, &name, MAX_STR)) != MAX_STR) {
		printf("get_account_info failed to read name.\n");
		printf("It read %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	if ((amt = read(sockfd, &birthday, sizeof(time_t))) != sizeof(time_t)) {
		printf("get_account_info failed to read birthday.\n");
		printf("It read %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
}

float get_balance(int sockfd, int acct_number) {
	int amt = 0; // Stores number of bytes read/written
	
	// Send the GET_BALANCE message and target account number to the server
	msg_enum msg_type = GET_BALANCE;
	if(write(sockfd, &msg_type, sizeof(msg_enum)) != sizeof(msg_enum)) {
		printf("clientFunc failed to write msg_type.\n");
		printf("It wrote %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	if ((amt = write(sockfd, &acct_number, sizeof(int))) != sizeof(int)) {
		printf("get_balance failed to write acct_number.\n");
		printf("It wrote %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	
	// Read the response message from the server after requesting account balance
	// Must receive BALANCE message from server
	msg_enum rsp_type;
	if ((amt = read(sockfd, &rsp_type, sizeof(msg_enum))) != sizeof(msg_enum)) {
		printf("get_balance failed to read rsp_type.\n");
		printf("It read %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	if (rsp_type != BALANCE) {
		printf("incorrect response from server. Expected balance\n");
		exit(EXIT_FAILURE);
	}
	
	// Read the target account's account number from the server
	if ((amt = read(sockfd, &acct_number, sizeof(float))) != sizeof(float)) {
		printf("get_balance failed to write acct_number.\n");
		printf("It wrote %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	
	// Read the balance of the target account from the server
	float balance;
	if ((amt = read(sockfd, &balance, sizeof(float))) != sizeof(float)) {
		printf("get_balance failed to read balance.\n");
		printf("It read %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	
	return balance;
}

void request_cash(int sockfd, float amount) {
	int amt = 0;
	
	// Send REQUEST_CASH message and requested amount to the server
	msg_enum msg_type = REQUEST_CASH;
	if(write(sockfd, &msg_type, sizeof(msg_enum)) != sizeof(msg_enum)) {
		printf("clientFunc failed to write msg_type.\n");
		printf("It wrote %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	if ((amt = write(sockfd, &amount, sizeof(float))) != sizeof(float)) {
		printf("request_cash failed to write amount.\n");
		printf("It read %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	
	// Read the response from the server after cash request
	// Must receive CASH message from the server
	msg_enum rsp_type;
	if ((amt = read(sockfd, &rsp_type, sizeof(msg_enum))) != sizeof(msg_enum)) {
		printf("request_cash failed to read rsp_type.\n");
		printf("It read %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	if (rsp_type != CASH) {
		printf("incorrect response from server. Expected cash\n");
		exit(EXIT_FAILURE);
	}
	
	// Read the amount of cash given from server
	if ((amt = read(sockfd, &amount, sizeof(float))) != sizeof(float)) {
		printf("request_cash failed to read amount.\n");
		printf("It read %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	
	// Add received cash to reserve
	cash += amount;
}

void transact(int sockfd, int acct_number, float amount) { 
	int amt = 0; // Stores the number of bytes read/written

	// Send TRANSACT message, target account number, and transaction amount to server
	msg_enum msg_type = TRANSACT;
	if(write(sockfd, &msg_type, sizeof(msg_enum)) != sizeof(msg_enum)) {
		printf("clientFunc failed to write msg_type.\n");
		printf("It wrote %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	if ((amt = write(sockfd, &acct_number, sizeof(int))) != sizeof(int)) {
		printf("transact failed to write acct_number.\n");
		printf("It wrote %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	if ((amt = write(sockfd, &amount, sizeof(float))) != sizeof(float)) {
		printf("transact failed to write amount.\n");
		printf("It wrote %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	
	// Add/revove cash in branch cash reserve used for transation in withdrawal/deposit
	cash -= amount;

	// Read the response message from server after transaction
	// Must receive back BALANCE message 
	msg_enum rsp_type;
	if ((amt = read(sockfd, &rsp_type, sizeof(msg_enum))) != sizeof(msg_enum)) {
		printf("transact failed to read rsp_type.\n");
		printf("It wrote %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	if (rsp_type != BALANCE) {
		printf("incorrect response from server. Expected balance\n");
		exit(EXIT_FAILURE);
	}
	
	// Read the target account's account number from the server
	if ((amt = read(sockfd, &acct_number, sizeof(float))) != sizeof(float)) {
		printf("transact failed to write acct_number.\n");
		printf("It wrote %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	
	// Read the new balance of the target account after transaction
	float balance;
	if ((amt = read(sockfd, &balance, sizeof(float))) != sizeof(float)) {
		printf("transact failed to read balance.\n");
		printf("It read %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
}

void terminate(int sockfd) {
	int amt = 0; // Stores the number of bytes read/written
	
	// Send the terminate message to the server
	msg_enum msg_type = TERMINATE;
	if ((amt = write(sockfd, &msg_type, sizeof(msg_enum))) != sizeof(msg_enum)) {
		printf("failed to write termiate after file.\n");
		printf("It read %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	
	// Close the connection to the server socket from client-side
	close(sockfd);
}

void error(int sockfd, msg_enum msg_type) {
	int amt = 0;
	if (msg_type == ERROR)
		printf("[CLIENT]: Detected an error. Sending error to server...\n");
	else 
		printf("[CLIENT]: Received unknown message type %d. Sending error to server...\n", msg_type);
	
	// Send the error message to the server
	msg_type = ERROR;
	if ((amt = write(sockfd, &msg_type, sizeof(msg_enum))) != sizeof(msg_enum)) {
		printf("failed to write termiate after file.\n");
		printf("It read %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
}

int clientFunc(int sockfd, FILE * fp) {	
	// Variables to help store input file lines
	size_t len = 1024;
	char *line = (char *)malloc(sizeof(char)*1024);
	
	// Grab input file lines until EOF is received
	while(getLineFromFile(fp, line, len) != -1) {
	
		// Parse line for all information (msg_type, acct_number, name, username, birthday, amount, num_trans)
		char *numstr;
		numstr = strtok (line,",");
		msg_enum msg_type = strtol(numstr, NULL, 10);
		numstr = strtok(NULL,",");      // Advance to next comma  
		int acct_number = strtol(numstr, NULL, 10); 
		numstr = strtok(NULL,",");      // Advance to next comma  
		char *name = numstr;
		numstr = strtok(NULL,",");      // Advance to next comma 
		char *username = numstr;
		numstr = strtok(NULL,",");      // Advance to next comma  
		time_t birthday = strtol(numstr, NULL, 10); 
		numstr = strtok(NULL,",");      // Advance to next comma  
		float amount = strtod(numstr, NULL); 
		numstr = strtok(NULL,",");      // Advance to next comma  
		int num_trans = strtol(numstr, NULL, 10);
	
		// Act with correct protocol procedure based on msg_type of input file line
		switch(msg_type) {
			case REGISTER:
				register_acct(sockfd, username, name, birthday);
				break;
			
			case GET_ACCOUNT_INFO:
				get_account_info(sockfd, acct_number);
				break;
				
			case TRANSACT:
				// Check if target account balance has enough money to perform transaction
				if (amount < 0) {
					float balance = get_balance(sockfd, acct_number);
					if((balance + amount) < 0) {
						break;
					}	
				}
				// Check if client cash reserve has enough money to perform transaction
				// Request cash until the transaction can be performed
				else while (cash < amount) {
					float cash_recv = CASH_AMOUNT; 
					request_cash(sockfd, cash_recv);
				}
				
				// Perform transaction on target account balance
				transact(sockfd, acct_number, amount);
				break;
				
			case GET_BALANCE:
				get_balance(sockfd, acct_number);
				break;
				
			case REQUEST_CASH:
				request_cash(sockfd, amount);
				break;
				
			case TERMINATE:
				terminate(sockfd);
				return 0; 
				
			default:
				error(sockfd, msg_type);
		}
	}
	
	// Input file completely parsed, close socket and return 1
	terminate(sockfd);
	return 1;
}

int main(int argc, char *argv[]){
    // Argument handling
    if(argc != 4) {
        printSyntax();
        return EXIT_FAILURE;
    }
    
    // Format input filepath
    char *inputFileName = (char *)malloc(sizeof(char)*50);
    memset(inputFileName, '\0', sizeof(char)*50);
    sprintf(inputFileName, "input/%s", argv[1]);
    
    // Grab server address and port from command line
    char *server_addr = argv[2];
    int server_port = strtol(argv[3], NULL, 10);
    
    // Initialize local client cash reserve to 10000 dollars
    float cash = START_CASH;
    
    // Variables and structures for handling socket creation
    int sockfd;
    struct sockaddr_in servaddr, cli;
    
    // Grab time before working on input file
    time_t start = clock();
    
    // Open the input file
    FILE * fp; 
    if ((fp = fopen(inputFileName, "r")) == NULL) {
    	printf("Error opening input file \"%s\". Exiting...\n", inputFileName);
        exit(EXIT_FAILURE);          
    }

    // Clear servaddr struct and assign IP and PORT to client socket
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(server_addr);
    servaddr.sin_port = htons(server_port);
    
    // While the input file has not been completely parsed
    do {
    	// Create and verify new client socket
    	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    	if (sockfd == -1) {
        	printf("Socket creation failed...\n");
        	exit(EXIT_FAILURE);
    	}
    
    	// Connect to server socket
    	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) != 0) {
        	printf("Reconnection with the server failed...\n");
        	exit(EXIT_FAILURE);
    	}
    // If clientFunc returns 0, there are still more lines to parse
    } while(!clientFunc(sockfd, fp));
    
    // Close the input file
    fclose(fp);
    
    // Calculate and print the time to complete work on input file
    float elapsed = ((float)(clock() - start)/CLOCKS_PER_SEC);
    printf("Elapsed Time: %.2f\n", elapsed);

    return EXIT_SUCCESS; 
}

