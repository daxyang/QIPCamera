#ifndef ANT_IPCAMERA_H
#define ANT_IPCAMERA_H
#include "QNetClient.h"
#include "ipcamera.h"
#include "GetDVOStream.h"
class ant_ipcamera : public IPCamera
{
public:
  ant_ipcamera();
  ~ant_ipcamera();
  int ipcamera_connect(char *ip,int port,int stream);
  void bound(QSlidingWindow *sliding_window);
  static ant_ipcamera *pthis;
  void login();
  void start();

protected:
  int get_data();
private:
  QNetClient *ant_client;
  static void connect_ipcam_ack(char *data,u32 len);
  static void get_stream_ack(char *data,u32 len);
  static void login_ack(char *data,u32 len);
  static void close_sk(int sig);
  QSlidingWindow *slidingWnd;
  int stream_id;
  int firstIFrame;
};

#endif
