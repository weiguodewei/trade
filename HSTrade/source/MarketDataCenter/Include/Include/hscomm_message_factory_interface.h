///恒生消息工厂接口定义
/*******************************************************
  源程序名称:hscomm_message_factory_interface.h
  软件著作权:恒生电子股份有限公司
  系统名称:  恒生消息工厂接口定义
  模块名称:  消息
  功能说明:  消息工厂接口定义
             
  作    者:  maoyinjie
  开发日期:  20100801
  备    注:  在windows下注意函数调用方式：__stdcall

  修改人员：maoyinjie 
  修改日期：20111220
  修改说明：增加PushInForce接口
			增加设置本队列所属插件名称的接口

20130319 yinghh 增加消息工厂获取指定业务号的模板信息二进制内容的接口
20130320 yinghh 增加消息工厂动态加载“密文”模板信息的接口
20130514 yinghh 增加消息工厂动态加载“明文”模板信息的接口
20130516 yinghh 增加释放工厂的接口
20140429 yinghh 工厂加载模板的接口取消业务号参数，修改后的逻辑为加载所有的模板
*********************************************************/

#ifndef __HSCOMM_MESSAGE_FACTORY_INTERFACE_H__
#define __HSCOMM_MESSAGE_FACTORY_INTERFACE_H__

#include <Include/pluginid.h>
#include <Include/plugin_interface.h>
#include <Include/hscomm_message_interface.h>

///	存放消息的Q接口,确保线程安全
struct IHsComQ : IKnown
{
	///	从消息队列中获得一个消息
	virtual IHsCommMessage* FUNCTION_CALL_MODE Pop(int iWaitTime, void** lppOwner, int iID=0)=0;
	///	往消息队列中插入一个新的消息, 返回0成功,非0:失败,需要调用者自己释放该消息
	virtual int FUNCTION_CALL_MODE Push(IHsCommMessage* lpMsg, void* lpOwner, int iID=0)=0;
	/// 获得Q中消息个数
	virtual int FUNCTION_CALL_MODE GetMSGsInQ()=0;
	///	初始化本Q内最多允许存在的消息个数，0不限制
	virtual int FUNCTION_CALL_MODE Init(int iMaxMsgsInQ=0)=0;
	///	获得Q内部曾经存在的最多的消息个数
	virtual int FUNCTION_CALL_MODE GetMaxMSGsInQ()=0;
	///	获得Q为空时被激活的次数
	virtual int FUNCTION_CALL_MODE GetIdles()=0;
	///	获得Q中已处理消息个数
	virtual int FUNCTION_CALL_MODE GetDealMsgs()=0;
	///	设置Q中允许积压的最多的消息个数,缺省0,无限制
	virtual int FUNCTION_CALL_MODE SetMaxMsgsInQ(int iMax)=0;

	//	maoyinjie 20111220 强制入列,不受队列最大积压个数限制
	///	往消息队列中插入一个新的消息, 总是成功
	virtual int FUNCTION_CALL_MODE PushInForce(IHsCommMessage* lpMsg, void* lpOwner, int iID=0)=0;
	///	增加设置本队列所属插件名称的接口
	virtual int FUNCTION_CALL_MODE SetOwnerID(HS_SID id)=0;
};

// 表示模板的版本信息
struct TemplateVersion
{
    int bizId;
    const char *version;
};

// 消息缓存工厂,应用需要用到消息以及Q时应该向工厂申请,线程安全
// 工厂加载模板的接口取消业务号参数，修改后的逻辑为加载所有的模板
struct IHsComMessageFactory  : IKnown
{
	// 申请Q队列
	virtual IHsComQ* FUNCTION_CALL_MODE CreateESBQ()=0;
	// 释放Q队列
	virtual void FUNCTION_CALL_MODE DestroyESBQ(IHsComQ* lpESBQ)=0;

	// 申请消息
	virtual IHsCommMessage* FUNCTION_CALL_MODE GetESBMsg()=0;


	virtual IHsCommMessage* FUNCTION_CALL_MODE OpenBuffer(char *lpBuffer, int iLen)=0;
	//	强制获得一个esbmsg,不受esbmsg工厂的限制
	virtual IHsCommMessage* FUNCTION_CALL_MODE GetESBMsgByForce()=0;

	virtual void FUNCTION_CALL_MODE SetDefaultBizID(uint8 byDefaultBiz)=0;


	//	//	maoyinjie 20101223
//////////////////////////////////////////////////////////////////////////
	/// 根据fieldid获得fieldname
	/**
	 *@param int iFieldID		字段id
	 *@param int iBizModulID	操作时用到的业务模板
	 * @return fieldname, 如果失败,返回""
	*/
	virtual const char* FUNCTION_CALL_MODE GetFieldNameByID(int iFieldID, int iBizModulID)=0;
	/// 根据名字获得id
	/**
	 *@param const char* lpFieldName		字段名
	 *@param int iBizModulID	操作时用到的业务模板
	 * @return >=0字段ID	<0,说明没找到
	*/
	virtual int FUNCTION_CALL_MODE GetFieldIDByName(const char* lpFieldName, int iBizModulID)=0;
	virtual const char* FUNCTION_CALL_MODE GetFuncNameByID(int iFuncD, int iPacketType, int iBizModulID)=0;

	//	获得一个字段的取值类型
	virtual int FUNCTION_CALL_MODE GetFieldType(int iBizModulID, int iTagID)=0;
	//	获得某个array类型的字段内部,具体特定成员的取值类型
	virtual int FUNCTION_CALL_MODE GetElementType(int iBizModulID, int iTagID, int iIndex)=0;

	virtual const char * FUNCTION_CALL_MODE GetFuncNote(int iFuncD, int iPacketType, int iBizModulID) = 0;

    // 20130319 yinghh 增加获取指定业务号的模板信息二进制内容的接口
    /// 获取指定业务号的模板信息的二进制内容（本接口供作为服务端消息插件时使用）
	/**
	*@param businessId 业务号
	*@param template_buffer 获取的模板信息二进制内存地址[OUT]
    *@param template_buffer_length 获取的模板信息二进制长度[OUT]
	*@return 返回0表示成功，返回-1表示不存在该业务号对应的模板信息
	*/
    virtual int FUNCTION_CALL_MODE GetTemplateByBusinessId(int businessId, const char **template_buffer, int *template_buffer_length) = 0;

    // 20130320 yinghh 增加消息工厂动态加载“密文”模板信息的接口（此接口只供h5sdk使用）
    virtual int FUNCTION_CALL_MODE LoadTemplate(IHSKnown *lpCfg, const char *template_buffer, int template_buffer_length) = 0;
    // 20130514 yinghh 增加消息工厂动态加载“明文”模板信息的接口（此接口只供h5sdk使用）
    virtual int FUNCTION_CALL_MODE LoadPlaintextTemplate(IHSKnown *lpCfg, const char *template_buffer, int template_buffer_length) = 0;

    // 20130516 yinghh 增加释放工厂的接口
    virtual int FUNCTION_CALL_MODE Delete() = 0;

    // 获取所有模板的版本信息
    // count为[IN|OUT]参数
    virtual int FUNCTION_CALL_MODE GetTemplateVersion(TemplateVersion *templateVersion, size_t *count) = 0;

    /// 根据功能名称来获取对应的功能号、包类型、业务号
    /**
	*@param funcName 功能名称
	*@param iFuncD 功能号[OUT]
    *@param iPacketType 包类型[OUT]
    *@param iBizModulID 业务号[OUT]
	*@return 返回0表示成功，返回-1表示不存在该功能名称对应的模板信息
	*/
    virtual int FUNCTION_CALL_MODE GetIDByFuncName(const char *funcName, int *iFuncD, int *iPacketType, int *iBizModulID) = 0;
};



//消息缓存工厂引出函数---------------------------
#ifdef __cplusplus
extern "C" { 
#endif
IKnown*	FUNCTION_CALL_MODE GetHsCommMessageFactoryInstance();
int		FUNCTION_CALL_MODE GetHsCommMessageFactoryInit(void* lpInstance, PFOnSetTimeOut pfOnSetTimeOut = NULL);
char*	FUNCTION_CALL_MODE GetHsCommMessageFactoryDependSvr(void * lpInstance);
int		FUNCTION_CALL_MODE GetHsCommMessageFactoryInfo(int iIndex, tagBaseServiceInfo* ppv);
IHsComMessageFactory * FUNCTION_CALL_MODE NewHsCommMessageFactory();

#ifdef __cplusplus
}
#endif


#endif
