CC=gcc
CFLAGS=-ldl -wall -fPIC

preload: preload.so
	$(CC) -shared -Wall -fPIC preload.c -o preload.so -ldl