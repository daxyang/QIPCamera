#ifndef QIPCAMERA_H
#define QIPCAMERA_H
#include "dvo_ipcamera.h"
//#include "qipcamera_global.h"
#include "ant_ipcamera.h"

//<modify by Antony 2016-8-19>
//class QIPCAMERASHARED_EXPORT QIPCamera
#define DVO_IPCAM 1
#define ANT_IPCAM 2
class QIPCamera
//<!2016-8-19>
{

public:
  QIPCamera();
  QIPCamera(int factory_id);

  void bound(QSlidingWindow *slidingwnd);
  void connect_ipc(char *ip,int port,int stream);
  void start();
private:
    dvo_IPCamera *dvoipcamera;
    ant_ipcamera *ant_ipc;
    int factory;



};

#endif // QIPCAMERA_H
