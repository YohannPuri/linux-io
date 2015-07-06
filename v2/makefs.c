#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


typedef struct fs_file{

	char* filename;
	unsigned int bytes_read;
	unsigned int bytes_written;
	unsigned int fd;
	void * stream_pointer;

};
int main (int argc, char *argv[])
{

	// Data we are trying to maintain
	/*
		We need filenames that we are trying to create...
		How many bytes are written to the file..
		How many bytes are read from the file..
		Make sure that many trash bytes exist
		Keep track of file descriptors for each opened file
		Keep track of file stream pointers for each opened file
	
	*/


	fs_file *fs = malloc(sizeof(fs_file));
	int file_count = 0;
	int size = 1;

	// Variables for palceholding
	char *newPath;
	char command[20];
	char str[100];
	char filemode;
	char* file_mode[10];
	file_mode[1] = malloc(10*sizeof(char));
	file_mode[1] = "WRONLY";
	void* file_ptr;
	int ret;
	int mode;
	if(argc != 1)
	{
		newPath = argv[1];
	}

	FILE* log = fopen("log.txt","r");
	FILE* program = fopen("program.c","w");

	double prog_time = 0.0, exec_time = 0.0;
	int pid = 0, tid = 0;
	useconds_t usec = 0;

	while(fscanf(log,"%lf %lf %d %d %s",&prog_time,&exec_time,&pid, &tid,command)!=EOF)
	{
		//printf("\n \n %lf %lf %d %d %s\n",prog_time,exec_time,pid,tid,command);
		usec = (unsigned int)(prog_time*1000000);
		fprintf(program, "usleep(%u);\n", (unsigned int)usec);

		if(command[0]=='f')
		{
			// Can be fopen, fclose, feof, ftell, fseek, fputs, fgetpos, fsetpos, fread, fwrite, fputc, fgetc

			if(command[1] == 'c')
			{
				// Has to be fclose

				fscanf(log,"%p %d", &file_ptr, &ret);
				printf("\n %p %d \n", file_ptr, ret);
			}
			else if(command[1] == 'o')
			{
				// Has to be fopen

				fscanf(log," %s %c %p\n",str,&filemode,&file_ptr);
				//printf("%s %c %p \n",str,filemode,file_ptr);
				fgetc(log);

				fprintf(program,"fopen(\"%s/%s\",\"%c\");\n",newPath,str,filemode);


			}
		}
		else if(command[0]=='o')
		{
			// Has to be open or open64
			if(command[4]!='6')
			{
				fscanf(log,"%s %d %d",str,&mode,&ret);
				fgetc(log);

				fprintf(program,"open(\"%s/%s\",\"%s\");\n",newPath,str,file_mode[mode]);
			}
		}


	}
	fclose(log);
	fclose(program);

	return 0;

}