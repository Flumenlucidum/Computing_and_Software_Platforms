#define _CRT_SECURE_NO_WARNINGS
#define MAX 10000
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <errno.h>
/*
-Array + characters and strings
-Pointers
-Multidimensional Array
-Structures and Unions
-File I/O
*/

void swap(int a, int b);
void swap_addr(int* a, int* b);
void changeArray(int* ptr);
struct scholar {
	char* name;
	int yearborn;
	char* major;

	struct scholar* friend;
};
int main_syn2(void) {
	/*Array
	Index starts from zero
	The values must be initialized first. Otherwise the array has dummy values.
	The size of array must be a constant, not a variable.
	*/
	int arr[10] = { 1,2,3,4,5,6,7,8,9,10 };
	int arr2[] = { 1,2,3 }; // size of 3
	float arr3[5] = { 1.0f,2.0f,3.0f }; // Other values are zero

	/*
	Multidimensional Array
	int arr[m][n][o]
	indexing->Think of row, column of matrix

	Ragged Arrays: array of pointers whose elemetns are used to point to arrays of varying sizes
	*/
	int arr5[2][5] = { {1,2,3,4,5},{1,2,3,4,5} };  //2 arrays with 5 elements 
	char* ragged[2] = { "abc","abcde" };

/*Character and string
-String is a one-dimensional array of type char, which is a POINTER.
-Unmutable because it is in literal pool
-There is null character '\0' at the end of a string.
-double quotes

*/
	char c = 'A';
	printf("%c\n", c);

	char str[7] = "coding"; //[c][o][d][i][n][g][\0]
	printf("%s\n", str);
	char str2[] = "coding";
	printf("%s\n", str2); //coding
	printf("%d\n", sizeof(str2)); //7

	char korean[] = "Åë°èÇÐ";
	printf("%s\n", korean);
	printf("%d\n", sizeof(korean)); 
	//3 letters but size of 7 because an Eng character needs one byte
	//but a Korean character needs two bytes.
	//Each index had value "?"

	char* string = "abc";
	printf("%s %s\n", string, string + 1); // abc bc
	"abc"[2]; //c
	*("abc" + 2);//c

	char c_array[10] = { 'c','o','d','e' };
	//Other indexes are filled with null characters.
	for (int i = 0; i < sizeof(c_array); i++)
	{
		printf("%d\n", c_array[i]);
	} 
	//Prints ASCII code of each character, Null char \0 is 0 in ASCII.
	//ASCII - standard coding system by ANSI, US
	//7 bits 0~127
	/*for (int i = 0; i < 128; i++)
	{
		printf("ASCII number %d : %c\n", i, i);
	}
	*/


	/*Some String-handling functions in stdlib.h
	1) char* strcat(char* s1,const char* s2): Concatenation on s1 
	2) int strcmp(const char *s1, const char *s2) : lexicographical comparison 
	3) char* strcpy(char* s1, const char* s2) : Characters in s2 is copied to s1 until \0 is moved and return s1
	4)size_t strlen(const char* s) : the number of characters before \0
	*/

	/*Pointer
	used to access memory and manipulate addresses (reference to a specific memory location)
	int *x  defines x which is a pointer to a integer.
	void *v generic pointer type
	x=&a (address of normal variable a)
	Dereferencing (obtaining the value pointed to by a pointer)
	a=*x (assign a with value in the memory where x points to)
	*x=a (change the value where x points to to a)
	if a null pointer is deferenced and segmentation fault occurs.
	%p in printf

	ARRAY NAME is a CONSTANT POINTER (its value cannot be changed)
	arr==&arr[0], the value of arr is the address of its first element
	*arr==*&arr[0]  value of the first element
	
	ARRAY arr[i]==*(arr+i)
	POINTER *(ptr+i)==ptr[i]

	Array as function argument
	double sum(double x[], int n) x[] pointer and int n is limit 
	*/

	int i = 4, j = 6, * p = &i, * q = &j;
	//*p=&i is NOT assignment, but declaration value of p is the address of i
	//**&p  -> *(&p)=p -> 4 

	p = (int*)3; // address as 3 
	double a[10], * p2, * q2; 
	p2 = a; //CAUTION p points to the first element of a
	q2 = p2 + 2; //q points to the 3rd element of a
	printf("%d\n", q2 - p2); //2
	printf("%d\n", (int)q2 - (int) p2); //16 1020-1004
	//(int) this thing is called typecasting, changing the data type of the variables
	
	int A[100], i, * p, sum = 0;

	for (p = A; p < &A[100]; p++)
	{
		sum += *p;
	}
	for (i = 0; i < 100; i++)
	{
		sum += *(A + i);
	}
	for (p = A, i = 0; i < 100; i++)
	{
		sum += p[i];
	}


	/*
	Swap function 
	Nothing is changed because of call by value.
	Memory address of a is different inside and outside the swap function.
	Need to to swapping with pointer

	NOTICE : Even when the pointer does not exist anymore, the fact that something has some address does not change.
	*/
	int a = 10, b = 20;

	printf("Before swap, a= %d, b= %d\n", a, b);
	swap(a, b);
	printf("After swap, a= %d, b= %d\n", a, b);
	
	printf("Before swap_addr, a= %d, b= %d\n", a, b);
	swap_addr(&a, &b);
	printf("After swap_addr, a= %d, b= %d\n", a, b);

	//Changing elements of an array 
	int arr4[3] = { 10,20,30 };
	changeArray(arr4); //changeArray(&arr2[0]) does the same thing.
	for (int i = 0; i < 3; i++)
	{
		printf("arr4 value number %d is %d\n", i + 1, arr4[i]);
	}


	/*Structures- collection of one or more variable(members) 
	Declaration above int main()
	sch1=sch2 -> copy all members of sch2 to sch1
	
	Unions-> the same syntax as that of structures
	but members share storage
	*/
	struct scholar sch1;
	struct scholar sch2;
	sch1.name = "Einstein";
	sch1.yearborn = 1879;
	sch1.major = "Physics";
	sch2.name = "Fisher";
	sch2.yearborn = 1890;
	sch2.major = "Genetics, Statisitcs, etc";

	//initialize
	struct scholar sch3 = { "Kim",1994,"Data Science" };

	//Array of structures
	struct scholar QuantumGroup[3] = { {"Bohr",1885,"Physics"},{"Heisenberg",1901,"Physics"},{"Kim",1970,"Physics"} };

	//Struct Pointer   (*ptr).name,  ptr->name
	struct scholar* schptr1;
	struct scholar* schptr2;
	schptr1 = &sch1;
	schptr2 = &sch2;
	printf("%s\n",(*schptr1).name);
	printf("%s\n", schptr2->name);

	/*+ typedef
	typedef char sval8b  explicity associate a type with an identifier
	typedef my_int int
	typedef struct scholar Prof
	typedef struct{} scholar
	*/


	/*File I/O 
	Stream : a source of input or a destination of output
	Each line in a text stream is terminated by '\n'
	
	fputs fgets usually used for string 
	fprintf fscanf -> used for certain format e.g. %d 
	long ftell(FILE* stream) : return current file position for stream
	int fseek(FILE* stream, long offset, int origin) : Change the file reading position
	origin may be changed to SEEK_SET(beginning), SEEK_CUR(current), SEEK_END(end of the file)

	*/
	
	char line[MAX];
	//Write a file 
	FILE* file = fopen("C:\\Users\\main\\Desktop\\test1.txt","wb"); //\\ as escape for \ mode(r w a) file type(t text b binary) 
	//fopen may be unsafe
	if (file == NULL)
	{
		printf("file open failed\n");
		return 1;
	}

	fputs("Let's write using fputs\n", file);
	fputs("Please check if it's ok\n", file);

	fclose(file); //Save and close the file. 

	//Read the file
	FILE* file2 = fopen("C:\\Users\\main\\Desktop\\test1.txt", "rb"); //\\ as escape for \ mode(r w a) file type(t text b binary) 
	//fopen may be unsafe
	if (file2 == NULL)
	{
		printf("file open failed\n");
		return 1;
	}

	//Reads at most the next n-1 characters into array line 
	//Stops if a newline is encountered
	while (fgets(line, MAX, file2) != NULL) 
	{
		printf("%s", line);
	}

	fclose(file2); //Save and close the file

	//fprintf fscanf
	int num[6] = { 0,0,0,0,0,0 };
	int bonus = 0;
	char str3[MAX];
	char str4[MAX];
	FILE* file3 = fopen("C:\\Users\\main\\Desktop\\test2.txt", "wb");
	if (file3 == NULL)
	{
		printf("file open failed\n");
		return 1;
	}

	fprintf(file3, "%s %d %d %d %d %d %d\n", "Lottery_number", 1, 2, 3, 4, 5, 6);
	fprintf(file3, "%s %d\n", "Bonus_number", 7);
	fclose(file3);
	
	FILE* file4 = fopen("C:\\Users\\main\\Desktop\\test2.txt", "rb");
	if (file4 == NULL)
	{
		printf("file open failed\n");
		return 1;
	}

	fscanf(file4, "%s %d %d %d %d %d %d", str3, &num[0], &num[1], &num[2], &num[3], &num[4], &num[5]);
	printf("%s %d %d %d %d %d %d\n", str3, num[0], num[1], num[2], num[3], num[4], num[5]);

	fscanf(file4, "%s %d", str4, &bonus);
	printf("%s %d\n", str4, bonus);

	fclose(file4);
	
	
	FILE* file5;
	file5= fopen("C:\\Users\\main\\Desktop\\test_alphabet.txt", "wb"); 
	char* alphabet = "abcdefghijklmnopqrstuvwxyz";
	char buf[30];
	if(file5 == NULL);
	{
		printf("file open failed\n");
		printf("Error %s\n", strerror(errno));
		return 1;
	}
	fputs("alphabet\n", file5);
	fputs(alphabet, file5);
	fseek(file5, 0, SEEK_SET);
	fgets(buf, 30, file5);
	printf("%s\n", buf);
	fclose(file5);

	return 0;
}
void swap(int a, int b)
{
	int temp=a;
	a = b;
	b = temp;
	printf("Within swap, a= %d, b= %d\n", a, b);
}
void swap_addr(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}
void changeArray(int* ptr)
{
	ptr[2] = 50;
}
