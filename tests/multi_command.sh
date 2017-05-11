#!/bin/sh
make

echo "echo hello && mkdir test || echo world; git status; rtn" > text.txt
bin/rshell text.txt 

echo "ecccco hello && mkdir test || echo world; git status; rtn" > text.txt
bin/rshell text.txt

echo "echo hello && mkdir test || echo world; git status || rtn" > text.txt
bin/rshell text.txt

echo "hello && mkdir test || echo world; ## git status; rtn" > text.txt
bin/rshell text.txt

echo "mkdir test || echo world; git status && echo hello && ecco science; exit" > text.txt
bin/rshell text.txt

echo "mkdir test || echo world; git status || echo hello || ecco pokemon; exit" > text.txt
bin/rshell text.txt

echo "mkdir test || echo world; git status || || echo hello || ecco pokemon; exit" > text.txt
bin/rshell text.txt

echo "mkdir test; echo world && && git status || echo hello || ecco pokemon; exit" > text.txt
bin/rshell text.txt

#bin/rshell