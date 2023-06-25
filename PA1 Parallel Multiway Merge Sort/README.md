/* test machine: csel-vole-35
 * group number: G41
 * name: Michael Spiese, Jack Vehmeier, Isaac Hein
 * x500: spies046, vehme003, hein0540
 */

# Project 1 : Parallel Multiway Merge Sort
### Project Group Number: 41
### Group members: Michael Spiese (spies046), Jack Vehmeier (vehme003), Isaac Hein (hein0540) 

## Individual Contributions
All work on this project was done together as a group remotely. Michael shared his screen and typed
while Jack and Isaac helped problem solve and look up syntax for some parts of the code.

## Leaf-Node Sorting Algorithm
The leaf-node sorting algorithm we used was Insertion Sort because of the simplicity of the code
and because time-complexity was not a concern with the amount of data sorted in this project.

## Assumptions
Outside of those in the manual, no other assumptions were made about the data or project.

## Compiling the Program
Compilation of the program was done with a Makefile. First, to ensure the executable program files
are up to date, the following terminal command removes all object files and executables from the
current directory:

	> make clean
	
The "make clean" command is analogous to running the following terminal commands in order:

	> rm lib/myutils.o master childProgram
	> rm -rf output
	> rm -rf *.dSYM

Next, to compile all source files into the files needed to run the program, the following command is used:

	> make
	
The "make" command is analagous to running the following terminal commands in order:
	
	> gcc -g -Iinclude -c src/myutils.c -o lib/myutils.o
	> gcc -g -Iinclude src/childprogram.c lib/utils.o lib/myutils.o -o childProgram
	> gcc -g -Iinclude src/master.c lib/utils.o lib/myutils.o -o master

## Execution
Finally, to execute the program with an input file, one of the following commands is used:

	> make run1
	> make run2 
	...
	> make run9
	
The "make run[#1-9]" command is analagous to running one of the following commands in the terminal:

	> ./master input1.file
	> ./master input2.file
	...
	> ./master input9.file

## Results
Check "output" folder for all output files from each process. Every output file's name is of the form 
"[Process ID].out" where [Process ID] is the ID given to the process as a result of it's position in 
the process tree. You can compare the output files here with the ones in the "expected" folder.
