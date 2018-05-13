#include "UnssockMatTransmissionClient.h"
 
int main()
{
    UnssockMatTransmissionClient socketMat;
    if (socketMat.socketConnect("127.0.0.1", 6666) < 0)
    {
        return 0;
    }
     
    cv::VideoCapture capture(0);
    cv::Mat image;
 
    while (1)
    {
        if (!capture.isOpened())
            return 0;
 
        capture >> image;
 
        if (image.empty())
            return 0;
 
        socketMat.transmit(image);
    }
 
    socketMat.socketDisconnect();
    return 0;
}
