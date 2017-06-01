#!/bin/sh
make

echo "git status; (echo hi && exit)" > text.txt
bin/rshell text.txt

echo "git status; ((echo hi && echo "hi all") && exit)" > text.txt
bin/rshell text.txt

echo "git status; (git status && test /bin/notexist.cpp; exit)" > text.txt
bin/rshell text.txt