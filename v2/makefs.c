#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char *argv[])
{
	char *newPath;
	if(argc != 1)
	{
		newPath = argv[1];
	}

	FILE* log = fopen("log.txt","r");

	double prog_time = 0.0, exec_time = 0.0;
	int pid = 0, tid = 0;

	fscanf(log,"%lf %lf %d %d %s",&prog_time,&exec_time,&pid, &tid,newPath);
	
		printf("%lf %lf %d %d %s\n",prog_time,exec_time,pid,tid,newPath);
	
	fclose(log);

	return 0;

}