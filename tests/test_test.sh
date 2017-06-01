#!/bin/sh
make

echo "test -e cs-work/cs100/assignment3/bin/libs/assn3.d && echo world || test -f cs-work/cs100/assignment3/src/assn3.cpp; exit" > text.txt
bin/rshell text.txt

echo "echo stop; echo making; echo us && echo these && eco assignments || echo assignments && test -d cs-work/cs100/bin; exit" > text.txt
bin/rshell text.txt

echo "test -e fork.cpp || test -f cs-work/cs100/assignment3/tests/exit.sh && echo works; exit" > text.txt
bin/rshell text.txt

echo "test -d cs-work/cs100/assignment3; ls -a" > text.txt
bin/rshell text.txt

echo "test cs-work/cs100" > text.txt
bin/rshell text.txt

echo "echo hi && (git status; test -f cs-work/cs100/assignment3) || echo "does not exist" " > text.txt
bin/rshell text.txt