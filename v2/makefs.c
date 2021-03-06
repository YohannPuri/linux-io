#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


typedef struct{

	char* filename;
	unsigned int bytes_read;
	unsigned int bytes_written;
	unsigned int fd;
	void * stream_pointer;

}fs_file;
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


	fs_file *fs = malloc(100*sizeof(*fs));
	int file_count = 0;
	//int size = 1;

	// Variables for palceholding
	char *newPath;
	char command[20];
	char str[100];
	char filemode;
	
	void* file_ptr;
	int ret;
	int mode;
	int filedes;
	int bts;
	int sz;
	int offset;
	long num;
	char receivedChar;



	if(argc != 1)
	{
		newPath = argv[1];
	}

	FILE* log = fopen("log.txt","r");
	FILE* program = fopen("program.c","w");
	FILE* fileinfo = fopen("fileinfo.txt","w");

	double prog_time = 0.0, exec_time = 0.0;
	int pid = 0, tid = 0;
	useconds_t usec = 0;

	fprintf(program,"#include <stdio.h>\n");
	fprintf(program,"#include <stdlib.h>\n");
	fprintf(program,"#include <unistd.h>\n");
	fprintf(program,"#include <sys/types.h>\n");
	fprintf(program,"#include <fcntl.h>\n");
	fprintf(program,"#include <string.h>\n");
	fprintf(program,"#include <stdarg.h>\n\n\n");

	fprintf(program,"int main(){\n\n\n");

	fprintf(program,"\tint ret_int = 0; \n");
	fprintf(program,"\tchar ret_char = ' '; \n");
	fprintf(program,"\tlong ret_long = 0; \n");
	fprintf(program,"\tchar* ret_char_ptr = NULL; \n");
	fprintf(program,"\tFILE* ret_file_ptr = 0; \n\n\n");


	while(fscanf(log,"%lf %lf %d %d %s",&prog_time,&exec_time,&pid, &tid,command)!=EOF)
	{
		//printf("\n \n %lf %lf %d %d %s\n",prog_time,exec_time,pid,tid,command);
		usec = (unsigned int)(prog_time*1000000);
		fprintf(program, "\tusleep(%u);\n\n", (unsigned int)usec);

		if(command[0]=='f')
		{
			// Can be fopen, fclose, feof, ftell, fseek, fputs, fgetpos, fsetpos, fread, fwrite, fputc, fgetc

			if(command[1] == 'c')
			{
				// Has to be fclose

				fscanf(log,"%p %d", &file_ptr, &ret);
				fgetc(log);
				int k = 0;
					while(fs[k].stream_pointer!=file_ptr && k < file_count)
					{
						k++;
					}


				fprintf(program,"\tret_int = fclose(%s);\n",fs[k].filename);
			}
			else if(command[1] == 'o')
			{
				// Has to be fopen

				fscanf(log," %s %c %p\n",str,&filemode,&file_ptr);
				//printf("%s %c %p \n",str,filemode,file_ptr);
				fgetc(log);

				fprintf(program,"\tret_file_ptr = fopen(\"%s/%s\",\"%c\");\n",newPath,str,filemode);
				/*
				if(file_count<size)
				{
					// No need to reallocate
				}
				else
				{
					size = size*2;
					fs_file *temp_fs;
					temp_fs = realloc(fs,size * sizeof(fs*));
					fs = temp_fs;
				}
				*/

				fs[file_count].filename = (char*) malloc(sizeof(strlen(str)));
					strcpy(fs[file_count].filename,str);
					fs[file_count].bytes_written = 0;
					fs[file_count].bytes_read = 0;
					fs[file_count].stream_pointer = file_ptr;
					//printf("%s %p \n",fs[file_count].filename,fs[file_count].stream_pointer);
					file_count++;



			}
			else if(command[1] == 'g')
			{
				// Can be fgetc, fgets, fgetpos

				if(command[4]=='c')
				{
					fscanf(log,"%p %c",&file_ptr,&receivedChar);
					fgetc(log);
					int k = 0;
					while(fs[k].stream_pointer!=file_ptr && k < file_count)
					{
						k++;
					}
					fprintf(program,"\tret_char = fgetc(%s);\n",fs[k].filename);

					fs[k].bytes_read++;
				}
				else if(command[4]=='s')
				{
					fscanf(log,"%d %p",&bts,&file_ptr);
					fgetc(log);
					fprintf(program,"\tchar read_str[%d];\n",bts);
					int k = 0;
					while(fs[k].stream_pointer!=file_ptr && k < file_count)
					{
						k++;
					}

					fprintf(program,"\tread_str = fgets(read_str,%d,%s);\n",bts,fs[k].filename);

					fs[k].bytes_read+=bts;

				}
				else
				{
					// Has to be fgetpos

					fscanf(log,"%p %d",&file_ptr,&ret);
					fgetc(log);
					fprintf(program,"\tfpos_t pos;\n");
					int k = 0;

					while(fs[k].stream_pointer!=file_ptr && k < file_count)
					{
						k++;
					}

					fprintf(program,"\tret_int = fgetpos(%s,pos);\n",fs[k].filename);



				}

			}
			else if(command[1]=='r')
			{
				// Has to be fread

				fscanf(log,"%d %d %p %d",&sz,&bts,&file_ptr,&ret);
				fgetc(log);

				int k = 0;
				while(fs[k].stream_pointer!=file_ptr && k < file_count)
					{
						k++;
					}
					fprintf(program,"\tret_int = fread(buffer,%d,%d,%s);\n",sz,bts,fs[k].filename);

					fs[k].bytes_read+=(sz*bts);
			}
			else if(command[1] == 's')
			{
				// fseek or fsetpos

				if(command[3]=='t')
				{
					fscanf(log,"%p %d",&file_ptr,&ret);
					fgetc(log);
					fprintf(program,"\tfpos_t pos2;\n");
					
					int k = 0;

					while(fs[k].stream_pointer!=file_ptr && k < file_count)
					{
						k++;
					}

					fprintf(program,"\tret_int = fsetpos(%s,pos2);\n",fs[k].filename);
				}
				else
				{
					// Seek

					fscanf(log,"%p %ld %d %d",&file_ptr,&num,&offset,&ret);
					fgetc(log);
					
					int k = 0;

					while(fs[k].stream_pointer!=file_ptr && k < file_count)
					{
						k++;
					}

					fprintf(program,"\tret_int = fseek(%s,%ld,%d);\n",fs[k].filename,num,offset);
				}
			}

			else if(command[1]=='p')
			{
				// Putc or puts
				if(command[4] == 'c')
				{
					// Has to be putc

					fscanf(log,"%c %p %d",&receivedChar,&file_ptr,&ret);

					fgetc(log);

					fprintf(program, "\tret_char = %c;\n",receivedChar);

					int k = 0;

					while(fs[k].stream_pointer!=file_ptr && k<file_count)
					{
						k++;
					}

					fprintf(program,"\tret_int = fputc(ret_char,%s);\n",fs[k].filename);


				}
				else
				{
					fscanf(log,"%p %d",&file_ptr,&ret);
					fgetc(log);

					int k = 0;

					while(fs[k].stream_pointer!=file_ptr && k<file_count)
					{
						k++;
					}

					fprintf(program, "\tchar putstr[%d] = \"",ret);
					int l =0;
					for(l=0;l<ret;l++)
					{
						fprintf(program,"a");
					}
					fprintf(program, "\";\n");

					fprintf(program,"\tret_int = fputs(putstr,%s);\n",fs[k].filename);

				}
			}
			else if(command[1] == 'w')
			{
				//fwrite
				fscanf(log,"%d %d %p %d",&sz,&bts,&file_ptr,&ret);
				fgetc(log);

				int k = 0;
				while(fs[k].stream_pointer!=file_ptr && k<file_count)
					{
						k++;
					}
					fprintf(program,"\tret_int = fwrite(buffer,%d,%d,%s);\n",sz,bts,fs[k].filename);

					fs[k].bytes_written+=(sz*bts);

			}
			else
			{
				fscanf(log,"%p %d",&file_ptr,&ret);
				int k = 0;
				while(fs[k].stream_pointer!=file_ptr && k<file_count)
					{
						k++;
					}
				fprintf(program,"\tret_long = ftell(%s);\n",fs[k].filename);

			}

		}
		else if(command[0]=='o')
		{
			// Has to be open or open64
			if(command[4]!='6')
			{
				fscanf(log,"%s %d %d",str,&mode,&ret);
				fgetc(log);

				fprintf(program,"\tret_int = open(\"%s/%s\",%d);\n",newPath,str,mode);
			}
		}
		else if(command[0]=='w')
		{

			// Has to be write or write64
			if(command[5]!='6')
			{
				fscanf(log,"%d %p %d %d",&filedes,&file_ptr,&bts,&ret);
				int j = 0;
				for(j=0;j<file_count;j++)
				{
					if(fs[j].fd == filedes)
						break;
				}

				fs[j].bytes_written += ret;
				fprintf(program,"\tchar buffer[%d] = \"",ret);
				int k = 0;
				for(k = 0; k< ret; k++)
				{
					fprintf(program,"a");
				}

				fprintf(program,"\";\n");
				fprintf(program,"\tret_int = write(%d,buffer,%d);\n",filedes,ret);

				fgetc(log);
			}
		}

		else if(command[0] == 'p')
		{
			if(command[1] == 'w')
			{
				// pwrite or pwrite64
				if(command[6]!='6')
				{
					// Has to be pwrite
				fscanf(log,"%d %p %d %d %d",&filedes,&file_ptr,&bts,&offset,&ret);

				int j = 0;
				for(j=0;j<file_count;j++)
				{
					if(fs[j].fd == filedes)
						break;
				}

				fs[j].bytes_written += ret;
				fprintf(program,"\tchar buffer2[%d] = \"",ret);
				int k = 0;
				for(k = 0; k< ret; k++)
				{
					fprintf(program,"a");
				}

				fprintf(program,"\";\n");
				fprintf(program,"\tret_int = pwrite(%d,buffer2,%d,%d);\n",filedes,ret,offset);

				fgetc(log);
				}
			}
			else
			{
				//Has to be pread or pread64

				if(command[6]!='6')
				{
					// Has to be pwrite
				fscanf(log,"%d %p %d %d %d",&filedes,&file_ptr,&bts,&offset,&ret);

				int j = 0;
				for(j=0;j<file_count;j++)
				{
					if(fs[j].fd == filedes)
						break;
				}

				fs[j].bytes_written += ret;
				fprintf(program,"\tchar buffer4[%d] = \"",ret);
				int k = 0;
				for(k = 0; k< ret; k++)
				{
					fprintf(program,"a");
				}

				fprintf(program,"\";\n");
				fprintf(program,"\tret_int = pread(%d,buffer4,%d,%d);\n",filedes,ret,offset);

				fgetc(log);
				}

				
			}
		}
		else if(command[0] == 'c')
		{
			// Has to be close...

			fscanf(log,"%d %d",&filedes,&ret);
			fprintf(program,"\tret_int = close(%d);\n",filedes);
			fgetc(log);
		}
		else if(command[0] == 'r')
		{
			fscanf(log,"%d %p %d %d",&filedes,&file_ptr,&bts,&ret);

			int j = 0;
				for(j=0;j<file_count;j++)
				{
					if(fs[j].fd == filedes)
						break;
				}

				fs[j].bytes_read += ret;
				fprintf(program,"\tchar buffer3[%d] = \"",ret);

				fprintf(program,"\";\n");
				fprintf(program,"\tret_int = read(%d,buffer3,%d,%d);\n",filedes,ret,offset);

				fgetc(log);

		}

	}

	fprintf(program,"\n\nreturn 0;\n\n}");
	fclose(log);
	fclose(program);

	int k = 0;
	while(k<file_count)
	{
		fprintf(fileinfo,"%s %d %d %p \n",fs[k].filename,fs[k].bytes_read,fs[k].bytes_written,fs[k].stream_pointer);
		k++;
	}

	fclose(fileinfo);
	return 0;

}