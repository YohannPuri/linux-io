#This repo contains code to profile linux input/output operations using LD_PRELOAD.

# To compile

gcc -shared -Wall -fPIC myfopen.c -o myfopen.so -ldl

# TO RUN

LD_PRELOAD=./myfopen.so ./smalltest
