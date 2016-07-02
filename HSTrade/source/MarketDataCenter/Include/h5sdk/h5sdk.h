#ifndef __H5SDK_H__
#define __H5SDK_H__

#include "h5sdk_export.h"
#include "velocimetry.h"

#include <Include/hscomm_message_factory_interface.h>
#include <Include/hscomm_message_interface.h>
#include <Include/sdk_tag.h>

// 会话选项
class SessionOptions: public IHSKnown {
public:
    virtual ~SessionOptions() {}

    // 设置心跳超时时间，默认为8秒，心跳超时后连接将被断开
    virtual int H5SDKAPI SetHeartbeatTimeout(int seconds) = 0;
    // 获取心跳超时时间
    virtual int H5SDKAPI GetHeartbeatTimeout() = 0;

    // 设置指定下标的服务端IP，默认为"127.0.0.1"
    virtual int H5SDKAPI SetServerIp(const char *ip, size_t index = 0) = 0;
    // 设置指定下标的服务端端口，默认为8888
    virtual int H5SDKAPI SetServerPort(unsigned short port, size_t index = 0) = 0;

    // 获取指定下标的服务端IP
    virtual const char * H5SDKAPI GetServerIp(size_t index = 0) = 0;
    // 获取指定下标的服务端端口
    virtual unsigned short H5SDKAPI GetServerPort(size_t index = 0) = 0;

    // 获取设置的服务端个数
    virtual size_t H5SDKAPI GetServerCount() = 0;

    // 设置重连间隔，单位为秒，默认为3秒
    virtual int H5SDKAPI SetReconnintvl(int reconnintvl) = 0;
    // 设置重连次数，默认为5次
    virtual int H5SDKAPI SetReconnretries(int reconnretries) = 0;

    // 获取重连间隔
    virtual int H5SDKAPI GetReconnintvl() = 0;
    // 获取重连次数
    virtual int H5SDKAPI GetReconnretries() = 0;
};

const int H5SDK_DISCONNECT          = -1;
const int H5SDK_TIMEOUT             = -3;
const int H5SDK_SDK_LOGIN_ERROR     = 1010;
const int H5SDK_SERIALIZATION_ERROR = 1002;
const int H5SDK_SET_DECODER_ERROR   = 1004;
const int H5SDK_SDK_LOGIN_TIMEOUT   = 1005;

class Session;

// H5Sdk回调接口
class H5SdkCallback
{
public:
    // 当会话连接成功时回调
    virtual void OnConnect(Session *session, const char *peerIp, int peerPort, const char *localIp, int localPort) = 0;

    // 当会话登录成功时回调
    virtual void OnSdkLogin(Session *session) = 0;

    // 当会话收到异步消息时回调
    // response需要业务层来释放
    virtual void OnReceived(Session *session, IHsCommMessage *response) = 0;

    // 当会话连接断开时回调
    virtual void OnClose(Session *session, int reason) = 0;

    // 当会话出错时回调
    // errorNo目前可能为H5SDK_SDK_LOGIN_TIMEOUT，表示sdk登录超时
    virtual void OnError(Session *session, int errorNo) = 0;

    // 当重连指定次数后仍然无法连上则回调
    virtual void OnCore(Session *session) = 0;
};

// 会话
class Session: public IHSKnown {
public:
    virtual ~Session() {}

    // 创建指定业务号、功能号、包类型的消息
    // 该方法必须在回调接口H5SdkCallback的方法OnSdkLogin回调后才可以调用
    virtual IHsCommMessage * H5SDKAPI CreateMessage(int businessId, int functionId, int packetType) = 0;

    // 同步调用
    // 同步调用不会收到异步应答
    // 超时应答会被自动丢弃，不会被其他SyncCall收到
    // @return 0 表示 成功
    // @return H5SDK_DISCONNECT 表示 连接未建立或已断开
    // @return H5SDK_SERIALIZATION_ERROR 表示 request序列化失败，具体原因可调用request的GetLastErrInfo方法来查看
    // @return H5SDK_TIMEOUT 表示 调用超时
    virtual int H5SDKAPI SyncCall(IHsCommMessage *request, IHsCommMessage **response, int milliseconds) = 0;

    // 异步发送
    // @return 0 表示 成功
    // @return H5SDK_DISCONNECT 表示 连接未建立或已断开
    // @return H5SDK_SERIALIZATION_ERROR 表示 request序列化失败，具体原因可调用request的GetLastErrInfo方法来查看
    virtual int H5SDKAPI AsyncSend(IHsCommMessage *request) = 0;

    // 获取本会话所关联的消息工厂
    virtual IHsComMessageFactory * H5SDKAPI GetMessageFactory() = 0;

    // 设置回调接口
    // 该接口必须在StartSetCallback之后，StartAllSessions之前调用
    // @return 0 表示 设置成功
    // @return H5SDK_SET_DECODER_ERROR 表示 设置SetDecoder失败
    virtual int H5SDKAPI SetCallback(H5SdkCallback *h5SdkCallback) = 0;

    // 发起异步连接
    // 如果要实现断线重连，可在回调接口H5SdkCallback的OnCore方法里进行调用
    // StartAllSessions调用后，会自动发起所有会话的AsyncConnect
    // 当连接成功时会通过回调接口H5SdkCallback的方法OnConnect进行通知
    // 当连接一直无法连上则会通过回调接口H5SdkCallback的方法OnCore进行通知
    // @return 0 表示 发起成功
    virtual int H5SDKAPI AsyncConnect() = 0;

    // 发起异步Sdk登录
    // millseconds表示超时时间，单位毫秒
    // 该方法在回调接口H5SdkCallback的OnConnect方法里进行调用
    // 当Sdk登录成功时会通过回调接口H5SdkCallback的方法OnSdkLogin进行通知
    // 当Sdk登录超时时会通过回调接口H5SdkCallback的方法OnError进行通知，其中errorNo为H5SDK_SDK_LOGIN_TIMEOUT
    // 当OnSdkLogin回调之后才可以发起业务层操作
    // @return 0 表示 发起成功
    virtual int H5SDKAPI AsyncSdkLogin(int millseconds) = 0;

    // 发起异步连接
    // 本接口和AsyncConnect的主要区别是本接口可以运行时指定IP和端口
    // @return 0 表示 发起成功
    virtual int H5SDKAPI AsyncConnectEx(const char *ip, unsigned short port) = 0;
};

#endif
