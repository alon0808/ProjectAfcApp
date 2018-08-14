 
#ifndef _MY_HTTP_H  
#define _MY_HTTP_H  



//ÑùÀý
// POST /mjc/webtrans/VPB_lb HTTP/1.1
// HOST: 172.17.236.60:60000
// User-Agent: Donjin Http 0.1
// Cache-Control: no-cache
// Content-Type: application/x-www-form-urlencoded;charset=UTF-8
// Accept: */*
// Content-Length: 93
// 
// 
// 00 5B 60 00 03 00 00 60 31 00 31 13 12 08 00 00 20 00 00 00 c0 00 16 00 00 01 31 30 30 30 30 31 35 39 38 38 30 32 31 30 30 31 30 32 31 30 31 36 30 00 11 00 00 00 01 00 30 00 29 53 65 71 75 65 6e 63 65 20 4e 6f 31 36 33 31 35 30 53 58 58 2d 34 43 33 30 34 31 31 39 00 03 30 31 20


// POST /mjc/webtrans/VPB_lb HTTP/1.1
// HOST: 145.4.206.244:5000
// User-Agent: Donjin Http 0.1
// Cache-Control: no-cache
// Content-Type:x-ISO-TPDU/x-auth
// Accept: */*
// Content-Length: 62


#define HTTP_POST "POST /mjc/webtrans/VPB_lb HTTP/1.1\r\nHOST: %s:%d\r\nUser-Agent: Donjin Http 0.1\r\nCache-Control: no-cache\r\nContent-Type:x-ISO-TPDU/x-auth\r\nAccept: */*\r\nContent-Length: %d\r\n\r\n"
//#define HTTP_POST "POST /mjc/webtrans/VPB_lb HTTP/1.1\r\nHOST: 101.231.114.200:5000\r\nUser-Agent: Donjin Http 0.1\r\nCache-Control: no-cache\r\nContent-Type:x-ISO-TPDU/x-auth\r\nAccept: */*\r\nContent-Length: %d\r\n\r\n"
//#define HTTP_POST "POST /mjc/webtrans/VPB_lb HTTP/1.1\r\nHOST: 145.4.206.244:5000\r\nUser-Agent: Donjin Http 0.1\r\nCache-Control: no-cache\r\nContent-Type:x-ISO-TPDU/x-auth\r\nAccept: */*\r\nContent-Length: %d\r\n\r\n"

#define HTTP_GET "GET /%s HTTP/1.1\r\nHOST: %s:%d\r\n\r\n"
//Range: bytes=0-801  
#define HTTP_GET_INDEX "GET /%s HTTP/1.1\r\nHOST: %s:%d\r\nRange: bytes=%d-%d\r\n\r\n"

#endif
