#ifndef QIPCAMERA_H
#define QIPCAMERA_H
#include "dvo_ipcamera.h"
#include "qipcamera_global.h"



class QIPCAMERASHARED_EXPORT QIPCamera
{

public:
    QIPCamera();

    void bound(QSlidingWindow *slidingwnd);
    void connect_ipc(char *ip,int port,int stream);
    void start();
private:
    dvo_IPCamera *dvoipcamera;



};

#endif // QIPCAMERA_H
