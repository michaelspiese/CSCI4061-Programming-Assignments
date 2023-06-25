#include "server.h"

void printSyntax(){
    printf("incorrect usage syntax! \n");
    printf("usage: $ ./server server_addr server_port num_workers\n");
}

void *bLog (void *arg) {
	while(1) {
		// Open the output file
		FILE * fpo = fopen("output/balances.csv", "w");
		
		// Grab lock to prevent log race condition and log all accounts' information
		for (int i=0; i<numAccs; i++) {
			pthread_mutex_lock(&balances[i].lock);
			fprintf(fpo, "%d,%.2f,%s,%s,%ld\n",i,balances[i].balance,balances[i].name,balances[i].username,balances[i].birthday);
			pthread_mutex_unlock(&balances[i].lock);
		}
		
		// Close the output file
		fclose(fpo);
		
		// Wait 5 seconds before logging data again
		sleep(LOG_SLEEP);
	}
}

void register_acct(int sockfd) {
	int amt = 0; // Stores the number of bytes read/written

	// Read the new account's info from the client (username, name and birthday)
	char username[MAX_STR];
	if ((amt = read(sockfd, &username, MAX_STR)) != MAX_STR) {
		printf("register failed to read username.\n");
		printf("It read %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	char name[MAX_STR];
	if ((amt = read(sockfd, &name, MAX_STR)) != MAX_STR) {
		printf("register failed to read name.\n");
		printf("It read %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	time_t birthday;
	if ((amt = read(sockfd, &birthday, sizeof(time_t))) != sizeof(time_t)) {
		printf("register failed to read birthday.\n");
		printf("It read %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	
	// Acquire the account lock before registering new account
    pthread_mutex_lock(&balances[numAccs].lock);
	
	// Create new account from username, name and birthday. Initially set balance to 0
	strcpy(balances[numAccs].username, username);
	strcpy(balances[numAccs].name, name);
	balances[numAccs].birthday = birthday;
	balances[numAccs].balance = 0;
	balances[numAccs].registered = 1;
	int acct_number = numAccs;
	
	// Signal to all threads waiting for the account to be registered
	pthread_cond_broadcast(&balances[numAccs].cond);
	pthread_mutex_unlock(&balances[numAccs].lock);
	
	// Increment the number of registered accounts
	numAccs++;
	
	// Must send back BALANCE message to client
	msg_enum rsp_type = BALANCE;
	if ((amt = write(sockfd, &rsp_type, sizeof(msg_enum))) != sizeof(msg_enum)) {
		printf("register failed to write rsp_type.\n");
		printf("It wrote %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	
	// Send the target account's account number back to the client 
	if ((amt = write(sockfd, &acct_number, sizeof(float))) != sizeof(float)) {
		printf("register failed to write acct_number.\n");
		printf("It wrote %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	
	// Send the initial balance of the new account to the client
	float balance = 0;
	if ((amt = write(sockfd, &balance, sizeof(float))) != sizeof(float)) {
		printf("register failed to write balance.\n");
		printf("It wrote %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
}

void get_account_info(int sockfd) {
	int amt = 0; // Stores the number of bytes read/written
	
	// Read the target account number from the client
	int acct_number;
	if ((amt = read(sockfd, &acct_number, sizeof(int))) != sizeof(int)) {
		printf("get_account_info failed to read acct_number.\n");
		printf("It read %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	
	// Acquire the lock and wait until the account has been registered before accessing account info
	pthread_mutex_lock(&balances[acct_number].lock);
	while (balances[acct_number].registered == 0)
		pthread_cond_wait(&balances[acct_number].cond, &balances[acct_number].lock);
	
	// Grab the target account information and release the lock
	char *username = balances[acct_number].username;
	char *name = balances[acct_number].name;
	time_t birthday = balances[acct_number].birthday;
	pthread_mutex_unlock(&balances[acct_number].lock);
			
	// Must send back ACCOUNT_INFO message to client
	msg_enum rsp_type = ACCOUNT_INFO;
	if ((amt = write(sockfd, &rsp_type, sizeof(int))) != sizeof(int)) {
		printf("get_account_info failed to write rsp_type.\n");
		printf("It wrote %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	
	// Send the account info back to the client
	if ((amt = write(sockfd, username, MAX_STR)) != MAX_STR) {
		printf("get_account_info failed to write username.\n");
		printf("It wrote %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	if ((amt = write(sockfd, name, MAX_STR)) != MAX_STR) {
		printf("get_account_info failed to write name.\n");
		printf("It wrote %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	if ((amt = write(sockfd, &birthday, sizeof(time_t))) != sizeof(time_t)) {
		printf("get_account_info failed to write birthday.\n");
		printf("It wrote %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
}

void transact (int sockfd) {
	int amt = 0; // Stores the number of bytes read/written
	
	// Read the target account information from the server
	int acct_number;
	if ((amt = read(sockfd, &acct_number, sizeof(int))) != sizeof(int)) {
		printf("transact failed to read acct_number.\n");
		printf("It read %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	
	// Read the transaction amount from the server
	float amount;
	if ((amt = read(sockfd, &amount, sizeof(float))) != sizeof(float)) {
		printf("transact failed to read amount.\n");
		printf("It read %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	
	// Acquire the lock and wait until account has been registered before accessing account information
	pthread_mutex_lock(&balances[acct_number].lock);
	while (balances[acct_number].registered == 0)
		pthread_cond_wait(&balances[acct_number].cond, &balances[acct_number].lock);
	
	// Make the transaction on the account's balance, store the new balance, and release the account's lock
	balances[acct_number].balance += amount;
	float newBalance = balances[acct_number].balance;
	pthread_mutex_unlock(&balances[acct_number].lock);
	
	// Must send back BALANCE to client
	msg_enum rsp_type = BALANCE;
	if ((amt = write(sockfd, &rsp_type, sizeof(msg_enum))) != sizeof(msg_enum)) {
		printf("transact failed to write rsp_type.\n");
		printf("It wrote %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	
	// Send the target account's account number back to the client 
	if ((amt = write(sockfd, &acct_number, sizeof(float))) != sizeof(float)) {
		printf("transact failed to write acct_number.\n");
		printf("It wrote %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	
	// Send the new balance of the target account to the client
	if ((amt = write(sockfd, &newBalance, sizeof(float))) != sizeof(float)) {
		printf("transact failed to write balance.\n");
		printf("It wrote %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
}

void get_balance(int sockfd) {
	int amt = 0; // Store number of bytes read/written
	
	// Read the target account number
	int acct_number;
	if ((amt = read(sockfd, &acct_number, sizeof(int))) != sizeof(int)) {
		printf("get_balance failed to read acct_number.\n");
		printf("It read %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	
	// Acquire lock and wait until account has been registered before accessing account information
	pthread_mutex_lock(&balances[acct_number].lock);
	while (balances[acct_number].registered == 0)
		pthread_cond_wait(&balances[acct_number].cond, &balances[acct_number].lock);
	
	// Grab the target account's current balance and release the account's lock
	float balance = balances[acct_number].balance;
	pthread_mutex_unlock(&balances[acct_number].lock);
	
	// Must send back BALANCE message to client
	msg_enum rsp_type = BALANCE;
	if ((amt = write(sockfd, &rsp_type, sizeof(int))) != sizeof(int)) {
		printf("get_balance failed to write rsp_type.\n");
		printf("It wrote %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	
	// Send the target account's account number back to the client 
	if ((amt = write(sockfd, &acct_number, sizeof(float))) != sizeof(float)) {
		printf("get_balance failed to write acct_number.\n");
		printf("It wrote %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	
	// Send the target account's new balance back to the client 
	if ((amt = write(sockfd, &balance, sizeof(float))) != sizeof(float)) {
		printf("get_balance failed to write balance.\n");
		printf("It wrote %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
}

void request_cash(int sockfd) {
	int amt = 0; // Stores number of bytes read/written
	
	// Read the amount of cast to send back
	float amount;
	if ((amt = read(sockfd, &amount, sizeof(float))) != sizeof(float)) {
		printf("request_cash failed to read amount.\n");
		printf("It read %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	
	// Must respond back with CASH message
	msg_enum rsp_type = CASH;
	if ((amt = write(sockfd, &rsp_type, sizeof(int))) != sizeof(int)) {
		printf("request_cash failed to write rsp_type.\n");
		printf("It wrote %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
	
	//Always return requested amount back to the client
	if ((amt = write(sockfd, &amount, sizeof(float))) != sizeof(float)) {		
		printf("request_cash failed to write amount.\n");
		printf("It wrote %d bytes.\n", amt);
		exit(EXIT_FAILURE);
	}
}

void * serverFunc(void *arg) {
	int sockfd = *(int *) arg; // Connection socket file descriptor
	int amt = 0; // Stores number of bytes read/written
	msg_enum msg_type; // Message type from client
	
	while(1) {
		// Read the incoming message type from the client
		if ((amt = read(sockfd, &msg_type, sizeof(msg_enum))) != sizeof(msg_enum)) {
			printf("serverFunc failed to read msg_type.\n");
			printf("It read %d bytes.\n", amt);
			exit(EXIT_FAILURE);
		}
		
		// Act based on the client's message
		switch(msg_type) {
			case REGISTER:
				register_acct(sockfd);
				break;
			
			case GET_ACCOUNT_INFO:
				get_account_info(sockfd);
				break;
				
			case TRANSACT:
				transact(sockfd);
				break;
				
			case GET_BALANCE:
				get_balance(sockfd);
				break;
			
			case REQUEST_CASH:
				request_cash(sockfd);
				break;
				
			case ERROR:
				// Print error message to terminal
				printf("[SERVER]: Received error from client.\n");
				break;
				
			case TERMINATE:
				// Close socket connection and return to release thread
				close(sockfd);
				return (void *) EXIT_SUCCESS;
					  
			default:
				// Print error message to terminal
				printf("[SERVER]: Unknown message type %d received fron client.\n", msg_type);
			
		}
	}
}

int main(int argc, char *argv[]){
    // argument handling
    if(argc != 4) {
        printSyntax();
        return EXIT_FAILURE;
    }
    
    // Store inputs from command-line
    char *server_addr = argv[1];
    int server_port = strtol(argv[2], NULL, 10);
    int num_workers = strtol(argv[3], NULL, 10);
    
    // create empty output folder
    bookeepingCode();
    
    // Create thread to log account info
    pthread_t logTid;
    if (pthread_create(&logTid, NULL, bLog, NULL) != 0) {
    	printf("server failed to create thread %ld.\n", logTid);
    	printf("Server exiting...\n");
        exit(EXIT_FAILURE);
    }

	// Variables and structures for handling socket creation
    int sockfd;
    struct sockaddr_in servaddr, cli;
    int len = sizeof(cli);
    
    // Clear servaddr struct and assign IP and PORT to server socket
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(server_port);

	// Create the server-side socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed. Server exiting...\n");
        exit(EXIT_FAILURE);
    }
    
    // Handle server bind error for successive runs
    int enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    	printf("setsockopt(SO_REUSEADDR) failed");
    
    // Bind server to the specified port from the command-line
    if ((bind(sockfd, (SA *) &servaddr, sizeof(servaddr))) != 0) {
        printf("Socket bind failed. Server exiting...\n");
        exit(EXIT_FAILURE);
    }

	// Make the server socket passive
    if ((listen(sockfd, num_workers)) != 0) { 
        printf("Listen failed. Server exiting...\n");
        exit(EXIT_FAILURE);
    }
    
    // Initialize global variables used for performing actions on user-accounts
    numAccs = 0;
    struct account balances[MAX_ACC];
    for (int i = 0; i < MAX_ACC; i++) {
    	memset(balances[i].name, '\0', MAX_STR);
    	memset(balances[i].username, '\0', MAX_STR);
    	balances[i].registered = 0;
    	pthread_mutex_init(&balances[i].lock, NULL);
    	pthread_cond_init(&balances[i].cond, NULL);
    }
    
    // Continuously accept client sockets and create a new thread to handle them
    pthread_t tid; // Variable to temporarily store tid of new server thread
    int connfd_arr[1023]; // Large array to handle socket file descriptors of new clients
    int idx = 0;
    while(1) {
    	// Wait and connect to incoming clients
    	if ((connfd_arr[idx] = accept(sockfd, (SA *) &cli, &len)) < 0) {
    		printf("server failed to accept connection with client.\n");
    		printf("Server exiting...\n");
        	exit(EXIT_FAILURE);
    	}
    	
    	// Create new thread to handle client
    	if (pthread_create(&tid, NULL, serverFunc, (void *) &connfd_arr[idx]) != 0) {
    		printf("server failed to create thread %ld.\n", tid);
    		printf("Server exiting...\n");
        	exit(EXIT_FAILURE);
    	}
    	idx++;
    }
    
    // Never reached by server
    return EXIT_SUCCESS; 
}

