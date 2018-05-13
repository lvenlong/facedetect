#include<opencv2/opencv.hpp>
#include<iostream>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include<errno.h>

using namespace std;
using namespace cv;

//待传输图像默认大小为 640*480，可修改
#define IMG_WIDTH 640   // 需传输图像的宽
#define IMG_HEIGHT 480  // 需传输图像的高
//默认格式为CV_8UC3
#define BUFFER_SIZE IMG_WIDTH*IMG_HEIGHT*3/32
#define SERV_PORT 6666
 
struct recvbuf
{
    char buf[BUFFER_SIZE];
    int flag;
};

class UnssockMatTransmissionServer
{
public:
	UnssockMatTransmissionServer();
	~UnssockMatTransmissionServer();
private:
	int sockClient,err,n;
	struct recvbuf data;
struct sockaddr_in servaddr, cliaddr;
	socklen_t cliaddr_len;
	int listenfd, connfd;

public:
 
    // 打开socket连接
    // params : PORT    传输端口
    // return : -1      连接失败
    //          1       连接成功
    int socketConnect(int PORT);
 
 
// 传输图像
    // params : image   待接收图像
    // return : -1      接收失败
    //          1       接收成功
    int receive(cv::Mat& image);
 
 
    // 断开socket连接
    void socketDisconnect(void);
};

