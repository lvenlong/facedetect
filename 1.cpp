#include <muduo/base/Logging.h>
#include <muduo/base/Timestamp.h>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpConnection.h>
#include<opencv2/opencv.hpp>

using namespace muduo;
using namespace muduo::net;

//待传输图像默认大小为 640*480，可修改
#define IMG_WIDTH 640   // 需传输图像的宽
#define IMG_HEIGHT 480  // 需传输图像的高
//默认格式为CV_8UC3
#define BUFFER_SIZE IMG_WIDTH*IMG_HEIGHT*3/32
#define SERV_PORT 6666

int pos = 0;
int count = 0;

struct recvbuf
{
	char buf[BUFFER_SIZE];
	int flag;
};

void onConnection(const TcpConnectionPtr &conn)
{
	LOG_INFO << "EchoServer - " << conn->peerAddress().toIpPort() << " -> "
		<< conn->localAddress().toIpPort() << " is "
		<< (conn->connected() ? "UP" : "DOWN");
}

cv::Mat img(IMG_HEIGHT, IMG_WIDTH, CV_8UC3, cv::Scalar(0));
void onMessage(const TcpConnectionPtr &conn,
	Buffer *buf,
	Timestamp time)
{

	int needRecv = sizeof(recvbuf);
	int i = 0;
	int len0 = buf->readableBytes();
	
	if (pos < needRecv)
	{
		//count = count + data.flag;

		int num1 = IMG_HEIGHT / 32 * i;
		for (int j = 0; j < IMG_HEIGHT / 32; j++)
		{
			int num2 = j * IMG_WIDTH * 3;
			uchar* ucdata = img.ptr<uchar>(j + num1);
			for (int k = 0; k < IMG_WIDTH * 3; k++)
			{
				ucdata[k] = *(buf->peek() + k);
			}
		}
		pos += len0;
		i++;
	}
	else
	{
		i = 0;
		pos = 0;
		cv::imshow("", img);
		cv::waitKey(30);
	}
}
int main(int argc, const char *argv[])
{
    EventLoop loop;
    InetAddress addr("127.0.0.1", 6666);
    TcpServer server(&loop, addr, "echo");
    server.setConnectionCallback(&onConnection);
    server.setMessageCallback(&onMessage);
    server.start();
    loop.loop();
    return 0;
}


