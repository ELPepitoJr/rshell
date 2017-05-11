#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

//Base Executable
Executable::Executable(string info) 
{
    this->info = info;
}


string Executable::getInfo() { return this->info; }

char Executable::getValidate() { return this->validate; }

void Executable::changeValidate(char validate)
{
    this->validate = validate;
}

void Executable::insert(Executable* entry, string object) 
{
    output.push_back(entry);
    input.push_back(object);
    
}

void Executable::print()
{
	for(unsigned int i = 0; i < output.size(); ++i)
	{
		cout << output.at(i)->info << " ";
		cout << input.at(i) << " ";
	}
	
	cout << endl << "Test case successfully printed." << endl << endl;
}
//http://stackoverflow.com/questions/347949/how-to-convert-a-stdstring-to-const-char-or-char
//Loop up the arguments for execvp()

void Executable::execute()
{
	// if(this->output.size() == 1)
	// {
	// 	if (output.at(0)->getInfo() == "exit") //working properly
	// 	{ 
	// 		cout << "Return command (rtn) was called. Terminating." << endl << endl;
	// 		this->deleteVector();
	// 		exit (EXIT_SUCCESS); 
	// 	}
		
	// 	char* c[3]; 
	// 	c[0] = (char*)this->output.at(0)->info.c_str();
	// 	c[1] = (char*)this->input.at(0).c_str();
	// 	c[2] = NULL;
		
	// 	if(execvp(c[0],c) == -1)
	// 	{
	// 		perror("exec");
	// 	}
	
	// 	return;
	// }

	for(unsigned int i = 0; i < this->output.size(); ++i)
	{
		if (output.at(i)->getInfo() == "exit") //working properly
		{ 
			cout << "Return command (exit) was called. Terminating." << endl << endl;
			this->deleteVector();
			exit (EXIT_SUCCESS); 
		}
		
		// if (input.at(i) == "rtn") //working properly
		// { 
		// 	cout << "Return command (rtn) was called. Terminating." << endl << endl;
		// 	return; 
		// }
		
		this->output.at(i)->apply(i, this->output, this->input);
	}
	this->deleteVector();
	//cout << "Successful; exit command not called" << endl << endl;
}
void Executable::deleteVector()
{
	this->output.resize(0);
	this->input.resize(0);
}

void Executable::apply(int x, vector<Executable*> out, vector<string> in)
{
	//will only call reeolved sub-class functions
}

//Semicolon
Semicolon::Semicolon():Executable() {}
Semicolon::Semicolon(string info):Executable(info) {}

void Semicolon::apply(int x, vector<Executable*> out, vector<string> in)
{
	//cout << x << endl; //test output
	if(out.at(x)->getInfo() == "exit") {exit (EXIT_SUCCESS);;}
	
	char* c[3];
	c[0] = (char*)out.at(x)->getInfo().c_str();
	c[1] = (char*)in.at(x).c_str();
	c[2] = NULL;
	
	pid_t pid = fork();
	
	if(pid < 0)
	{
		perror("fork()");
	}
	else if(pid == 0)
	{
		execvp(c[0], c);
		exit(0);
	}
	else
	{
		int status;
		
		if(wait(&status) == -1)
		{
			perror("wait()");
		}
		
		if(WIFEXITED(status))
		{
			if(WEXITSTATUS(status) == 0)
			{
				out.at(x)->changeValidate('y');
			}
			else
			{
				out.at(x)->changeValidate('n');
			}
		}
		else
		{
			perror("execvp error");
		}
		
	}
}

//Ampersand
Ampersand::Ampersand():Executable() {}

Ampersand::Ampersand(string info):Executable(info) {}

void Ampersand::apply(int x, vector<Executable*> out, vector<string> in)
{
	 //cout << x << endl; //test output
	 
	if(out.at(x)->getInfo() == "exit") {exit (EXIT_SUCCESS);}
	
	if( out.at(x - 1)->getValidate() == 'n') 
	{
		out.at(x)->changeValidate('n');
		return;
	}
	
	char* c[3];
	c[0] = (char*)out.at(x)->getInfo().c_str();
	c[1] = (char*)in.at(x).c_str();
	c[2] = NULL;
	
	pid_t pid = fork();
	
	if(pid < 0)
	{
		perror("fork()");
	}
	else if(pid == 0)
	{
		execvp(c[0], c);
		exit(0);
	}
	else
	{
		int status;
		
		if(wait(&status) == -1)
		{
			perror("wait()");
		}
		
		if(WIFEXITED(status))
		{
			if(WEXITSTATUS(status) == 0)
			{
				out.at(x)->changeValidate('y');
			}
			else
			{
				out.at(x)->changeValidate('n');
			}
		}
		else
		{
			perror("execvp error");
		}
		
	}
}

//vertical
Vertical::Vertical():Executable() {}

Vertical::Vertical(string info):Executable(info) {}

void Vertical::apply(int x, vector<Executable*> out, vector<string> in) 
{
	if( out.at(x - 1)->getValidate() == 'y') 
	{
		out.at(x)->changeValidate('n');
		return;
	}
	
	//cout << x << endl; //test output
	
	char* c[3];
	c[0] = (char*)out.at(x)->getInfo().c_str();
	c[1] = (char*)in.at(x).c_str();
	c[2] = NULL;
	
	pid_t pid = fork();
	
	if(pid < 0)
	{
		perror("fork()");
	}
	else if(pid == 0)
	{
		execvp(c[0], c);
		exit(0);
	}
	else
	{
		int status;
		
		if(wait(&status) == -1)
		{
			perror("wait()");
		}
		
		if(WIFEXITED(status))
		{
			if(WEXITSTATUS(status) == 0)
			{
				out.at(x)->changeValidate('y');
			}
			else
			{
				out.at(x)->changeValidate('n');
			}
		}
		else
		{
			perror("execvp error");
		}
		
	}
}


