#define _GNU_SOURCE

#include <time.h>               // For clock ticks
#include <stdio.h>              // For file I/O functions
#include <stdlib.h>
#include <unistd.h>             // For file I/O functions
#include <sys/types.h>
#include <dlfcn.h>              // For dlsym()
#include <fcntl.h>
#include <stdarg.h>             // For variable argumenrs
#include <string.h>             
#include <limits.h>

// Global Variable to hold clock ticks at program start
static clock_t program_start;
static FILE *logFile;

// Defining a new constructor and destructor
static void wrap_init(void) __attribute__((constructor));
static void wrap_deinit(void) __attribute__((destructor));

// Pointers to original stdio.h functions
static FILE* (*_fopen) (const char *, const char *) = NULL;
static int   (*_fclose)(FILE *stream)               = NULL;
static int   (*_fgetc) (FILE *__stream)             = NULL;
static int   (*_fputc) (int character, FILE *stream)= NULL;
static size_t(*_fread) ( void * ptr, size_t size, size_t count, FILE * stream ) = NULL;
static size_t(*_fwrite)( const void * ptr, size_t size, size_t count, FILE * stream ) = NULL;
static int   (*_fputs) ( const char * str, FILE * stream ) = NULL;
static char* (*_fgets) ( char * str, int num, FILE * stream ) = NULL;
static int   (*_fgetpos) ( FILE * stream, fpos_t * pos ) = NULL;
static int   (*_fsetpos) ( FILE * stream, const fpos_t * pos ) = NULL;
static int   (*_fseek) ( FILE * stream, long int offset, int origin ) = NULL;
static long int (*_ftell) ( FILE * stream ) = NULL;
static int (*_feof) ( FILE * stream ) = NULL;
static int (*_close)(int fd) = NULL;
/*
int fprintf(FILE *stream, const char *format, ...);

int vfprintf(FILE *stream, const char *format, va_list ap);

*/

// Pointers to original fcntl.h and unistd.h functions
static int (*_open)(const char *, int, mode_t) = NULL;

static ssize_t (*_read)(int fd, void *buf, size_t count) = NULL;
static ssize_t (*_write)(int fd, const void *buf, size_t count) = NULL;
static ssize_t (*_pread)(int fd, void *buf, size_t count, off_t offset) = NULL;

static ssize_t (*_pwrite)(int fd, const void *buf, size_t count, off_t offset) = NULL;
//static int (*_open64)(const char *, int, mode_t) = NULL;


// Constructor

static void wrap_init(void)
{
    

    // Set original stdio functions

    _fopen = dlsym(RTLD_NEXT,"fopen");
    _fclose = dlsym(RTLD_NEXT,"fclose");
    //_fprintf = dlsym(RTLD_NEXT,"fprintf");
    //_vfprintf = dlsym(RTLD_NEXT,"vfprintf");
    //_fscanf = dlsym(RTLD_NEXT,"fscanf");
    //_sscanf = dlsym(RTstreamLD_NEXT,"sscanf");
    _fgetc = dlsym(RTLD_NEXT,"fgetc");
    _fputc = dlsym(RTLD_NEXT,"fputc");
    _feof = dlsym(RTLD_NEXT,"feof");
    _fread = dlsym(RTLD_NEXT,"fread");
    _fwrite = dlsym(RTLD_NEXT,"fwrite");
    _fgets = dlsym(RTLD_NEXT,"fgets");
    _fputs = dlsym(RTLD_NEXT,"fputs");
    _fseek = dlsym(RTLD_NEXT,"fseek");
    //_freopen = dlsym(RTLD_NEXT,"freopen");
    _fgetpos = dlsym(RTLD_NEXT,"fgetpos");
    _fsetpos = dlsym(RTLD_NEXT,"fsetpos");
    _ftell = dlsym(RTLD_NEXT,"ftell");


    // Set original fcntl and unistd functions

    _open = dlsym(RTLD_NEXT,"open");
    //_open64 = dlsym(RTLD_NEXT,"open64");
    _close = dlsym(RTLD_NEXT,"close");
    _read = dlsym(RTLD_NEXT,"read");
    _write = dlsym(RTLD_NEXT,"write");
    _pread = dlsym(RTLD_NEXT,"pread");
    _pwrite = dlsym(RTLD_NEXT,"pwrite");
    //_creat = dlsym(RTLD_NEXT,"creat");

    printf("START\n");

    logFile = _fopen("log.txt","w");

    program_start = clock();           // Set global program clock

    printf("%lf      ",(double)program_start/CLOCKS_PER_SEC);

}

// Destructor

static void wrap_deinit(void)
{
    _fclose(logFile);
    printf("End\n");
}


/* WRAPPER FOR FOPEN */

FILE* fopen(const char *pathname,const char *mode)
{   
    //printf("INTERCEPTED Fopen\n");

    clock_t start = clock();

    FILE * ret = _fopen(pathname,mode);

    clock_t end = clock();

    double called_time = (double)(start-program_start)/(double)(CLOCKS_PER_SEC);

    double exec_time = (double)(end-start)/(double)(CLOCKS_PER_SEC);

    pid_t pid = getpid();


    fprintf(logFile,"%lf %lf %ld fopen %s %s = %p \n",called_time,exec_time,(long)pid,pathname,mode,ret);

    return ret;
}

/* WRAPPER FOR FCLOSE */

int fclose(FILE *stream)
{
    //printf("INTERCEPTED Fclose\n");

    clock_t start = clock();

    int ret = _fclose(stream);

    clock_t end = clock();

    double called_time = (double)(start-program_start)/(double)(CLOCKS_PER_SEC);

    double exec_time = (double)(end-start)/(double)(CLOCKS_PER_SEC);

    pid_t pid = getpid();


    fprintf(logFile,"%lf %lf %ldfclose %p = %d\n",called_time,exec_time,(long)pid,stream,ret);

    return ret;
}

/* WRAPPER FOR FGETC */

int fgetc(FILE *__stream)
{
    //printf("INTERCEPTED Fgetc\n");

    clock_t start = clock();

    int ret = _fgetc(__stream);
    
    clock_t end = clock();

    double called_time = (double)(start-program_start)/(double)(CLOCKS_PER_SEC);

    double exec_time = (double)(end-start)/(double)(CLOCKS_PER_SEC);

    pid_t pid = getpid();

    fprintf(logFile,"%lf %lf %ld fgetc %p = %d\n",called_time,exec_time,(long)pid,__stream,ret);

    return ret;
}

/* WRAPPER FOR FPUTC*/

int fputc(int character, FILE *stream)
{
    //printf("INTERCEPTED Fputc\n");

    clock_t start = clock();

    int ret = _fputc(character,stream);

    clock_t end = clock();

    double called_time = (double)(start-program_start)/(double)(CLOCKS_PER_SEC);

    double exec_time = (double)(end-start)/(double)(CLOCKS_PER_SEC);

    pid_t pid = getpid();

    fprintf(logFile,"%lf %lf %ld fputc %d %p = %d\n",called_time,exec_time,(long)pid,character,stream,ret);

    return ret;
}

/* WRAPPER FOR FREAD */

size_t fread( void * ptr, size_t size, size_t count, FILE * stream )
{
    //printf("INTERCEPTED fread\n");

    clock_t start = clock();

    size_t ret = _fread(ptr,size,count,stream);

    clock_t end = clock();

    double called_time = (double)(start-program_start)/(double)(CLOCKS_PER_SEC);

    double exec_time = (double)(end-start)/(double)(CLOCKS_PER_SEC);

    pid_t pid = getpid();

    fprintf(logFile,"%lf %lf %ld fread %zu %zu %p = %zu\n",called_time,exec_time,(long)pid,size,count,stream,ret);

    return ret;
}

size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream )
{
    //printf("INTERCEPTED fwriten");

    clock_t start = clock();

    size_t ret = _fwrite(ptr,size,count,stream);

    clock_t end = clock();

    double called_time = (double)(start-program_start)/(double)(CLOCKS_PER_SEC);

    double exec_time = (double)(end-start)/(double)(CLOCKS_PER_SEC);

    pid_t pid = getpid();

    fprintf(logFile,"%lf %lf %ld fwrite %zu %zu %p = %zu\n",called_time,exec_time,(long)pid,size,count,stream,ret);

    return ret;
}


int fsetpos ( FILE * stream, const fpos_t * pos )
{
    //printf("INTERCEPTED fsetpos\n");

    clock_t start = clock();

    int ret = _fsetpos(stream,pos);

    clock_t end = clock();

    double called_time = (double)(start-program_start)/(double)(CLOCKS_PER_SEC);

    double exec_time = (double)(end-start)/(double)(CLOCKS_PER_SEC);

    pid_t pid = getpid();

    fprintf(logFile,"%lf %lf %ld fsetpos %p = %d\n",called_time,exec_time,(long)pid,stream,ret);

    return ret;
}

int fgetpos ( FILE * stream, fpos_t * pos )
{
    //printf("INTERCEPTED fgetpos\n");

    clock_t start = clock();

    int ret = _fgetpos(stream,pos);

    clock_t end = clock();

    double called_time = (double)(start-program_start)/(double)(CLOCKS_PER_SEC);

    double exec_time = (double)(end-start)/(double)(CLOCKS_PER_SEC);

    pid_t pid = getpid();

    fprintf(logFile,"%lf %lf %ld fgetpos %p = %d\n",called_time,exec_time,(long)pid,stream,ret);

    return ret;
}

char * fgets ( char * str, int num, FILE * stream )
{
    //printf("INTERCEPTED fgets\n");

    clock_t start = clock();

    char* ret = _fgets(str,num,stream);
	
	clock_t end = clock();

    double called_time = (double)(start-program_start)/(double)(CLOCKS_PER_SEC);

    double exec_time = (double)(end-start)/(double)(CLOCKS_PER_SEC);

    pid_t pid = getpid();

    fprintf(logFile,"%lf %lf %ld fgets %s %d %p = %s\n",called_time,exec_time,(long)pid,str,num,stream,ret);


    return ret;

}

int fputs ( const char * str, FILE * stream )
{
    //printf("INTERCEPTED fputs\n");

    clock_t start = clock();

    int ret = _fputs(str,stream);

	clock_t end = clock();

    double called_time = (double)(start-program_start)/(double)(CLOCKS_PER_SEC);

    double exec_time = (double)(end-start)/(double)(CLOCKS_PER_SEC);

    pid_t pid = getpid();

    fprintf(logFile,"%lf %lf %ld fputs %s %p = %d\n",called_time,exec_time,(long)pid,str,stream,ret);


    return ret;

}

int fseek ( FILE * stream, long int offset, int origin )
{
    //printf("INTERCEPTED fseek\n");

    clock_t start = clock();

    int ret = _fseek(stream,offset,origin);
	
	clock_t end = clock();

    double called_time = (double)(start-program_start)/(double)(CLOCKS_PER_SEC);

    double exec_time = (double)(end-start)/(double)(CLOCKS_PER_SEC);

    pid_t pid = getpid(); 

    fprintf(logFile,"%lf %lf %ld fseek %p %ld %d = %d\n",called_time,exec_time,(long)pid,stream,offset,origin,ret);


    return ret;
}

long int ftell ( FILE * stream )
{
    //printf("INTERCEPTED ftell\n");

    clock_t start = clock();

    long int ret = _ftell(stream);

	clock_t end = clock();

    double called_time = (double)(start-program_start)/(double)(CLOCKS_PER_SEC);

    double exec_time = (double)(end-start)/(double)(CLOCKS_PER_SEC);

    pid_t pid = getpid();

    fprintf(logFile,"%lf %lf %ld ftell %p = %ld\n",called_time,exec_time,(long)pid,stream,ret);


    return ret;
}

int feof ( FILE * stream )
{
    //printf("INTERCEPTED feof\n");

    clock_t start = clock();

    int ret = _feof(stream);

	clock_t end = clock();

    double called_time = (double)(start-program_start)/(double)(CLOCKS_PER_SEC);

    double exec_time = (double)(end-start)/(double)(CLOCKS_PER_SEC);

    pid_t pid = getpid();

    fprintf(logFile,"%lf %lf %ld feof %p = %d\n",called_time,exec_time,(long)pid,stream,ret);


    return ret;
}


/* WRAPPER FOR OPEN */
int open(const char *pathname, int flags, ...)
{

    //printf("INTERCEPTED Open\n");

    mode_t mode;
    va_list vl;
    va_start(vl, flags);
    mode = va_arg(vl, mode_t);
    va_end(vl);

    clock_t start = clock();
    
    int ret = _open(pathname, flags, mode);

	clock_t end = clock();

    double called_time = (double)(start-program_start)/(double)(CLOCKS_PER_SEC);

    double exec_time = (double)(end-start)/(double)(CLOCKS_PER_SEC);

    pid_t pid = getpid();

    fprintf(logFile,"%lf %lf %ld open %s %d = %d\n",called_time,exec_time,(long)pid,pathname,flags,ret);


    return ret;
	

}

ssize_t read(int fd, void *buf, size_t count)
{
    //printf("INTERCEPTED read\n");
	
	clock_t start = clock();
    
    ssize_t ret = _read(fd,buf,count);

	clock_t end = clock();

    double called_time = (double)(start-program_start)/(double)(CLOCKS_PER_SEC);

    double exec_time = (double)(end-start)/(double)(CLOCKS_PER_SEC);

    pid_t pid = getpid();

    fprintf(logFile,"%lf %lf %ld read %d %p %zu = %zu\n",called_time,exec_time,(long)pid,fd,buf,count,ret);


    return ret;
}
ssize_t write(int fd, const void *buf, size_t count)
{
    //printf("INTERCEPTED write\n");
	
	clock_t start = clock();
    
    ssize_t ret = _write(fd,buf,count);

	clock_t end = clock();

    double called_time = (double)(start-program_start)/(double)(CLOCKS_PER_SEC);

    double exec_time = (double)(end-start)/(double)(CLOCKS_PER_SEC);

    pid_t pid = getpid();

    fprintf(logFile,"%lf %lf %ld write %d %p %zu = %zu\n",called_time,exec_time,(long)pid,fd,buf,count,ret);


    return ret;
    
}

ssize_t pread(int fd, void *buf, size_t count, off_t offset)
{
    //printf("INTERCEPTED pread\n");
	
		
	clock_t start = clock();
    
    ssize_t ret = _pread(fd,buf,count,offset);

	clock_t end = clock();

    double called_time = (double)(start-program_start)/(double)(CLOCKS_PER_SEC);

    double exec_time = (double)(end-start)/(double)(CLOCKS_PER_SEC);

    pid_t pid = getpid();

    fprintf(logFile,"%lf %lf %ld pread %d %p %zu %ld = %zu\n",called_time,exec_time,(long)pid,fd,buf,count,offset,ret);


    return ret;

 
}


ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset)
{
    //printf("INTERCEPTED pwrite\n");

	clock_t start = clock();
    
    ssize_t ret = _pwrite(fd,buf,count,offset);

	clock_t end = clock();

    double called_time = (double)(start-program_start)/(double)(CLOCKS_PER_SEC);

    double exec_time = (double)(end-start)/(double)(CLOCKS_PER_SEC);

    pid_t pid = getpid();

    fprintf(logFile,"%lf %lf %ld pwrite %d %p %zu %ld = %zu\n",called_time,exec_time,(long)pid,fd,buf,count,offset,ret);
    

    return ret;
    
}

int close(int fd)
{
   
	
	clock_t start = clock();
    
    int ret = _close(fd);

	clock_t end = clock();

    double called_time = (double)(start-program_start)/(double)(CLOCKS_PER_SEC);

    double exec_time = (double)(end-start)/(double)(CLOCKS_PER_SEC);

    pid_t pid = getpid();

    fprintf(logFile,"%lf %lf %ld close %d = %d\n",called_time,exec_time,(long)pid,fd,ret);

    return ret;
}

/*
int open64(const char *pathname, int flags, ...)
{
    printf("INTERCEPTED Open64\n");

    mode_t mode;
    va_list vl;
    va_start(vl, flags);
    mode = va_arg(vl, mode_t);
    va_end(vl);


    return _open64(pathname, flags, mode);
}
*/
