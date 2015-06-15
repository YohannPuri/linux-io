CC=gcc
CFLAGS=-ldl -wall -fPIC -I

preload: preload.so
	$(CC) -shared -Wall -fPIC preload.c -o preload.so -ldl -I