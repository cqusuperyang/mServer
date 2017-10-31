// #include<iostream>
// #include<fstream>
// #include<stdlib.h> //for exit()
// using namespace std;

// //head files of Linux
// #include<netinet/in.h>
// #include<unistd.h>   //for fork and read
// #include<sys/types.h>   //for socket
// #include<sys/socket.h>  //for socket
// #include<string.h> // for bzero
// #include<arpa/inet.h>

// #include<map>
// #include<algorithm>


// void server(const char* path)
// {
//     const unsigned short SERVERPORT = 53556;
//     const int BACKLOG = 10; //10 个最大的连接数
//     const int MAXSIZE = 1024;
//     int sock, client_fd;
//     sockaddr_in myAddr;
//     sockaddr_in remoteAddr;
//     sock = socket(AF_INET, SOCK_STREAM, 0);
//     //create socket
//     if( sock == -1)
//     {
//         cerr<<"socket create fail!"<<endl;
//         exit(1);
//     }

//     cout<<"sock :"<<sock<<endl;

//     //bind
//     myAddr.sin_family = AF_INET;
//     myAddr.sin_port = htons(SERVERPORT);
//     myAddr.sin_addr.s_addr = INADDR_ANY;
//     bzero( &(myAddr.sin_zero), 8);
//     if(bind(sock, (sockaddr*)(&myAddr), sizeof(sockaddr)) ==-1 )
//     {
//         cerr<<"bind error!"<<endl;
//         exit(1);
//     }

//     //listen
//     if(listen(sock, BACKLOG) == -1)
//     {
//         cerr<<"listen error"<<endl;
//         exit(1);
//     }

//     while(true)
//     {
//         unsigned int sin_size = sizeof(sockaddr_in);
//         if( (client_fd = accept(sock, (sockaddr*)(&remoteAddr), &sin_size)) ==-1 )
//         {
//             cerr<<"accept error!"<<endl;
//             continue;
//         }
//         cout<<"Received a connection from "<<static_cast<char*>(inet_ntoa(remoteAddr.sin_addr) )<<endl;

//         //fork add new process to do same thing
        
//         //if(!fork() )
//        // {
//             int rval;
//             char buf[MAXSIZE];
//             memset(buf,'\0',MAXSIZE);
//             if( (rval = read(client_fd, buf, MAXSIZE) ) <0)
//             {
//                 cerr<<"Reading stream error!\n";
//                 continue;
//             }
//             // cout<<sizeof(buf)<<endl;
//             // cout<<buf<<endl;
//             string key,value;
//             map<string,string> request;
//             map<string,string>::iterator iter;
//             bool space_1=true,line_1=true;
//             for(int i=0;i<MAXSIZE;i++){
//                 if(buf[i]!='\n'&&line_1){
//                         key+=buf[i];
//                 }
//                 else if(buf[i]=='\n'&&line_1){
//                     line_1=false;
//                     int pos1,pos2;
//                     pos1=key.find(' ',0);
//                     pos2=key.find(' ',pos1+1);
//                     request.insert(pair<string,string>("METHOD",key.substr(0,pos1)));
//                     request.insert(pair<string,string>("PATH",key.substr(pos1+1,pos2-pos1)));
//                     request.insert(pair<string,string>("PROTOCOL",key.substr(pos2+1,key.size()-pos2)));
//                     key="";
//                 }
    
//                 else if((buf[i]!=':')&&space_1){
//                     key+=buf[i];
//                 }
//                 else if((buf[i]==':')&&space_1){
//                     space_1=false;
//                 }
//                 else if((buf[i]!='\n')&&(!space_1)){
//                     value+=buf[i];
//                 }
//                 else{
//                     space_1=true;
//                     request.insert(pair<string,string>(key,value));
//                     key="";
//                     value="";
//                 }
//             }
//             // for(iter=request.begin();iter!=request.end();iter++){
                
//             //     cout<<iter->first<<":"<<iter->second<<endl;
//             // }
//             // cout<<"write header: "<<endl;
//             //向客户端发送信息
//            // request["Accept"]=" image/png";
//             string header="HTTP/1.1 200 OK\r\nConnection: Keep-Alive\r\n\r\n";
//             // header+="Connection: Keep-Alive"+"\r\n\r\n";
//              cout<<"header:"<<header.c_str()<<endl;
//              fstream file1;
//              file1.open(path,ios::binary|ios::in);
//              if( send(client_fd, const_cast<char*>(header.c_str()), header.size(), 0) == -1)
//              cerr<<"send error!"<<endl;
//              if(file1.fail()){

//                  const char* msg = "404,not find!";
//                  if( send(client_fd, const_cast<char*>(msg), strlen(msg), 0) == -1)
//                      cerr<<"send error!"<<endl;
//                      close(client_fd);
//              }
//             else{
//                 char * buffer=new char[MAXSIZE];
//                 do{
//                     file1.read(buffer,MAXSIZE);
//                     cout<<"buffer real size:"<<strlen(buffer)<<endl;
//                     if(send(client_fd,const_cast<char*>(buffer),MAXSIZE,0)==-1){
//                         cerr<<"send error!"<<endl;
//                         close(client_fd);
//                         exit(-1);
//                     }
//                    //memset(buffer,'\0',MAXSIZE);
//                 }while(!file1.eof());
//                 delete []buffer;
//                 file1.close();
//             }      
//             //exit(0);
//        // }
//        close(client_fd);
//     }
//     close(sock);
// }       
#include<string>
#include "mserver.h"
using namespace std;

int main()
{
    Mserver server;
    server.run();
    server.shutdown();
    return 0;
}