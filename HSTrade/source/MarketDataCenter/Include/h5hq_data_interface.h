#ifndef _H5HQ_DATA_INTERFACE_H__
#define _H5HQ_DATA_INTERFACE_H__

#include "h5hq_const_define.h"


///	市场状态
enum enumMktStatu
{
	MKT_LEVEL2_LOGIN_OTHER_ERROR		=-4,//	Level2登录其他错误：如用户名密码到期等，请查阅h5api日志确认
	MKT_LEVEL2_USER_MAX_ONLINE_LIMIT	=-3,//	Level2在线用户数超出现限制
	MKT_LEVEL2_USER_INFO_ERROR			=-2,//	Level2登录失败: 用户名密码错误
	MKT_FAIL		=-1,//	市场不可用(和市场对应的连接被中断,类似于MKT_CREATED)
	MKT_CREATED		=0,	//	市场刚刚创建,尚未连接
	MKT_CONNECTING	=1,	//	正在创建连接
	MKT_CONNECTED	=2,	//	和市场对应的连接已创建
	MKT_LOGINED		=3, //	市场已登录
	MKT_INITIALIZING=4,	//	市场正在初始化中  
	MKT_GETTINGOVERVIEW=5,	//	取市场全镜像中
	MKT_INITIALIZED	=6,	//	市场已初始化, (就绪状态)
	MKT_MAX_COUNT,		//	市场状态最大个数。无业务含义，一般用于定义数组长度
};

enum enumExpireStatus
{
	ES_FAIL = 0,		// 授权错误。表示在授权过程中出现了错误
	ES_OK = 1,			// 授权成功。未到期且离到期日还有较长时间
	ES_TO_EXPIRE = 2,	// 授权成功。将要到期但还未到期
	ES_EXPIRED = 3,		// 授权失败。已经到期
	ES_MAX_COUNT,		// 最大状态个数。无业务含义，一般用于定义数组长度
};

// SetApiParam接口的参数枚举类型
enum enumApiParamType
{
	APT_LEVEL2_USER_INFO = 0,	// Level2用户名密码信息，对应参数结构类型: LEVEL2_USER_INFO
};

// 对应APT_LEVEL2_USER_INFO类型
typedef struct tagLevel2UserInfo
{
	char	szUserName[48];
	char	szUserPassword[48];
}LEVEL2_USER_INFO, *LP_LEVEL2_USER_INFO;


class IStockInfo;
////////////////	以下给出所有元数据定义(目前仅定义常用的几个,待完善)	////////////////////////////
//	市场初始化信息,股票的静态信息,初始化后不会再变化
class IStockInitInfo : public IHSKnown
{
public:
	virtual const char* FUNCTION_CALL_MODE GetStockCode() = 0;
	virtual const char* FUNCTION_CALL_MODE GetStockName() = 0;
	virtual double		FUNCTION_CALL_MODE GetPrevPrice() = 0;		//	获得昨收盘
	/*
	*交易状态字段可以理解为一个静态的字段，在一般情况下取该字段到初始化信息中去取，
	*但它又不完全是一个静态字段，主推的时候该字段请到ISimpleQuote接口中去取，
	*如果非主推的情况去ISimpleQuote接口去，会触发h5api向行情服务器去请求实时数据。
	*就算是主推或者请求到实时数据后，也会同步对初始化数据里的交易状态字段进行更新，
	*也就是说无论何什么情况下，从IStockInitInfo和ISimpleQuote接口取交易状态都是一致的
	*/
	virtual int			FUNCTION_CALL_MODE GetStockStatus() = 0;	//	获得交易状态: 0:正常 1:停牌
	virtual int			FUNCTION_CALL_MODE GetCodeType() = 0;	
	virtual int			FUNCTION_CALL_MODE GetMkt() = 0;	

	virtual ~IStockInitInfo();
};

//	和特定市场关联的服务器信息
typedef struct tagSvrAddress
{
	char	szIPAddress[16];
	uint16	wPort;
}SVR_ADDRESS, *LPSVR_ADDRESS;


//	市场概要信息,供应用了解当前市场的业务信息,以及提供市场信息的服务器地址信息
class IMarketOverview : public IHSKnown
{
public:
	virtual enumMktStatu FUNCTION_CALL_MODE GetMktStatus() = 0;	//	获得市场状态	0:	市场信息尚未获得,也就是说API目前还没有该市场信息,不能操作请稍等
	virtual int FUNCTION_CALL_MODE GetMarketType() = 0;	//	获得市场代码
	virtual int FUNCTION_CALL_MODE GetChecksum() = 0;	//	市场信息校验和(暂时未实现),应用必须判断当前校验和是否一致
	virtual int FUNCTION_CALL_MODE GetTradeDate() = 0;	//	获得交易日期暂时不支持
	virtual int FUNCTION_CALL_MODE GetMktTime() = 0;	//	获得当前交易时间

	//	获得上级服务器信息
	virtual int FUNCTION_CALL_MODE GetMktSvrCount() = 0;//	获得该市场对应的上级服务器个数
	virtual LPSVR_ADDRESS FUNCTION_CALL_MODE GetMktSvrInfo(int iIndex) = 0;	//	获得市场对应的服务器地址

	//	获得初始化代码表
	virtual int FUNCTION_CALL_MODE GetTotalCount() = 0;	//	股票代码个数
	virtual IStockInitInfo* FUNCTION_CALL_MODE GetStockInitInfo(int iIndex) = 0; //	获得市场下第几个股票的初始化信息

	virtual ~IMarketOverview() = 0;
};

//	基本分时
struct TrendUnit	//	分时单元
{
	double dfLastPrice;		//	最新价
	double dfAveragePrice;	//	均价
	uint64 ddwTotal;		//	成交量
};

class ISimpleTrend : public IHSKnown
{
public:
	//	返回所有分时线分时线,iCount指明有多少根
	/************************************************************************/
	/* 出参 iCount：当前快照分时的数目                                                                     */
	/************************************************************************/
	virtual TrendUnit* FUNCTION_CALL_MODE GetTrend(int& iCount) = 0;	//	获得分时线

	//	按序号取分时线
	virtual TrendUnit* FUNCTION_CALL_MODE GetTrendByIndex(int iIndex) = 0;	//	获得第几根分时线

	virtual ~ISimpleTrend() = 0;
};

//	基本报价(开高低新)
class ISimpleQuote : public IHSKnown
{
public:
	/*
	*交易状态字段可以理解为一个静态的字段，在一般情况下取该字段到初始化信息中去取，
	*但它又不完全是一个静态字段，主推的时候该字段请到ISimpleQuote接口中去取，
	*如果非主推的情况去ISimpleQuote接口去，会触发h5api向行情服务器去请求实时数据。
	*就算是主推或者请求到实时数据后，也会同步对初始化数据里的交易状态字段进行更新，
	*也就是说无论何什么情况下，从IStockInitInfo和ISimpleQuote接口取交易状态都是一致的
	*/
	virtual int    FUNCTION_CALL_MODE GetStockStatus() = 0;		//	获得股票状态: 0:正常 1:停牌
	virtual double FUNCTION_CALL_MODE GetOpenPrice() = 0;		//	获得开盘价
	virtual double FUNCTION_CALL_MODE GetHighPrice() = 0;		//	获得最高价
	virtual double FUNCTION_CALL_MODE GetLowPrice() = 0;		//	获得最低价
	virtual double FUNCTION_CALL_MODE GetLastPrice() = 0;		//	获得最新价
	virtual double FUNCTION_CALL_MODE GetPrevPrice() = 0;		//	获得昨收价
	virtual double FUNCTION_CALL_MODE GetUpPrice() = 0;			//	获得涨停价
	virtual double FUNCTION_CALL_MODE GetDownPrice() = 0;		//	获得跌停价
	virtual double FUNCTION_CALL_MODE GetVolum() = 0;			//	获得成交额
	virtual long   FUNCTION_CALL_MODE GetAmount() = 0;			//	获得成交量,成交量为最小单位
	virtual int   FUNCTION_CALL_MODE GetTimes() = 0;			//	获得当前分钟数
	virtual int   FUNCTION_CALL_MODE GetSecond() = 0;			//	获得当前分钟内第几秒

	virtual double   FUNCTION_CALL_MODE GetPreJieSuanPrice() = 0;			//	获得前结算价（仅期货和个股期权使用）
	virtual long   FUNCTION_CALL_MODE GetPosition() = 0;		// 获取持仓量（仅期货和个股期权使用）

	virtual int	   FUNCTION_CALL_MODE GetHand() = 0;			// 对于股票为每手股数，对于期货为合约乘数等

	virtual unsigned long FUNCTION_CALL_MODE GetTotalBuy() = 0;	// 总买量，目前仅支持组合合约
	virtual unsigned long FUNCTION_CALL_MODE GetTotalSell() = 0;// 总买量，目前仅支持组合合约

	//2013年11月27日	caich	添加个股期权特有字段
	virtual double   FUNCTION_CALL_MODE GetAutionPrice() = 0;			//	获得动态参考价（仅期权）

	virtual unsigned int   FUNCTION_CALL_MODE GetMatchVolume() = 0;			//	获得虚拟匹配数量（仅期权）

	//2014年3月21日 caich 取消未平仓合约数下发
	//virtual unsigned int   FUNCTION_CALL_MODE GetTotalLongPosition() = 0;			//	获得未匹配数量（仅期权）
	
	virtual const char * FUNCTION_CALL_MODE GetTradingphasecode()=0;	//获取产品实施标志（仅期权，4个字节）

	virtual double   FUNCTION_CALL_MODE GetAvgPrice() = 0;			//	获得均价（仅期货）

	virtual double   FUNCTION_CALL_MODE GetJieSuanPrice() = 0;			//	获得结算价（仅期货和个股期权使用）

	virtual ~ISimpleQuote() = 0;
};

//	买卖档位信息
struct LevelInfo
{
	double 	price;		//	价
	long	Amount;		//	量
};

class IBidOffer : public IHSKnown
{
public:	
	virtual int FUNCTION_CALL_MODE GetLevels() = 0;							//	获得档位个数
	virtual LevelInfo *FUNCTION_CALL_MODE GetBidInfo(int iLevelNo) = 0;		//	获得第几个买档的信息
	virtual LevelInfo *FUNCTION_CALL_MODE GetOfferInfo(int iLevelNo) = 0;	//	获得第几个卖档的信息

	virtual ~IBidOffer() = 0;
};


// NEEQ投资者档位
struct NEEQZSLevel
{
	double m_lfPrice;		// 委托价格
	uint64 m_nSize;			// 委托数量
};

//	股转做市投资者档位行情
class INEEQZSLevelQuote : public IHSKnown
{
public:
	virtual uint32 FUNCTION_CALL_MODE GetTimestamp() = 0;						//	获取行情时间戳
	virtual int   FUNCTION_CALL_MODE GetTimes() = 0;							//	获得当前分钟数
	virtual int   FUNCTION_CALL_MODE GetSecond() = 0;							//	获得当前分钟内第几秒
	virtual int FUNCTION_CALL_MODE GetLevels() = 0;								//	获得档位个数
	virtual NEEQZSLevel *FUNCTION_CALL_MODE GetBidLevelInfo(int iLevelNo) = 0;	//	获得第几个买档的信息
	virtual NEEQZSLevel *FUNCTION_CALL_MODE GetOfferLevelInfo(int iLevelNo) = 0;//	获得第几个卖档的信息
	
	virtual ~INEEQZSLevelQuote() = 0;
};

// NEEQ做市商申报
struct NEEQZSOrder
{
	double m_lfPrice;		// 申报价格
	uint64 m_nSize;			// 申报数量
	uint32 m_nTimestamp;	// 申报时间
};

//	股转做市做市商申报行情
class INEEQZSOrderQuote : public IHSKnown
{
public:
	virtual uint32 FUNCTION_CALL_MODE GetTimestamp() = 0;						//	获取行情时间戳
	virtual int   FUNCTION_CALL_MODE GetTimes() = 0;							//	获得当前分钟数
	virtual int   FUNCTION_CALL_MODE GetSecond() = 0;							//	获得当前分钟内第几秒
	virtual int FUNCTION_CALL_MODE GetBidOrderCount() = 0;						//	获得申买笔数
	virtual NEEQZSOrder *FUNCTION_CALL_MODE GetBidOrderInfo(int iIndex) = 0;	//	获得第几个买档的信息
	virtual int FUNCTION_CALL_MODE GetOfferOrderCount() = 0;					//	获得申卖笔数
	virtual NEEQZSOrder *FUNCTION_CALL_MODE GetOfferOrderInfo(int iIndex) = 0;	//	获得第几个卖档的信息
	
	virtual ~INEEQZSOrderQuote() = 0;
};

struct NEEQXYOrder
{
	double			m_fOrderPrice;			//申报价格
	uint64			m_nOrderVolume;			//申报数量
	uint32			m_nDealNO;				//成交约定号
	uint32			m_nOrderTime;			//申报时间
	uint32			m_nStatus;				//记录状态
	uint32			m_nSellSide;			//买卖方向
	char			m_cJYDY[6];				//业务单元
	char			m_cReserved[1];			//备用标志位
};

//股转协议转让申报行情
class INEEQXYOrderQuote : public IHSKnown
{
public:
	virtual uint32 FUNCTION_CALL_MODE GetTimestamp() = 0;						//获取行情时间戳
	virtual int	FUNCTION_CALL_MODE GetAnsType() = 0;							//获取当前行情的应答的数据类型0:表示数据未更新;1:表示无数据;2:表示有数据
	virtual int	FUNCTION_CALL_MODE GetOrderCount() = 0;							//获取当前申报行情数
	virtual NEEQXYOrder *FUNCTION_CALL_MODE GetXYOrderInfo(int iIndex) = 0;		//获得第几条申报行情

	virtual ~INEEQXYOrderQuote() = 0;
};


enum enumKType
{
	KTYPE_UNKNOWN=0,	//	未知
	KTYPE_MINUTE1,		//	1分钟线
	KTYPE_MINUTE5,		//	5分钟线
	KTYPE_MINUTE15,		//	15分钟线
	KTYPE_MINUTE30,		//	30分钟线
	KTYPE_MINUTE60,		//	60分钟线
	KTYPE_MINUTE120,	//	120分钟线
	KTYPE_DAY,			//	日线
	KTYPE_WEEK,			//	周线
	KTYPE_MONTH,		//	月线
	KTYPE_MAX			//	最大类型
};

//	K线
struct KInfo
{
	/************************************************************************/
	/* K线数据中时间说明：
	K线日期，对于分钟数据包含分钟数。
	对于周期大于等于日的k线时间格式为YYYYMMDD,如iTime=20140507，表示2014年5月7日
	对于分钟K线格式为YYMMDDHHmm，如2405071300，由于两位年“24”是基于1990年开始算起的，所以“24”将特殊处理，1990+24=2014年，表示的结果为2014年5月7日13点00分                                                                     */
	/************************************************************************/
	int    iTime;			//	时间
	double dfOpenPrice;		//	开盘价
	double dfHighPrice;		//	最高价
	double dfLowPrice;		//	最低价
	double dfClosePrice;	//	收盘价
	uint64 ddwTotalVolum;	//	总成交量
	double dfAvgePrice;		//	成交金额
};
class IKInfo : public IHSKnown
{
public:
	// 
	/**
	入参:
	iType:K线类型  如 1分钟线 5分钟线该值作为枚举类型定义(为了保持接口的一致性保留，此参数无用)
	iDeriction:	查询方向	0:往前查	1:往后查（目前都是向后查找，此参数无用）
	iOffset:	开始查询的偏移,0作为一个特殊的值,当往前查时,表示从最近一根线往前查,往后查时,表示从第一根线往后查（为了保持接口的一致性保留，此参数无用）

	出参:	iNumber,当前快照k线根线
	**/
	virtual KInfo* FUNCTION_CALL_MODE GetKInfo(enumKType KType, int iDeriction, int iOffset, int& iNumber) = 0;

	virtual KInfo* FUNCTION_CALL_MODE GetKInfoByIndex(int iIndex) = 0;

	virtual ~IKInfo() = 0;
};

////////////////////////	股票所有的结果集	//////////////////////////////////
typedef int (FUNCTION_CALL_MODE * H5_RECV_FUNCTION)(int iRegID, IStockInfo* lpStockInfo, int iResults, int* lpResultid, int iConnectID);
typedef int (FUNCTION_CALL_MODE * H5_RECV_FUNCTION_EX)(int iRegID, int iStockCount, IStockInfo** lpStockInfos, int iResults, int* lpResultid, int iConnectID);
struct  IResultSetInterface;

class IStockInfo : public IHSKnown
{
public:
	/**
	这里的回调应该是临时的,创建一个临时的订阅,当应答回来后,通知应用,同时删除该订阅项
	**/

	//	取基本分时
	/************************************************************************/
	/* 入参：RecvFunc：
	*		RecvFunc=NULL，同步调用，api会阻塞调用，一直到取到数据或者超时返回
	*		RecvFunc不为NULL，异步调用，api将异步向行情服务器获取基本分时数据，
	*		等到行情服务器返回后，异步回调此回调函数
	*	iConnectID：连接号，用户不需要关心
	/************************************************************************/
	virtual ISimpleTrend* 	FUNCTION_CALL_MODE GetSimpleTrendInfo(H5_RECV_FUNCTION RecvFunc=NULL, int iConnectID=-1) = 0;	//	获得简单分时
	
	/************************************************************************/
	/* 入参：
	* lpDate：历史分时日期，格式为“YYYYMMDD”
	* RecvFunc：
	*		RecvFunc=NULL，同步调用，api会阻塞调用，一直到取到数据或者超时返回
	*		RecvFunc不为NULL，异步调用，api将异步向行情服务器获取历史分时数据，
	*		等到行情服务器返回后，异步回调此回调函数
	*	iConnectID：连接号，用户不需要关心
	/************************************************************************/
	virtual ISimpleTrend* 	FUNCTION_CALL_MODE GetHisSimpleTrendInfo(const char* lpDate, H5_RECV_FUNCTION RecvFunc=NULL, int iConnectID=-1) = 0;	//	获得历史简单分时
	
	
	/* 使用说明：订阅之前使用，订阅后，将直接返回当前的快照信息
	* 入参：
	* RecvFunc：
	*		RecvFunc=NULL，同步调用，api会阻塞调用，一直到取到数据或者超时返回
	*		RecvFunc不为NULL，异步调用，如果当前的快照信息比较新或者改代码被订阅了，将直接返回当前的快照；
	*		如果没有被订阅或者数据已经过期，api将异步向行情服务器获取基本报价，等到行情服务器返回后，异步回调此回调函数
	*iConnectID：连接号，用户不需要关心
	/************************************************************************/
	virtual ISimpleQuote*	FUNCTION_CALL_MODE GetSimpleQuoteInfo(H5_RECV_FUNCTION RecvFunc=NULL, int iConnectID=-1) = 0;	//	获得简单报价信息
	
	
	/* 使用说明：订阅之前使用，订阅后，将直接返回当前的快照信息
	* 入参：
	* RecvFunc：
	*		RecvFunc=NULL，同步调用，api会阻塞调用，一直到取到数据或者超时返回
	*		RecvFunc不为NULL，异步调用，如果当前的快照信息比较新或者改代码被订阅了，将直接返回当前的快照；
	*		如果没有被订阅或者数据已经过期，api将异步向行情服务器获取买卖档位信息，等到行情服务器返回后，异步回调此回调函数
	*iConnectID：连接号，用户不需要关心
	/************************************************************************/
	virtual IBidOffer*  	FUNCTION_CALL_MODE GetBidOffer(H5_RECV_FUNCTION RecvFunc=NULL, int iConnectID=-1) = 0;			//	获得买卖档位信息


	//	取股票初始化信息,同步调用
	virtual IStockInitInfo*	FUNCTION_CALL_MODE GetInItInfo()=0;
	
	// 
	/**
	入参:
	iType:K线类型  如 1分钟线 5分钟线该值作为枚举类型定义,只有请求/应答,没有订阅
	iDeriction:	查询方向	0:往前查	1:往后查（目前无用）
	iOffset:	开始查询的偏移,0作为一个特殊的值,当往前查时,表示从最近一根线往前查,往后查时,表示从第一根线往后查
	iNumber:	查询多少根线
	**/
	virtual IKInfo*	FUNCTION_CALL_MODE GetKInfo(enumKType KType, int iDeriction, int iOffset, int& iNumber, H5_RECV_FUNCTION RecvFunc=NULL, int iConnectID=-1) = 0;	//	获得K线


	//	以下是自定义结果集,暂时是个空实现
	virtual IResultSetInterface* FUNCTION_CALL_MODE GetResultSet(int iResultID, H5_RECV_FUNCTION RecvFunc=NULL, int iConnectID=-1) = 0;
	
	//caich 2014年4月15日 添加新接口
	/**
	当前缓存中数据，不会与行情服务器交互，不能在异步回调函数或订阅回调函数之外使用此接口
	**/
	virtual ISimpleTrend* 	FUNCTION_CALL_MODE GetSimpleTrendCache() = 0;	//	获得分时快照。 对应事件 RESULT_TREND=2;基本分时
	virtual IKInfo*	FUNCTION_CALL_MODE GetKLineCache()=0;//获取k线快照。对应事件 RESULT_KLINE=4;基本K线
	virtual IBidOffer*  	FUNCTION_CALL_MODE GetBidOfferCache()=0;//获得买卖档位信息快照。对应事件RESULT_BIDOFF=5;买卖档位信息
	virtual ISimpleQuote*	FUNCTION_CALL_MODE GetSimpleQuoteInfoCache()=0;//获得简单报价信息快照。对应事件RESULT_SIMPLE_QUOTE=3;基本报价

	virtual INEEQZSLevelQuote*	FUNCTION_CALL_MODE GetNEEQZSLevelQuote(H5_RECV_FUNCTION RecvFunc = NULL, int iConnectID = -1) = 0;//获得股转做市投资者档位行情。对应事件RESULT_NEEQ_ZS_LEVEL=6;股转做市投资者档位行情
	virtual INEEQZSLevelQuote*	FUNCTION_CALL_MODE GetNEEQZSLevelQuoteCache() = 0;//获得股转做市投资者档位行情快照。对应事件RESULT_NEEQ_ZS_LEVEL=6;股转做市投资者档位行情
	virtual INEEQZSOrderQuote*	FUNCTION_CALL_MODE GetNEEQZSOrderQuote(H5_RECV_FUNCTION RecvFunc = NULL, int iConnectID = -1) = 0;//获得股转做市做市商申报行情。对应事件RESULT_NEEQ_ZS_ORDER=7;股转做市做市商申报行情
	virtual INEEQZSOrderQuote*	FUNCTION_CALL_MODE GetNEEQZSOrderQuoteCache() = 0;//获得股转做市做市商申报行情快照。对应事件RESULT_NEEQ_ZS_ORDER=7;股转做市做市商申报行情

	virtual INEEQXYOrderQuote*  FUNCTION_CALL_MODE GetNEEQXYOrderQuote(H5_RECV_FUNCTION RecvFunc = NULL, int iConnectID = -1) = 0;//获得股转协议转让申报行情。对应事件RESULT_NEEQ_XY_ORDER=8;股转协议转让申报行情
	virtual INEEQXYOrderQuote*  FUNCTION_CALL_MODE GetNEEQXYOrderQuoteCache() = 0;//获得股转协议转让申报行情。对应事件RESULT_NEEQ_XY_ORDER=8;股转协议转让申报行情

	virtual ~IStockInfo() = 0;
};


class IStockRealTimeInfo :public IHSKnown
{
public:
	virtual int FUNCTION_CALL_MODE GetMarketType()= 0;
	virtual const char * FUNCTION_CALL_MODE GetCode()= 0;
	virtual ISimpleQuote*	FUNCTION_CALL_MODE GetSimpleQuoteInfo()= 0;
	virtual IBidOffer*  	FUNCTION_CALL_MODE GetBidOffer()= 0;
	virtual ~IStockRealTimeInfo() = 0;
};

class IRealTimeEX :public IHSKnown
{
public:
	virtual int FUNCTION_CALL_MODE GetCount() = 0;
	virtual IStockRealTimeInfo *FUNCTION_CALL_MODE GetStockRealTimeInfo(int index)= 0;
	//释放内存。
	//如果是内存对象不需要使用此接口，当c++使用时候当做指针使用，必须调用此接口,delphi接口中不用显示调用。
	virtual unsigned long  FUNCTION_CALL_MODE Release() =  0;
	virtual ~IRealTimeEX()=0;
};

//////////////////////////////	自定义结果集暂时不实现	////////////////////////////////
//	作为自定义结果集类型使用,接口最完备,但是,按照结果集取值,操作相对复杂
struct  IResultSetInterface :public IHSKnown
{
	/// 返回重复组内记录个数
	virtual int FUNCTION_CALL_MODE GetRecordCount() const = 0;
	virtual int FUNCTION_CALL_MODE GetValurByField(int iFieldID) const = 0;

	virtual ~IResultSetInterface() = 0;
};
#endif
