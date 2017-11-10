#include<iostream>
#include<fstream>
#include<stdlib.h> //for exit()

//head files of Linux
#include<netinet/in.h>
#include<unistd.h>   //for fork and read
#include<sys/types.h>   //for socket
#include<sys/socket.h>  //for socket
#include<string.h> // for bzero
#include<arpa/inet.h>
#include <sys/stat.h> 

#include<map>
#include<algorithm>
#include<pthread.h>
#include "mserver.h"

using namespace std;

string Mserver::path=".";

Mserver::Mserver(unsigned int port) {
    this->port=port;
}

string Mserver::getPath() {
    return path;
}

string Mserver::setPath(const char * direc) {
    path=direc;
    return direc;
}

unsigned int Mserver::getPort() {
    return this->port;
}

unsigned int Mserver::setPort(unsigned int p) {
    this->port=p;
    return p;
}

map<string,string> Mserver::parseReq(const char* data,unsigned int MAXSIZE=1024) {
    string key,value;
    map<string,string> request;
    bool space_1=true,line_1=true;
    for(int i=0; i<MAXSIZE; i++) {
        if(data[i]!='\n'&&line_1) {
            key+=data[i];
        }
        else if(data[i]=='\n'&&line_1) {
            line_1=false;
            int pos1,pos2;
            pos1=key.find(' ',0);
            pos2=key.find(' ',pos1+1);
            request.insert(pair<string,string>("Method",key.substr(0,pos1)));
            request.insert(pair<string,string>("Path",key.substr(pos1+1,pos2-pos1-1)));
            request.insert(pair<string,string>("Protocol",key.substr(pos2+1,key.size()-pos2-1)));
            key="";
        }
        else if((data[i]!=':')&&space_1) {
            key+=data[i];
        }
        else if((data[i]==':')&&space_1) {
            space_1=false;
            i++;//跳过空格
        }
        else if((data[i]!='\n')&&(!space_1)) {
            value+=data[i];
        }
        else {
            space_1=true;
            request.insert(pair<string,string>(key,value));
            key="";
            value="";
        }
    }
    return request;
}

void* Mserver::mthread(void *client_p) {
    pthread_detach(pthread_self());
    int client=*(int *) client_p;
    int rval;
    const int MAXSIZE=1024;
    char buf[MAXSIZE];
    string header;
    fstream file;
    string filepath;
    string file_type;
    map<string,string> request;
    memset(buf,'\0',MAXSIZE);
    if( (rval = read(client, buf, MAXSIZE) ) <0) {
        cerr<<"Reading stream error!\n";
        perror("reading error");
        //res_500(client);
    }
    else {
        int flag;
        request=parseReq(buf,1024);
        filepath=path+(request["Path"].compare("/")==0?"/index.html":request["Path"]);
        //如果是目录请求，则禁止
        struct stat info;
        stat(filepath.c_str(), &info);
        if((filepath.substr(filepath.length() - 1 , 1) == "/") || S_ISDIR(info.st_mode)) {
            res_403(client);
        }else{
            cout<<"path visit:"<<filepath<<endl;
            file_type = get_file_type(filepath);
            unsigned int file_size = get_file_size(filepath.c_str());
            cout<<file_size<<endl;
            file.open(filepath.c_str(),ios::binary|ios::in);
            flag=file.fail();
            memset(buf,'\0',MAXSIZE);
            if(flag) {
                cout<<flag<<" "<<file.gcount()<<endl;
                res_404(client);
            } else {
                header="HTTP/1.1 200 OK\r\nServer: Mserver\r\nContent-length: " + int2string(file_size) + "\r\nContent-type: " + file_type + "\r\n\r\n";
                
                if( send(client, const_cast<char*>(header.c_str()), header.size(), 0) == -1) {
                    cerr<<"send3 error!"<<client<<endl;
                    printf("errno=%d\n",errno);
                }

                while(!file.eof()){
                    file.read(buf,MAXSIZE);
                    if(send(client,const_cast<char*>(buf),file.gcount(),0)==-1) {
                        cerr<<"send4 error!"<<client<<endl;
                        printf("errno=%d\n",errno);
                        break;
                    }
                }
            }
            file.close();
        }
    }

    ::close(client);

    if (client_p != 0){
        delete (int *)client_p;
        client_p = 0;
    }
    
    pthread_exit(0);

}

void Mserver::run() {
    const int BACKLOG =500;
    const int MAXSIZE = 1024;
    pthread_t newthread;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if( sock == -1)
    {
        cerr<<"socket create fail!"<<endl;
        exit(1);
    }
    myAddr.sin_family = AF_INET;
    myAddr.sin_port = htons(port);
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

    unsigned int sin_size = sizeof(sockaddr_in);
    while(true)
    {

        if( (client_fd = accept(sock, (sockaddr*)(&remoteAddr), &sin_size)) ==-1 )
        {
            cerr<<"accept error!"<<endl;
            perror("accept");
            continue
            ;
        }
        cout<<"run::accept socket:"<<client_fd<<endl;
        int *client=new int(client_fd);

        if(pthread_create(&newthread,NULL,mthread,client)!=0) {
            cerr<<"pthread_create failed"<<endl;
        }
    }

}

void Mserver::shutdown() {
    if (sock >= 0){
        ::close(sock);
    }
}

Mserver::~Mserver() {
	//保证socket能够在异常情况下关闭
	if (sock >= 0){
		::close(sock);
	}
}

//403:禁止访问
void Mserver::res_403(int client) {
    const char* msg = "403,forbidden!";
    string header="HTTP/1.1 403 FORBIDDEN!\r\nServer: Mserverl\r\n\r\n";
    if( send(client, const_cast<char*>(header.c_str()), header.size(), 0) == -1)
        cerr<<"send 403 error!"<<endl;
    if( send(client, const_cast<char*>(msg), strlen(msg), 0) == -1)
        cerr<<"send 403 error!"<<endl;
}

//404：未找到资源
void Mserver::res_404(int client) {
    const char* msg = "404,not find!";
    string header="HTTP/1.1 404 BAD\r\nServer: Mserverl\r\n\r\n";
    if( send(client, const_cast<char*>(header.c_str()), header.size(), 0) == -1)
        cerr<<"send 404 error!"<<endl;
    if( send(client, const_cast<char*>(msg), strlen(msg), 0) == -1)
        cerr<<"send 404 error!"<<endl;
}

//500：服务器错误
void Mserver::res_500(int client) {
    const char* msg = "500,server error!";
    string header="HTTP/1.1 500 BAD\r\nServer: Mserverl\r\n\r\n";
    if( send(client, const_cast<char*>(header.c_str()), header.size(), 0) == -1)
        cerr<<"send 505 error!"<<endl;
    if( send(client, const_cast<char*>(msg), strlen(msg), 0) == -1)
        cerr<<"send 505 error!"<<endl;
}

//根据文件类型确定http response头中MIME类型
string Mserver::get_file_type(string filepath){
    string file_type = "text/plain";
	string suffixStr = filepath.substr(filepath.find_last_of('.') + 1);//获取文件后缀

    if(suffixStr == "html") file_type = "text/html";
    else if(suffixStr == "xml") file_type = "text/xml";
    else if(suffixStr == "gif") file_type = "image/gif";
    else if(suffixStr == "pdf") file_type = "application/pdf";
    else if(suffixStr == "jpg") file_type = "image/jpeg";
    else if(suffixStr == "png") file_type = "image/png";
    else if(suffixStr == "css") file_type = "text/css";
    else if(suffixStr == "js") file_type = "application/x-javascript";
    else if(suffixStr == "json") file_type = "application/json";
    else if(suffixStr == "gz") file_type = "application/x-gzip";

    return file_type;
}

//获取文件大小
unsigned long Mserver::get_file_size(const char *path)  
{  
  unsigned long filesize = -1;      
  struct stat statbuff;  
  if(stat(path, &statbuff) < 0){  
      return filesize;  
  }else{  
      filesize = statbuff.st_size;  
  }  
  return filesize;  
} 

// 将整数转换为string类型
string Mserver::int2string(unsigned int n){ 
	char ss[32];
	sprintf(ss, "%d", n); 
	std::string s(ss); 

	return s; 
} 