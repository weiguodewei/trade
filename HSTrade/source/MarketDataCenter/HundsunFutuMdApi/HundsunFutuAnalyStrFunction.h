#ifndef HUNDSUNFUTUANALYSTRFUNCTION_H
#define HUNDSUNFUTUANALYSTRFUNCTION_H

#define  UDP_DELIMITE_STR  "\1"

inline void ParseValue(double &val, int size , char *str)
{
    val = atof(str);
}

inline void ParseValue(char& val, int size , char *str)
{
    val = str[0];
}

inline void ParseValue(int& val, int size , char *str)
{
    val = atoi(str);
}

inline void ParseValue(float& val, int size, char *str)
{
    val = atof(str);
}

inline void ParseValue(char *val, int size , char *str)
{
    hs_strncpy(val, str, size);
}

inline void ParseValue(unsigned int& val, int size , char *str)
{
    val = 0;
    for(int i= 0; str[i] != 0; i++)
    {
        if('0' <= str[i] && str[i] <= '9')
        {
            val = val * 10 + str[i] - '0';
        }
        else
        {
            val = 0;
            break;
        }
    }
}

#define BEGINPPARSE(type,outdataPtr,indataStrptr, delim) bool __packValid = true; \
    do{   type *__dataptr = outdataPtr; \
    char *__delim = delim; \
    char *__toksaveptr; \
    char *__token;\
    __token = strtok_t(indataStrptr, __delim, &__toksaveptr);

#define PARSEVALUE(val) if( __token != NULL ) \
{   \
    ParseValue(__dataptr->val,sizeof(__dataptr->val), __token); \
    __token = strtok_t(NULL, __delim, &__toksaveptr); \
}else {  __packValid = false; break; }

#define INGOREVALUE(val) if( __token != NULL ) \
{   \
    __token = strtok_t(NULL, __delim, &__toksaveptr); \
}else {  __packValid = false; break; }

#define ENDPARSE }while(0);

#define ISPACKValid  __packValid

//解析主推-期货单腿行情
inline bool ParseFuncQhSingleHqPush(CHundsunFutuRtnFuncQhSingleHqPushField *pFuncQhSingleHqPush, char *qhstr)
{
    memset(pFuncQhSingleHqPush, 0, sizeof(CHundsunFutuRtnFuncQhSingleHqPushField));
    BEGINPPARSE(CHundsunFutuRtnFuncQhSingleHqPushField,pFuncQhSingleHqPush,qhstr,UDP_DELIMITE_STR);
    PARSEVALUE(QuoteType);              //行情类型(0-境内普通行情1-境外行情)
    PARSEVALUE(QuoteLength);            //行情数据长度（从第3字段开始到最后一个字段计算长度）
    PARSEVALUE(UpdateTime);             //时间戳(eg:144510500,14点45分10秒500毫秒)
    PARSEVALUE(QuoteIndex);             //行情序号
    PARSEVALUE(FutuExchType);           //交易类别
    PARSEVALUE(CommodityType);          //商品代码
    PARSEVALUE(FutuCode);               //合约代码
    PARSEVALUE(PreSettlementPrice);     //昨日结算价
    PARSEVALUE(PreClosePrice);          //昨日收盘价
    PARSEVALUE(PreOpenInterest);        //昨日空盘量
    PARSEVALUE(PreDelta);               //昨日虚实度
    PARSEVALUE(FutuOpenPrice);          //开盘价
    PARSEVALUE(FutuLastPrice);          //最新价格
    PARSEVALUE(BuyHighPrice);           //最高买入价
    PARSEVALUE(BuyHighAmount);          //最高买价买入量
    PARSEVALUE(LowSellPrice);           //最低卖价价格
    PARSEVALUE(LowSellAmount);          //最低卖价卖量
    PARSEVALUE(FutuHighPrice);          //最高价
    PARSEVALUE(FutuLowPrice);           //最低价
    PARSEVALUE(UplimitedPrice);         //涨停板
    PARSEVALUE(DownlimitedPrice);       //跌停板
    PARSEVALUE(AveragePrice);           //均价
    PARSEVALUE(ChangeDirection);        //趋势
    PARSEVALUE(BusinessAmount);         //成交量
    PARSEVALUE(OpenInterest);           //总持量
    PARSEVALUE(BusinessBalance);        //成交额
    PARSEVALUE(FutuClosePrice);         //今日收盘价
    PARSEVALUE(SquarePrice);            //结算价
    PARSEVALUE(CurrDate);               //当前日期(yyyymmdd)
    ENDPARSE;
    return ISPACKValid;
}

//解析主推-期货委托回报信息
inline bool ParseFuncQhEntrustPush(CHundsunFutuRtnFuncQhEntrustPushField *pFuncQhEntrustPush, char *qhstr)
{
    memset(pFuncQhEntrustPush, 0, sizeof(CHundsunFutuRtnFuncQhEntrustPushField));
    BEGINPPARSE(CHundsunFutuRtnFuncQhEntrustPushField,pFuncQhEntrustPush,qhstr,UDP_DELIMITE_STR);
    PARSEVALUE(EntrustNo);              //委托号
    PARSEVALUE(FuturesAccount);         //交易编码
    PARSEVALUE(FutuExchType);           //交易所类别（F1-郑州F2大连-F3上海-F4中金）
    PARSEVALUE(FutuCode);               //合约代码
    PARSEVALUE(EntrustBs);              //买卖标识(1-买入 2-卖出)
    PARSEVALUE(FuturesDirection);       //开平标识(1-开仓2-平仓4-平今仓，境外为空)
    PARSEVALUE(HedgeType);              //套保标识(0-投机1-套保2-套利)
    PARSEVALUE(FundAccount);            //资金账户
    PARSEVALUE(ReportId);               //本地单号
    PARSEVALUE(FirmId);                 //会员号
    PARSEVALUE(OperatorNo);             //操作员号
    PARSEVALUE(ClientGroup);            //客户类别
    PARSEVALUE(EntrustAmount);          //委托数量
    PARSEVALUE(TotalBusinessAmount);    //成交总数量
    PARSEVALUE(WithdrawAmount);         //撤单数量
    PARSEVALUE(FutuEntrustPrice);       //委托价格
    PARSEVALUE(EntrustStatus);          //委托状态
    PARSEVALUE(BranchNo);               //营业部号
    PARSEVALUE(BatchNo);                //委托批号/报价编号
    PARSEVALUE(EntrustType);            //委托类型
    PARSEVALUE(AmountPerHand);          //合约乘数
    PARSEVALUE(ForcedropReason);        //强平原因
    PARSEVALUE(InitDate);               //交易日期
    PARSEVALUE(EntrustTime);            //当前时间
    PARSEVALUE(ConfirmId);              //主场单号
    PARSEVALUE(EntrustOccasion);        //委托场景
    PARSEVALUE(FutuEntrustPrice2);      //委托价格
    PARSEVALUE(EntrustProp);            //期货委托属性(0：限价单；1：市价单 2：止损定单 3：止盈定单 4：限价止损定单 5：限价止盈定单 6：止损 7：组合定单 A：跨期套利确认；B：持仓套保确认；C：请求报价；D：期权权力行使；E：期权权力放弃；F：双边报价)
    PARSEVALUE(ArbitCode);              //套利合约代码
    PARSEVALUE(EntrustReference);       //委托引用
    PARSEVALUE(ErrorMessage);           //错误说明
    PARSEVALUE(PositionStr);            //定位串
    PARSEVALUE(SessionNo);              //会话编号
    PARSEVALUE(SecondCode);             //第二腿合约代码
    PARSEVALUE(WeaveType);              //组合类型
    PARSEVALUE(DeltaEntrustFare);       //委托费用变化
    PARSEVALUE(DeltaEntrustMargin);     //委托保证金变化
    PARSEVALUE(DeltaEntrustPremium);    //委托权利金变化
    ENDPARSE;
    return ISPACKValid;
}

//解析主推-期货成交回报信息
inline bool ParseFuncQhRealPush(CHundsunFutuRtnFuncQhRealPushField *pFuncQhRealPush, char *qhstr)
{
    memset(pFuncQhRealPush, 0, sizeof(CHundsunFutuRtnFuncQhRealPushField));
    BEGINPPARSE(CHundsunFutuRtnFuncQhRealPushField,pFuncQhRealPush,qhstr,UDP_DELIMITE_STR);
    PARSEVALUE(EntrustNo);              //委托号
    PARSEVALUE(FuturesAccount);         //交易编码
    PARSEVALUE(FutuExchType);           //交易所类别
    PARSEVALUE(BusinessNo);             //成交编号
    PARSEVALUE(FutuCode);               //合约代码
    PARSEVALUE(EntrustBs);              //买卖标识(1-买入 2-卖出)
    PARSEVALUE(FuturesDirection);       //开平标识(1-开仓2-平仓3-平今仓  境外为空)
    PARSEVALUE(FutuBusinessPrice);      //成交价格
    PARSEVALUE(BusinessAmount);         //成交数量
    PARSEVALUE(HedgeType);              //套保标识(0-投机1-套保2-套利)
    PARSEVALUE(FundAccount);            //资金账户
    PARSEVALUE(ReportId);               //本地单号
    PARSEVALUE(FirmId);                 //会员号
    PARSEVALUE(OperatorNo);             //操作员号
    PARSEVALUE(ClientGroup);            //客户类别
    PARSEVALUE(EntrustAmount);          //委托数量
    PARSEVALUE(TotalBusinessAmount);    //成交总数量
    PARSEVALUE(WithdrawAmount);         //撤单数量
    PARSEVALUE(FutuEntrustPrice);       //委托价格
    PARSEVALUE(EntrustStatus);          //委托状态
    PARSEVALUE(BranchNo);               //营业部号
    PARSEVALUE(BatchNo);                //委托批号/报价编号
    PARSEVALUE(EntrustType);            //委托类型
    PARSEVALUE(AmountPerHand);          //合约乘数
    PARSEVALUE(ForcedropReason);        //强平原因
    PARSEVALUE(InitDate);               //交易日期
    PARSEVALUE(BusinessTime);           //成交时间
    PARSEVALUE(ConfirmId);              //主场单号
    PARSEVALUE(EntrustOccasion);        //委托场景
    PARSEVALUE(FutuEntrustPrice2);      //委托数量(委托输入价格)
    PARSEVALUE(EntrustProp);            //期货委托属性(0：限价单；1：市价单 2：止损定单 3：止盈定单 4：限价止损定单 5：限价止盈定单 6：止损 7：组合定单 A：跨期套利确认；B：持仓套保确认；C：请求报价；D：期权权力行使；E：期权权力放弃；F：双边报价)
    PARSEVALUE(ArbitCode);              //套利合约代码
    PARSEVALUE(EntrustReference);       //委托引用
    PARSEVALUE(PositionStr);            //定位串
    PARSEVALUE(SessionNo);              //会话编号
    PARSEVALUE(DeltaEntrustFare);       //委托费用变化
    PARSEVALUE(DeltaBusinessFare);      //成交费用变化
    PARSEVALUE(DeltaEntrustMargin);     //委托保证金变化
    PARSEVALUE(DeltaHoldMargin);        //持仓保证金变化
    PARSEVALUE(DeltaEntrustPremium);    //委托权利金变化
    PARSEVALUE(DeltaPremium);           //成交权利金变化
    PARSEVALUE(DeltaDropIncome);        //平仓盯市盈亏
    PARSEVALUE(DeltaDropIncomeFloat);   //平仓浮动盈亏
    ENDPARSE;
    return ISPACKValid;
}

//解析主推-期货组合行情
inline bool ParseFuncQhCombinePush(CHundsunFutuRtnFuncQhCombinePushField *pFuncQhCombinePush, char *qhstr)
{
    memset(pFuncQhCombinePush, 0, sizeof(CHundsunFutuRtnFuncQhCombinePushField));
    BEGINPPARSE(CHundsunFutuRtnFuncQhCombinePushField,pFuncQhCombinePush,qhstr,UDP_DELIMITE_STR);
    PARSEVALUE(ArbitCode);              //套利合约号
    PARSEVALUE(FutuExchType);           //交易类别
    PARSEVALUE(FirstCode);              //第一腿
    PARSEVALUE(SecondCode);             //第二腿
    PARSEVALUE(WeaveType);              //组合类型1-SPD, 2 -IPS
    PARSEVALUE(FutuBuyPrice);           //最高买入价
    PARSEVALUE(BuyAmount);              //最高买入价买量
    PARSEVALUE(TotalBuyAmount);         //全部买量
    PARSEVALUE(FutuSellPrice);          //最低卖价价格
    PARSEVALUE(SellAmount);             //最低卖价卖量
    PARSEVALUE(TotalSaleAmount);        //全部卖量
    PARSEVALUE(FutuHighPrice);          //最高价
    PARSEVALUE(FutuLowPrice);           //最低价
    PARSEVALUE(UplimitedPrice);         //涨停板价格
    PARSEVALUE(DownlimitedPrice);       //跌停板价格
    ENDPARSE;
    return ISPACKValid;
}

//解析主推-期货交易所状态信息
inline bool ParseFuncQhExchStatusPush(CHundsunFutuRtnFuncQhExchStatusPushField *pFuncQhExchStatusPush, char *qhstr)
{
    memset(pFuncQhExchStatusPush, 0, sizeof(CHundsunFutuRtnFuncQhExchStatusPushField));
    BEGINPPARSE(CHundsunFutuRtnFuncQhExchStatusPushField,pFuncQhExchStatusPush,qhstr,UDP_DELIMITE_STR);
    PARSEVALUE(FutuExchType);           //交易所类别（F1-郑州F2大连-F3上海-F4中金）
    PARSEVALUE(ExchStatus);             //交易所状态（0连接断开,1开盘前, 2集合报价 3竞价平衡 4竞价撮合 5 连续交易 6暂停交易 7闭市 X未知）
    PARSEVALUE(EntradeFlag);            //是否可交易（0不可交易，1可交易）
    ENDPARSE;
    return ISPACKValid;
}

//解析主推-期货询价通知
inline bool ParseFuncQhInquiryPush(CHundsunFutuRtnFuncQhInquiryPushField *pFuncQhInquiryPush, char *qhstr)
{
    memset(pFuncQhInquiryPush, 0, sizeof(CHundsunFutuRtnFuncQhInquiryPushField));
    BEGINPPARSE(CHundsunFutuRtnFuncQhInquiryPushField,pFuncQhInquiryPush,qhstr,UDP_DELIMITE_STR);
    PARSEVALUE(InquiryNo);              //询价编号
    PARSEVALUE(FutuCode);               //合约代码
    PARSEVALUE(InitDate);               //交易日期
    PARSEVALUE(UpdateTime);             //询价时间(eg:144510500,14点45分10秒500毫秒)
    ENDPARSE;
    return ISPACKValid;
}

//解析主推-期货报价回报
inline bool ParseFuncQhQuotePush(CHundsunFutuRtnFuncQhQuotePushField *pFuncQhQuotePush, char *qhstr)
{
    memset(pFuncQhQuotePush, 0, sizeof(CHundsunFutuRtnFuncQhQuotePushField));
    BEGINPPARSE(CHundsunFutuRtnFuncQhQuotePushField,pFuncQhQuotePush,qhstr,UDP_DELIMITE_STR);
    PARSEVALUE(FutuExchType);           //交易所类别（F1-郑州F2大连-F3上海-F4中金）
    PARSEVALUE(FutuCode);               //合约代码
    PARSEVALUE(BuyFuturesDirection);    //买方开平标志
    PARSEVALUE(BuyHedgeType);           //买方套保标志
    PARSEVALUE(BuyQuotePrice);          //买方价格
    PARSEVALUE(BuyQuoteAmount);         //买方量
    PARSEVALUE(SellFuturesDirection);   //卖方开平标志
    PARSEVALUE(SellHedgeType);          //卖方套保标志
    PARSEVALUE(SellQuotePrice);         //卖方价格
    PARSEVALUE(SellQuoteAmount);        //卖方量
    PARSEVALUE(QuoteNo);                //报价编号
    PARSEVALUE(FundAccount);            //资金账户
    PARSEVALUE(ReportId);               //本地单号
    PARSEVALUE(FirmId);                 //会员号
    PARSEVALUE(OperatorNo);             //操作员号
    PARSEVALUE(QuoteStatus);            //委托状态
    PARSEVALUE(BranchNo);               //营业部号
    PARSEVALUE(InitDate);               //交易日期
    PARSEVALUE(CurrTime);               //当前时间
    PARSEVALUE(ConfirmId);              //主场单号
    PARSEVALUE(BuyConfirmId);           //买方主场单号
    PARSEVALUE(SellConfirmId);          //卖方主场单号
    PARSEVALUE(EntrustOccasion);        //委托场景
    PARSEVALUE(EntrustReference);       //报价引用
    PARSEVALUE(RequestId);              //请求编号
    PARSEVALUE(PositionStr);            //定位串
    ENDPARSE;
    return ISPACKValid;
}

//解析主推-个股期权成交回报
inline bool ParseFuncOptDealPush(CHundsunFutuRtnFuncOptDealPushField *pFuncOptDealPush, char *qhstr)
{
    memset(pFuncOptDealPush, 0, sizeof(CHundsunFutuRtnFuncOptDealPushField));
    BEGINPPARSE(CHundsunFutuRtnFuncOptDealPushField,pFuncOptDealPush,qhstr,UDP_DELIMITE_STR);
    PARSEVALUE(BranchNo);               //分支机构
    PARSEVALUE(ClientId);               //客户编号
    PARSEVALUE(FundAccount);            //资产账户
    PARSEVALUE(RealType);               //成交类型
    PARSEVALUE(RealStatus);             //处理标志
    PARSEVALUE(ExchangeType);           //交易类别
    PARSEVALUE(ReportSeat);             //申报席位
    PARSEVALUE(ReportNo);               //申请编号
    PARSEVALUE(ReportAccount);          //申报账号
    PARSEVALUE(StockAccount);           //证券账号
    PARSEVALUE(OptholdType);            //期权持仓类别
    PARSEVALUE(OptionCode);             //期权合约编码
    PARSEVALUE(OptionType);             //期权种类
    PARSEVALUE(RecordNo);               //委托库记录号
    PARSEVALUE(ReportBs);               //申报方向
    PARSEVALUE(EntrustBs);              //买卖方向
    PARSEVALUE(EntrustOc);              //开平仓方向
    PARSEVALUE(CoveredFlag);            //备兑标志
    PARSEVALUE(EntrustStatus);          //委托状态
    PARSEVALUE(EntrustNo);              //委托编号
    PARSEVALUE(BusinessId);             //成交编号
    PARSEVALUE(BusinessAmount);         //成交数量
    PARSEVALUE(OptBusinessPrice);       //成交价格
    PARSEVALUE(BusinessTime);           //成交时间
    PARSEVALUE(ExternCode);             //外部错误代码
    PARSEVALUE(ReportTime);             //申报时间
    PARSEVALUE(EntrustAmount);          //委托数量
    PARSEVALUE(BatchNo);                //委托批号
    PARSEVALUE(OptEntrustPrice);        //委托价格
    PARSEVALUE(EntrustType);            //委托类别
    ENDPARSE;
    return ISPACKValid;
}

//解析主推-个股期权委托回写
inline bool ParseFuncOptEntrustPush(CHundsunFutuRtnFuncOptEntrustPushField *pFuncOptEntrustPush, char *qhstr)
{
    memset(pFuncOptEntrustPush, 0, sizeof(CHundsunFutuRtnFuncOptEntrustPushField));
    BEGINPPARSE(CHundsunFutuRtnFuncOptEntrustPushField,pFuncOptEntrustPush,qhstr,UDP_DELIMITE_STR);
    PARSEVALUE(FundAccount);            //资产账户
    PARSEVALUE(ReportNo);               //申请编号
    PARSEVALUE(OptionCode);             //期权合约编码
    PARSEVALUE(EntrustBs);              //买卖方向
    PARSEVALUE(EntrustOc);              //开平仓方向
    PARSEVALUE(CoveredFlag);            //备兑标志
    PARSEVALUE(EntrustStatus);          //委托状态
    PARSEVALUE(EntrustNo);              //委托编号
    PARSEVALUE(EntrustAmount);          //委托数量
    PARSEVALUE(BatchNo);                //委托批号
    PARSEVALUE(OptEntrustPrice);        //委托价格
    PARSEVALUE(EntrustType);            //委托类别
    PARSEVALUE(ErrorNo);                //错误代码
    PARSEVALUE(ErrorInfo);              //错误提示
    ENDPARSE;
    return ISPACKValid;
}

#endif
