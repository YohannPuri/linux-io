#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
int main()
{
  FILE *in = fopen("test.txt","r");
  char ch;

	int fd = open("bro.c",0);
	if(in!=NULL)
	{
	  ch = fgetc(in);
	  while(ch!=EOF)
		{
		  printf("Current character is %c \n",ch);
		  ch = fgetc(in);
		}	
	}
	printf("%d \n ",fd);
	return 0;
}

