#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main (int argc, char *argv[])
{
	char *newPath;
	if(argc != 1)
	{
		newPath = argv[1];
	}

	FILE* log = fopen("log.txt","r");
	FILE* program = fopen("program.c","w");

	double prog_time = 0.0, exec_time = 0.0;
	int pid = 0, tid = 0;
	useconds_t usec = 0;

	while(fscanf(log,"%lf %lf %d %d %s",&prog_time,&exec_time,&pid, &tid,newPath)!=EOF)
	{
		printf("%lf %lf %d %d %s\n",prog_time,exec_time,pid,tid,newPath);
		usec = (unsigned int)(prog_time*1000000);
		fprintf(program, "usleep(%u)", (unsigned int)usec);

	}
	fclose(log);
	fclose(program);

	return 0;

}