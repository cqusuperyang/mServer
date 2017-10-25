#include<iostream>
#include<string>
#include<map>

using namespace std;

int main(){
    map<string,string> req;
    string a="abc";
    cout<<a.size()<<endl;
    req.insert(pair<string,string>("key","value"));
    cout<<req["key"]<<endl;
    return 0;
}