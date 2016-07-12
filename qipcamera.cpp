#include "qipcamera.h"


QIPCamera::QIPCamera()
{
    dvoipcamera = new dvo_IPCamera();

}
void QIPCamera::bound(QSlidingWindow *slidingwnd)
{
    dvoipcamera->bound(slidingwnd);
}
void QIPCamera::connect_ipc(char *ip, int port, int stream)
{
    dvoipcamera->ipcamera_connect(ip,port,stream);
}
void QIPCamera::start()
{
    dvoipcamera->start_getdata_fromIPC();
}
