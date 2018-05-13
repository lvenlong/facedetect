#include "UnssockMatTransmissionServer.h"

UnssockMatTransmissionServer::UnssockMatTransmissionServer(void)
{
}

UnssockMatTransmissionServer::~UnssockMatTransmissionServer(void)
{
}

int UnssockMatTransmissionServer::socketConnect(int PORT)
{
int listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	listen(listenfd, 20);
cliaddr_len = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);

return listenfd;
}



void UnssockMatTransmissionServer::socketDisconnect(void)
{
    close(connfd);
}


int UnssockMatTransmissionServer::receive(cv::Mat& image)
{
    cv::Mat img(IMG_HEIGHT,IMG_WIDTH,CV_8UC3,cv::Scalar(0));
 
    int needRecv = sizeof(recvbuf);
    int count = 0;
 
    while (1)
    {
        for (int i = 0; i < 32; i++)
        {
            int pos = 0;
            int len0 = 0;
 
            while (pos < needRecv)
            {
                len0 = recv(connfd, (char*)(&data) + pos, needRecv - pos, 0);
                if (len0 < 0)
                {
                    printf("Server Recieve Data Failed!\n");
                    return -1;
                }
                pos += len0;
            }
 
            count = count + data.flag;
 
            int num1 = IMG_HEIGHT / 32 * i;
            for (int j = 0; j < IMG_HEIGHT / 32; j++)
            {
                int num2 = j * IMG_WIDTH * 3;
                uchar* ucdata = img.ptr<uchar>(j + num1);
                for (int k = 0; k < IMG_WIDTH * 3; k++)
                {
                    ucdata[k] = data.buf[num2 + k];
                }
            }
 
            if (data.flag == 2)
            {
                if (count == 33)
                {
                    image = img;
                    return 1;
                    count = 0;
                }
                else
                {
                    count = 0;
                    i = 0;
                }
            }
        }
    }
}
