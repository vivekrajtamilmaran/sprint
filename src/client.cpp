//The program to create a test TCP client
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <limits>
#include <sstream>
#include <string.h>
#include<arpa/inet.h>
#include <termios.h>
#define ADDRSERV "127.0.0.1"
#define MAX 1024
#define PORT 8028
using namespace std;

class Client{
	int sockfd;
	sockaddr_in servaddr;
	int portnumber,slen,mlen,connectfd;
	string ip_address;
	
	public:
		Client(){
			slen=sizeof(sockaddr_in);
                	memset(&servaddr,0,slen);
	
        	        servaddr.sin_family = AF_INET; 
        	        servaddr.sin_addr.s_addr = inet_addr(ADDRSERV);
        	        servaddr.sin_port = htons(PORT);
	
	
	                sockfd=socket(AF_INET, SOCK_STREAM, 0);
	                if(sockfd < 0){
	                        fputs("Socket can't be created ",stderr);
	                        exit(EXIT_FAILURE);
	                }
		}

		string getCredentials(){
			string  uName ;	
    			string password;
			cout << "Enter the Username :" ;
			getline(cin,uName);
			if(uName=="\n"){
				uName="";
			}
			termios oldt;
    			tcgetattr(STDIN_FILENO, &oldt);
    			termios newt = oldt;
    			newt.c_lflag &= ~ECHO;
    			tcsetattr(STDIN_FILENO, TCSANOW, &newt);

			cout  <<"Enter the password :" ;
    			getline(cin, password);
			if(password=="\n"){
				password="";
			}
			tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
			cout << endl ;
			string concat = uName + ":" + password ;
			return concat;
		}
		int serverConnect( ){
	                connectfd=connect(sockfd,(struct sockaddr *)&servaddr,slen);
	                if(connectfd < 0){
	                        fputs("Socket can't be created ",stderr);
        	                exit(EXIT_FAILURE);
       	         	}	
			return connectfd;
		}

		int sendData(string input){
			    
                    	    mlen=send(sockfd,input.c_str(),strlen(input.c_str()),0);
			    
				if(input=="bye"||input=="Bye" && mlen ==0){
					closeClientConnections();
				}
	
				    return mlen ;
			    
        	}
		
		string recvData(){
				 char output[1024] ;
				 memset(output,0,1024);
				 mlen = recv(sockfd,output,1024,0);
				 if(mlen==0){
				 	closeClientConnections();
				}
				 return string(output);
		}
		string getUserCommands(string type){
		//	int type[3]={1,2,3} ;
			while(1){
				string subcommand("");
				string copysubcommand;
				cout << "Enter the subcommand :" ;
				getline(cin, subcommand);
				copysubcommand=subcommand;
				if(type=="authenticated user"){
					string command(""),input1(""),input2("");
					stringstream ss(subcommand);
					getline(ss,command,' ');
					getline(ss,input1,',');
					getline(ss,input2,',');
					if(command =="ADD"){
						if(input1!=""&&input2!=""){
							return copysubcommand;
						}
						else{
							cout << "Enter two fields" << endl ;
							continue;
						}
					}
					else if(command =="rm"){
						if(input1!="" && input2 == ""){
							return copysubcommand ;
						}
						else{
							cout << "Enter one field" << endl ;
							continue;
						}
					}
					else if(command =="list"){
						if(input1!="" && input2 == ""){
							return copysubcommand ;
						}
						else{
							cout << "Enter one field" << endl ;
							continue;
						}
					}
					else if(command == "chgrp"){
						if(input1!="" && input2 == ""){
							return copysubcommand ;
						}
						else{
							cout << "Enter groupname only" << endl ;
							continue;
						}
					}
					else if(command =="Bye"||command=="bye"){
						if(input1==""&&input2==""){
							return copysubcommand ;
						}
						else{
							cout << "No inputs with bye/Bye" << endl ;
						}
					}
					else{
						cout << "Incorrect subcommand" << endl ;
						continue;
					}
									
				}
				if(type=="admin"){
					string command(""),input1(""),input2("");
					stringstream ss(subcommand);
					getline(ss,command,' ');
					getline(ss,input1);
					getline(ss,input2,',');
					if(command =="ADD"){
						if(input1!="" && input2==""){
							return copysubcommand ;
						}
						else{
							cout << "Enter groupname only" << endl ;
							continue;
						}
					}
					else if(command == "rm"){
						if(input1!="" && input2 ==""){
							return copysubcommand ;
						}
						else{
							cout << "Enter groupname only" << endl ;
							continue;
						}
					}
					else if(command =="Bye"||command=="bye"){
						if(input1==""&&input2==""){
							return copysubcommand ;
						}
						else{
							cout << "No inputs with bye/Bye" << endl ;
						}
					}
					else{
						cout << "Incorrect subcommand" << endl ;
						continue;
					}
				}
				if(type=="anonymous user"){
					string command(""),input1(""),input2("");
					stringstream ss(subcommand);
					getline(ss,command,' ');
					getline(ss,input1,',');
					getline(ss,input2,',');
					if(command =="ADD"){
						if(input1!=""&input2!=""){
							return copysubcommand ;
						}
							else{
							cout << "Enter two fields" << endl ;
							continue;
						}
					}
					else if(command =="Bye"||command=="bye"){
						if(input1==""&&input2==""){
							return copysubcommand ;
						}
						else{
							cout << "No inputs with bye/Bye" << endl ;
						}
					}
					else{
						cout << "Incorrect subcommand" << endl ;
						continue;
					}
				}
			}
				
			cout << endl ;

			
			return "" ;
		}
		void displayRecvData(string recvData){
			cout << recvData << endl ;
		}

		void closeClientConnections(){
			close(sockfd);
			exit(0);
		}
};


int main () {
 	string recvdata,credentials , commands;
	Client client;
	credentials=client.getCredentials() ;
	client.serverConnect() ;
	client.sendData(credentials);
	recvdata=client.recvData();
	client.displayRecvData(recvdata) ; 
//	cin.clear();
//	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	while(1){
		commands=client.getUserCommands(recvdata) ;
		if( commands != ""){
			client.sendData(commands);
			recvdata=client.recvData();
			client.displayRecvData(recvdata);
		}
		else{
			cout << "Enter correct subcommand from main " << endl ;
		}
				
	}

}


