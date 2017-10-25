#ifndef MSERVER_H
#define MSERVER_H

#include<string>

class Mserver{
private:
    string path;
    int port;
    Response res;
    Request req;
public:
    Mserver();
    string getPath();
    string setPath(string path);
    int getPort();
    int setPort(int port);
    void run();
};




#endif