CC=gcc

preload: preload.c
	$(CC) -shared -Wall -fPIC preload.c -o preload.so -ldl

preload_file:
	$(CC) -shared -Wall -fPIC $(pf).c -o $(pf).so -ldl


test:
	$(CC) -shared -Wall -fPIC $(pf).c -o $(pf).so -ldl
	LD_PRELOAD=./$(pf).so $(command)

testfile:
	touch test.txt
	touch test2.txt
	touch test3.txt
	touch test4.txt
	$(CC) -shared -Wall -fPIC preload.c -o $(pf).so -ldl
	$(CC) -Wall -Werror $(file).c -o $(file)
	chmod +x $(file)
	LD_PRELOAD=./$(pf).so ./$(file)

clean:
	rm -f test.txt 
	rm -f test2.txt 
	rm -f test3.txt 
	rm -f test4.txt
	rm -f preload.so
	rm -f preload_tsc.so
	rm -f log.txt
	rm -f smalltest