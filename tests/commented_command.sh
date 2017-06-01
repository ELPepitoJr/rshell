#!/bin/sh
make

echo "echo hi || echo no || (echo why && echo no### || echo )" > text.txt
bin/rshell text.txt 

echo "git status && ((echo no || echo why) && echo why ## echo no" > text.txt
bin/rshell text.txt 

echo "git status || (echo world || echo hi) || test -e libs/assn3.d" > text.txt
bin/rshell text.txt 

echo "git status; (echo world || echo hi) && test -e libs/assn3.d && echo## world" > text.txt
bin/rshell text.txt 

echo "echo "pokemon mystery" && (echo hi ## error error error)" > text.txt
bin/rshell text.txt 

echo "git status ##(echo no && echo hi) && mkdir master" > text.txt
bin/rshell text.txt 

echo "echo no; (echo## sfnsfksjfsfkjsfk" > text.txt
bin/rshell text.txt