#pragma once

#include "stragegymanager.h"
#include "TradeInterface.h"
#include "HundsunMdCenterStruct.h"
#include <string>
#include "HundsunMdCenterApi.h"
#include "HundsunMdCenterApi.h"
#include "FunctionPub.h"
#include "MdSpiCTP.h"
#include "MdSpi.h"
#include "tool.h"


using std::string;



class CRequest
{

public:
	CRequest();
	~CRequest();


public:

	CStrategyManager *m_stgy;
	CTradeInterface  *m_tde;
	CStockInfo *m_code;

	void init(CStrategyManager *p1,CTradeInterface  *p2,CStockInfo *p3);

	void handle(string str);

	void subcode(string str);

	void unsubcode(string str);
};