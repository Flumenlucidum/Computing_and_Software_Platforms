#include <stdio.h>
#include <math.h>

int main(){
int input;
int multitime=0;
int dividend=2;
static int board[1][2000];
int number=-1;
scanf("%d",&input);
if (input==1){
	printf("\n");
}

else
{
	while(input!=1){
		
	while (input % dividend != 0 && input!=1){
		                                dividend++;
						        }

	while (input % dividend ==0){
		multitime++;
		input=input/dividend;
		
		}
	
	number++;
	board[0][number]=dividend;
	board[1][number]=multitime;
	dividend++;
	multitime=0;
	
	if (input==1) break;
		
	}

int i;
int h;
for (i=0;i<=number;i++){
	for(h=0;h<=board[1][i]-1;h++){
		if(i==number && h==board[1][i]-1){
		printf("%d\n",board[0][i]);
		}	
		else{
		printf("%d ",board[0][i]);
		}
}

}

}
return 0;

}

