#ifndef MSERVER_H
#define MSERVER_H

#include<string>
#include<map>
#include<sys/types.h>   //for socket
#include<sys/socket.h>  //for socket
#include<netinet/in.h>
#include <errno.h>

using namespace std;

//用于获取系统函数调用出错时的错误码
extern int errno;

class Mserver {
private:
    int sock,client_fd;
    unsigned int port;
    sockaddr_in myAddr;
    sockaddr_in remoteAddr;

public:
    static string path;
    Mserver(unsigned int port=8080);
    ~Mserver();
    unsigned int getPort();
    unsigned int setPort(unsigned int );
    void method_Get();
    void method_Post();
    void run();
    void shutdown();
    static string getPath();
    static string setPath(const char *);
    static void res_403(int);
    static void res_404(int);
    static void res_500(int);
    static void* mthread(void*);
    static map<string,string> parseReq(const char *,unsigned int);
    static string get_file_type(string filepath);
	static unsigned long get_file_size(const char *path);
	static string int2string(unsigned int n);
};





#endif