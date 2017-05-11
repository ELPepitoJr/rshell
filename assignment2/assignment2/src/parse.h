#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <vector>

using namespace std;



class Executable 
{
    protected:
        string info;
        char validate; //may not use
        vector<Executable*> output;
        vector<string> input;
        
    public:
		Executable() {};
		Executable(string info);
		
		string getInfo();
		char getValidate();
		void changeValidate(char validate);
		
		void print(); //test string outputs
        void insert(Executable* entry, string object);
        void execute(); //has a loop to call apply
		virtual void apply(int x, vector<Executable*> out, vector<string> in); //utilizes loop from execute() to execute commands
		void deleteVector();

};

class Semicolon : public Executable 
{
    public:
		Semicolon();
		Semicolon(string info);
		void apply(int x, vector<Executable*> out, vector<string> in);
};


class Ampersand  : public Executable 
{
    public:
		Ampersand();
		Ampersand(string info);
		void apply(int x, vector<Executable*> out, vector<string> in);
};


class Vertical : public Executable 
{
    public:
		Vertical();
		Vertical(string info);
		void apply(int x, vector<Executable*> out, vector<string> in);
};


#endif 