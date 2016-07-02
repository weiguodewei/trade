#pragma once
#define TRADE_STATUS_INIT 0
#define TRADE_STATUS_BK_SIGNAL 1
#define TRADE_STATUS_BK_TRADE  2
#define TRADE_STATUS_BK_CANCEL 3
#define TRADE_STATUS_SP_SIGNAL 4
#define TRADE_STATUS_SP_TRADE  5
#define TRADE_STATUS_SP_CANCEL 6
#define TRADE_STATUS_SK_SIGNAL 7
#define TRADE_STATUS_SK_TRADE  8
#define TRADE_STATUS_SK_CANCEL 9
#define TRADE_STATUS_BP_SIGNAL 10
#define TRADE_STATUS_BP_TRADE 11
#define TRADE_STATUS_BP_CANCEL 12


#define	WRITE_LOG			CinfoLogger::instance()->LogInfo

#define	STRING_FORMAT		CinfoLogger::instance()->LogInfo

#define	ENTER_FUNC			CinfoLogger::instance()->EnterFunction

#define	LEAVE_FUNC			CinfoLogger::instance()->LeaveFunction

#define	GET_TIME_STRING		CinfoLogger::instance()->GetCurTimeToString


#define TRADE_MSG_TYPE_LOG 0
#define TRADE_MSG_TYPE_LOG_PRV 1
#define TRADE_MSG_TYPE_OTHER 2
#define TRADE_MSG_TYPE_HQ 3
#define TRADE_MSG_TYPE_QUERY 4
#define TRADE_MSG_TYPE_SIG 5
#define TRADE_MSG_TYPE_ARBITRAGE_HQ 6
#define TRADE_MSG_TYPE_POSTION_INFO 7
#define TRADE_MSG_TYPE_CODE_QUERY_DONE 8


//////////////////
///订单管理类型
//////////////////
#define ORDER_TYPE_DONOTHING			-1     //下单后不进行订单管理
#define ORDER_TYPE_TIMEOUT_ABORT		0      //下单后超时未成交或者部分成交，撤单，然后放弃
#define ORDER_TYPE_ZUIJIA_NEWPRICE      1      //下单后未成交，根据最新价的对价追价
#define ORDER_TYPE_ZUIJIA_JIACHA		2      //下单后，当行情波动了指定的价差后还没有成交，进行追价


//////////////////
///CTP接口买卖方向
//////////////////
#define POSTION_DIRECTION_BUY '0'
#define POSTION_DIRECTION_SELL '1'

//////////////////
///UFX接口买卖方向
//////////////////
#define UFX_ORDER_BUY '1'
#define UFX_ORDER_SELL '2'

//////////////////
///期货UFX接口开平方向
//////////////////
#define UFX_ORDER_OPEN '1'
#define UFX_ORDER_CLOSE '2'
#define UFX_ORDER_CLOSETODAY '4'

//////////////////
///股票期权UFX接口开平方向
//////////////////
#define UFX_SP_OPEN 'O'
#define UFX_SP_CLOSE 'C'
#define UFX_SP_EXOPT 'X'
#define UFX_SP_AUTOEXOPT 'A'


//////////////////
///回报类型
//////////////////
#define RTN_TYPE_ORDER   0
#define RTN_TYPE_TRADE   1



//////////////////
///CTP接口开平方向
//////////////////
#define HS_ORDER_AllTraded '0'   //已成
#define HS_ORDER_PartTraded '1'	 //部成
#define HS_ORDER_Unknown 'a'	 //待报
#define HS_ORDER_HASREPORT '3'	 //已报
#define HS_ORDER_CANCELED '5'	 //已撤



//////////////////
///CTP接口开平方向
//////////////////
///开仓
#define HS_THOST_FTDC_OF_Open '0'
///平仓
#define HS_THOST_FTDC_OF_Close '1'
///强平
#define HS_THOST_FTDC_OF_ForceClose '2'
///平今
#define HS_THOST_FTDC_OF_CloseToday '3'
///平昨
#define HS_THOST_FTDC_OF_CloseYesterday '4'
///强减
#define HS_THOST_FTDC_OF_ForceOff '5'
///本地强平
#define HS_THOST_FTDC_OF_LocalForceClose '6'


//////////////////
///交易接口类型
//////////////////
#define TRADE_CTP		0
#define TRADE_UFX_F		1
#define TRADE_UFX_S		2
#define TRADE_UFX_AUTO	3
#define TRADE_UFX_OPT	4



struct CTickData
{
	CTickData()
	{
		memset(szInstrument, 0, 32);
		lastprice = 0.0;
	}
	char szDate[9];
	char szTime[32];
	char szInstrument[32];
	double askprice[5];
	double bidprice[5];
	int   askvol[5];
	int   bidvol[5];	
	double lastprice;				//最新价
	double UpperLimitPrice;			//涨停价
	long  Volume;					//成交量
	double highestprice;			//最高价
	double preClosePrice;			//昨收盘
	double LowerLimitPrice;			//跌停价

};


//指令信息
struct COrderInfo
{
	char szInstrument[32]; 
	int ref;
	double orderprice;
	double tradeprice;
	int tradecount;
	int ordercount;
	int withdrawcount;
	char bs;
	char kp;
	char selfclose;
	//其他信息
	//订单管理类型，
	int ordertype;
	int timeOut;
	double jiacha;
	char account[20];
};

struct StockInfo
{
	char code[31];				//合约代码
	char  Name[9];				//证券名称
	long  J_start;				//上市日期
	double ActiveCapital;		//流通股本
	int exchid;                 //交易所id 
};

//期货合约
struct FutuCodeInfo
{
	char code[31];				//合约代码
	char exchid[9];				//交易所代码
	char name[21];				//合约名称
	int iVolumeMultiple;		//合约乘数
	double LMarginRatio;		//多头保证金
	double SMarginRatio;		//空头保证金
	double PriceTick;			//最小价差

};


struct ArbitrageCodeInfo
{
	char sCode[31];				//合约代码
	char sExchid[9];			//交易所代码
	
	int iMinTradeVol;
	int iMaxTradeVol;


};

//股票期权
struct StockOpt
{
	char code[31];
	int exchid;
	char name[20];
	int type;					//看涨看跌

};


struct CHoldsinfo
{
	char szInstrument[32]; 
	int PosiDirection;			//多空
	int YdPosition;				//昨仓
	int TdPosition;				//今仓
	int Position;				//总持仓
	int LongFrozen;				//多头冻结
	int ShortFrozen;			//空头冻结
	int TdFrozen;				//今冻结
	int YdFrozen;				//昨冻结
	int TdAvaiable;				//今可用
	int YdAvaiable;				//昨可用
	double dTradePrice;			//成本价

};

//实时持仓，计算平仓盈亏
struct CHoldsEx
{	
	char szInstrument[32]; 
	int LongPosition;			//多仓
	int ShortPosition;			//空仓
	double dLongTradePrice;		//多成本价
	double dShortTradePrice;	//多成本价
	double dLongMargin;			//多头保证金
	double dShortMargin;		//空头保证金

	CHoldsEx()
	{
		memset(this,0,sizeof(CHoldsEx));
	}

};


//合约状态信息
struct CCodeStatus
{
	char szInstrument[32];
	char status;
	char entertime[9];

};

//委托、查询信息
struct CEntrustInfo
{
	char *account;
	char szInstrument[32];
	int entrustno;
	int status;
	char bs;
	char kp;
	int orderAmount;
	int tradeAmount;
	int cancelAmount;
};

//K线数据
struct CKStrdata
{
#ifdef _DEBUG

	~CKStrdata()
	{
		int i = 0;
	}
#endif
	int    type; 
//	char   code[31];
	//char   name[15];
	double priceH;
	double priceK;
	double priceL;
	double priceS;
	double vol;
	char    curDate[9];
	char   curTime[5];


};
