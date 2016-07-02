#if !defined(MARKDATAUSERAPISTRUCT_H)
#define MARKDATAUSERAPISTRUCT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PackerVersion 0x20
#define PANICHEARTBEATTIME (20*1000)

#define FuncMarkdatainsert              338499  ///行情回库

//错误信息
struct CMarkDataRspInfoField
{
    int       ErrorNo;                  //错误代码
    char      ErrorInfo[255];           //错误提示
};

//用户信息
struct CMarkDataUserInfoField
{
    char      UserToken[512];           //用户口令
    int       OpBranchNo;               //操作分支机构
    char      OpEntrustWay;             //委托方式
    char      OpStation[255];           //站点地址
    int       BranchNo;                 //分支机构
    char      ClientId[18];             //客户编号
    char      FundAccount[18];          //资产账户
    char      Password[50];             //密码
};

//338499 行情回库请求
struct CMarkDataReqFuncMarkdatainsertField
{
    int       UpdateTime;               //最后冻结更新时间
    char      FutuExchType[4];          //交易类别
    char      FutuCode[30];             //合约代码
    char      FutuName[30];             //品种简称
    double    LastPrice;                //最新价
    double    PreSettlementPrice;       //昨日结算价
    double    PreClosePrice;            //昨收盘
    double    PreOpenInterest;          //昨日空盘量
    double    OpenPrice;                //开盘价
    double    HighPrice;                //最高价
    double    LowPrice;                 //最低价
    double    BusinessBalance;          //成交金额
    int       BusinessAmount;           //成交数量
    int       BusinCount;               //发生笔数
    double    OpenInterest;             //总持量
    double    ClosePrice;               //昨收盘
    double    SquarePrice;              //结算价
    double    UplimitedPrice;           //涨停板价格
    double    DownlimitedPrice;         //跌停板价格
    double    BuyPrice1;                //申买价一
    int       BuyAmount1;               //申买量一
    double    SalePrice1;               //申卖价一
    int       SaleAmount1;              //申卖量一
    double    BuyPrice2;                //申买价二
    int       BuyAmount2;               //申买量二
    double    SalePrice2;               //申卖价二
    int       SaleAmount2;              //申卖量二
    double    BuyPrice3;                //申买价三
    int       BuyAmount3;               //申买量三
    double    SalePrice3;               //申卖价三
    int       SaleAmount3;              //申卖量三
    double    BuyPrice4;                //申买价四
    int       BuyAmount4;               //申买量四
    double    SalePrice4;               //申卖价四
    int       SaleAmount4;              //申卖量四
    double    BuyPrice5;                //申买价五
    int       BuyAmount5;               //申买量五
    double    SalePrice5;               //申卖价五
    int       SaleAmount5;              //申卖量五
};

//338499 行情回库应答
struct CMarkDataRspFuncMarkdatainsertField
{
};

#endif
