#include "qipcamera.h"


QIPCamera::QIPCamera()
{
  //<modify by Antony 2016-8-20>
  //dvoipcamera = new dvo_IPCamera();
  //ant_ipc = new ant_ipcamera();
  //<!2016-8-20>
}
//<add by Antony 2016-8-20>
QIPCamera::QIPCamera(int factory_id)
{
  factory = factory_id;
  if(factory == ANT_IPCAM)
    printf("connect is Ant IPCamera\n");
  if(factory == DVO_IPCAM)
    printf("connect is dvo IPCamera\n");
  if(factory == ANT_IPCAM)
    ant_ipc = new ant_ipcamera();
  if(factory == DVO_IPCAM)
    dvoipcamera = new dvo_IPCamera();
}
//<!2016-8-20>
void QIPCamera::bound(QSlidingWindow *slidingwnd)
{
  if(factory == DVO_IPCAM)
    dvoipcamera->bound(slidingwnd);
  if(factory == ANT_IPCAM)
    ant_ipc->bound(slidingwnd);
}
void QIPCamera::connect_ipc(char *ip, int port, int stream)
{
  if(factory == DVO_IPCAM)
    dvoipcamera->ipcamera_connect(ip,port,stream);
  if(factory == ANT_IPCAM)
  {
    ant_ipc->ipcamera_connect(ip,port,stream);
    ant_ipc->login();
  }
}
void QIPCamera::start()
{
  if(factory == DVO_IPCAM)
    dvoipcamera->start_getdata_fromIPC();
  if(factory == ANT_IPCAM)
    ant_ipc->start();
}
