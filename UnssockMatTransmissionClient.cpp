#include "UnssockMatTransmissionClient.h"

UnssockMatTransmissionClient::UnssockMatTransmissionClient(void)
{
}

UnssockMatTransmissionClient::~UnssockMatTransmissionClient(void)
{
}

int UnssockMatTransmissionClient::socketConnect(const char* IP, int PORT)
{
sockClient = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
	servaddr.sin_port = htons(SERV_PORT);

	connect(sockClient, (struct sockaddr *)&servaddr, sizeof(servaddr));
return sockClient;
}

void UnssockMatTransmissionClient::socketDisconnect(void)
{
    close(sockClient);
}


int UnssockMatTransmissionClient::transmit(cv::Mat image)
{
    if (image.empty())
    {
        printf("empty image\n\n");
        return -1;
    }
 
    if(image.cols != IMG_WIDTH || image.rows != IMG_HEIGHT || image.type() != CV_8UC3)
    {
	printf("image.cols=%d,image.rows=%d",image.cols,image.rows);
        printf("the image must satisfy : cols == IMG_WIDTH（%d）  rows == IMG_HEIGHT（%d） type == CV_8UC3\n\n", IMG_WIDTH, IMG_HEIGHT);
        return -1;
    }
 
    for(int k = 0; k < 32; k++) 
    {
        int num1 = IMG_HEIGHT / 32 * k;
        for (int i = 0; i < IMG_HEIGHT / 32; i++)
        {
int num2 = i * IMG_WIDTH * 3;
            uchar* ucdata = image.ptr<uchar>(i + num1);
            for (int j = 0; j < IMG_WIDTH * 3; j++)
            {
                data.buf[num2 + j] = ucdata[j];
            }
        }
 
        if(k == 31)
            data.flag = 2;
        else
            data.flag = 1;
 
        if (send(sockClient, (char *)(&data), sizeof(data), 0) < 0)
        {
            printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
            return -1;
        }
    }
}
