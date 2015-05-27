#This repo contains code to profile linux input/output operations using LD_PRELOAD.

# To compile

gcc -shared -Wall -fPIC myfopen.c -o myfopen.so -ldl

# TO RUN

LD_PRELOAD=./myfopen.so ./smalltest

#Remove reference(Genereal info. No relation to this repo)

git filter-branch -f --index-filter "git rm -rf --cached --ignore-unmatch FOLDER/FILENAME" -- --all

rm -rf .git/refs/original/
git reflog expire --expire=now --all
git gc --prune=now
git gc --aggressive --prune=now

git push --all --force


