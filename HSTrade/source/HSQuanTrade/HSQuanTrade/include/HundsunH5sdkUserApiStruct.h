#if !defined(HUNDSUNH5SDKUSERAPISTRUCT_H)
#define HUNDSUNH5SDKUSERAPISTRUCT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <queue>
using namespace std;

//错误信息
struct CHundsunH5sdkRspInfoField
{
    int       ErrorNo;                  //错误代码
    char      ErrorInfo[255];           //错误提示
};

//1001 登入请求请求
struct CHundsunH5sdkReqLoginField
{
    char      UserName[255];            //登录类型
    char      Password[255];            //枚举。登录数据类型
    char      HqToken[255];             //访问令牌
    char      DynPassword[255];         //登录数据
    char      AppName[255];             //应用名称，包括版本信息
    char      PhoneNumber[255];         //手机号
    int       SdkVersion;               //sdk版本号（sdk自动填入）
    char      OsVersion[255];           //操作系统版本（sdk自动填入）
    char      MacAddress[255];          //物理地址（sdk自动填入）

    CHundsunH5sdkReqLoginField()
    {
        memset(UserName, 0, sizeof(UserName));
        memset(Password, 0, sizeof(Password));
        memset(HqToken, 0, sizeof(HqToken));
        memset(DynPassword, 0, sizeof(DynPassword));
        memset(AppName, 0, sizeof(AppName));
        memset(PhoneNumber, 0, sizeof(PhoneNumber));
        SdkVersion = 0;
        memset(OsVersion, 0, sizeof(OsVersion));
        memset(MacAddress, 0, sizeof(MacAddress));
    };
};

//1001 登入请求应答
struct CHundsunH5sdkRspLoginField
{
    int       ErrorNo;                  //错误号。使用H5PROTO_ENUM_EN_CUSTOM则使用CustomErrorNo和LoginData，否则使用ErrorInfo字段
    char      ErrorInfo[255];           //错误信息
    char      OriginalData[255];        //登录返回数据。比如Level2校验后台返回的数据
    int       HeartbeatInterval;        //心跳间隔

    CHundsunH5sdkRspLoginField()
    {
        ErrorNo = 0;
        memset(ErrorInfo, 0, sizeof(ErrorInfo));
        memset(OriginalData, 0, sizeof(OriginalData));
        HeartbeatInterval = 0;
    };
};

//1002 登出信息请求
struct CHundsunH5sdkReqLogoutField
{
    char      QuitReason[255];          //错误信息

    CHundsunH5sdkReqLogoutField()
    {
        memset(QuitReason, 0, sizeof(QuitReason));
    };
};

//1002 登出信息应答
struct CHundsunH5sdkRspLogoutField
{

    CHundsunH5sdkRspLogoutField()
    {
    };
};

//1003 心跳请求
struct CHundsunH5sdkReqHeartbeatField
{

    CHundsunH5sdkReqHeartbeatField()
    {
    };
};

//1003 心跳应答
struct CHundsunH5sdkRspHeartbeatField
{

    CHundsunH5sdkRspHeartbeatField()
    {
    };
};

//1004 请求服务器信息请求
struct CHundsunH5sdkReqServerInfoField
{

    CHundsunH5sdkReqServerInfoField()
    {
    };
};

//1004 请求服务器信息应答 - 服务器支持的交易所
struct CHundsunH5sdkRspServerInfoFinanceGrpField
{
    char      FinanceMic[255];          //市场类型
    char      FinanceName[255];         //市场交易日期
    int       InitDate;                 //交易所日期
    int       BusinessTime;             //交易所时间

    CHundsunH5sdkRspServerInfoFinanceGrpField()
    {
        memset(FinanceMic, 0, sizeof(FinanceMic));
        memset(FinanceName, 0, sizeof(FinanceName));
        InitDate = 0;
        BusinessTime = 0;
    };
};

//1004 请求服务器信息应答
struct CHundsunH5sdkRspServerInfoField
{
    char      SrvName[255];             //服务器名称
    long      SrvTime;                  //当前服务器时间
    int       CurrOnlineCount;          //当前在线（暂不使用）
    int       MaxOnlineCount;           //最大允许在线（暂不使用）
    vector<CHundsunH5sdkRspServerInfoFinanceGrpField> FinanceGrp; //服务器支持的交易所

    CHundsunH5sdkRspServerInfoField()
    {
        memset(SrvName, 0, sizeof(SrvName));
        SrvTime = 0;
        CurrOnlineCount = 0;
        MaxOnlineCount = 0;
    };
};

//1006 批处理请求 - 用于存放批处理子请求的重复组
struct CHundsunH5sdkReqBatchBatchGrpField
{
    char      SubPacket[2000];          //子请求的报文二进制

    CHundsunH5sdkReqBatchBatchGrpField()
    {
        memset(SubPacket, 0, sizeof(SubPacket));
    };
};

//1006 批处理请求
struct CHundsunH5sdkReqBatchField
{
    vector<CHundsunH5sdkReqBatchBatchGrpField> BatchGrp; //用于存放批处理子请求的重复组

    CHundsunH5sdkReqBatchField()
    {
    };
};

//1006 批处理应答 - 用于存放批处理子应答的重复组
struct CHundsunH5sdkRspBatchBatchGrpField
{
    char      SubPacket[2000];          //子应答的报文二进制

    CHundsunH5sdkRspBatchBatchGrpField()
    {
        memset(SubPacket, 0, sizeof(SubPacket));
    };
};

//1006 批处理应答
struct CHundsunH5sdkRspBatchField
{
    vector<CHundsunH5sdkRspBatchBatchGrpField> BatchGrp; //用于存放批处理子应答的重复组

    CHundsunH5sdkRspBatchField()
    {
    };
};

//2001 请求静态文件请求
struct CHundsunH5sdkReqFileField
{
    int       HqFileType;               //文件类型
    char      HqFileName[255];          //文件相对名称
    int       Crc;                      //文件CRC
    long      FileOffset;               //文件偏移
    int       FileLength;               //请求的长度（超过一定数值后，根据应答下次请求）

    CHundsunH5sdkReqFileField()
    {
        HqFileType = 0;
        memset(HqFileName, 0, sizeof(HqFileName));
        Crc = 0;
        FileOffset = 0;
        FileLength = 0;
    };
};

//2001 请求静态文件应答
struct CHundsunH5sdkRspFileField
{
    int       ErrorNo;                  //错误号
    int       HqFileType;               //文件类型
    char      HqFileName[255];          //文件名称
    int       FileLength;               //剩余长度
    long      FileOffset;               //下次请求的offset
    int       Crc;                      //文件CRC
    char      OriginalData[255];        //文件数据

    CHundsunH5sdkRspFileField()
    {
        ErrorNo = 0;
        HqFileType = 0;
        memset(HqFileName, 0, sizeof(HqFileName));
        FileLength = 0;
        FileOffset = 0;
        Crc = 0;
        memset(OriginalData, 0, sizeof(OriginalData));
    };
};

//2002 市场分类信息请求 - 交易所代码
struct CHundsunH5sdkReqMarketTypesFinanceMicGrpField
{
    char      FinanceMic[255];          //交易所代码

    CHundsunH5sdkReqMarketTypesFinanceMicGrpField()
    {
        memset(FinanceMic, 0, sizeof(FinanceMic));
    };
};

//2002 市场分类信息请求
struct CHundsunH5sdkReqMarketTypesField
{
    vector<CHundsunH5sdkReqMarketTypesFinanceMicGrpField> FinanceMicGrp; //交易所代码

    CHundsunH5sdkReqMarketTypesField()
    {
    };
};

//2002 市场分类信息应答 - 交易时间段
struct CHundsunH5sdkRspMarketTypesTradeSectionGrpField
{
    int       Opentime;                 //开市时间
    int       Closetime;                //闭市时间

    CHundsunH5sdkRspMarketTypesTradeSectionGrpField()
    {
        Opentime = 0;
        Closetime = 0;
    };
};

//2002 市场分类信息应答 - 证券分类
struct CHundsunH5sdkRspMarketTypesTypeGrpField
{
    char      HqTypeCode[255];          //分类代码
    char      HqTypeName[255];          //分类名称
    int       PxScale;                  //分类放大倍数
    int       PxPrecision;              //价格精度
    int       InitDate;                 //交易日期
    vector<CHundsunH5sdkRspMarketTypesTradeSectionGrpField> TradeSectionGrp; //交易时间段

    CHundsunH5sdkRspMarketTypesTypeGrpField()
    {
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        memset(HqTypeName, 0, sizeof(HqTypeName));
        PxScale = 0;
        PxPrecision = 0;
        InitDate = 0;
    };
};

//2002 市场分类信息应答 - 交易所代码
struct CHundsunH5sdkRspMarketTypesFinanceMicGrpField
{
    char      FinanceMic[255];          //交易所代码
    char      FinanceName[255];         //交易所名称
    int       MarketDate;               //市场日期
    int       InitDate;                 //交易日期
    int       Timezone;                 //市场所在时区
    char      TimezoneCode[255];        //时区码
    int       Dst;                      //夏令时标志
    vector<CHundsunH5sdkRspMarketTypesTypeGrpField> TypeGrp; //证券分类

    CHundsunH5sdkRspMarketTypesFinanceMicGrpField()
    {
        memset(FinanceMic, 0, sizeof(FinanceMic));
        memset(FinanceName, 0, sizeof(FinanceName));
        MarketDate = 0;
        InitDate = 0;
        Timezone = 0;
        memset(TimezoneCode, 0, sizeof(TimezoneCode));
        Dst = 0;
    };
};

//2002 市场分类信息应答
struct CHundsunH5sdkRspMarketTypesField
{
    vector<CHundsunH5sdkRspMarketTypesFinanceMicGrpField> FinanceMicGrp; //交易所代码

    CHundsunH5sdkRspMarketTypesField()
    {
    };
};

//2003 市场代码表请求
struct CHundsunH5sdkReqMarketReferenceField
{
    char      HqTypeCode[255];          //市场类别

    CHundsunH5sdkReqMarketReferenceField()
    {
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
    };
};

//2003 市场代码表应答 - 代码
struct CHundsunH5sdkRspMarketReferenceProdGrpField
{
    char      ProdCode[255];            //证券代码
    char      HqTypeCode[255];          //类型代码
    char      ProdName[255];            //证券名称
    int       PreclosePx;               //前收盘价
    int       UpPx;                     //涨停价格
    int       DownPx;                   //跌停价格

    CHundsunH5sdkRspMarketReferenceProdGrpField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        memset(ProdName, 0, sizeof(ProdName));
        PreclosePx = 0;
        UpPx = 0;
        DownPx = 0;
    };
};

//2003 市场代码表应答
struct CHundsunH5sdkRspMarketReferenceField
{
    char      FinanceMic[255];          //交易所代码
    char      MicAbbr[255];             //交易所MIC缩写
    char      FinanceName[255];         //交易所名称
    int       MarketDate;               //市场日期
    int       InitDate;                 //交易日期
    int       Timezone;                 //市场所在时区
    char      TimezoneCode[255];        //时区码
    int       Dst;                      //夏令时标志
    vector<CHundsunH5sdkRspMarketReferenceProdGrpField> ProdGrp; //代码

    CHundsunH5sdkRspMarketReferenceField()
    {
        memset(FinanceMic, 0, sizeof(FinanceMic));
        memset(MicAbbr, 0, sizeof(MicAbbr));
        memset(FinanceName, 0, sizeof(FinanceName));
        MarketDate = 0;
        InitDate = 0;
        Timezone = 0;
        memset(TimezoneCode, 0, sizeof(TimezoneCode));
        Dst = 0;
    };
};

//3001 行情快照请求 - 代码列表
struct CHundsunH5sdkReqSnapshotProdGrpField
{
    char      ProdCode[255];            //股票代码
    char      HqTypeCode[255];          //类型代码,[subtype][.cficode].exchage

    CHundsunH5sdkReqSnapshotProdGrpField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
    };
};

//3001 行情快照请求 - 需要的字段列表
struct CHundsunH5sdkReqSnapshotFieldGrpField
{
    int       FieldId;                  //以此ID为准，当id为空时候，取filename
    char      FieldName[255];           //字段ID

    CHundsunH5sdkReqSnapshotFieldGrpField()
    {
        FieldId = 0;
        memset(FieldName, 0, sizeof(FieldName));
    };
};

//3001 行情快照请求
struct CHundsunH5sdkReqSnapshotField
{
    int       HqLevel;                  //行情级别: level1, evel2(对于美股level1,表示延迟,level2表示实时)
    vector<CHundsunH5sdkReqSnapshotProdGrpField> ProdGrp; //代码列表
    vector<CHundsunH5sdkReqSnapshotFieldGrpField> FieldGrp; //需要的字段列表

    CHundsunH5sdkReqSnapshotField()
    {
        HqLevel = 0;
    };
};

//3001 行情快照应答 - 委托订单列表
struct CHundsunH5sdkRspSnapshotEntrustGrpField
{
    int       EntrustAmount;            //委托量

    CHundsunH5sdkRspSnapshotEntrustGrpField()
    {
        EntrustAmount = 0;
    };
};

//3001 行情快照应答 - 委买档位
struct CHundsunH5sdkRspSnapshotBidGrpField
{
    int       EntrustPx;                //委托价格
    long      TotalEntrustAmount;       //委托量
    int       EntrustCount;             //委托量
    vector<CHundsunH5sdkRspSnapshotEntrustGrpField> EntrustGrp; //委托订单列表

    CHundsunH5sdkRspSnapshotBidGrpField()
    {
        EntrustPx = 0;
        TotalEntrustAmount = 0;
        EntrustCount = 0;
    };
};

//3001 行情快照应答 - 委卖档位
struct CHundsunH5sdkRspSnapshotOfferGrpField
{
    int       EntrustPx;                //委托价格
    long      TotalEntrustAmount;       //委托量
    int       EntrustCount;             //委托量
    vector<CHundsunH5sdkRspSnapshotEntrustGrpField> EntrustGrp; //委托订单列表

    CHundsunH5sdkRspSnapshotOfferGrpField()
    {
        EntrustPx = 0;
        TotalEntrustAmount = 0;
        EntrustCount = 0;
    };
};

//3001 行情快照应答 - 领涨股票
struct CHundsunH5sdkRspSnapshotRiseFirstGrpField
{
    char      ProdCode[255];            //证券代码
    char      ProdName[255];            //证券名称
    char      HqTypeCode[255];          //证券名称
    int       LastPx;                   //最新价
    int       PxChangeRate;             //涨跌幅

    CHundsunH5sdkRspSnapshotRiseFirstGrpField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(ProdName, 0, sizeof(ProdName));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        LastPx = 0;
        PxChangeRate = 0;
    };
};

//3001 行情快照应答 - 领地
struct CHundsunH5sdkRspSnapshotFallFirstGrpField
{
    char      ProdCode[255];            //证券代码
    char      ProdName[255];            //证券名称
    char      HqTypeCode[255];          //证券名称
    int       LastPx;                   //最新价
    int       PxChangeRate;             //涨跌幅

    CHundsunH5sdkRspSnapshotFallFirstGrpField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(ProdName, 0, sizeof(ProdName));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        LastPx = 0;
        PxChangeRate = 0;
    };
};

//3001 行情快照应答 - 代码列表
struct CHundsunH5sdkRspSnapshotProdGrpField
{
    char      ProdCode[255];            //证券代码
    char      ProdName[255];            //证券名称
    char      HqTypeCode[255];          //证券名称
    char      IndustryCode[255];        //行情代码（未定义）
    char      MoneyType[255];           //货币代码（未定义）
    int       DataTimestamp;            //时间戳
    int       TradeMins;                //交易分钟数
    char      TradeStatus[255];         //交易状态
    int       PreclosePx;               //昨收价
    int       OpenPx;                   //今开盘
    int       LastPx;                   //最新成交价
    int       HighPx;                   //最高价
    int       LowPx;                    //最低价
    int       ClosePx;                  //今日收盘
    int       AvgPx;                    //均价
    int       WavgPx;                   //加权平均价
    int       BusinessCount;            //成交笔数
    long      BusinessAmount;           //总成交量
    long      BusinessBalance;          //总成交额
    int       UpPx;                     //涨停价格
    int       DownPx;                   //跌停价格
    long      CurrentAmount;            //最近成交量(现手)
    long      BusinessAmountIn;         //内盘成交量
    long      BusinessAmountOut;        //外盘成交量
    long      TotBuyAmount;             //总委买量（未定义）
    long      TotSellAmount;            //总委卖量（未定义）
    int       WavgBidPx;                //加权平均委买价格（未定义）
    int       WavgOfferPx;              //加权平均委卖价格（未定义）
    vector<CHundsunH5sdkRspSnapshotBidGrpField> BidGrp; //委买档位
    vector<CHundsunH5sdkRspSnapshotOfferGrpField> OfferGrp; //委卖档位
    int       EntrustRate;              //委比
    long      EntrustDiff;              //委差
    int       W52LowPx;                 //52周最低价（未定义）
    int       W52HighPx;                //52周最高价（未定义）
    int       PxChange;                 //涨跌额
    int       Amplitude;                //振幅
    int       PxChangeRate;             //涨跌幅
    int       PopcPx;                   //盘前盘后价
    int       TradeSection;             //当前交易阶段:0:闭市(没有延迟数据) 1:盘前 2:盘中 3:盘后 4:实时盘后(没有延迟数据)
    long      Svol;                     //当前交易节成交量（未定义）
    long      CirculationAmount;        //流通股本
    long      TotalShares;              //总股本
    long      MarketValue;              //市值
    long      CirculationValue;         //流通市值
    int       VolRatio;                 //量比
    int       TurnoverRatio;            //换手率
    long      Amount;                   //持仓量
    long      PrevAmount;               //昨持仓
    long      AmountDelta;              //日增持
    int       PrevSettlement;           //昨结算
    int       Settlement;               //结算价
    int       Iopv;                     //iopv
    int       DebtFundValue;            //国债基金净值
    int       Eps;                      //每股收益
    int       Bps;                      //每股净资产
    int       DynPbRate;                //市净率
    int       PeRate;                   //市盈率（动态）
    int       FinQuarter;               //财务季度
    int       FinEndDate;               //财务截至日期
    int       SharesPerHand;            //每手股数
    int       RiseCount;                //上涨家数
    int       FallCount;                //下跌家数
    int       MemberCount;              //成员个数
    vector<CHundsunH5sdkRspSnapshotRiseFirstGrpField> RiseFirstGrp; //领涨股票
    vector<CHundsunH5sdkRspSnapshotFallFirstGrpField> FallFirstGrp; //领地

    CHundsunH5sdkRspSnapshotProdGrpField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(ProdName, 0, sizeof(ProdName));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        memset(IndustryCode, 0, sizeof(IndustryCode));
        memset(MoneyType, 0, sizeof(MoneyType));
        DataTimestamp = 0;
        TradeMins = 0;
        memset(TradeStatus, 0, sizeof(TradeStatus));
        PreclosePx = 0;
        OpenPx = 0;
        LastPx = 0;
        HighPx = 0;
        LowPx = 0;
        ClosePx = 0;
        AvgPx = 0;
        WavgPx = 0;
        BusinessCount = 0;
        BusinessAmount = 0;
        BusinessBalance = 0;
        UpPx = 0;
        DownPx = 0;
        CurrentAmount = 0;
        BusinessAmountIn = 0;
        BusinessAmountOut = 0;
        TotBuyAmount = 0;
        TotSellAmount = 0;
        WavgBidPx = 0;
        WavgOfferPx = 0;
        EntrustRate = 0;
        EntrustDiff = 0;
        W52LowPx = 0;
        W52HighPx = 0;
        PxChange = 0;
        Amplitude = 0;
        PxChangeRate = 0;
        PopcPx = 0;
        TradeSection = 0;
        Svol = 0;
        CirculationAmount = 0;
        TotalShares = 0;
        MarketValue = 0;
        CirculationValue = 0;
        VolRatio = 0;
        TurnoverRatio = 0;
        Amount = 0;
        PrevAmount = 0;
        AmountDelta = 0;
        PrevSettlement = 0;
        Settlement = 0;
        Iopv = 0;
        DebtFundValue = 0;
        Eps = 0;
        Bps = 0;
        DynPbRate = 0;
        PeRate = 0;
        FinQuarter = 0;
        FinEndDate = 0;
        SharesPerHand = 0;
        RiseCount = 0;
        FallCount = 0;
        MemberCount = 0;
    };
};

//3001 行情快照应答
struct CHundsunH5sdkRspSnapshotField
{
    int       HqLevel;                  //行情级别: level1, evel2(对于美股level1,表示延迟,level2表示实时)
    vector<CHundsunH5sdkRspSnapshotProdGrpField> ProdGrp; //代码列表

    CHundsunH5sdkRspSnapshotField()
    {
        HqLevel = 0;
    };
};

//3002 键盘精灵消息请求 - 需要筛选的证券分类
struct CHundsunH5sdkReqKeyboardWizardTypeGrpField
{
    char      HqTypeCode[255];          //分类代码

    CHundsunH5sdkReqKeyboardWizardTypeGrpField()
    {
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
    };
};

//3002 键盘精灵消息请求
struct CHundsunH5sdkReqKeyboardWizardField
{
    char      ProdCode[255];            //
    int       Maxvalue;                 //最大返回个数
    vector<CHundsunH5sdkReqKeyboardWizardTypeGrpField> TypeGrp; //需要筛选的证券分类

    CHundsunH5sdkReqKeyboardWizardField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        Maxvalue = 0;
    };
};

//3002 键盘精灵消息应答 - 代码列表
struct CHundsunH5sdkRspKeyboardWizardProdGrpField
{
    char      ProdCode[255];            //证券代码
    char      ProdName[255];            //证券名称
    char      HqTypeCode[255];          //证券类别

    CHundsunH5sdkRspKeyboardWizardProdGrpField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(ProdName, 0, sizeof(ProdName));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
    };
};

//3002 键盘精灵消息应答
struct CHundsunH5sdkRspKeyboardWizardField
{
    vector<CHundsunH5sdkRspKeyboardWizardProdGrpField> ProdGrp; //代码列表

    CHundsunH5sdkRspKeyboardWizardField()
    {
    };
};

//4001 行情快照订阅请求 - 
struct CHundsunH5sdkReqSubscribeProdGrpField
{
    char      HqTypeCode[255];          //
    char      ProdCode[255];            //

    CHundsunH5sdkReqSubscribeProdGrpField()
    {
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        memset(ProdCode, 0, sizeof(ProdCode));
    };
};

//4001 行情快照订阅请求
struct CHundsunH5sdkReqSubscribeField
{
    int       HqLevel;                  //行情级别: level1, evel2(对于美股level1,表示延迟,level2表示实时)
    vector<CHundsunH5sdkReqSubscribeProdGrpField> ProdGrp; //
    int       SubType;                  //订阅动作

    CHundsunH5sdkReqSubscribeField()
    {
        HqLevel = 0;
        SubType = 0;
    };
};

//4001 行情快照订阅应答 - 
struct CHundsunH5sdkRspSubscribeProdGrpField
{
    char      HqTypeCode[255];          //
    char      ProdCode[255];            //代码
    int       ErrorNo;                  //0成功,其他:失败

    CHundsunH5sdkRspSubscribeProdGrpField()
    {
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        memset(ProdCode, 0, sizeof(ProdCode));
        ErrorNo = 0;
    };
};

//4001 行情快照订阅应答
struct CHundsunH5sdkRspSubscribeField
{
    int       HqLevel;                  //行情级别: level1, evel2(对于美股level1,表示延迟,level2表示实时)
    vector<CHundsunH5sdkRspSubscribeProdGrpField> ProdGrp; //
    int       SubType;                  //原样返回订阅动作

    CHundsunH5sdkRspSubscribeField()
    {
        HqLevel = 0;
        SubType = 0;
    };
};

//5001 排序请求请求 - 代码列表
struct CHundsunH5sdkReqSortSortProdGrpField
{
    char      ProdCode[255];            //股票代码
    char      HqTypeCode[255];          //类型代码,[subtype][.cficode].exchage

    CHundsunH5sdkReqSortSortProdGrpField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
    };
};

//5001 排序请求请求 - 证券分类
struct CHundsunH5sdkReqSortSortTypeGrpField
{
    char      HqTypeCode[255];          //分类代码

    CHundsunH5sdkReqSortSortTypeGrpField()
    {
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
    };
};

//5001 排序请求请求 - 需要的字段列表
struct CHundsunH5sdkReqSortFieldGrpField
{
    int       FieldId;                  //以此ID为准，当id为空时候，取filename
    char      FieldName[255];           //字段ID

    CHundsunH5sdkReqSortFieldGrpField()
    {
        FieldId = 0;
        memset(FieldName, 0, sizeof(FieldName));
    };
};

//5001 排序请求请求
struct CHundsunH5sdkReqSortField
{
    vector<CHundsunH5sdkReqSortSortProdGrpField> SortProdGrp; //代码列表
    vector<CHundsunH5sdkReqSortSortTypeGrpField> SortTypeGrp; //证券分类
    int       SortFieldId;              //需要排序的字段ID，以此为准，当这个字段不存在时候取sort_field_name
    char      SortFieldName[255];       //需要排序的字段
    vector<CHundsunH5sdkReqSortFieldGrpField> FieldGrp; //需要的字段列表
    int       StartPos;                 //起始位置,默认为0
    int       DataCount;                //代码数目,默认100只代码，最大请求数目10000
    int       SortType;                 //排序方式/默认降序

    CHundsunH5sdkReqSortField()
    {
        SortFieldId = 0;
        memset(SortFieldName, 0, sizeof(SortFieldName));
        StartPos = 0;
        DataCount = 0;
        SortType = 0;
    };
};

//5001 排序请求应答 - 委托订单列表
struct CHundsunH5sdkRspSortEntrustGrpField
{
    int       EntrustAmount;            //委托量

    CHundsunH5sdkRspSortEntrustGrpField()
    {
        EntrustAmount = 0;
    };
};

//5001 排序请求应答 - 委买档位
struct CHundsunH5sdkRspSortBidGrpField
{
    int       EntrustPx;                //委托价格
    long      TotalEntrustAmount;       //委托量
    int       EntrustCount;             //委托量
    vector<CHundsunH5sdkRspSortEntrustGrpField> EntrustGrp; //委托订单列表

    CHundsunH5sdkRspSortBidGrpField()
    {
        EntrustPx = 0;
        TotalEntrustAmount = 0;
        EntrustCount = 0;
    };
};

//5001 排序请求应答 - 委卖档位
struct CHundsunH5sdkRspSortOfferGrpField
{
    int       EntrustPx;                //委托价格
    long      TotalEntrustAmount;       //委托量
    int       EntrustCount;             //委托量
    vector<CHundsunH5sdkRspSortEntrustGrpField> EntrustGrp; //委托订单列表

    CHundsunH5sdkRspSortOfferGrpField()
    {
        EntrustPx = 0;
        TotalEntrustAmount = 0;
        EntrustCount = 0;
    };
};

//5001 排序请求应答 - 领涨股票
struct CHundsunH5sdkRspSortRiseFirstGrpField
{
    char      ProdCode[255];            //证券代码
    char      ProdName[255];            //证券名称
    char      HqTypeCode[255];          //证券名称
    int       LastPx;                   //最新价
    int       PxChangeRate;             //涨跌幅

    CHundsunH5sdkRspSortRiseFirstGrpField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(ProdName, 0, sizeof(ProdName));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        LastPx = 0;
        PxChangeRate = 0;
    };
};

//5001 排序请求应答 - 领地
struct CHundsunH5sdkRspSortFallFirstGrpField
{
    char      ProdCode[255];            //证券代码
    char      ProdName[255];            //证券名称
    char      HqTypeCode[255];          //证券名称
    int       LastPx;                   //最新价
    int       PxChangeRate;             //涨跌幅

    CHundsunH5sdkRspSortFallFirstGrpField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(ProdName, 0, sizeof(ProdName));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        LastPx = 0;
        PxChangeRate = 0;
    };
};

//5001 排序请求应答 - 证券分类
struct CHundsunH5sdkRspSortSortTypeGrpField
{
    char      HqTypeCode[255];          //分类代码

    CHundsunH5sdkRspSortSortTypeGrpField()
    {
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
    };
};

//5001 排序请求应答 - 代码列表
struct CHundsunH5sdkRspSortSortProdGrpField
{
    char      ProdCode[255];            //证券代码
    char      ProdName[255];            //证券名称
    char      HqTypeCode[255];          //证券名称
    char      IndustryCode[255];        //行情代码（未定义）
    char      MoneyType[255];           //货币代码（未定义）
    int       DataTimestamp;            //时间戳
    int       TradeMins;                //交易分钟数
    char      TradeStatus[255];         //交易状态
    int       PreclosePx;               //昨收价
    int       OpenPx;                   //今开盘
    int       LastPx;                   //最新成交价
    int       HighPx;                   //最高价
    int       LowPx;                    //最低价
    int       ClosePx;                  //今日收盘
    int       AvgPx;                    //均价
    int       WavgPx;                   //加权平均价
    int       BusinessCount;            //成交笔数
    long      BusinessAmount;           //总成交量
    long      BusinessBalance;          //总成交额
    int       UpPx;                     //涨停价格
    int       DownPx;                   //跌停价格
    long      CurrentAmount;            //最近成交量(现手)
    long      BusinessAmountIn;         //内盘成交量
    long      BusinessAmountOut;        //外盘成交量
    long      TotBuyAmount;             //总委买量（未定义）
    long      TotSellAmount;            //总委卖量（未定义）
    int       WavgBidPx;                //加权平均委买价格（未定义）
    int       WavgOfferPx;              //加权平均委卖价格（未定义）
    vector<CHundsunH5sdkRspSortBidGrpField> BidGrp; //委买档位
    vector<CHundsunH5sdkRspSortOfferGrpField> OfferGrp; //委卖档位
    int       EntrustRate;              //委比
    long      EntrustDiff;              //委差
    int       W52LowPx;                 //52周最低价（未定义）
    int       W52HighPx;                //52周最高价（未定义）
    int       PxChange;                 //涨跌额
    int       Amplitude;                //振幅
    int       PxChangeRate;             //涨跌幅
    int       PopcPx;                   //盘前盘后价
    int       TradeSection;             //当前交易阶段:0:闭市(没有延迟数据) 1:盘前 2:盘中 3:盘后 4:实时盘后(没有延迟数据)
    long      Svol;                     //当前交易节成交量（未定义）
    long      CirculationAmount;        //流通股本
    long      TotalShares;              //总股本
    long      MarketValue;              //市值
    long      CirculationValue;         //流通市值
    int       VolRatio;                 //量比
    int       TurnoverRatio;            //换手率
    long      Amount;                   //持仓量
    long      PrevAmount;               //昨持仓
    long      AmountDelta;              //日增持
    int       PrevSettlement;           //昨结算
    int       Settlement;               //结算价
    int       Iopv;                     //iopv
    int       DebtFundValue;            //国债基金净值
    int       Eps;                      //每股收益
    int       Bps;                      //每股净资产
    int       DynPbRate;                //市净率
    int       PeRate;                   //市盈率（动态）
    int       FinQuarter;               //财务季度
    int       FinEndDate;               //财务截至日期
    int       SharesPerHand;            //每手股数
    int       RiseCount;                //上涨家数
    int       FallCount;                //下跌家数
    int       MemberCount;              //成员个数
    vector<CHundsunH5sdkRspSortRiseFirstGrpField> RiseFirstGrp; //领涨股票
    vector<CHundsunH5sdkRspSortFallFirstGrpField> FallFirstGrp; //领地

    CHundsunH5sdkRspSortSortProdGrpField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(ProdName, 0, sizeof(ProdName));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        memset(IndustryCode, 0, sizeof(IndustryCode));
        memset(MoneyType, 0, sizeof(MoneyType));
        DataTimestamp = 0;
        TradeMins = 0;
        memset(TradeStatus, 0, sizeof(TradeStatus));
        PreclosePx = 0;
        OpenPx = 0;
        LastPx = 0;
        HighPx = 0;
        LowPx = 0;
        ClosePx = 0;
        AvgPx = 0;
        WavgPx = 0;
        BusinessCount = 0;
        BusinessAmount = 0;
        BusinessBalance = 0;
        UpPx = 0;
        DownPx = 0;
        CurrentAmount = 0;
        BusinessAmountIn = 0;
        BusinessAmountOut = 0;
        TotBuyAmount = 0;
        TotSellAmount = 0;
        WavgBidPx = 0;
        WavgOfferPx = 0;
        EntrustRate = 0;
        EntrustDiff = 0;
        W52LowPx = 0;
        W52HighPx = 0;
        PxChange = 0;
        Amplitude = 0;
        PxChangeRate = 0;
        PopcPx = 0;
        TradeSection = 0;
        Svol = 0;
        CirculationAmount = 0;
        TotalShares = 0;
        MarketValue = 0;
        CirculationValue = 0;
        VolRatio = 0;
        TurnoverRatio = 0;
        Amount = 0;
        PrevAmount = 0;
        AmountDelta = 0;
        PrevSettlement = 0;
        Settlement = 0;
        Iopv = 0;
        DebtFundValue = 0;
        Eps = 0;
        Bps = 0;
        DynPbRate = 0;
        PeRate = 0;
        FinQuarter = 0;
        FinEndDate = 0;
        SharesPerHand = 0;
        RiseCount = 0;
        FallCount = 0;
        MemberCount = 0;
    };
};

//5001 排序请求应答
struct CHundsunH5sdkRspSortField
{
    int       SortType;                 //排序方式/默认降序
    vector<CHundsunH5sdkRspSortSortTypeGrpField> SortTypeGrp; //证券分类
    int       SortFieldId;              //需要排序的字段ID
    char      SortFieldName[255];       //需要排序的字段
    int       StartPos;                 //起始位置,默认为0
    vector<CHundsunH5sdkRspSortSortProdGrpField> SortProdGrp; //代码列表

    CHundsunH5sdkRspSortField()
    {
        SortType = 0;
        SortFieldId = 0;
        memset(SortFieldName, 0, sizeof(SortFieldName));
        StartPos = 0;
    };
};

//6001 按指定的日期或偏移取分时数据请求
struct CHundsunH5sdkReqTrendField
{
    char      ProdCode[255];            //证券代码
    char      HqTypeCode[255];          //类型代码
    int       Date;                     //日期:YYYYMMDD,0:表示今天
    int       DateOffset;               //距离当前日期的偏移天数,

    CHundsunH5sdkReqTrendField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        Date = 0;
        DateOffset = 0;
    };
};

//6001 按指定的日期或偏移取分时数据应答 - 分时数据
struct CHundsunH5sdkRspTrendTrendGrpField
{
    int       MinTime;                  //分时时间hhmm
    int       HqPx;                     //价格
    int       AvgPx;                    //均价
    int       WavgPx;                   //加权均价
    long      BusinessAmount;           //总成交量
    long      BusinessBalance;          //总成交额

    CHundsunH5sdkRspTrendTrendGrpField()
    {
        MinTime = 0;
        HqPx = 0;
        AvgPx = 0;
        WavgPx = 0;
        BusinessAmount = 0;
        BusinessBalance = 0;
    };
};

//6001 按指定的日期或偏移取分时数据应答
struct CHundsunH5sdkRspTrendField
{
    char      ProdCode[255];            //证券代码
    char      HqTypeCode[255];          //类型代码
    int       Date;                     //日期。格式YYYYMMDD
    vector<CHundsunH5sdkRspTrendTrendGrpField> TrendGrp; //分时数据

    CHundsunH5sdkRspTrendField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        Date = 0;
    };
};

//6002 按偏移取K线请求
struct CHundsunH5sdkReqCandleByOffsetField
{
    char      ProdCode[255];            //证券代码
    char      HqTypeCode[255];          //类型代码
    int       CandlePeriod;             //k线类型
    int       CandleMode;               //
    int       Date;                     //起始日期，不填写表示当前日期
    int       MinTime;                  //分钟K线的时间HHMM,对于短周期K线类型使用(1min,5min等)，不填写表示最新的市场时间，若填写必须同时填写date字段
    int       SearchDirection;          //方向
    int       DataCount;                //请求的个数

    CHundsunH5sdkReqCandleByOffsetField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        CandlePeriod = 0;
        CandleMode = 0;
        Date = 0;
        MinTime = 0;
        SearchDirection = 0;
        DataCount = 0;
    };
};

//6002 按偏移取K线应答 - 分时数据
struct CHundsunH5sdkRspCandleByOffsetCandleGrpField
{
    int       Date;                     //起始日期，0：表示当前时间
    int       MinTime;                  //分钟K线的时间，HHMM
    int       OpenPx;                   //开盘价
    int       HighPx;                   //最高价
    int       LowPx;                    //最低价
    int       ClosePx;                  //收盘价
    long      BusinessAmount;           //成交量
    long      BusinessBalance;          //成交额

    CHundsunH5sdkRspCandleByOffsetCandleGrpField()
    {
        Date = 0;
        MinTime = 0;
        OpenPx = 0;
        HighPx = 0;
        LowPx = 0;
        ClosePx = 0;
        BusinessAmount = 0;
        BusinessBalance = 0;
    };
};

//6002 按偏移取K线应答
struct CHundsunH5sdkRspCandleByOffsetField
{
    char      ProdCode[255];            //证券全代码
    char      HqTypeCode[255];          //类型代码
    int       CandlePeriod;             //k线类型
    int       CandleMode;               //
    int       AllDataFlag;              //行情服务器是否还有数据 1表示还有数据，表示用户请求数据量大于目前支持最大的根数，需要用户根据返回结果再次请求
    vector<CHundsunH5sdkRspCandleByOffsetCandleGrpField> CandleGrp; //分时数据

    CHundsunH5sdkRspCandleByOffsetField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        CandlePeriod = 0;
        CandleMode = 0;
        AllDataFlag = 0;
    };
};

//6003 按日期时间范围取K线请求
struct CHundsunH5sdkReqCandleByRangeField
{
    char      ProdCode[255];            //证券代码
    char      HqTypeCode[255];          //类型代码
    int       CandlePeriod;             //k线类型
    int       CandleMode;               //
    int       StartDate;                //
    int       EndDate;                  //

    CHundsunH5sdkReqCandleByRangeField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        CandlePeriod = 0;
        CandleMode = 0;
        StartDate = 0;
        EndDate = 0;
    };
};

//6003 按日期时间范围取K线应答 - 分时数据
struct CHundsunH5sdkRspCandleByRangeCandleGrpField
{
    int       Date;                     //起始日期，0：表示当前时间
    int       MinTime;                  //
    int       OpenPx;                   //开盘价
    int       HighPx;                   //最高价
    int       LowPx;                    //最低价
    int       ClosePx;                  //收盘价
    long      BusinessAmount;           //成交量
    long      BusinessBalance;          //成交额

    CHundsunH5sdkRspCandleByRangeCandleGrpField()
    {
        Date = 0;
        MinTime = 0;
        OpenPx = 0;
        HighPx = 0;
        LowPx = 0;
        ClosePx = 0;
        BusinessAmount = 0;
        BusinessBalance = 0;
    };
};

//6003 按日期时间范围取K线应答
struct CHundsunH5sdkRspCandleByRangeField
{
    char      ProdCode[255];            //证券代码
    char      HqTypeCode[255];          //类型代码
    int       CandlePeriod;             //k线类型
    int       CandleMode;               //
    int       AllDataFlag;              //行情服务器是否还有数据 1表示还有数据
    vector<CHundsunH5sdkRspCandleByRangeCandleGrpField> CandleGrp; //分时数据

    CHundsunH5sdkRspCandleByRangeField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        CandlePeriod = 0;
        CandleMode = 0;
        AllDataFlag = 0;
    };
};

//6004 指定分钟的分笔请求
struct CHundsunH5sdkReqTickMinField
{
    char      ProdCode[255];            //证券代码
    char      HqTypeCode[255];          //类型代码
    int       Date;                     //日期:YYYYMMDD,0:表示今天
    int       StartMin;                 //起始分钟数
    int       EndMin;                   //结束分钟数
    int       DataCount;                //最大多少个，默认全部返回

    CHundsunH5sdkReqTickMinField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        Date = 0;
        StartMin = 0;
        EndMin = 0;
        DataCount = 0;
    };
};

//6004 指定分钟的分笔应答 - 分时数据
struct CHundsunH5sdkRspTickMinTickGrpField
{
    int       BusinessNo;               //分笔序号
    int       BusinessTime;             //时间戳
    int       HqPx;                     //价格
    long      BusinessAmount;           //成交量
    long      BusinessBalance;          //成交额
    int       BusinessCount;            //成交笔数
    int       BusinessDirection;        //成交方向（0：卖，1：买)

    CHundsunH5sdkRspTickMinTickGrpField()
    {
        BusinessNo = 0;
        BusinessTime = 0;
        HqPx = 0;
        BusinessAmount = 0;
        BusinessBalance = 0;
        BusinessCount = 0;
        BusinessDirection = 0;
    };
};

//6004 指定分钟的分笔应答
struct CHundsunH5sdkRspTickMinField
{
    char      ProdCode[255];            //证券代码
    char      HqTypeCode[255];          //类型代码
    int       Date;                     //日期。格式YYYYMMDD
    int       StartMin;                 //起始分钟数
    int       EndMin;                   //结束分钟数
    int       AllDataFlag;              //行情服务器是否还有数据 1表示还有数据
    vector<CHundsunH5sdkRspTickMinTickGrpField> TickGrp; //分时数据

    CHundsunH5sdkRspTickMinField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        Date = 0;
        StartMin = 0;
        EndMin = 0;
        AllDataFlag = 0;
    };
};

//6005 按偏移取分笔请求
struct CHundsunH5sdkReqTickDirectionField
{
    char      ProdCode[255];            //证券代码
    char      HqTypeCode[255];          //类型代码
    int       Date;                     //日期:YYYYMMDD,0:表示今天
    int       StartPos;                 //起始位置,默认为0
    int       SearchDirection;          //方向
    int       DataCount;                //最大多少个，默认全部返回

    CHundsunH5sdkReqTickDirectionField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        Date = 0;
        StartPos = 0;
        SearchDirection = 0;
        DataCount = 0;
    };
};

//6005 按偏移取分笔应答 - 分时数据
struct CHundsunH5sdkRspTickDirectionTickGrpField
{
    int       BusinessNo;               //分笔序号
    int       BusinessTime;             //时间戳
    int       HqPx;                     //价格
    long      BusinessAmount;           //成交量
    long      BusinessBalance;          //成交额
    int       BusinessCount;            //成交笔数
    int       BusinessDirection;        //成交方向（0：卖，1：买)

    CHundsunH5sdkRspTickDirectionTickGrpField()
    {
        BusinessNo = 0;
        BusinessTime = 0;
        HqPx = 0;
        BusinessAmount = 0;
        BusinessBalance = 0;
        BusinessCount = 0;
        BusinessDirection = 0;
    };
};

//6005 按偏移取分笔应答
struct CHundsunH5sdkRspTickDirectionField
{
    char      ProdCode[255];            //证券代码
    char      HqTypeCode[255];          //类型代码
    int       Date;                     //日期:YYYYMMDD,0:表示今天
    int       StartPos;                 //起始位置,默认为0，向前取时，0表示最近的一笔，向后取时0表示最早的一个分笔
    int       SearchDirection;          //方向
    int       AllDataFlag;              //行情服务器是否还有数据 1表示还有数据
    vector<CHundsunH5sdkRspTickDirectionTickGrpField> TickGrp; //分时数据

    CHundsunH5sdkRspTickDirectionField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        Date = 0;
        StartPos = 0;
        SearchDirection = 0;
        AllDataFlag = 0;
    };
};

//6006 补线使用_分钟k线请求
struct CHundsunH5sdkReqCandleByRangeMaintainField
{
    char      ProdCode[255];            //证券代码
    char      HqTypeCode[255];          //类型代码
    int       CandlePeriod;             //k线类型(只需要分钟k线)
    int       CandleMode;               //
    int       StartDate;                //
    int       EndDate;                  //

    CHundsunH5sdkReqCandleByRangeMaintainField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        CandlePeriod = 0;
        CandleMode = 0;
        StartDate = 0;
        EndDate = 0;
    };
};

//6006 补线使用_分钟k线应答 - 分时数据
struct CHundsunH5sdkRspCandleByRangeMaintainCandleGrpField
{
    int       Date;                     //起始日期，0：表示当前时间
    int       MinTime;                  //时间
    int       WavgPx;                   //均价
    int       OpenPx;                   //开盘价
    int       HighPx;                   //最高价
    int       LowPx;                    //最低价
    int       ClosePx;                  //收盘价
    long      BusinessAmount;           //成交量
    long      BusinessBalance;          //成交额
    long      Amount;                   //

    CHundsunH5sdkRspCandleByRangeMaintainCandleGrpField()
    {
        Date = 0;
        MinTime = 0;
        WavgPx = 0;
        OpenPx = 0;
        HighPx = 0;
        LowPx = 0;
        ClosePx = 0;
        BusinessAmount = 0;
        BusinessBalance = 0;
        Amount = 0;
    };
};

//6006 补线使用_分钟k线应答
struct CHundsunH5sdkRspCandleByRangeMaintainField
{
    char      ProdCode[255];            //证券代码
    char      HqTypeCode[255];          //类型代码
    int       CandlePeriod;             //k线类型
    int       CandleMode;               //
    int       AllDataFlag;              //行情服务器是否还有数据 1表示还有数据
    vector<CHundsunH5sdkRspCandleByRangeMaintainCandleGrpField> CandleGrp; //分时数据

    CHundsunH5sdkRspCandleByRangeMaintainField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
        CandlePeriod = 0;
        CandleMode = 0;
        AllDataFlag = 0;
    };
};

//6007 5日分时请求
struct CHundsunH5sdkReqDay5TrendField
{
    char      ProdCode[255];            //证券代码
    char      HqTypeCode[255];          //类型代码

    CHundsunH5sdkReqDay5TrendField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
    };
};

//6007 5日分时应答 - 分时数据
struct CHundsunH5sdkRspDay5TrendTrendGrpField
{
    int       Date;                     //日期:YYYYMMDD
    int       MinTime;                  //分时时间hhmm
    int       HqPx;                     //价格
    int       WavgPx;                   //加权均价
    long      BusinessAmount;           //总成交量
    long      BusinessBalance;          //总成交额

    CHundsunH5sdkRspDay5TrendTrendGrpField()
    {
        Date = 0;
        MinTime = 0;
        HqPx = 0;
        WavgPx = 0;
        BusinessAmount = 0;
        BusinessBalance = 0;
    };
};

//6007 5日分时应答
struct CHundsunH5sdkRspDay5TrendField
{
    char      ProdCode[255];            //证券代码
    char      HqTypeCode[255];          //类型代码
    vector<CHundsunH5sdkRspDay5TrendTrendGrpField> TrendGrp; //分时数据

    CHundsunH5sdkRspDay5TrendField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
    };
};

//7001 补线返回请求 - 代码列表
struct CHundsunH5sdkReqMaintainReturnProdGrpField
{
    char      ProdCode[255];            //股票代码
    char      HqTypeCode[255];          //类型代码,[subtype][.cficode].exchage

    CHundsunH5sdkReqMaintainReturnProdGrpField()
    {
        memset(ProdCode, 0, sizeof(ProdCode));
        memset(HqTypeCode, 0, sizeof(HqTypeCode));
    };
};

//7001 补线返回请求
struct CHundsunH5sdkReqMaintainReturnField
{
    int       Subkind;                  //补线类型
    int       Maintainreturnvalue;      //
    vector<CHundsunH5sdkReqMaintainReturnProdGrpField> ProdGrp; //代码列表

    CHundsunH5sdkReqMaintainReturnField()
    {
        Subkind = 0;
        Maintainreturnvalue = 0;
    };
};

//7001 补线返回应答
struct CHundsunH5sdkRspMaintainReturnField
{

    CHundsunH5sdkRspMaintainReturnField()
    {
    };
};

#endif
