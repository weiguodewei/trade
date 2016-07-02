#if !defined(HUNDSUNMDCENTERSTRUCT_H)
#define HUNDSUNMDCENTERSTRUCT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PackerVersion 0x20

/////////////////////////////////////////////////////////////////////////
///THundsunFtdcErrorIDType是一个错误代码类型
/////////////////////////////////////////////////////////////////////////
typedef int THundsunFtdcErrorIDType;

/////////////////////////////////////////////////////////////////////////
///THundsunFtdcErrorMsgType是一个错误信息类型
/////////////////////////////////////////////////////////////////////////
typedef char THundsunFtdcErrorMsgType[81];

/////////////////////////////////////////////////////////////////////////
///THundsunExchangeType是Api创建交易所类型
/////////////////////////////////////////////////////////////////////////
///上海证券交易所
#define HUNDSUN_FTDC_SH        1
///深圳证券交易所
#define HUNDSUN_FTDC_SZ        2
//	上海个股期权
#define HUNDSUN_FTDC_GQ_SH     3
//	深圳个股期权
#define HUNDSUN_FTDC_GQ_SZ	   4
///上海期货交易所
#define HUNDSUN_FTDC_SHFE      11
///郑州商品交易所
#define HUNDSUN_FTDC_CZCE      12
///大连商品交易所
#define HUNDSUN_FTDC_DCE       13
///中国金融期货交易所
#define HUNDSUN_FTDC_CFFEX     14
///上海国际能源交易中心股份有限公司
#define HUNDSUN_FTDC_INE       15

typedef int THundsunExchangeType;

/////////////////////////////////////////////////////////////////////////
///THundsunFtdcInstrument是一个合约代码类型
/////////////////////////////////////////////////////////////////////////
typedef char THundsunFtdcInstrument[31];

/////////////////////////////////////////////////////////////////////////
///THundsunFtdcInstrumentName是一个合约名称类型
/////////////////////////////////////////////////////////////////////////
typedef char THundsunFtdcInstrumentName[21];

/////////////////////////////////////////////////////////////////////////
///THundsunMarkerDataType是Api创建行情类型
/////////////////////////////////////////////////////////////////////////
///DBF
#define HUNDSUN_FTDC_DBF '1'
///H5
#define HUNDSUN_FTDC_H5 '2'
///Future
#define HUNDSUN_FTDC_Futu '3'
///H5SDK
#define HUNDSUN_FTDC_H5SDK '4'

typedef char THundsunMarkerDataType;

/////////////////////////////////////////////////////////////////////////
///THundsunFtdcPriceType是一个价格类型
/////////////////////////////////////////////////////////////////////////
typedef double THundsunFtdcPriceType;

/////////////////////////////////////////////////////////////////////////
///THundsunFtdcVolumeType是一个数量类型
/////////////////////////////////////////////////////////////////////////
typedef int THundsunFtdcVolumeType;

/////////////////////////////////////////////////////////////////////////
///THundsunFtdcVolumeLongType是一个数量长整类型
/////////////////////////////////////////////////////////////////////////
typedef long THundsunFtdcVolumeLongType;

/////////////////////////////////////////////////////////////////////////
///THundsunFtdcFundAccountType是一个资金账号
/////////////////////////////////////////////////////////////////////////
typedef char THundsunFtdcFundAccountType[19];

/////////////////////////////////////////////////////////////////////////
///THundsunFtdcPasswordType是一个密码类型
/////////////////////////////////////////////////////////////////////////
typedef char THundsunFtdcPasswordType[41];

/////////////////////////////////////////////////////////////////////////
///THundsunFtdcDateType是一个日期类型
/////////////////////////////////////////////////////////////////////////
typedef int THundsunFtdcDateType;

/////////////////////////////////////////////////////////////////////////
///THundsunFtdcSessionIDType是一个会话编号类型
/////////////////////////////////////////////////////////////////////////
typedef int THundsunFtdcSessionIDType;

/////////////////////////////////////////////////////////////////////////
///THundsunFtdcUserTokenType是一个用户密钥类型
/////////////////////////////////////////////////////////////////////////
typedef char THundsunFtdcUserTokenType[512];


///错误信息
struct CHundsunFtdcRspInfoField
{
	///错误代码
    THundsunFtdcErrorIDType   ErrorID; 
	///错误提示
    THundsunFtdcErrorMsgType  ErrorMsg;
};

///用户登录请求
struct CHundsunFtdcReqUserLoginField
{
	///行情类型
	THundsunMarkerDataType    MarkerDataType;
	///资金账号
	THundsunFtdcFundAccountType FundAccount;
	///密码
	THundsunFtdcPasswordType  Password;
};

///用户登录应答
struct CHundsunFtdcRspUserLoginField
{
	///交易日
	THundsunFtdcDateType      TradingDay;
	///行情类型
	THundsunMarkerDataType    MarkerDataType;
	///资金账号
	THundsunFtdcFundAccountType FundAccount;
	///用户密钥
	THundsunFtdcUserTokenType UserToken;
	///会话编号
	THundsunFtdcSessionIDType SessionID;
};

///合约查询请求
struct CHundsunFtdcQryInstrumentField
{
	///合约代码
	THundsunFtdcInstrument    Instrument;
	///交易所代码
	THundsunExchangeType      ExchangeID;
};

///合约
struct CHundsunFtdcInstrumentField
{
	///合约代码
	THundsunFtdcInstrument    Instrument;
	///交易所代码
	THundsunExchangeType      ExchangeID;
	///合约名称
	THundsunFtdcInstrumentName InstrumentName;
};

struct CHundsunSpecificInstrumentField
{
	///交易所代码
	THundsunExchangeType      ExchangeID;
	///合约代码
	THundsunFtdcInstrument    Instrument;
};

struct CHundsunDepthMarketDataField
{
	//对照关系                                                   上海        深圳         期货
	THundsunFtdcVolumeType    UpdateTime;          //最后修改时间                         UpdateTime
	THundsunFtdcVolumeType    UpdateMillisec;      //最后修改毫秒                         UpdateMillisec
	THundsunExchangeType      ExchangeID;          //交易所代码                           ExchangeID
	THundsunFtdcInstrument    Instrument;          //合约代码    S1          HQZQDM       InstrumentID
	THundsunFtdcInstrumentName InstrumentName;     //合约名称    S2          HQZQJC       InstrumentName
	THundsunFtdcPriceType     LastPrice;           //最新价      S8          HQZJCJ       LastPrice
	THundsunFtdcPriceType     PreSettlementPrice;  //上次结算价                           PreSettlementPrice
	THundsunFtdcPriceType     PreClosePrice;       //昨收盘价    S3          HQZRSP       PreClosePrice
	THundsunFtdcPriceType     PreOpenInterest;     //昨持仓量                             PreOpenInterest
	THundsunFtdcPriceType     OpenPrice;           //今开盘价    S4          HQJRKP       OpenPrice
	THundsunFtdcPriceType     HighestPrice;        //今最高价    S6          HQZGCJ       HighestPrice
	THundsunFtdcPriceType     LowestPrice;         //今最低价    S7          HQZDCJ       LowestPrice
	THundsunFtdcPriceType     Turnover;            //今成交金额  S5          HQCJJE       Turnover
	THundsunFtdcVolumeLongType Volume;             //成交数量    S11         HQCJSL       Volume
	THundsunFtdcVolumeType    Count;               //成交笔数                HQCJBS
	THundsunFtdcPriceType     OpenInterest;        //持仓量                               OpenInterest
	THundsunFtdcPriceType     ClosePrice;          //今收盘价                             ClosePrice
	THundsunFtdcPriceType     SettlementPrice;     //本次结算价                           SettlementPrice
	THundsunFtdcPriceType     UpperLimitPrice;     //涨停板价                             UpperLimitPrice
	THundsunFtdcPriceType     LowerLimitPrice;     //跌停板价                             LowerLimitPrice
	THundsunFtdcPriceType     BuyPrice1;           //买一价      S9          HQBJW1       BidPrice1
	THundsunFtdcVolumeType    BuyAmount1;          //买一量      S15         HQBSL1       BidVolume1
	THundsunFtdcPriceType     SalePrice1;          //卖一价      S10         HQSJW1       AskPrice1
	THundsunFtdcVolumeType    SaleAmount1;         //卖一量      S21         HQSSL1       AskVolume1
	THundsunFtdcPriceType     BuyPrice2;           //买二价      S16         HQBJW2       BidPrice2
	THundsunFtdcVolumeType    BuyAmount2;          //买二量      S17         HQBSL2       BidVolume2
	THundsunFtdcPriceType     SalePrice2;          //卖二价      S22         HQSJW2       AskPrice2
	THundsunFtdcVolumeType    SaleAmount2;         //卖二量      S23         HQSSL2       AskVolume2
	THundsunFtdcPriceType     BuyPrice3;           //买三价      S18         HQBJW3       BidPrice3
	THundsunFtdcVolumeType    BuyAmount3;          //买三量      S19         HQBSL3       BidVolume3
	THundsunFtdcPriceType     SalePrice3;          //卖三价      S24         HQSJW3       AskPrice3
	THundsunFtdcVolumeType    SaleAmount3;         //卖三量      S25         HQSSL3       AskVolume3
	THundsunFtdcPriceType     BuyPrice4;           //买四价      S26         HQBJW4       BidPrice4
	THundsunFtdcVolumeType    BuyAmount4;          //买四量      S27         HQBSL4       BidVolume4
	THundsunFtdcPriceType     SalePrice4;          //卖四价      S30         HQSJW4       AskPrice4
	THundsunFtdcVolumeType    SaleAmount4;         //卖四量      S31         HQSSL4       AskVolume4
	THundsunFtdcPriceType     BuyPrice5;           //买五价      S28         HQBJW5       BidPrice5
	THundsunFtdcVolumeType    BuyAmount5;          //买五量      S29         HQBSL5       BidVolume5
	THundsunFtdcPriceType     SalePrice5;          //卖五价      S32         HQSJW5       AskPrice5
	THundsunFtdcVolumeType    SaleAmount5;         //卖五量      S33         HQSSL5       AskVolume5
	THundsunFtdcPriceType     BuyPrice6;           //买六价
	THundsunFtdcVolumeType    BuyAmount6;          //买六量
	THundsunFtdcPriceType     SalePrice6;          //卖六价
	THundsunFtdcVolumeType    SaleAmount6;         //卖六量
	THundsunFtdcPriceType     BuyPrice7;           //买七价
	THundsunFtdcVolumeType    BuyAmount7;          //买七量
	THundsunFtdcPriceType     SalePrice7;          //卖七价
	THundsunFtdcVolumeType    SaleAmount7;         //卖七量
	THundsunFtdcPriceType     BuyPrice8;           //买八价
	THundsunFtdcVolumeType    BuyAmount8;          //买八量
	THundsunFtdcPriceType     SalePrice8;          //卖八价
	THundsunFtdcVolumeType    SaleAmount8;         //卖八量
	THundsunFtdcPriceType     BuyPrice9;           //买九价
	THundsunFtdcVolumeType    BuyAmount9;          //买九量
	THundsunFtdcPriceType     SalePrice9;          //卖九价
	THundsunFtdcVolumeType    SaleAmount9;         //卖九量
	THundsunFtdcPriceType     BuyPrice10;          //买十价
	THundsunFtdcVolumeType    BuyAmount10;         //买十量
	THundsunFtdcPriceType     SalePrice10;         //卖十价
	THundsunFtdcVolumeType    SaleAmount10;        //卖十量
};

struct CHundsunKLineDataField
{
	THundsunFtdcVolumeType    KLineDate;           //K线日期
	THundsunFtdcVolumeType    KLineTime;           //K线时间段
	THundsunFtdcPriceType     LastPrice;           //最新价
	THundsunFtdcVolumeType    KLineVolume;         //K线成交数量
	THundsunFtdcVolumeType    KLineOpenInterest;   //K线持仓数量
	THundsunFtdcPriceType     KLineOpenPrice;      //K线开盘
	THundsunFtdcPriceType     KLineHighestPrice;   //K线最高价
	THundsunFtdcPriceType     KLineLowestPrice;    //K线最低价
	THundsunFtdcPriceType     KLineClosePrice;     //K线收盘
	THundsunFtdcPriceType     OpenPrice;           //今开盘
	THundsunFtdcPriceType     HighestPrice;        //最高价
	THundsunFtdcPriceType     LowestPrice;         //最低价
	THundsunFtdcPriceType     ClosePrice;          //今收盘
};

#endif