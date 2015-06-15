CC=gcc

preload: preload.c
	$(CC) -shared -Wall -fPIC preload.c -o preload.so -ldl

test:
	LD_PRELOAD=./preload.so $(command)

testfile:
	$(CC) -Wall -Werror $(file).c -o $(file)
	LD_PRELOAD=./preload.so $(file)

clean:
	rm -f preload.so