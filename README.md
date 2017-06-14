### Assignment 3, By Jonnathein Chambers & Salvador Esquivias

##Note: Assignment 4 was built under the assumption that this program will work independently from the tasks of previous assignments

The program is broken down into different components:

assn.cpp parses the command string taken from text.txt.

The .sh files echo test cases to text.txt, so that the cpp file takes takes
input from the file stream and inserts each command into a vector (for student testing only).

The parse.cpp file is responsible for inserting strings, Base pointers, getting info, and executing commands.

Here is a summary of known bugs in our program:

1. Parsing (in assn4.cpp)
  a. string logic errors (fixed)
  b. parsing not handling i/o operators properly (fixed)

2. Executing Commands
  a. throw out of range errors (resolved)
  b. crashing with fork() function (resolved)
  c. accessing protected members error (resolved)
  d. apply not handling piping and dup correctly (fixed)
  

3. Shell Files
  a. errors from improper syntax (resolved)

4. Assignment 4 Problems
  a. tr and grep when used deletes file instead of changing file with its respective arguments
  b. some edge test cases will not work (especially with tr and grep) most liekly will just get empty files
  c. everything else should work as it is supposed to
  d. input redirection ('<') may have some bugs with inserting a filename

