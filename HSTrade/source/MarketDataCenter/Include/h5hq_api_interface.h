#ifndef _H5HQ_API_INTERFACE_H__
#define _H5HQ_API_INTERFACE_H__

#include "h5hq_data_interface.h"		//	对外暴露的股票结构

#ifdef __cplusplus
extern "C" {
#endif

/// 到期状态通知回调函数类型
/**
	enumExpireStatus eExpireStatus:	到期状态。
	unsigned int nExpireDate:		到期日期。格式YYYYMMDD
	const char *szExpireTip:		到期提示字符串。
 **/
typedef int (FUNCTION_CALL_MODE * FP_ON_EXPIRE)(enumExpireStatus eExpireStatus, unsigned int nExpireDate, const char *szExpireTip);


/// 对H5API进行授权，请确保在调用InitApi前调用此函数。只有在正确调用此函数并授权成功后才能正常使用h5api！
/**
	入参：
		const char *lpLicenseFile:	许可授权xml文件名，如"h5apilicense.xml"
		授权格式如下：

			<?xml  version="1.0" encoding="GBK"?>
			<root>
				<licensekey note="许可密钥">jx8k16XitJM=</licensekey>
				<toexpiretip note="将要到期提示。提前一个月进行提示，每10分钟回调提示一下。%u将输出10进制数字形式的多少天后过期；%s将输出格式如2014年2月1日的到期日。配置时请保证%u在%s之前。">H5API将在%u天后到期，到期日为%s！</toexpiretip>
				<expiredtip note="到期提示。当前机器日期大于过期日后每10分钟回调提示一下。%s将输出格式如2014年2月1日的过期日">H5API已经到期了，到期日为%s！</expiredtip>
			</root>

		FP_ON_EXPIRE fpOnExpire:	到期状态回调函数。此函数必须正确传入且不能为空！
			
			不管在什么情况下，AuthorizeApi中会马上同步回调fpOnExpire函数，
			而在AuthorizeApi函数返回后，会视授权情况异步回调fpOnExpire函数。

			授权错误:
				eExpireStatus为ES_FAIL，nExpireDate为0，szExpireTip为错误描述，AuthorizeApi返回-1。
				AuthorizeApi返回后会每隔10分钟异步回调一下fpOnExpire，用于提醒h5api使用方能及时发现错误。
			授权许可成功且离到期日超过一个月:
				eExpireStatus为ES_OK，nExpireDate为实际许可到期日，szExpireTip为形如授权成功的字符串，AuthorizeApi返回0。
				AuthorizeApi返回后不会再异步回调fpOnExpire。
			授权许可成功但离到期日只有一个月:
				eExpireStatus为ES_TO_EXPIRE，nExpireDate为实际许可到期日，szExpireTip为许可授权xml文件中配置的toexpiretip字符串，AuthorizeApi返回0。
				AuthorizeApi返回后会每隔10分钟异步回调一下fpOnExpire，用于提醒h5api使用方再次授权以使不影响正常使用。
			授权许可到期:
				eExpireStatus为ES_EXPIRED，nExpireDate为实际许可到期日，szExpireTip为许可授权xml文件中配置的expiredtip字符串，AuthorizeApi返回1。
				AuthorizeApi返回后会每隔10分钟异步回调一下fpOnExpire，用于提醒h5api使用方再次授权从而能再次正常使用。


	返回值：
		-1:		授权许可错误。表示在授权过程中出现了错误，错误描述在回调函数中提供。
		0:		授权许可成功。
		1:		授权许可到期。

 **/
int FUNCTION_CALL_MODE AuthorizeApi(const char *lpLicenseFile, FP_ON_EXPIRE fpOnExpire);


///	特定市场的状态变化通知	这里 iMktType 和 iConnectID 一一对应,且在API的整个生命周期内,该对应关系不会发生变化
/**	
	int iMktType:	市场类别	见h5hq_const_define.h的定义
	int iConnectID:	该市场对应的连接号
	备注:只有当MktStatu= MKT_INITIALIZED时该市场才可用，才能在该市场执行其他的操作。
		当MKTstatu= MKT_FAIL市场连接被中断，市场取的信息都是不可用的，所以这个信息也应该是用户关心的。
		MktStatu= MKT_INITIALIZING时，用户应该停止使用该市场以及市场代码的所有访问请求，直到初始完成（MKT_INITIALIZED），同时API会清空订阅列表，用户需要在收到MKT_INITIALIZED状态后，重新订阅。
		其余的一些状态，用户可以不关心。
**/
typedef int (FUNCTION_CALL_MODE * ON_MKT_STATUS)(int iMktType, int iConnectID, enumMktStatu MktStatu);

///	根据配置文件,初始化API,配置文件类似于:	
/**	
	<?xml  version="1.0" encoding="GBK"?>
	<supportmkt>
		<market mkt="" user="" password="">
			<svraddr svrip=""  svrport=""/>
			<svraddr svrip=""  svrport=""/>
		</market>
		<market mkt="" user="" password="">
			<svraddr svrip=""  svrport=""/>
			<svraddr svrip=""  svrport=""/>
		</market>
		<market mkt="" user="" password="">
			<svraddr svrip=""  svrport=""/>
			<svraddr svrip=""  svrport=""/>
		</market>
	</supportmkt>
	配置文件列出API支持的所有市场,以及该市场对应的服务器地址,允许配置多地址,API会自行在某个服务器失效时,连接另外一个服务器。

	入参:	
		lpCfgFile:		配置文件名
		MktStatusFunc:	市场状态变化时的回调函数(一般来说,应用关心的是市场初始化事件)
		iTimeOut:		同步调用情况下的超时时间,如果某些请求,采用同步调用模式,那么API在等待多少时间后,
						如果还没有应答返回,则超时返回,
						单位毫秒
	返回:
		0:		成功
		其他:	失败

**/	
int FUNCTION_CALL_MODE InitApi(const char* lpCfgFile, ON_MKT_STATUS MktStatusFunc=NULL, int iTimeOut=1000);



////////////////////////////	以下2函数,用以关心市场初始化的应用	////////////////////////////
///	获得市场概况,必须在初始化完成、， nbvcxczsAqaZr6后才能执行
/**
	入参:
		int iMktType:	市场类别
		int iTimeOut:	超时时间
	出参:
		iErrno:	错误代码	0:成功	其他错误（保留，暂时无用）
	返回:
		市场概要信息指针
**/
IMarketOverview* FUNCTION_CALL_MODE GetSupportMarketInfo(int iMktType, int iTimeOut, int& iErrno);


///	获得特定股票的信息,股票信息包含该股票所有的标准元数据,以及自定义结果集
/**	
	入参:	
		iMktType	市场代码
		lpStockCode	股票代码
**/
IStockInfo* FUNCTION_CALL_MODE GetStockInfo(int iMktType, const char* lpStockCode);




//	订阅股票信息，订阅动作
enum enumSubAction
{
	SUB_CANCEL	= 0,		// 取消订阅
	SUB_ADD		= 1,		// 追加式
	SUB_OVER	= 2,		// 覆盖式
};


/**	无论是自定义结果集还是标准的内置的元数据,API都会为其分配一个ID,编号定义见h5hq_const_define.h
	当需要服务端主推某个自定义结果集或者某个元数据时,可以订阅一个结果集编号的组合
	只要该股票的这几个结果集中的任意一个发生变化,API就会调用注册的回调,以通知应用

	cSubType:	订阅动作，见enumSubAction
	iRegID:	应用自行定义其含义,当行情变化时,API回调应用注册的函数时,确保原样返回
	int iMktType, const char* lpStockCode:	订阅的股票
	int iResults, int* lpResultid:	订阅事件列表（目前能订阅的是:买卖档位,基本报价）
	RecvFunc:	回调函数
	iConnectID:	应用指定从某个连接去订阅行情,对于并不打算自行维护各个市场连接的应用来说,无须传入该值,
				由API根据配置自动为你选择合适的连接，对于使用者来说是不可见的，使用者可以忽略此参数
**/
int FUNCTION_CALL_MODE SubStock(unsigned char cSubType, int iRegID, int iMktType, const char* lpStockCode, int iResults, int* lpResultid, H5_RECV_FUNCTION RecvFunc, int iConnectID=-1);


/**
	批量代码订阅
	在SubStock的基础上能够提供多支代码同时订阅的接口
	int iStockCount: 代码数量
	int* iMktTypes, const char** lpStockCodes: 以iStockCount为长度代码列表数组。 其中iMktTypes[0]与lpStockCodes[0]对应
	H5_RECV_FUNCTION_EX RecvFunc: 支持批量代码的回调函数
	其他参数意义与SubStock的相同

	返回值：0为正常，非0为异常

	说明：
	1. 覆盖式订阅会将当前iRegID的批量订阅(包括所有的iResultID)进行覆盖，但不影响其他iRegID的批量订阅。追加和删除订阅也是同样的情况。
	2. 仅支持代码数为0的覆盖式订阅，操作为清空当前iRegID下所有iResultID订阅的所有代码的批量订阅。不支持代码数为0的追加或删除订阅。
 **/
int FUNCTION_CALL_MODE SubStockEx(unsigned char cSubType, int iRegID, int iStockCount, int* iMktTypes, const char** lpStockCodes, int iResults, int* lpResultid, H5_RECV_FUNCTION_EX RecvFunc, int iConnectID=-1);

//	应用在退出前,释放API占用的资源
int FUNCTION_CALL_MODE UnInitApi();

/*
 *创建一块RealTimeEX内存
 *
 **/
IRealTimeEX *FUNCTION_CALL_MODE CreatRealTimeEX();
/**
	批量请求行情
	在SubStock的基础上能够提供多支代码同时获取的接口
	int iMktTypes:市场
	int iStockCount: 代码数量
	const char** lpStockCodes：代码
	RealTimeEX *lpRealTimeEX：结果返回
 **/
int FUNCTION_CALL_MODE GetRealTimeEx(IRealTimeEX *lpRealTimeEX, int iMktType,int iStockCount, const char** lpStockCodes,int iResultid);


/**
	说明：
	在调用InitApi之前使用，预设h5api参数

	参数说明：
	enumApiParamType eApiParamType: 需要设置的参数类型
	void *lpParam: 需要设置的参数，每种enumApiParamType参数类型会对应一个结构体

	返回值：0为正常，非0异常

**/
int FUNCTION_CALL_MODE SetApiParam(enumApiParamType eApiParamType, void *lpParam);

#ifdef __cplusplus
}
#endif	//	__cplusplus

#endif	//	_H5HQ_API_INTERFACE_H__
