#include <iostream>  
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#ifdef linux  
#include <unistd.h>  
#include <dirent.h>  
#endif  
#ifdef WIN32  
#include <direct.h>  
#include <io.h>  
#endif 

using namespace std;

int main(){
    cout<<"vscode success"<<endl;
   // cout<<i<<endl;
   system("ls");
   char buf[200];
   memset(buf,'\0',200);
   getcwd(buf,200);
   cout<<buf<<endl;
 
    return 0;
}