#include<iostream>
#include<stdlib.h> //for exit()
using namespace std;

//head files of Linux
#include<netinet/in.h>
#include<unistd.h>   //for fork and read
#include<sys/types.h>   //for socket
#include<sys/socket.h>  //for socket
#include<string.h> // for bzero
#include<arpa/inet.h>

#include<map>
#include<algorithm>


void server()
{
    const unsigned short SERVERPORT = 53556;
    const int BACKLOG = 10; //10 个最大的连接数
    const int MAXSIZE = 1024;
    int sock, client_fd;
    sockaddr_in myAddr;
    sockaddr_in remoteAddr;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    //create socket
    if( sock == -1)
    {
        cerr<<"socket create fail!"<<endl;
        exit(1);
    }

    cout<<"sock :"<<sock<<endl;

    //bind
    myAddr.sin_family = AF_INET;
    myAddr.sin_port = htons(SERVERPORT);
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
        unsigned int sin_size = sizeof(sockaddr_in);
        if( (client_fd = accept(sock, (sockaddr*)(&remoteAddr), &sin_size)) ==-1 )
        {
            cerr<<"accept error!"<<endl;
            continue;
        }
        cout<<"Received a connection from "<<static_cast<char*>(inet_ntoa(remoteAddr.sin_addr) )<<endl;

        //子线程
        if(!fork() )
        {
            int rval;
            char buf[MAXSIZE];
            memset(buf,'\0',MAXSIZE);
            if( (rval = read(client_fd, buf, MAXSIZE) ) <0)
            {
                cout<<"Reading stream error!\n";
                continue;
            }
            cout<<sizeof(buf)<<endl;
            cout<<buf<<endl;
            string key,value;
            map<string,string> request;
            map<string,string>::iterator iter;
            bool space_1=true;
            for(char *c,i=0;i<MAXSIZE;i++){
                if(buf[i]!=' '&&space_1){
                    key+=buf[i];
                }
                else if(buf[i]=' '&&space_1){
                    space_1=false;
                }
                else if(buf[i]!='\n'&&(!space_1)){
                    value+=buf[i];
                }
                else{
                    space_1=true;
                    key="";
                    value="";
                    request.insert(pair<string,string>(key,value));
                }
            }
            cout<<"get value is:"<<request["GET"]<<endl;
            cout<<"iter get :"<<request.begin()->first<<endl;
            //向客户端发送信息
            // const char* msg = "Hello, I am xiaojian. You are connected !";
            // if( send(client_fd, const_cast<char*>(msg), strlen(msg), 0) == -1)
            //     cerr<<"send error!"<<endl;
            close(client_fd);
            exit(0);
        }
    }
}       


int main()
{
    server();
    
}