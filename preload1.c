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
clock_t program_start;


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
    program_start = clock();            // Set global program clock

    // Set original stdio functions

    _fopen = dlsym(RTLD_NEXT,"fopen");
    _fclose = dlsym(RTLD_NEXT,"fclose");
    //_fprintf = dlsym(RTLD_NEXT,"fprintf");
    //_vfprintf = dlsym(RTLD_NEXT,"vfprintf");
    //_fscanf = dlsym(RTLD_NEXT,"fscanf");
    //_sscanf = dlsym(RTLD_NEXT,"sscanf");
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

}

// Destructor

static void wrap_deinit(void)
{
    printf("End\n");
}


/* WRAPPER FOR FOPEN */

FILE* fopen(const char *pathname,const char *mode)
{   

    printf("INTERCEPTED Fopen\n");


    return _fopen(pathname,mode);

}

/* WRAPPER FOR FCLOSE */

int fclose(FILE *stream)
{
    printf("INTERCEPTED Fclose\n");

    return _fclose(stream);
}

/* WRAPPER FOR FGETC */

int fgetc(FILE *__stream)
{
    printf("INTERCEPTED Fgetc\n");

    return _fgetc(__stream);
}

/* WRAPPER FOR FPUTC*/

int fputc(int character, FILE *stream)
{
    printf("INTERCEPTED Fputc\n");

    return _fputc(character,stream);
}

/* WRAPPER FOR FREAD */

size_t fread( void * ptr, size_t size, size_t count, FILE * stream )
{
    printf("INTERCEPTED fread\n");

    return _fread(ptr,size,count,stream);
}

size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream )
{
    printf("INTERCEPTED fwriten");
    return _fwrite(ptr,size,count,stream);
}


int fsetpos ( FILE * stream, const fpos_t * pos )
{
    printf("INTERCEPTED fsetpos\n");

    return _fsetpos(stream,pos);
}

int fgetpos ( FILE * stream, fpos_t * pos )
{
    printf("INTERCEPTED fgetpos\n");

    return _fgetpos(stream,pos);
}

char * fgets ( char * str, int num, FILE * stream )
{
    printf("INTERCEPTED fgets\n");

    return _fgets(str,num,stream);

}

int fputs ( const char * str, FILE * stream )
{
    printf("INTERCEPTED fputs\n");

    return _fputs(str,stream);

}

int fseek ( FILE * stream, long int offset, int origin )
{
    printf("INTERCEPTED fseek\n");
    return _fseek(stream,offset,origin);
}

long int ftell ( FILE * stream )
{
    printf("INTERCEPTED ftell\n");
    return _ftell(stream);
}

int feof ( FILE * stream )
{
    printf("INTERCEPTED feof\n");
    return _feof(stream);
}


/* WRAPPER FOR OPEN */
int open(const char *pathname, int flags, ...)
{

    printf("INTERCEPTED Open\n");

    mode_t mode;
    va_list vl;
    va_start(vl, flags);
    mode = va_arg(vl, mode_t);
    va_end(vl);

        
    return _open(pathname, flags, mode);

}

ssize_t read(int fd, void *buf, size_t count)
{
    printf("INTERCEPTED read\n");

    return _read(fd,buf,count);
}
ssize_t write(int fd, const void *buf, size_t count)
{
    printf("INTERCEPTED write\n");

    return _write(fd,buf,count);
}

ssize_t pread(int fd, void *buf, size_t count, off_t offset)
{
    printf("INTERCEPTED pread\n");

    return _pread(fd,buf,count,offset);
}


ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset)
{
    printf("INTERCEPTED pwrite\n");

    return _pwrite(fd,buf,count,offset);
}

int close(int fd)
{
    printf("INTERCEPTED Close\n");

    return _close(fd);
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
