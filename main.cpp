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