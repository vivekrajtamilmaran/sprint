//The program to create a test TCP server
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/wait.h>
#define ADDRSERV "127.0.0.1"
#define MAX 1024
#define PORT 8028
using namespace std;

int main(){
        struct sockaddr_in servaddr, cliaddr;
        char smsg[MAX];
        char cmsg[MAX];
        int sockfd , mlen , ret , slen ,connectfd;
        socklen_t clen;
        pid_t pid;

        slen=sizeof(sockaddr_in);
        memset(&servaddr,0,slen);
        memset(&cliaddr,0,slen);
        memset(cmsg,0,MAX);
        memset(smsg,0,MAX);

        servaddr.sin_family = AF_INET; //To use Internet addressing family
        servaddr.sin_addr.s_addr = inet_addr(ADDRSERV);
        servaddr.sin_port = htons(PORT);


        sockfd=socket(AF_INET, SOCK_STREAM, 0);
        if(sockfd < 0){
                fputs("Socket creation has not been done",stderr);
                exit(EXIT_FAILURE);
        }
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

        while(1){
                connectfd=accept(sockfd,(struct sockaddr*)&servaddr,(socklen_t *)&slen);
                if(connectfd < 0){
                        fputs("Conncetion is not established",stderr);
                        exit(EXIT_FAILURE);
                }
                pid = fork();
                if(pid==0){
                        close(sockfd);
                        while(1){
        			memset(cmsg,0,MAX);
                                mlen = read(connectfd,cmsg,MAX);
                                if(mlen==0){
                                        close(connectfd);
                                        cout << "Client connection Closed "<< endl;
                                        break;
                                }
                                mlen=write(1,"The message received from client :",35);
                                mlen=write(1,cmsg,strlen(cmsg));
                                mlen=write(1,"\n",1);
                               // mlen=write(1,"Enter the message from server to client :",42);
                //              //cin.getline(smsg,MAX);
                               // mlen=read(0,smsg,MAX);
                                mlen=write(connectfd,"hi",strlen("hi"));
                                cout << "Message sent from the server " << endl;
                        }
                                close(connectfd);
                                exit(EXIT_SUCCESS);
                }
//              
                close(connectfd);
        }
}

