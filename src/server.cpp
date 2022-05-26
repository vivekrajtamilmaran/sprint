//Function to create bind and listen socket
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <string>
#include <string.h>
#include <arpa/inet.h>
#include <vector>
#include <sys/ipc.h>
#include <fstream>
#include <sstream>
#define ADDRSERV "0.0.0.0"
#define MAX 256
#define PORT 8028
using namespace std;

class User{
        string username ,password ;
	vector<string> group;
        public:
                User(string username ,string password){
                        this->username = username;
                        this->password = password;
                }
                User(string username ,string password ,vector<string>group){
                        this->username = username;
                        this->password = password;
			for(auto i: group){
				this->group.push_back(i);
			}
                }
		


                bool  authenticate(User user){
			if(this->username == user.username && this->password == user.password ){
				return true ;
			}
			else{
				return false ;
			}
		}
};
class Server{
        int sockfd ;
        sockaddr_in servaddr ;
        sockaddr_in cliaddr ;
        vector <User> users ;
        int slen = 0 ,connectfd = 0, mlen = 0 ;
	pid_t pid ;
        public:
                Server(){
		fstream inputfile;
		string line;
		inputfile.open("data");
		if(inputfile.is_open()){
			while(getline(inputfile,line)){
				stringstream ss(line);
				string username;
				string password;
				string group;
				vector<string> groups;
				getline(ss,username,':');
				getline(ss,password,':');
				while(getline(ss,group,':')){
					groups.push_back(group);
				}
				if(username!="" && password != "" ){
					users.push_back(User(username,password,groups));
				}
			}
		}
		else{
			cout << "No file existing " << endl ;
		}
                        slen=sizeof(sockaddr_in);
                        memset(&servaddr,0,slen);
                        memset(&cliaddr,0,slen);

                        servaddr.sin_family = AF_INET; //To use Internet addressing family
                        servaddr.sin_addr.s_addr = inet_addr(ADDRSERV);
                        servaddr.sin_port = htons(PORT);


                        sockfd=socket(AF_INET, SOCK_STREAM, 0);
                        if(sockfd < 0){
                                perror("Socket creation has not been done");
                                exit(EXIT_FAILURE);
                          }
                        int ret;
                        ret=bind(sockfd, (struct sockaddr *)&servaddr, slen);
                        if(ret < 0){
                                fputs("Binding has not been done",stderr);
                                exit(EXIT_FAILURE);
                        }
                        ret=listen(sockfd, 5);
                        if(ret< 0){
                                fputs("Socket is not Listening",stderr);
                                exit(EXIT_FAILURE);
                        }
                }

                int acceptConnections(){
			while(1){
                        	connectfd=accept(sockfd,(struct sockaddr*)&servaddr,(socklen_t *)&slen);
                        	if(connectfd < 0){
                        	fputs("Conncetion is not established",stderr);
                        	exit(EXIT_FAILURE);
                        	}
                        	pid = fork();
				if (pid == 0){ 
					string type =authenticateUser();
					string type1 ;
					if(type != "admin" &&type != ""){
						type1 = "authenticated user" ;
					}
					else if(type ==""){
						type1 ="anonymous user" ;
					}
                        		mlen=send(connectfd,type1.c_str(),strlen(type1.c_str()),0);
					while(1){
						string recvdata = recvData();
						if(type1==admin)
						cout << recvdata << endl ;
                        		      //  mlen=send(connectfd,type.c_str(),strlen(type.c_str()),0);
					}

                                }


                	}
			return mlen;
		}

                string recvData(){
                        	char recvdata[1024] ;
				recv(connectfd , recvdata,1024 , 0) ;
                               	if(strcmp(recvdata,"bye")==0 || strcmp(recvdata,"Bye")==0){
                                  	close(connectfd);
                                      	cout << "Client left" << endl;
                                      	exit(0);
				}
				return string(recvdata) ;
		}

		string authenticateUser(){
		        char  receivedata[1024];
			memset(receivedata,0,1024);
			string username ,password ;
			recv(connectfd ,receivedata,1024 , 0) ;
			string recvdata=string(receivedata);
			stringstream ss(recvdata);
			getline(ss ,username , ':');
			getline(ss , password, ':');
			for (auto i : users){
				if(i.authenticate(User(username , password))){
					return username;
				}
			}
			return "";
		}

};


int main(){
	Server server;
	server.acceptConnections() ;
}

