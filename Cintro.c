#include <stdio.h>

static int Twice(int num) 
{
	int result = num * 3;
	result = result - num;
	return(result);
};
void swap(int x, int y) {
	int temp;
	temp = x;
	x = y;
	y = temp;

};
static void swap2(int* x, int* y) {
	int temp;
	temp = *x;
	*x = *y;
	*y = temp;
};
void foo();
int main2() /* main function which returns interger */
{
	printf("Hello World! \n");
	/*basic data type such as char int short unsigned long -> in the class material*/
	int a;
	a = 10;
	printf("The value of a is : %d \n", a);

	int b;
	b = 127;
	printf("The value of b in base %d is %o. \n", 8, b);
	printf("The value of b in base %d is %d. \n", 10, b);
	printf("The value of b in base %d is %x. \n", 16, b);


	/* Truncation moves a value from a type to a smaller type  */
	char ch;
	int i;
	i = 325;
	ch = i;
	double pi;
	int c;
	pi = 3.14159;
	c = pi;
	printf("%c, %d \n", ch, c);

	/* var++ or -- post variant
	++ or --var prevariant*/

	/*Logical operator ! not && and || or
	bitwise operator ~ negation & |  ^ (exclusively or 1 when they are different)
	>> Right shift (divide by power of 2)  left part is filled with the left number at the beginning
	<< left shift (multiply by power of 2) right part is filled with zero*/

	/*assignment operator
	+= -= *= %= /=  >>= &= etc. */

	float d = 3.141592f;
	double e = 3.141592;
	printf("d : %f \n", d);
	printf("e : %f \n", e);

	printf("%f \n", 1.3902);

	printf("%5.4f \n", 1048333.39966);
	printf("%d %% %d is %d \n", 10, 5, 10 % 5); /* %% means % in string*/


	/*character*/
	char j;
	j = 'joy';
	printf("character: %c, number: %d \n", j, j);

	/*Celsius to Fahrenheit*/
	double celsius;
	printf("This is a program which turns celsius to fahrenheit\n");
	printf("Pleas Enter Celsius temperature : ");
	scanf_s("%lf", &celsius);//Gets the celsius double is through LF//
	printf("%.2f degree celsius is %.2f degree fahrenheit \n", celsius, 9 * celsius / 5 + 32);


	int h;
	printf("Enter a number you want : ");
	scanf_s("%d", &h);
	if (h == 1)
	{
		printf("You're still my number one. \n");

	}
	printf("You selected number %d \n", h);

	/*if -else */

	int num;
	printf("How old are you? :");
	scanf_s("%d", &num);
	if (num < 20) {
		printf("you may not enter this place");
	}
	else
	{
		printf("You are %d years old, you may go. \n", num);
	}

	int num1,sum=0;
	for (num=0; num<=22; num++)
	{
		sum += num;
	}
	printf("sum of the number : %d", sum);

	//or statement
	int x =4;
	int y = 5;
	int min;
	min = (x < y) ? x : y;
	printf("%d", min);
	//Switch statement
	int answer;
	printf("What is capital of Sweden?\n");
	printf("1.Helsinki\n");
	printf("2. Oslo\n ");
	printf("3. Stockholm\n");
	printf("Please enter the number :");
	scanf_s("%d", &answer);

	switch (answer)
	{
	case 1:
		printf("It is Capital of Finland.\n");
		break;
	case 2:
		printf(" It is Capital of Norway. \n");
		break;
	case 3:
		printf("Correct!\n");
	default:
		printf("Wrong input\n");

	}

	//while statement
	int a1 = 1;
	while (a1 < 10) {
		int b1 = 1;

		while (b1 < 10) {
			printf("%d * %d = %d\n", a1, b1, a1 * b1);
			b1++;

		}
		a1++;
		printf("\n");
}
	//break works with loop and switch, continue moves the control to the bottom of the statement.

	/*******Section 3 ********/
	/***Complex Data Types****/
	//structures=array + records

	struct fraction {
		int numerator;
		int denominator;
	};
	struct fraction f1, f2;
	f1.numerator = 7;
	f1.denominator = 22;
	f2 = f1;

	//Arrays
	int scores[100]; //array of 100 integers possible
	scores[0] = 13;
	scores[99] = 58;

//the name of the array works as a pointer to starting point of array
	//Multidimensional Array
	int board[10][10];
	board[0][0] = 99;
	board[9][9] = 1;

	//Array of structs
	struct fraction numbers[1000];
	numbers[0].numerator = 22;
	numbers[0].denominator = 7;
	
	// Pointer -This saves the address number of variables

	int* intptr;
	char* charptr;
	struct fraction* ff1, * ff2;
	//ff1 - struct fraction*
	//*ff1 - struct fraction 
	// (*ff1).numerator -> int
	struct fraction** fp; //a pointer to a pointer to fp
	struct fraction fract_array[20];
	struct fraction* fract_ptr_array[20];

	// Avoid circular definition problem (referring to itself)

	struct node {
		int data;
		struct node* next;
	}; //node in a linked list

/*& operator sets the pointer to point at something (variable which takes up space in a stack or heap (LValue)*/
	/*void foo();*/
	/*foo(){
		int* pointer; //pointer to a integer
		int pointee; //integer
		pointer = &pointee; //set p to point to i
		*pointer = 13; // change what p points to to 13

	}*/ //pointer's liftime is when the foo is executing
 
	// pointer pointing at nothing is assigned with value 0 and work as false in boolean context.
	//1) Pointer and pointee declared and allocated
	//2) pointer must be initialized so that it points to pointee.
	//Even without 2) the pointer works fine but the pointer will misuse the memory

	int* pointer;
	int pointee;
	struct fraction fff1;

	pointer = &pointee;
	*pointer = 42;
	pointer = &(fff1.numerator);
	*pointer = 22;
	pointer = &(fff1.denominator);
	*pointer = 7;

	/*C string*/
	/* Cstring is an array of char with the null character '/0' at th end*/
	// = is not copying so library function is needed
	{
		char localstring[10];
		strcpy(localstring, "binky");
	}
	// b i n k y 0 X X X X

	{char string[1000];
	int len;
	strcpy(string, "binky");
	len = strlen(string);
	/*Reverse the chars in the string. i goes forward, j goes backward*/
	int i, j;
	char temp;
	for (i = 0, j = len - 1; i < j; i++, j--) {
		temp = string[i];
		string[i] = string[j];
		string[j] = temp;
	}
	}

	/*Lack of large enough strings causes many kinds of bugs so char is usually handled by the pointer not the array itself.*/
	/* Typedef makes a synonym for the type name */

	typedef struct treenode* Tree;
	struct treenode {
		int data;
		Tree smaller, larger;
	};


	/**********Section 4 Functions*****************/
	//example
	/*static int Twice(int num); {
		int result = num * 3;
		result = result - num;
		return(result);
	};*/
	//static : only available to the callers in the file where it is declared
	//int - type of returning value 
	// num and result are 'local' to the function and get 'local memory'

	int num2 = 13;
	int a2 = 1;
	int b2;
	b2 = Twice(num2);
	printf("%d", b2);

	/*
	The expression passed to a function in the file is "actual parameter." The expression inside function is
	"formal parameter."*/
	/*C passes parameter "by value." Caller and callee each has their own copy of the actual parameter value.*/
	/*1) Modification to Callee is not communicated back to the caller.*/
	/*2) When value is large, copying is undesirable.*/
	/* by reference : Pass the POINTER to the Value*/

	// void swap function
	int a3 = 1;
	int b3 = 2;
	swap(a3, b3);
	/*This function does not work because the swapping a and b means the 
	change in the local memory x, y temp*/

	//Reference parameter technique
/*static void swap2(int* x, int* y) {
	int temp;
	temp = *x;
	*x = *y;
	*y = temp;
}*/
	int a4=1;
	int b4 = 2;
	swap2(&a4, &b4);
	//Formal parameter by *.  Caller use & to compute pointers to its local memory (a4 b4)
	//Callee dereference formal parameter pointer back to 
	//get the caller's memory. & gets the address of variable.

	/****Section 5. Odds and Ends***/
	/* C program use Prototypes and preprocessors
	to make separate file to cooperate and compiler to work on each file independently.*/
	// Prototypes
	/* 1) static function - used only in the file below declaration
	: no need to make separate prototype*/
	/*2) Non-static function : needs prototype definition*/
	//Preprocessor
    // #define -symbolic replacement  
    //#include - brings in text from different file   "foo.h" user file <foo.h> system file
	/* foo.h contains prototypes for foo.c.
	# if -> select place for and not for compiling
	#assert -check the bounds*/
	

	/**********Section 6. Advanced Arrays and Pointers************/

	{
		int array[6];
		int sum1 = 0;
		sum1 += array[0] + array[1];
	}
	
	/* 4*6 bytes and from the base address, the address number moves by 4 bytes  */
	
	
	int intArray[6];
	intArray + 3;//it is a pointer to the thrid element
		// but it is dereferenced already  intArray +3 is same as (&(intArray[3]))
	//intArray[3] is same as *(intArray +3)

	// Pointer ++    p is pointer and p+1 is pointer to the next element in the array
	//p++ is possible;
		/*

		void strcpy1(char dest[], const char source []){
		int i =0;
		while (1){
		dest[i]=source[i];
		if (dest[i]=='\0') break;
		i++;
		}
		}*/
		/* void strcpy4(char dest[], const char source[])
		 {
		 while (*dest++ = *source**);
		 }
		 */

		 /* Pointer Type effect */
	int* pointer2;
	pointer2 = 30;
	pointer2 = pointer2 + 12;
	// this means incrementing pointer2 by 12 bytes
	//(size of ) char is always 1byte   
	pointer2 = (int*)(((char*)pointer2) + 12); //cast pointer back to int

	/* Array and Pointers*/
	
	{
		int intArray2[6];
		int* intPtr;
		int index;

		intPtr = &index;
		intArray2[3] = 13; //ok
		intPtr[0] = 12; //odd but ok- > changes i to 12
		intPtr[3] = 13; //BAD -> no change here 
	}
	//Array names: pointer to the base address cannot be changed in the code
	{
		int ints[100];
		int* p;
		int index2;
		//ints goes to the base address

		/*ints=NULL; NO
		ints =&i ; NO
		ints= ints +1 ; NO
		ints ++ NO

		p=ints; OK
		p++, p=NULL, p=&i OK

		//Heap Memory 
		void* malloc(size_t size)
		void free(void* block)
		void* realloc(void* block, size_t size) */
	}
	
	//Dynamic Array
	//allocate array in the heap using malloc
	{
		int arr[1000];
		int* bbb;
		bbb = (int*)malloc(sizeof(int) * 1000);
		assert(b != NULL); //check success in allocation

		arr[123] = 13;
		bbb[123] = 13;
		free(bbb);


		/**Advantages for being in the heap**/
		// Size can be defined at runtime
		//array will exist until deallocation with free()
		// change the size of the array at will at runtime with realloc()
		bbb = realloc(b, sizeof(int) * 2000);
		assert(bbb != NULL);
	
		/*Disadvantages*/
		//need to keep track of allocation 


		//Dynamic Strings
		/*   char* MakeStringHeap(const char* source) {
			char* newString;
			newString = (char*)malloc(strlen(source) + 1); // +1 for the '\0'
			assert(newString != NULL);
			strcpy(newString, source);
			return(newString);
		} */
	
	}
	

	/********Section7. Details and library functions********/

	/*  stdio.h file inputand output
		ctype.h character tests
		string.h string operations
		math.h mathematical functions such as sin() and cos()
		stdlib.h utility functions such as malloc() and rand()
		assert.h the assert() debugging macro
		stdarg.h support for functions with variable numbers of arguments
		setjmp.h support for non - local flow control jumps
		signal.h support for exceptional condition signals
		time.h dateand time
		limits.h, float.h constants which define type range values such as INT_MAX
		*/

	//Specific Function on Essential C by Nick Parlante



	return 0;/* return 1 means the program has problems*/

	}

	
