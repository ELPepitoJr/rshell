### Assignment 3, By Jonnathein Chambers & Salvador Esquivias

The program is broken down into different components:
assn.cpp parses the command string taken from text.txt.
The .sh files echo test cases to text.txt, so that the cpp file takes takes
input from the file stream and inserts each command into a vector. 
The parse.cpp file is responsible for inserting strings, Base pointers, getting info, and executing commands.

Here is a summary of known bugs resolved in our program:

1. Parsing (in assn3.cpp)
  a. Fixed bugs leftover from Assignment 2
  b. nested precedence causing parsing errors (fixed)
  c. comments not working inside precedence (fixed)
  d. Recursive parsing failing to store in sub-vectors (fixed)
  e. strings not being stored into recursive function properly (fixed)
  f. extra parenthesis being parsed (fixed)

2. Executing Commands
  a. throw out of range errors (resolved)
  b. crashing with fork() function (resolved)
  b.2) forking not working with test function (resolved)
  c. accessing protected members error (resolved)
  d. calling apply on unitialized vector (resolved)
  e. program did not recongnize the (&& || ;) conditions (resolved)
  e.2) program did not recognize conditional expresions for precedence (resolved)
  f. if a semi colon between commands, it still runs the next command (fixed in Assignment 3)

3. Shell Files
  a. errors from improper syntax (resolved)