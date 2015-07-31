#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


// These structures store our file info. 100 of these are currently being allocated at the beginning of the program
// They are initialized as files are opened.


typedef struct{

	char* filename;
	int pid;
	int tid;
	unsigned int bytes_read;
	unsigned int bytes_written;
	int fd;
	int fpi;
	int fdi;
	void * stream_pointer;

}fs_file;


typedef struct node {

	int num_of_children;
	char name[30];
	struct node *children;
	struct node *next;

}node;


void print(node *root);

	// Data we are trying to maintain
	/*
		We need filenames that we are trying to create...
		How many bytes are written to the file..
		How many bytes are read from the file..
		Make sure that many trash bytes exist
		Keep track of file descriptors for each opened file
		Keep track of file stream pointers for each opened file
	
	*/


int main (int argc, char *argv[])
{

	fs_file fs[100];			// Allocating 100 of our structures


	int file_count = 0;			// Gross count on the number of open files
	int file_ptr_index = 0;		// Number of files opened using stdio functions
	int file_desc_index = 0;	// Number of files opened using fcntl functions



	//int size = 1;

	// Variables for placeholding
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

	//int buffer_index = 0;

	if(argc != 1)
	{
		newPath = argv[1];
	}

	FILE* log = fopen("log.txt","r");
	FILE* program = fopen("program.c","w");
	FILE* fileinfo = fopen("fileinfo.txt","w");
	FILE* graph = fopen("graph.txt","w+");

	int l = 0,w=0;

	node *root = malloc(sizeof(node));

	node *temp = NULL;

	temp = root;

	temp->num_of_children = 0;

	char path[50];

	for(l=0;newPath[l]!='\0';l++)
	{
		if(newPath[l]!='/')
		{
			temp->name[w] = newPath[l];
		}
		else
		{
			temp->name[w] = '\0';
			temp->next = NULL;
			temp->children = malloc(sizeof(node));
			temp->num_of_children++;
			w=0;
			temp = temp->children;
			continue;
		}
		w++;
	}

	temp->name[w] = '\0';
	temp->children = NULL;
	temp->next = NULL;
	temp->num_of_children = 0;

	node *base = temp;

	//fputc('\n',graph);

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
	fprintf(program,"\tint i = 0; \n");
	fprintf(program,"\tchar ret_char = ' '; \n");
	fprintf(program,"\tlong ret_long = 0; \n");
	fprintf(program,"\tchar* ret_char_ptr = NULL; \n");
	fprintf(program,"\tFILE* ret_file_ptr = 0; \n");
	fprintf(program,"\tchar* buffer = NULL; \n");
	fprintf(program,"\tchar* buffer2 = NULL; \n");
	fprintf(program,"\tchar* buffer3 = NULL; \n");
	fprintf(program,"\tchar* buffer4 = NULL; \n\n\n");



	// Our detection loop begins here

	//---------------------------------------------------------------------------------


	while(fscanf(log,"%lf %lf %d %d %s",&prog_time,&exec_time,&pid, &tid,command)!=EOF)
	{
		
		//printf("\n \n %lf %lf %d %d %s\n",prog_time,exec_time,pid,tid,command);
		
		usec = (unsigned int)(prog_time*1000000);
		if(usec == 0)
			break;
		fprintf(program, "\tusleep(%u);\n\n", (unsigned int)usec);			// Delay is printed into program



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


				fprintf(program,"\tret_int = fclose(file_ptr%d);\n",fs[k].fpi);
			}
			else if(command[1] == 'o')
			{
				// Has to be fopen

				fscanf(log," %s %c %p\n",str,&filemode,&file_ptr);
				//printf("%s %c %p \n",str,filemode,file_ptr);
				fgetc(log);

				fprintf(program,"\tfile_ptr%d = fopen(\"%s/%s\",\"%c\");\n",file_ptr_index,newPath,str,filemode);

				int f = 0;
				w = 0;
				temp = base;

				while(str[f]!='\0')
				{

					if(str[f]!='/')
					{
						// FORWARD SLASH HASNT BEEN HIT. COPYING NAME
						path[w] = str[f];
						f++;
						continue;
					}
					else
					{
						path[w] = '\0';
						w = 0;
					}

					// YOU HAVE THE PATH - >>> NOW CHECK CHILDREN

					if(temp->num_of_children > 0)
						{

								// BASE HAS CHILDREN. THEY MUST BE CHECKED FOR PRE EXISTENCE

								node *traverse = temp->children;
								node *prev;
								int exists = 0;

								while(traverse!=NULL && exists == 0)
								{
									if(strcmp(traverse->name,path)==0)
									{
										
										temp = traverse;
										exists = 1;
									}
									else
									{
									prev = traverse;
									traverse = traverse->next;
									}
								}

								if(exists==0)
								{
									node *new_node = malloc(sizeof(node));
									new_node->num_of_children = 0;
									new_node->children = NULL;
									new_node->next = NULL;
									strcpy(new_node->name,path);

									traverse = prev;
									traverse ->next = new_node;
									temp->num_of_children++;
									temp = new_node;
								}

						}
					else
					{
						node *new_node = malloc(sizeof(node));
						new_node->num_of_children = 0;
						new_node->children = NULL;
						new_node->next = NULL;
						strcpy(new_node->name,path);

						temp->children = new_node;
						temp->num_of_children++;
						temp = temp->children;
					}
					w++;
					f++;
				}




					fs[file_count].filename = (char*) malloc(sizeof(strlen(str)));
					strcpy(fs[file_count].filename,str);

					// Initialize byes written and read
					fs[file_count].bytes_written = 0;
					fs[file_count].bytes_read = 0;


					fs[file_count].stream_pointer = file_ptr;

					fs[file_count].fpi = file_ptr_index;

					fs[file_count].pid = pid;
					fs[file_count].tid = tid;

					fs[file_count].fd = -1;
					fs[file_count].fdi = -1;

					file_ptr_index++;
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

				fprintf(program,"\tfd%d = open(\"%s/%s\",%d);\n",file_desc_index,newPath,str,mode);


				fs[file_count].filename = (char*) malloc(sizeof(strlen(str)));
				strcpy(fs[file_count].filename,str);

				// Initialize byes written and read
				fs[file_count].bytes_written = 0;
				fs[file_count].bytes_read = 0;


					fs[file_count].fd = ret;

					fs[file_count].fdi = file_desc_index;

					fs[file_count].pid = pid;
					fs[file_count].tid = tid;

					file_desc_index++;
					file_count++;

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
				fprintf(program,"\tbuffer = malloc(sizeof(char)*%d);\n",ret);
				int k = 0;

				fprintf(program,"\tfor(i=0;i<%d;i++)\n\t{\n\tchar[i] = 'a';\n\t}\n",ret);

				fprintf(program,"\tret_int = write(%d,buffer,%d);\n",filedes,ret);
				fprintf(program,"\tfree(buffer);\n");


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

				fprintf(program,"\tbuffer2 = malloc(sizeof(char)*%d);\n",ret);
				int k = 0;

				fprintf(program,"\tfor(i=0;i<%d;i++)\n\t{\n\tchar[i] = 'a';\n\t}\n",ret);
				
				fprintf(program,"\tret_int = pwrite(%d,buffer2,%d,%d);\n",filedes,ret,offset);
				fprintf(program,"\tfree(buffer2);\n");

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
				fprintf(program,"\tbuffer4 = malloc(sizeof(char)*%d);\n",ret);
				int k = 0;

				fprintf(program,"\tfor(i=0;i<%d;i++)\n\t{\n\tchar[i] = 'a';\n\t}\n",ret);

				fprintf(program,"\tret_int = pread(%d,buffer4,%d,%d);\n",filedes,ret,offset);
				fprintf(program,"\tfree(buffer4);\n");
				fgetc(log);
				}

				
			}
		}
		else if(command[0] == 'c')
		{
			// Has to be close...

			fscanf(log,"%d %d",&filedes,&ret);

			int k = 0;
			while(fs[k].fd!=filedes && k < file_count)
			{
						k++;
			}

			fprintf(program,"\tret_int = close(fd%d);\n",fs[k].fdi);
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
				
				fprintf(program,"\tbuffer3 = malloc(sizeof(char)*%d);\n",ret);
				int k = 0;

				fprintf(program,"\tfor(i=0;i<%d;i++)\n\t{\n\tchar[i] = 'a';\n\t}\n",ret);


				fprintf(program,"\tret_int = read(%d,buffer3,%d,%d);\n",filedes,ret,offset);
				fprintf(program,"\tfree(buffer3);\n");


				fgetc(log);

		}

	}
	temp = root;

	print(temp);

	fprintf(program,"\n\nreturn 0;\n\n}");
	fclose(log);
	fclose(program);
	fclose(graph);

	fprintf(fileinfo,"Filename\t\tpid\ttid\tBytesRead\tBytesWritten\tfd\tStreamPointer\tfdi\tfpi\n");


	int k = 0;
	while(k<file_count)
	{
		fprintf(fileinfo,"%s\t\t%d\t%d\t\t%d\t\t%d\t%d\t%p\t%d\t%d\n",fs[k].filename,fs[k].pid,fs[k].tid,fs[k].bytes_read,fs[k].bytes_written,fs[k].fd,fs[k].stream_pointer,fs[k].fdi,fs[k].fpi);
		k++;
	}

	fclose(fileinfo);
	return 0;

}


void print(node *root)
{
	printf("%s \n",root->name);
	int i = 0;
	node *temp = root->children;

	while(i<root->num_of_children)
	{
		
		print(temp);
		temp = temp->next;
		i++;
	}

}