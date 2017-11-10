#ifndef MSERVER_H
#define MSERVER_H

#include<string>
#include<map>
#include<sys/types.h>   //for socket
#include<sys/socket.h>  //for socket
#include<netinet/in.h>

using namespace std;

class Mserver {
private:
    int sock,client_fd;
    unsigned int port;
    sockaddr_in myAddr;
    sockaddr_in remoteAddr;
public:
    static string path;
    Mserver(unsigned int port=8080);
    unsigned int getPort();
    unsigned int setPort(unsigned int );
    void method_Get();
    void method_Post();
    void run();
    void shutdown();
    static string getPath();
    static string setPath(const char *);
    static void res_404(int);
    static void res_500(int);
    static void* mthread(void*);
    static map<string,string> parseReq(const char *,unsigned int);

};





#endif