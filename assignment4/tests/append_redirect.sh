#!/bin/sh
make 

#!/bin/sh
make

echo "ls > test.txt" >> text.txt
bin/rshell text.txt

echo "ls | tail > test.txt" >> text.txt
bin/rshell text.txt

echo "ls | sort | tail | awk | uniq > test.txt" >> text.txt
bin/rshell text.txt

echo "ls | sort > a.txt" >> text.txt
bin/rshell text.txt

echo "ls | head | sort > test.txt" >> text.txt
bin/rshell text.txt

echo "ls | uniq | head > text.txt" >> text.txt
bin/rshell text.txt