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
#include <sys/stat.h>
#include <sys/stat.h> 
#include <fcntl.h>
using namespace std;

//Base Executable

Executable::Executable() {}
Executable::Executable(string info) 
{
    this->info = info;
}

string Executable::getInfo() { return this->info; }

char Executable::getValidate() { return this->validate; }
char Executable::get_ignore() { return this->ignore; }
char** Executable::get_cmnd() { return this->cmnd; }
unsigned int Executable::getVectorSize() { return this->output.size(); }

void Executable::changeValidate(char validate)
{
    this->validate = validate;
}
void Executable::changeIgnore(char ignore)
{
	this->ignore = ignore;
}
void Executable::changeType(char type)
{
	this->type = type;
}

void Executable::insert(Executable* entry, string object) 
{
    output.push_back(entry);
    input.push_back(object);
    
}

void Executable::print()
{
	cout << output.size() << endl;
	for(unsigned int i = 0; i < output.size(); ++i)
	{
		cout << output.at(i)->info << " ";
		cout << input.at(i) << " ";
	}
	
	cout << endl << "Test case successfully printed." << endl << endl;
}

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
	// 		perror("this");
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
		if(output.at(i)->getValidate() == 'y')
 		{
 			this->validate = 'y';
 		}
 		if(i == output.size() - 1 && this->validate != 'y' && this->validate != 'n')
	 	{
	 		this->validate = 'n';
	 	}
	 	
	 	//cout << output.at(i)->getValidate() << endl; //test
	}
	this->deleteVector();
	//cout << "Successful; exit command not called" << endl << endl;
}
void Executable::deleteVector()
{
	this->output.resize(0);
	this->input.resize(0);
}

void Executable::apply(unsigned int x, vector<Executable*> out, vector<string> in)
{
	//will only call reeolved sub-class functions
}

//Semicolon
Semicolon::Semicolon():Executable() {}
Semicolon::Semicolon(string info):Executable(info) {}

void Semicolon::apply(unsigned int x, vector<Executable*> out, vector<string> in)
{
	//cout << x << endl; //test output
	
	if(out.at(x)->getInfo() == "exit") { exit (EXIT_SUCCESS);}
	
	int status;
	//cout << status << " barb" << endl;
	
	if(out.at(x)->getInfo() == "test")
	{
		string var1;
		string var2;
		
		istringstream inSS(in.at(x));
		
		inSS >> var1;
		inSS >> var2;
		
		pid_t pid = fork();
		
		if(pid < 0 )
		{
			perror("fork()");
		}
		if(pid == 0)
		{
			char* arg = (char*) var2.c_str();
		
			struct stat sb;
			
			stat(arg, &sb);
			
			if(var1 == "-e")
			{
				if(S_ISREG(sb.st_mode) || S_ISDIR(sb.st_mode))
				{
					status = 0;
					cout << "(True)" << endl;
				}
				else
				{
					status = 1;
					cout << "(False)" << endl;
				}
			}
			else if(var1 == "-f")
			{
				if(S_ISREG(sb.st_mode))
				{
					status = 0;
					cout << "(True)" << endl;
				}
				else
				{
					status = 1;
					cout << "(False)" << endl;
				}
			}
			else if(var1 == "-d")
			{
				if(S_ISDIR(sb.st_mode))
				{
					status = 0;
					cout << "(True)" << endl;
				}
				else
				{
					status = 1;
					cout << "(False)" << endl;
				}
			}
			exit(status);
		}
		else
		{
			wait(&status);
			
			if(WIFEXITED(status))
			{
				
				if(WEXITSTATUS(status) == 0)
				{
					out.at(x)->changeValidate('y');
					this->validate = 'y';
					return;
					
				}
				else
				{
					out.at(x)->changeValidate('n');
					return;
				}
			}
			else
			{
				perror("test error");
			}
		}
		
	}
	
	else
	{
		char* c[3];
		c[0] = (char*)out.at(x)->getInfo().c_str();
		c[1] = (char*)in.at(x).c_str();
		c[2] = NULL;
		
		//cout << status << endl;
		
		pid_t pid = fork();
		
		if(pid < 0)
		{
			perror("fork()");
		}
		else if(pid == 0)
		{
			if(execvp(c[0], c))
			{
				status = 1;
			}
			else { status = 0; }
			//cout << status << endl;
			exit(status);
		}
		else
		{
			wait(&status);
			//if(wait(&status) == -1)
			//{
				//perror("wait()");
			//}
			
			if(WIFEXITED(status))
			{
				
				if(WEXITSTATUS(status) == 0)
				{
					//cout << in.at(x) << endl;
					out.at(x)->changeValidate('y');
					this->validate = 'y';
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
	
//	cout << out.at(x)->getValidate() << endl;
}

//Ampersand
Ampersand::Ampersand():Executable() {}

Ampersand::Ampersand(string info):Executable(info) {}

void Ampersand::apply(unsigned int x, vector<Executable*> out, vector<string> in)
{
	 //cout << x << endl; //test output
	
	if(out.at(x)->getInfo() == "exit") {exit (EXIT_SUCCESS);}
	
	if( out.at(x - 1)->getValidate() == 'n') 
	{
		out.at(x)->changeValidate('n');
		return;
	}
	
	//int hold_Size = out.at(x)->getInfo().size();
	int status;
	
	if(out.at(x)->getInfo() == "test")
	{
		string var1;
		string var2;
		
		istringstream inSS(in.at(x));
		
		inSS >> var1;
		inSS >> var2;
		
		pid_t pid = fork();
		
		if(pid < 0 )
		{
			perror("fork()");
		}
		if(pid == 0)
		{
			char* arg = (char*) var2.c_str();
		
			struct stat sb;
			
			stat(arg, &sb);
			
			if(var1 == "-e")
			{
				if(S_ISREG(sb.st_mode))
				{
					status = 0;
					cout << "(True)" << endl;
				}
				else if(S_ISDIR(sb.st_mode))
				{
					status = 0;
					cout << "(True)" << endl;
				}
				else
				{
					status = 1;
					cout << "(False)" << endl;
				}
			}
			else if(var1 == "-f")
			{
				if(S_ISREG(sb.st_mode))
				{
					status = 0;
					cout << "(True)" << endl;
				}
				else
				{
					status = 1;
					cout << "(False)" << endl;
				}
			}
			else if(var1 == "-d")
			{
				if(S_ISDIR(sb.st_mode))
				{
					status = 0;
					cout << "(True)" << endl;
				}
				else
				{
					status = 1;
					cout << "(False)" << endl;
				}
			}
			exit(status);
		}
		else
		{
			wait(&status);
			
			
			if(WIFEXITED(status))
			{
				if(WEXITSTATUS(status) == 0)
				{
					out.at(x)->changeValidate('y');
					this->validate = 'y';
				}
				else
				{
					out.at(x)->changeValidate('n');
				}
			}
			else
			{
				perror("test error");
			}
		}
	}
	
	else
	{
		char* c[3];
		c[0] = (char*)out.at(x)->getInfo().c_str();
		c[1] = (char*)in.at(x).c_str();
		c[2] = NULL;
		
		//cout << status << endl;
		
		pid_t pid = fork();
		
		if(pid < 0)
		{
			perror("fork()");
		}
		else if(pid == 0)
		{
			if(execvp(c[0], c))
			{
				status = 1;
			}
			else { status = 0; }
			//cout << status << endl;
			exit(status);
		}
		else
		{
			wait(&status);
			//if(wait(&status) == -1)
			//{
				//perror("wait()");
			//}
			
			if(WIFEXITED(status))
			{
				
				if(WEXITSTATUS(status) == 0)
				{
					//cout << in.at(x) << endl;
					out.at(x)->changeValidate('y');
					this->validate = 'y';
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
}

//vertical
Vertical::Vertical():Executable() {}

Vertical::Vertical(string info):Executable(info) {}

void Vertical::apply(unsigned int x, vector<Executable*> out, vector<string> in) 
{
	if( out.at(x - 1)->getValidate() == 'y') 
	{
		out.at(x)->changeValidate('n');
		return;
	}
	
	//cout << x << endl; //test output
	int status;
	
	if(out.at(x)->getInfo() == "test")
	{
		string var1;
		string var2;
		
		istringstream inSS(in.at(x));
		
		inSS >> var1;
		inSS >> var2;
		
		pid_t pid = fork();
		
		if(pid < 0 )
		{
			perror("fork()");
		}
		if(pid == 0)
		{
			char* arg = (char*) var2.c_str();
		
			struct stat sb;
			
			stat(arg, &sb);
			
			if(var1 == "-e")
			{
				if(S_ISREG(sb.st_mode) || S_ISDIR(sb.st_mode))
				{
					status = 0;
					cout << "(True)" << endl;
				}
				else
				{
					status = 1;
					cout << "(False)" << endl;
				}
			}
			else if(var1 == "-f")
			{
				if(S_ISREG(sb.st_mode))
				{
					status = 0;
					cout << "(True)" << endl;
				}
				else
				{
					status = 1;
					cout << "(False)" << endl;
				}
			}
			else if(var1 == "-d")
			{
				if(S_ISDIR(sb.st_mode))
				{
					status = 0;
					cout << "(True)" << endl;
				}
				else
				{
					status = 1;
					cout << "(False)" << endl;
				}
			}
			exit(status);
		}
		else
		{
			wait(&status);
			
			if(WIFEXITED(status))
			{
				if(WEXITSTATUS(status) == 0)
				{
					out.at(x)->changeValidate('y');
					this->validate = 'y';
				}
				else
				{
					out.at(x)->changeValidate('n');
				}
			}
			else
			{
				perror("test error");
			}
		}
	}
	
	else
	{
		char* c[3];
		c[0] = (char*)out.at(x)->getInfo().c_str();
		c[1] = (char*)in.at(x).c_str();
		c[2] = NULL;
		
		//cout << status << endl;
		
		pid_t pid = fork();
		
		if(pid < 0)
		{
			perror("fork()");
		}
		else if(pid == 0)
		{
			if(execvp(c[0], c))
			{
				status = 1;
			}
			else { status = 0; }
			//cout << status << endl;
			exit(status);
		}
		else
		{
			wait(&status);
			//if(wait(&status) == -1)
			//{
				//perror("wait()");
			//}
			
			if(WIFEXITED(status))
			{
				
				if(WEXITSTATUS(status) == 0)
				{
					//cout << in.at(x) << endl;
					out.at(x)->changeValidate('y');
					this->validate = 'y';
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
}


ParVertical::ParVertical():Executable() {}

void ParVertical::parse(string arg_hold, string& arg3, istringstream& inSS, istringstream& inSS2, istringstream& inSS3)
{
	string arg;
	string arg2;
	string object;
	string prev = ";";
	
		arg_hold.erase(0,1);
		arg3.erase(0,1);
		
		if (arg_hold.find('#') != string::npos)  //searching for hashtags
	    {
		    //unsigned sz = object.size(); //size of parsed string
		    unsigned index = object.find('#'); //location of searched item
		    arg.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
		    if(arg.size() == 0) {return;}
		    
		    Executable* entry = new Semicolon(arg);
		    this->insert(entry, NULL);
		    return;
	    }
		if(arg_hold.size() == 0)
		{
			
		}
		
		else if(arg_hold.find("(") == 0)
    	{
	    		Executable* par_entry = new ParSemicolon();
	    		par_entry->parse(arg_hold, arg3, inSS, inSS2, inSS3);
	    		this->insert(par_entry, "null");
	    		
	    		if (arg3.find('#') != string::npos)  //searching for hashtags
			    {
				   return;
			    }
	    		
	    		if(arg3.at(arg3.size() - 1) == ')' && arg3.at(arg3.size() - 2) == ')')
	    		{
	    			arg3.resize(arg3.size() - 1);
	    			return;
	    		}
	    		
	    		if(inSS.eof()) { return; }
	    		
    	}
    	
    	else
    	{
			Executable* entry_hold = new Semicolon(arg_hold);
    	
		
			inSS >> object;
	        inSS2 >> arg2;
	        inSS3 >> arg3;
	        
	        if(arg == "test") //parsing test
	        {
	        	string multi, multi_2;
	        	
	        	if(object == "-e" || object == "-f" || object == "-d")
	        	{
	        		inSS >> multi;
	        		inSS2 >> multi_2;
	        		inSS3 >> arg3;
	        		object = object + " " + multi;
	        	}
	        	else //if no flag included, default to -e
	        	{
	        		multi = object;
	        		object = "-e";
	        		object = object + " " + multi;
	        	}
	        	
	        }
	        
	        else if(arg == "[") //check for symbolic type test
	        {
	        	arg = "test";
	        	
	        	string multi, multi_2;
	        	
	        	if(object == "-e" || object == "-f" || object == "-d")
	        	{
	        		inSS >> multi;
	        		inSS2 >> multi_2;
	        		inSS3 >> arg3;
	        		
	        		object = object + " " + multi;
	        	}
	        	else //if no flag included, default to -e
	        	{
	        		multi = object;
	        		object = "-e";
	        		object = object + " " + multi;
	        	}
	        	
	        	inSS2 >> arg2;
	        	inSS3 >> arg3;
	        	if(arg2 == "]") //check for closing condition
	        	inSS >> arg2;
	        	
	        }
	        
	        if(object.find("\"") == 0) //for multiple command arguements
	        {
	        	object.erase(0,1);
	        	arg2.erase(0,1);
	        	arg3.erase(0,1);
	        	
	        	string multi; //to hold multiple strings
	        	string multi_2;
	        	while(!inSS.eof())
	        	{
	        		inSS >> multi;
	        		inSS2 >> multi_2;
	        		inSS3 >> arg3;
	        		
	        		object = object + " " + multi;
	        		
	        		if(object.find("\"") == object.size() - 1)
	        		{
	        			object.resize(object.size() - 1);
	        			//cout << object << endl; //test output
	        			return;
	        		}
	        		
	        	}
	        }
	    	
	    	if (object.find('#') != string::npos)  //searching for hashtags
		    {
			    //unsigned sz = object.size(); //size of parsed string
			    unsigned index = object.find('#'); //location of searched item
			    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
			    if(object.size() == 0) {return;}
			    
			    this->insert(entry_hold, object);
			    return;
		    }
		    
		    prev = object;
		     
	        if ( (object.find(';') != string::npos) && (object.find(';') == object.size() - 1) ) 
			{
				//unsigned sz = object.size(); //size of parsed string
			    unsigned index = object.find(';'); //location of searched item
			    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
			    
			}
			
			
			if(object.find(")") != string::npos && object.at(object.size() - 1) == ';')
			{
				object.erase(object.find(")"), object.size() - object.find(")"));
				this->insert(entry_hold, object);
				return;
			}
			
			else if(object.find(")") != string::npos && object.at(object.size() - 1) == ')')
			{
				object.erase(object.find(")"), object.size() - object.find(")"));
				this->insert(entry_hold, object);
				return;
			}
			else if(object.find(")") != string::npos && object.at(object.size() - 1) != ')')
			{
				cout << "error: foreign item detected after parenthesis" << endl;
				this->insert(entry_hold, "null");
				return;
			}
			
			 this->insert(entry_hold, object);
			 
	  	    if(inSS.eof()) 
	    	{ 
	    		this->insert(entry_hold, object);
	    		return; 
	    	}
    	}
		
		while(!inSS.eof() )
		{
			inSS >> arg;
			inSS2 >> arg2;
			inSS3 >> arg3;
			
			if (arg.find('#') != string::npos)  //searching for hashtags
		    {
			    //unsigned sz = object.size(); //size of parsed string
			    unsigned index = object.find('#'); //location of searched item
			    arg.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
			    if(arg.size() == 0) {return;}
			    
			    Executable* entry = new Semicolon(arg);
			    this->insert(entry, NULL);
			    return;
		    }
			
			if (arg.find("&&") == 0) //if a double Ampersand is found countains no other characters
			{
			    
			    if(!inSS.eof() ) //making sure there is a command after the ampersands
			    {
					inSS >> arg;
					inSS2 >> arg2;
			        inSS3 >> arg3;
			        
			        if (arg.find('#') != string::npos)  //searching for hashtags
				    {
					    //unsigned sz = object.size(); //size of parsed string
					    unsigned index = object.find('#'); //location of searched item
					    arg.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
					    if(arg.size() == 0) {return;}
					    
					    Executable* entry = new Semicolon(arg);
					    this->insert(entry, NULL);
					    return;
				    }
			        
			    	if(arg.find("(") == 0)
			    	{
				    		Executable* par_entry = new ParAmpersand();
				    		par_entry->parse(arg, arg3, inSS, inSS2, inSS3);
				    		this->insert(par_entry, "null");
				    		
				    		if(arg3.at(arg3.size() - 1) == ')' && arg3.at(arg3.size() - 2) == ')')
				    		{
				    			arg3.resize(arg3.size() - 1);
				    			return;
				    		}
				    		
				    		if(inSS.eof()) { return; }
			    	}
			        
			        else
			        {
				        Executable* entry = new Ampersand(arg);
				        if(inSS.eof()) 
				        {
				        	this->insert(entry, "null");
				        	return;
				        }
				        	
				        inSS >> object;
				        inSS2 >> arg2;
				        inSS3 >> arg3;
				        
				        
				        if(arg == "test") //parsing test
				        {
				        	string multi, multi_2;
				        	
				        	if(object == "-e" || object == "-f" || object == "-d")
				        	{
				        		inSS >> multi;
				        		inSS2 >> multi_2;
				        		inSS3 >> arg3;
				        		
				        		object = object + " " + multi;
				        	}
				        	else //if no flag included, default to -e
				        	{
				        		multi = object;
				        		object = "-e";
				        		object = object + " " + multi;
				        	}
				        	
				        }
				        
				        else if(arg == "[") //check for symbolic type test
				        {
				        	arg = "test";
				        	
				        	string multi, multi_2;
				        	
				        	if(object == "-e" || object == "-f" || object == "-d")
				        	{
				        		inSS >> multi;
				        		inSS2 >> multi_2;
				        		inSS3 >> arg3;
				        		
				        		object = object + " " + multi;
				        	}
				        	else //if no flag included, default to -e
				        	{
				        		multi = object;
				        		object = "-e";
				        		object = object + " " + multi;
				        	}
				        	
				        	inSS2 >> arg2;
				        	inSS3 >> arg3;
				        	if(arg2 == "]") //check for closing condition
				        	inSS >> arg2;
				        	
				        }
				        
				        if(object.find("\"") == 0) //for multiple command arguements
				        {
				        	object.erase(0,1);
				        	arg2.erase(0,1);
				        	arg3.erase(0,1);
				        	
				        	string multi; //to hold multiple strings
				        	string multi_2;
				        	while(!inSS.eof())
				        	{
				        		inSS >> multi;
				        		inSS2 >> multi_2;
				        		inSS3 >> arg3;
				        		
				        		object = object + " " + multi;
				        		
				        		if(object.find("\"") == object.size() - 1 || object.find("\"") == object.size() - 2)
				        		{
				        			object.resize(object.size() - 2);
				        			//cout << object << endl; //test output
				        			return;
				        		}
				        		
				        	}
				        	
				        	if(inSS.eof()) 
				        	{ 
				        		this->insert(entry, object);
				        		return; 
				        	}
				        }
				        
				        //cout << object << endl; //test output
				        
				        if (object.find('#') != string::npos)  //searching for hashtags
					    {
						    //unsigned sz = object.size(); //size of parsed string
						    unsigned index = object.find('#'); //location of searched item
						    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
						    if(object.size() == 0) {return;}
						    
						    this->insert(entry, object);
						    return;
					    }
					    
					    prev = object;
					     
				        if ( (object.find(';') != string::npos) && (object.find(';') == object.size() - 1) ) 
						{
							//unsigned sz = object.size(); //size of parsed string
						    unsigned index = object.find(';'); //location of searched item
						    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
						    
						}
						
						if(object.find(")") != string::npos && object.at(object.size() - 1) == ';')
						{
							object.erase(object.find(")"), object.size() - object.find(")"));
							this->insert(entry, object);
							return;
						}
						
						else if(object.find(")") != string::npos && object.at(object.size() - 1) == ')')
						{
							object.erase(object.find(")"), object.size() - object.find(")"));
							this->insert(entry, object);
							return;
						}
						else if(object.find(")") != string::npos && object.at(object.size() - 1) != ')')
						{
							cout << "error: foreign item detected after parenthesis" << endl;
							this->insert(entry, "null");
							return;
						}
					
						this->insert(entry, object);
			        }
				}
	
			 }
	
			
			else if (arg.find("||") == 0) //if a double vertical is found countains no other characters
			{
			    if(!inSS.eof() ) //making sure there is a command after the ampersands
			    {
					inSS >> arg;
					inSS2 >> arg2;
			        inSS3 >> arg3;
			        
			        if (arg.find('#') != string::npos)  //searching for hashtags
				    {
					    //unsigned sz = object.size(); //size of parsed string
					    unsigned index = object.find('#'); //location of searched item
					    arg.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
					    if(arg.size() == 0) {return;}
					    
					    Executable* entry = new Semicolon(arg);
					    this->insert(entry, NULL);
					    return;
				    }
			        
			        if (arg.find('#') != string::npos)  //searching for hashtags
				    {
					    //unsigned sz = object.size(); //size of parsed string
					    unsigned index = object.find('#'); //location of searched item
					    arg.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
					    if(arg.size() == 0) {return;}
					    
					    Executable* entry = new Semicolon(arg);
					    this->insert(entry, NULL);
					    return;
				    }
			        
			    	if(arg.find("(") == 0)
			    	{
			    		Executable* par_entry = new ParVertical();
			    		par_entry->parse(arg, arg3, inSS, inSS2, inSS3);
			    		this->insert(par_entry, "null");
			    		
			    		if(arg3.at(arg3.size() - 1) == ')' && arg3.at(arg3.size() - 2) == ')')
			    		{
			    			arg3.resize(arg3.size() - 1);
			    			return;
			    		}
			    		
			    		if(inSS.eof()) { return; }
			    	}
			        
			        else
			        {
				        Executable* entry = new Vertical(arg);
				        if(inSS.eof()) 
				        {
				        	this->insert(entry, "null");
				        	return;
				        }
				        	
				        inSS >> object;
				        inSS2 >> arg2;
				        inSS3 >> arg3;
				        
				        if(arg == "test") //parsing test
				        {
				        	string multi, multi_2;
				        	
				        	if(object == "-e" || object == "-f" || object == "-d")
				        	{
				        		inSS >> multi;
				        		inSS2 >> multi_2;
				        		inSS3 >> arg3;
				        		
				        		object = object + " " + multi;
				        	}
				        	else //if no flag included, default to -e
				        	{
				        		multi = object;
				        		object = "-e";
				        		object = object + " " + multi;
				        	}
				        	
				        }
				        
				        else if(arg == "[") //check for symbolic type test
				        {
				        	arg = "test";
				        	
				        	string multi, multi_2;
				        	
				        	if(object == "-e" || object == "-f" || object == "-d")
				        	{
				        		inSS >> multi;
				        		inSS2 >> multi_2;
				        		inSS3 >> arg3;
				        		
				        		object = object + " " + multi;
				        	}
				        	else //if no flag included, default to -e
				        	{
				        		multi = object;
				        		object = "-e";
				        		object = object + " " + multi;
				        	}
				        	
				        	inSS2 >> arg2;
				        	inSS3 >> arg3;
				        	if(arg2 == "]") //check for closing condition
				        	inSS >> arg2;
				        	
				        }
				        
				        if(object.find("\"") == 0) //for multiple command arguements
				        {
				        	object.erase(0,1);
				        	arg2.erase(0,1);
				        	arg3.erase(0,1);
				        	
				        	string multi; //to hold multiple strings
				        	string multi_2;
				        	while(!inSS.eof())
				        	{
				        		inSS >> multi;
				        		inSS2 >> multi_2;
				        		inSS3 >> arg3;
				        		
				        		object = object + " " + multi;
				        		
				        		if(object.find("\"") == object.size() - 1 || object.find("\"") == object.size() - 2)
				        		{
				        			object.resize(object.size() - 2);
				        			//cout << object << endl; //test output
				        			return;
				        		}
				        		
				        	}
				        	
				        	if(inSS.eof()) 
				        	{ 
				        		this->insert(entry, object);
				        		return; 
				        	}
				        }
				        
				        //cout << object << endl; //test output
				        
				        if (object.find('#') != string::npos)  //searching for hashtags
					    {
						    //unsigned sz = object.size(); //size of parsed string
						    unsigned index = object.find('#'); //location of searched item
						    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
						    if(object.size() == 0) {return;}
						    
						    this->insert(entry, object);
						    return;
					    }
					    
					    prev = object;
					     
				        if ( (object.find(';') != string::npos) && (object.find(';') == object.size() - 1) ) 
						{
							//unsigned sz = object.size(); //size of parsed string
						    unsigned index = object.find(';'); //location of searched item
						    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
						    
						}
						
						if(object.find(")") != string::npos && object.at(object.size() - 1) == ';')
						{
							object.erase(object.find(")"), object.size() - object.find(")"));
							this->insert(entry, object);
							return;
						}
						
						else if(object.find(")") != string::npos && object.at(object.size() - 1) == ')')
						{
							object.erase(object.find(")"), object.size() - object.find(")"));
							//cout << object << "//test" << endl;
							this->insert(entry, object);
							return;
						}
						else if(object.find(")") != string::npos && object.at(object.size() - 1) != ')')
						{
							cout << "error: foreign item detected after parenthesis" << endl;
							this->insert(entry, "null");
							return;
						}
					
						this->insert(entry, object);
			        }
				}		
		    }
		    
		    else if (prev.at(prev.size() - 1) == ';')
		    {
		    	if(arg.find(")") == arg.size() - 1)
				{
					return;
				}
				
				Executable* entry = new Semicolon(arg);
				
				if(object.find(")") != string::npos && object.at(object.size() - 1) == ';')
				{
					object.erase(object.find(")"), object.size() - object.find(")"));
					this->insert(entry, object);
					return;
				}
				
				else if(object.find(")") != string::npos && object.at(object.size() - 1) == ')')
				{
					object.erase(object.find(")"), object.size() - object.find(")"));
					this->insert(entry, object);
					return;
				}
				else if(object.find(")") != string::npos && object.at(object.size() - 1) != ')')
				{
					cout << "error: foreign item detected after parenthesis" << endl;
					this->insert(entry, "null");
					return;
				}
		    	
		    	
		    	
		    	if(inSS.eof()) 
		    	{
			        	this->insert(entry, "null");
			        	return;
			    }

		    
		    
		        if(arg.find("(") == 0)
		    	{
		    		Executable* par_entry = new ParSemicolon();
		    		par_entry->parse(arg, arg3, inSS, inSS2, inSS3);
		    		this->insert(par_entry, "null");
		    		
		    		if(arg3.at(arg3.size() - 1) == ')' && arg3.at(arg3.size() - 2) == ')')
		    		{
		    			arg3.resize(arg3.size() - 1);
		    			return;
		    		}
		    		
		    		if(inSS.eof()) { return; }
		    	}
		    	
		    	
		    	else
		    	{
		    		inSS >> object;
		        	inSS2 >> arg2;
		    		inSS3 >> arg3;
		    		
		    		if(arg == "test") //parsing test
			        {
			        	string multi, multi_2;
			        	
			        	if(object == "-e" || object == "-f" || object == "-d")
			        	{
			        		inSS >> multi;
			        		inSS2 >> multi_2;
			        		inSS3 >> arg3;
			        		
			        		object = object + " " + multi;
			        	}
			        	else //if no flag included, default to -e
			        	{
			        		multi = object;
			        		object = "-e";
			        		object = object + " " + multi;
			        	}
			        	
			        }
			        
			        else if(arg == "[") //check for symbolic type test
			        {
			        	arg = "test";
			        	
			        	string multi, multi_2;
			        	
			        	if(object == "-e" || object == "-f" || object == "-d")
			        	{
			        		inSS >> multi;
			        		inSS2 >> multi_2;
			        		inSS3 >> arg3;
			        		
			        		object = object + " " + multi;
			        	}
			        	else //if no flag included, default to -e
			        	{
			        		multi = object;
			        		object = "-e";
			        		object = object + " " + multi;
			        	}
			        	
			        	inSS2 >> arg2;
			        	inSS3 >> arg3;
			        	if(arg2 == "]") //check for closing condition
			        	inSS >> arg2;
			        	
			        }
			        if(object.find("\"") == 0) //for multiple command arguements
			        {
			        	object.erase(0,1);
			        	arg2.erase(0,1);
			        	arg3.erase(0,1);
			        	
			        	string multi; //to hold multiple strings
			        	string multi_2;
			        	while(!inSS.eof())
			        	{
			        		inSS >> multi;
			        		inSS2 >> multi_2;
			        		inSS3 >> arg3;
			        		
			        		object = object + " " + multi;
			        		
			        		if(object.find("\"") == object.size() - 1)
			        		{
			        			object.resize(object.size() - 1);
			        			//cout << object << endl; //test output
			        			return;
			        		}
			        		
			        	}
			        	
			        	if(inSS.eof()) 
			        	{ 
			        		this->insert(entry, object);
			        		return; 
			        	}
			        }
			        
			        if (object.find('#') != string::npos) 
				    {
					    //unsigned sz = object.size(); //size of parsed string
					    unsigned index = object.find('#'); //location of searched item
					    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
					    if(object.size() == 0) {return;}
					    
					    this->insert(entry, object);
					    return;
				    }
				    
				    prev = object;
				    
			        if ( (object.find(';') != string::npos) && (object.find(';') == object.size() - 1) ) 
					{
						//unsigned sz = object.size(); //size of parsed string
					    unsigned index = object.find(';'); //location of searched item
					    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
					    
					}
				
					this->insert(entry, object);
			    	
			    }
			}
			
			else
			{
				cout << "error " << endl;
				exit(0);
			}
		}
}
void ParVertical::apply(unsigned int x, vector<Executable*> out, vector<string> in)
{
 	// cout << out.size() << endl;
 	// cout << out.at(x)->getVectorSize() << endl;
 	 //cout << out.at(x - 1)->getValidate() << endl;
 	if(x > 0 && (out.at(x - 1)->getValidate() == 'y') )
 	{
 		out.at(x)->changeValidate('n');
 		if(x == out.size() - 1 && this->validate != 'y' && this->validate != 'n')
	 	{
	 		this->validate = 'n';
	 	}
 		return;
 	}
 	if(out.at(x)->getVectorSize() >= 1)
 	{
 		out.at(x)->execute();
 		//if(out.at(x)->getValidate() == 'y')
 		// {
 		// 	this->validate = 'y';
 		// }
 		// if(x == out.size() - 1 && this->validate != 'y' && this->validate != 'n')
	 	// {
	 	// 	this->validate = 'n';
	 	// }
 	}
 	
}

//Parentitical Version of Ampersand condition
ParAmpersand::ParAmpersand():Executable() {}

void ParAmpersand::parse(string arg_hold, string& arg3, istringstream& inSS, istringstream& inSS2, istringstream& inSS3)
{
	string arg;
	string arg2;
	string object;
	string prev = ";";
	
		//cout << arg_hold << endl;
		arg_hold.erase(0,1);
		arg3.erase(0,1);
		
		if (arg_hold.find('#') != string::npos)  //searching for hashtags
	    {
		    //unsigned sz = object.size(); //size of parsed string
		    unsigned index = object.find('#'); //location of searched item
		    arg.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
		    if(arg.size() == 0) {return;}
		    
		    Executable* entry = new Semicolon(arg);
		    this->insert(entry, NULL);
		    return;
	    }
	    
		if(arg_hold.size() == 0)
		{
			
		}
		
		else if(arg_hold.find("(") == 0)
    	{
	    		Executable* par_entry = new ParSemicolon();
	    		par_entry->parse(arg_hold, arg3, inSS, inSS2, inSS3);
	    		this->insert(par_entry, "null");
	    		
	    		if (arg3.find('#') != string::npos)  //searching for hashtags
			    {
				    //unsigned sz = object.size(); //size of parsed string
				    
				    return;
			    }
	    		
	    		if(arg3.at(arg3.size() - 1) == ')' && arg3.at(arg3.size() - 2) == ')')
	    		{
	    			arg3.resize(arg3.size() - 1);
	    			return;
	    		}
	    		
	    		if(inSS.eof()) { return; }
	    		
    	}
    	
    	else
    	{
			Executable* entry_hold = new Semicolon(arg_hold);
    	
		
			inSS >> object;
	        inSS2 >> arg2;
	        inSS3 >> arg3;
	        
	        if(arg == "test") //parsing test
	        {
	        	string multi, multi_2;
	        	
	        	if(object == "-e" || object == "-f" || object == "-d")
	        	{
	        		inSS >> multi;
	        		inSS2 >> multi_2;
	        		inSS3 >> arg3;
	        		object = object + " " + multi;
	        	}
	        	else //if no flag included, default to -e
	        	{
	        		multi = object;
	        		object = "-e";
	        		object = object + " " + multi;
	        	}
	        	
	        }
	        
	        else if(arg == "[") //check for symbolic type test
	        {
	        	arg = "test";
	        	
	        	string multi, multi_2;
	        	
	        	if(object == "-e" || object == "-f" || object == "-d")
	        	{
	        		inSS >> multi;
	        		inSS2 >> multi_2;
	        		inSS3 >> arg3;
	        		
	        		object = object + " " + multi;
	        	}
	        	else //if no flag included, default to -e
	        	{
	        		multi = object;
	        		object = "-e";
	        		object = object + " " + multi;
	        	}
	        	
	        	inSS2 >> arg2;
	        	inSS3 >> arg3;
	        	if(arg2 == "]") //check for closing condition
	        	inSS >> arg2;
	        	
	        }
	        
	        if(object.find("\"") == 0) //for multiple command arguements
	        {
	        	object.erase(0,1);
	        	arg2.erase(0,1);
	        	arg3.erase(0,1);
	        	
	        	string multi; //to hold multiple strings
	        	string multi_2;
	        	while(!inSS.eof())
	        	{
	        		inSS >> multi;
	        		inSS2 >> multi_2;
	        		inSS3 >> arg3;
	        		
	        		object = object + " " + multi;
	        		
	        		if(object.find("\"") == object.size() - 1)
	        		{
	        			object.resize(object.size() - 1);
	        			//cout << object << endl; //test output
	        			return;
	        		}
	        		
	        	}
	        }
	    	
	    	if (object.find('#') != string::npos)  //searching for hashtags
		    {
			    //unsigned sz = object.size(); //size of parsed string
			    unsigned index = object.find('#'); //location of searched item
			    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
			    if(object.size() == 0) {return;}
			    
			    this->insert(entry_hold, object);
			    return;
		    }
		    
		    prev = object;
		     
	        if ( (object.find(';') != string::npos) && (object.find(';') == object.size() - 1) ) 
			{
				//unsigned sz = object.size(); //size of parsed string
			    unsigned index = object.find(';'); //location of searched item
			    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
			    
			}
			
			if(object.find(")") != string::npos && object.at(object.size() - 1) == ';')
			{
				object.erase(object.find(")"), object.size() - object.find(")"));
				this->insert(entry_hold, object);
				return;
			}
			
			else if(object.find(")") != string::npos && object.at(object.size() - 1) == ')')
			{
				object.erase(object.find(")"), object.size() - object.find(")"));
				this->insert(entry_hold, object);
				return;
			}
			else if(object.find(")") != string::npos && object.at(object.size() - 1) != ')')
			{
				cout << "error: foreign item detected after parenthesis" << endl;
				this->insert(entry_hold, "null");
				return;
			}
			
			 this->insert(entry_hold, object);
			 
	  	    if(inSS.eof()) 
	    	{ 
	    		this->insert(entry_hold, object);
	    		return; 
	    	}
    	}
		
		while(!inSS.eof() )
		{
			inSS >> arg;
			inSS2 >> arg2;
			inSS3 >> arg3;
			
			if (arg.find('#') != string::npos)  //searching for hashtags
		    {
			    //unsigned sz = object.size(); //size of parsed string
			    unsigned index = object.find('#'); //location of searched item
			    arg.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
			    if(arg.size() == 0) {return;}
			    
			    Executable* entry = new Semicolon(arg);
			    this->insert(entry, NULL);
			    return;
		    }
		    
			if (arg.find("&&") == 0) //if a double Ampersand is found countains no other characters
			{
			    
			    if(!inSS.eof() ) //making sure there is a command after the ampersands
			    {
					inSS >> arg;
					inSS2 >> arg2;
			        inSS3 >> arg3;
			        
			        if (arg.find('#') != string::npos)  //searching for hashtags
				    {
					    //unsigned sz = object.size(); //size of parsed string
					    unsigned index = object.find('#'); //location of searched item
					    arg.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
					    if(arg.size() == 0) {return;}
					    
					    Executable* entry = new Semicolon(arg);
					    this->insert(entry, NULL);
					    return;
				    }
			        
			    	if(arg.find("(") == 0)
			    	{
			    		Executable* par_entry = new ParAmpersand();
			    		par_entry->parse(arg, arg3, inSS, inSS2, inSS3);
			    		this->insert(par_entry, "null");
			    		
			    		if(arg3.at(arg3.size() - 1) == ')' && arg3.at(arg3.size() - 2) == ')')
			    		{
			    			arg3.resize(arg3.size() - 1);
			    			return;
			    		}
			    		
			    		if(inSS.eof()) { return; }
			    	}
			        
			        else
			        {
				        Executable* entry = new Ampersand(arg);
				        if(inSS.eof()) 
				        {
				        	this->insert(entry, "null");
				        	return;
				        }
				        	
				        inSS >> object;
				        inSS2 >> arg2;
				        inSS3 >> arg3;
				        
				        if(arg == "test") //parsing test
				        {
				        	string multi, multi_2;
				        	
				        	if(object == "-e" || object == "-f" || object == "-d")
				        	{
				        		inSS >> multi;
				        		inSS2 >> multi_2;
				        		inSS3 >> arg3;
				        		
				        		object = object + " " + multi;
				        	}
				        	else //if no flag included, default to -e
				        	{
				        		multi = object;
				        		object = "-e";
				        		object = object + " " + multi;
				        	}
				        	
				        }
				        
				        else if(arg == "[") //check for symbolic type test
				        {
				        	arg = "test";
				        	
				        	string multi, multi_2;
				        	
				        	if(object == "-e" || object == "-f" || object == "-d")
				        	{
				        		inSS >> multi;
				        		inSS2 >> multi_2;
				        		inSS3 >> arg3;
				        		
				        		object = object + " " + multi;
				        	}
				        	else //if no flag included, default to -e
				        	{
				        		multi = object;
				        		object = "-e";
				        		object = object + " " + multi;
				        	}
				        	
				        	inSS2 >> arg2;
				        	inSS3 >> arg3;
				        	if(arg2 == "]") //check for closing condition
				        	inSS >> arg2;
				        	
				        }
				        
				        if(object.find("\"") == 0) //for multiple command arguements
				        {
				        	object.erase(0,1);
				        	arg2.erase(0,1);
				        	arg3.erase(0,1);
				        	
				        	string multi; //to hold multiple strings
				        	string multi_2;
				        	while(!inSS.eof())
				        	{
				        		inSS >> multi;
				        		inSS2 >> multi_2;
				        		inSS3 >> arg3;
				        		
				        		object = object + " " + multi;
				        		
				        		if(object.find("\"") == object.size() - 1 || object.find("\"") == object.size() - 2)
				        		{
				        			object.resize(object.size() - 2);
				        			//cout << object << endl; //test output
				        			return;
				        		}
				        		
				        	}
				        	
				        	if(inSS.eof()) 
				        	{ 
				        		this->insert(entry, object);
				        		return; 
				        	}
				        }
				        
				        //cout << object << endl; //test output
				        
				        if (object.find('#') != string::npos)  //searching for hashtags
					    {
						    //unsigned sz = object.size(); //size of parsed string
						    unsigned index = object.find('#'); //location of searched item
						    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
						    if(object.size() == 0) {return;}
						    
						    this->insert(entry, object);
						    return;
					    }
					     
					    prev = object; 
					     
				        if ( (object.find(';') != string::npos) && (object.find(';') == object.size() - 1) ) 
						{
							//unsigned sz = object.size(); //size of parsed string
						    unsigned index = object.find(';'); //location of searched item
						    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
						    
						}
						
						if(object.find(")") != string::npos && object.at(object.size() - 1) == ';')
						{
							object.erase(object.find(")"), object.size() - object.find(")"));
							this->insert(entry, object);
							return;
						}
						
						else if(object.find(")") != string::npos && object.at(object.size() - 1) == ')')
						{
							object.erase(object.find(")"), object.size() - object.find(")"));
							this->insert(entry, object);
							return;
						}
						else if(object.find(")") != string::npos && object.at(object.size() - 1) != ')')
						{
							cout << "error: foreign item detected after parenthesis" << endl;
							this->insert(entry, "null");
							return;
						}
					
						this->insert(entry, object);
						//this->print();
			        }
				}
	
			 }
	
			
			else if (arg.find("||") == 0) //if a double vertical is found countains no other characters
			{
			    if(!inSS.eof() ) //making sure there is a command after the ampersands
			    {
					inSS >> arg;
					inSS2 >> arg2;
			        inSS3 >> arg3;
			        
			        if (arg.find('#') != string::npos)  //searching for hashtags
				    {
					    //unsigned sz = object.size(); //size of parsed string
					    unsigned index = object.find('#'); //location of searched item
					    arg.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
					    if(arg.size() == 0) {return;}
					    
					    Executable* entry = new Semicolon(arg);
					    this->insert(entry, NULL);
					    return;
				    }
			        
			    	if(arg.find("(") == 0)
			    	{
			    		Executable* par_entry = new ParVertical();
			    		par_entry->parse(arg, arg3, inSS, inSS2, inSS3);
			    		this->insert(par_entry, "null");
			    		
			    		if(arg3.at(arg3.size() - 1) == ')' && arg3.at(arg3.size() - 2) == ')')
			    		{
			    			arg3.resize(arg3.size() - 1);
			    			return;
			    		}
			    		
			    		if(inSS.eof()) { return; }
			    	}
			        
			        else
			        {
				        Executable* entry = new Vertical(arg);
				        if(inSS.eof()) 
				        {
				        	this->insert(entry, "null");
				        	return;
				        }
				        	
				        inSS >> object;
				        inSS2 >> arg2;
				        inSS3 >> arg3;
				        
				        if(arg == "test") //parsing test
				        {
				        	string multi, multi_2;
				        	
				        	if(object == "-e" || object == "-f" || object == "-d")
				        	{
				        		inSS >> multi;
				        		inSS2 >> multi_2;
				        		inSS3 >> arg3;
				        		
				        		object = object + " " + multi;
				        	}
				        	else //if no flag included, default to -e
				        	{
				        		multi = object;
				        		object = "-e";
				        		object = object + " " + multi;
				        	}
				        	
				        }
				        
				        else if(arg == "[") //check for symbolic type test
				        {
				        	arg = "test";
				        	
				        	string multi, multi_2;
				        	
				        	if(object == "-e" || object == "-f" || object == "-d")
				        	{
				        		inSS >> multi;
				        		inSS2 >> multi_2;
				        		inSS3 >> arg3;
				        		
				        		object = object + " " + multi;
				        	}
				        	else //if no flag included, default to -e
				        	{
				        		multi = object;
				        		object = "-e";
				        		object = object + " " + multi;
				        	}
				        	
				        	inSS2 >> arg2;
				        	inSS3 >> arg3;
				        	if(arg2 == "]") //check for closing condition
				        	inSS >> arg2;
				        	
				        }
				        
				        if(object.find("\"") == 0) //for multiple command arguements
				        {
				        	object.erase(0,1);
				        	arg2.erase(0,1);
				        	arg3.erase(0,1);
				        	
				        	string multi; //to hold multiple strings
				        	string multi_2;
				        	while(!inSS.eof())
				        	{
				        		inSS >> multi;
				        		inSS2 >> multi_2;
				        		inSS3 >> arg3;
				        		
				        		object = object + " " + multi;
				        		
				        		if(object.find("\"") == object.size() - 1 || object.find("\"") == object.size() - 2)
				        		{
				        			object.resize(object.size() - 2);
				        			//cout << object << endl; //test output
				        			return;
				        		}
				        		
				        	}
				        	
				        	if(inSS.eof()) 
				        	{ 
				        		this->insert(entry, object);
				        		return; 
				        	}
				        }
				        
				        //cout << object << endl; //test output
				        
				        if (object.find('#') != string::npos)  //searching for hashtags
					    {
						    //unsigned sz = object.size(); //size of parsed string
						    unsigned index = object.find('#'); //location of searched item
						    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
						    if(object.size() == 0) {return;}
						    
						    this->insert(entry, object);
						    return;
					    }
					    
					    prev = object;
					     
				        if ( (object.find(';') != string::npos) && (object.find(';') == object.size() - 1) ) 
						{
							//unsigned sz = object.size(); //size of parsed string
						    unsigned index = object.find(';'); //location of searched item
						    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
						    
						}
						
						if(object.find(")") != string::npos && object.at(object.size() - 1) == ';')
						{
							object.erase(object.find(")"), object.size() - object.find(")"));
							this->insert(entry, object);
							return;
						}
						
						else if(object.find(")") != string::npos && object.at(object.size() - 1) == ')')
						{
							object.erase(object.find(")"), object.size() - object.find(")"));
							this->insert(entry, object);
							return;
						}
						else if(object.find(")") != string::npos && object.at(object.size() - 1) != ')')
						{
							cout << "error: foreign item detected after parenthesis" << endl;
							this->insert(entry, "null");
							return;
						}
					
						this->insert(entry, object);
			        }
				}		
		    }
		    
		    else if (prev.at(prev.size() - 1) == ';')
		    {
		    	if(arg.find(")") == arg.size() - 1)
				{
					return;
				}
		    	
		    	Executable* entry = new Semicolon(arg);
		    	
		    	if(inSS.eof()) 
		    	{
			        	this->insert(entry, "null");
			        	return;
			    }
			 
		        if(arg.find("(") == 0)
		    	{
		    		Executable* par_entry = new ParSemicolon();
		    		par_entry->parse(arg, arg3, inSS, inSS2, inSS3);
		    		this->insert(par_entry, "null");
		    		
		    		if (arg3.find('#') != string::npos)  //searching for hashtags
				    {
					    //unsigned sz = object.size(); //size of parsed string
					    
					    return;
				    }
		    		
		    		if(arg3.at(arg3.size() - 1) == ')' && arg3.at(arg3.size() - 2) == ')')
		    		{
		    			arg3.resize(arg3.size() - 1);
		    			return;
		    		}
		    		
		    		if(inSS.eof()) { return; }
		    	}
		  
		    	else
		    	{
		    		inSS >> object;
		        	inSS2 >> arg2;
		    		inSS3 >> arg3;
		    		
		    		if(arg == "test") //parsing test
			        {
			        	string multi, multi_2;
			        	
			        	if(object == "-e" || object == "-f" || object == "-d")
			        	{
			        		inSS >> multi;
			        		inSS2 >> multi_2;
			        		inSS3 >> arg3;
			        		
			        		object = object + " " + multi;
			        	}
			        	else //if no flag included, default to -e
			        	{
			        		multi = object;
			        		object = "-e";
			        		object = object + " " + multi;
			        	}
			        	
			        }
			        
			        else if(arg == "[") //check for symbolic type test
			        {
			        	arg = "test";
			        	
			        	string multi, multi_2;
			        	
			        	if(object == "-e" || object == "-f" || object == "-d")
			        	{
			        		inSS >> multi;
			        		inSS2 >> multi_2;
			        		inSS3 >> arg3;
			        		
			        		object = object + " " + multi;
			        	}
			        	else //if no flag included, default to -e
			        	{
			        		multi = object;
			        		object = "-e";
			        		object = object + " " + multi;
			        	}
			        	
			        	inSS2 >> arg2;
			        	inSS3 >> arg3;
			        	if(arg2 == "]") //check for closing condition
			        	inSS >> arg2;
			        	
			        }
			        if(object.find("\"") == 0) //for multiple command arguements
			        {
			        	object.erase(0,1);
			        	arg2.erase(0,1);
			        	arg3.erase(0,1);
			        	
			        	string multi; //to hold multiple strings
			        	string multi_2;
			        	while(!inSS.eof())
			        	{
			        		inSS >> multi;
			        		inSS2 >> multi_2;
			        		inSS3 >> arg3;
			        		
			        		object = object + " " + multi;
			        		
			        		if(object.find("\"") == object.size() - 1)
			        		{
			        			object.resize(object.size() - 1);
			        			//cout << object << endl; //test output
			        			return;
			        		}
			        		
			        	}
			        	
			        	if(inSS.eof()) 
			        	{ 
			        		this->insert(entry, object);
			        		return; 
			        	}
			        }
			        
			        if (object.find('#') != string::npos) 
				    {
					    //unsigned sz = object.size(); //size of parsed string
					    unsigned index = object.find('#'); //location of searched item
					    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
					    if(object.size() == 0) {return;}
					    
					    this->insert(entry, object);
					    return;
				    }
				    
				    //cout << prev << endl;
					prev = object;
					
			        if ( (object.find(';') != string::npos) && (object.find(';') == object.size() - 1) ) 
					{
						//unsigned sz = object.size(); //size of parsed string
					    unsigned index = object.find(';'); //location of searched item
					    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
					    
					}
					
					if(object.find(")") != string::npos && object.at(object.size() - 1) == ';')
					{
						object.erase(object.find(")"), object.size() - object.find(")"));
						this->insert(entry, object);
						return;
					}
					
					else if(object.find(")") != string::npos && object.at(object.size() - 1) == ')')
					{
						object.erase(object.find(")"), object.size() - object.find(")"));
						this->insert(entry, object);
						return;
					}
					else if(object.find(")") != string::npos && object.at(object.size() - 1) != ')')
					{
						cout << "error: foreign item detected after parenthesis" << endl;
						this->insert(entry, "null");
						return;
					}
				
					this->insert(entry, object);
			    	
			    }
			}
			
			else
			{
				cout << "error " << endl;
				exit(0);
			}
			
		}
}
void ParAmpersand::apply(unsigned int x, vector<Executable*> out, vector<string> in)
{
 	// cout << out.size() << endl;
 	// cout << out.at(x)->getVectorSize() << endl;
 	
 	if(x > 0 && (out.at(x - 1)->getValidate() == 'n') )
 	{
 		out.at(x)->changeValidate('n');
 		if(x == out.size() - 1 && this->validate != 'y' && this->validate != 'n')
	 	{
	 		this->validate = 'n';
	 	}
 		return;
 	}
 	if(out.at(x)->getVectorSize() >= 1)
 	{
		out.at(x)->execute();
		// if(out.at(x)->getValidate() == 'y')
 	// 	{
 	// 		this->validate = 'y';
 	// 	}
 	// 	if(x == out.size() - 1 && this->validate != 'y' && this->validate != 'n')
	 //	{
	 //		this->validate = 'n';
	 //	}
 	}
 	
}

ParSemicolon::ParSemicolon():Executable() {}

void ParSemicolon::parse(string arg_hold, string& arg3, istringstream& inSS, istringstream& inSS2, istringstream& inSS3)
{
	string arg;
	string arg2;
	string object;
	string prev = ";";
	
		arg_hold.erase(0,1);
		arg3.erase(0,1);
		//cout << arg_hold << endl;
		
		if (arg_hold.find('#') != string::npos)  //searching for hashtags
	    {
		    //unsigned sz = object.size(); //size of parsed string
		    unsigned index = object.find('#'); //location of searched item
		    arg.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
		    if(arg.size() == 0) {return;}
		    
		    Executable* entry = new Semicolon(arg);
		    this->insert(entry, NULL);
		    return;
	    }
		
		if(arg_hold.size() == 0)
		{
			
		}
		
		else if(arg_hold.find("(") == 0)
    	{
    		Executable* par_entry = new ParSemicolon();
    		par_entry->parse(arg_hold, arg3, inSS, inSS2, inSS3);
    		this->insert(par_entry, "null");
    		
    		if(arg3.at(arg3.size() - 1) == ')' && arg3.at(arg3.size() - 2) == ')')
    		{
    			arg3.resize(arg3.size() - 1);
    			return;
    		}
    		
    		if(inSS.eof()) { return; }
    		
    	}
    	
    	else
    	{
			Executable* entry_hold = new Semicolon(arg_hold);
    	
		
			inSS >> object;
	        inSS2 >> arg2;
	        inSS3 >> arg3;
	        
	        if(arg == "test") //parsing test
	        {
	        	string multi, multi_2;
	        	
	        	if(object == "-e" || object == "-f" || object == "-d")
	        	{
	        		inSS >> multi;
	        		inSS2 >> multi_2;
	        		inSS3 >> arg3;
	        		
	        		object = object + " " + multi;
	        	}
	        	else //if no flag included, default to -e
	        	{
	        		multi = object;
	        		object = "-e";
	        		object = object + " " + multi;
	        	}
	        	
	        }
	        
	        else if(arg == "[") //check for symbolic type test
	        {
	        	arg = "test";
	        	
	        	string multi, multi_2;
	        	
	        	if(object == "-e" || object == "-f" || object == "-d")
	        	{
	        		inSS >> multi;
	        		inSS2 >> multi_2;
	        		inSS3 >> arg3;
	        		
	        		object = object + " " + multi;
	        	}
	        	else //if no flag included, default to -e
	        	{
	        		multi = object;
	        		object = "-e";
	        		object = object + " " + multi;
	        	}
	        	
	        	inSS2 >> arg2;
	        	inSS3 >> arg3;
	        	if(arg2 == "]") //check for closing condition
	        	inSS >> arg2;
	        	
	        }
	        
	        if(object.find("\"") == 0) //for multiple command arguements
	        {
	        	object.erase(0,1);
	        	arg2.erase(0,1);
	        	arg3.erase(0,1);
	        	
	        	string multi; //to hold multiple strings
	        	string multi_2;
	        	while(!inSS.eof())
	        	{
	        		inSS >> multi;
	        		inSS2 >> multi_2;
	        		inSS3 >> arg3;
	        		
	        		object = object + " " + multi;
	        		
	        		if(object.find("\"") == object.size() - 1)
	        		{
	        			object.resize(object.size() - 1);
	        			//cout << object << endl; //test output
	        			return;
	        		}
	        		
	        	}
	        }
	    	
	    	if (object.find('#') != string::npos)  //searching for hashtags
		    {
			    //unsigned sz = object.size(); //size of parsed string
			    unsigned index = object.find('#'); //location of searched item
			    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
			    if(object.size() == 0) {return;}
			    
			    this->insert(entry_hold, object);
			    return;
		    }
		    
		    prev = object;
		     
	        if ( (object.find(';') != string::npos) && (object.find(';') == object.size() - 1) ) 
			{
				//unsigned sz = object.size(); //size of parsed string
			    unsigned index = object.find(';'); //location of searched item
			    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
			    
			}
			
			if(object.find(")") != string::npos && object.at(object.size() - 1) == ';')
			{
				object.erase(object.find(")"), object.size() - object.find(")"));
				this->insert(entry_hold, object);
				return;
			}
			
			else if(object.find(")") != string::npos && object.at(object.size() - 1) == ')')
			{
				object.erase(object.find(")"), object.size() - object.find(")"));
				this->insert(entry_hold, object);
				return;
			}
			else if(object.find(")") != string::npos && object.at(object.size() - 1) != ')')
			{
				cout << "error: foreign item detected after parenthesis" << endl;
				this->insert(entry_hold, "null");
				return;
			}
			
			 this->insert(entry_hold, object);
			 
	  	    if(inSS.eof()) 
	    	{ 
	    		this->insert(entry_hold, object);
	    		return; 
	    	}
    	}
		
		while(!inSS.eof() )
		{
			inSS >> arg;
			inSS2 >> arg2;
			inSS3 >> arg3;
			
			if (arg.find('#') != string::npos)  //searching for hashtags
		    {
			    //unsigned sz = object.size(); //size of parsed string
			    unsigned index = object.find('#'); //location of searched item
			    arg.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
			    if(arg.size() == 0) {return;}
			    
			    Executable* entry = new Semicolon(arg);
			    this->insert(entry, NULL);
			    return;
		    }
			if (arg.find("&&") == 0) //if a double Ampersand is found countains no other characters
			{
			    
			    if(!inSS.eof() ) //making sure there is a command after the ampersands
			    {
					inSS >> arg;
					inSS2 >> arg2;
			        inSS3 >> arg3;
			        
			        if (arg.find('#') != string::npos)  //searching for hashtags
				    {
					    //unsigned sz = object.size(); //size of parsed string
					    unsigned index = object.find('#'); //location of searched item
					    arg.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
					    if(arg.size() == 0) {return;}
					    
					    Executable* entry = new Semicolon(arg);
					    this->insert(entry, NULL);
					    return;
				    }
			        
			    	if(arg.find("(") == 0)
			    	{
			    		Executable* par_entry = new ParAmpersand();
			    		par_entry->parse(arg, arg3, inSS, inSS2, inSS3);
			    		this->insert(par_entry, "null");
			    		
			    		if(arg3.at(arg3.size() - 1) == ')' && arg3.at(arg3.size() - 2) == ')')
			    		{
			    			arg3.resize(arg3.size() - 1);
			    			return;
			    		}
				    		
				    		if(inSS.eof()) { return; }
			    	}
			        
			        else
			        {
				        Executable* entry = new Ampersand(arg);
				        if(inSS.eof()) 
				        {
				        	this->insert(entry, "null");
				        	return;
				        }
				        	
				        inSS >> object;
				        inSS2 >> arg2;
				        inSS3 >> arg3;
				        
				        if(arg == "test") //parsing test
				        {
				        	string multi, multi_2;
				        	
				        	if(object == "-e" || object == "-f" || object == "-d")
				        	{
				        		inSS >> multi;
				        		inSS2 >> multi_2;
				        		inSS3 >> arg3;
				        		
				        		object = object + " " + multi;
				        	}
				        	else //if no flag included, default to -e
				        	{
				        		multi = object;
				        		object = "-e";
				        		object = object + " " + multi;
				        	}
				        	
				        }
				        
				        else if(arg == "[") //check for symbolic type test
				        {
				        	arg = "test";
				        	
				        	string multi, multi_2;
				        	
				        	if(object == "-e" || object == "-f" || object == "-d")
				        	{
				        		inSS >> multi;
				        		inSS2 >> multi_2;
				        		inSS3 >> arg3;
				        		
				        		object = object + " " + multi;
				        	}
				        	else //if no flag included, default to -e
				        	{
				        		multi = object;
				        		object = "-e";
				        		object = object + " " + multi;
				        	}
				        	
				        	inSS2 >> arg2;
				        	inSS3 >> arg3;
				        	if(arg2 == "]") //check for closing condition
				        	inSS >> arg2;
				        	
				        }
				        
				        if(object.find("\"") == 0) //for multiple command arguements
				        {
				        	object.erase(0,1);
				        	arg2.erase(0,1);
				        	arg3.erase(0,1);
				        	
				        	string multi; //to hold multiple strings
				        	string multi_2;
				        	while(!inSS.eof())
				        	{
				        		inSS >> multi;
				        		inSS2 >> multi_2;
				        		inSS3 >> arg3;
				        		object = object + " " + multi;
				        		
				        		if(object.find("\"") == object.size() - 1 || object.find("\"") == object.size() - 2)
				        		{
				        			object.resize(object.size() - 2);
				        			//cout << object << endl; //test output
				        			return;
				        		}
				        		
				        	}
				        	
				        	if(inSS.eof()) 
				        	{ 
				        		this->insert(entry, object);
				        		return; 
				        	}
				        }
				        
				        //cout << object << endl; //test output
				        
				        if (object.find('#') != string::npos)  //searching for hashtags
					    {
						    //unsigned sz = object.size(); //size of parsed string
						    unsigned index = object.find('#'); //location of searched item
						    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
						    if(object.size() == 0) {return;}
						    
						    this->insert(entry, object);
						    return;
					    }
					    
					    prev = object;
					     
				        if ( (object.find(';') != string::npos) && (object.find(';') == object.size() - 1) ) 
						{
							//unsigned sz = object.size(); //size of parsed string
						    unsigned index = object.find(';'); //location of searched item
						    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
						    
						}
						
						if(object.find(")") != string::npos && object.at(object.size() - 1) == ';')
						{
							object.erase(object.find(")"), object.size() - object.find(")"));
							this->insert(entry, object);
							return;
						}
						
						else if(object.find(")") != string::npos && object.at(object.size() - 1) == ')')
						{
							object.erase(object.find(")"), object.size() - object.find(")"));
							this->insert(entry, object);
							return;
						}
						else if(object.find(")") != string::npos && object.at(object.size() - 1) != ')')
						{
							cout << "error: foreign item detected after parenthesis" << endl;
							this->insert(entry, "null");
							return;
						}
						
					
						this->insert(entry, object);
			        }
				}
	
			 }
	
			
			else if (arg.find("||") == 0) //if a double vertical is found countains no other characters
			{
			    if(!inSS.eof() ) //making sure there is a command after the ampersands
			    {
					inSS >> arg;
					inSS2 >> arg2;
			        inSS3 >> arg3;
			        
			        if (arg.find('#') != string::npos)  //searching for hashtags
				    {
					    //unsigned sz = object.size(); //size of parsed string
					    unsigned index = object.find('#'); //location of searched item
					    arg.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
					    if(arg.size() == 0) {return;}
					    
					    Executable* entry = new Semicolon(arg);
					    this->insert(entry, NULL);
					    return;
				    }
			        
			    	if(arg.find("(") == 0)
			    	{
			    		Executable* par_entry = new ParVertical();
			    		par_entry->parse(arg, arg3, inSS, inSS2, inSS3);
			    		this->insert(par_entry, "null");
			    		
			    		if(arg3.at(arg3.size() - 1) == ')' && arg3.at(arg3.size() - 2) == ')')
			    		{
			    			arg3.resize(arg3.size() - 1);
			    			return;
			    		}
			    		
			    		if(inSS.eof()) { return; }
			    	}
			        
			        else
			        {
				        Executable* entry = new Vertical(arg);
				        if(inSS.eof()) 
				        {
				        	this->insert(entry, "null");
				        	return;
				        }
				        	
				        inSS >> object;
				        inSS2 >> arg2;
				        inSS3 >> arg3;
				        
				        if(arg == "test") //parsing test
				        {
				        	string multi, multi_2;
				        	
				        	if(object == "-e" || object == "-f" || object == "-d")
				        	{
				        		inSS >> multi;
				        		inSS2 >> multi_2;
				        		inSS3 >> arg3;
				        		
				        		object = object + " " + multi;
				        	}
				        	else //if no flag included, default to -e
				        	{
				        		multi = object;
				        		object = "-e";
				        		object = object + " " + multi;
				        	}
				        	
				        }
				        
				        else if(arg == "[") //check for symbolic type test
				        {
				        	arg = "test";
				        	
				        	string multi, multi_2;
				        	
				        	if(object == "-e" || object == "-f" || object == "-d")
				        	{
				        		inSS >> multi;
				        		inSS2 >> multi_2;
				        		inSS3 >> arg3;
				        		
				        		object = object + " " + multi;
				        	}
				        	else //if no flag included, default to -e
				        	{
				        		multi = object;
				        		object = "-e";
				        		object = object + " " + multi;
				        	}
				        	
				        	inSS2 >> arg2;
				        	inSS3 >> arg3;
				        	if(arg2 == "]") //check for closing condition
				        	inSS >> arg2;
				        	
				        }
				        
				        if(object.find("\"") == 0) //for multiple command arguements
				        {
				        	object.erase(0,1);
				        	arg2.erase(0,1);
				        	arg3.erase(0,1);
				        	
				        	string multi; //to hold multiple strings
				        	string multi_2;
				        	while(!inSS.eof())
				        	{
				        		inSS >> multi;
				        		inSS2 >> multi_2;
				        		inSS3 >> arg3;
				        		object = object + " " + multi;
				        		
				        		if(object.find("\"") == object.size() - 1 || object.find("\"") == object.size() - 2)
				        		{
				        			object.resize(object.size() - 2);
				        			//cout << object << endl; //test output
				        			return;
				        		}
				        		
				        	}
				        	
				        	if(inSS.eof()) 
				        	{ 
				        		this->insert(entry, object);
				        		return; 
				        	}
				        }
				        
				        //cout << object << endl; //test output
				        
				        if (object.find('#') != string::npos)  //searching for hashtags
					    {
						    //unsigned sz = object.size(); //size of parsed string
						    unsigned index = object.find('#'); //location of searched item
						    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
						    if(object.size() == 0) {return;}
						    
						    this->insert(entry, object);
						    return;
					    }
					    
					    prev = object;
					     
				        if ( (object.find(';') != string::npos) && (object.find(';') == object.size() - 1) ) 
						{
							//unsigned sz = object.size(); //size of parsed string
						    unsigned index = object.find(';'); //location of searched item
						    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
						    
						}
						
						if(object.find(")") != string::npos && object.at(object.size() - 1) == ';')
						{
							object.erase(object.find(")"), object.size() - object.find(")"));
							this->insert(entry, object);
							return;
						}
						
						else if(object.find(")") != string::npos && object.at(object.size() - 1) == ')')
						{
							object.erase(object.find(")"), object.size() - object.find(")"));
							this->insert(entry, object);
							return;
						}
						else if(object.find(")") != string::npos && object.at(object.size() - 1) != ')')
						{
							cout << "error: foreign item detected after parenthesis" << endl;
							this->insert(entry, "null");
							return;
						}
					
						this->insert(entry, object);
			        }
				}		
		    }
		    
		    else if (prev.at(prev.size() - 1) == ';')
		    {
		    	if(arg.find(")") == arg.size() - 1)
				{
					return;
				}
		    	
		    	Executable* entry = new Semicolon(arg);
		    	
		    	if(inSS.eof()) 
		    	{
			        	this->insert(entry, "null");
			        	return;
			    }
			           
		        
		        if(arg.find("(") == 0)
		    	{
		    		Executable* par_entry = new ParSemicolon();
		    		par_entry->parse(arg, arg3, inSS, inSS2, inSS3);
		    		this->insert(par_entry, "null");
		    		
		    		if(arg3.at(arg3.size() - 1) == ')' && arg3.at(arg3.size() - 2) == ')')
		    		{
		    			arg3.resize(arg3.size() - 1);
		    			return;
		    		}
		    		
		    		if(inSS.eof()) { return; }
		    	}
	
		    	else
		    	{
		    		inSS >> object;
		        	inSS2 >> arg2;
		    		inSS3 >> arg3;
		    		
		    		if(arg == "test") //parsing test
			        {
			        	string multi, multi_2;
			        	
			        	if(object == "-e" || object == "-f" || object == "-d")
			        	{
			        		inSS >> multi;
			        		inSS2 >> multi_2;
			        		inSS3 >> arg3;
			        		
			        		object = object + " " + multi;
			        	}
			        	else //if no flag included, default to -e
			        	{
			        		multi = object;
			        		object = "-e";
			        		object = object + " " + multi;
			        	}
			        	
			        }
			        
			        else if(arg == "[") //check for symbolic type test
			        {
			        	arg = "test";
			        	
			        	string multi, multi_2;
			        	
			        	if(object == "-e" || object == "-f" || object == "-d")
			        	{
			        		inSS >> multi;
			        		inSS2 >> multi_2;
			        		inSS3 >> arg3;
			        		
			        		object = object + " " + multi;
			        	}
			        	else //if no flag included, default to -e
			        	{
			        		multi = object;
			        		object = "-e";
			        		object = object + " " + multi;
			        	}
			        	
			        	inSS2 >> arg2;
			        	inSS3 >> arg3;
			        	if(arg2 == "]") //check for closing condition
			        	inSS >> arg2;
			        	
			        }
			        if(object.find("\"") == 0) //for multiple command arguements
			        {
			        	object.erase(0,1);
			        	arg2.erase(0,1);
			        	arg3.erase(0,1);
			        	
			        	string multi; //to hold multiple strings
			        	string multi_2;
			        	while(!inSS.eof())
			        	{
			        		inSS >> multi;
			        		inSS2 >> multi_2;
			        		inSS3 >> arg3;
			        		
			        		object = object + " " + multi;
			        		
			        		if(object.find("\"") == object.size() - 1)
			        		{
			        			object.resize(object.size() - 1);
			        			//cout << object << endl; //test output
			        			return;
			        		}
			        		
			        	}
			        	
			        	if(inSS.eof()) 
			        	{ 
			        		this->insert(entry, object);
			        		return; 
			        	}
			        }
			        
			        if (object.find('#') != string::npos) 
				    {
					    //unsigned sz = object.size(); //size of parsed string
					    unsigned index = object.find('#'); //location of searched item
					    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
					    if(object.size() == 0) {return;}
					    
					    this->insert(entry, object);
					    return;
				    }
				    
				    prev = object;
				    
			        if ( (object.find(';') != string::npos) && (object.find(';') == object.size() - 1) ) 
					{
						//unsigned sz = object.size(); //size of parsed string
					    unsigned index = object.find(';'); //location of searched item
					    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
					    
					}
					
					if(object.find(")") != string::npos && object.at(object.size() - 1) == ';')
					{
						object.erase(object.find(")"), object.size() - object.find(")"));
						this->insert(entry, object);
						return;
					}
					
					else if(object.find(")") != string::npos && object.at(object.size() - 1) == ')')
					{
						object.erase(object.find(")"), object.size() - object.find(")"));
						this->insert(entry, object);
						return;
					}
					else if(object.find(")") != string::npos && object.at(object.size() - 1) != ')')
					{
						cout << "error: foreign item detected after parenthesis" << endl;
						this->insert(entry, "null");
						return;
					}
				
					
					this->insert(entry, object);
			    	
			    }
			}
			
			else
			{
				cout << "error " << endl;
				exit(0);
			}
			
		}
}
void ParSemicolon::apply(unsigned int x, vector<Executable*> out, vector<string> in)
{
 	// cout << out.size() << endl;
 	// cout << out.at(x)->getVectorSize() << endl;

 	if(out.at(x)->getVectorSize() >= 1)
 	{
 		out.at(x)->execute();
 		// if(out.at(x)->getValidate() == 'y')
 		// {
 		// 	this->validate = 'y';
 		// }
 		// if(x == out.size() - 1 && this->validate != 'y' && this->validate != 'n')
	 	// {
	 	// 	this->validate = 'n';
	 	// }
 		return;
 	}
 	
}

Direction::Direction(string info, char ignore, char type):Executable(info)
{
	this->info = info;
	this->ignore = ignore;
	this->type = type;
	
	if(info.find("tr") == string::npos || info.find("ls") == string::npos || info.find("grep") == string::npos)
	{
		this->cmnd = new char * [2];
		this->cmnd[0] = (char*)info.c_str();
		this->cmnd[1] = NULL;
	}
	else
	{
		istringstream inSS(info);
		string holder;
		string combine;
		inSS >> holder;
		
		this->cmnd = new char * [3];
		this->cmnd[0] = (char*)holder.c_str();
		
		while(!inSS.eof())
		{
			inSS >> holder;
			combine = combine + " " + holder;
		}
		this->cmnd[1] = (char*)combine.c_str();
	}
}
Direction::Direction(string info, string file, char ignore, char type):Executable(info)
{
	this->info = info;
	this->ignore = ignore;
	this->type = type;
	this->file = file;
	this->cmnd = new char * [2];
	this->cmnd[0] = (char*)info.c_str();
	this->cmnd[1] = NULL;
}
char Direction::get_ignore()
{
	return this->ignore;
}
string Direction::get_file() { return this->file; }
char Direction::get_type()
{
	return this->type;
}

void Direction::apply(unsigned int x, vector<Executable*> out, vector<string> in)
{
	//cout << out.size() << endl;
	
	unsigned int h = 0;

	if(this->get_ignore() == 'y') {return;}
	for(h = x; h < out.size(); ++h)
	{
		if(out.at(h)->get_ignore() != 'n') { break; }
		//cout << out.at(h)->get_cmnd()[0] << h << endl;
		//cout << out.at(x)->get_type() << endl;
		//cout << h << endl;
	} 
	--h;
	
	int p[2]; //the difernce between where the piping starts and ends
    pid_t pid;
    int fd_in = 0;
	
	//cout << h << endl; 
	//int i = 1;
	while(x <= h) 
	{
	    if(out.at(x)->get_ignore() == 'n')
	    {
	    //cout << x << endl; 
	    out.at(x)->changeIgnore('y');
	 /*   char* c[2];
		c[0] = (char*)out.at(x)->getInfo().c_str();
		c[1] = NULL;*/
		
	      if(pipe(p))
	      {
	      	perror("Pipe failed");
	      }
	      if ((pid = fork()) == -1)
	        {
	          exit(EXIT_FAILURE);
	        }
	      else if (pid == 0)
	        {
	          
	          dup2(fd_in, 0); //change the input according to the old one
	    
	          if (x < h)
	          {
	            dup2(p[1], 1);
	          }
	          close(p[0]);
	          if(out.at(x)->get_type() == 'p') //check if cmd is used for piping
	          {
	        	execvp(out.at(x)->get_cmnd()[0], out.at(x)->get_cmnd());
	        	//exit(1);
	          }
	          else if (out.at(x)->get_type() == 'a') //appending to a file
	          {
	          	//cout << "barb" << endl; //test output
	          	close(1); // Release fd no - 1
    			fopen((char*)out.at(x)->get_file().c_str(), "a" );
	          	execvp(out.at(x)->get_cmnd()[0], out.at(x)->get_cmnd());
	          	//exit(1);
	          }
	          else if(out.at(x)->get_type() == 'w') //overwriting a file
	          {
	          	close(1); // Release fd no - 1
    			fopen((char*)out.at(x)->get_file().c_str(), "w" ); // Open a file with fd no = 1
	          	execvp(out.at(x)->get_cmnd()[0], out.at(x)->get_cmnd());
	          	//exit(1);
	          }
	          else if(out.at(x)->get_type() == 'i') //overwriting a file
	          {
	          	 out.at(x)->changeIgnore('n');
	          	 out.at(x)->changeType(out.at(x + 1)->get_type());
	          	 out.at(x + 1)->changeIgnore('y');
	          	 execvp(out.at(x + 1)->get_cmnd()[0], out.at(x)->get_cmnd());
	          	 --x;
	          }
	          else
	          {
	          	execvp(out.at(x)->get_cmnd()[0], out.at(x)->get_cmnd());
	          }
	          exit(EXIT_FAILURE);
	        }
	      else
	        {
	          wait(NULL);
	          close(p[1]);
	          fd_in = p[0]; //save the input for the next command
	          ++x;
	        }
	    }
	    
	}
}






