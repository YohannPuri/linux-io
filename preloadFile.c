#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

// Aliases for our original functions
typedef int (*orig_open_type)(const char* __file, int __oflags, ...);


// Holds value of number of ticks at start
clock_t program_start;


// Init function. Always called before Preload
void _init(void)
{
	orig_open_type orig_open;
	orig_open = (orig_open_type)dlsym(RTLD_NEXT,"open");

	orig_close_type orig_close;
	orig_close = (orig_close_type)dlsym(RTLD_NEXT,"close");

	orig_read_type orig_read;
	orig_read = (orig_read_type)dlsym(RTLD_NEXT,"read");

	orig_write_type orig_write;
	orig_write = (orig_write_type)dlsym(RTLD_NEXT,"write");

	orig_creat_type orig_creat;
	orig_creat = (orig_creat_type)dlsym(RTLD_NEXT,"creat");

	orig_pread_type orig_read;
	orig_pread = (orig_pread_type)dlsym(RTLD_NEXT,"pread");

	orig_pwrite_type orig_pwrite;
	orig_write = (orig_pwrite_type)dlsym(RTLD_NEXT,"pwrite"); 

	program_start = clock();
}

// Our new functions which will call the original functions as well

int creat(__const char* __file,__mode_t __mode)
{

}

int open(const char *__file, int __oflag, ...)
{
	if(strcmp("log.txt",__file)!=0)
	{
		clock_t start = clock();

		int temp = orig_open(__file,__oflag,0);

		clock_t end = clock();

		double start_diff = ((double)(start-program_start))/(double)CLOCKS_PER_SEC;
		double end_diff = ((double)(end-program_start))/(double)CLOCKS_PER_SEC;
	
		FILE *log = fopen("log.txt","a");
		fprintf(log,"%lf %lf open %s %d",start_diff,end_diff,__file,__oflag);
		fclose(log);
	
		fds[temp] = malloc(sizeof(char)*strlen(__file));
		strcpy("")
		return temp;
	}

	else
	{
		return orig_open(__file,__oflag,0);
	}
}
