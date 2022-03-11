#include <stdio.h>
#include <omp.h> //OpenMP
/*
Core : a hardware unit that fetches instructions and executes them (ALU, FPU, L1,L2 caches)
CPU/Processor : combination of one or more cores with shared resources
OpenMP - An API for shared-memory parallelism in C, C++, and Fortran
-> gcc -fopenmp (Compiling)
-> Thread : an execution entity with a stack (thread-private memory)
-> Thread-safe routine
->Fork-join parallelism (fork: mater thread with ID 0, join : team of threads join in the parallel section and terminate synchronously)
	-->
	-->
	-->
------>------->

barrier, master, etc...
*/

int main(void)
{
	#pragma omp parallel  // Orders the compiler, a team of threads is created (Single Program Multi Data)
	{
		printf("thread number %d\n",omp_get_thread_num());
	}
	//quadcore CPU -> appears 4 times
	
	omp_set_num_threads(8);
	#pragma omp parallel  
	{
		printf("thread number %d\n", omp_get_thread_num());
	}
	//Thread 0~7 with no order


	/*
	private : make the variable as thread-local
	shared : Accessible by all threads in the team
	firstprivate : private and copy initial value from global variable
	lastprivate : copy back final value to gloval variable
	*/
	/*int idx = 10;
	int tid;
	omp_set_num_threads(4);
	printf("start tid = X idx=%d\n", idx);
	#pragma omp parallel private(tid) firstprivate(idx)
	{
		tid = omp_get_thread_num();
		printf("tid= %d, idx=%d\n", tid, idx);
		idx = 20;
	}
	tid = omp_get_thread_num();
	printf("end tid=%d, idx=%d\n", tid, idx); //tid=0, idx=10 since the idx=20 is thread-local
	*/
	int aa[10], tid, idx;
	omp_set_num_threads(4);
	#pragma omp parallel shared(aa) private(tid)
	{
		tid = omp_get_thread_num();
		aa[tid] = tid + 1;
	}
	for (idx = 0; idx < 4; idx++)
	{
		printf("aa[%d] = %d \n", idx, aa[idx]);
	}


	/*
	Loop Construct
	#pragma omp for num_threads(number)
	#pragma omp for schedule(something [, chunk size])
	1) static - chunks are assigned to the threads 0~9 10~19 ....
	2) dynamic - each thread request another chunk right after execution of each chunk ends
	3) guided - reduce overhead of dynamicby making chunk size change (not less than assigned size)
	4) auto - delegated to the compiler
	*/
	int a[1000];
	int b[1000];
	int c[1000];
	int i;
	#pragma omp parallel for schedule(dynamic)
	for (i = 0; i < 1000; i++)
	{
		c[i] = a[i] + b[i]; // 4 cores -> O(n)/4 (Slower in reality)		}
	}

	/*
	Section Construct --> Task parallelism -each thread taking different section
	*/
	int j;
	int n = 100;
	int q[100], r[100], s[100],t[100];
	#pragma omp parallel shared(n,q,r,s,t) private(j)
	{
		#pragma omp sections nowait
		{
			#pragma omp section
			for (j = 0; j < n; j++)
			{
				t[j] = 2 * q[j];
			}
			#pragma omp section
			{
				for (j = 0; j < n; j++)
				{
					s[j] = q[j] + r[j];
				}
			}
		} //omp end sections
	} //omp end parallel

	//pragma omp barrier : makes explicit barrier to wait
	//pragma omp master : master thread

	/*
	Memory consistency
	thread can have temporary view of memory and the value written is forced to go to memory
	so reading value can result in different values
	OpenMP flush makes a thread's temporary view of memory consistent with memory
	Implicit flush in start and end of omp parallel
	*/

	int flag1 = 0;
	int flag2 = 0;
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			flag2 = 1;
			#pragma omp flush(flag1,flag2)
			if (flag1 == 0)
			{
				printf("Thread 0 flag 1:%d flag 2:%d\n", flag1, flag2);
			}
		}

		#pragma omp section
		{
			flag1 = 1;
			#pragma omp flush(flag1,flag2)
			if (flag2 == 0)
			{
				printf("Thread 1 flag 1:%d flag 2:%d\n", flag1, flag2);
			}//Not printed because of the flag2=1 in the first thread
		}
	}
	return 0;
}