#include "UnssockMatTransmissionServer.h"
 
int main()
{
    UnssockMatTransmissionServer socketMat;
    if (socketMat.socketConnect(6666) < 0)
    {
        return 0;
    }
 
    cv::Mat image;
    while (1)
    {
        if(socketMat.receive(image) > 0)
        {
            cv::imshow("",image);
            cv::waitKey(30);
        }
    }
 
    socketMat.socketDisconnect();
    return 0;
}
