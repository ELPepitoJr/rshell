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

int main(int argc, const char * argv []) 
{
    string arg; //holds the string for the commands; ex: ls, git, echo, etc;
    string object; //holds string for the object of commands; 
					//example git "status", echo "hello" (stores "status" or "hello")
    string hold;
	 
	  //initializing what will store the commands
	 
	 //ifstream myFile (argv[argc - 1]); //input from text file (.sh test files 
		// 								//echo the cases to a text file, which s then read by this cpp file)
	 
	 //myFile >> arg; //dumping arguments into a string;
	 //Executable* entry = new Semicolon(arg); //holds proper resolve type for executing
    if (argc >= 2) //version #1 for students to test for other purposes; PLEASE IGNORE!!!!
    {
        Executable* exec = new Executable();
        for(int i = 1; i < argc; ++i)
        {
			 
            arg = argv[i];
				
                if (arg.find("&&") == 0) //if a double Ampersand is found countains no other characters
				{
                    if(i < argc - 1 ) //making sure there is a command after the ampersands
				    {
                        ++i;
						arg = argv[i];
				        Executable* entry = new Ampersand(arg);
				        
				        if(i == argc - 1) {
				        	exec->insert(entry, "null");
				        	break;
				        }
				        	
				        ++i;
						object = argv[i];
				        
				        if (object.find('#') != string::npos)  //searching for hashtags
					    {
						    //unsigned sz = object.size(); //size of parsed string
						    unsigned index = object.find('#'); //location of searched item
						    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
						    if(object.size() == 0) {break;}
						    
						    exec->insert(entry, object);
						    break;
					    }
					     
				        if ( (object.find(';') != string::npos) && (object.find(';') == object.size() - 1) ) 
						{
							//unsigned sz = object.size(); //size of parsed string
						    unsigned index = object.find(';'); //location of searched item
						    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
						    
						}
						
						 exec->insert(entry, object);
					}
		
				 }
				
				
				// if ( (arg.find(';') != string::npos) && (arg.find(';')== arg.size() - 1) ) 
				// {
				//   if( !myFile.eof() ) //making sure there is a command after the semicolon
				//   {
				// 		unsigned sz = arg.size(); //size of parsed string
				// 	    unsigned index = arg.find(';'); //location of searched item
				// 	    arg.resize(index - 1); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
				// 	    if(arg.size() >= 1) //
				// 	    {
				// 	        Executable* entry = new Executable(arg);
				// 	    }
				//           if(myFile.eof() ) {break;}
				          
				//         myFile >> object;
				//         exec->insert(entry, object);
				// 	}
				// }
				
				else if (arg.find("||") == 0) //if a double vertical is found countains no other characters
				{
				    if(i < argc - 1) //making sure there is a command after vertical lines
				    {
					    ++i;
					    arg = argv[i];
				        Executable* entry = new Vertical(arg);
				        
				        if(i == argc - 1) 
				        {
				        	exec->insert(entry, "null");
				        	break;
				        }
				           
				        ++i;
					    object = argv[i];
				        
				        if (object.find('#') != string::npos) 
					    {
						    //unsigned sz = object.size(); //size of parsed string
						    unsigned index = object.find('#'); //location of searched item
						    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
						    if(object.size() == 0) {break;}
						    
						    exec->insert(entry, object);
						    break;
					    }
					     
				        if ( (object.find(';') != string::npos) && (object.find(';') == object.size() - 1) ) 
						{
							//unsigned sz = object.size(); //size of parsed string
						    unsigned index = object.find(';'); //location of searched item
						    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
						    
						}
						
						exec->insert(entry, object);
						
				    }		
			    }
		  
		    // else if(arg == "rtn") //our exit command
		    // {
		    // 	Executable* entry = new Semicolon(arg);
		    // 	exec->insert(entry, "null"); //second arguement is nulled 
		                                     //because the exit function doesn't need a target;
		    // }
		    
		    else
		    {
		    	Executable* entry = new Semicolon(arg);
		    	
		    	if(i == argc - 1) 
		    	{
		    		exec->insert(entry, "null");
		    		break;
		    	}
			    
			    ++i;
		        object = argv[i];
		        
		        if (object.find('#') != string::npos) 
			    {
				    //unsigned sz = object.size(); //size of parsed string
				    unsigned index = object.find('#'); //location of searched item
				    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
				    if(object.size() == 0) {break;}
				    
				    exec->insert(entry, object);
				    break;
			    }
			     
		        if ( (object.find(';') != string::npos) && (object.find(';') == object.size() - 1) ) 
				{
					//unsigned sz = object.size(); //size of parsed string
				    unsigned index = object.find(';'); //location of searched item
				    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
				    
				}
				
				exec->insert(entry, object);
		    	
		    }
		
			exec->execute();
		}
	 }
	
	
	else //final product for instructors
	{
		string prev = ";";
		while(true)
		{
			cout << "$ ";
			getline(cin, hold);
			istringstream inSS(hold);
			istringstream inSS2(hold);
			istringstream inSS3(hold);
			
			string arg2, arg3;
			
			Executable* exec = new Executable(); 
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
				    if(object.size() == 0) {break;}
				    
				    Executable* entry = new Ampersand(NULL);
				    exec->insert(entry, NULL);
				    break;
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
						    if(object.size() == 0) {break;}
						    
						    Executable* entry = new Ampersand(NULL);
						    exec->insert(entry, NULL);
						    break;
					    }
				        
				    	if(arg.find("(") == 0)
				    	{
				    		Executable* par_entry = new ParAmpersand();
				    		par_entry->parse(arg, arg3, inSS, inSS2, inSS3);
				    		exec->insert(par_entry, "null");
				    		
				    		if (arg3.find('#') != string::npos)  //searching for hashtags
						    {
							   break;
						    }
				    	}
				    
					    
				        else
				        {

					        if(inSS.eof()) 
					        {
					        	Executable* entry = new Ampersand(arg);
					        	exec->insert(entry, "null");
					        	break;
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
					        
					        else if(object.find("\"") == 0) //for multiple command arguements
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
					        			break;
					        		}
					        		
					        	}
					        	
					        	if(inSS.eof()) 
					        	{ 
					        		Executable* entry = new Ampersand(arg);
					        		exec->insert(entry, object);
					        		break; 
					        	}
					        }
				        
				        //cout << object << endl; //test output
				        
					        if (object.find('#') != string::npos)  //searching for hashtags
						    {
							    //unsigned sz = object.size(); //size of parsed string
							    unsigned index = object.find('#'); //location of searched item
							    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
							    if(object.size() == 0) {break;}
							    
							    Executable* entry = new Ampersand(arg);
							    exec->insert(entry, object);
							    break;
						    }
						    
						    prev = object;
						    
					        if ( (object.find(';') != string::npos) && (object.find(';') == object.size() - 1) ) 
							{
								//unsigned sz = object.size(); //size of parsed string
							    unsigned index = object.find(';'); //location of searched item
							    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
							    
							}
						
						Executable* entry = new Ampersand(arg);
						 exec->insert(entry, object);
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
						    if(object.size() == 0) {break;}
						    
						    Executable* entry = new Ampersand(NULL);
						    exec->insert(entry, NULL);
						    break;
					    }
				        
				    	if(arg.find("(") == 0)
				    	{
				    		Executable* par_entry = new ParVertical();
				    		par_entry->parse(arg, arg3, inSS, inSS2, inSS3);
				    		exec->insert(par_entry, "null");
				    		
				    		if (arg3.find('#') != string::npos)  //searching for hashtags
						    {
							   break;
						    }
				    	}
				    
					    
				        else
				        {

					        if(inSS.eof()) 
					        {
					        	Executable* entry = new Vertical(arg);
					        	exec->insert(entry, "null");
					        	break;
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
					        
					        else if(object.find("\"") == 0) //for multiple command arguements
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
					        			break;
					        		}
					        		
					        	}
					        	
					        	if(inSS.eof()) 
					        	{ 
					        		Executable* entry = new Vertical(arg);
					        		exec->insert(entry, object);
					        		break; 
					        	}
					        }
				        
				        //cout << object << endl; //test output
				        
					        if (object.find('#') != string::npos)  //searching for hashtags
						    {
							    //unsigned sz = object.size(); //size of parsed string
							    unsigned index = object.find('#'); //location of searched item
							    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
							    if(object.size() == 0) {break;}
							    
							    Executable* entry = new Vertical(arg);
							    exec->insert(entry, object);
							    break;
						    }
						    
						    prev = object;
						     
					        if ( (object.find(';') != string::npos) && (object.find(';') == object.size() - 1) ) 
							{
								//unsigned sz = object.size(); //size of parsed string
							    unsigned index = object.find(';'); //location of searched item
							    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
							    
							}
						
						 Executable* entry = new Vertical(arg);
						 exec->insert(entry, object);
				        }
				         
					}
		
				 }
			    
			    else if(prev.at(prev.size() - 1) == ';')
			    {
			    	
			    	if(inSS.eof()) 
			    	{
			        	Executable* entry = new Semicolon(arg);
			        	exec->insert(entry, "null");
			        	break;
				    }
				           
			        // inSS >> object;
			        // inSS2 >> arg2;
			       
			    	if(arg.find("(") == 0)
			    	{
			    		Executable* par_entry = new ParSemicolon();
			    		par_entry->parse(arg, arg3, inSS, inSS2, inSS3);
			    		exec->insert(par_entry, "null");
			    		
			    		if (arg3.find('#') != string::npos)  //searching for hashtags
					    {
						   break;
					    }
			    	}
			    
				    
			        else
			        {

				        if(inSS.eof()) 
				        {
				        	Executable* entry = new Semicolon(arg);
				        	exec->insert(entry, "null");
				        	break;
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
				        
				        else if(object.find("\"") == 0) //for multiple command arguements
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
				        			break;
				        		}
				        		
				        	}
				        	
				        	if(inSS.eof()) 
				        	{ 
				        		Executable* entry = new Semicolon(arg);
				        		exec->insert(entry, object);
				        		break; 
				        	}
				        }
			        
			        //cout << object << endl; //test output
			        
				        if (object.find('#') != string::npos)  //searching for hashtags
					    {
						    //unsigned sz = object.size(); //size of parsed string
						    unsigned index = object.find('#'); //location of searched item
						    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
						    if(object.size() == 0) {break;}
						    
						    Executable* entry = new Semicolon(arg);
						    exec->insert(entry, object);
						    break;
					    }
					    
					    prev = object;
					     
				        if ( (object.find(';') != string::npos) && (object.find(';') == object.size() - 1) ) 
						{
							//unsigned sz = object.size(); //size of parsed string
						    unsigned index = object.find(';'); //location of searched item
						    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
						    
						}
					
					 Executable* entry = new Semicolon(arg);
					 exec->insert(entry, object);
			        }

			}
			
			else
			{
				exec->execute();
				cout << "error! exiting..." << endl;
				return 0;
			}
			//exec->print();
		}
		exec->execute();
		prev = ";";
	}	
	
	

	//exec->print(); //testing commands being parsed correctly
	
	return 0;
	}	
}

