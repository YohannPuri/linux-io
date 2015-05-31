#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

// Aliases for our original functions
//typedef int (*orig_open_type)(const char* pathname, int flags);
//typedef int (*orig_open2_type)(const char *pathname, int flags, mode_t mode);
typedef int (*orig_close_type)(int fd);
//typedef int (*orig_creat_type)(const char *pathname, mode_t mode);
typedef ssize_t (*orig_read_type)(int fd, void *buf, size_t count);
static ssize_t (*orig_write)(int fd, const void *buf, size_t count) = NULL;
typedef ssize_t (*orig_pread_type)(int fd, void *buf, size_t count, off_t offset);
typedef ssize_t (*orig_pwrite_type)(int fd, const void *buf, size_t count, off_t offset);

static int (*real_puts)(const char* str) = NULL;

// Holds value of number of ticks at start
clock_t program_start;

/*
// Init function. Always called before Preload
void __attribute__((constructor)) my_init(void)
{
	
	program_start = clock();
}
*/
// Our new functions which will call the original functions as well

int puts(const char* str)
{
	printf("Arerere");

	real_puts =dlsym(RTLD_NEXT, "puts");
	return real_puts(str);

}

/*
int creat(const char *pathname, mode_t mode)
{
	orig_creat_type orig_creat;
	orig_creat = (orig_creat_type)dlsym(RTLD_NEXT,"creat");

	clock_t start = clock();

		int fd = orig_creat(pathname,mode);

	clock_t end = clock();

		double start_diff = ((double)(start-program_start))/(double)CLOCKS_PER_SEC;
		double end_diff = ((double)(end-program_start))/(double)CLOCKS_PER_SEC;

		return fd;
}

int open(const char *pathname, int flags)
{

	orig_open_type orig_open;
	orig_open = (orig_open_type)dlsym(RTLD_NEXT,"open");

	clock_t start = clock();

		int fd = orig_open(pathname,flags);

	clock_t end = clock();

		double start_diff = ((double)(start-program_start))/(double)CLOCKS_PER_SEC;
		double end_diff = ((double)(end-program_start))/(double)CLOCKS_PER_SEC;

		return fd;

}


int open(const char *pathname, int flags, mode_t mode)
{

		orig_open2_type orig_open2;
	orig_open2 = (orig_open2_type)dlsym(RTLD_NEXT,"open");

		clock_t start = clock();

		int fd = orig_open2(pathname,flags,mode);

		clock_t end = clock();

		double start_diff = ((double)(start-program_start))/(double)CLOCKS_PER_SEC;
		double end_diff = ((double)(end-program_start))/(double)CLOCKS_PER_SEC;

		return fd;

}
*/
int close(int fd)
{
	orig_close_type orig_close;
	orig_close = (orig_close_type)dlsym(RTLD_NEXT,"close");

	clock_t start = clock();

		int temp = orig_close(fd);

	clock_t end = clock();

		double start_diff = ((double)(start-program_start))/(double)CLOCKS_PER_SEC;
		double end_diff = ((double)(end-program_start))/(double)CLOCKS_PER_SEC;
		printf("%lf %lf Close Called \n", start_diff, end_diff);
		return temp;
}

ssize_t read(int fd, void *buf, size_t count)
{
	orig_read_type orig_read;
	orig_read = (orig_read_type)dlsym(RTLD_NEXT,"read");

		clock_t start = clock();

		ssize_t temp = orig_read(fd,buf,count);

		clock_t end = clock();

		double start_diff = ((double)(start-program_start))/(double)CLOCKS_PER_SEC;
		double end_diff = ((double)(end-program_start))/(double)CLOCKS_PER_SEC;
		printf("%lf %lf Read Called \n", start_diff, end_diff);
		return temp;
}

ssize_t write(int fd, const void *buf, size_t count)
{
	orig_write = dlsym(RTLD_NEXT,"write");

		clock_t start = clock();

		ssize_t temp = orig_write(fd,buf,count);

		clock_t end = clock();

		double start_diff = ((double)(start-program_start))/(double)CLOCKS_PER_SEC;
		double end_diff = ((double)(end-program_start))/(double)CLOCKS_PER_SEC;
		printf("%lf %lf Write Called \n", start_diff, end_diff);
		return temp;
}

ssize_t pread(int fd, void *buf, size_t count, off_t offset)
{
	orig_pread_type orig_pread;
	orig_pread = (orig_pread_type)dlsym(RTLD_NEXT,"pread");

	
		clock_t start = clock();

		ssize_t temp = orig_pread(fd,buf,count,offset);

		clock_t end = clock();

		double start_diff = ((double)(start-program_start))/(double)CLOCKS_PER_SEC;
		double end_diff = ((double)(end-program_start))/(double)CLOCKS_PER_SEC;
		printf("%lf %lf PRead Called \n", start_diff, end_diff);
		return temp;
}

ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset)
{
	orig_pwrite_type orig_pwrite;
	orig_pwrite = (orig_pwrite_type)dlsym(RTLD_NEXT,"pwrite"); 


		clock_t start = clock();

		ssize_t temp = orig_pwrite(fd,buf,count,offset);

		clock_t end = clock();

		double start_diff = ((double)(start-program_start))/(double)CLOCKS_PER_SEC;
		double end_diff = ((double)(end-program_start))/(double)CLOCKS_PER_SEC;
		printf("%lf %lf PWrite Called \n", start_diff, end_diff);
		return temp;
}

