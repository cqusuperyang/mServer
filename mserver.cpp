#include<iostream>
#include<fstream>
#include<stdlib.h> //for exit()

//head files of Linux
#include<netinet/in.h>
#include<unistd.h>   //for fork and read
#include<sys/types.h>   //for socket
#include<sys/socket.h>  //for socket
#include<string.h> // for bzero
#include<arpa/inet.h>

#include<map>
#include<algorithm>
#include<pthread.h>
#include "mserver.h"

using namespace std;

Mserver::Mserver(string directory,unsigned int port){
    this->path=directory;
    this->port=port;
}

string Mserver::getPath(){
    return path;
}

string Mserver::setPath(const char * direc){
    path=direc;
    return path;
}

unsigned int Mserver::getPort(){
    return port;
}

unsigned int Mserver::setPort(unsigned int p){
    port=p;
    return port;
}

void Mserver::parseReq(const char* data,unsigned int MAXSIZE=1024){
    string key,value;
    bool space_1=true,line_1=true;
    for(int i=0;i<MAXSIZE;i++){
        if(data[i]!='\n'&&line_1){
                key+=data[i];
        }
        else if(data[i]=='\n'&&line_1){
            line_1=false;
            int pos1,pos2;
            pos1=key.find(' ',0);
            pos2=key.find(' ',pos1+1);
            request.insert(pair<string,string>("Method",key.substr(0,pos1)));
            request.insert(pair<string,string>("Path",key.substr(pos1+1,pos2-pos1)));
            request.insert(pair<string,string>("Protocol",key.substr(pos2+1,key.size()-pos2)));
            key="";
        }

        else if((data[i]!=':')&&space_1){
            key+=data[i];
        }
        else if((data[i]==':')&&space_1){
            space_1=false;
            i++;//跳过空格
        }
        else if((data[i]!='\n')&&(!space_1)){
            value+=data[i];
        }
        else{
            space_1=true;
            request.insert(pair<string,string>(key,value));
            key="";
            value="";
        }
    }
}
void Mserver::mthread(int client_fd){
    pthread_detach(pthread_self());

        int rval;
        const int MAXSIZE=1024;
        char buf[MAXSIZE];
        memset(buf,'\0',MAXSIZE);
        if( (rval = read(client_fd, buf, MAXSIZE) ) <0)
        {
            cerr<<"Reading stream error!\n";
            ::close(client_fd);           
        }
               
        string header="HTTP/1.1 200 OK\r\nConnection: Keep-Alive\r\n\r\n";
        fstream file1;
        string filepath;
        filepath=path+"."+request["Path"];
        file1.open(filepath,ios::binary|ios::in);
         if( send(client_fd, const_cast<char*>(header.c_str()), header.size(), 0) == -1)
         cerr<<"send error!"<<endl;
         if(file1.fail()){

             const char* msg = "404,not find!";
             if( send(client_fd, const_cast<char*>(msg), strlen(msg), 0) == -1)
                 cerr<<"send error!"<<endl;
                 ::close(client_fd);
         }
        else{
            char * buffer=new char[MAXSIZE];
            do{
                file1.read(buffer,MAXSIZE);
                cout<<"buffer real size:"<<strlen(buffer)<<endl;
                if(send(client_fd,const_cast<char*>(buffer),MAXSIZE,0)==-1){
                    cerr<<"send error!"<<endl;
                    ::close(client_fd);                        
                }                   
            }while(!file1.eof());
            delete []buffer;
            file1.close();
        }      
      ::close(client_fd);
      pthread_exit(0);

}
void Mserver::run(){
    const int BACKLOG = 10; //10 个最大的连接数
    const int MAXSIZE = 1024;
    int  client_fd;
    pthread_t newthread;
     sock = socket(AF_INET, SOCK_STREAM, 0);
    if( sock == -1)
    {
        cerr<<"socket create fail!"<<endl;
        exit(1);
    }
    myAddr.sin_family = AF_INET;
    myAddr.sin_port = htons(port);
    myAddr.sin_addr.s_addr = INADDR_ANY;
    bzero( &(myAddr.sin_zero), 8);
    if(bind(sock, (sockaddr*)(&myAddr), sizeof(sockaddr)) ==-1 )
    {
        cerr<<"bind error!"<<endl;
        exit(1);
    }

    //listen
    if(listen(sock, BACKLOG) == -1)
    {
        cerr<<"listen error"<<endl;
        exit(1);
    }

    while(true)
    {
        
        int client_fd;
        
        unsigned int sin_size = sizeof(sockaddr_in);
        if( (client_fd = accept(sock, (sockaddr*)(&remoteAddr), &sin_size)) ==-1 )
        {
            cerr<<"accept error!"<<endl;
            continue;
        }

        if(pthread_create(&newthread,NULL,mthread,client_fd)!=0){
            cerr<<"pthread_create failed"<<endl;
        }
    }
   
}       

void Mserver::shutdown(){
    ::close(sock);
}
