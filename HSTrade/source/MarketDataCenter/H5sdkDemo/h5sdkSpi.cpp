#include "h5sdkSpi.h"

const char* GetHSExchagneType(const char* pszHqTypeCode)
{
	if(!pszHqTypeCode)
		return "";

	if(strcmp(pszHqTypeCode, "XSHG") == 0)
		return "SS";
	else if(strcmp(pszHqTypeCode, "XSHE") == 0)
		return "SZ";
	else if(strcmp(pszHqTypeCode, "CCFX") == 0)
		return "CFFE";
	else if(strcmp(pszHqTypeCode, "XSGE") == 0)
		return "SHFE";
	else if(strcmp(pszHqTypeCode, "XDCE") == 0)
		return "DCE";
	else if(strcmp(pszHqTypeCode, "XZCE") == 0)
		return "CZCE";
	else 
		return "";
}

const char* GetCandlePeriodName(int iCandlePeriod)
{
	switch (iCandlePeriod)
	{
	case 1:
		return "1分钟线";
		break;
	case 2:
		return "5分钟线";
		break;
	case 3:
		return "15分钟线";
		break;
	case 4:
		return "30分钟线";
		break;
	case 5:
		return "60分钟线";
		break;
	case 6:
		return "日线";
		break;
	case 7:
		return "周线";
		break;
	case 8:
		return "月线";
		break;
	case 9:
		return "年线";
		break;
	default:
		return "";
		break;
	}
}

bool CH5sdkSpi::IsErrorRspInfo(CHundsunH5sdkRspInfoField *pRspInfo)
{
	bool ret = ((pRspInfo) && (pRspInfo->ErrorNo != 0));
	if (ret)
	{
		printf("IsErrorRspInfo\n");
		cerr<<" 响应 | "<<pRspInfo->ErrorInfo<<endl;
	}
	return ret;
}

int CH5sdkSpi::ReqQryMarket()
{
	CHundsunH5sdkReqMarketReferenceField m_req;
	memset(&m_req, 0, sizeof(CHundsunH5sdkReqMarketReferenceField));
	hs_strncpy(m_req.HqTypeCode, "SS", sizeof(m_req.HqTypeCode));

	return lpUserApi->ReqH5sdkMarketReference(&m_req);
}

int CH5sdkSpi::ReqQryInstrument()
{
	CHundsunH5sdkReqMarketTypesField m_ReqMarketTypes;
	CHundsunH5sdkReqMarketTypesFinanceMicGrpField m_FinanceMicGrp;
	memset(&m_FinanceMicGrp, 0, sizeof(CHundsunH5sdkReqMarketTypesFinanceMicGrpField));
	hs_strncpy(m_FinanceMicGrp.FinanceMic, "SS", sizeof(m_FinanceMicGrp.FinanceMic));
	m_ReqMarketTypes.FinanceMicGrp.push_back(m_FinanceMicGrp);

	return lpUserApi->ReqH5sdkMarketTypes(&m_ReqMarketTypes);
}

int CH5sdkSpi::ReqH5sdkSnapshot()
{
	CHundsunH5sdkReqSnapshotField m_req;

	m_req.HqLevel = 1;

	CHundsunH5sdkReqSnapshotProdGrpField m_ProdGrp;
	hs_strncpy(m_ProdGrp.ProdCode, "600570", sizeof(m_ProdGrp.ProdCode));
	hs_strncpy(m_ProdGrp.HqTypeCode, "XSHG", sizeof(m_ProdGrp.HqTypeCode));
	m_req.ProdGrp.push_back(m_ProdGrp);

	CHundsunH5sdkReqSnapshotFieldGrpField m_FieldGrp;
	m_req.FieldGrp.push_back(m_FieldGrp);

	return lpUserApi->ReqH5sdkSnapshot(&m_req);
}

int CH5sdkSpi::ReqH5sdkSubscribe()
{
	CHundsunH5sdkReqSubscribeField m_req;

	m_req.HqLevel = 1;
	CHundsunH5sdkReqSubscribeProdGrpField m_ProdGrp;
	hs_strncpy(m_ProdGrp.ProdCode, "600570", sizeof(m_ProdGrp.ProdCode));
	hs_strncpy(m_ProdGrp.HqTypeCode, "XSHG", sizeof(m_ProdGrp.HqTypeCode));
	m_req.ProdGrp.push_back(m_ProdGrp);
	m_req.SubType = 1;

	return lpUserApi->ReqH5sdkSubscribe(&m_req);
}

int CH5sdkSpi::ReqH5sdkTrend()
{
	CHundsunH5sdkReqTrendField m_req;
	
	hs_strncpy(m_req.ProdCode, "600570", sizeof(m_req.ProdCode));
	hs_strncpy(m_req.HqTypeCode, "XSHG", sizeof(m_req.HqTypeCode));
	m_req.Date = 20160216;

	return lpUserApi->ReqH5sdkTrend(&m_req);
}

int CH5sdkSpi::ReqH5sdkCandleByRange()
{
	CHundsunH5sdkReqCandleByRangeField m_req;

	hs_strncpy(m_req.ProdCode, szProdCode, sizeof(m_req.ProdCode));
	hs_strncpy(m_req.HqTypeCode, szHqTypeCode, sizeof(m_req.HqTypeCode));
	m_req.CandlePeriod = iCandlePeriod;
	m_req.StartDate = atoi(szQueryDate);
	m_req.EndDate = atoi(szQueryDate);

	printf("查询[%s]\n", szQueryDate);

	return lpUserApi->ReqH5sdkCandleByRange(&m_req);
}

int CH5sdkSpi::ReqH5sdkTickMin()
{
	CHundsunH5sdkReqTickMinField m_req;
	hs_strncpy(m_req.ProdCode, "600570", sizeof(m_req.ProdCode));
	hs_strncpy(m_req.HqTypeCode, "XSHG", sizeof(m_req.HqTypeCode));
	m_req.Date = 0;
	//m_req.StartMin = 91500;
	//m_req.EndMin = 150000;
	m_req.DataCount = 240;

	return lpUserApi->ReqH5sdkTickMin(&m_req);
}

bool CH5sdkSpi::GetQueryStatus()
{
	return b_Query;
}

void CH5sdkSpi::SetQueryStatus(bool val)
{
	b_Query = val;
}

void CH5sdkSpi::SetHqTypeCode(const char* pszHqTypeCode)
{
	hs_strncpy(szHqTypeCode, pszHqTypeCode, sizeof(szHqTypeCode));
}

void CH5sdkSpi::SetProdCode(const char* pszProdCode)
{
	hs_strncpy(szProdCode, pszProdCode, sizeof(szProdCode));
}

void CH5sdkSpi::SetCandlePeriod(int val)
{
	iCandlePeriod = val;
}

void CH5sdkSpi::SetQueryDate(const char* pszDate)
{
	hs_strncpy(szQueryDate, pszDate, sizeof(szQueryDate));
}

void CH5sdkSpi::OnH5sdkFrontConnected()
{
	printf("连接成功...\n");
}

void CH5sdkSpi::OnH5sdkFrontDisconnected(int nReason)
{
	printf("连接断开...\n");
}

void CH5sdkSpi::OnRspH5sdkLogin(CHundsunH5sdkRspLoginField *pRspLogin, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printf("登录成功...\n");
}

void CH5sdkSpi::OnRspH5sdkMarketTypes(CHundsunH5sdkRspMarketTypesField *pRspMarketTypes, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	int ct = pRspMarketTypes->FinanceMicGrp.size();

	for(int i = 0; i < ct; i++)
	{
		CHundsunH5sdkRspMarketTypesFinanceMicGrpField* lpFinanceMicGrp = &pRspMarketTypes->FinanceMicGrp[i];
		
		cout << "交易所代码:" << lpFinanceMicGrp->FinanceMic << "交易所名称:" << lpFinanceMicGrp->FinanceName << endl;
		
		int ct_t = lpFinanceMicGrp->TypeGrp.size();
		for(int j = 0; j < ct_t; j++)
		{
			CHundsunH5sdkRspMarketTypesTypeGrpField* lpTypeGrp = &lpFinanceMicGrp->TypeGrp[j];

			cout << "分类名称:" << lpTypeGrp->HqTypeName << endl;
		}
	}
}

void CH5sdkSpi::OnRspH5sdkMarketReference(CHundsunH5sdkRspMarketReferenceField *pRspMarketReference, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	int ct = pRspMarketReference->ProdGrp.size();

	cout << "交易所名称:" << pRspMarketReference->FinanceName << endl;

	for(int i = 0; i < ct; i++)
	{
		CHundsunH5sdkRspMarketReferenceProdGrpField* lpReferenceProdGrp = &pRspMarketReference->ProdGrp[i];

		cout << "证券代码:" << lpReferenceProdGrp->ProdCode << "证券名称:" << lpReferenceProdGrp->ProdName << endl;
	}
}

void CH5sdkSpi::OnRspH5sdkSnapshot(CHundsunH5sdkRspSnapshotField *pRspSnapshot, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	int ct = pRspSnapshot->ProdGrp.size();

	for(int i = 0; i < ct; i++)
	{
		CHundsunH5sdkRspSnapshotProdGrpField* lpProdGrp = &pRspSnapshot->ProdGrp[i];

		cout << "证券代码:" << lpProdGrp->ProdCode << endl;

		int ct_bug = lpProdGrp->BidGrp.size();
		for(int j = 0; j < ct_bug; j++)
		{
			CHundsunH5sdkRspSnapshotBidGrpField* lpBidGrp = &lpProdGrp->BidGrp[j];

			cout << "委托价格:" << lpBidGrp->EntrustPx << "委托量:" << lpBidGrp->TotalEntrustAmount << endl;

			int ct_count = lpBidGrp->EntrustGrp.size();
			for(int k = 0; k < ct_count; k++)
			{
				cout << "委托订单列表:" << lpBidGrp->EntrustGrp[k].EntrustAmount << endl;
			}
		}
	}
}

void CH5sdkSpi::OnRspH5sdkSubscribe(CHundsunH5sdkRspSubscribeField *pRspSubscribe, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

void CH5sdkSpi::OnRspH5sdkTrend(CHundsunH5sdkRspTrendField *pRspTrend, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
}

void CH5sdkSpi::OnRspH5sdkCandleByRange(CHundsunH5sdkRspCandleByRangeField *pRspCandleByRange, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	char path[1024];
	char sMsg[1024];
	char HSExchangeType[255];

	hs_strncpy(HSExchangeType, GetHSExchagneType(pRspCandleByRange->HqTypeCode), sizeof(HSExchangeType));
	mkdirFullPath("./Data");
	sprintf(path,"./Data/%s.txt", pRspCandleByRange->ProdCode);
	FILE* fpCmd = fopen(path, "rb");
	if(!lpLog)
		lpLog = new CLogInfo(path);
	if(fpCmd == NULL)
	{
		sprintf(sMsg, "%s XXXX %s 前复权", pRspCandleByRange->ProdCode, GetCandlePeriodName(pRspCandleByRange->CandlePeriod));
		lpLog->AddLog(sMsg, LOGINFO_Save);
		if(pRspCandleByRange->CandlePeriod <= 5)
			lpLog->AddLog("      日期      时间	    开盘	    最高	    最低	    收盘	    成交量	    成交额", LOGINFO_Save);
		else
			lpLog->AddLog("      日期	    开盘	    最高	    最低	    收盘	    成交量	    成交额", LOGINFO_Save);
	}

	int ct = pRspCandleByRange->CandleGrp.size();
	for(int i = 0; i < ct; i++)
	{
		CHundsunH5sdkRspCandleByRangeCandleGrpField* lpCandleByRangeCandleGrp = &pRspCandleByRange->CandleGrp[i];

		if(pRspCandleByRange->CandlePeriod <= 5)
			sprintf(sMsg, "%d	%d	%0.2f	%0.2f	%0.2f	%0.2f	%d	%ld", lpCandleByRangeCandleGrp->Date, lpCandleByRangeCandleGrp->MinTime, 
				(double)lpCandleByRangeCandleGrp->OpenPx / 1000, (double)lpCandleByRangeCandleGrp->HighPx / 1000, (double)lpCandleByRangeCandleGrp->LowPx/ 1000, (double)lpCandleByRangeCandleGrp->ClosePx / 1000,
				(int)lpCandleByRangeCandleGrp->BusinessAmount / 100, lpCandleByRangeCandleGrp->BusinessBalance);
		else
			sprintf(sMsg, "%d	%0.2f	%0.2f	%0.2f	%0.2f	%d	%ld", lpCandleByRangeCandleGrp->Date,
				(double)lpCandleByRangeCandleGrp->OpenPx / 1000, (double)lpCandleByRangeCandleGrp->HighPx / 1000, (double)lpCandleByRangeCandleGrp->LowPx/ 1000, (double)lpCandleByRangeCandleGrp->ClosePx / 1000,
				(int)lpCandleByRangeCandleGrp->BusinessAmount / 100, lpCandleByRangeCandleGrp->BusinessBalance);

		lpLog->AddLog(sMsg, LOGINFO_Save);
	}

	date dTemp = from_undelimited_string(szQueryDate);
	CDate now;
	dTemp += days(1);
	SetQueryDate(to_iso_string(dTemp).c_str());
	if(atoi(to_iso_string(dTemp).c_str()) <= now.GetDate())
		ReqH5sdkCandleByRange();
	else
		SetQueryStatus(false);
}

void CH5sdkSpi::OnRspH5sdkTickMin(CHundsunH5sdkRspTickMinField *pRspTickMin, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

