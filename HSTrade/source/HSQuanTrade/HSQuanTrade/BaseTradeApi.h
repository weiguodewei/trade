#pragma once
#include <string>
#include "data.h"


#define USERTOKENLEN	256		///令牌长度


class CTradeInterface;

class CStrategyManager;

class CBaseTradeApi
{
public:
	CBaseTradeApi();

	~CBaseTradeApi(void);

	virtual void init(CString serverAddress) = 0;

	char m_szTradeServer[256];

	char m_szPwd[16];

	char m_szUserID[16];

	char m_szBroker[16];

	int m_nOrderRefOffset;

	//一些帐号相关信息
	int g_iSystemNo;

	int g_iBranchNo;

	bool g_bLogin;

	CString m_exchAccount;

	char g_cUserToken[USERTOKENLEN];

	HWND m_hMsgWnd;

	int m_bRisk;		//是否启用风控

	void Log(char * msg);

	virtual int InsertOrder(char * instrument, int count, char direction, char offsetflag, double price,int orderref)=0;

	virtual int WithDraw(int entrustno,char *sCode)=0;

	virtual int QueryEntrust(int entrustno,int reqid)=0;

	virtual int QueryPosition(char *account){return 0;};

	virtual int QueryTrade(char *account){return 0;};

	virtual int QueryFund(char *account){return 0;};

	virtual void RtnOrder(int orderref,int ordersysid,char status,char bs,char kp,char * ins,int entrustAmount,int dealAmount,int restAmount)=0;

	virtual void RtnTrade(char *userid,int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins)=0;
	
	void SetInterface(CTradeInterface * pt);

	virtual void OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid)=0; 

	//报单状态信息
	virtual bool IsWeiBao(char status)=0;    //未报

	virtual bool IsDaiBao(char status)=0;    //待报

	virtual bool IsYiBao(char status)=0;     //已报

	virtual bool IsBuChe(char status)=0;     //部撤

	virtual bool IsYiChe(char status)=0;     //已撤

	virtual bool IsBuCheng(char status)=0;   //部成

	virtual bool IsYiCheng(char status)=0;   //已成

	virtual bool IsFeiDan(char status)=0;    //废单

	virtual std::string TransBS(char bs)=0;

	virtual std::string TransKP(char kp)=0;

	virtual std::string TransStatus(char st)=0;


	//风控
	void SetRiskInfo(double margin,int count,double pl,int open);

	bool CheckRisk(char *code,int count,double price,char bs,char kp,int type); //type=0 下单  type=1 撤单

	void UpdateRiskInfo(char *code,int vol,double price,char bs,char kp);

	bool ExistOrder(int orderref);

	void AddOrder(int orderref);


	//判断接口类型  因为不同接口类型有些字段  比如报单状态差异较大
	//ctp	0;	ufx_f	1;	ufx_s	2;	ufx_auto	3;	ufx_opt		4  
	virtual int GetInterfaceType()=0;


protected:

public:

	CTradeInterface * m_pTradeInterface;

	//每个账户的数据
	CAccData data;

};

