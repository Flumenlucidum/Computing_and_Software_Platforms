#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>





int main(int argc, char **argv) {
  	
	
	

	
	
	int cnt=0;
	const int length=strlen(argv[1]);
	printf("%d\n",length);
	FILE *fp1=fopen(argv[2],"r");
	fseek(fp1,0,SEEK_END);
	long filelength=ftell(fp1);
	printf("%ld\n",filelength);
	fseek(fp1,0,SEEK_SET);
	char * s1;
	char * temp;
	char  * s2;
	
	//printf("%ld\n",filelength);
	s1=(char *)malloc (filelength+1);
	temp=s1;
	s2=temp;

	
	s1[filelength]='\0';
	
	fread(s1,1,filelength,fp1);
	
	fclose(fp1);
			                			                

	while (*s1!='\0')
	{
		while (*s1!=argv[1][0])
		{s1++;}
		
		
		int correct=0;
		for (int j=0;j<=length-1;++j)
		{
			if (*s1+j=='\0')
			{	break;
				printf("%d",99999);}
			else
			{
			if (*(s1+j)==argv[1][j])
			{	correct++;
			}
			
			

			
			//printf("%d\n",*s1);
			//printf("%d\n",correct);
			}
		}
		if (correct==length )
		{
			cnt++; 
			//printf("%d\n",cnt);
		}
		s1++;
		
		
		
	}


		
	
	printf("%d\n",cnt);
	
	free(s2);
	//printf("0\n");
  return 0;
}
