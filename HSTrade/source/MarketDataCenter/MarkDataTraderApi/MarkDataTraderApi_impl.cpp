#include "MarkDataTraderApi_impl.h"
#include "MarkDataAnalyStrFunction.h"

//处理错误信息
inline void DealErrorInfo(IF2UnPacker *lpUnPacker, CMarkDataRspInfoField *RspInfo)
{
    RspInfo->ErrorNo = lpUnPacker->GetInt("error_no");
    if(RspInfo->ErrorNo == 0)
    {
        hs_strncpy(RspInfo->ErrorInfo, "No Error", sizeof(RspInfo->ErrorInfo));
    }
    else
    {
        hs_strncpy(RspInfo->ErrorInfo, lpUnPacker->GetStr("error_info"), sizeof(RspInfo->ErrorInfo));
    }
};

CMarkDataTraderApi *CMarkDataTraderApi::CreateMarkDataTraderApi(const char *pszFlowPath, bool bSaveLog)
{
    return new CMarkDataTraderApi_impl(pszFlowPath, bSaveLog);
}

CMarkDataTraderApi_impl::CMarkDataTraderApi_impl(const char *pszFlowPath, bool bSaveLog)
{
    lpTraderSpi = NULL;
    lpTraderApi_log = NULL;

    m_localip = "";
    m_localMac = "";
    m_Logined = false;
    m_SaveLog = bSaveLog;
    m_bHeartKeepStop = false;

    memset(&m_userInfo, 0, sizeof(CMarkDataUserInfoField));
}

CMarkDataTraderApi_impl::~CMarkDataTraderApi_impl()
{
    m_bHeartKeepStop = true;
    if(m_HeartKeepTrd.IsRunning())
    {
        m_HeartKeepTrd.Join();
    }
    StopTrd();

    hJoinEvent.Notify();
}

void CMarkDataTraderApi_impl::ReleaseApi()
{
    delete this;
}

void CMarkDataTraderApi_impl::Init(const char *pszLicFile, const char *pszPwd, const char *pszSslFile, const char *pszSslPwd)
{
    InitT2Connect(pszLicFile, pszPwd, pszSslFile, pszSslPwd);
    return;
}

int CMarkDataTraderApi_impl::Join()
{
    hJoinEvent.Wait(-1);
    return 0;
}

void CMarkDataTraderApi_impl::RegisterFront(const char *pszFrontAddress, int nSubSystemNo, int nCompanyID, const char *pszConnName)
{
    //赋值前置地址
    hs_strncpy(m_szFrontAddress, pszFrontAddress, sizeof(m_szFrontAddress));
    //赋值子系统号
    m_SubSystemNo = nSubSystemNo;
    //赋值公司编号
    m_CompanyID = nCompanyID;
    //生成T2的Login_name
    hs_strncpy(m_szConnName, pszConnName, sizeof(m_szConnName));

    return;
}

void CMarkDataTraderApi_impl::RegisterSpi(CMarkDataTraderSpi *pSpi)
{
    lpTraderSpi = pSpi;
    if(pSpi != NULL)
    {
        StartTrd();
    }
    else
    {
        StopTrd();
    }
}

CMarkDataUserInfoField* CMarkDataTraderApi_impl::GetUserInfo()
{
    return &m_userInfo;
}

//338499 行情回库请求
int CMarkDataTraderApi_impl::ReqFuncMarkdatainsert(CMarkDataReqFuncMarkdatainsertField *pReqFuncMarkdatainsert, int nRequestID, void** ppRetData, int* pnItems, CMarkDataRspInfoField *pRspInfo)
{
    if(m_SaveLog && lpTraderApi_log != NULL)
        lpTraderApi_log->AddLog(LOGINFO_Request, FuncMarkdatainsert, 0, pReqFuncMarkdatainsert, sizeof(CMarkDataReqFuncMarkdatainsertField));

    //创建Pack包
    IF2Packer *lpF2Packer = NewPacker(PackerVersion);
    lpF2Packer->AddRef();
    lpF2Packer->BeginPack();
    lpF2Packer->AddField("user_token",'S',512);                                 //用户口令
    lpF2Packer->AddField("op_branch_no",'I');                                   //操作分支机构
    lpF2Packer->AddField("op_entrust_way",'C');                                 //委托方式
    lpF2Packer->AddField("op_station",'S',255);                                 //站点地址
    lpF2Packer->AddField("branch_no",'I');                                      //分支机构
    lpF2Packer->AddField("client_id",'S',18);                                   //客户编号
    lpF2Packer->AddField("fund_account",'S',18);                                //资产账户
    lpF2Packer->AddField("password",'S',50);                                    //密码
    lpF2Packer->AddField("update_time",'I');                                    //最后冻结更新时间
    lpF2Packer->AddField("futu_exch_type",'S',4);                               //交易类别
    lpF2Packer->AddField("futu_code",'S',30);                                   //合约代码
    lpF2Packer->AddField("futu_name",'S',30);                                   //品种简称
    lpF2Packer->AddField("last_price",'F', 12, 6);                              //最新价
    lpF2Packer->AddField("pre_settlement_price",'F', 12, 6);                    //昨日结算价
    lpF2Packer->AddField("pre_close_price",'F', 12, 6);                         //昨收盘
    lpF2Packer->AddField("pre_open_interest",'F', 16, 2);                       //昨日空盘量
    lpF2Packer->AddField("open_price",'F', 12, 6);                              //开盘价
    lpF2Packer->AddField("high_price",'F', 12, 6);                              //最高价
    lpF2Packer->AddField("low_price",'F', 12, 6);                               //最低价
    lpF2Packer->AddField("business_balance",'F', 16, 2);                        //成交金额
    lpF2Packer->AddField("business_amount",'I');                                //成交数量
    lpF2Packer->AddField("busin_count",'I');                                    //发生笔数
    lpF2Packer->AddField("open_interest",'F', 16, 2);                           //总持量
    lpF2Packer->AddField("close_price",'F', 12, 6);                             //昨收盘
    lpF2Packer->AddField("square_price",'F', 12, 6);                            //结算价
    lpF2Packer->AddField("uplimited_price",'F', 12, 6);                         //涨停板价格
    lpF2Packer->AddField("downlimited_price",'F', 12, 6);                       //跌停板价格
    lpF2Packer->AddField("buy_price1",'F', 12, 6);                              //申买价一
    lpF2Packer->AddField("buy_amount1",'I');                                    //申买量一
    lpF2Packer->AddField("sale_price1",'F', 12, 6);                             //申卖价一
    lpF2Packer->AddField("sale_amount1",'I');                                   //申卖量一
    lpF2Packer->AddField("buy_price2",'F', 12, 6);                              //申买价二
    lpF2Packer->AddField("buy_amount2",'I');                                    //申买量二
    lpF2Packer->AddField("sale_price2",'F', 12, 6);                             //申卖价二
    lpF2Packer->AddField("sale_amount2",'I');                                   //申卖量二
    lpF2Packer->AddField("buy_price3",'F', 12, 6);                              //申买价三
    lpF2Packer->AddField("buy_amount3",'I');                                    //申买量三
    lpF2Packer->AddField("sale_price3",'F', 12, 6);                             //申卖价三
    lpF2Packer->AddField("sale_amount3",'I');                                   //申卖量三
    lpF2Packer->AddField("buy_price4",'F', 12, 6);                              //申买价四
    lpF2Packer->AddField("buy_amount4",'I');                                    //申买量四
    lpF2Packer->AddField("sale_price4",'F', 12, 6);                             //申卖价四
    lpF2Packer->AddField("sale_amount4",'I');                                   //申卖量四
    lpF2Packer->AddField("buy_price5",'F', 12, 6);                              //申买价五
    lpF2Packer->AddField("buy_amount5",'I');                                    //申买量五
    lpF2Packer->AddField("sale_price5",'F', 12, 6);                             //申卖价五
    lpF2Packer->AddField("sale_amount5",'I');                                   //申卖量五

    lpF2Packer->AddStr(m_userInfo.UserToken);
    lpF2Packer->AddInt(m_userInfo.OpBranchNo);
    lpF2Packer->AddChar(m_userInfo.OpEntrustWay);
    lpF2Packer->AddStr(m_userInfo.OpStation);
    lpF2Packer->AddInt(m_userInfo.BranchNo);
    lpF2Packer->AddStr(m_userInfo.ClientId);
    lpF2Packer->AddStr(m_userInfo.FundAccount);
    lpF2Packer->AddStr(m_userInfo.Password);
    lpF2Packer->AddInt(pReqFuncMarkdatainsert->UpdateTime);
    lpF2Packer->AddStr(pReqFuncMarkdatainsert->FutuExchType);
    lpF2Packer->AddStr(pReqFuncMarkdatainsert->FutuCode);
    lpF2Packer->AddStr(pReqFuncMarkdatainsert->FutuName);
    lpF2Packer->AddDouble(pReqFuncMarkdatainsert->LastPrice);
    lpF2Packer->AddDouble(pReqFuncMarkdatainsert->PreSettlementPrice);
    lpF2Packer->AddDouble(pReqFuncMarkdatainsert->PreClosePrice);
    lpF2Packer->AddDouble(pReqFuncMarkdatainsert->PreOpenInterest);
    lpF2Packer->AddDouble(pReqFuncMarkdatainsert->OpenPrice);
    lpF2Packer->AddDouble(pReqFuncMarkdatainsert->HighPrice);
    lpF2Packer->AddDouble(pReqFuncMarkdatainsert->LowPrice);
    lpF2Packer->AddDouble(pReqFuncMarkdatainsert->BusinessBalance);
    lpF2Packer->AddInt(pReqFuncMarkdatainsert->BusinessAmount);
    lpF2Packer->AddInt(pReqFuncMarkdatainsert->BusinCount);
    lpF2Packer->AddDouble(pReqFuncMarkdatainsert->OpenInterest);
    lpF2Packer->AddDouble(pReqFuncMarkdatainsert->ClosePrice);
    lpF2Packer->AddDouble(pReqFuncMarkdatainsert->SquarePrice);
    lpF2Packer->AddDouble(pReqFuncMarkdatainsert->UplimitedPrice);
    lpF2Packer->AddDouble(pReqFuncMarkdatainsert->DownlimitedPrice);
    lpF2Packer->AddDouble(pReqFuncMarkdatainsert->BuyPrice1);
    lpF2Packer->AddInt(pReqFuncMarkdatainsert->BuyAmount1);
    lpF2Packer->AddDouble(pReqFuncMarkdatainsert->SalePrice1);
    lpF2Packer->AddInt(pReqFuncMarkdatainsert->SaleAmount1);
    lpF2Packer->AddDouble(pReqFuncMarkdatainsert->BuyPrice2);
    lpF2Packer->AddInt(pReqFuncMarkdatainsert->BuyAmount2);
    lpF2Packer->AddDouble(pReqFuncMarkdatainsert->SalePrice2);
    lpF2Packer->AddInt(pReqFuncMarkdatainsert->SaleAmount2);
    lpF2Packer->AddDouble(pReqFuncMarkdatainsert->BuyPrice3);
    lpF2Packer->AddInt(pReqFuncMarkdatainsert->BuyAmount3);
    lpF2Packer->AddDouble(pReqFuncMarkdatainsert->SalePrice3);
    lpF2Packer->AddInt(pReqFuncMarkdatainsert->SaleAmount3);
    lpF2Packer->AddDouble(pReqFuncMarkdatainsert->BuyPrice4);
    lpF2Packer->AddInt(pReqFuncMarkdatainsert->BuyAmount4);
    lpF2Packer->AddDouble(pReqFuncMarkdatainsert->SalePrice4);
    lpF2Packer->AddInt(pReqFuncMarkdatainsert->SaleAmount4);
    lpF2Packer->AddDouble(pReqFuncMarkdatainsert->BuyPrice5);
    lpF2Packer->AddInt(pReqFuncMarkdatainsert->BuyAmount5);
    lpF2Packer->AddDouble(pReqFuncMarkdatainsert->SalePrice5);
    lpF2Packer->AddInt(pReqFuncMarkdatainsert->SaleAmount5);
    lpF2Packer->EndPack();
    //发送请求包
    int iRet = SendBizMessage(lpF2Packer, FuncMarkdatainsert, 0, nRequestID, ppRetData, pnItems, pRspInfo);
    //最后释放打包器
    lpF2Packer->FreeMem(lpF2Packer->GetPackBuf());
    lpF2Packer->Release();

    return iRet;
};


void CMarkDataTraderApi_impl::OnConnected()
{
    char addr[64];
    char ip[64];
    strcpy(&addr[0], lpConnection->GetRealAddress());
    if(strcmp(addr, "") != 0)
    {
        sscanf(addr, "%[^:]s", ip);
        m_localip = ip;
    }
    strcpy(&addr[0], lpConnection->GetSelfMac());
    if(strcmp(addr, "") != 0)
    {
        m_localMac = addr;
    }
    sprintf(m_userInfo.OpStation, "%s:%s", m_localip.c_str(), m_localMac.c_str());

    if(lpTraderSpi)
    {
        lpTraderSpi->OnFrontConnected();
    }

}

void CMarkDataTraderApi_impl::OnClosed()
{
    if(lpTraderSpi)
    {
        lpTraderSpi->OnFrontDisconnected(0x1001);
    }
}

void CMarkDataTraderApi_impl::DealAsyBizMessage(IBizMessage* lpMsg)
{
    if (lpMsg!=NULL)
    {
        //成功,应用程序不能释放lpBizMessageRecv消息
        if (lpMsg->GetErrorNo() ==0)
        {
            int iLen = 0;
            const void *lpBuffer = lpMsg->GetContent(iLen);
            IF2UnPacker *lpUnPacker = NewUnPacker((void *)lpBuffer,iLen);
            if(lpUnPacker != NULL)
                lpUnPacker->AddRef();
            int iLen_key = 0;
            const void *lpBuffer_key = lpMsg->GetKeyInfo(iLen_key);
            IF2UnPacker *lpUnPacker_key = NewUnPacker((void *)lpBuffer_key,iLen_key);
            if(lpUnPacker_key != NULL)
                lpUnPacker_key->AddRef();

            switch(lpMsg->GetFunction())
            {
            case 620003:
                {
                }
                break;
            case 620001:
                {
                }
                break;
            case 620002:
                {
                }
                break;
            case FuncMarkdatainsert:
                OnAsyResponse_338499(lpUnPacker, lpMsg->GetSenderId());
                break;
            default:
                break;
            }

            if(lpUnPacker != NULL)
                lpUnPacker->Release();
            if(lpUnPacker_key != NULL)
                lpUnPacker_key->Release();

        }
        else
        {
            puts(lpMsg->GetErrorInfo());
        }
    }
};

int CMarkDataTraderApi_impl::DealSynBizMessage(const void* lpBuffer, int nLen, void** ppRetData, int* pnItems, void *pRspInfo)
{
    IBizMessage* lpMsg = NewBizMessage();
    lpMsg->AddRef();
    lpMsg->SetBuff(lpBuffer, nLen);

    int iRet = lpMsg->GetReturnCode();
    if (iRet == 0 || iRet > 1)
    {
        int iLen = 0;
        const void *lpBuffer = lpMsg->GetContent(iLen);
        IF2UnPacker *lpUnPacker = NewUnPacker((void *)lpBuffer,iLen);
        int iLen_key = 0;
        const void *lpBuffer_key = lpMsg->GetKeyInfo(iLen_key);
        IF2UnPacker *lpUnPacker_key = NewUnPacker((void *)lpBuffer_key,iLen_key);

        switch(lpMsg->GetFunction())
        {
        case FuncMarkdatainsert:
            OnSynResponse_338499(lpUnPacker, ppRetData, pnItems, (CMarkDataRspInfoField *)pRspInfo);
            break;
        default:
            break;
        }
    }
    else
    {
        //取-1、1错误信息
        GetMsgErrorInfo(iRet, pRspInfo);
    }

    lpMsg->Release();
    return iRet;
}

void CMarkDataTraderApi_impl::RecHeartbeet()
{
    m_iHeartKeep = 0;
}

void CMarkDataTraderApi_impl::GetMsgErrorInfo(int nErrorCode, void *pRspInfo)
{
    if(pRspInfo != NULL)
    {
        ((CMarkDataRspInfoField *)pRspInfo)->ErrorNo = nErrorCode;
        hs_strncpy(((CMarkDataRspInfoField *)pRspInfo)->ErrorInfo, lpConnection->GetErrorMsg(nErrorCode), sizeof(((CMarkDataRspInfoField *)pRspInfo)->ErrorInfo));
    }
}

//338499 行情回库
void CMarkDataTraderApi_impl::OnAsyResponse_338499(IF2UnPacker *lpUnPacker, int nRequestID)
{
    //判断错误信息
    CMarkDataRspInfoField RspInfo;
    //返回信息结构体
    CMarkDataRspFuncMarkdatainsertField RspFuncMarkdatainsert;
    memset(&RspFuncMarkdatainsert, 0, sizeof(CMarkDataRspFuncMarkdatainsertField));

    //获得错误信息
    DealErrorInfo(lpUnPacker, &RspInfo);
    if( RspInfo.ErrorNo != 0 )
    {
        if(m_SaveLog && lpTraderApi_log != NULL)
            lpTraderApi_log->AddLog(LOGINFO_Response, FuncMarkdatainsert, 0, &RspFuncMarkdatainsert, sizeof(CMarkDataRspFuncMarkdatainsertField), &RspInfo, sizeof(CMarkDataRspInfoField));
        lpTraderSpi->OnRspFuncMarkdatainsert(NULL, &RspInfo, nRequestID, true);
    }
    else
    {

        //记录日志
        if(m_SaveLog && lpTraderApi_log != NULL)
            lpTraderApi_log->AddLog(LOGINFO_Response, FuncMarkdatainsert, 0, &RspFuncMarkdatainsert, sizeof(CMarkDataRspFuncMarkdatainsertField), &RspInfo, sizeof(CMarkDataRspInfoField));
        //回调
        lpTraderSpi->OnRspFuncMarkdatainsert(&RspFuncMarkdatainsert, &RspInfo, nRequestID, true);
    }
}

//338499 行情回库
void CMarkDataTraderApi_impl::OnSynResponse_338499(IF2UnPacker *lpUnPacker, void** ppRetData, int* pnItems, CMarkDataRspInfoField *pRspInfo)
{
    //获得错误信息
    DealErrorInfo(lpUnPacker, pRspInfo);
    if( pRspInfo->ErrorNo != 0 )
        return;

    *pnItems = lpUnPacker->GetRowCount();
    if(*pnItems > 0)
    {
        *ppRetData = new CMarkDataRspFuncMarkdatainsertField[*pnItems];
        CMarkDataRspFuncMarkdatainsertField* ppRspFuncMarkdatainsert = (CMarkDataRspFuncMarkdatainsertField*) (*ppRetData);

        int ct = 0;
        while (!lpUnPacker->IsEOF())
        {

            ct++;
            lpUnPacker->Next();
        }
    }
    return;
}

void CMarkDataTraderApi_impl::HeartKeep(void* lpvoid)
{
    CMarkDataTraderApi_impl *helper = (CMarkDataTraderApi_impl*)lpvoid;
    while( helper->m_bHeartKeepStop == false )
    {
        helper->m_HeartKeepEvent.Wait(PANICHEARTBEATTIME);
        helper->m_iHeartKeep++;
        if (helper->m_iHeartKeep >= 3)
        {
            if(helper->m_Logined)
            {
                //默认订阅主推信息
            }
        }
    }
}
