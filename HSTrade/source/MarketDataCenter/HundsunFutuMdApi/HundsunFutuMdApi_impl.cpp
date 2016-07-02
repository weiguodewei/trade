#include "HundsunFutuMdApi_impl.h"
#include "HundsunFutuAnalyStrFunction.h"

//处理错误信息
inline void DealErrorInfo(IF2UnPacker *lpUnPacker, CHundsunFutuRspInfoField *RspInfo)
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

CHundsunFutuMdApi *CHundsunFutuMdApi::CreateHundsunFutuMdApi(const char *pszFlowPath, bool bSaveLog)
{
    return new CHundsunFutuMdApi_impl(pszFlowPath, bSaveLog);
}

CHundsunFutuMdApi_impl::CHundsunFutuMdApi_impl(const char *pszFlowPath, bool bSaveLog)
{
    lpMdSpi = NULL;
    lpMdApi_log = NULL;

    m_localip = "";
    m_localMac = "";
    m_Logined = false;
    m_SaveLog = bSaveLog;
    m_bHeartKeepStop = false;

    memset(&m_userInfo, 0, sizeof(CHundsunFutuUserInfoField));
}

CHundsunFutuMdApi_impl::~CHundsunFutuMdApi_impl()
{
    m_bHeartKeepStop = true;
    if(m_HeartKeepTrd.IsRunning())
    {
        m_HeartKeepTrd.Join();
    }
    StopTrd();

    m_SubFutuCodeSet.clear();
    m_SubArbitCodeSet.clear();
    hJoinEvent.Notify();
}

void CHundsunFutuMdApi_impl::ReleaseApi()
{
    delete this;
}

void CHundsunFutuMdApi_impl::Init(const char *pszLicFile, const char *pszPwd, const char *pszSslFile, const char *pszSslPwd)
{
    InitT2Connect(pszLicFile, pszPwd, pszSslFile, pszSslPwd);
    return;
}

int CHundsunFutuMdApi_impl::Join()
{
    hJoinEvent.Wait(-1);
    return 0;
}

void CHundsunFutuMdApi_impl::RegisterFront(const char *pszFrontAddress, int nSubSystemNo, int nCompanyID, const char *pszConnName)
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

void CHundsunFutuMdApi_impl::RegisterSpi(CHundsunFutuMdSpi *pSpi)
{
    lpMdSpi = pSpi;
    if(pSpi != NULL)
    {
        StartTrd();
    }
    else
    {
        StopTrd();
    }
}

CHundsunFutuUserInfoField* CHundsunFutuMdApi_impl::GetUserInfo()
{
    return &m_userInfo;
}

//331100 客户登录请求
int CHundsunFutuMdApi_impl::ReqFuncClientLogin(CHundsunFutuReqFuncClientLoginField *pReqFuncClientLogin, int nRequestID, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo)
{
    if(lpMdApi_log == NULL)
    {
        lpMdApi_log = new CHundsunFutuMdApi_log(pReqFuncClientLogin->AccountContent);
    }
    m_userInfo.OpEntrustWay = pReqFuncClientLogin->OpEntrustWay; //保存委托方式
    hs_strncpy(m_userInfo.Password, pReqFuncClientLogin->Password, sizeof(m_userInfo.Password)); //保存交易密码

    if(m_SaveLog && lpMdApi_log != NULL)
        lpMdApi_log->AddLog(LOGINFO_Request, FuncClientLogin, 0, pReqFuncClientLogin, sizeof(CHundsunFutuReqFuncClientLoginField));

    //创建Pack包
    IF2Packer *lpF2Packer = NewPacker(PackerVersion);
    lpF2Packer->AddRef();
    lpF2Packer->BeginPack();
    lpF2Packer->AddField("op_branch_no",'I');                                   //操作分支机构
    lpF2Packer->AddField("op_entrust_way",'C');                                 //委托方式
    lpF2Packer->AddField("op_station",'S',255);                                 //站点地址
    lpF2Packer->AddField("branch_no",'I');                                      //分支机构
    lpF2Packer->AddField("password",'S',50);                                    //密码
    lpF2Packer->AddField("password_type",'C');                                  //密码类别
    lpF2Packer->AddField("input_content",'C');                                  //客户标志类别
    lpF2Packer->AddField("account_content",'S',30);                             //输入内容
    lpF2Packer->AddField("content_type",'S',6);                                 //银行号、市场类别
    lpF2Packer->AddField("asset_prop",'C');                                     //资产属性

    lpF2Packer->AddInt(m_userInfo.OpBranchNo);
    lpF2Packer->AddChar(m_userInfo.OpEntrustWay);
    lpF2Packer->AddStr(m_userInfo.OpStation);
    lpF2Packer->AddInt(m_userInfo.BranchNo);
    lpF2Packer->AddStr(m_userInfo.Password);
    lpF2Packer->AddChar(m_userInfo.PasswordType);
    lpF2Packer->AddChar(pReqFuncClientLogin->InputContent);
    lpF2Packer->AddStr(pReqFuncClientLogin->AccountContent);
    lpF2Packer->AddStr(pReqFuncClientLogin->ContentType);
    lpF2Packer->AddChar(pReqFuncClientLogin->AssetProp);
    lpF2Packer->EndPack();
    //发送请求包
    int iRet = SendBizMessage(lpF2Packer, FuncClientLogin, 0, nRequestID, ppRetData, pnItems, pRspInfo);
    //最后释放打包器
    lpF2Packer->FreeMem(lpF2Packer->GetPackBuf());
    lpF2Packer->Release();

    return iRet;
};

//330851 期货合约保证金查询请求
int CHundsunFutuMdApi_impl::ReqFuncFutuContractbailQry(CHundsunFutuReqFuncFutuContractbailQryField *pReqFuncFutuContractbailQry, int nRequestID, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo)
{
    if(m_SaveLog && lpMdApi_log != NULL)
        lpMdApi_log->AddLog(LOGINFO_Request, FuncFutuContractbailQry, 0, pReqFuncFutuContractbailQry, sizeof(CHundsunFutuReqFuncFutuContractbailQryField));

    //将请求对象保存，分页查询使用
    //异步请求才实现分页查询，同步请求上层实现
    if(strcmp(pReqFuncFutuContractbailQry->PositionStr, "") == 0 && ppRetData == NULL)
    {
        CQueryInfo *qryinfo = new CQueryInfo(FuncFutuContractbailQry, nRequestID, pReqFuncFutuContractbailQry, sizeof(CHundsunFutuReqFuncFutuContractbailQryField));
        AddCQueryInfo(qryinfo);
    }

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
    lpF2Packer->AddField("futu_exch_type",'S',4);                               //交易类别
    lpF2Packer->AddField("futu_code",'S',30);                                   //合约代码
    lpF2Packer->AddField("request_num",'I');                                    //请求行数
    lpF2Packer->AddField("position_str",'S',100);                               //定位串

    lpF2Packer->AddStr(pReqFuncFutuContractbailQry->UserToken);
    lpF2Packer->AddInt(m_userInfo.OpBranchNo);
    lpF2Packer->AddChar(m_userInfo.OpEntrustWay);
    lpF2Packer->AddStr(m_userInfo.OpStation);
    lpF2Packer->AddInt(m_userInfo.BranchNo);
    lpF2Packer->AddStr(m_userInfo.ClientId);
    lpF2Packer->AddStr(m_userInfo.FundAccount);
    lpF2Packer->AddStr(m_userInfo.Password);
    lpF2Packer->AddStr(pReqFuncFutuContractbailQry->FutuExchType);
    lpF2Packer->AddStr(pReqFuncFutuContractbailQry->FutuCode);
    lpF2Packer->AddInt(pReqFuncFutuContractbailQry->RequestNum);
    lpF2Packer->AddStr(pReqFuncFutuContractbailQry->PositionStr);
    lpF2Packer->EndPack();
    //发送请求包
    int iRet = SendBizMessage(lpF2Packer, FuncFutuContractbailQry, 0, nRequestID, ppRetData, pnItems, pRspInfo);
    //最后释放打包器
    lpF2Packer->FreeMem(lpF2Packer->GetPackBuf());
    lpF2Packer->Release();

    return iRet;
};

//620001_33100 订阅-期货单腿行情请求
int CHundsunFutuMdApi_impl::ReqFuncQhSingleHqSubscribe(char *ppInstrumentID[], int nCount, int nRequestID, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo)
{
    //创建Pack包
    IF2Packer *lpF2Packer = NewPacker(PackerVersion);
    lpF2Packer->AddRef();
    lpF2Packer->BeginPack();
    lpF2Packer->AddField("op_branch_no",'I');                                   //操作分支机构
    lpF2Packer->AddField("op_entrust_way",'C');                                 //委托方式
    lpF2Packer->AddField("op_station",'S',255);                                 //站点地址
    lpF2Packer->AddField("branch_no",'I');                                      //分支机构
    lpF2Packer->AddField("client_id",'S',18);                                   //客户编号
    lpF2Packer->AddField("fund_account",'S',18);                                //资金账户
    lpF2Packer->AddField("password",'S',10);                                    //密码
    lpF2Packer->AddField("user_token",'S',512);                                 //用户口令
    lpF2Packer->AddField("issue_type",'I');                                     //发布类型（必须为33100-期货单腿变动行情）
    lpF2Packer->AddField("futu_code",'S',30);                                   //期货代码

    for(int i = 0; i < nCount; i++)
    {
        if(isArbit(ppInstrumentID[i]))
            continue;
        if( m_SubFutuCodeSet.find(ppInstrumentID[i]) == m_SubFutuCodeSet.end())
        {
            m_SubFutuCodeSet.insert(ppInstrumentID[i]);
        }
        lpF2Packer->AddInt(m_userInfo.OpBranchNo);
        lpF2Packer->AddChar(m_userInfo.OpEntrustWay);
        lpF2Packer->AddStr(m_userInfo.OpStation);
        lpF2Packer->AddInt(m_userInfo.BranchNo);
        lpF2Packer->AddStr(m_userInfo.ClientId);
        lpF2Packer->AddStr(m_userInfo.FundAccount);
        lpF2Packer->AddStr(m_userInfo.Password);
        lpF2Packer->AddStr(m_userInfo.UserToken);
        lpF2Packer->AddInt(33100);
        lpF2Packer->AddStr(ppInstrumentID[i]);
    }
    lpF2Packer->EndPack();
    //发送请求包
    int iRet = SendBizMessage(lpF2Packer, 620001, 33100, nRequestID, ppRetData, pnItems, pRspInfo);
    //最后释放打包器
    lpF2Packer->FreeMem(lpF2Packer->GetPackBuf());
    lpF2Packer->Release();

    return iRet;
};

//620002_33100 订阅取消-期货单腿行情请求
int CHundsunFutuMdApi_impl::ReqFuncQhSingleHqCancel(char *ppInstrumentID[], int nCount, int nRequestID, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo)
{
    //创建Pack包
    IF2Packer *lpF2Packer = NewPacker(PackerVersion);
    lpF2Packer->AddRef();
    lpF2Packer->BeginPack();
    lpF2Packer->AddField("issue_type",'I');                                     //发布类型
    lpF2Packer->AddField("futu_code",'S',30);                                   //期货代码

    for(int i = 0; i < nCount; i++)
    {
        if(isArbit(ppInstrumentID[i]))
            continue;
        if( m_SubFutuCodeSet.find(ppInstrumentID[i]) != m_SubFutuCodeSet.end())
        {
            m_SubFutuCodeSet.erase(ppInstrumentID[i]);
        }
        lpF2Packer->AddInt(33100);
        lpF2Packer->AddStr(ppInstrumentID[i]);
    }
    lpF2Packer->EndPack();
    //发送请求包
    int iRet = SendBizMessage(lpF2Packer, 620002, 33100, nRequestID, ppRetData, pnItems, pRspInfo);
    //最后释放打包器
    lpF2Packer->FreeMem(lpF2Packer->GetPackBuf());
    lpF2Packer->Release();

    return iRet;
};

//620001_33102 订阅-期货组合行情请求
int CHundsunFutuMdApi_impl::ReqFuncQhCombineSubscribe(char *ppInstrumentID[], int nCount, int nRequestID, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo)
{
    //创建Pack包
    IF2Packer *lpF2Packer = NewPacker(PackerVersion);
    lpF2Packer->AddRef();
    lpF2Packer->BeginPack();
    lpF2Packer->AddField("op_branch_no",'I');                                   //操作分支机构
    lpF2Packer->AddField("op_entrust_way",'C');                                 //委托方式
    lpF2Packer->AddField("op_station",'S',255);                                 //站点地址
    lpF2Packer->AddField("branch_no",'I');                                      //分支机构
    lpF2Packer->AddField("client_id",'S',18);                                   //客户编号
    lpF2Packer->AddField("fund_account",'S',18);                                //资金账户
    lpF2Packer->AddField("password",'S',10);                                    //密码
    lpF2Packer->AddField("user_token",'S',512);                                 //用户口令
    lpF2Packer->AddField("issue_type",'I');                                     //发布类型（必须为33102-期货组合变动行情)
    lpF2Packer->AddField("arbit_code",'S',30);                                  //套利合约号

    for(int i = 0; i < nCount; i++)
    {
        if(!isArbit(ppInstrumentID[i]))
            continue;
        if( m_SubArbitCodeSet.find(ppInstrumentID[i]) == m_SubArbitCodeSet.end())
        {
            m_SubArbitCodeSet.insert(ppInstrumentID[i]);
        }
        lpF2Packer->AddInt(m_userInfo.OpBranchNo);
        lpF2Packer->AddChar(m_userInfo.OpEntrustWay);
        lpF2Packer->AddStr(m_userInfo.OpStation);
        lpF2Packer->AddInt(m_userInfo.BranchNo);
        lpF2Packer->AddStr(m_userInfo.ClientId);
        lpF2Packer->AddStr(m_userInfo.FundAccount);
        lpF2Packer->AddStr(m_userInfo.Password);
        lpF2Packer->AddStr(m_userInfo.UserToken);
        lpF2Packer->AddInt(33102);
        lpF2Packer->AddStr(ppInstrumentID[i]);
    }
    lpF2Packer->EndPack();
    //发送请求包
    int iRet = SendBizMessage(lpF2Packer, 620001, 33102, nRequestID, ppRetData, pnItems, pRspInfo);
    //最后释放打包器
    lpF2Packer->FreeMem(lpF2Packer->GetPackBuf());
    lpF2Packer->Release();

    return iRet;
};

//620002_33102 订阅取消-期货组合行情请求
int CHundsunFutuMdApi_impl::ReqFuncQhCombineCancel(char *ppInstrumentID[], int nCount, int nRequestID, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo)
{
    //创建Pack包
    IF2Packer *lpF2Packer = NewPacker(PackerVersion);
    lpF2Packer->AddRef();
    lpF2Packer->BeginPack();
    lpF2Packer->AddField("issue_type",'I');                                     //发布类型
    lpF2Packer->AddField("arbit_code",'S',30);                                  //套利合约号

    for(int i = 0; i < nCount; i++)
    {
        if(!isArbit(ppInstrumentID[i]))
            continue;
        if( m_SubArbitCodeSet.find(ppInstrumentID[i]) != m_SubArbitCodeSet.end())
        {
            m_SubArbitCodeSet.erase(ppInstrumentID[i]);
        }
        lpF2Packer->AddInt(33102);
        lpF2Packer->AddStr(ppInstrumentID[i]);
    }
    lpF2Packer->EndPack();
    //发送请求包
    int iRet = SendBizMessage(lpF2Packer, 620002, 33102, nRequestID, ppRetData, pnItems, pRspInfo);
    //最后释放打包器
    lpF2Packer->FreeMem(lpF2Packer->GetPackBuf());
    lpF2Packer->Release();

    return iRet;
};

void CHundsunFutuMdApi_impl::OnConnected()
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

    if(lpMdSpi)
    {
        lpMdSpi->OnFrontConnected();
    }

    //重连之后自动订阅
    if(true)//m_AutoReConnect
    {
        m_SubSetCs.Lock();
        if(m_SubFutuCodeSet.size() > 0)
        {
            char** pptr = new char*[m_SubFutuCodeSet.size() + 1]; 
            int    i = 0;
            for(set<string>::iterator it = m_SubFutuCodeSet.begin(); it != m_SubFutuCodeSet.end(); it++, i++)
            {
                pptr[i] = (char*)(*it).c_str(); 
            }
            pptr[i] = 0;
            ReqFuncQhSingleHqSubscribe(pptr, m_SubFutuCodeSet.size(), -33100);
        }
        if(m_SubArbitCodeSet.size() > 0)
        {
            char** pptr = new char*[m_SubArbitCodeSet.size() + 1]; 
            int    i = 0;
            for(set<string>::iterator it = m_SubArbitCodeSet.begin(); it != m_SubArbitCodeSet.end(); it++, i++)
            {
                pptr[i] = (char*)(*it).c_str(); 
            }
            pptr[i] = 0;
            ReqFuncQhCombineSubscribe(pptr, m_SubArbitCodeSet.size(), -33102);
        }
        m_SubSetCs.UnLock();
    }
}

void CHundsunFutuMdApi_impl::OnClosed()
{
    if(lpMdSpi)
    {
        lpMdSpi->OnFrontDisconnected(0x1001);
    }
}

void CHundsunFutuMdApi_impl::DealAsyBizMessage(IBizMessage* lpMsg)
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
                    if(lpMsg->GetIssueType() == 33100)
                        OnAsyRtn_620003_33100(lpUnPacker, lpMsg->GetSenderId());
                    if(lpMsg->GetIssueType() == 33102)
                        OnAsyRtn_620003_33102(lpUnPacker, lpMsg->GetSenderId());
                }
                break;
            case 620001:
                {
                    if(lpMsg->GetIssueType() == 33100)
                        OnAsyRsp_620001_33100(lpUnPacker_key, lpMsg->GetSenderId());
                    if(lpMsg->GetIssueType() == 33102)
                        OnAsyRsp_620001_33102(lpUnPacker_key, lpMsg->GetSenderId());
                }
                break;
            case 620002:
                {
                    if(lpMsg->GetIssueType() == 33100)
                        OnAsyRsp_620002_33100(lpUnPacker_key, lpMsg->GetSenderId());
                    if(lpMsg->GetIssueType() == 33102)
                        OnAsyRsp_620002_33102(lpUnPacker_key, lpMsg->GetSenderId());
                }
                break;
            case FuncClientLogin:
                OnAsyResponse_331100(lpUnPacker, lpMsg->GetSenderId());
                break;
            case FuncFutuContractbailQry:
                OnAsyResponse_330851(lpUnPacker, lpMsg->GetSenderId());
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

int CHundsunFutuMdApi_impl::DealSynBizMessage(const void* lpBuffer, int nLen, void** ppRetData, int* pnItems, void *pRspInfo)
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
        case 620001:
            {
                if(lpMsg->GetIssueType() == 33100)
                    OnSynRsp_620001_33100(lpUnPacker_key, ppRetData, pnItems, (CHundsunFutuRspInfoField *)pRspInfo);
                if(lpMsg->GetIssueType() == 33102)
                    OnSynRsp_620001_33102(lpUnPacker_key, ppRetData, pnItems, (CHundsunFutuRspInfoField *)pRspInfo);
            }
            break;
        case 620002:
            {
                if(lpMsg->GetIssueType() == 33100)
                    OnSynRsp_620002_33100(lpUnPacker_key, ppRetData, pnItems, (CHundsunFutuRspInfoField *)pRspInfo);
                if(lpMsg->GetIssueType() == 33102)
                    OnSynRsp_620002_33102(lpUnPacker_key, ppRetData, pnItems, (CHundsunFutuRspInfoField *)pRspInfo);
            }
            break;
        case FuncClientLogin:
            OnSynResponse_331100(lpUnPacker, ppRetData, pnItems, (CHundsunFutuRspInfoField *)pRspInfo);
            break;
        case FuncFutuContractbailQry:
            OnSynResponse_330851(lpUnPacker, ppRetData, pnItems, (CHundsunFutuRspInfoField *)pRspInfo);
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

void CHundsunFutuMdApi_impl::RecHeartbeet()
{
    m_iHeartKeep = 0;
}

void CHundsunFutuMdApi_impl::GetMsgErrorInfo(int nErrorCode, void *pRspInfo)
{
    if(pRspInfo != NULL)
    {
        ((CHundsunFutuRspInfoField *)pRspInfo)->ErrorNo = nErrorCode;
        hs_strncpy(((CHundsunFutuRspInfoField *)pRspInfo)->ErrorInfo, lpConnection->GetErrorMsg(nErrorCode), sizeof(((CHundsunFutuRspInfoField *)pRspInfo)->ErrorInfo));
    }
}

//331100 客户登录
void CHundsunFutuMdApi_impl::OnAsyResponse_331100(IF2UnPacker *lpUnPacker, int nRequestID)
{
    //判断错误信息
    CHundsunFutuRspInfoField RspInfo;
    //返回信息结构体
    CHundsunFutuRspFuncClientLoginField RspFuncClientLogin;
    memset(&RspFuncClientLogin, 0, sizeof(CHundsunFutuRspFuncClientLoginField));

    //获得错误信息
    DealErrorInfo(lpUnPacker, &RspInfo);
    if( RspInfo.ErrorNo != 0 )
    {
        if(m_SaveLog && lpMdApi_log != NULL)
            lpMdApi_log->AddLog(LOGINFO_Response, FuncClientLogin, 0, &RspFuncClientLogin, sizeof(CHundsunFutuRspFuncClientLoginField), &RspInfo, sizeof(CHundsunFutuRspInfoField));
        lpMdSpi->OnRspFuncClientLogin(&RspFuncClientLogin, &RspInfo, nRequestID, true);
    }
    else
    {
        //交易日期
        RspFuncClientLogin.InitDate = lpUnPacker->GetInt("init_date");
        //系统状态
        RspFuncClientLogin.SysStatus = lpUnPacker->GetChar("sys_status");
        //公司名称
        hs_strncpy(RspFuncClientLogin.CompanyName, lpUnPacker->GetStr("company_name"), sizeof(RspFuncClientLogin.CompanyName));
        //银行号、市场类别
        hs_strncpy(RspFuncClientLogin.ContentType, lpUnPacker->GetStr("content_type"), sizeof(RspFuncClientLogin.ContentType));
        //输入内容
        hs_strncpy(RspFuncClientLogin.AccountContent, lpUnPacker->GetStr("account_content"), sizeof(RspFuncClientLogin.AccountContent));
        //分支机构
        RspFuncClientLogin.BranchNo = lpUnPacker->GetInt("branch_no");
        //客户编号
        hs_strncpy(RspFuncClientLogin.ClientId, lpUnPacker->GetStr("client_id"), sizeof(RspFuncClientLogin.ClientId));
        //客户姓名
        hs_strncpy(RspFuncClientLogin.ClientName, lpUnPacker->GetStr("client_name"), sizeof(RspFuncClientLogin.ClientName));
        //公司客户类型
        RspFuncClientLogin.CorpClientGroup = lpUnPacker->GetInt("corp_client_group");
        //客户风险等级
        RspFuncClientLogin.CorpRiskLevel = lpUnPacker->GetInt("corp_risk_level");
        //客户风险测评日
        RspFuncClientLogin.CorpBeginDate = lpUnPacker->GetInt("corp_begin_date");
        //客户风险到期日
        RspFuncClientLogin.CorpEndDate = lpUnPacker->GetInt("corp_end_date");
        //有效标志
        RspFuncClientLogin.ValidFlag = lpUnPacker->GetChar("valid_flag");
        //资产账号个数
        RspFuncClientLogin.FundaccountCount = lpUnPacker->GetInt("fundaccount_count");
        //资产账户
        hs_strncpy(RspFuncClientLogin.FundAccount, lpUnPacker->GetStr("fund_account"), sizeof(RspFuncClientLogin.FundAccount));
        //客户权限
        hs_strncpy(RspFuncClientLogin.ClientRights, lpUnPacker->GetStr("client_rights"), sizeof(RspFuncClientLogin.ClientRights));
        //上次登陆委托方式
        RspFuncClientLogin.LastOpEntrustWay = lpUnPacker->GetChar("last_op_entrust_way");
        //上次登陆站点/电话
        hs_strncpy(RspFuncClientLogin.LastOpStation, lpUnPacker->GetStr("last_op_station"), sizeof(RspFuncClientLogin.LastOpStation));
        //系统节点编号
        RspFuncClientLogin.SysnodeId = lpUnPacker->GetInt("sysnode_id");
        //用户口令
        hs_strncpy(RspFuncClientLogin.UserToken, lpUnPacker->GetStr("user_token"), sizeof(RspFuncClientLogin.UserToken));
        //错误代码
        RspFuncClientLogin.ErrorNo = lpUnPacker->GetInt("error_no");
        //错误提示
        hs_strncpy(RspFuncClientLogin.ErrorInfo, lpUnPacker->GetStr("error_info"), sizeof(RspFuncClientLogin.ErrorInfo));
        //资产属性
        RspFuncClientLogin.AssetProp = lpUnPacker->GetChar("asset_prop");
        //产品标志
        RspFuncClientLogin.ProductFlag = lpUnPacker->GetChar("product_flag");
        //消息标志
        RspFuncClientLogin.MessageFlag = lpUnPacker->GetChar("message_flag");
        //强制账单确认
        RspFuncClientLogin.TabconfirmFlag = lpUnPacker->GetChar("tabconfirm_flag");
        //上次提成日期
        RspFuncClientLogin.LastDate = lpUnPacker->GetInt("last_date");
        //会话编号
        RspFuncClientLogin.SessionNo = lpUnPacker->GetInt("session_no");

        m_userInfo.BranchNo = lpUnPacker->GetInt("branch_no"); //设置营业部号
        hs_strncpy(m_userInfo.ClientId, lpUnPacker->GetStr("client_id"), sizeof(m_userInfo.ClientId)); //设置客户编号
        hs_strncpy(m_userInfo.FundAccount, lpUnPacker->GetStr("fund_account"), sizeof(m_userInfo.FundAccount)); //设置资金账号
        hs_strncpy(m_userInfo.UserToken, lpUnPacker->GetStr("user_token"), sizeof(m_userInfo.UserToken)); //设置用户口令
        m_Logined = true; //设置是否登录

        //启动心跳维护
        if(!m_HeartKeepTrd.IsRunning())
            m_HeartKeepTrd.Start(HeartKeep, 0, this);

        //日志落地
        if(m_SaveLog && lpMdApi_log != NULL)
            lpMdApi_log->AddLog(LOGINFO_Response, FuncClientLogin, 0, &RspFuncClientLogin, sizeof(CHundsunFutuRspFuncClientLoginField), &RspInfo, sizeof(CHundsunFutuRspInfoField));
        //回调
        lpMdSpi->OnRspFuncClientLogin(&RspFuncClientLogin, &RspInfo, nRequestID, true);
    }
}

//330851 期货合约保证金查询
void CHundsunFutuMdApi_impl::OnAsyResponse_330851(IF2UnPacker *lpUnPacker, int nRequestID)
{
    //判断错误信息
    CHundsunFutuRspInfoField RspInfo;
    //返回信息结构体
    CHundsunFutuRspFuncFutuContractbailQryField RspFuncFutuContractbailQry;
    memset(&RspFuncFutuContractbailQry, 0, sizeof(CHundsunFutuRspFuncFutuContractbailQryField));

    //获得错误信息
    DealErrorInfo(lpUnPacker, &RspInfo);
    if( RspInfo.ErrorNo != 0 )
    {
        if(m_SaveLog && lpMdApi_log != NULL)
            lpMdApi_log->AddLog(LOGINFO_Response, FuncFutuContractbailQry, 0, &RspFuncFutuContractbailQry, sizeof(CHundsunFutuRspFuncFutuContractbailQryField), &RspInfo, sizeof(CHundsunFutuRspInfoField));
        lpMdSpi->OnRspFuncFutuContractbailQry(&RspFuncFutuContractbailQry, &RspInfo, nRequestID, true);
    }
    else
    {
        //获得请求参数
        CQueryInfo *qryinfo = GetCQueryInfo(nRequestID);
        //获得总行数
        int ct = lpUnPacker->GetRowCount();
        if( ct == 0 )
        {
            lpMdSpi->OnRspFuncFutuContractbailQry(NULL, &RspInfo, nRequestID, true);
            if(qryinfo)
                DelQuery(qryinfo);
        }
        else
        {
            string pos = "";
            while (!lpUnPacker->IsEOF())
            {
                ct--;

                //交易类别
                hs_strncpy(RspFuncFutuContractbailQry.FutuExchType, lpUnPacker->GetStr("futu_exch_type"), sizeof(RspFuncFutuContractbailQry.FutuExchType));
                //合约代码
                hs_strncpy(RspFuncFutuContractbailQry.FutuCode, lpUnPacker->GetStr("futu_code"), sizeof(RspFuncFutuContractbailQry.FutuCode));
                //品种简称
                hs_strncpy(RspFuncFutuContractbailQry.FutuName, lpUnPacker->GetStr("futu_name"), sizeof(RspFuncFutuContractbailQry.FutuName));
                //合约代码类别
                hs_strncpy(RspFuncFutuContractbailQry.FutucodeType, lpUnPacker->GetStr("futucode_type"), sizeof(RspFuncFutuContractbailQry.FutucodeType));
                //币种类别
                hs_strncpy(RspFuncFutuContractbailQry.MoneyType, lpUnPacker->GetStr("money_type"), sizeof(RspFuncFutuContractbailQry.MoneyType));
                //停止交易标志
                RspFuncFutuContractbailQry.StopFlag = lpUnPacker->GetChar("stop_flag");
                //合约乘数
                RspFuncFutuContractbailQry.AmountPerHand = lpUnPacker->GetInt("amount_per_hand");
                //申报单位
                RspFuncFutuContractbailQry.FutuReportUnit = lpUnPacker->GetChar("futu_report_unit");
                //价格单位
                RspFuncFutuContractbailQry.PriceUnit = lpUnPacker->GetChar("price_unit");
                //最小价差
                RspFuncFutuContractbailQry.FutuPriceStep = lpUnPacker->GetDouble("futu_price_step");
                //开仓单位
                RspFuncFutuContractbailQry.OpenUnit = lpUnPacker->GetDouble("open_unit");
                //平仓单位
                RspFuncFutuContractbailQry.DropUnit = lpUnPacker->GetDouble("drop_unit");
                //市价委托每笔最大下单数量
                RspFuncFutuContractbailQry.MaxMarketAmount = lpUnPacker->GetInt("max_market_amount");
                //限价委托每笔最大下单数量
                RspFuncFutuContractbailQry.MaxLimitedAmount = lpUnPacker->GetInt("max_limited_amount");
                //开仓成交金额比例
                RspFuncFutuContractbailQry.OpenRatio = lpUnPacker->GetDouble("open_ratio");
                //开仓单位数量保证金金额
                RspFuncFutuContractbailQry.OpenMargin = lpUnPacker->GetDouble("open_margin");
                //期权系列
                hs_strncpy(RspFuncFutuContractbailQry.FoptExpiry, lpUnPacker->GetStr("fopt_expiry"), sizeof(RspFuncFutuContractbailQry.FoptExpiry));
                //期权执行价格
                RspFuncFutuContractbailQry.FoptStrikePrice = lpUnPacker->GetDouble("fopt_strike_price");
                //昨虚实度
                RspFuncFutuContractbailQry.PreDelta = lpUnPacker->GetDouble("pre_delta");
                //产品类别
                RspFuncFutuContractbailQry.FutuProductType = lpUnPacker->GetChar("futu_product_type");
                //期权合约类型
                RspFuncFutuContractbailQry.FoptType = lpUnPacker->GetChar("fopt_type");
                //标的合约代码
                hs_strncpy(RspFuncFutuContractbailQry.UnderlyingCode, lpUnPacker->GetStr("underlying_code"), sizeof(RspFuncFutuContractbailQry.UnderlyingCode));
                //到期日
                RspFuncFutuContractbailQry.ExpireDate = lpUnPacker->GetInt("expire_date");
                //执行日期(上海期权采用百慕大执行方式,由交易所指定T日)
                RspFuncFutuContractbailQry.StrikeDate = lpUnPacker->GetInt("strike_date");
                //是否单向大边优惠
                RspFuncFutuContractbailQry.HighSidePreferFlag = lpUnPacker->GetChar("high_side_prefer_flag");
                //昨虚实值
                RspFuncFutuContractbailQry.PreDeltaValue = lpUnPacker->GetDouble("pre_delta_value");
                //定位串
                hs_strncpy(RspFuncFutuContractbailQry.PositionStr, lpUnPacker->GetStr("position_str"), sizeof(RspFuncFutuContractbailQry.PositionStr));

                //获得定位字符串
                const char *lpStrPos = lpUnPacker->GetStr("position_str");
                if( lpStrPos == 0)
                    pos = "";
                else
                    pos = lpStrPos;
                //记录日志
                if(m_SaveLog && lpMdApi_log != NULL)
                    lpMdApi_log->AddLog(LOGINFO_Response, FuncFutuContractbailQry, 0, &RspFuncFutuContractbailQry, sizeof(CHundsunFutuRspFuncFutuContractbailQryField), &RspInfo, sizeof(CHundsunFutuRspInfoField));
                //回调
                if(qryinfo)
                    lpMdSpi->OnRspFuncFutuContractbailQry(&RspFuncFutuContractbailQry, &RspInfo, nRequestID, false);
                else
                    lpMdSpi->OnRspFuncFutuContractbailQry(&RspFuncFutuContractbailQry, &RspInfo, nRequestID, ct == 0 ? true : false);

                lpUnPacker->Next();
            }
            if(qryinfo)
            {
                if( pos.length() == 0)
                {
                    lpMdSpi->OnRspFuncFutuContractbailQry(NULL, &RspInfo, nRequestID, true);
                    DelQuery(qryinfo);
                }
                else
                {
                    //查询下一页
                    CHundsunFutuReqFuncFutuContractbailQryField *pReqFuncFutuContractbailQry = (CHundsunFutuReqFuncFutuContractbailQryField *)qryinfo->GetReqData();
                    hs_strncpy(pReqFuncFutuContractbailQry->PositionStr, pos.c_str(), sizeof(pReqFuncFutuContractbailQry->PositionStr));
                    ReqFuncFutuContractbailQry(pReqFuncFutuContractbailQry, nRequestID);
                }
            }
        }
    }
}

//620001_33100 订阅-期货单腿行情应答
void CHundsunFutuMdApi_impl::OnAsyRsp_620001_33100(IF2UnPacker *lpUnPacker, int nRequestID)
{
    //判断错误信息
    CHundsunFutuRspInfoField RspInfo;
    //获得错误信息
    DealErrorInfo(lpUnPacker, &RspInfo);
    //回调应答
    if(nRequestID != -33100)
        lpMdSpi->OnRspFuncQhSingleHqSubscribe(&RspInfo, nRequestID, true);
}

//620002_33100 订阅取消-期货单腿行情应答
void CHundsunFutuMdApi_impl::OnAsyRsp_620002_33100(IF2UnPacker *lpUnPacker, int nRequestID)
{
    //判断错误信息
    CHundsunFutuRspInfoField RspInfo;
    //获得错误信息
    DealErrorInfo(lpUnPacker, &RspInfo);
    //回调应答
    if(nRequestID != -33100)
        lpMdSpi->OnRspFuncQhSingleHqCancel(&RspInfo, nRequestID, true);
}

//620003_33100 主推-期货单腿行情
void CHundsunFutuMdApi_impl::OnAsyRtn_620003_33100(IF2UnPacker *lpUnPacker, int nRequestID)
{
    char buf[1024];
    hs_strncpy(buf, lpUnPacker->GetStr("QH"), strlen(lpUnPacker->GetStr("QH")));
    //返回信息结构体
    CHundsunFutuRtnFuncQhSingleHqPushField RtnFuncQhSingleHqPush;
    memset(&RtnFuncQhSingleHqPush, 0, sizeof(CHundsunFutuRtnFuncQhSingleHqPushField));
    //解析主推信息
    ParseFuncQhSingleHqPush(&RtnFuncQhSingleHqPush, buf);
    //日志落地
    if(m_SaveLog && lpMdApi_log != NULL)
        lpMdApi_log->AddLog(LOGINFO_Return, 620003, 33100, &RtnFuncQhSingleHqPush, sizeof(CHundsunFutuRtnFuncQhSingleHqPushField));
    //回调主推
    lpMdSpi->OnRtnFuncQhSingleHqPush(&RtnFuncQhSingleHqPush);
}

//620001_33102 订阅-期货组合行情应答
void CHundsunFutuMdApi_impl::OnAsyRsp_620001_33102(IF2UnPacker *lpUnPacker, int nRequestID)
{
    //判断错误信息
    CHundsunFutuRspInfoField RspInfo;
    //获得错误信息
    DealErrorInfo(lpUnPacker, &RspInfo);
    //回调应答
    if(nRequestID != -33102)
        lpMdSpi->OnRspFuncQhCombineSubscribe(&RspInfo, nRequestID, true);
}

//620002_33102 订阅取消-期货组合行情应答
void CHundsunFutuMdApi_impl::OnAsyRsp_620002_33102(IF2UnPacker *lpUnPacker, int nRequestID)
{
    //判断错误信息
    CHundsunFutuRspInfoField RspInfo;
    //获得错误信息
    DealErrorInfo(lpUnPacker, &RspInfo);
    //回调应答
    if(nRequestID != -33102)
        lpMdSpi->OnRspFuncQhCombineCancel(&RspInfo, nRequestID, true);
}

//620003_33102 主推-期货组合行情
void CHundsunFutuMdApi_impl::OnAsyRtn_620003_33102(IF2UnPacker *lpUnPacker, int nRequestID)
{
    char buf[1024];
    hs_strncpy(buf, lpUnPacker->GetStr("QH"), strlen(lpUnPacker->GetStr("QH")));
    //返回信息结构体
    CHundsunFutuRtnFuncQhCombinePushField RtnFuncQhCombinePush;
    memset(&RtnFuncQhCombinePush, 0, sizeof(CHundsunFutuRtnFuncQhCombinePushField));
    //解析主推信息
    ParseFuncQhCombinePush(&RtnFuncQhCombinePush, buf);
    //日志落地
    if(m_SaveLog && lpMdApi_log != NULL)
        lpMdApi_log->AddLog(LOGINFO_Return, 620003, 33102, &RtnFuncQhCombinePush, sizeof(CHundsunFutuRtnFuncQhCombinePushField));
    //回调主推
    lpMdSpi->OnRtnFuncQhCombinePush(&RtnFuncQhCombinePush);
}

//331100 客户登录
void CHundsunFutuMdApi_impl::OnSynResponse_331100(IF2UnPacker *lpUnPacker, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo)
{
    //获得错误信息
    DealErrorInfo(lpUnPacker, pRspInfo);
    if( pRspInfo->ErrorNo != 0 )
        return;

    *pnItems = lpUnPacker->GetRowCount();
    if(*pnItems > 0)
    {
        *ppRetData = new CHundsunFutuRspFuncClientLoginField[*pnItems];
        CHundsunFutuRspFuncClientLoginField* ppRspFuncClientLogin = (CHundsunFutuRspFuncClientLoginField*) (*ppRetData);

        int ct = 0;
        while (!lpUnPacker->IsEOF())
        {
            //交易日期
            ppRspFuncClientLogin[ct].InitDate = lpUnPacker->GetInt("init_date");
            //系统状态
            ppRspFuncClientLogin[ct].SysStatus = lpUnPacker->GetChar("sys_status");
            //公司名称
            hs_strncpy(ppRspFuncClientLogin[ct].CompanyName, lpUnPacker->GetStr("company_name"), sizeof(ppRspFuncClientLogin[ct].CompanyName));
            //银行号、市场类别
            hs_strncpy(ppRspFuncClientLogin[ct].ContentType, lpUnPacker->GetStr("content_type"), sizeof(ppRspFuncClientLogin[ct].ContentType));
            //输入内容
            hs_strncpy(ppRspFuncClientLogin[ct].AccountContent, lpUnPacker->GetStr("account_content"), sizeof(ppRspFuncClientLogin[ct].AccountContent));
            //分支机构
            ppRspFuncClientLogin[ct].BranchNo = lpUnPacker->GetInt("branch_no");
            //客户编号
            hs_strncpy(ppRspFuncClientLogin[ct].ClientId, lpUnPacker->GetStr("client_id"), sizeof(ppRspFuncClientLogin[ct].ClientId));
            //客户姓名
            hs_strncpy(ppRspFuncClientLogin[ct].ClientName, lpUnPacker->GetStr("client_name"), sizeof(ppRspFuncClientLogin[ct].ClientName));
            //公司客户类型
            ppRspFuncClientLogin[ct].CorpClientGroup = lpUnPacker->GetInt("corp_client_group");
            //客户风险等级
            ppRspFuncClientLogin[ct].CorpRiskLevel = lpUnPacker->GetInt("corp_risk_level");
            //客户风险测评日
            ppRspFuncClientLogin[ct].CorpBeginDate = lpUnPacker->GetInt("corp_begin_date");
            //客户风险到期日
            ppRspFuncClientLogin[ct].CorpEndDate = lpUnPacker->GetInt("corp_end_date");
            //有效标志
            ppRspFuncClientLogin[ct].ValidFlag = lpUnPacker->GetChar("valid_flag");
            //资产账号个数
            ppRspFuncClientLogin[ct].FundaccountCount = lpUnPacker->GetInt("fundaccount_count");
            //资产账户
            hs_strncpy(ppRspFuncClientLogin[ct].FundAccount, lpUnPacker->GetStr("fund_account"), sizeof(ppRspFuncClientLogin[ct].FundAccount));
            //客户权限
            hs_strncpy(ppRspFuncClientLogin[ct].ClientRights, lpUnPacker->GetStr("client_rights"), sizeof(ppRspFuncClientLogin[ct].ClientRights));
            //上次登陆委托方式
            ppRspFuncClientLogin[ct].LastOpEntrustWay = lpUnPacker->GetChar("last_op_entrust_way");
            //上次登陆站点/电话
            hs_strncpy(ppRspFuncClientLogin[ct].LastOpStation, lpUnPacker->GetStr("last_op_station"), sizeof(ppRspFuncClientLogin[ct].LastOpStation));
            //系统节点编号
            ppRspFuncClientLogin[ct].SysnodeId = lpUnPacker->GetInt("sysnode_id");
            //用户口令
            hs_strncpy(ppRspFuncClientLogin[ct].UserToken, lpUnPacker->GetStr("user_token"), sizeof(ppRspFuncClientLogin[ct].UserToken));
            //错误代码
            ppRspFuncClientLogin[ct].ErrorNo = lpUnPacker->GetInt("error_no");
            //错误提示
            hs_strncpy(ppRspFuncClientLogin[ct].ErrorInfo, lpUnPacker->GetStr("error_info"), sizeof(ppRspFuncClientLogin[ct].ErrorInfo));
            //资产属性
            ppRspFuncClientLogin[ct].AssetProp = lpUnPacker->GetChar("asset_prop");
            //产品标志
            ppRspFuncClientLogin[ct].ProductFlag = lpUnPacker->GetChar("product_flag");
            //消息标志
            ppRspFuncClientLogin[ct].MessageFlag = lpUnPacker->GetChar("message_flag");
            //强制账单确认
            ppRspFuncClientLogin[ct].TabconfirmFlag = lpUnPacker->GetChar("tabconfirm_flag");
            //上次提成日期
            ppRspFuncClientLogin[ct].LastDate = lpUnPacker->GetInt("last_date");
            //会话编号
            ppRspFuncClientLogin[ct].SessionNo = lpUnPacker->GetInt("session_no");

            m_userInfo.BranchNo = lpUnPacker->GetInt("branch_no"); //设置营业部号
            hs_strncpy(m_userInfo.ClientId, lpUnPacker->GetStr("client_id"), sizeof(m_userInfo.ClientId)); //设置客户编号
            hs_strncpy(m_userInfo.FundAccount, lpUnPacker->GetStr("fund_account"), sizeof(m_userInfo.FundAccount)); //设置资金账号
            hs_strncpy(m_userInfo.UserToken, lpUnPacker->GetStr("user_token"), sizeof(m_userInfo.UserToken)); //设置用户口令
            m_Logined = true; //设置是否登录

            //启动心跳维护
            if(!m_HeartKeepTrd.IsRunning())
                m_HeartKeepTrd.Start(HeartKeep, 0, this);

            ct++;
            lpUnPacker->Next();
        }
    }
    return;
}

//330851 期货合约保证金查询
void CHundsunFutuMdApi_impl::OnSynResponse_330851(IF2UnPacker *lpUnPacker, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo)
{
    //获得错误信息
    DealErrorInfo(lpUnPacker, pRspInfo);
    if( pRspInfo->ErrorNo != 0 )
        return;

    *pnItems = lpUnPacker->GetRowCount();
    if(*pnItems > 0)
    {
        *ppRetData = new CHundsunFutuRspFuncFutuContractbailQryField[*pnItems];
        CHundsunFutuRspFuncFutuContractbailQryField* ppRspFuncFutuContractbailQry = (CHundsunFutuRspFuncFutuContractbailQryField*) (*ppRetData);

        int ct = 0;
        while (!lpUnPacker->IsEOF())
        {
            //交易类别
            hs_strncpy(ppRspFuncFutuContractbailQry[ct].FutuExchType, lpUnPacker->GetStr("futu_exch_type"), sizeof(ppRspFuncFutuContractbailQry[ct].FutuExchType));
            //合约代码
            hs_strncpy(ppRspFuncFutuContractbailQry[ct].FutuCode, lpUnPacker->GetStr("futu_code"), sizeof(ppRspFuncFutuContractbailQry[ct].FutuCode));
            //品种简称
            hs_strncpy(ppRspFuncFutuContractbailQry[ct].FutuName, lpUnPacker->GetStr("futu_name"), sizeof(ppRspFuncFutuContractbailQry[ct].FutuName));
            //合约代码类别
            hs_strncpy(ppRspFuncFutuContractbailQry[ct].FutucodeType, lpUnPacker->GetStr("futucode_type"), sizeof(ppRspFuncFutuContractbailQry[ct].FutucodeType));
            //币种类别
            hs_strncpy(ppRspFuncFutuContractbailQry[ct].MoneyType, lpUnPacker->GetStr("money_type"), sizeof(ppRspFuncFutuContractbailQry[ct].MoneyType));
            //停止交易标志
            ppRspFuncFutuContractbailQry[ct].StopFlag = lpUnPacker->GetChar("stop_flag");
            //合约乘数
            ppRspFuncFutuContractbailQry[ct].AmountPerHand = lpUnPacker->GetInt("amount_per_hand");
            //申报单位
            ppRspFuncFutuContractbailQry[ct].FutuReportUnit = lpUnPacker->GetChar("futu_report_unit");
            //价格单位
            ppRspFuncFutuContractbailQry[ct].PriceUnit = lpUnPacker->GetChar("price_unit");
            //最小价差
            ppRspFuncFutuContractbailQry[ct].FutuPriceStep = lpUnPacker->GetDouble("futu_price_step");
            //开仓单位
            ppRspFuncFutuContractbailQry[ct].OpenUnit = lpUnPacker->GetDouble("open_unit");
            //平仓单位
            ppRspFuncFutuContractbailQry[ct].DropUnit = lpUnPacker->GetDouble("drop_unit");
            //市价委托每笔最大下单数量
            ppRspFuncFutuContractbailQry[ct].MaxMarketAmount = lpUnPacker->GetInt("max_market_amount");
            //限价委托每笔最大下单数量
            ppRspFuncFutuContractbailQry[ct].MaxLimitedAmount = lpUnPacker->GetInt("max_limited_amount");
            //开仓成交金额比例
            ppRspFuncFutuContractbailQry[ct].OpenRatio = lpUnPacker->GetDouble("open_ratio");
            //开仓单位数量保证金金额
            ppRspFuncFutuContractbailQry[ct].OpenMargin = lpUnPacker->GetDouble("open_margin");
            //期权系列
            hs_strncpy(ppRspFuncFutuContractbailQry[ct].FoptExpiry, lpUnPacker->GetStr("fopt_expiry"), sizeof(ppRspFuncFutuContractbailQry[ct].FoptExpiry));
            //期权执行价格
            ppRspFuncFutuContractbailQry[ct].FoptStrikePrice = lpUnPacker->GetDouble("fopt_strike_price");
            //昨虚实度
            ppRspFuncFutuContractbailQry[ct].PreDelta = lpUnPacker->GetDouble("pre_delta");
            //产品类别
            ppRspFuncFutuContractbailQry[ct].FutuProductType = lpUnPacker->GetChar("futu_product_type");
            //期权合约类型
            ppRspFuncFutuContractbailQry[ct].FoptType = lpUnPacker->GetChar("fopt_type");
            //标的合约代码
            hs_strncpy(ppRspFuncFutuContractbailQry[ct].UnderlyingCode, lpUnPacker->GetStr("underlying_code"), sizeof(ppRspFuncFutuContractbailQry[ct].UnderlyingCode));
            //到期日
            ppRspFuncFutuContractbailQry[ct].ExpireDate = lpUnPacker->GetInt("expire_date");
            //执行日期(上海期权采用百慕大执行方式,由交易所指定T日)
            ppRspFuncFutuContractbailQry[ct].StrikeDate = lpUnPacker->GetInt("strike_date");
            //是否单向大边优惠
            ppRspFuncFutuContractbailQry[ct].HighSidePreferFlag = lpUnPacker->GetChar("high_side_prefer_flag");
            //昨虚实值
            ppRspFuncFutuContractbailQry[ct].PreDeltaValue = lpUnPacker->GetDouble("pre_delta_value");
            //定位串
            hs_strncpy(ppRspFuncFutuContractbailQry[ct].PositionStr, lpUnPacker->GetStr("position_str"), sizeof(ppRspFuncFutuContractbailQry[ct].PositionStr));

            ct++;
            lpUnPacker->Next();
        }
    }
    return;
}

//620001_33100 订阅-期货单腿行情应答
void CHundsunFutuMdApi_impl::OnSynRsp_620001_33100(IF2UnPacker *lpUnPacker, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo)
{
    //获得错误信息
    DealErrorInfo(lpUnPacker, pRspInfo);
}

//620002_33100 订阅取消-期货单腿行情应答
void CHundsunFutuMdApi_impl::OnSynRsp_620002_33100(IF2UnPacker *lpUnPacker, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo)
{
    //获得错误信息
    DealErrorInfo(lpUnPacker, pRspInfo);
}

//620001_33102 订阅-期货组合行情应答
void CHundsunFutuMdApi_impl::OnSynRsp_620001_33102(IF2UnPacker *lpUnPacker, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo)
{
    //获得错误信息
    DealErrorInfo(lpUnPacker, pRspInfo);
}

//620002_33102 订阅取消-期货组合行情应答
void CHundsunFutuMdApi_impl::OnSynRsp_620002_33102(IF2UnPacker *lpUnPacker, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo)
{
    //获得错误信息
    DealErrorInfo(lpUnPacker, pRspInfo);
}

void CHundsunFutuMdApi_impl::HeartKeep(void* lpvoid)
{
    CHundsunFutuMdApi_impl *helper = (CHundsunFutuMdApi_impl*)lpvoid;
    while( helper->m_bHeartKeepStop == false )
    {
        helper->m_HeartKeepEvent.Wait(PANICHEARTBEATTIME);
        helper->m_iHeartKeep++;
        if (helper->m_iHeartKeep >= 3)
        {
            if(helper->m_Logined)
            {
                //默认订阅主推信息
                helper->m_SubSetCs.Lock();
                if(helper->m_SubFutuCodeSet.size() > 0)
                {
                    char** pptr = new char*[helper->m_SubFutuCodeSet.size() + 1]; 
                    int    i = 0;
                    for(set<string>::iterator it = helper->m_SubFutuCodeSet.begin(); it != helper->m_SubFutuCodeSet.end(); it++, i++)
                    {
                        pptr[i] = (char*)(*it).c_str(); 
                    }
                    pptr[i] = 0;
                    helper->ReqFuncQhSingleHqSubscribe(pptr, helper->m_SubFutuCodeSet.size(), -33100);
                }
                if(helper->m_SubArbitCodeSet.size() > 0)
                {
                    char** pptr = new char*[helper->m_SubArbitCodeSet.size() + 1]; 
                    int    i = 0;
                    for(set<string>::iterator it = helper->m_SubArbitCodeSet.begin(); it != helper->m_SubArbitCodeSet.end(); it++, i++)
                    {
                        pptr[i] = (char*)(*it).c_str(); 
                    }
                    pptr[i] = 0;
                    helper->ReqFuncQhCombineSubscribe(pptr, helper->m_SubArbitCodeSet.size(), -33102);
                }
                helper->m_SubSetCs.UnLock();
            }
        }
    }
}
