#ifndef IPCAMERA_H
#define IPCAMERA_H
#include "qslidingwindow.h"
#include "pthread.h"

#if WIN32
#include "winsock.h"
#include "winsock2.h"
#else
#include <sys/socket.h>
#include <netinet/in.h> // struct sockaddr_in
#include <arpa/inet.h>  //inet_pton()
#include <sys/ioctl.h>  //FIONBIO
#endif

#include <sys/types.h>
#include <unistd.h>     //close()
#include "pthread.h"
#include "errno.h"
#include <fcntl.h>
#include <QObject>

#define TIMEOUT  5

class IPCamera
{
public:
    IPCamera();
    ~IPCamera();
    int start_getdata_fromIPC(); //启动线程
    void exit();
    virtual int ipcamera_connect(char *ip,int port,int stream) = 0;
    virtual void bound(QSlidingWindow *sliding_window) = 0;

protected:
    virtual int get_data()=0;  //读取数据虚函数
    int READ(int sk,char *buf,int len);//读取指定长度的数据
    int WRITE(int sk,char *buf,int len);//写指定长度数据
    int connect_ipcam(char *ip,int port);//连接相机
    int iSockID;
    QSlidingWindow *slidingWindow;
    struct _frame_info_t *frame_info;

private:
    int create_connect(char *ip,int port);
    static void *get_data_thread(void *ptr);
    pthread_t write_data_from_ipcam_thread_id;

private:
    char *data_buffer;

};

#endif // IPCAMERA_H
