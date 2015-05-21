#include <stdio.h>
#include <stdlib.h>

int main()
{
  FILE *in = fopen("test.txt","r");
  char ch = 'a';
	if(in!=NULL)
	{
	  while(ch!=EOF)
		{
		  ch = fgetc(in);
		  printf("Current character is %c \n",ch);
		}	
	}
}

