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
        char ignore;
        char **cmnd;
        char type; //specifies what type of direction/piping is being used
		string file;
        
    public:
		Executable();
		Executable(string info);
		
		string getInfo();
		char get_ignore();
		char getValidate();
		char** get_cmnd();
		string get_file() { return this->file; }
		char get_type() { return this->type; }
		
		unsigned int getVectorSize();
		void changeValidate(char validate);
		void changeIgnore(char ignore);
		void changeType(char type);
		void print(); //test string outputs
        void insert(Executable* entry, string object);
        virtual void parse(string arg_hold, string& arg3, istringstream& inSS, istringstream& inSS2, istringstream& inSS3) {};
        void execute(); //has a loop to call apply
		virtual void apply(unsigned int x, vector<Executable*> out, vector<string> in); //utilizes loop from execute() to execute commands
		void deleteVector();

};

class Semicolon : public Executable 
{
    public:
		Semicolon();
		Semicolon(string info);
		void apply(unsigned int x, vector<Executable*> out, vector<string> in);
};


class Ampersand  : public Executable 
{
    public:
		Ampersand();
		Ampersand(string info);
		void apply(unsigned int x, vector<Executable*> out, vector<string> in);
};


class Vertical : public Executable 
{
    public:
		Vertical();
		Vertical(string info);
		void apply(unsigned int x, vector<Executable*> out, vector<string> in);
};


class ParVertical : public Executable
{
	// private:
	// 	vector<Executable*> v_hold;
	public:
		ParVertical();
		void parse(string arg_hold, string& arg3, istringstream& inSS, istringstream& inSS2, istringstream& inSS3); //special parsing for recursion
		void apply(unsigned int x, vector<Executable*> out, vector<string> in);
};

class ParAmpersand : public Executable
{
	// private:
	// 	vector<Executable*> a_hold;
	public:
		ParAmpersand();
		void parse(string arg_hold, string& arg3, istringstream& inSS, istringstream& inSS2, istringstream& inSS3); //special parsing for recursion
		void apply(unsigned int x, vector<Executable*> out, vector<string> in);
};

class ParSemicolon : public Executable
{
	// private:
	// 	vector<Executable*> s_hold;
	public:
		ParSemicolon();
		void parse(string arg_hold, string& arg3, istringstream& inSS, istringstream& inSS2, istringstream& inSS3); //special parsing for recursion
		void apply(unsigned int x, vector<Executable*> out, vector<string> in);
};

class Direction : public Executable
{
	public:
		Direction():Executable() {};
		Direction(string info, char ignore, char type);
		Direction(string info, string file, char ignore, char type);
		
		char get_ignore();
		string get_file();
		char get_type();
		void apply(unsigned int x, vector<Executable*> out, vector<string> in);
};



#endif 