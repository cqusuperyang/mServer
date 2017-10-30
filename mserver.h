#ifndef MSERVER_H
#define MSERVER_H
#include<string>

class Mserver{
private:
    string path;
    unsigned int port;
    map<string,string> res;
    map<string,string> req;
public:
    Mserver(const char *,unsigned int);
    string getPath();
    string setPath(const char *);
    unsigned int getPort();
    unsigned int setPort(unsigned int );
    void parseReq(const char *);
    void run();
};




#endif