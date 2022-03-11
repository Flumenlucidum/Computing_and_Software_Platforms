#include <stdio.h>
#include <string.h>
int startback(int n)
{if (n>=26)
	
	{n=n%26;
	return n;
	}
else 
{ return n;}
}
void substring(char s[], char sub[], int p, int l) {
	   int c = 0;
	      
	      while (c < l) {
		            sub[c] = s[p+c-1];
			          c++;
				     }
	         sub[c] = '\0';
}


int main(void){

char alpha[] ="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
int number;
scanf("%d", &number);

if(number % 2==0)
{
	int k=number/2;
	int realk=k;
	
	char result[100][100];
	int g;
	int h;
	char A[]="A";
	for (g=0;g<=number-1;g++)
	{
	for (h=0;h<=number-1;h++)
	{
		result[g][h]=A[0];
	}}
	int startpoint;
/*startpoint are in matrix (1,1), (2,2),....(n,n) points*/
	for (startpoint=0;startpoint<realk;startpoint++)
	{
		int i;
		
	
	/*saving the element in the first line*/
	for (i=0;i<=number-2*startpoint-1;i++)
	{
	result[startpoint][startpoint+i]=alpha[startback(i)];

	}

	
	/*right part of the vortex*/
	for (i=0;i<=number-2*startpoint-2;i++){
	result[number-1-startpoint-i][number-startpoint-1]=alpha[startback(4*k-2-i)];
	
	}

	/*bottom line */
	for(i=0;i<=number-2-2*startpoint;i++)
	{

		result[number-1-startpoint][i+startpoint]=alpha[startback(6*k-3-i)];	
	}
	
	
	/*left part*/
	for (i=1;i<=number-2-2*startpoint;i++)
	{
		result[startpoint+i][startpoint]=alpha[startback(8*k-4-i)];
		
		
	}

	
	int new=8*k-4;
	
	
	
	char temp[40];
	char beta[40];
	char B;
	B='A';
	for (i=0;i<=39;i++)
	{
	temp[i]=B;
	beta[i]=B;
	}	
	
	for (i=0;i<=25;i++)
	{temp[i]=alpha[i];
		beta[i]=temp[i];
	}

	for (i=0;i<=25-startback(new);i++)
	{alpha[i]=alpha[startback(new)+i];
	}
	for (i=0;i<=startback(new)-1;i++)
	{
		alpha[startback(26-startback(new)+i)]=beta[i];
	}

		
	k=k-1;
	 

	
	
}
int i;
int j;
        for (i=0;i<=number-1;i++)
	        {
	 for (j=0;j<=number-1;j++)					               
	 {    if (j<number-1)
		{ printf("%c",result[i][j]);}
		 else
		     {printf("%c\n",result[i][j]);}
		}

	}

}

if (number%2!=0)
{    int k=number/2;
       	int realk=k;

	 char result[100][100];
	 int g;
	 int h;
	 char A[]="A";
	 for (g=0;g<=number-1;g++)								        {
										        for (h=0;h<=number-1;h++)												     {
										 		result[g][h]=A[0];
																        }}
	 int startpoint;

	 /*startpoint are in matrix (1,1), (2,2),....(n,n) points*/

	result[k][k]=alpha[startback(4*k*k+4*k)];
	 for (startpoint=0;startpoint<realk;startpoint++)
	 {
		 int i;
		 
 /*saving the element in the first line */ 
	 for (i=0;i<=number-2*startpoint-1;i++)
	 {
																
	       	 result[startpoint][startpoint+i]=alpha[startback(i)];										 																				        }
                                                                                                                                                                                            
																
	 /*right part of the vortex*/
	 for (i=0;i<=number-2*startpoint-2;i++){
		 result[number-1-startpoint-i][number-startpoint-1]=alpha[startback(4*k-i)];
		 														        }
	 
																	        /*bottom line */
	 for(i=0;i<=number-2-2*startpoint;i++)													      { result[number-1-startpoint][i+startpoint]=alpha[startback(6*k-i)];
																		       																							        }


																	        /*left part*/
																	        for (i=1;i<=number-2-2*startpoint;i++)		     {
																                result[startpoint+i][startpoint]=alpha[startback(8*k-i)];
																               
																							        }
																	     int new=8*k;
																	     


																	        char temp[40];
																	        char beta[40];
																	        char B;																       B='A';
																        for (i=0;i<=39;i++)  {
																												       
																		        temp[i]=B;
																		        beta[i]=B;						 										        }

																			 for (i=0;i<=25;i++)														        {temp[i]=alpha[i];
																	                beta[i]=temp[i];																        }

																	 for (i=0;i<=25-startback(new);i++)
																        {alpha[i]=alpha[startback(new)+i];
																				  }																										


 for (i=0;i<=startback(new)-1;i++)
	         {
			                 alpha[startback(26-startback(new)+i)]=beta[i];
					         }


         k=k-1;




	 }
	 int i;
	 int j;
	         for (i=0;i<=number-1;i++)
			                 {
						          for (j=0;j<=number-1;j++)
								           {    if (j<number-1)
										                   { printf("%c",result[i][j]);}
												                    else
															                         {printf("%c\n",result[i][j]);}
														                    }

							          }

}



return 0;
}

