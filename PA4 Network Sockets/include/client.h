#ifndef CLIENT_H
#define CLIENT_H

#include "utils.h"

// Variable to store the local cash reserve
float cash;

/**
 * Handles incorrect usage of program from the command-line
 *
 * @return none
 */
void printSyntax();

/**
 * Sends REGISTER message and username, name and birthday of new account. Reads back the
 * new account number and the initial balance of the account from the server.
 *
 * @return none
 */
void register_acct(int sockfd, char *username, char *name, time_t birthday);

/**
 * Sends GET_ACCOUNT_INFO message and the target account number to server and reads back the 
 * account information of the account from the server
 *
 * @return none
 */
void get_account_info(int sockfd, int acct_number);

/**
 * Sends GET_BALANCE message and the target account number to server and reads back the 
 * account number and the current account balance
 *
 * @return none
 */
float get_balance(int sockfd, int acct_number);

/**
 * Sends REQUEST_CASH message to server, reads back the cash granted by the server, and updates
 * the local cash reserve
 *
 * @return none
 */
void request_cash(int sockfd, float amount);

/**
 * Sends TRANSACT message, account number, and transaction amount to server, and reads back the 
 * account number and the new balance
 *
 * @return none
 */
void transact(int sockfd, int acct_number, float amount);

/**
 * Sends TERMINATE message to server and closes the connection with the server from the client side
 *
 * @return none
 */
void terminate(int sockfd);

/**
 * Prints error message to terminal and sends ERROR message to server
 *
 * @param sockfd    The socket file descriptor between the client and server
 * @param msg_type    The message type read from input file
 * @return none
 */
void error(int sockfd, msg_enum msg_type);

/**
 * Parses the next line of the input file, chooses which action from the protocol to use on the account info
 *
 * @param sockfd    The socket file descriptor between the client and server
 * @param fp    The file pointer of the input file
 * @return 0 if socket connection terminated before EOF, 1 if socket connection terminated aftef EOF
 */
int clientFunc(int sockfd, FILE * fp);

#endif
