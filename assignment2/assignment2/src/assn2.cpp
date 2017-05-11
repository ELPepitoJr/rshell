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
    if (argc >= 2) //version #1 for students to test for other purposes
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
		Executable* exec = new Executable();
		while(true)
		{
			cout << "$ ";
			getline(cin, hold);
			istringstream inSS(hold);
			istringstream inSS2(hold);
			string arg2;
			     
			while(!inSS.eof() )
			{
				inSS >> arg;
				inSS2 >> arg2;
				if (arg.find("&&") == 0) //if a double Ampersand is found countains no other characters
				{
				    if(!inSS.eof() ) //making sure there is a command after the ampersands
				    {
						inSS >> arg;
						inSS2 >> arg2;
				        Executable* entry = new Ampersand(arg);
				        
				        if(inSS.eof()) 
				        {
				        	exec->insert(entry, "null");
				        	break;
				        }
				        	
				        inSS >> object;
				        inSS2 >> arg2;
				        
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
				    if(!inSS.eof() ) //making sure there is a command after vertical lines
				    {
					    inSS >> arg;
					    inSS2 >> arg2;
				        Executable* entry = new Vertical(arg);
				        
				        if(inSS.eof()) 
				        {
				        	exec->insert(entry, "null");
				        	break;
				        }
				           
				        inSS >> object;
				        inSS2 >> arg2;
				        
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
			    
			    else
			    {
			    	Executable* entry = new Semicolon(arg);
			    	
			    	if(inSS.eof()) 
			    	{
				        	exec->insert(entry, "null");
				        	break;
				    }
				           
			        inSS >> object;
			        inSS2 >> arg2;
			        
			        if (object.find('#') != string::npos) 
				    {
					    //unsigned sz = object.size(); //size of parsed string
					    unsigned index = object.find('#'); //location of searched item
					    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
					    if(object.size() == 0) {break;}
					    
					    exec->insert(entry, object);
					    break;
				    }
				    inSS2 >> arg2;
				    
			        if ( (object.find(';') != string::npos) && (object.find(';') == object.size() - 1) ) 
					{
						//unsigned sz = object.size(); //size of parsed string
					    unsigned index = object.find(';'); //location of searched item
					    object.resize(index); //resizing string to delete all chars that follow after the '#' "ls#a" becomes "ls"
					    
					    if(arg2 == "&&" || arg2 == "||") 
						{
								exec->insert(entry, object);
								Executable* mal = new Semicolon(arg2);
								exec->insert(mal, "null");
								break;
						}
					    
					}
					else if( !inSS.eof() )
					{
    					
						if(arg2 != "&&" || arg2 != "||") 
						{
								exec->insert(entry, object);
								break;
						}
					}
					
					exec->insert(entry, object);
			    	
			    }
				
			}
			exec->execute();
		}
	}	
	
	

	//exec->print(); //testing commands being parsed correctly
	
	return 0;
}


