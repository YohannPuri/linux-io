CC=gcc

preload: preload.c
	$(CC) -shared -Wall -fPIC preload.c -o preload.so -ldl

preload_file:
	$(CC) -shared -Wall -fPIC $(pf).c -o $(pf).so -ldl


test:
	$(CC) -shared -Wall -fPIC $(pf).c -o $(pf).so -ldl
	LD_PRELOAD=./$(pf).so $(command)

testfile:
	mkdir folder1
	touch folder1/test.txt
	mkdir folder1/sub1
	touch folder1/sub1/test2.txt
	touch test3.txt
	mkdir folder2
	touch folder2/test4.txt
	$(CC) -shared -Wall -fPIC $(pf).c -o $(pf).so -ldl
	$(CC) -g -Wall -Werror $(file).c -o $(file)
	chmod +x $(file)
	LD_PRELOAD=./$(pf).so ./$(file)

fs:
	gcc -g makefs.c -o makefs
	./makefs mount/1/2

clean:
	rm -f test.txt 
	rm -f test2.txt 
	rm -f test3.txt 
	rm -f test4.txt
	rm -f preload.so
	rm -f preload_tsc.so
	rm -f log.txt
	rm -f smalltest