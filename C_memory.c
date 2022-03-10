#include <stdio.h>
#include <stdlib.h>
/*
-Memory Structure
-Storage Classes
-Dynamic Memory Allocation
*/

int a, b, c; //access available from other codes by extern int a
int main(void)
{
	/*Von Neumann Architecture
	-Input and output devices, Main memory, and CPU (CU/ALU)

	-C program is converted to a program for machine instructions
	--> Machine instructions use registers
	-->Registers : a type of computer memory usable immediately by the CPU
	X32 X64 is the size of the register

	-Subroutines : a sequence of instructions
	--> Called from multple places
	--Most CPUs include support for stacks
	--Stack pointer points to the top of the stack
	--Link register holds the address to return to when a function call completes.
	*/

	/*
	Storage Classes
	1)Auto - default : variables declared within function bodies and blocks
	-Temporary storage for the lifetime of the function of block (stack frame)
	auto int a, b, c;
	2)Extern : Variables declared outside a function
	-Permanent storage for the lifetime of the program
	-All functions have external storage class
	-extern tells the compiler to look for variables in this file or other files
	3)Register - tells the compiler that the variable should be stored in a register if possible
	4)Static - to make a local variable to retain its value when the block is reentered 
	-foo.c code has 
	static int count=0; This is not accessable from other file
	-static int foo(void) : static function -> visible only within the file 
	*/

	/*
	Dynamic memory allocation
	calloc() and malloc() --stdlib.h
	1)void* calloc(size_t n, size_t s) -> returns a pointer
	-allocates contiguous space in memory (nxs bytes)
	-initialized with 0
	2)void* malloc(size_t s)
	-allocates contiguous space in memory (s bytes)
	-NO Initialization
	3)void* realloc(void* ptr,size_t s)
	-Allocate memory again
	*/
	void* x;
	int n = 100;
	x = calloc(n, sizeof(int));
	x = malloc(n * sizeof(int));
	//Explicitly return the space
	free(x); //Garbage collector in Python 
	//x must be the base address

	int* ptr = NULL;
	ptr = (int*)malloc(4); //4 bytes allocated
	*ptr = 1234; //Use the memory
	free(ptr); //deallocate the memory space
	//BUT still the ptr points to 1234. It just means the space can be overwritten now
	ptr = NULL; //So we need to initialize the pointer  
	
	return 0;
}