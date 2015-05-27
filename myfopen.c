#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

//static int tbytes = 0;
//typedef FILE* (*orig_fopen_type)(const char* path, const char* mode);
//typedef int (*orig_fgetc_type)(FILE* file);

typedef int (*orig_open_type)(const char* __file, int __oflags, ...);

int creat(__const char* __file,__mode_t __mode)
{
	printf("Bro");
	return 4;
}

int open(const char *__file, int __oflag, ...)
{
	clock_t start = clock();
	orig_open_type orig_open;
	orig_open = (orig_open_type)dlsym(RTLD_NEXT,"open");

	int temp = orig_open(__file,__oflag,0);

	clock_t end = clock();

	double diff = ((double)(end-start))/(double)CLOCKS_PER_SEC;

	printf("%lf \n",diff);
	
	return temp;
}

/*
FILE* fopen(const char *path, const char * mode)
{

	struct timeval tv1,tv2;

	orig_fopen_type orig_fopen;
	orig_fopen = (orig_fopen_type)dlsym(RTLD_NEXT,"fopen");

	//time_t t;
	//time(&t);

	//printf("%s",ctime(&t));
	clock_t start = clock();
	
	gettimeofday(&tv1,NULL);
	FILE *temp = orig_fopen(path,mode);
	gettimeofday(&tv2,NULL);	

	clock_t end = clock();
	//time(&t);
	//printf("%s",ctime(&t));

	double diff = ((double)(end-start))/(double)CLOCKS_PER_SEC;

	printf("%lf \n",diff);

	printf("Total time for fopen = %lf seconds \n", (double)(tv2.tv_usec - tv1.tv_usec)/1000000 + (double)(tv2.tv_sec - tv1.tv_sec));
	return temp;
}
*/

/*
int fgetc(FILE* __stream)
{
	tbytes++;
	struct timeval tv1,tv2;
	orig_fgetc_type orig_fgetc;
	orig_fgetc = (orig_fgetc_type)dlsym(RTLD_NEXT,"fgetc");

	int temp;

	gettimeofday(&tv1,NULL);
	temp = orig_fgetc(__stream);
	gettimeofday(&tv2,NULL);

	//printf("Total time for fgetc = %lf seconds \n", (double)(tv2.tv_usec - tv1.tv_usec)/1000000 + (double)(tv2.tv_sec - tv1.tv_sec));
	//printf("Total bytes read until now is %d \n",tbytes);
	return temp;
}

*/


