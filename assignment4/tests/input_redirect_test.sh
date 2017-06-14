#!/bin/sh
make

echo "uniq < tester.txt" > text.txt
bin/rshell text.txt

echo "head < tester.txt | sort | awk" > text.txt
bin/rshell text.txt

echo "tail < tester.txt | sort | fmt > a.txt" > text.txt
bin/rshell text.txt

echo "sort < head | fmt | sort > p.txt" > text.txt
bin/rshell text.txt

echo "fmt < test.txt | head | sort | fmt | tail" > text.txt
bin/rshell text.txt