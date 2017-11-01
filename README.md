# mServer
要求可以访问指定目录静态文件 文本和图片
## 知识点
1. socket 使用
2. 一切皆文件
3. 不同类型文件传输
4. 可以考虑线程使用

## webbench test
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