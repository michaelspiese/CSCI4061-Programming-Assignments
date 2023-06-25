#ifndef SERVER_H
#define SERVER_H

#include "utils.h"

// User account structure
struct account {
	char username[MAX_STR];    // Username of account user
	char name[MAX_STR];    // Name of account user
	time_t birthday;    // Birthday of account user
	float balance;    // Amount of money stored in account
	pthread_mutex_t lock;    // Lock for accessing account info
	pthread_cond_t cond;    // Conditional variable for waiting on account registration
	int registered;    // Integer to verify account registration
};

// The global running total of registered accounts (0-indexed)
int numAccs;

// Global array of user account structures
struct account balances[MAX_ACC];

/**
 * Handles incorrect usage of program from the command-line
 *
 * @return none
 */
void printSyntax();

/**
 * Continuously logs the account information of every registered account in an output file.
 *
 * @return none
 */
void * bLog (void *arg);

/**
 * Receives a request to register a new account along with the new account's name, username, and
 * birthday, and sends back the BALANCE message with the account's number and the initial balance
 * of the account.
 *
 * @return none
 */
void register_acc(int sockfd);

/**
 * Receives a request for an account's information and the target account number, and sends back
 * the ACCOUNT_INFO message with account's name, username, and birthday.
 *
 * @return none
 */
void get_account_info(int sockfd);

/**
 * Receives a request to make a transaction on an account, the target account number, and the
 * transaction amount, and sends back the BALANCE message, the account number, and the new
 * balance of the account.
 *
 * @return none
 */
void transact (int sockfd);

/**
 * Receives a request for an account's balance the target account number, and sends back the
 * BALANCE message, the account number, and the balance of the account.
 *
 * @return none
 */
void get_balance(int sockfd);

/**
 * Recieves a request for some amount of cash, and sends back the CASH message along with the
 * amount of cash requested.
 *
 * @return none
 */
void request_cash(int sockfd);

/**
 * Function run by new threads that handles the connection with each client over the socket.
 *
 * @return none
 */
void * serverFunc(void *arg);

#endif

