#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>


int main()
{

  clock_t program_start = clock();           // Set global program clock

  printf("%lf \n",(double)program_start/CLOCKS_PER_SEC);
  
  FILE *in = fopen("folder1/test.txt","r");
  char ch;

	fpos_t pos;

  int fd = open("folder1/sub1/test2.txt",O_WRONLY);
	//int fd2 = open64("test3.txt",O_WRONLY);

	char buffer[20] = "THIS IS JUST TEXT";

	write(fd,(void*)buffer,10);

	pwrite(fd,(void*)buffer,10,1);

	close(fd);

	fd = open("folder2/test4.txt",O_RDONLY);

	read(fd,(void*)buffer,10);

	pread(fd,(void*)buffer,10,1);

	close(fd);
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

	fclose(out);
	return 0;
}

