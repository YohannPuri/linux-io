CC=gcc

preload: preload.c
	$(CC) -shared -Wall -fPIC preload.c -o preload.so -ldl

test:
	LD_PRELOAD=./preload.so $(command)

clean:
	rm -f preload.so