#!/bin/sh
make

echo "echo hello" > text.txt

bin/rshell

echo "ls -a" > text.txt

bin/rshell

echo "git status" > text.txt

bin/rshell

echo "mkdir test" > text.txt

bin/rshell

echo "exit" > text.txt


bin/rshell