# mServer
要求可以访问指定目录静态文件 文本和图片

## 背景资料
1. [HTTP请求报文格式及HTTP响应报文格式](http://blog.csdn.net/a19881029/article/details/14002273)
2. [web服务器与浏览器的实现原理](http://blog.csdn.net/changhenshui1990/article/details/70052991)

## 知识点
1. socket 使用
2. 一切皆文件
3. 不同类型文件传输
4. 可以考虑线程使用

## 编译及运行
1. 在当前目录下输入：make
2. 编译成功后会在当前目录下生成start文件
3. 输入：./start [path] [port]
   >其中:path,port可以不写，默认为.和8080

## Environment
>Linux version 3.10.0-514.el7.x86_64 (builder@kbuilder.dev.centos.org) (gcc version 4.8.5 20150623 (Red Hat 4.8.5-11) (GCC) ) #1 SMP Tue Nov 22 16:42:41 UTC 2016

## Webbench test
 > Test result

``` [root@localhost webbench-1.5]# webbench -c 300 -t 60 http://127.0.0.1/index.html
Webbench - Simple Web Benchmark 1.5
Copyright (c) Radim Kolar 1997-2004, GPL Open Source Software.

Benchmarking: GET http://127.0.0.1/index.html
300 clients, running 60 sec.

Speed=93110 pages/min, 572650 bytes/sec.
Requests: 93110 susceed, 0 failed.
[root@localhost webbench-1.5]# webbench -c 1000 -t 60 http://127.0.0.1/index.html
Webbench - Simple Web Benchmark 1.5
Copyright (c) Radim Kolar 1997-2004, GPL Open Source Software.

Benchmarking: GET http://127.0.0.1/index.html
1000 clients, running 60 sec.

Speed=3147 pages/min, 18450 bytes/sec.
Requests: 2992 susceed, 155 fail ```