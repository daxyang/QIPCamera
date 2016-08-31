#include "ipcamera.h"
#define TIME_OUT 10
IPCamera::IPCamera()
{

}

IPCamera::~IPCamera()
{

}

/****************************************************************************
 * public function
*/

/****************************************************************************
 * 启动线程
 ****************************************************************************
*/
int IPCamera::start_getdata_fromIPC()
{
#if WIN32
    pthread_create(&write_data_from_ipcam_thread_id,NULL,get_data_thread,this);
#else
    pthread_attr_t attr;
    pthread_attr_init (&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&write_data_from_ipcam_thread_id,&attr,get_data_thread,this);
    pthread_attr_destroy (&attr);
#endif
    return 0;
}
/**********************************************************
 * 退出线程
 **********************************************************
*/
void IPCamera::exit()
{
    pthread_cancel(write_data_from_ipcam_thread_id);
    pthread_join(write_data_from_ipcam_thread_id,NULL);
}


/**********************************************************
 * private function
*/
/**********************************************************
 * 连接相机
 *  输入:
 *    ip:相机IP地址
 *    port:相机端口号
 *
 **********************************************************
*/

int IPCamera::create_connect(char *ip, int port)
{
    struct sockaddr_in serv_addr;
    fd_set fdr, fdw;
    struct timeval timeout;
    int flags,res;
    unsigned long ul;
    printf("connect to Ipcamera.\n\r");
    int iSocketID;
#ifdef __WIN32
    WORD sockVersion = MAKEWORD(2,2);
    WSADATA data;

    if(WSAStartup(sockVersion, &data) != 0)
    {
        printf("ESAStart error\n");
        return -1;
    }
    if(LOBYTE(data.wVersion)!=2 || HIBYTE(data.wVersion)!=2)
    {
        WSACleanup();
        printf("Invalid WinSock version!\n");
        return -1;
    }
    iSocketID = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(iSocketID == INVALID_SOCKET)
    {
        printf("socket create error %ld\n",GetLastError());
        printf("create socket failed!\n");
        return -1;
    }
#else
    if ((iSocketID = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("create socket failed!\n");
        return -1;
    }
#endif
#ifdef _WIN32
    ul = 0;

    if((flags = ioctlsocket(iSocketID, FIONBIO, &ul)) < 0) {

#else
    ul = 1;
    if((flags = ioctl(iSocketID,FIONBIO,&ul)) < 0) {
#endif
        perror("Netwrok test...\n");

        close(iSocketID);
        return -1;
    }

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
#ifdef _WIN32
    serv_addr.sin_addr.S_un.S_addr = inet_addr(ip);
#else
    if (inet_pton(AF_INET,ip,&serv_addr.sin_addr) <= 0)
    {
        printf("ip error!\n");
        close(iSocketID);
        return -1;
    }
#endif
    int optval = 1 * 1024 * 1024;
    int size_len = sizeof(int);
    int sokopt = setsockopt(iSocketID,SOL_SOCKET,SO_RCVBUF,&optval,size_len);
    if(sokopt < 0)
      printf("set socket opt error\n");

    if(::connect(iSocketID, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        if(errno != EINPROGRESS)
        { // EINPROGRESS

            printf("Network test1...\n");
            close(iSocketID);
            return -1;
        }
    }
    else {
        printf("Connected1!\n");
    }
    FD_ZERO(&fdr);
    FD_ZERO(&fdw);
    FD_SET(iSocketID, &fdr);
    FD_SET(iSocketID, &fdw);

    timeout.tv_sec = TIMEOUT;
    timeout.tv_usec = 0;


    res = select(iSocketID + 1, &fdr, &fdw, NULL, &timeout);
    if(res < 0) {
        perror("Network test...\n");
        close(iSocketID);
        return -1;
    }
    if(res == 0) {
        printf("Connect server timeout\n");
        close(iSocketID);
        return -1;
    }
    if(res == 1) {
        if(FD_ISSET(iSocketID, &fdw))
        {
            printf("Connected2!\n");
        }

    }

    /* Not necessary */
    if(res == 2) {
        printf("Connect server timeout\n");
        close(iSocketID);
        return -1;
    }
    ul = 0;
#ifdef _WIN32
    ioctlsocket(iSocketID,FIONBIO,&ul);
#else
    ioctl(iSocketID, FIONBIO, &ul); //重新将socket设置成阻塞模式
#endif
    return iSocketID;
}

/*********************************************************
 * 读取数据线程
 *********************************************************
*/
void *IPCamera::get_data_thread(void *ptr)
{
    IPCamera *pThis = (IPCamera *)ptr;
    pThis->get_data();

}
/********************************************************
 * 读取/写入指定长度的数据-网络
 * 输入:
 *    sk:soctk id
 *    buf:缓冲区
 *    len:数据长度
 ********************************************************
*/
int IPCamera::READ(int sk, char *buf, int len)
{
    int ret;
    int left = len;
    int pos = 0;

    while (left > 0)
    {
        if((ret = recv(sk,&buf[pos], left,0))<0)
        {
            printf("read data failed!ret,left: %d,%d,%s\n",ret,left,strerror(errno));
            return -1;
        }

        left -= ret;
        pos += ret;
    }

    return 0;
}
int IPCamera::WRITE(int sk, char *buf, int len)
{
    int ret;
    int left = len;
    int pos = 0;

    while (left > 0)
    {
        if((ret = send(sk,&buf[pos], left,0))<0)
        {
            printf("write data failed!\n");
            return -1;
        }

        left -= ret;
        pos += ret;
    }

    return 0;
}

/*************************************************************
 * 连接相机
 *************************************************************
*/
int IPCamera::connect_ipcam(char *ip, int port)
{
    int iSocketID;
    if( (iSocketID =  create_connect(ip,port)) < 0)
    {
        printf("create connect failed!\n");
        return -1;
    }
    iSockID = iSocketID;
    return iSocketID;
}
