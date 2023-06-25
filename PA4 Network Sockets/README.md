
[//]: # test machine: csel-kh4250-49 
[//]: # group number: G41
[//]: # name: Michael Spiese, Jack Vehmeier, Isaac Hein 
[//]: # x500: spies046, vehme003, hein0540

# Project 4 : Network Sockets
### Project Group Number: 41
### Group members: Michael Spiese (spies046), Jack Vehmeier (vehme003), Isaac Hein (hein0540) 

## Extra Credit
The extra credit problems of a thread pool and transaction history were not implemented in this submission.

## Individual Contributions
Due to the complexity of this project, all work was done remotely as a group despite what is stated on our interim report.

## Assumptions
Outside of those in the manual and the macros defined in utils.h, the only assumption made in this submissionis 
that there will never be more that 1023 server threads at once.

## Compiling the Program
Compilation of the program was done with a Makefile. First, to ensure the executable program files
are up to date, the following terminal command removes all object files and executables from the
current directory:

	> make clean
	
The "make clean" command is analogous to running the following terminal command:

	> rm server client launcher lib/utils.o

Next, to compile all source files into the files needed to run the program, the following command is used:

	> make
	
The "make" command is analagous to running the following terminal commands in order:
	
	> gcc -pthread -g -Iinclude -c src/utils.c -o lib/utils.o
	> gcc -pthread -g -Iinclude lib/utils.o src/client.c -o client
	> gcc -pthread -g -Iinclude lib/utils.o src/server.c -o server
	> gcc -pthread -g -Iinclude lib/utils.o src/launcher.c -o launcher

## Execution
To execute a single testcase of the program with a single terminal, one of the following commands is used:

	> make t1
	> make t2
	> make t3
	
The "make t[1-3]" command first runs the following command:

	> ./launcher server_addr server_port num_workers input_file [input_file] [input_file] ...

./launcher is a given helper program that launches both the individual processes running ./server and ./client with a given server address, server port, 
number of worker threads (for unimplemented extra credit problem) and the input files for the clients. At least one input file must be given The ./server 
program creates a socket on the given port and local address and waits for a client before making a new thread to service the client. The server also creates 
a log file in a seperate thread to show all account information. The ./client program connects to the server and performs various banking requests that are 
serviced by the server. After 31 seconds, ./launcher signals the server process to terminate.

Next, each of the make t[1-3] commands also has a built in compare function that compares the balance.csv generated by the server process to a file that was 
given to us in the expected folder. It should contain the information (account number, balance, name, username, and birthday) of all customers in the server's 
database. Since test cases 2 and 3 share the same input files, the output of these cases are both compared to expected/balances2.csv. It compares the two txt 
files with the built in command diff which then echos the word "passed" if the diff does not find any differences in the two .txts. This is represented by the 
following terminal commands: 

	> diff expected/balances[1-2].csv output/balances.csv
	> @echo passed


So all together individual make t[1-3] calls would be analagous to the following terminal commands in order:

	make t1:
	> ./launcher 127.0.0.1 9047 1 input1.csv
	> diff expected/balances1.csv output/balances.csv
	> @echo passed

	make t2:
	> ./launcher 127.0.0.1 9047 1 input2_0.csv input2_1.csv input2_2.csv input2_3.csv input2_4.csv input2_5.csv
	> diff expected/balances2.csv output/balances.csv
	> @echo passed

	make t3:
	> ./launcher 127.0.0.1 9047 8 input2_0.csv input2_1.csv input2_2.csv input2_3.csv input2_4.csv input2_5.csv
	> diff expected/balances2.csv output/balances.csv
	> @echo passed


## Results
The results of each test case are saved in a log file called balances.csv in the output directory. This file contains the account info (account number,
balance, name, username, and birthday) of every registered user in the database printed on individual lines in the following format:

	[account number],[balance],[name],[username],[birthday]

Additionally, although no results are printed to the terminal in the program, the test cases echo the word passed to show that the final results match the 
expected results.
	
