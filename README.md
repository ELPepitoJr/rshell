### Assignment 2, By Jonnathein Chambers & Salvador Esquivias

The program is broken down into different components:
assn.cpp parses the command string taken from text.txt.
The .sh files echo test cases to text.txt, so that the cpp file takes takes
input from the file stream and inserts each command into a vector. 
The parse.cpp file is responsible for inserting strings, Base pointers, getting info, and executing commands.

Here is a summary of known bugs resolved in our program:

1. Parsing (in assn2.cpp)
  a. semicolons, ampersands, and vertical lines were not being eliminated from the strings (resolved)
  b. segfault for storing strings in a vector (resolved)
  c. Every odd numbered command is only being stored (resolved)
  d. Base pointer vector is not storing resolved type pointers properly (resolved)
  e. file streaming not inserting info into strings corectly (resolved)
2. Constructors
  a. Constructors are not coded properly (resolved)
  b. Subclasses were not inheriting constructors from base class (resolved)
3. Executing Commands
  a. throw out of range errors (resolved)
  b. crashing with fork() function (resolved)
  c. accessing protected members error (resolved)
  d. calling apply on unitialized vector (resolved)
  e. program did not recongnize the (&& || ;) conditions (resolved)
  f. if a a semicolonbetween commands, it still runs the next command
4. Shell Files
  a. errors from improper syntax (resolved)