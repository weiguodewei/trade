#ifndef __VELOCIMETRY_H__
#define __VELOCIMETRY_H__

#include "h5sdk_export.h"
#include <Include/hscomm_message_interface.h>

// 测速失败的错误号枚举
enum VelocimetryErrorNo {
    TIME_OUT = 1,  // 超时
    USER_CANCEL,   // 用户主动取消测速
    PORT_NOT_OPEN, // 服务端程序未启动或服务端程序繁忙，该错误也可能暗示连到了错误的服务器上
    SERVER_CLOSE,  // 服务端主动关闭连接，该错误一般意味着连到了错误的服务器上
};

// 测速站点及回带数据定义
class Endpoint {
public:
    char userData[32];
    char ip[16];
    unsigned short port;
};

// 测速回调接口
class VelocimetryCallback {
public:
    // 当某个站点测速成功的回调
    // delay为该服务端完成测速的耗时，单位:毫秒
    // load为该服务端的负载，该值范围为[0, 100]，0表示负载最低，100表示负载最高
    virtual int H5SDKAPI OnVelocimetrySucceed(Endpoint *endpoint, int delay, int load) = 0;
    // 当某个站点测速失败的回调
    virtual int H5SDKAPI OnVelocimetryFail(Endpoint *endpoint, VelocimetryErrorNo errorNo) = 0;
    // 当所有站点都测速结束（无论成功失败）的回调
    virtual int H5SDKAPI OnVelocimetryComplete() = 0;
};

// 测速接口
class Velocimetry: public IHSKnown {
public:
    virtual ~Velocimetry() {}

    // 发起测速
    virtual int H5SDKAPI Start(Endpoint *endpoints, size_t endpointCount, VelocimetryCallback *velocimetryCallback, int milliseconds) = 0;
    // 取消测速
    virtual int H5SDKAPI Stop() = 0;
};

#endif
