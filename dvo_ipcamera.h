#ifndef DVO_IPCAMERA_H
#define DVO_IPCAMERA_H
#include "ipcamera.h"
/*
 * DVO相机相关的头信息
 */
#ifdef __cplusplus
extern "C" {
#endif

//整个系统的消息分为六大类，分为系统，编码，图像，PTZ，事件，音频，其他，对应的命令字CmdType如下
#define		NET_SYS					    0x0001
#define		NET_ENC						0x0002
#define		NET_IMG						0x0003
#define		NET_PTZ						0x0004
#define		NET_EVT						0x0005
#define		NET_ADO						0x0006
#define		NET_MSC						0x0007
//data type cmds

#define         DATA_TYPE_CMD           0x0001
#define         DATA_TYPE_VIDEO         0x0002
#define         DATA_TYPE_AUDIO         0x0003
//system cmds
#define        SYS_LOGIN_IN            0x0001
#define        SYS_LOGIN_OUT           0x0002
#define        SYS_NET_SET             0x0003
#define        SYS_NET_CHECK           0x0004
#define        SYS_TIME_SET            0x0005
#define        SYS_TIME_CHECK          0x0006
#define        SYS_REBOOT              0x0007
#define        SYS_SHUTDOWN            0x0008
#define        SYS_RESTORE_DEFAULT_SET 0x0009
#define        SYS_NUCLEAR_UPDATE      0x000a
#define        SYS_COM_SET             0x000b
#define        SYS_COM_CHECK           0x000c
#define        SYS_RES_CHECK           0x000d
#define        SYS_LOG_GET             0x000e
#define        SYS_DEVICE_CHECK        0x000f
#define        SYS_DEVICE_SET          0x0010
#define        SYS_DISKINFO_CHECK      0x0011
#define        SYS_KEEP_LIVE_SET       0x0012
#define        SYS_KEEP_LIVE           0x0013
#define        SYS_KEEP_LIVE_BACK      0x0014
#define        SYS_SN_MAC_SET          0x0015
#define        SYS_SN_MAC_GET          0x0016
#define        SYS_VOUT_SET            0x0017
#define        SYS_VOUT_GET            0x0018
#define        SYS_NTP_SET             0x0019
#define        SYS_NTP_GET             0x001a
//encode
#define        ENC_VIDEO_PARA_SET         0x0001
#define        ENC_VIDEO_PARA_GET         0x0002
#define        ENC_STREAM_START           0x0003
#define        ENC_OSD_SET                0x0005
#define        ENC_OSD_GET                0x0006
#define        ENC_PM_SET                 0x0007
#define        ENC_CUSTOM_OSD_ZONE_SET    0x0008
#define        ENC_CUSTOM_OSD_ZONE_GET    0x0009
#define        ENC_CUSTOM_OSD_LINE_PARA_SET 0x000a
#define        ENC_CUSTOM_OSD_LINE_PARA_GET 0x000b
#define        ENC_CUSTOM_OSD_LINE_DATA_SET 0x000c
#define        ENC_CUSTOM_OSD_LINE_DATA_GET 0x000d

//img
#define        IMG_COLOR_SET           0x0001
#define        IMG_COLOR_GET           0x0002
#define        IMG_EXP_SET             0x0003
#define        IMG_EXP_GET             0x0004
#define        IMG_BW_SET              0x0005
#define        IMG_BW_GET              0x0006
#define        IMG_DAY_NIGHT_SET       0x0007
#define        IMG_DAY_NIGHT_GET       0x0008
#define        IMG_VIDEO_MODE_SET      0x0009
#define        IMG_VIDEO_MODE_GET      0x000a

//ptz
#define        PTZ_SET                 0x0001
#define        PTZ_TRANSPARENT_SEND    0x0002

//EVT
#define         EVT_IO_ALARM_SET        0x0001
#define         EVT_IO_ALARM_CHECK      0x0002
#define         EVT_IO_MD_SET           0x0003
#define         EVT_IO_MD_CHECK         0x0004
#define         EVT_IO_MD_PARA_SET      0x0005
#define         EVT_IO_MD_PARA_GET      0x0006
#define         EVT_485_ALARM_SET       0x0007
#define         EVT_485_ALARM_GET       0x0008
#define         EVT_485_ALARM_PARA_SET  0x0009
#define         EVT_485_ALARM_PARA_GET  0x000a

//ado
#define         ADO_INPUT_DATA_CTL_SET  0x0001
#define         ADO_INPUT_DATA_CTL_GET  0x0002
#define         ADO_OUTPUT_DATA_CTL_SET 0x0003
#define         ADO_OUTPUT_DATA_CTL_GET 0x0004
#define         ADO_INPUT_ENC_PARA_SET  0x0005
#define         ADO_INPUT_ENC_PARA_GET  0x0006
#define         ADO_OUTPUT_ENC_PARA_SET 0x0007
#define         ADO_OUTPUT_ENC_PARA_GET 0x0008
#define         ADO_DATA_RECORD_PACKET  0x0009
#define         ADO_DATA_SPEAK_PACKET   0x000A

//misc
#define		NET_MSC_DES10		        0x0001
#define     NET_MSC_WIFI_SET            0x0002
#define     NET_MSC_WIFI_GET            0x0003
#define     NET_MSC_AUDIO_OUT_START     0x0004
#define     NET_MSC_485_DEV_SET         0x0005
#define     NET_MSC_485_DEV_GET         0x0006
#define     NET_MSC_485_OSD_PARA_SET    0x0007
#define     NET_MSC_485_OSD_PARA_GET    0x0008
#define     NET_MSC_485_INPUT_DATA_GET  0x0009

//启动停止发送码流命令
typedef struct
{
    u32			chn;//视频输入通道号，取值0~MAX-1.
    u32			stream;//码流编号：0:主码流，1：子码流，2：第三码流。
    u32			enable;//0:停止发送，1：开始发送。
    u32			nettype;//0:tcp,1:udp.
    u32			port;//udp port.
    u32			rev[3];
}app_net_tcp_enc_ctl_t;

typedef struct{
    u32			chn;//视频输入通道号，取值0~MAX-1.
    u32			stream;//码流编号：0:主码流，1：子码流，2：第三码流。
    u32			state;//0:成功，1：失败。
    u32			rev;
}app_net_tcp_enc_ctl_ack_t;

#define       NET_PORT   6001

typedef struct tagMsgHead_t
{
    char Magic1[4];
    u32  Pktlen;
    u16  Version;
    u16  Hwtype;
    u32  Sn;
    u16  CmdType;
    u16  CmdSubType;
    u16  DataType;
    u16  Rev1;
    u32  Rev2;
    u32  Rev3;
}tagMsgHead_t;

#define BUILD_MSG_HEADER(p, cmd_type, cmd_sub_type, data_type, pkt_len, sn) \
    (p)->Magic1[0] = 0xf5; \
    (p)->Magic1[1] = 0x5a; \
    (p)->Magic1[2] = 0xa5; \
    (p)->Magic1[3] = 0x5f; \
    (p)->CmdType = htons(cmd_type); \
    (p)->CmdSubType = htons(cmd_sub_type); \
    (p)->DataType = htons(data_type); \
    (p)->Pktlen = htonl(pkt_len); \
    (p)->Version = htons(0x12); \
    (p)->Hwtype = 0; \
    (p)->Sn = htonl(sn);

#define MSG_HEADER_LEN sizeof(tagMsgHead_t)

typedef struct{
    u32		chn;	//视频输入通道号，取值0~MAX-1.
    u32		stream;//码流编号：0:主码流，1：子码流，2：第三码流。
    u32		frame_type;//帧类型，范围参考APP_NET_TCP_STREAM_TYPE
    u32		frame_num;//帧序号，范围0~0xFFFFFFFF.
    u32		sec;	//时间戳,单位：秒，为1970年以来的秒数。
    u32		usec;//时间戳,单位：微秒。
    u32		rev[2];
}app_net_tcp_enc_stream_head_t;

struct stream_info_t
{
    int stamp_sec;
    int stamp_usec;
    int len;

    int rcv_sec;
    int rcv_usec;
};




#ifdef __cplusplus
}
#endif

class dvo_IPCamera : public IPCamera
{
public:
    dvo_IPCamera();
    ~dvo_IPCamera();
    int ipcamera_connect(char *ip,int port,int stream);
    void bound(QSlidingWindow *sliding_window);


protected:
    int get_data();
private:

    int decode_i_Frame;
    int iStreamID;

    int dvo_send_enc_ctl(int sk, int stream_id);
    int dvo_rcv_enc_ctl_ack(int sk, int stream_id);

    int send_stream_cmd(char *ip,int port,int stream_id);
};

#endif // DVO_IPCAMERA_H
