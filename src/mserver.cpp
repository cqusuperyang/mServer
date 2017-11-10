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

string Mserver::path=".";

Mserver::Mserver(unsigned int port) {
    this->port=port;
}

string Mserver::getPath() {
    return path;
}

string Mserver::setPath(const char * direc) {
    path=direc;
    return direc;
}

unsigned int Mserver::getPort() {
    return this->port;
}

unsigned int Mserver::setPort(unsigned int p) {
    this->port=p;
    return p;
}

map<string,string> Mserver::parseReq(const char* data,unsigned int MAXSIZE=1024) {
    string key,value;
    map<string,string> request;
    bool space_1=true,line_1=true;
    for(int i=0; i<MAXSIZE; i++) {
        if(data[i]!='\n'&&line_1) {
            key+=data[i];
        }
        else if(data[i]=='\n'&&line_1) {
            line_1=false;
            int pos1,pos2;
            pos1=key.find(' ',0);
            pos2=key.find(' ',pos1+1);
            request.insert(pair<string,string>("Method",key.substr(0,pos1)));
            request.insert(pair<string,string>("Path",key.substr(pos1+1,pos2-pos1-1)));
            request.insert(pair<string,string>("Protocol",key.substr(pos2+1,key.size()-pos2-1)));
            key="";
        }
        else if((data[i]!=':')&&space_1) {
            key+=data[i];
        }
        else if((data[i]==':')&&space_1) {
            space_1=false;
            i++;//跳过空格
        }
        else if((data[i]!='\n')&&(!space_1)) {
            value+=data[i];
        }
        else {
            space_1=true;
            request.insert(pair<string,string>(key,value));
            key="";
            value="";
        }
    }
    return request;
}
void* Mserver::mthread(void *client_p) {
    pthread_detach(pthread_self());
    int client=*(int *) client_p;
    int rval;
    const int MAXSIZE=1024;
    char buf[MAXSIZE];
    string header;
    fstream file;
    string filepath;
    map<string,string> request;
    memset(buf,'\0',MAXSIZE);
    if( (rval = read(client, buf, MAXSIZE) ) <0) {
        cerr<<"Reading stream error!\n";
        perror("reading error");
        //res_500(client);
    }
    else {
        int flag;
        request=parseReq(buf,1024);
        filepath=path+(request["Path"].compare("/")==0?"/index.html":request["Path"]);
        cout<<"path visit:"<<filepath<<endl;
        file.open(filepath.c_str(),ios::binary|ios::in);
        flag=file.fail();
        memset(buf,'\0',MAXSIZE);
        file.read(buf,MAXSIZE);
        if(flag||(file.gcount()==0)) {
            res_404(client);
        } else {
            header="HTTP/1.1 200 OK\r\nServer: Mserver\r\n\r\n";
            if( send(client, const_cast<char*>(header.c_str()), header.size(), 0) == -1) {
                cerr<<"send3 error!"<<endl;
            }
            do {
                if(send(client,const_cast<char*>(buf),file.gcount(),0)==-1) {
                    cerr<<"send4 error!"<<endl;
                    break;
                }
                file.read(buf,MAXSIZE);
            } while(!file.eof());
        }
        file.close();
    }
    ::close(client);
    delete client_p;
    pthread_exit(0);

}
void Mserver::run() {
    const int BACKLOG =500; //10 个最大的连接数
    const int MAXSIZE = 1024;
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

    unsigned int sin_size = sizeof(sockaddr_in);
    while(true)
    {

        if( (client_fd = accept(sock, (sockaddr*)(&remoteAddr), &sin_size)) ==-1 )
        {
            cerr<<"accept error!"<<endl;
            perror("accept");
            continue
            ;
        }
        int *client=new int(client_fd);

        if(pthread_create(&newthread,NULL,mthread,client)!=0) {
            cerr<<"pthread_create failed"<<endl;
        }
    }

}

void Mserver::shutdown() {
    ::close(sock);
}

void Mserver::res_404(int client) {
    const char* msg = "404,not find!";
    string header="HTTP/1.1 404 BAD\r\nServer: Mserverl\r\n\r\n";
    if( send(client, const_cast<char*>(header.c_str()), header.size(), 0) == -1)
        cerr<<"send 404 error!"<<endl;
    if( send(client, const_cast<char*>(msg), strlen(msg), 0) == -1)
        cerr<<"send 404 error!"<<endl;
}

void Mserver::res_500(int client) {
    const char* msg = "500,server error!";
    string header="HTTP/1.1 500 BAD\r\nServer: Mserverl\r\n\r\n";
    if( send(client, const_cast<char*>(header.c_str()), header.size(), 0) == -1)
        cerr<<"send 505 error!"<<endl;
    if( send(client, const_cast<char*>(msg), strlen(msg), 0) == -1)
        cerr<<"send 505 error!"<<endl;
}
