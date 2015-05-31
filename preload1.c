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
static FILE *(*_fopen)(const char *, const char *) = NULL;

// Pointers to original fcntl.h and unistd.h functions



// Constructor

static void wrap_init(void)
{
    program_start = clock();

    _fopen = dlsym(RTLD_NEXT,"fopen");
    

}

// Destructor

static void wrap_deinit(void)
{

}


/* WRAPPER FOR FOPEN */

FILE* fopen(const char *pathname,const char *mode)
{	
		
    _fopen("log.txt","w");
		return _fopen(pathname,mode);

}


int open(const char *pathname, int flags, ...)
{

	printf("INTERCEPTED OPEN \n ");
    static int (*_open)(const char *, int, mode_t) = NULL;
    mode_t mode;
    va_list vl;
    va_start(vl, flags);
    mode = va_arg(vl, mode_t);
    va_end(vl);

    if (!_open) {
        _open = dlsym(RTLD_NEXT, "open");
    }
        
    return _open(pathname, flags, mode);

}

int open64(const char *pathname, int flags, ...)
{
	printf("INTERCEPTED OPEN 64 \n ");
    static int (*_open64)(const char *, int, mode_t) = NULL;
    mode_t mode;
    va_list vl;
    va_start(vl, flags);
    mode = va_arg(vl, mode_t);
    va_end(vl);

    if (!_open64) {
        _open64 = dlsym(RTLD_NEXT, "open64");
    }

    return _open64(pathname, flags, mode);
}
