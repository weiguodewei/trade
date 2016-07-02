#pragma once
#include "..\HSQuanTrade\StrategyBase.h"




typedef struct tagSt_Condtion_Type 
{
	char m_szSymbol[8];
	int m_iType;
}St_Condition_Type;

class CStrategyArbitrageImpl :
	public CStrategyBase
{
public:
	CStrategyArbitrageImpl(void);
	virtual ~CStrategyArbitrageImpl(void);

	virtual CStrategyBase * NewInstance() { 

		CStrategyBase* pStrategy = new CStrategyArbitrageImpl();
		if (pStrategy)
		{
			pStrategy->CreateStatisticObject();
		}
		return pStrategy;

	};
	virtual void BeginRun();
	virtual void OnTradeReturn(char * ins,char * time,int orderref,int ordersystemid,char direction,char offsetflag,int vol,double price);
	virtual void OnOrderReturn(int orderref,int ordersystemid,char orderstatus,char bs,char kp,char * ins);
	virtual void OnErrorRtn(int code,char * errormsg);

	virtual void OnSubcode();
	virtual void Tick(CTickData * pt);

	
	virtual void SetParam(CString paramname,CString value);
	
	virtual void SetParam(CString value);

	virtual CString GetParamValue();

	virtual CString GetParamValue(CString paramname);
	virtual void CreateStatisticObject();
	virtual bool IsRightTickTime(const char* szTime);

	virtual void ResumePostionInfo(CString strPosInfo);
	virtual CString GetPostionInfo();


protected:

	std::map<string, int> m_arCodeSubs;

	CString m_strStdCode;
	CString m_strCode1;
	CString m_strCode2;

	bool m_bIsStdArbitrage;
	CString m_sOpenCondtion;
	double m_dbOpenPriceDiv;
	int m_iOpenNum;

	int m_iPostionNum;

	CString m_sCloseCondtion1;
	double m_dbClosePriceDiv1;

	CString m_sCloseCondtion2;
	double m_dbClosePriceDiv2;

	bool m_bStrategyOrder;
	int m_iFirstOpenFlag;

	int m_iOpenDir;//开仓方向

	int m_iMaxDayOpenNum;//当天累计开仓次数上限, 一开一平为一次
	int m_iTodayDayOpenNum;//当天累计开仓次数

	int m_iOpenPosAcc;
	int m_iClosePosAcc;
	int m_iMinutesPreStop;//收盘前n分钟不开仓

	//夜盘收盘时间, 需要读取配置文件
	char m_szCloseTime1[16];
	char m_szCloseTime2[16];

	bool m_bEnabled;//启用，停用标志， 停用时不改变状态，不开平仓，行情仍上报

	void ReadCloseTimeCfg();

	void OnStdArbitrageTick(CTickData* pt);
	bool IsRightOpenPostionTime(const char* szTime);

	static St_Condition_Type s_arCondtionType[];
	
	int GetConditionType(const char* szSymbol);

	//standard arbitrage contract
	double CalcDivPrice(CTickData* pt);
	bool ConditionJudge(double dbDiv, int bOpenOrClose);
	bool CanOpenPostion(CTickData* pt, double &dbDiv);
	bool CanClosePostion(CTickData* pt, double &dbDiv);

	int m_iLastRef;
	

	void OnNotifyPostion(const char* szCode, int iOpenClose, int iBuySell, int iPosNum, double dbAvgPrice, const char* szOpenTime);
	//custom arbitrage contracts

	
	int m_nLeg1Status;
	int m_nLeg2Status;
	int m_iLeg1OpenPosAcc;
	int m_iLeg2OpenPosAcc;
	int m_iLeg1ClosePosAcc;
	int m_iLeg2ClosePosAcc;
	int m_iLeg1Ref;
	int m_iLeg2Ref;

	//double m_dbPriceSum;
	double m_dbLeg1PriceSum;
	double m_dbLeg2PriceSum;

	CString m_strOpenTime;
	CString m_strLeg1OpenTime;
	CString m_strLeg2OpenTime;


	CTickData m_oLeg1Tick;
	CTickData m_oLeg2Tick;

	int m_iLeg1VolMul; //合约乘数
	int m_iLeg2VolMul; 

	int GetContractVolMul(const char* szCode);

	
};

