#!/bin/sh
make 

echo "ls | head" > text.txt
bin/rshell text.txt

echo "ls | tail" > text.txt
bin/rshell text.txt

echo "ls | tail | fmt | sort" > text.txt
bin/rshell text.txt

echo "ls | fmt | sort" > text.txt
bin/rshell text.txt

echo "ls | fmt | uniq | sort" > text.txt
bin/rshell text.txt

echo "ls | uniq | head | sort" > text.txt
bin/rshell text.txt
