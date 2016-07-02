#pragma once

#include "include/t2sdk_interface.h"

#include "BaseTradeApi.h"

#define SYNCSEND		0	///同步发送

#define ASYNCSEND		1	///异步发送

#define USERSECTION		"user"

#define T2SDK           "t2sdk"

#define ISSUE_TYPE_REALTIME_SECU 	33099//12			///证券成交类型

class CUFXTrade;
class CSECUTrade;
//CBusiness
//这里是期货的实现类
class CBusiness
{
public:
	CBusiness();

	virtual ~CBusiness();

	void SetConfig(CConfigInterface *pConfig)
	{
		m_pConfig = pConfig;
	}

	int DealReceive(int funcid,IF2UnPacker *unpacker,int reqid);

	int onRsp331100(IF2UnPacker *unpacker,int reqid);

	int onRsp338202(IF2UnPacker *unpacker,int reqid);

	int onRsp338217(IF2UnPacker *unpacker,int reqid);

	int onRsp338301(IF2UnPacker *unpacker,int reqid);

	//-----------------------期货----------------------------
	int Login(char * account,char * pwd,int reqid);

	int FutureEntrust(int systemid,char * fundaccount,char * pwd,char * instrument, int count, char direction, char offsetflag, double price,int reqid);

	int WithDraw(char * account,char * pwd,int ref,int reqid);

	int QueryEntrust(char * account,char * pwd,int entrustno,int reqid);

	int FutureAccountInfo(char * account,char * pwd,int reqid);

	int QryFuturePosition(char * account,char * pwd,int reqid);

	int Subscribe(int issue_type,int reqid); ///发布类型

	int CancelSub(int issueType,int reqid);


	
private:
	CConfigInterface *m_pConfig;

	int SendBizMessage(IF2Packer *lpF2Packer, int nFunction, int nIssueType, int nRequestID, void** ppRetData, int* pnItems, void *pRspInfo);

public:	

	CConnectionInterface * g_pConnection;

	CUFXTrade * m_pTrade;
};




//证券的实现类
class CSECUBusiness
{
public:
	CSECUBusiness();

	virtual ~CSECUBusiness();

	void SetConfig(CConfigInterface *pConfig)
	{
		m_SecuConfig = pConfig;
	}

	int DealReceive(int funcid,IF2UnPacker *unpacker,int reqid);

	int onRsp331100(IF2UnPacker *unpacker,int reqid);
	
	int onRsp333002(IF2UnPacker *unpacker,int reqid);    //下单

	int onRsp333017(IF2UnPacker *unpacker,int reqid);    //撤单
	



	


	//-----------------------证券----------------------------

	int Login(char * account,char * pwd,int reqid);

	int SecuEntrust(int systemid,char * fundaccount,char * pwd,char * instrument, int count, char direction, double price,int reqid);

	int SearchSecuEntrust(char * fundaccount,char * pwd,char * exchtype,int reqid);

	int SecuEntrustWithdraw(char * fundaccount,char * pwd,int entrustno,int reqid);

	int SecuRealtimeQry(char * fundaccount,char * pwd,int reqid);

    int SecuStockQry(char * fundaccount,char * pwd,int reqid);

	int ClientFundAllQry(char * fundaccount,char * pwd,int reqid);

	int Subscribe(int issue_type,int reqid); ///发布类型

	int CancelSub(int issueType,int reqid);


private:
	CConfigInterface *m_SecuConfig;

	int SendBizMessage(IF2Packer *lpF2Packer, int nFunction, int nIssueType, int nRequestID, void** ppRetData, int* pnItems, void *pRspInfo);


public:	

	CConnectionInterface * g_SecuConnection;

	CSECUTrade * m_secuTrade;
};



//回调

class CCallback : public CCallbackInterface
{
public:
    // 因为CCallbackInterface的最终纯虚基类是IKnown，所以需要实现一下这3个方法
    unsigned long  FUNCTION_CALL_MODE QueryInterface(const char *iid, IKnown **ppv);

    unsigned long  FUNCTION_CALL_MODE AddRef();

    unsigned long  FUNCTION_CALL_MODE Release();
	
    // 各种事件发生时的回调方法，实际使用时可以根据需要来选择实现，对于不需要的事件回调方法，可直接return
    // Reserved?为保留方法，为以后扩展做准备，实现时可直接return或return 0。
    void FUNCTION_CALL_MODE OnConnect(CConnectionInterface *lpConnection);

    void FUNCTION_CALL_MODE OnSafeConnect(CConnectionInterface *lpConnection);

    void FUNCTION_CALL_MODE OnRegister(CConnectionInterface *lpConnection);

    void FUNCTION_CALL_MODE OnClose(CConnectionInterface *lpConnection);

    void FUNCTION_CALL_MODE OnSent(CConnectionInterface *lpConnection, int hSend, void *reserved1, void *reserved2, int nQueuingData);

    void FUNCTION_CALL_MODE Reserved1(void *a, void *b, void *c, void *d);

    void FUNCTION_CALL_MODE Reserved2(void *a, void *b, void *c, void *d);

    int  FUNCTION_CALL_MODE Reserved3();

    void FUNCTION_CALL_MODE Reserved4();

    void FUNCTION_CALL_MODE Reserved5();

    void FUNCTION_CALL_MODE Reserved6();

    void FUNCTION_CALL_MODE Reserved7();

    void FUNCTION_CALL_MODE OnReceivedBiz(CConnectionInterface *lpConnection, int hSend, const void *lpUnPackerOrStr, int nResult);

	void FUNCTION_CALL_MODE OnReceivedBizEx(CConnectionInterface *lpConnection, int hSend, LPRET_DATA lpRetData, const void *lpUnpackerOrStr, int nResult);
	
	void FUNCTION_CALL_MODE OnReceivedBizMsg(CConnectionInterface *lpConnection, int hSend, IBizMessage* lpMsg);

public:

	CBusiness * m_pBusiness;     //期货

	CSECUBusiness * m_secuBusiness;   //证券

	void CallbackLog(char *msg);
};


//期货UFX
class CUFXTrade :public  CBaseTradeApi
{
public:
	CUFXTrade(void);

	~CUFXTrade(void);

	virtual void init(CString serverAddress);

	CConfigInterface * m_lpConfig; 

	CBusiness g_szBusiness;

	virtual int InsertOrder(char * instrument, int count, char direction, char offsetflag, double price,int orderref);

	virtual int WithDraw(int entrustno,char *sCode);

	virtual int QueryEntrust(int entrustno,int reqid);

	virtual void RtnOrder(int orderref,int ordersysid,char status,char bs,char kp,char * ins,int entrustAmount,int dealAmount,int restAmount);

	virtual void RtnTrade(char *userid,int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins);

	virtual void OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid); 

	CCallback  m_szCallback;


	//报单状态信息
	virtual bool IsWeiBao(char status){return false;};    //未报

	virtual bool IsDaiBao(char status){return false;};    //待报

	virtual bool IsYiBao(char status){return false;};     //已报

	virtual bool IsBuChe(char status){return false;};     //部撤

	virtual bool IsYiChe(char status){return false;};     //已撤

	virtual bool IsBuCheng(char status){return false;};   //部成

	virtual bool IsYiCheng(char status){return false;};   //已成

	virtual bool IsFeiDan(char status){return false;};    //废单

	virtual std::string TransBS(char bs){return "";};

	virtual std::string TransKP(char kp){return "";};

	virtual std::string TransStatus(char st){return "";};
};


//证券UFX
class CSECUTrade:public CBaseTradeApi
{
public:
	CSECUTrade(void);

	~CSECUTrade(void);

	virtual void init(CString serverAddress);

	CConfigInterface * m_secuConfig;

	CSECUBusiness g_secuBusiness;

	virtual int InsertOrder(char* instrument,int count,char direction,char offsetflag,double price,int reqid);

	virtual int WithDraw(int entrustno,char *sCode);

	virtual int QueryEntrust(int entrustno,int reqid);

	virtual void RtnOrder(int orderref,int ordersysid,char status,char bs,char kp,char * ins,int entrustAmount,int dealAmount,int restAmount);

	virtual void RtnTrade(char *userid,int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins);

	virtual void OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid);

	CCallback  m_secuCallback;

	//报单状态信息
	virtual bool IsWeiBao(char status){return false;};    //未报

	virtual bool IsDaiBao(char status){return false;};    //待报

	virtual bool IsYiBao(char status){return false;};     //已报

	virtual bool IsBuChe(char status){return false;};     //部撤

	virtual bool IsYiChe(char status){return false;};     //已撤

	virtual bool IsBuCheng(char status){return false;};   //部成

	virtual bool IsYiCheng(char status){return false;};   //已成

	virtual bool IsFeiDan(char status){return false;};    //废单

	virtual std::string TransBS(char bs){return "";};

	virtual std::string TransKP(char kp){return "";};

	virtual std::string TransStatus(char st){return "";};

	virtual int GetInterfaceType(){return TRADE_UFX_S;};

};


