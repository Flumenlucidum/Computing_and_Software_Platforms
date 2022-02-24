#include <stdio.h>
#include <time.h>
#include <stdlib.h>
/*
-C language
-Data types
-Condition and Loop
-Functions
*/

/*
	Compiler : a program that translates a program to machine code
	Source code to executable object program (binary file)
	gcc (GNU compiler collection) used mainly in Linux OS
	foo.c -> preprocessing -> compile -> foo.o -> linking with libraries -> foo
	
	C preprocessor (cpp)
	#include <stdio.h> make the preprocessor to includ a copy of the header file
	main() -> Every C program has this function 
	--> arguments int main(int argc, char *argv[])
	-->argc: the number of arguments, argv an array of strings (words of command line)
	OS calls this main() to execute the program
	int main (void) : Takes no argument and return 0 : no error

	OS loader loads the application program into the main memory like RAM and initialize execution environments
	Control is transferred to the main() CU brings the statements from the memory and decode and send them to ALU.
	After the return in main, control is returned to OS

	*/
//Declaration of function
void func_without_return();
int func_with_return();
void func_without_params();
int main_syntax(void) {
	
	printf("Hello World \n");

	/*Variables : Named memory location
	Assignment : Storing the value of expression in a variable using = 
	Comments  by /*  //  -> CPP changes these into a single blank character
	Key Words e.g. auto break do for goto -> cannot be redefined  or modified
	Simple statements ;
	{Compound statements;}

	Operators 
	a++ do the statement and add one when going to next line 
	++a add first and go next line 
	+= /= possible
	Relational Operators < > <= >=  Equality op == != Result 0(False) or 1(True)(int)
	Logical operator && and || or    ! negation 

	goto label; control is unconditionally transferred to a labeled statement.
	label : {
	} 

	Call by Value always! when a=4, a is not changed, value is passed to function.
	Safe but needs more memory

	Declaration
	type identifier    int a,b,c;    float x,y=3.14;

	Data Types 
	1)char : small interger values 1byte (8 bits, 256 distinct values, ASCII character code)
	char is defined as signed char or unsigned char depending on compiler
	signed : -128~127  unsigned 0~255
	Escape letter is used in C \n \t \v \0 etc

	2)int: 4bytes 2^32  -2^31<=a<=2^31 -1 
	integer overflow can occur but the program continues 

	short : 2-bytes integer -2^15<=a<=2^15 -1
	long : 4-bytes integer, compiler may provide more storage than int
	unsigned : 4-bytes unsigned integer 0<=a<=2^32 -1 

	3)Floating types : hold real values
	float : 4bytes, 6 decimal places 2^32
	double : 8bytes, 15 decimal placeds
	long double : 8 bytes , compiler may provide more storage than double 

	4)Enumeration constants
	: a data type consisting of a set of named values called elements
	enum day { mon=1,tue,wed,thu, fri} p,q,r;

	*/
	
	//Integers
	int age = 12;
	printf("%d\n", age);
	age = 13;
	printf("%d\n", age);
	printf("1\n");

	//float -- write f in the end
	float f = 46.5f;
	printf("%f\n", f); //46.50000
	printf("%.2f\n", f); //46.50

	double d = 4.428;
	printf("%.2lf\n", d); //4.43.

	//Constants
	const int YEAR = 1994;
	printf("The year you are born is : %d\n", YEAR);

	int add = 3 + 7; 
	printf("3+7=%d\n", add);
	printf("%d x %d =%d\n", 3, 7, 3 * 7);
	/*
	//scanf 
	int input;
	printf("Please enter any integer value:");
	scanf_s("%d", &input);
	printf("You entered %d\n", input);

	int one, two, three;
	printf("Enter three integers : ");
	scanf_s("%d %d %d", &one, &two, &three); // Put space between them
	printf("%d\n", one);
	printf("%d\n", two);
	printf("%d\n", three);
	*/
	/*Character(single) and String(more than one) 
	character enclosed in single-quote marks
	string enclosed in double-quote marks
	"abc" "def" -> cpp -> "abcdef"
	*/
	/*char c = 'A';
	printf("%c\n", c);
	char str[256]; //A string of 256 characters
	scanf_s("%s", str, sizeof(str));
	printf("%s\n", str);
	*/
	/*
	Loop
	for(declaration;condition;change){}

	while(condition){}

	do {} while(condition);
	*/

	for (int i = 1; i <= 10; i++)
	{
		printf("Number %d\n", i);
	}
	int i = 1;
	while (i<=10)
	{
		printf("Number while %d\n", i++);
		//i++;
	}

	do {
		printf("Number do while %d\n", i++);
	} while (i <= 20);

	for (int i = 1; i <= 3; i++)
	{
		printf("first %d\n", i);
		for (int j = 1; j <= 3; j++)
		{
			printf("	second %d\n", j);
		}
	}

	/*
	Condition
	if()
	else  -> attaches to the nearest if

	break -get out of for loop

	continue - go to next number in for loop
	*/

	age = 15;
	if (age >= 20)
	{
		printf("You are an adult.\n");
	}
	else if (age>=10)
	{
		printf("A teenager\n");
	}
	else
	{
		printf("A child\n");
	}

	for (int i = 1; i <= 30; i++)
	{
		if (i >= 10)
		{
			printf("Go home!\n");
			break;
		}
		printf("Student number %d\n", i);

	}

	for (int i = 1; i <= 30; i++)
	{
		if (i >= 6 && i <= 10)
		{
			if (i == 7)
			{
				printf("Lucky number %d\n", i);
				continue;
			}
			printf("Number %d\n", i);
		}
	}

	/*Random from time.h
	Switch case*/

	srand(time(NULL)); //Initialize random number
	int num = rand() % 3; // 0,1,2   %3+1 1,2,3 
	switch (num)
	{
	case 0: printf("Scissors\n"); break;
	case 1: printf("Rock\n"); break;
	case 2: printf("Paper\n"); break;
	default: printf("No idea\n"); break;
	}
	
	/*
	Function 
	returning_type function_name (input_type)
	nothing->void
	declaration befor main and define after return 0

	types of function
	1) Without return 2) With return 
	1) Without parameter 2) With parameter
	*/

	func_without_return();
	int ret = func_with_return(3, 8);
	printf("%d\n", ret);

	func_without_params();

	return 0;
}

void func_without_return()
{
	printf("No Return\n");
}
int func_with_return(int num1, int num2)
{
	return num1 + num2;
}
void func_without_params()
{
	printf("No Params\n");
}
