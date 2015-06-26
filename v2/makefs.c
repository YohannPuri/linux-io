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

	while(fscanf(log,"%lf %lf %s",&prog_time,&exec_time,newPath) != EOF)
	{
		printf("%lf %lf %s\n",&prog_time,&exec_time,newPath);

	}
	
	

	return 0;

}