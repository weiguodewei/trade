#include "HundsunH5sdkApi_impl.h"

//处理错误信息
inline void DealErrorInfo(IF2UnPacker *lpUnPacker, CHundsunH5sdkRspInfoField *RspInfo)
{
};

CHundsunH5sdkApi *CHundsunH5sdkApi::CreateHundsunH5sdkApi(const char *pszFlowPath, bool bSaveLog)
{
    return new CHundsunH5sdkApi_impl(pszFlowPath, bSaveLog);
}

CHundsunH5sdkApi_impl::CHundsunH5sdkApi_impl(const char *pszFlowPath, bool bSaveLog)
{
    lpH5sdkSpi = NULL;
}

CHundsunH5sdkApi_impl::~CHundsunH5sdkApi_impl()
{
    StopTrd();

    hJoinEvent.Notify();
}

void CHundsunH5sdkApi_impl::ReleaseApi()
{
    delete this;
}

void CHundsunH5sdkApi_impl::Init()
{
    InitH5sdk();
    return;
}

int CHundsunH5sdkApi_impl::Join()
{
    hJoinEvent.Wait(-1);
    return 0;
}

void CHundsunH5sdkApi_impl::RegisterFront(const char *pszFrontAddress)
{
	hs_strncpy(m_szFrontAddress, pszFrontAddress, sizeof(m_szFrontAddress));

    return;
}

void CHundsunH5sdkApi_impl::RegisterSpi(CHundsunH5sdkSpi *pSpi)
{
    lpH5sdkSpi = pSpi;
    if(pSpi != NULL)
    {
        StartTrd();
    }
    else
    {
        StopTrd();
    }
}

//1001 登入请求请求
int CHundsunH5sdkApi_impl::ReqH5sdkLogin(CHundsunH5sdkReqLoginField *pReqLogin, int nRequestID)
{
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_LOGIN, REQUEST);
    //打包入参
    //登录类型
    lpRequest->GetItem(H5SDK_TAG_USER_NAME)->SetString(pReqLogin->UserName);
    //枚举。登录数据类型
    lpRequest->GetItem(H5SDK_TAG_PASSWORD)->SetString(pReqLogin->Password);
    //访问令牌
    lpRequest->GetItem(H5SDK_TAG_HQ_TOKEN)->SetString(pReqLogin->HqToken);
    //登录数据
    lpRequest->GetItem(H5SDK_TAG_DYNMIC_PASSWORD)->SetString(pReqLogin->DynPassword);
    //应用名称，包括版本信息
    lpRequest->GetItem(H5SDK_TAG_APP_NAME)->SetString(pReqLogin->AppName);
    //手机号
    lpRequest->GetItem(H5SDK_TAG_PHONE_NUMBER)->SetString(pReqLogin->PhoneNumber);
    //sdk版本号（sdk自动填入）
    lpRequest->GetItem(H5SDK_TAG_SDK_VERSION)->SetInt32(pReqLogin->SdkVersion);
    //操作系统版本（sdk自动填入）
    lpRequest->GetItem(H5SDK_TAG_OS_VERSION)->SetString(pReqLogin->OsVersion);
    //物理地址（sdk自动填入）
    lpRequest->GetItem(H5SDK_TAG_MAC_ADDRESS)->SetString(pReqLogin->MacAddress);

    //发送Message
    SendCommMessage(lpRequest, nRequestID);

    // 释放消息
    lpRequest->Release();

    return 0;
};

//1002 登出信息请求
int CHundsunH5sdkApi_impl::ReqH5sdkLogout(CHundsunH5sdkReqLogoutField *pReqLogout, int nRequestID)
{
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_LOGOUT, REQUEST);
    //打包入参
    //错误信息
    lpRequest->GetItem(H5SDK_TAG_QUIT_REASON)->SetString(pReqLogout->QuitReason);

    //发送Message
    SendCommMessage(lpRequest, nRequestID);

    // 释放消息
    lpRequest->Release();

    return 0;
};

//1003 心跳请求
int CHundsunH5sdkApi_impl::ReqH5sdkHeartbeat(CHundsunH5sdkReqHeartbeatField *pReqHeartbeat, int nRequestID)
{
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_HEARTBEAT, REQUEST);
    //打包入参

    //发送Message
    SendCommMessage(lpRequest, nRequestID);

    // 释放消息
    lpRequest->Release();

    return 0;
};

//1004 请求服务器信息请求
int CHundsunH5sdkApi_impl::ReqH5sdkServerInfo(CHundsunH5sdkReqServerInfoField *pReqServerInfo, int nRequestID)
{
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_SERVER_INFO, REQUEST);
    //打包入参

    //发送Message
    SendCommMessage(lpRequest, nRequestID);

    // 释放消息
    lpRequest->Release();

    return 0;
};

//1006 批处理请求
int CHundsunH5sdkApi_impl::ReqH5sdkBatch(CHundsunH5sdkReqBatchField *pReqBatch, int nRequestID)
{
    IGroup* lpGroup = NULL;
    IRecord* lpRecord = NULL;
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_BATCH, REQUEST);
    //打包入参
    //打包BatchGrp的Group参数
    if(pReqBatch->BatchGrp.size() > 0)
    {
        lpGroup = lpRequest->SetGroup(H5SDK_TAG_BATCH_GRP);
        vector<CHundsunH5sdkReqBatchBatchGrpField>::iterator itBatchGrp;
        for(itBatchGrp = pReqBatch->BatchGrp.begin(); itBatchGrp != pReqBatch->BatchGrp.end(); itBatchGrp++)
        {
            lpRecord = lpGroup->AddRecord();
            //子请求的报文二进制
            lpRecord->GetItem(H5SDK_TAG_SUB_PACKET)->SetString(itBatchGrp->SubPacket);
        }
    }

    //发送Message
    SendCommMessage(lpRequest, nRequestID);

    // 释放消息
    lpRequest->Release();

    return 0;
};

//2001 请求静态文件请求
int CHundsunH5sdkApi_impl::ReqH5sdkFile(CHundsunH5sdkReqFileField *pReqFile, int nRequestID)
{
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_FILE, REQUEST);
    //打包入参
    //文件类型
    lpRequest->GetItem(H5SDK_TAG_HQ_FILE_TYPE)->SetInt32(pReqFile->HqFileType);
    //文件相对名称
    lpRequest->GetItem(H5SDK_TAG_HQ_FILE_NAME)->SetString(pReqFile->HqFileName);
    //文件CRC
    lpRequest->GetItem(H5SDK_TAG_CRC)->SetInt32(pReqFile->Crc);
    //文件偏移
    lpRequest->GetItem(H5SDK_TAG_FILE_OFFSET)->SetInt64(pReqFile->FileOffset);
    //请求的长度（超过一定数值后，根据应答下次请求）
    lpRequest->GetItem(H5SDK_TAG_FILE_LENGTH)->SetInt32(pReqFile->FileLength);

    //发送Message
    SendCommMessage(lpRequest, nRequestID);

    // 释放消息
    lpRequest->Release();

    return 0;
};

//2002 市场分类信息请求
int CHundsunH5sdkApi_impl::ReqH5sdkMarketTypes(CHundsunH5sdkReqMarketTypesField *pReqMarketTypes, int nRequestID)
{
    IGroup* lpGroup = NULL;
    IRecord* lpRecord = NULL;
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_MARKET_TYPES, REQUEST);
    //打包入参
    //打包FinanceMicGrp的Group参数
    if(pReqMarketTypes->FinanceMicGrp.size() > 0)
    {
        lpGroup = lpRequest->SetGroup(H5SDK_FINANCE_MIC_GRP);
        vector<CHundsunH5sdkReqMarketTypesFinanceMicGrpField>::iterator itFinanceMicGrp;
        for(itFinanceMicGrp = pReqMarketTypes->FinanceMicGrp.begin(); itFinanceMicGrp != pReqMarketTypes->FinanceMicGrp.end(); itFinanceMicGrp++)
        {
            lpRecord = lpGroup->AddRecord();
            //交易所代码
            lpRecord->GetItem(H5SDK_TAG_FINANCE_MIC)->SetString(itFinanceMicGrp->FinanceMic);
        }
    }

    //发送Message
    SendCommMessage(lpRequest, nRequestID);

    // 释放消息
    lpRequest->Release();

    return 0;
};

//2003 市场代码表请求
int CHundsunH5sdkApi_impl::ReqH5sdkMarketReference(CHundsunH5sdkReqMarketReferenceField *pReqMarketReference, int nRequestID)
{
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_MARKET_REFERENCE, REQUEST);
    //打包入参
    //市场类别
    lpRequest->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->SetString(pReqMarketReference->HqTypeCode);

    //发送Message
    SendCommMessage(lpRequest, nRequestID);

    // 释放消息
    lpRequest->Release();

    return 0;
};

//3001 行情快照请求
int CHundsunH5sdkApi_impl::ReqH5sdkSnapshot(CHundsunH5sdkReqSnapshotField *pReqSnapshot, int nRequestID)
{
    IGroup* lpGroup = NULL;
    IRecord* lpRecord = NULL;
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_SNAPSHOT, REQUEST);
    //打包入参
    //行情级别: level1, evel2(对于美股level1,表示延迟,level2表示实时)
    lpRequest->GetItem(H5SDK_TAG_HQ_LEVEL)->SetInt8(pReqSnapshot->HqLevel);
    //打包ProdGrp的Group参数
    if(pReqSnapshot->ProdGrp.size() > 0)
    {
        lpGroup = lpRequest->SetGroup(H5SDK_TAG_PROD_GRP);
        vector<CHundsunH5sdkReqSnapshotProdGrpField>::iterator itProdGrp;
        for(itProdGrp = pReqSnapshot->ProdGrp.begin(); itProdGrp != pReqSnapshot->ProdGrp.end(); itProdGrp++)
        {
            lpRecord = lpGroup->AddRecord();
            //股票代码
            lpRecord->GetItem(H5SDK_TAG_PROD_CODE)->SetString(itProdGrp->ProdCode);
            //类型代码,[subtype][.cficode].exchage
            lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->SetString(itProdGrp->HqTypeCode);
        }
    }
    //打包FieldGrp的Group参数
    if(pReqSnapshot->FieldGrp.size() > 0)
    {
        lpGroup = lpRequest->SetGroup(H5SDK_TAG_FIELD_GRP);
        vector<CHundsunH5sdkReqSnapshotFieldGrpField>::iterator itFieldGrp;
        for(itFieldGrp = pReqSnapshot->FieldGrp.begin(); itFieldGrp != pReqSnapshot->FieldGrp.end(); itFieldGrp++)
        {
            lpRecord = lpGroup->AddRecord();
            //以此ID为准，当id为空时候，取filename
            lpRecord->GetItem(H5SDK_TAG_FIELD_ID)->SetInt32(itFieldGrp->FieldId);
            //字段ID
            lpRecord->GetItem(H5SDK_TAG_FIELD_NAME)->SetString(itFieldGrp->FieldName);
        }
    }

    //发送Message
    SendCommMessage(lpRequest, nRequestID);

    // 释放消息
    lpRequest->Release();

    return 0;
};

//3002 键盘精灵消息请求
int CHundsunH5sdkApi_impl::ReqH5sdkKeyboardWizard(CHundsunH5sdkReqKeyboardWizardField *pReqKeyboardWizard, int nRequestID)
{
    IGroup* lpGroup = NULL;
    IRecord* lpRecord = NULL;
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_KEYBOARD_WIZARD, REQUEST);
    //打包入参
    //
    lpRequest->GetItem(H5SDK_TAG_PROD_CODE)->SetString(pReqKeyboardWizard->ProdCode);
    //最大返回个数
    lpRequest->GetItem(H5SDK_TAG_MAX_VALUE)->SetInt32(pReqKeyboardWizard->Maxvalue);
    //打包TypeGrp的Group参数
    if(pReqKeyboardWizard->TypeGrp.size() > 0)
    {
        lpGroup = lpRequest->SetGroup(H5SDK_TAG_TYPE_GRP);
        vector<CHundsunH5sdkReqKeyboardWizardTypeGrpField>::iterator itTypeGrp;
        for(itTypeGrp = pReqKeyboardWizard->TypeGrp.begin(); itTypeGrp != pReqKeyboardWizard->TypeGrp.end(); itTypeGrp++)
        {
            lpRecord = lpGroup->AddRecord();
            //分类代码
            lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->SetString(itTypeGrp->HqTypeCode);
        }
    }

    //发送Message
    SendCommMessage(lpRequest, nRequestID);

    // 释放消息
    lpRequest->Release();

    return 0;
};

//4001 行情快照订阅请求
int CHundsunH5sdkApi_impl::ReqH5sdkSubscribe(CHundsunH5sdkReqSubscribeField *pReqSubscribe, int nRequestID)
{
    IGroup* lpGroup = NULL;
    IRecord* lpRecord = NULL;
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_SUBSCRIBE, REQUEST);
    //打包入参
    //行情级别: level1, evel2(对于美股level1,表示延迟,level2表示实时)
    lpRequest->GetItem(H5SDK_TAG_HQ_LEVEL)->SetInt8(pReqSubscribe->HqLevel);
    //打包ProdGrp的Group参数
    if(pReqSubscribe->ProdGrp.size() > 0)
    {
        lpGroup = lpRequest->SetGroup(H5SDK_TAG_PROD_GRP);
        vector<CHundsunH5sdkReqSubscribeProdGrpField>::iterator itProdGrp;
        for(itProdGrp = pReqSubscribe->ProdGrp.begin(); itProdGrp != pReqSubscribe->ProdGrp.end(); itProdGrp++)
        {
            lpRecord = lpGroup->AddRecord();
            //
            lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->SetString(itProdGrp->HqTypeCode);
            //
            lpRecord->GetItem(H5SDK_TAG_PROD_CODE)->SetString(itProdGrp->ProdCode);
        }
    }
    //订阅动作
    lpRequest->GetItem(H5SDK_TAG_SUB_TYPE)->SetInt32(pReqSubscribe->SubType);

    //发送Message
    SendCommMessage(lpRequest, nRequestID);

    // 释放消息
    lpRequest->Release();

    return 0;
};

//5001 排序请求请求
int CHundsunH5sdkApi_impl::ReqH5sdkSort(CHundsunH5sdkReqSortField *pReqSort, int nRequestID)
{
    IGroup* lpGroup = NULL;
    IRecord* lpRecord = NULL;
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_SORT, REQUEST);
    //打包入参
    //打包SortProdGrp的Group参数
    if(pReqSort->SortProdGrp.size() > 0)
    {
        lpGroup = lpRequest->SetGroup(H5SDK_TAG_SORT_PROD_GRP);
        vector<CHundsunH5sdkReqSortSortProdGrpField>::iterator itSortProdGrp;
        for(itSortProdGrp = pReqSort->SortProdGrp.begin(); itSortProdGrp != pReqSort->SortProdGrp.end(); itSortProdGrp++)
        {
            lpRecord = lpGroup->AddRecord();
            //股票代码
            lpRecord->GetItem(H5SDK_TAG_PROD_CODE)->SetString(itSortProdGrp->ProdCode);
            //类型代码,[subtype][.cficode].exchage
            lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->SetString(itSortProdGrp->HqTypeCode);
        }
    }
    //打包SortTypeGrp的Group参数
    if(pReqSort->SortTypeGrp.size() > 0)
    {
        lpGroup = lpRequest->SetGroup(H5SDK_TAG_SORT_TYPE_GRP);
        vector<CHundsunH5sdkReqSortSortTypeGrpField>::iterator itSortTypeGrp;
        for(itSortTypeGrp = pReqSort->SortTypeGrp.begin(); itSortTypeGrp != pReqSort->SortTypeGrp.end(); itSortTypeGrp++)
        {
            lpRecord = lpGroup->AddRecord();
            //分类代码
            lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->SetString(itSortTypeGrp->HqTypeCode);
        }
    }
    //需要排序的字段ID，以此为准，当这个字段不存在时候取sort_field_name
    lpRequest->GetItem(H5SDK_TAG_SORT_FIELD_ID)->SetInt32(pReqSort->SortFieldId);
    //需要排序的字段
    lpRequest->GetItem(H5SDK_TAG_SORT_FIELD_NAME)->SetString(pReqSort->SortFieldName);
    //打包FieldGrp的Group参数
    if(pReqSort->FieldGrp.size() > 0)
    {
        lpGroup = lpRequest->SetGroup(H5SDK_TAG_FIELD_GRP);
        vector<CHundsunH5sdkReqSortFieldGrpField>::iterator itFieldGrp;
        for(itFieldGrp = pReqSort->FieldGrp.begin(); itFieldGrp != pReqSort->FieldGrp.end(); itFieldGrp++)
        {
            lpRecord = lpGroup->AddRecord();
            //以此ID为准，当id为空时候，取filename
            lpRecord->GetItem(H5SDK_TAG_FIELD_ID)->SetInt32(itFieldGrp->FieldId);
            //字段ID
            lpRecord->GetItem(H5SDK_TAG_FIELD_NAME)->SetString(itFieldGrp->FieldName);
        }
    }
    //起始位置,默认为0
    lpRequest->GetItem(H5SDK_TAG_START_POS)->SetInt32(pReqSort->StartPos);
    //代码数目,默认100只代码，最大请求数目10000
    lpRequest->GetItem(H5SDK_TAG_DATA_COUNT)->SetInt32(pReqSort->DataCount);
    //排序方式/默认降序
    lpRequest->GetItem(H5SDK_TAG_SORT_TYPE)->SetInt8(pReqSort->SortType);

    //发送Message
    SendCommMessage(lpRequest, nRequestID);

    // 释放消息
    lpRequest->Release();

    return 0;
};

//6001 按指定的日期或偏移取分时数据请求
int CHundsunH5sdkApi_impl::ReqH5sdkTrend(CHundsunH5sdkReqTrendField *pReqTrend, int nRequestID)
{
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_TREND, REQUEST);
    //打包入参
    //证券代码
    lpRequest->GetItem(H5SDK_TAG_PROD_CODE)->SetString(pReqTrend->ProdCode);
    //类型代码
    lpRequest->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->SetString(pReqTrend->HqTypeCode);
    //日期:YYYYMMDD,0:表示今天
    lpRequest->GetItem(H5SDK_TAG_DATE)->SetInt32(pReqTrend->Date);
    //距离当前日期的偏移天数,
    lpRequest->GetItem(H5SDK_TAG_DATE_OFFSET)->SetInt32(pReqTrend->DateOffset);

    //发送Message
    SendCommMessage(lpRequest, nRequestID);

    // 释放消息
    lpRequest->Release();

    return 0;
};

//6002 按偏移取K线请求
int CHundsunH5sdkApi_impl::ReqH5sdkCandleByOffset(CHundsunH5sdkReqCandleByOffsetField *pReqCandleByOffset, int nRequestID)
{
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_CANDLE_BY_OFFSET, REQUEST);
    //打包入参
    //证券代码
    lpRequest->GetItem(H5SDK_TAG_PROD_CODE)->SetString(pReqCandleByOffset->ProdCode);
    //类型代码
    lpRequest->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->SetString(pReqCandleByOffset->HqTypeCode);
    //k线类型
    lpRequest->GetItem(H5SDK_TAG_CANDLE_PEROID)->SetInt32(pReqCandleByOffset->CandlePeriod);
    //
    lpRequest->GetItem(H5SDK_TAG_CANDLE_MODE)->SetInt8(pReqCandleByOffset->CandleMode);
    //起始日期，不填写表示当前日期
    lpRequest->GetItem(H5SDK_TAG_DATE)->SetInt32(pReqCandleByOffset->Date);
    //分钟K线的时间HHMM,对于短周期K线类型使用(1min,5min等)，不填写表示最新的市场时间，若填写必须同时填写date字段
    lpRequest->GetItem(H5SDK_TAG_MIN_TIME)->SetInt32(pReqCandleByOffset->MinTime);
    //方向
    lpRequest->GetItem(H5SDK_TAG_DIRECTION)->SetInt8(pReqCandleByOffset->SearchDirection);
    //请求的个数
    lpRequest->GetItem(H5SDK_TAG_DATA_COUNT)->SetInt32(pReqCandleByOffset->DataCount);

    //发送Message
    SendCommMessage(lpRequest, nRequestID);

    // 释放消息
    lpRequest->Release();

    return 0;
};

//6003 按日期时间范围取K线请求
int CHundsunH5sdkApi_impl::ReqH5sdkCandleByRange(CHundsunH5sdkReqCandleByRangeField *pReqCandleByRange, int nRequestID)
{
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_CANDLE_BY_RANGE, REQUEST);
    //打包入参
    //证券代码
    lpRequest->GetItem(H5SDK_TAG_PROD_CODE)->SetString(pReqCandleByRange->ProdCode);
    //类型代码
    lpRequest->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->SetString(pReqCandleByRange->HqTypeCode);
    //k线类型
    lpRequest->GetItem(H5SDK_TAG_CANDLE_PEROID)->SetInt32(pReqCandleByRange->CandlePeriod);
    //
    lpRequest->GetItem(H5SDK_TAG_CANDLE_MODE)->SetInt8(pReqCandleByRange->CandleMode);
    //
    lpRequest->GetItem(H5SDK_TAG_START_DATE)->SetInt32(pReqCandleByRange->StartDate);
    //
    lpRequest->GetItem(H5SDK_TAG_END_DATE)->SetInt32(pReqCandleByRange->EndDate);

    //发送Message
    SendCommMessage(lpRequest, nRequestID);

    // 释放消息
    lpRequest->Release();

    return 0;
};

//6004 指定分钟的分笔请求
int CHundsunH5sdkApi_impl::ReqH5sdkTickMin(CHundsunH5sdkReqTickMinField *pReqTickMin, int nRequestID)
{
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_TICK_MIN, REQUEST);
    //打包入参
    //证券代码
    lpRequest->GetItem(H5SDK_TAG_PROD_CODE)->SetString(pReqTickMin->ProdCode);
    //类型代码
    lpRequest->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->SetString(pReqTickMin->HqTypeCode);
    //日期:YYYYMMDD,0:表示今天
    lpRequest->GetItem(H5SDK_TAG_DATE)->SetInt32(pReqTickMin->Date);
    //起始分钟数
    lpRequest->GetItem(H5SDK_TAG_START_MIN)->SetInt32(pReqTickMin->StartMin);
    //结束分钟数
    lpRequest->GetItem(H5SDK_TAG_END_MIN)->SetInt32(pReqTickMin->EndMin);
    //最大多少个，默认全部返回
    lpRequest->GetItem(H5SDK_TAG_DATA_COUNT)->SetInt32(pReqTickMin->DataCount);

    //发送Message
    SendCommMessage(lpRequest, nRequestID);

    // 释放消息
    lpRequest->Release();

    return 0;
};

//6005 按偏移取分笔请求
int CHundsunH5sdkApi_impl::ReqH5sdkTickDirection(CHundsunH5sdkReqTickDirectionField *pReqTickDirection, int nRequestID)
{
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_TICK_DIRECTION, REQUEST);
    //打包入参
    //证券代码
    lpRequest->GetItem(H5SDK_TAG_PROD_CODE)->SetString(pReqTickDirection->ProdCode);
    //类型代码
    lpRequest->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->SetString(pReqTickDirection->HqTypeCode);
    //日期:YYYYMMDD,0:表示今天
    lpRequest->GetItem(H5SDK_TAG_DATE)->SetInt32(pReqTickDirection->Date);
    //起始位置,默认为0
    lpRequest->GetItem(H5SDK_TAG_START_POS)->SetInt32(pReqTickDirection->StartPos);
    //方向
    lpRequest->GetItem(H5SDK_TAG_DIRECTION)->SetInt8(pReqTickDirection->SearchDirection);
    //最大多少个，默认全部返回
    lpRequest->GetItem(H5SDK_TAG_DATA_COUNT)->SetInt32(pReqTickDirection->DataCount);

    //发送Message
    SendCommMessage(lpRequest, nRequestID);

    // 释放消息
    lpRequest->Release();

    return 0;
};

//6006 补线使用_分钟k线请求
int CHundsunH5sdkApi_impl::ReqH5sdkCandleByRangeMaintain(CHundsunH5sdkReqCandleByRangeMaintainField *pReqCandleByRangeMaintain, int nRequestID)
{
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_CANDLE_BY_RANGE_MAINTAIN, REQUEST);
    //打包入参
    //证券代码
    lpRequest->GetItem(H5SDK_TAG_PROD_CODE)->SetString(pReqCandleByRangeMaintain->ProdCode);
    //类型代码
    lpRequest->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->SetString(pReqCandleByRangeMaintain->HqTypeCode);
    //k线类型(只需要分钟k线)
    lpRequest->GetItem(H5SDK_TAG_CANDLE_PEROID)->SetInt32(pReqCandleByRangeMaintain->CandlePeriod);
    //
    lpRequest->GetItem(H5SDK_TAG_CANDLE_MODE)->SetInt8(pReqCandleByRangeMaintain->CandleMode);
    //
    lpRequest->GetItem(H5SDK_TAG_START_DATE)->SetInt32(pReqCandleByRangeMaintain->StartDate);
    //
    lpRequest->GetItem(H5SDK_TAG_END_DATE)->SetInt32(pReqCandleByRangeMaintain->EndDate);

    //发送Message
    SendCommMessage(lpRequest, nRequestID);

    // 释放消息
    lpRequest->Release();

    return 0;
};

//6007 5日分时请求
int CHundsunH5sdkApi_impl::ReqH5sdkDay5Trend(CHundsunH5sdkReqDay5TrendField *pReqDay5Trend, int nRequestID)
{
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_DAY5_TREND, REQUEST);
    //打包入参
    //证券代码
    lpRequest->GetItem(H5SDK_TAG_PROD_CODE)->SetString(pReqDay5Trend->ProdCode);
    //类型代码
    lpRequest->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->SetString(pReqDay5Trend->HqTypeCode);

    //发送Message
    SendCommMessage(lpRequest, nRequestID);

    // 释放消息
    lpRequest->Release();

    return 0;
};

//7001 补线返回请求
int CHundsunH5sdkApi_impl::ReqH5sdkMaintainReturn(CHundsunH5sdkReqMaintainReturnField *pReqMaintainReturn, int nRequestID)
{
    IGroup* lpGroup = NULL;
    IRecord* lpRecord = NULL;
    IHsCommMessage* lpRequest = lpSession->CreateMessage(BIZ_H5PROTO, H5SDK_MAINTAIN_RETURN, REQUEST);
    //打包入参
    //补线类型
    lpRequest->GetItem(H5SDK_TAG_SUB_KIND)->SetInt16(pReqMaintainReturn->Subkind);
    //
    lpRequest->GetItem(H5SDK_TAG_MAINTAIN_RETURN_VALUE)->SetInt32(pReqMaintainReturn->Maintainreturnvalue);
    //打包ProdGrp的Group参数
    if(pReqMaintainReturn->ProdGrp.size() > 0)
    {
        lpGroup = lpRequest->SetGroup(H5SDK_TAG_PROD_GRP);
        vector<CHundsunH5sdkReqMaintainReturnProdGrpField>::iterator itProdGrp;
        for(itProdGrp = pReqMaintainReturn->ProdGrp.begin(); itProdGrp != pReqMaintainReturn->ProdGrp.end(); itProdGrp++)
        {
            lpRecord = lpGroup->AddRecord();
            //股票代码
            lpRecord->GetItem(H5SDK_TAG_PROD_CODE)->SetString(itProdGrp->ProdCode);
            //类型代码,[subtype][.cficode].exchage
            lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->SetString(itProdGrp->HqTypeCode);
        }
    }

    //发送Message
    SendCommMessage(lpRequest, nRequestID);

    // 释放消息
    lpRequest->Release();

    return 0;
};

void CHundsunH5sdkApi_impl::OnConnected()
{
    if(lpH5sdkSpi)
    {
        lpH5sdkSpi->OnH5sdkFrontConnected();
    }

}

void CHundsunH5sdkApi_impl::OnClosed(int nReason)
{
    if(lpH5sdkSpi)
    {
        lpH5sdkSpi->OnH5sdkFrontDisconnected(nReason);
    }
}

void CHundsunH5sdkApi_impl::DealAsyCommMessage(IHsCommMessage* lpResponse)
{
    if (lpResponse!=NULL)
    {
        // 收到消息时, 先对该应答消息本身做正确性判断
        IRecord *head = lpResponse->GetHead();
        uint32 error_number = head->GetItem(H5PROTO_TAG_ERROR_NO)->GetInt32();
        if (error_number == 0)
        {
            // 获取报文的功能号
            int function_id = lpResponse->GetFunction();
            // 获取报文的消息体号
            IRecord *lpBody = lpResponse->GetBody();

            switch(function_id)
            {
            case H5SDK_MSG_LOGIN:
                OnAsyResponse_1001(lpBody, 0);
                break;
            case H5SDK_MSG_LOGOUT:
                OnAsyResponse_1002(lpBody, 0);
                break;
            case H5SDK_MSG_HEARTBEAT:
                OnAsyResponse_1003(lpBody, 0);
                break;
            case H5SDK_SERVER_INFO:
                OnAsyResponse_1004(lpBody, 0);
                break;
            case H5SDK_MSG_BATCH:
                OnAsyResponse_1006(lpBody, 0);
                break;
            case H5SDK_MSG_FILE:
                OnAsyResponse_2001(lpBody, 0);
                break;
            case H5SDK_MSG_MARKET_TYPES:
                OnAsyResponse_2002(lpBody, 0);
                break;
            case H5SDK_MSG_MARKET_REFERENCE:
                OnAsyResponse_2003(lpBody, 0);
                break;
            case H5SDK_MSG_SNAPSHOT:
                OnAsyResponse_3001(lpBody, 0);
                break;
            case H5SDK_KEYBOARD_WIZARD:
                OnAsyResponse_3002(lpBody, 0);
                break;
            case H5SDK_MSG_SUBSCRIBE:
                OnAsyResponse_4001(lpBody, 0);
                break;
            case H5SDK_MSG_SORT:
                OnAsyResponse_5001(lpBody, 0);
                break;
            case H5SDK_MSG_TREND:
                OnAsyResponse_6001(lpBody, 0);
                break;
            case H5SDK_MSG_CANDLE_BY_OFFSET:
                OnAsyResponse_6002(lpBody, 0);
                break;
            case H5SDK_MSG_CANDLE_BY_RANGE:
                OnAsyResponse_6003(lpBody, 0);
                break;
            case H5SDK_MSG_TICK_MIN:
                OnAsyResponse_6004(lpBody, 0);
                break;
            case H5SDK_MSG_TICK_DIRECTION:
                OnAsyResponse_6005(lpBody, 0);
                break;
            case H5SDK_MSG_CANDLE_BY_RANGE_MAINTAIN:
                OnAsyResponse_6006(lpBody, 0);
                break;
            case H5SDK_MSG_DAY5_TREND:
                OnAsyResponse_6007(lpBody, 0);
                break;
            case H5SDK_MAINTAIN_RETURN:
                OnAsyResponse_7001(lpBody, 0);
                break;
            default:
                break;
            }

        }
        else
        {

        }
    }
};

//1001 登入请求
void CHundsunH5sdkApi_impl::OnAsyResponse_1001(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //判断错误信息
    CHundsunH5sdkRspInfoField RspInfo;
    //返回信息结构体
    CHundsunH5sdkRspLoginField RspLogin;

    //错误号。使用H5PROTO_ENUM_EN_CUSTOM则使用CustomErrorNo和LoginData，否则使用ErrorInfo字段
    RspLogin.ErrorNo = lpBody->GetItem(H5SDK_TAG_ERROR_NO)->GetInt32();
    //错误信息
    hs_strncpy(RspLogin.ErrorInfo, U2G(lpBody->GetItem(H5SDK_TAG_ERROR_INFO)->GetString()), sizeof(RspLogin.ErrorInfo));
    //登录返回数据。比如Level2校验后台返回的数据
    hs_strncpy(RspLogin.OriginalData, U2G(lpBody->GetItem(H5SDK_TAG_ORGINAL_DATA)->GetString()), sizeof(RspLogin.OriginalData));
    //心跳间隔
    RspLogin.HeartbeatInterval = lpBody->GetItem(H5SDK_TAG_HEARTBEAT_INTERVAL)->GetInt32();

    //回调
    lpH5sdkSpi->OnRspH5sdkLogin(&RspLogin, &RspInfo, nRequestID, true);
}

//1002 登出信息
void CHundsunH5sdkApi_impl::OnAsyResponse_1002(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //判断错误信息
    CHundsunH5sdkRspInfoField RspInfo;
    //返回信息结构体
    CHundsunH5sdkRspLogoutField RspLogout;


    //回调
    lpH5sdkSpi->OnRspH5sdkLogout(&RspLogout, &RspInfo, nRequestID, true);
}

//1003 心跳
void CHundsunH5sdkApi_impl::OnAsyResponse_1003(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //判断错误信息
    CHundsunH5sdkRspInfoField RspInfo;
    //返回信息结构体
    CHundsunH5sdkRspHeartbeatField RspHeartbeat;


    //回调
    lpH5sdkSpi->OnRspH5sdkHeartbeat(&RspHeartbeat, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_ServerInfoFinanceGrp(CHundsunH5sdkRspServerInfoField* lpRspServerInfo, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspServerInfoFinanceGrpField m_FinanceGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //市场类型
        hs_strncpy(m_FinanceGrp.FinanceMic, U2G(lpRecord->GetItem(H5SDK_TAG_FINANCE_MIC)->GetString()), sizeof(m_FinanceGrp.FinanceMic));
        //市场交易日期
        hs_strncpy(m_FinanceGrp.FinanceName, U2G(lpRecord->GetItem(H5SDK_TAG_FINANCE_NAME)->GetString()), sizeof(m_FinanceGrp.FinanceName));
        //交易所日期
        m_FinanceGrp.InitDate = lpRecord->GetItem(H5SDK_TAG_INIT_DATE)->GetInt32();
        //交易所时间
        m_FinanceGrp.BusinessTime = lpRecord->GetItem(H5SDK_TAG_BUSINESS_TIME)->GetInt32();

        //塞入CHundsunH5sdkRspServerInfoFinanceGrpField数组
        lpRspServerInfo->FinanceGrp.push_back(m_FinanceGrp);
    }
}

//1004 请求服务器信息
void CHundsunH5sdkApi_impl::OnAsyResponse_1004(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //判断错误信息
    CHundsunH5sdkRspInfoField RspInfo;
    //返回信息结构体
    CHundsunH5sdkRspServerInfoField RspServerInfo;

    //服务器名称
    hs_strncpy(RspServerInfo.SrvName, U2G(lpBody->GetItem(H5SDK_TAG_SERVER_NAME)->GetString()), sizeof(RspServerInfo.SrvName));
    //当前服务器时间
    RspServerInfo.SrvTime = lpBody->GetItem(H5SDK_TAG_SERVER_TIME)->GetInt64();
    //当前在线（暂不使用）
    RspServerInfo.CurrOnlineCount = lpBody->GetItem(H5SDK_TAG_CURR_ONLINE_COUNT)->GetInt32();
    //最大允许在线（暂不使用）
    RspServerInfo.MaxOnlineCount = lpBody->GetItem(H5SDK_TAG_MAX_ONLINE_COUNT)->GetInt32();
    //服务器支持的交易所
    DealGroupAsyResponse_ServerInfoFinanceGrp(&RspServerInfo, lpBody->GetGroup(H5SDK_TAG_FINANCE_GRP));

    //回调
    lpH5sdkSpi->OnRspH5sdkServerInfo(&RspServerInfo, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_BatchBatchGrp(CHundsunH5sdkRspBatchField* lpRspBatch, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspBatchBatchGrpField m_BatchGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //子应答的报文二进制
        hs_strncpy(m_BatchGrp.SubPacket, U2G(lpRecord->GetItem(H5SDK_TAG_SUB_PACKET)->GetString()), sizeof(m_BatchGrp.SubPacket));

        //塞入CHundsunH5sdkRspBatchBatchGrpField数组
        lpRspBatch->BatchGrp.push_back(m_BatchGrp);
    }
}

//1006 批处理
void CHundsunH5sdkApi_impl::OnAsyResponse_1006(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //判断错误信息
    CHundsunH5sdkRspInfoField RspInfo;
    //返回信息结构体
    CHundsunH5sdkRspBatchField RspBatch;

    //用于存放批处理子应答的重复组
    DealGroupAsyResponse_BatchBatchGrp(&RspBatch, lpBody->GetGroup(H5SDK_TAG_BATCH_GRP));

    //回调
    lpH5sdkSpi->OnRspH5sdkBatch(&RspBatch, &RspInfo, nRequestID, true);
}

//2001 请求静态文件
void CHundsunH5sdkApi_impl::OnAsyResponse_2001(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //判断错误信息
    CHundsunH5sdkRspInfoField RspInfo;
    //返回信息结构体
    CHundsunH5sdkRspFileField RspFile;

    //错误号
    RspFile.ErrorNo = lpBody->GetItem(H5SDK_TAG_ERROR_NO)->GetInt32();
    //文件类型
    RspFile.HqFileType = lpBody->GetItem(H5SDK_TAG_HQ_FILE_TYPE)->GetInt32();
    //文件名称
    hs_strncpy(RspFile.HqFileName, U2G(lpBody->GetItem(H5SDK_TAG_HQ_FILE_NAME)->GetString()), sizeof(RspFile.HqFileName));
    //剩余长度
    RspFile.FileLength = lpBody->GetItem(H5SDK_TAG_FILE_LENGTH)->GetInt32();
    //下次请求的offset
    RspFile.FileOffset = lpBody->GetItem(H5SDK_TAG_FILE_OFFSET)->GetInt64();
    //文件CRC
    RspFile.Crc = lpBody->GetItem(H5SDK_TAG_CRC)->GetInt32();
    //文件数据
    hs_strncpy(RspFile.OriginalData, U2G(lpBody->GetItem(H5SDK_TAG_ORGINAL_DATA)->GetString()), sizeof(RspFile.OriginalData));

    //回调
    lpH5sdkSpi->OnRspH5sdkFile(&RspFile, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_MarketTypesTypeGrpTradeSectionGrp(CHundsunH5sdkRspMarketTypesTypeGrpField* lpRspMarketTypesTypeGrp, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspMarketTypesTradeSectionGrpField m_TradeSectionGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //开市时间
        m_TradeSectionGrp.Opentime = lpRecord->GetItem(H5SDK_TAG_OPEN_TIME)->GetInt32();
        //闭市时间
        m_TradeSectionGrp.Closetime = lpRecord->GetItem(H5SDK_TAG_CLOSE_TIME)->GetInt32();

        //塞入CHundsunH5sdkRspMarketTypesTradeSectionGrpField数组
        lpRspMarketTypesTypeGrp->TradeSectionGrp.push_back(m_TradeSectionGrp);
    }
}

inline void DealGroupAsyResponse_MarketTypesFinanceMicGrpTypeGrp(CHundsunH5sdkRspMarketTypesFinanceMicGrpField* lpRspMarketTypesFinanceMicGrp, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspMarketTypesTypeGrpField m_TypeGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //分类代码
        hs_strncpy(m_TypeGrp.HqTypeCode, U2G(lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(m_TypeGrp.HqTypeCode));
        //分类名称
        hs_strncpy(m_TypeGrp.HqTypeName, U2G(lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_NAME)->GetString()), sizeof(m_TypeGrp.HqTypeName));
        //分类放大倍数
        m_TypeGrp.PxScale = lpRecord->GetItem(H5SDK_TAG_PRICE_SCALE)->GetInt32();
        //价格精度
        m_TypeGrp.PxPrecision = lpRecord->GetItem(HSSDK_TAG_PX_PRECISION)->GetInt32();
        //交易日期
        m_TypeGrp.InitDate = lpRecord->GetItem(H5SDK_TAG_INIT_DATE)->GetInt32();
        //交易时间段
        DealGroupAsyResponse_MarketTypesTypeGrpTradeSectionGrp(&m_TypeGrp, lpRecord->GetGroup(H5SDK_TAG_TRADE_SECTION_GRP));

        //塞入CHundsunH5sdkRspMarketTypesTypeGrpField数组
        lpRspMarketTypesFinanceMicGrp->TypeGrp.push_back(m_TypeGrp);
    }
}

inline void DealGroupAsyResponse_MarketTypesFinanceMicGrp(CHundsunH5sdkRspMarketTypesField* lpRspMarketTypes, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspMarketTypesFinanceMicGrpField m_FinanceMicGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //交易所代码
        hs_strncpy(m_FinanceMicGrp.FinanceMic, U2G(lpRecord->GetItem(H5SDK_TAG_FINANCE_MIC)->GetString()), sizeof(m_FinanceMicGrp.FinanceMic));
        //交易所名称
        hs_strncpy(m_FinanceMicGrp.FinanceName, U2G(lpRecord->GetItem(H5SDK_TAG_FINANCE_NAME)->GetString()), sizeof(m_FinanceMicGrp.FinanceName));
        //市场日期
        m_FinanceMicGrp.MarketDate = lpRecord->GetItem(H5SDK_TAG_MARKET_DATE)->GetInt32();
        //交易日期
        m_FinanceMicGrp.InitDate = lpRecord->GetItem(H5SDK_TAG_INIT_DATE)->GetInt32();
        //市场所在时区
        m_FinanceMicGrp.Timezone = lpRecord->GetItem(H5SDK_TAG_TIMEZONE)->GetInt32();
        //时区码
        hs_strncpy(m_FinanceMicGrp.TimezoneCode, U2G(lpRecord->GetItem(H5SDK_TAG_TIMEZONE_CODE)->GetString()), sizeof(m_FinanceMicGrp.TimezoneCode));
        //夏令时标志
        m_FinanceMicGrp.Dst = lpRecord->GetItem(H5SDK_TAG_DST_FLAG)->GetInt8();
        //证券分类
        DealGroupAsyResponse_MarketTypesFinanceMicGrpTypeGrp(&m_FinanceMicGrp, lpRecord->GetGroup(H5SDK_TAG_TYPE_GRP));

        //塞入CHundsunH5sdkRspMarketTypesFinanceMicGrpField数组
        lpRspMarketTypes->FinanceMicGrp.push_back(m_FinanceMicGrp);
    }
}

//2002 市场分类信息
void CHundsunH5sdkApi_impl::OnAsyResponse_2002(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //判断错误信息
    CHundsunH5sdkRspInfoField RspInfo;
    //返回信息结构体
    CHundsunH5sdkRspMarketTypesField RspMarketTypes;

    //交易所代码
    DealGroupAsyResponse_MarketTypesFinanceMicGrp(&RspMarketTypes, lpBody->GetGroup(H5SDK_FINANCE_MIC_GRP));

    //回调
    lpH5sdkSpi->OnRspH5sdkMarketTypes(&RspMarketTypes, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_MarketReferenceProdGrp(CHundsunH5sdkRspMarketReferenceField* lpRspMarketReference, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspMarketReferenceProdGrpField m_ProdGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //证券代码
        hs_strncpy(m_ProdGrp.ProdCode, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(m_ProdGrp.ProdCode));
        //类型代码
        hs_strncpy(m_ProdGrp.HqTypeCode, U2G(lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(m_ProdGrp.HqTypeCode));
        //证券名称
        hs_strncpy(m_ProdGrp.ProdName, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_NAME)->GetString()), sizeof(m_ProdGrp.ProdName));
        //前收盘价
        m_ProdGrp.PreclosePx = lpRecord->GetItem(H5SDK_TAG_PRECLOSE_PX)->GetInt32();
        //涨停价格
        m_ProdGrp.UpPx = lpRecord->GetItem(H5SDK_TAG_UP_PRICE)->GetInt32();
        //跌停价格
        m_ProdGrp.DownPx = lpRecord->GetItem(H5SDK_TAG_DOWN_PRICE)->GetInt32();

        //塞入CHundsunH5sdkRspMarketReferenceProdGrpField数组
        lpRspMarketReference->ProdGrp.push_back(m_ProdGrp);
    }
}

//2003 市场代码表
void CHundsunH5sdkApi_impl::OnAsyResponse_2003(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //判断错误信息
    CHundsunH5sdkRspInfoField RspInfo;
    //返回信息结构体
    CHundsunH5sdkRspMarketReferenceField RspMarketReference;

    //交易所代码
    hs_strncpy(RspMarketReference.FinanceMic, U2G(lpBody->GetItem(H5SDK_TAG_FINANCE_MIC)->GetString()), sizeof(RspMarketReference.FinanceMic));
    //交易所MIC缩写
    hs_strncpy(RspMarketReference.MicAbbr, U2G(lpBody->GetItem(H5SDK_TAG_MIC_ABBR)->GetString()), sizeof(RspMarketReference.MicAbbr));
    //交易所名称
    hs_strncpy(RspMarketReference.FinanceName, U2G(lpBody->GetItem(H5SDK_TAG_FINANCE_NAME)->GetString()), sizeof(RspMarketReference.FinanceName));
    //市场日期
    RspMarketReference.MarketDate = lpBody->GetItem(H5SDK_TAG_MARKET_DATE)->GetInt32();
    //交易日期
    RspMarketReference.InitDate = lpBody->GetItem(H5SDK_TAG_INIT_DATE)->GetInt32();
    //市场所在时区
    RspMarketReference.Timezone = lpBody->GetItem(H5SDK_TAG_TIMEZONE)->GetInt32();
    //时区码
    hs_strncpy(RspMarketReference.TimezoneCode, U2G(lpBody->GetItem(H5SDK_TAG_TIMEZONE_CODE)->GetString()), sizeof(RspMarketReference.TimezoneCode));
    //夏令时标志
    RspMarketReference.Dst = lpBody->GetItem(H5SDK_TAG_DST_FLAG)->GetInt8();
    //代码
    DealGroupAsyResponse_MarketReferenceProdGrp(&RspMarketReference, lpBody->GetGroup(H5SDK_TAG_PROD_GRP));

    //回调
    lpH5sdkSpi->OnRspH5sdkMarketReference(&RspMarketReference, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_SnapshotBidGrpEntrustGrp(CHundsunH5sdkRspSnapshotBidGrpField* lpRspSnapshotBidGrp, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSnapshotEntrustGrpField m_EntrustGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //委托量
        m_EntrustGrp.EntrustAmount = lpRecord->GetItem(H5SDK_TAG_ENTRUST_AMOUNT)->GetInt32();

        //塞入CHundsunH5sdkRspSnapshotEntrustGrpField数组
        lpRspSnapshotBidGrp->EntrustGrp.push_back(m_EntrustGrp);
    }
}

inline void DealGroupAsyResponse_SnapshotOfferGrpEntrustGrp(CHundsunH5sdkRspSnapshotOfferGrpField* lpRspSnapshotOfferGrp, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSnapshotEntrustGrpField m_EntrustGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //委托量
        m_EntrustGrp.EntrustAmount = lpRecord->GetItem(H5SDK_TAG_ENTRUST_AMOUNT)->GetInt32();

        //塞入CHundsunH5sdkRspSnapshotEntrustGrpField数组
        lpRspSnapshotOfferGrp->EntrustGrp.push_back(m_EntrustGrp);
    }
}

inline void DealGroupAsyResponse_SnapshotProdGrpBidGrp(CHundsunH5sdkRspSnapshotProdGrpField* lpRspSnapshotProdGrp, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSnapshotBidGrpField m_BidGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //委托价格
        m_BidGrp.EntrustPx = lpRecord->GetItem(H5SDK_TAG_ENTRUST_PX)->GetInt32();
        //委托量
        m_BidGrp.TotalEntrustAmount = lpRecord->GetItem(H5SDK_TAG_TOTAL_ENTRUST_AMOUNT)->GetInt64();
        //委托量
        m_BidGrp.EntrustCount = lpRecord->GetItem(H5SDK_TAG_ENTRUST_COUNT)->GetInt32();
        //委托订单列表
        DealGroupAsyResponse_SnapshotBidGrpEntrustGrp(&m_BidGrp, lpRecord->GetGroup(H5SDK_TAG_ENTRUST_GRP));

        //塞入CHundsunH5sdkRspSnapshotBidGrpField数组
        lpRspSnapshotProdGrp->BidGrp.push_back(m_BidGrp);
    }
}

inline void DealGroupAsyResponse_SnapshotProdGrpOfferGrp(CHundsunH5sdkRspSnapshotProdGrpField* lpRspSnapshotProdGrp, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSnapshotOfferGrpField m_OfferGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //委托价格
        m_OfferGrp.EntrustPx = lpRecord->GetItem(H5SDK_TAG_ENTRUST_PX)->GetInt32();
        //委托量
        m_OfferGrp.TotalEntrustAmount = lpRecord->GetItem(H5SDK_TAG_TOTAL_ENTRUST_AMOUNT)->GetInt64();
        //委托量
        m_OfferGrp.EntrustCount = lpRecord->GetItem(H5SDK_TAG_ENTRUST_COUNT)->GetInt32();
        //委托订单列表
        DealGroupAsyResponse_SnapshotOfferGrpEntrustGrp(&m_OfferGrp, lpRecord->GetGroup(H5SDK_TAG_ENTRUST_GRP));

        //塞入CHundsunH5sdkRspSnapshotOfferGrpField数组
        lpRspSnapshotProdGrp->OfferGrp.push_back(m_OfferGrp);
    }
}

inline void DealGroupAsyResponse_SnapshotProdGrpRiseFirstGrp(CHundsunH5sdkRspSnapshotProdGrpField* lpRspSnapshotProdGrp, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSnapshotRiseFirstGrpField m_RiseFirstGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //证券代码
        hs_strncpy(m_RiseFirstGrp.ProdCode, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(m_RiseFirstGrp.ProdCode));
        //证券名称
        hs_strncpy(m_RiseFirstGrp.ProdName, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_NAME)->GetString()), sizeof(m_RiseFirstGrp.ProdName));
        //证券名称
        hs_strncpy(m_RiseFirstGrp.HqTypeCode, U2G(lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(m_RiseFirstGrp.HqTypeCode));
        //最新价
        m_RiseFirstGrp.LastPx = lpRecord->GetItem(H5SDK_TAG_LAST_PX)->GetInt32();
        //涨跌幅
        m_RiseFirstGrp.PxChangeRate = lpRecord->GetItem(H5SDK_TAG_PX_CHANGE_RATE)->GetInt32();

        //塞入CHundsunH5sdkRspSnapshotRiseFirstGrpField数组
        lpRspSnapshotProdGrp->RiseFirstGrp.push_back(m_RiseFirstGrp);
    }
}

inline void DealGroupAsyResponse_SnapshotProdGrpFallFirstGrp(CHundsunH5sdkRspSnapshotProdGrpField* lpRspSnapshotProdGrp, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSnapshotFallFirstGrpField m_FallFirstGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //证券代码
        hs_strncpy(m_FallFirstGrp.ProdCode, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(m_FallFirstGrp.ProdCode));
        //证券名称
        hs_strncpy(m_FallFirstGrp.ProdName, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_NAME)->GetString()), sizeof(m_FallFirstGrp.ProdName));
        //证券名称
        hs_strncpy(m_FallFirstGrp.HqTypeCode, U2G(lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(m_FallFirstGrp.HqTypeCode));
        //最新价
        m_FallFirstGrp.LastPx = lpRecord->GetItem(H5SDK_TAG_LAST_PX)->GetInt32();
        //涨跌幅
        m_FallFirstGrp.PxChangeRate = lpRecord->GetItem(H5SDK_TAG_PX_CHANGE_RATE)->GetInt32();

        //塞入CHundsunH5sdkRspSnapshotFallFirstGrpField数组
        lpRspSnapshotProdGrp->FallFirstGrp.push_back(m_FallFirstGrp);
    }
}

inline void DealGroupAsyResponse_SnapshotProdGrp(CHundsunH5sdkRspSnapshotField* lpRspSnapshot, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSnapshotProdGrpField m_ProdGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //证券代码
        hs_strncpy(m_ProdGrp.ProdCode, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(m_ProdGrp.ProdCode));
        //证券名称
        hs_strncpy(m_ProdGrp.ProdName, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_NAME)->GetString()), sizeof(m_ProdGrp.ProdName));
        //证券名称
        hs_strncpy(m_ProdGrp.HqTypeCode, U2G(lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(m_ProdGrp.HqTypeCode));
        //行情代码（未定义）
        hs_strncpy(m_ProdGrp.IndustryCode, U2G(lpRecord->GetItem(H5SDK_TAG_INDUSTRY_CODE)->GetString()), sizeof(m_ProdGrp.IndustryCode));
        //货币代码（未定义）
        hs_strncpy(m_ProdGrp.MoneyType, U2G(lpRecord->GetItem(H5SDK_TAG_MONEY_TYPE)->GetString()), sizeof(m_ProdGrp.MoneyType));
        //时间戳
        m_ProdGrp.DataTimestamp = lpRecord->GetItem(H5SDK_TAG_DATA_TIMESTAMP)->GetInt32();
        //交易分钟数
        m_ProdGrp.TradeMins = lpRecord->GetItem(H5SDK_TAG_TRADE_MINS)->GetInt32();
        //交易状态
        hs_strncpy(m_ProdGrp.TradeStatus, U2G(lpRecord->GetItem(H5SDK_TAG_TRADE_STATUS)->GetString()), sizeof(m_ProdGrp.TradeStatus));
        //昨收价
        m_ProdGrp.PreclosePx = lpRecord->GetItem(H5SDK_TAG_PRECLOSE_PX)->GetInt32();
        //今开盘
        m_ProdGrp.OpenPx = lpRecord->GetItem(H5SDK_TAG_OPEN_PX)->GetInt32();
        //最新成交价
        m_ProdGrp.LastPx = lpRecord->GetItem(H5SDK_TAG_LAST_PX)->GetInt32();
        //最高价
        m_ProdGrp.HighPx = lpRecord->GetItem(H5SDK_TAG_HIGH_PX)->GetInt32();
        //最低价
        m_ProdGrp.LowPx = lpRecord->GetItem(H5SDK_TAG_LOW_PX)->GetInt32();
        //今日收盘
        m_ProdGrp.ClosePx = lpRecord->GetItem(H5SDK_TAG_CLOSE_PX)->GetInt32();
        //均价
        m_ProdGrp.AvgPx = lpRecord->GetItem(H5SDK_TAG_AVG_PX)->GetInt32();
        //加权平均价
        m_ProdGrp.WavgPx = lpRecord->GetItem(H5SDK_TAG_WAVG_PX)->GetInt32();
        //成交笔数
        m_ProdGrp.BusinessCount = lpRecord->GetItem(H5SDK_TAG_BUSINESS_COUNT)->GetInt32();
        //总成交量
        m_ProdGrp.BusinessAmount = lpRecord->GetItem(H5SDK_TAG_BUSINESS_AMOUNT)->GetInt64();
        //总成交额
        m_ProdGrp.BusinessBalance = lpRecord->GetItem(H5SDK_TAG_BUSINESS_BALANCE)->GetInt64();
        //涨停价格
        m_ProdGrp.UpPx = lpRecord->GetItem(H5SDK_TAG_UP_PRICE)->GetInt32();
        //跌停价格
        m_ProdGrp.DownPx = lpRecord->GetItem(H5SDK_TAG_DOWN_PRICE)->GetInt32();
        //最近成交量(现手)
        m_ProdGrp.CurrentAmount = lpRecord->GetItem(H5SDK_TAG_CURRENT_AMOUNT)->GetInt64();
        //内盘成交量
        m_ProdGrp.BusinessAmountIn = lpRecord->GetItem(H5SDK_TAG_BUSINESS_AMOUNT_IN)->GetInt64();
        //外盘成交量
        m_ProdGrp.BusinessAmountOut = lpRecord->GetItem(H5SDK_TAG_BUSINESS_AMOUNT_OUT)->GetInt64();
        //总委买量（未定义）
        m_ProdGrp.TotBuyAmount = lpRecord->GetItem(H5SDK_TAG_TOTAL_BUY_AMOUNT)->GetInt64();
        //总委卖量（未定义）
        m_ProdGrp.TotSellAmount = lpRecord->GetItem(H5SDK_TAG_TOTAL_SELL_AMOUNT)->GetInt64();
        //加权平均委买价格（未定义）
        m_ProdGrp.WavgBidPx = lpRecord->GetItem(H5SDK_TAG_WAVG_BID_PX)->GetInt32();
        //加权平均委卖价格（未定义）
        m_ProdGrp.WavgOfferPx = lpRecord->GetItem(H5SDK_TAG_WAVG_OFFER_PX)->GetInt32();
        //委买档位
        DealGroupAsyResponse_SnapshotProdGrpBidGrp(&m_ProdGrp, lpRecord->GetGroup(H5SDK_TAG_BID_GRP));

        //塞入CHundsunH5sdkRspSnapshotProdGrpField数组
        lpRspSnapshot->ProdGrp.push_back(m_ProdGrp);
    }
}

//3001 行情快照
void CHundsunH5sdkApi_impl::OnAsyResponse_3001(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //判断错误信息
    CHundsunH5sdkRspInfoField RspInfo;
    //返回信息结构体
    CHundsunH5sdkRspSnapshotField RspSnapshot;

    //行情级别: level1, evel2(对于美股level1,表示延迟,level2表示实时)
    RspSnapshot.HqLevel = lpBody->GetItem(H5SDK_TAG_HQ_LEVEL)->GetInt8();
    //代码列表
    DealGroupAsyResponse_SnapshotProdGrp(&RspSnapshot, lpBody->GetGroup(H5SDK_TAG_PROD_GRP));

    //回调
    lpH5sdkSpi->OnRspH5sdkSnapshot(&RspSnapshot, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_KeyboardWizardProdGrp(CHundsunH5sdkRspKeyboardWizardField* lpRspKeyboardWizard, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspKeyboardWizardProdGrpField m_ProdGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //证券代码
        hs_strncpy(m_ProdGrp.ProdCode, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(m_ProdGrp.ProdCode));
        //证券名称
        hs_strncpy(m_ProdGrp.ProdName, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_NAME)->GetString()), sizeof(m_ProdGrp.ProdName));
        //证券类别
        hs_strncpy(m_ProdGrp.HqTypeCode, U2G(lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(m_ProdGrp.HqTypeCode));

        //塞入CHundsunH5sdkRspKeyboardWizardProdGrpField数组
        lpRspKeyboardWizard->ProdGrp.push_back(m_ProdGrp);
    }
}

//3002 键盘精灵消息
void CHundsunH5sdkApi_impl::OnAsyResponse_3002(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //判断错误信息
    CHundsunH5sdkRspInfoField RspInfo;
    //返回信息结构体
    CHundsunH5sdkRspKeyboardWizardField RspKeyboardWizard;

    //代码列表
    DealGroupAsyResponse_KeyboardWizardProdGrp(&RspKeyboardWizard, lpBody->GetGroup(H5SDK_TAG_PROD_GRP));

    //回调
    lpH5sdkSpi->OnRspH5sdkKeyboardWizard(&RspKeyboardWizard, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_SubscribeProdGrp(CHundsunH5sdkRspSubscribeField* lpRspSubscribe, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSubscribeProdGrpField m_ProdGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //
        hs_strncpy(m_ProdGrp.HqTypeCode, U2G(lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(m_ProdGrp.HqTypeCode));
        //代码
        hs_strncpy(m_ProdGrp.ProdCode, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(m_ProdGrp.ProdCode));
        //0成功,其他:失败
        m_ProdGrp.ErrorNo = lpRecord->GetItem(H5SDK_TAG_ERROR_NO)->GetInt32();

        //塞入CHundsunH5sdkRspSubscribeProdGrpField数组
        lpRspSubscribe->ProdGrp.push_back(m_ProdGrp);
    }
}

//4001 行情快照订阅
void CHundsunH5sdkApi_impl::OnAsyResponse_4001(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //判断错误信息
    CHundsunH5sdkRspInfoField RspInfo;
    //返回信息结构体
    CHundsunH5sdkRspSubscribeField RspSubscribe;

    //行情级别: level1, evel2(对于美股level1,表示延迟,level2表示实时)
    RspSubscribe.HqLevel = lpBody->GetItem(H5SDK_TAG_HQ_LEVEL)->GetInt8();
    //
    DealGroupAsyResponse_SubscribeProdGrp(&RspSubscribe, lpBody->GetGroup(H5SDK_TAG_PROD_GRP));
    //原样返回订阅动作
    RspSubscribe.SubType = lpBody->GetItem(H5SDK_TAG_SUB_TYPE)->GetInt32();

    //回调
    lpH5sdkSpi->OnRspH5sdkSubscribe(&RspSubscribe, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_SortBidGrpEntrustGrp(CHundsunH5sdkRspSortBidGrpField* lpRspSortBidGrp, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSortEntrustGrpField m_EntrustGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //委托量
        m_EntrustGrp.EntrustAmount = lpRecord->GetItem(H5SDK_TAG_ENTRUST_AMOUNT)->GetInt32();

        //塞入CHundsunH5sdkRspSortEntrustGrpField数组
        lpRspSortBidGrp->EntrustGrp.push_back(m_EntrustGrp);
    }
}

inline void DealGroupAsyResponse_SortOfferGrpEntrustGrp(CHundsunH5sdkRspSortOfferGrpField* lpRspSortOfferGrp, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSortEntrustGrpField m_EntrustGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //委托量
        m_EntrustGrp.EntrustAmount = lpRecord->GetItem(H5SDK_TAG_ENTRUST_AMOUNT)->GetInt32();

        //塞入CHundsunH5sdkRspSortEntrustGrpField数组
        lpRspSortOfferGrp->EntrustGrp.push_back(m_EntrustGrp);
    }
}

inline void DealGroupAsyResponse_SortSortProdGrpBidGrp(CHundsunH5sdkRspSortSortProdGrpField* lpRspSortSortProdGrp, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSortBidGrpField m_BidGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //委托价格
        m_BidGrp.EntrustPx = lpRecord->GetItem(H5SDK_TAG_ENTRUST_PX)->GetInt32();
        //委托量
        m_BidGrp.TotalEntrustAmount = lpRecord->GetItem(H5SDK_TAG_TOTAL_ENTRUST_AMOUNT)->GetInt64();
        //委托量
        m_BidGrp.EntrustCount = lpRecord->GetItem(H5SDK_TAG_ENTRUST_COUNT)->GetInt32();
        //委托订单列表
        DealGroupAsyResponse_SortBidGrpEntrustGrp(&m_BidGrp, lpRecord->GetGroup(H5SDK_TAG_ENTRUST_GRP));

        //塞入CHundsunH5sdkRspSortBidGrpField数组
        lpRspSortSortProdGrp->BidGrp.push_back(m_BidGrp);
    }
}

inline void DealGroupAsyResponse_SortSortProdGrpOfferGrp(CHundsunH5sdkRspSortSortProdGrpField* lpRspSortSortProdGrp, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSortOfferGrpField m_OfferGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //委托价格
        m_OfferGrp.EntrustPx = lpRecord->GetItem(H5SDK_TAG_ENTRUST_PX)->GetInt32();
        //委托量
        m_OfferGrp.TotalEntrustAmount = lpRecord->GetItem(H5SDK_TAG_TOTAL_ENTRUST_AMOUNT)->GetInt64();
        //委托量
        m_OfferGrp.EntrustCount = lpRecord->GetItem(H5SDK_TAG_ENTRUST_COUNT)->GetInt32();
        //委托订单列表
        DealGroupAsyResponse_SortOfferGrpEntrustGrp(&m_OfferGrp, lpRecord->GetGroup(H5SDK_TAG_ENTRUST_GRP));

        //塞入CHundsunH5sdkRspSortOfferGrpField数组
        lpRspSortSortProdGrp->OfferGrp.push_back(m_OfferGrp);
    }
}

inline void DealGroupAsyResponse_SortSortProdGrpRiseFirstGrp(CHundsunH5sdkRspSortSortProdGrpField* lpRspSortSortProdGrp, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSortRiseFirstGrpField m_RiseFirstGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //证券代码
        hs_strncpy(m_RiseFirstGrp.ProdCode, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(m_RiseFirstGrp.ProdCode));
        //证券名称
        hs_strncpy(m_RiseFirstGrp.ProdName, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_NAME)->GetString()), sizeof(m_RiseFirstGrp.ProdName));
        //证券名称
        hs_strncpy(m_RiseFirstGrp.HqTypeCode, U2G(lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(m_RiseFirstGrp.HqTypeCode));
        //最新价
        m_RiseFirstGrp.LastPx = lpRecord->GetItem(H5SDK_TAG_LAST_PX)->GetInt32();
        //涨跌幅
        m_RiseFirstGrp.PxChangeRate = lpRecord->GetItem(H5SDK_TAG_PX_CHANGE_RATE)->GetInt32();

        //塞入CHundsunH5sdkRspSortRiseFirstGrpField数组
        lpRspSortSortProdGrp->RiseFirstGrp.push_back(m_RiseFirstGrp);
    }
}

inline void DealGroupAsyResponse_SortSortProdGrpFallFirstGrp(CHundsunH5sdkRspSortSortProdGrpField* lpRspSortSortProdGrp, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSortFallFirstGrpField m_FallFirstGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //证券代码
        hs_strncpy(m_FallFirstGrp.ProdCode, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(m_FallFirstGrp.ProdCode));
        //证券名称
        hs_strncpy(m_FallFirstGrp.ProdName, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_NAME)->GetString()), sizeof(m_FallFirstGrp.ProdName));
        //证券名称
        hs_strncpy(m_FallFirstGrp.HqTypeCode, U2G(lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(m_FallFirstGrp.HqTypeCode));
        //最新价
        m_FallFirstGrp.LastPx = lpRecord->GetItem(H5SDK_TAG_LAST_PX)->GetInt32();
        //涨跌幅
        m_FallFirstGrp.PxChangeRate = lpRecord->GetItem(H5SDK_TAG_PX_CHANGE_RATE)->GetInt32();

        //塞入CHundsunH5sdkRspSortFallFirstGrpField数组
        lpRspSortSortProdGrp->FallFirstGrp.push_back(m_FallFirstGrp);
    }
}

inline void DealGroupAsyResponse_SortSortTypeGrp(CHundsunH5sdkRspSortField* lpRspSort, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSortSortTypeGrpField m_SortTypeGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //分类代码
        hs_strncpy(m_SortTypeGrp.HqTypeCode, U2G(lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(m_SortTypeGrp.HqTypeCode));

        //塞入CHundsunH5sdkRspSortSortTypeGrpField数组
        lpRspSort->SortTypeGrp.push_back(m_SortTypeGrp);
    }
}

inline void DealGroupAsyResponse_SortSortProdGrp(CHundsunH5sdkRspSortField* lpRspSort, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspSortSortProdGrpField m_SortProdGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //证券代码
        hs_strncpy(m_SortProdGrp.ProdCode, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(m_SortProdGrp.ProdCode));
        //证券名称
        hs_strncpy(m_SortProdGrp.ProdName, U2G(lpRecord->GetItem(H5SDK_TAG_PROD_NAME)->GetString()), sizeof(m_SortProdGrp.ProdName));
        //证券名称
        hs_strncpy(m_SortProdGrp.HqTypeCode, U2G(lpRecord->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(m_SortProdGrp.HqTypeCode));
        //行情代码（未定义）
        hs_strncpy(m_SortProdGrp.IndustryCode, U2G(lpRecord->GetItem(H5SDK_TAG_INDUSTRY_CODE)->GetString()), sizeof(m_SortProdGrp.IndustryCode));
        //货币代码（未定义）
        hs_strncpy(m_SortProdGrp.MoneyType, U2G(lpRecord->GetItem(H5SDK_TAG_MONEY_TYPE)->GetString()), sizeof(m_SortProdGrp.MoneyType));
        //时间戳
        m_SortProdGrp.DataTimestamp = lpRecord->GetItem(H5SDK_TAG_DATA_TIMESTAMP)->GetInt32();
        //交易分钟数
        m_SortProdGrp.TradeMins = lpRecord->GetItem(H5SDK_TAG_TRADE_MINS)->GetInt32();
        //交易状态
        hs_strncpy(m_SortProdGrp.TradeStatus, U2G(lpRecord->GetItem(H5SDK_TAG_TRADE_STATUS)->GetString()), sizeof(m_SortProdGrp.TradeStatus));
        //昨收价
        m_SortProdGrp.PreclosePx = lpRecord->GetItem(H5SDK_TAG_PRECLOSE_PX)->GetInt32();
        //今开盘
        m_SortProdGrp.OpenPx = lpRecord->GetItem(H5SDK_TAG_OPEN_PX)->GetInt32();
        //最新成交价
        m_SortProdGrp.LastPx = lpRecord->GetItem(H5SDK_TAG_LAST_PX)->GetInt32();
        //最高价
        m_SortProdGrp.HighPx = lpRecord->GetItem(H5SDK_TAG_HIGH_PX)->GetInt32();
        //最低价
        m_SortProdGrp.LowPx = lpRecord->GetItem(H5SDK_TAG_LOW_PX)->GetInt32();
        //今日收盘
        m_SortProdGrp.ClosePx = lpRecord->GetItem(H5SDK_TAG_CLOSE_PX)->GetInt32();
        //均价
        m_SortProdGrp.AvgPx = lpRecord->GetItem(H5SDK_TAG_AVG_PX)->GetInt32();
        //加权平均价
        m_SortProdGrp.WavgPx = lpRecord->GetItem(H5SDK_TAG_WAVG_PX)->GetInt32();
        //成交笔数
        m_SortProdGrp.BusinessCount = lpRecord->GetItem(H5SDK_TAG_BUSINESS_COUNT)->GetInt32();
        //总成交量
        m_SortProdGrp.BusinessAmount = lpRecord->GetItem(H5SDK_TAG_BUSINESS_AMOUNT)->GetInt64();
        //总成交额
        m_SortProdGrp.BusinessBalance = lpRecord->GetItem(H5SDK_TAG_BUSINESS_BALANCE)->GetInt64();
        //涨停价格
        m_SortProdGrp.UpPx = lpRecord->GetItem(H5SDK_TAG_UP_PRICE)->GetInt32();
        //跌停价格
        m_SortProdGrp.DownPx = lpRecord->GetItem(H5SDK_TAG_DOWN_PRICE)->GetInt32();
        //最近成交量(现手)
        m_SortProdGrp.CurrentAmount = lpRecord->GetItem(H5SDK_TAG_CURRENT_AMOUNT)->GetInt64();
        //内盘成交量
        m_SortProdGrp.BusinessAmountIn = lpRecord->GetItem(H5SDK_TAG_BUSINESS_AMOUNT_IN)->GetInt64();
        //外盘成交量
        m_SortProdGrp.BusinessAmountOut = lpRecord->GetItem(H5SDK_TAG_BUSINESS_AMOUNT_OUT)->GetInt64();
        //总委买量（未定义）
        m_SortProdGrp.TotBuyAmount = lpRecord->GetItem(H5SDK_TAG_TOTAL_BUY_AMOUNT)->GetInt64();
        //总委卖量（未定义）
        m_SortProdGrp.TotSellAmount = lpRecord->GetItem(H5SDK_TAG_TOTAL_SELL_AMOUNT)->GetInt64();
        //加权平均委买价格（未定义）
        m_SortProdGrp.WavgBidPx = lpRecord->GetItem(H5SDK_TAG_WAVG_BID_PX)->GetInt32();
        //加权平均委卖价格（未定义）
        m_SortProdGrp.WavgOfferPx = lpRecord->GetItem(H5SDK_TAG_WAVG_OFFER_PX)->GetInt32();
        //委买档位
        DealGroupAsyResponse_SortSortProdGrpBidGrp(&m_SortProdGrp, lpRecord->GetGroup(H5SDK_TAG_BID_GRP));

        //塞入CHundsunH5sdkRspSortSortProdGrpField数组
        lpRspSort->SortProdGrp.push_back(m_SortProdGrp);
    }
}

//5001 排序请求
void CHundsunH5sdkApi_impl::OnAsyResponse_5001(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //判断错误信息
    CHundsunH5sdkRspInfoField RspInfo;
    //返回信息结构体
    CHundsunH5sdkRspSortField RspSort;

    //排序方式/默认降序
    RspSort.SortType = lpBody->GetItem(H5SDK_TAG_SORT_TYPE)->GetInt8();
    //证券分类
    DealGroupAsyResponse_SortSortTypeGrp(&RspSort, lpBody->GetGroup(H5SDK_TAG_SORT_TYPE_GRP));
    //需要排序的字段ID
    RspSort.SortFieldId = lpBody->GetItem(H5SDK_TAG_SORT_FIELD_ID)->GetInt32();
    //需要排序的字段
    hs_strncpy(RspSort.SortFieldName, U2G(lpBody->GetItem(H5SDK_TAG_SORT_FIELD_NAME)->GetString()), sizeof(RspSort.SortFieldName));
    //起始位置,默认为0
    RspSort.StartPos = lpBody->GetItem(H5SDK_TAG_START_POS)->GetInt32();
    //代码列表
    DealGroupAsyResponse_SortSortProdGrp(&RspSort, lpBody->GetGroup(H5SDK_TAG_SORT_PROD_GRP));

    //回调
    lpH5sdkSpi->OnRspH5sdkSort(&RspSort, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_TrendTrendGrp(CHundsunH5sdkRspTrendField* lpRspTrend, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspTrendTrendGrpField m_TrendGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //分时时间hhmm
        m_TrendGrp.MinTime = lpRecord->GetItem(H5SDK_TAG_MIN_TIME)->GetInt32();
        //价格
        m_TrendGrp.HqPx = lpRecord->GetItem(H5SDK_TAG_HQ_PRICE)->GetInt32();
        //均价
        m_TrendGrp.AvgPx = lpRecord->GetItem(H5SDK_TAG_AVG_PX)->GetInt32();
        //加权均价
        m_TrendGrp.WavgPx = lpRecord->GetItem(H5SDK_TAG_WAVG_PX)->GetInt32();
        //总成交量
        m_TrendGrp.BusinessAmount = lpRecord->GetItem(H5SDK_TAG_BUSINESS_AMOUNT)->GetInt64();
        //总成交额
        m_TrendGrp.BusinessBalance = lpRecord->GetItem(H5SDK_TAG_BUSINESS_BALANCE)->GetInt64();

        //塞入CHundsunH5sdkRspTrendTrendGrpField数组
        lpRspTrend->TrendGrp.push_back(m_TrendGrp);
    }
}

//6001 按指定的日期或偏移取分时数据
void CHundsunH5sdkApi_impl::OnAsyResponse_6001(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //判断错误信息
    CHundsunH5sdkRspInfoField RspInfo;
    //返回信息结构体
    CHundsunH5sdkRspTrendField RspTrend;

    //证券代码
    hs_strncpy(RspTrend.ProdCode, U2G(lpBody->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(RspTrend.ProdCode));
    //类型代码
    hs_strncpy(RspTrend.HqTypeCode, U2G(lpBody->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(RspTrend.HqTypeCode));
    //日期。格式YYYYMMDD
    RspTrend.Date = lpBody->GetItem(H5SDK_TAG_DATE)->GetInt32();
    //分时数据
    DealGroupAsyResponse_TrendTrendGrp(&RspTrend, lpBody->GetGroup(H5SDK_TAG_TREND_GRP));

    //回调
    lpH5sdkSpi->OnRspH5sdkTrend(&RspTrend, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_CandleByOffsetCandleGrp(CHundsunH5sdkRspCandleByOffsetField* lpRspCandleByOffset, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspCandleByOffsetCandleGrpField m_CandleGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //起始日期，0：表示当前时间
        m_CandleGrp.Date = lpRecord->GetItem(H5SDK_TAG_DATE)->GetInt32();
        //分钟K线的时间，HHMM
        m_CandleGrp.MinTime = lpRecord->GetItem(H5SDK_TAG_MIN_TIME)->GetInt32();
        //开盘价
        m_CandleGrp.OpenPx = lpRecord->GetItem(H5SDK_TAG_OPEN_PX)->GetInt32();
        //最高价
        m_CandleGrp.HighPx = lpRecord->GetItem(H5SDK_TAG_HIGH_PX)->GetInt32();
        //最低价
        m_CandleGrp.LowPx = lpRecord->GetItem(H5SDK_TAG_LOW_PX)->GetInt32();
        //收盘价
        m_CandleGrp.ClosePx = lpRecord->GetItem(H5SDK_TAG_CLOSE_PX)->GetInt32();
        //成交量
        m_CandleGrp.BusinessAmount = lpRecord->GetItem(H5SDK_TAG_BUSINESS_AMOUNT)->GetInt64();
        //成交额
        m_CandleGrp.BusinessBalance = lpRecord->GetItem(H5SDK_TAG_BUSINESS_BALANCE)->GetInt64();

        //塞入CHundsunH5sdkRspCandleByOffsetCandleGrpField数组
        lpRspCandleByOffset->CandleGrp.push_back(m_CandleGrp);
    }
}

//6002 按偏移取K线
void CHundsunH5sdkApi_impl::OnAsyResponse_6002(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //判断错误信息
    CHundsunH5sdkRspInfoField RspInfo;
    //返回信息结构体
    CHundsunH5sdkRspCandleByOffsetField RspCandleByOffset;

    //证券全代码
    hs_strncpy(RspCandleByOffset.ProdCode, U2G(lpBody->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(RspCandleByOffset.ProdCode));
    //类型代码
    hs_strncpy(RspCandleByOffset.HqTypeCode, U2G(lpBody->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(RspCandleByOffset.HqTypeCode));
    //k线类型
    RspCandleByOffset.CandlePeriod = lpBody->GetItem(H5SDK_TAG_CANDLE_PEROID)->GetInt32();
    //
    RspCandleByOffset.CandleMode = lpBody->GetItem(H5SDK_TAG_CANDLE_MODE)->GetInt8();
    //行情服务器是否还有数据 1表示还有数据，表示用户请求数据量大于目前支持最大的根数，需要用户根据返回结果再次请求
    RspCandleByOffset.AllDataFlag = lpBody->GetItem(HSSDK_TAG_ALL_DATA_FLAG)->GetInt8();
    //分时数据
    DealGroupAsyResponse_CandleByOffsetCandleGrp(&RspCandleByOffset, lpBody->GetGroup(H5SDK_TAG_CANDLE_GRP));

    //回调
    lpH5sdkSpi->OnRspH5sdkCandleByOffset(&RspCandleByOffset, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_CandleByRangeCandleGrp(CHundsunH5sdkRspCandleByRangeField* lpRspCandleByRange, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspCandleByRangeCandleGrpField m_CandleGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //起始日期，0：表示当前时间
        m_CandleGrp.Date = lpRecord->GetItem(H5SDK_TAG_DATE)->GetInt32();
        //
        m_CandleGrp.MinTime = lpRecord->GetItem(H5SDK_TAG_MIN_TIME)->GetInt32();
        //开盘价
        m_CandleGrp.OpenPx = lpRecord->GetItem(H5SDK_TAG_OPEN_PX)->GetInt32();
        //最高价
        m_CandleGrp.HighPx = lpRecord->GetItem(H5SDK_TAG_HIGH_PX)->GetInt32();
        //最低价
        m_CandleGrp.LowPx = lpRecord->GetItem(H5SDK_TAG_LOW_PX)->GetInt32();
        //收盘价
        m_CandleGrp.ClosePx = lpRecord->GetItem(H5SDK_TAG_CLOSE_PX)->GetInt32();
        //成交量
        m_CandleGrp.BusinessAmount = lpRecord->GetItem(H5SDK_TAG_BUSINESS_AMOUNT)->GetInt64();
        //成交额
        m_CandleGrp.BusinessBalance = lpRecord->GetItem(H5SDK_TAG_BUSINESS_BALANCE)->GetInt64();

        //塞入CHundsunH5sdkRspCandleByRangeCandleGrpField数组
        lpRspCandleByRange->CandleGrp.push_back(m_CandleGrp);
    }
}

//6003 按日期时间范围取K线
void CHundsunH5sdkApi_impl::OnAsyResponse_6003(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //判断错误信息
    CHundsunH5sdkRspInfoField RspInfo;
    //返回信息结构体
    CHundsunH5sdkRspCandleByRangeField RspCandleByRange;

    //证券代码
    hs_strncpy(RspCandleByRange.ProdCode, U2G(lpBody->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(RspCandleByRange.ProdCode));
    //类型代码
    hs_strncpy(RspCandleByRange.HqTypeCode, U2G(lpBody->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(RspCandleByRange.HqTypeCode));
    //k线类型
    RspCandleByRange.CandlePeriod = lpBody->GetItem(H5SDK_TAG_CANDLE_PEROID)->GetInt32();
    //
    RspCandleByRange.CandleMode = lpBody->GetItem(H5SDK_TAG_CANDLE_MODE)->GetInt8();
    //行情服务器是否还有数据 1表示还有数据
    RspCandleByRange.AllDataFlag = lpBody->GetItem(HSSDK_TAG_ALL_DATA_FLAG)->GetInt8();
    //分时数据
    DealGroupAsyResponse_CandleByRangeCandleGrp(&RspCandleByRange, lpBody->GetGroup(H5SDK_TAG_CANDLE_GRP));

    //回调
    lpH5sdkSpi->OnRspH5sdkCandleByRange(&RspCandleByRange, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_TickMinTickGrp(CHundsunH5sdkRspTickMinField* lpRspTickMin, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspTickMinTickGrpField m_TickGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //分笔序号
        m_TickGrp.BusinessNo = lpRecord->GetItem(H5SDK_TAG_BUSINESS_NO)->GetInt32();
        //时间戳
        m_TickGrp.BusinessTime = lpRecord->GetItem(H5SDK_TAG_BUSINESS_TIME)->GetInt32();
        //价格
        m_TickGrp.HqPx = lpRecord->GetItem(H5SDK_TAG_HQ_PRICE)->GetInt32();
        //成交量
        m_TickGrp.BusinessAmount = lpRecord->GetItem(H5SDK_TAG_BUSINESS_AMOUNT)->GetInt64();
        //成交额
        m_TickGrp.BusinessBalance = lpRecord->GetItem(H5SDK_TAG_BUSINESS_BALANCE)->GetInt64();
        //成交笔数
        m_TickGrp.BusinessCount = lpRecord->GetItem(H5SDK_TAG_BUSINESS_COUNT)->GetInt32();
        //成交方向（0：卖，1：买)
        m_TickGrp.BusinessDirection = lpRecord->GetItem(HSSDK_TAG_BUSINESS_DIRECTION)->GetInt32();

        //塞入CHundsunH5sdkRspTickMinTickGrpField数组
        lpRspTickMin->TickGrp.push_back(m_TickGrp);
    }
}

//6004 指定分钟的分笔
void CHundsunH5sdkApi_impl::OnAsyResponse_6004(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //判断错误信息
    CHundsunH5sdkRspInfoField RspInfo;
    //返回信息结构体
    CHundsunH5sdkRspTickMinField RspTickMin;

    //证券代码
    hs_strncpy(RspTickMin.ProdCode, U2G(lpBody->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(RspTickMin.ProdCode));
    //类型代码
    hs_strncpy(RspTickMin.HqTypeCode, U2G(lpBody->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(RspTickMin.HqTypeCode));
    //日期。格式YYYYMMDD
    RspTickMin.Date = lpBody->GetItem(H5SDK_TAG_DATE)->GetInt32();
    //起始分钟数
    RspTickMin.StartMin = lpBody->GetItem(H5SDK_TAG_START_MIN)->GetInt32();
    //结束分钟数
    RspTickMin.EndMin = lpBody->GetItem(H5SDK_TAG_END_MIN)->GetInt32();
    //行情服务器是否还有数据 1表示还有数据
    RspTickMin.AllDataFlag = lpBody->GetItem(HSSDK_TAG_ALL_DATA_FLAG)->GetInt8();
    //分时数据
    DealGroupAsyResponse_TickMinTickGrp(&RspTickMin, lpBody->GetGroup(H5SDK_TAG_TICK_GRP));

    //回调
    lpH5sdkSpi->OnRspH5sdkTickMin(&RspTickMin, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_TickDirectionTickGrp(CHundsunH5sdkRspTickDirectionField* lpRspTickDirection, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspTickDirectionTickGrpField m_TickGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //分笔序号
        m_TickGrp.BusinessNo = lpRecord->GetItem(H5SDK_TAG_BUSINESS_NO)->GetInt32();
        //时间戳
        m_TickGrp.BusinessTime = lpRecord->GetItem(H5SDK_TAG_BUSINESS_TIME)->GetInt32();
        //价格
        m_TickGrp.HqPx = lpRecord->GetItem(H5SDK_TAG_HQ_PRICE)->GetInt32();
        //成交量
        m_TickGrp.BusinessAmount = lpRecord->GetItem(H5SDK_TAG_BUSINESS_AMOUNT)->GetInt64();
        //成交额
        m_TickGrp.BusinessBalance = lpRecord->GetItem(H5SDK_TAG_BUSINESS_BALANCE)->GetInt64();
        //成交笔数
        m_TickGrp.BusinessCount = lpRecord->GetItem(H5SDK_TAG_BUSINESS_COUNT)->GetInt32();
        //成交方向（0：卖，1：买)
        m_TickGrp.BusinessDirection = lpRecord->GetItem(HSSDK_TAG_BUSINESS_DIRECTION)->GetInt32();

        //塞入CHundsunH5sdkRspTickDirectionTickGrpField数组
        lpRspTickDirection->TickGrp.push_back(m_TickGrp);
    }
}

//6005 按偏移取分笔
void CHundsunH5sdkApi_impl::OnAsyResponse_6005(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //判断错误信息
    CHundsunH5sdkRspInfoField RspInfo;
    //返回信息结构体
    CHundsunH5sdkRspTickDirectionField RspTickDirection;

    //证券代码
    hs_strncpy(RspTickDirection.ProdCode, U2G(lpBody->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(RspTickDirection.ProdCode));
    //类型代码
    hs_strncpy(RspTickDirection.HqTypeCode, U2G(lpBody->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(RspTickDirection.HqTypeCode));
    //日期:YYYYMMDD,0:表示今天
    RspTickDirection.Date = lpBody->GetItem(H5SDK_TAG_DATE)->GetInt32();
    //起始位置,默认为0，向前取时，0表示最近的一笔，向后取时0表示最早的一个分笔
    RspTickDirection.StartPos = lpBody->GetItem(H5SDK_TAG_START_POS)->GetInt32();
    //方向
    RspTickDirection.SearchDirection = lpBody->GetItem(H5SDK_TAG_DIRECTION)->GetInt8();
    //行情服务器是否还有数据 1表示还有数据
    RspTickDirection.AllDataFlag = lpBody->GetItem(HSSDK_TAG_ALL_DATA_FLAG)->GetInt8();
    //分时数据
    DealGroupAsyResponse_TickDirectionTickGrp(&RspTickDirection, lpBody->GetGroup(H5SDK_TAG_TICK_GRP));

    //回调
    lpH5sdkSpi->OnRspH5sdkTickDirection(&RspTickDirection, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_CandleByRangeMaintainCandleGrp(CHundsunH5sdkRspCandleByRangeMaintainField* lpRspCandleByRangeMaintain, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspCandleByRangeMaintainCandleGrpField m_CandleGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //起始日期，0：表示当前时间
        m_CandleGrp.Date = lpRecord->GetItem(H5SDK_TAG_DATE)->GetInt32();
        //时间
        m_CandleGrp.MinTime = lpRecord->GetItem(H5SDK_TAG_MIN_TIME)->GetInt32();
        //均价
        m_CandleGrp.WavgPx = lpRecord->GetItem(H5SDK_TAG_WAVG_PX)->GetInt32();
        //开盘价
        m_CandleGrp.OpenPx = lpRecord->GetItem(H5SDK_TAG_OPEN_PX)->GetInt32();
        //最高价
        m_CandleGrp.HighPx = lpRecord->GetItem(H5SDK_TAG_HIGH_PX)->GetInt32();
        //最低价
        m_CandleGrp.LowPx = lpRecord->GetItem(H5SDK_TAG_LOW_PX)->GetInt32();
        //收盘价
        m_CandleGrp.ClosePx = lpRecord->GetItem(H5SDK_TAG_CLOSE_PX)->GetInt32();
        //成交量
        m_CandleGrp.BusinessAmount = lpRecord->GetItem(H5SDK_TAG_BUSINESS_AMOUNT)->GetInt64();
        //成交额
        m_CandleGrp.BusinessBalance = lpRecord->GetItem(H5SDK_TAG_BUSINESS_BALANCE)->GetInt64();
        //
        m_CandleGrp.Amount = lpRecord->GetItem(H5SDK_TAG_AMOUNT)->GetInt64();

        //塞入CHundsunH5sdkRspCandleByRangeMaintainCandleGrpField数组
        lpRspCandleByRangeMaintain->CandleGrp.push_back(m_CandleGrp);
    }
}

//6006 补线使用_分钟k线
void CHundsunH5sdkApi_impl::OnAsyResponse_6006(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //判断错误信息
    CHundsunH5sdkRspInfoField RspInfo;
    //返回信息结构体
    CHundsunH5sdkRspCandleByRangeMaintainField RspCandleByRangeMaintain;

    //证券代码
    hs_strncpy(RspCandleByRangeMaintain.ProdCode, U2G(lpBody->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(RspCandleByRangeMaintain.ProdCode));
    //类型代码
    hs_strncpy(RspCandleByRangeMaintain.HqTypeCode, U2G(lpBody->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(RspCandleByRangeMaintain.HqTypeCode));
    //k线类型
    RspCandleByRangeMaintain.CandlePeriod = lpBody->GetItem(H5SDK_TAG_CANDLE_PEROID)->GetInt32();
    //
    RspCandleByRangeMaintain.CandleMode = lpBody->GetItem(H5SDK_TAG_CANDLE_MODE)->GetInt8();
    //行情服务器是否还有数据 1表示还有数据
    RspCandleByRangeMaintain.AllDataFlag = lpBody->GetItem(HSSDK_TAG_ALL_DATA_FLAG)->GetInt8();
    //分时数据
    DealGroupAsyResponse_CandleByRangeMaintainCandleGrp(&RspCandleByRangeMaintain, lpBody->GetGroup(H5SDK_TAG_CANDLE_GRP));

    //回调
    lpH5sdkSpi->OnRspH5sdkCandleByRangeMaintain(&RspCandleByRangeMaintain, &RspInfo, nRequestID, true);
}

inline void DealGroupAsyResponse_Day5TrendTrendGrp(CHundsunH5sdkRspDay5TrendField* lpRspDay5Trend, IGroup* lpGroup)
{
    if(!lpGroup)
        return;

    // 获取记录条数
    int cnt = lpGroup->GetRecordCount();

    IRecord* lpRecord = NULL;
    CHundsunH5sdkRspDay5TrendTrendGrpField m_TrendGrp;
    for (int i = 0; i < cnt; i++)
    {
        lpRecord = lpGroup->GetRecord(i);
        //日期:YYYYMMDD
        m_TrendGrp.Date = lpRecord->GetItem(H5SDK_TAG_DATE)->GetInt32();
        //分时时间hhmm
        m_TrendGrp.MinTime = lpRecord->GetItem(H5SDK_TAG_MIN_TIME)->GetInt32();
        //价格
        m_TrendGrp.HqPx = lpRecord->GetItem(H5SDK_TAG_HQ_PRICE)->GetInt32();
        //加权均价
        m_TrendGrp.WavgPx = lpRecord->GetItem(H5SDK_TAG_WAVG_PX)->GetInt32();
        //总成交量
        m_TrendGrp.BusinessAmount = lpRecord->GetItem(H5SDK_TAG_BUSINESS_AMOUNT)->GetInt64();
        //总成交额
        m_TrendGrp.BusinessBalance = lpRecord->GetItem(H5SDK_TAG_BUSINESS_BALANCE)->GetInt64();

        //塞入CHundsunH5sdkRspDay5TrendTrendGrpField数组
        lpRspDay5Trend->TrendGrp.push_back(m_TrendGrp);
    }
}

//6007 5日分时
void CHundsunH5sdkApi_impl::OnAsyResponse_6007(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //判断错误信息
    CHundsunH5sdkRspInfoField RspInfo;
    //返回信息结构体
    CHundsunH5sdkRspDay5TrendField RspDay5Trend;

    //证券代码
    hs_strncpy(RspDay5Trend.ProdCode, U2G(lpBody->GetItem(H5SDK_TAG_PROD_CODE)->GetString()), sizeof(RspDay5Trend.ProdCode));
    //类型代码
    hs_strncpy(RspDay5Trend.HqTypeCode, U2G(lpBody->GetItem(H5SDK_TAG_HQ_TYPE_CODE)->GetString()), sizeof(RspDay5Trend.HqTypeCode));
    //分时数据
    DealGroupAsyResponse_Day5TrendTrendGrp(&RspDay5Trend, lpBody->GetGroup(H5SDK_TAG_TREND_GRP));

    //回调
    lpH5sdkSpi->OnRspH5sdkDay5Trend(&RspDay5Trend, &RspInfo, nRequestID, true);
}

//7001 补线返回
void CHundsunH5sdkApi_impl::OnAsyResponse_7001(IRecord *lpBody, int nRequestID)
{
    IRecord* lpRecord = NULL;
    //判断错误信息
    CHundsunH5sdkRspInfoField RspInfo;
    //返回信息结构体
    CHundsunH5sdkRspMaintainReturnField RspMaintainReturn;


    //回调
    lpH5sdkSpi->OnRspH5sdkMaintainReturn(&RspMaintainReturn, &RspInfo, nRequestID, true);
}

