#include "timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
struct timer {
	struct timespec start;
	struct timespec end;
	double length;
	int click;
};

struct timer *timerPtr;
//struct timer timerlist[];

void timer_init(int n) {
	//Makes N stop watch and initailize at 0  from 0 to N-1  use malloc
	
	timerPtr=(struct timer *)malloc (sizeof (struct timer) * n);
	
	
	for (int i=0;i<n;++i)
	{ 
	      (timerPtr+i)->length=0;
		(timerPtr +i) -> click=0;
	}
	/*for (int j=0; j<n;++j){
		timerPtr[j]=timerlist[j];
	}*/


	
}

void timer_finalize() {
	extern struct timer *timerPtr;
	//delete all stop watch made
	free(timerPtr);

}

void timer_start(int idx) {
	//start the idx th stopwatch if working let it go
	extern struct timer* timerPtr; 
	if ( (timerPtr+idx)-> click %2==0)
	{
	clock_gettime(CLOCK_MONOTONIC,&((timerPtr+idx)->start));
	(timerPtr+idx)->click+=1;
	}
}

void timer_stop(int idx) {
	//stop the stopwatch and let it just be as it were if already stopped
	extern struct timer* timerPtr;
	if ((timerPtr+idx)->click %2 != 0){
	clock_gettime(CLOCK_MONOTONIC,&((timerPtr+idx)->end));
	(timerPtr+idx)->click+=1;
	(timerPtr+idx)->length+=((double)(timerPtr+idx)->end.tv_nsec/1000000000-(double)(timerPtr+idx)->start.tv_nsec/1000000000+(timerPtr+idx)->end.tv_sec -(timerPtr+idx)->start.tv_sec );}
}


double timer_read(int idx) {
	//return the time as the double in seconds
	extern struct timer* timerPtr;
	if ((timerPtr+idx)->click %2 !=0){
	clock_gettime(CLOCK_MONOTONIC, &((timerPtr+idx)->end));
	return ( (timerPtr+idx)->length+(double)(timerPtr+idx)->end.tv_nsec/1000000000-(double)(timerPtr+idx)->start.tv_nsec/1000000000)+(double)((timerPtr+idx)->end.tv_sec - (timerPtr+idx)->start.tv_sec) ;
	}
	else
	{ return (timerPtr+idx)->length;}

}

void timer_reset(int idx) {
	//stop the idx th stop watch and make it zero
	extern struct timer* timerPtr; 
	(timerPtr+idx)->click=0;
	(timerPtr+idx)->length=0;
	

}
