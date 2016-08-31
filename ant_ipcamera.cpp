/*
 * use the QAntProtocol and get dvo_ipc stream
 * author:Antony
 * (copyright:2016-8 Ver0.01)
 *
 */
#include "ant_ipcamera.h"
ant_ipcamera *ant_ipcamera::pthis = NULL;
ant_ipcamera::ant_ipcamera()
{
  pthis = this;
  ant_client = new QNetClient();
  frame_info = new _frame_info_t();

}
ant_ipcamera::~ant_ipcamera()
{}
void ant_ipcamera::login()
{
  u32 len = sizeof(app_net_ctrl_login);
  char *buffer = (char *)malloc(sizeof(char) * len);
  app_net_ctrl_login *login = (app_net_ctrl_login *)buffer;
  strcpy(login->name,"admin");
  strcpy(login->passwd,"123456");
  ant_client->main_cmd_process(NET_TCP_TYPE_CTRL,NET_CTRL_LOGIN,buffer,len);
  free(buffer);
}
int ant_ipcamera::ipcamera_connect(char *ip,int port,int stream)
{
  ant_client->set_protocol_ack_callback(NET_TCP_TYPE_VID,NET_VID_CONNECT,connect_ipcam_ack);
  ant_client->set_protocol_ack_callback(NET_TCP_TYPE_VID,NET_VID_STREAM,get_stream_ack);
  ant_client->set_protocol_ack_callback(NET_TCP_TYPE_CTRL,NET_CTRL_LOGIN,login_ack);
  ant_client->client_close = close_sk;
  ant_client->connect_server(ip,port);
  ant_client->start();
  stream_id = stream;
  usleep(2000);
}
void ant_ipcamera::start()
{
  u32 len = sizeof(app_net_vid_connect);
  char *buffer = (char *)malloc(sizeof(char) * len);
  app_net_vid_connect *connect = (app_net_vid_connect *)buffer;
  connect->streamid = stream_id;
  ant_client->main_cmd_process(NET_TCP_TYPE_VID,NET_VID_CONNECT,buffer,len);
  free(buffer);
}
void ant_ipcamera::bound(QSlidingWindow *sliding_window)
{
  firstIFrame = 0;
  slidingWnd = sliding_window;
}

void ant_ipcamera::connect_ipcam_ack(char *data,u32 len)
{
  printf("vid connect ack\n");
  pthis->ant_client->main_cmd_process(NET_TCP_TYPE_VID,NET_VID_STREAM,NULL,0);
}
void ant_ipcamera::get_stream_ack(char *data,u32 len)
{
  pthis->frame_info->frame_type = 2;
  u32 pkg_len = sizeof(app_net_vid_ack_stream);
  app_net_vid_ack_stream *stream_ack = (app_net_vid_ack_stream *)data;
  pthis->frame_info->frame_type = ntohl(stream_ack->frame_type);
  //printf("frame type:%ld %ld\n",pthis->frame_info->frame_type);
  if((pthis->firstIFrame == 0) && (pthis->frame_info->frame_type == 1))
    pthis->firstIFrame = 2;
  if(pthis->firstIFrame == 2)
    pthis->slidingWnd->write_data_to_buffer(len-pkg_len,data+pkg_len,pthis->frame_info);
}
int ant_ipcamera::get_data()
{

}
void ant_ipcamera::login_ack(char *data,u32 len)
{
  printf("login \n");
}
void ant_ipcamera::close_sk(int sig)
{
  pthis->ant_client->quit = 1;
  usleep(10000);
  delete pthis->ant_client;

}
