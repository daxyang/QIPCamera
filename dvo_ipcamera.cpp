#include "dvo_ipcamera.h"
#include "sys/syscall.h"
dvo_IPCamera::dvo_IPCamera()
{

}
dvo_IPCamera::~dvo_IPCamera()
{

}

void dvo_IPCamera::bound(QSlidingWindow *sliding_window)
{
    slidingWindow = sliding_window;
}
int dvo_IPCamera::get_data()
{
    int iFindFirstIFrame = 0;
    tagMsgHead_t *msg_head;
    app_net_tcp_enc_stream_head_t *stream_head;
    struct stream_info_t *stream_info = (stream_info_t *)malloc(sizeof(struct stream_info_t));
    char msg[MSG_HEADER_LEN + sizeof(app_net_tcp_enc_stream_head_t)];
    char len = MSG_HEADER_LEN + sizeof(app_net_tcp_enc_stream_head_t);
    char *stream_buffer;
    char *cmd_system = (char *)malloc(sizeof(char) * 1024);
    sprintf(cmd_system,"/bin/taskset -p 0x20 %d",syscall(SYS_gettid));
    system(cmd_system);
    free(cmd_system);
    stream_buffer = (char *)malloc(sizeof(char) * 2 * 1024 * 1024);
    frame_info = new struct _frame_info_t;
    while(1)
    {
        pthread_testcancel();  //在Mac下，当程序处在后台运行，该句会使该线程退出
       if (READ(iSockID, msg, len) < 0)
        {
            printf("read enc stream head failed!\n");
            return -1;
        }

        msg_head = (tagMsgHead_t *)&msg[0];
        stream_head = (app_net_tcp_enc_stream_head_t *)&msg[MSG_HEADER_LEN];
        stream_info->len  = ntohl(msg_head->Pktlen) - MSG_HEADER_LEN - sizeof(app_net_tcp_enc_stream_head_t);
        stream_info->stamp_sec  = ntohl(stream_head->sec);
        stream_info->stamp_usec = ntohl(stream_head->usec);
        if(0)
            printf("(DEBUG_READ_DATA:%d)read:stream_id = %u, frame_type = %u, frame_num = %u, sec = %d, usec = %d, len = %d\n", \
                   iSockID,ntohl(stream_head->stream), ntohl(stream_head->frame_type), \
                   ntohl(stream_head->frame_num), stream_info->stamp_sec, stream_info->stamp_usec, stream_info->len);

        //if (stream_info->len >(int) slidingwindow->video->size)
       if(stream_info->len > slidingWindow->buffer_len)
        {
            printf("stream len error!\n");
            return -1;
        }

        if (READ(iSockID, (char *)stream_buffer, stream_info->len) < 0)
        {
            printf("read stream failed!\n");
            return -1;
        }

        if(iFindFirstIFrame == 0)
        {
            if(ntohs(msg_head->DataType) == 2)
            {
                if((ntohl(stream_head->frame_type) == 1)  || (ntohl(stream_head->frame_type)  == 5))
                    iFindFirstIFrame = 1;
            }
        }
        frame_info->frame_type = ntohl(stream_head->frame_type);
        if(iFindFirstIFrame == 1)
        {
            if(decode_i_Frame == 1)
            {
                if(ntohl(stream_head->frame_type) == 1)//这里是只将I帧送去解码
                {
                    slidingWindow->write_data_to_buffer((u32)stream_info->len,(char *)stream_buffer,frame_info);
                }

            }
            else
            {
              slidingWindow->write_data_to_buffer((u32)stream_info->len,(char *)stream_buffer,frame_info);
            }

        }

       // usleep(1000);

        pthread_testcancel();
#if defined(Q_OS_WIN32)
        usleep(1000);
#elif defined(Q_OS_MACX)
        pthread_yield_np();
#elif defined(Q_OS_UNIX)
        usleep(5000);
        pthread_yield();
#endif
    }
    printf("read data quit\n");
    free(stream_buffer);
    return 0;
}
/***********************************************************
 * 连接相机，并连接相应的码流，读取码流并进行确认
 * int dvo_ipcamera::send_stream_cmd(char *ip,int port,int stream_id)
 * 继承父类虚函数 virtual int send_stream_cmd(char *ip,int port,int stream_id) = 0;
 * *********************************************************
 */
int dvo_IPCamera::send_stream_cmd(char *ip,int port,int stream_id)
/*
 * ip: ip地址
 * port:端口号 缺省6001
 * stream_id:码流号: 0-主码流 1-次码流 2-第三码流(JPEG)
 */
{
    iStreamID = stream_id;
    int sockid;
    //连接相机
    if((sockid = connect_ipcam(ip,port)) < 0)
    {
        printf("create socket failed!\n");
        return -1;
    }
    else
        printf("create socket ok!\n");
    //发送码流连接协议
    if(dvo_send_enc_ctl(sockid,stream_id) < 0)
    {
        printf("send enc ctl failed!\n");
        return -1;
    }
    //接收确认信息
    if(dvo_rcv_enc_ctl_ack(sockid,stream_id) < 0)
    {
        printf("rec enc ctl failed\n");
        return -1;
    }
    return sockid;
}
/*
 * 接受并确认信息
 */
int dvo_IPCamera::dvo_rcv_enc_ctl_ack(int sk, int stream_id)
/*
 * sk: socket
 * stream_id: 码流
 */
{
    char msg[MSG_HEADER_LEN + sizeof(app_net_tcp_enc_ctl_ack_t)];
    int  len = MSG_HEADER_LEN + sizeof(app_net_tcp_enc_ctl_ack_t);
    tagMsgHead_t *head;
    app_net_tcp_enc_ctl_ack_t *ctl_ack;

    if (READ(sk, msg, len) < 0)
    {
        printf("read enc ctl ack failed!\n");
        return -1;
    }

    head    = (tagMsgHead_t *)&msg[0];
    ctl_ack = (app_net_tcp_enc_ctl_ack_t *)&msg[MSG_HEADER_LEN];

    if ((ntohs(head->DataType) != DATA_TYPE_CMD)
            || (ntohs(head->CmdType) != NET_ENC)
            || (ntohs(head->CmdSubType) != ENC_STREAM_START))
    {
        printf("msg header error!\n");
        return -1;
    }

    if (ntohl(ctl_ack->stream) != (uint32_t)stream_id)
    {
        printf("stream id error!\n");
        return -1;
    }
    return 0;
}
//发送确认头信息
int dvo_IPCamera::dvo_send_enc_ctl(int sk, int stream_id)
{
    char msg[MSG_HEADER_LEN + sizeof(app_net_tcp_enc_ctl_t)];
    int  len = MSG_HEADER_LEN + sizeof(app_net_tcp_enc_ctl_t);
    app_net_tcp_enc_ctl_t *enc_ctl = (app_net_tcp_enc_ctl_t *)&msg[MSG_HEADER_LEN];

    BUILD_MSG_HEADER((tagMsgHead_t *)&msg[0], NET_ENC, ENC_STREAM_START, DATA_TYPE_CMD, len, 0);

    enc_ctl->stream = htonl(stream_id);

    WRITE(sk, msg, len);
    return 0;
}
int dvo_IPCamera::ipcamera_connect(char *ip, int port, int stream)
{
   return send_stream_cmd(ip,port,stream);
}
