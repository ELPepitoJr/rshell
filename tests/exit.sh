#!/bin/sh
make

echo "exit" > text.txt
bin/rshell text.txt

echo "echo hello; echo world; git status; exit" > text.txt
bin/rshell text.txt

echo "echo hello; echo world; darn status || rtn" > text.txt
bin/rshell text.txt

echo "echo hello; echo world; non status && rtn" > text.txt
bin/rshell text.txt