#include<string>
#include<stdlib.h>
#include<iostream>
#include "mserver.h"
using namespace std;

int main(int argc,char *argv[])
{
    Mserver server;
    if(3==argc){
        server.setPath(argv[1]);
        server.setPort(atoi(argv[2]));       
    }
    cout<<"the server run at diretory "<<server.getPath()<<endl;
    cout<<"the server run at port:"<<server.getPort()<<endl;
    server.run();
    server.shutdown();
    return 0;
}