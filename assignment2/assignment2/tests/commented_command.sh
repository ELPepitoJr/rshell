#!/bin/sh
make

echo "echo te#st || echo world; git status && echo hello && ecco science; exit" > text.txt
bin/rshell text.txt

echo "ls -a; echo world; #git status && echo hello && ecco science; exit" > text.txt
bin/rshell text.txt

echo "mkdir test; echo world; git status && echo hello && ecco science; exit#nono" > text.txt
bin/rshell text.txt
