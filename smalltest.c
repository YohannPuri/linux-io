#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


int main()
{
  FILE *in = fopen("test.txt","r");
  char ch;

	fpos_t pos;

  int fd = open("test2.txt",O_WRONLY);
	//int fd2 = open64("test3.txt",O_WRONLY);

	if(in!=NULL)
	{
	  ch = fgetc(in);
	}

	char starr[5];

	fread((void*)starr,1,5,in);
	
	fgets(starr,2,in);

	fgetpos(in,&pos);

	fsetpos(in,&pos);

	fclose(in);

	FILE *out = fopen("test3.txt","w");
	fputc(ch,out);
	int arr[5] = {1,2,3,4,5};
	fwrite((void*)arr,1,5,out);

	long int curr = ftell(out);

	starr[5] = '\0';
	fseek(out,curr,SEEK_SET);
	fputs(starr,out);
	
	if(feof(out)==0)
		printf("Yes %d\n",fd);

	fclose(out);
	return 0;
}

