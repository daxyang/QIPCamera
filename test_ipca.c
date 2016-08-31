#include "qipcamera.h"
#include "QNetClient.h"
//#include "ant_ipcamera.h"
int main()
{
  // QNetClient *ant_client = new QNetClient();
  // // ant_client->set_protocol_ack_callback(NET_TCP_TYPE_VID,NET_VID_CONNECT,connect_ipcam_ack);
  // // ant_client->set_protocol_ack_callback(NET_TCP_TYPE_VID,NET_VID_STREAM,get_stream_ack);
  // // ant_client->set_protocol_ack_callback(NET_TCP_TYPE_CTRL,NET_CTRL_LOGIN,login_ack);
  // // ant_client->reply = close_sk;
  // ant_client->connect_server("192.168.3.124",6666);
  // ant_client->start();
  // usleep(2000);
  // u32 len = sizeof(app_net_ctrl_login);
  // char *buffer = (char *)malloc(sizeof(char) * len);
  // app_net_ctrl_login *login = (app_net_ctrl_login *)buffer;
  // strcpy(login->name,"admin");
  // strcpy(login->passwd,"123456");
  // ant_client->main_cmd_process(NET_TCP_TYPE_CTRL,NET_CTRL_LOGIN,buffer,len);
  // pause();
  // free(buffer);

  //ant_ipcamera *ipca = new ant_ipcamera();
  QIPCamera *ipca = new QIPCamera(ANT_IPCAM);
  ipca->connect_ipc("192.168.3.25",6666,0);
  //ipca->ipcamera_connect("192.168.3.124",6666,0);
  usleep(100000);
  ipca->start();
  pause();
  //ipca->start();
  return 0;
}
