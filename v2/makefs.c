#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main (int argc, char *argv[])
{
	//char *newPath;
	char command[20] = "BLANK";
	char *str;
	char filemode;
	void* file_ptr;
	int ret;
	/*if(argc != 1)
	{
		newPath = argv[1];
	}
*/
	FILE* log = fopen("log.txt","r");
	FILE* program = fopen("program.c","w");

	double prog_time = 0.0, exec_time = 0.0;
	int pid = 0, tid = 0;
	useconds_t usec = 0;

	while(fscanf(log,"%lf %lf %d %d %s",&prog_time,&exec_time,&pid, &tid,command)!=EOF)
	{
		printf("%lf %lf %d %d %s\n",prog_time,exec_time,pid,tid,command);
		usec = (unsigned int)(prog_time*1000000);
		fprintf(program, "usleep(%u)\n;", (unsigned int)usec);

		if(command[0]=='f')
		{
			// Can be fopen, fclose, feof, ftell, fseek, fputs, fgetpos, fsetpos, fread, fwrite, fputc, fgetc

			if(command[1] == 'c')
			{
				// Has to be fclose

				fscanf(log,"%p %d", &file_ptr, &ret);
				printf("\n %p %d \n", file_ptr, ret);
			}
			if(command[1] == 'o')
			{
				// Has to be fopen

				fscanf(log,"%s %c %p",str,&filemode,&file_ptr);
				printf("%s %c %p",str,filemode,file_ptr);

			}
		}


	}
	fclose(log);
	fclose(program);

	return 0;

}