/** @file
* h5sdk动态库引出函数
* @author  应鸿浩
* @author  恒生电子股份有限公司
* @version 1.0
* @date    20140428
*/

#ifndef __H5SDK_EXPORT_H__
#define __H5SDK_EXPORT_H__

#ifdef _WIN32
#  define H5SDKAPI __stdcall
#else
#  define H5SDKAPI
#endif

class SessionOptions;
class Session;
class Velocimetry;

extern "C" {
    // 获取SDK版本
    const char * H5SDKAPI GetSdkVersion();

    // 创建会话选项SessionOptions
    SessionOptions * H5SDKAPI CreateSessionOptions();

    // 用会话选项SessionOptions来创建会话Session
    Session * H5SDKAPI CreateSession(SessionOptions *sessionOptions);

    // 创建测速对象Velocimetry
    Velocimetry * H5SDKAPI CreateVelocimetry();

    // 获取错误号对应的错误信息
    const char * H5SDKAPI GetErrorStringByErrorNo(int errorNo);

    // 开始设置所有会话的回调接口
    // 该接口必须在所有会话创建完后进行调用
    int H5SDKAPI StartSetCallback();

    // 启动所有会话
    int H5SDKAPI StartAllSessions();
}

#endif
