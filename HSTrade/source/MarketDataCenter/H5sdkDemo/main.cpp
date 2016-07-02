#include "h5sdkSpi.h"
#include "HundsunH5sdkApi.h"
#include "public.h"

void ShowList()
{
	cerr<<"------------------------------------------------"<<endl;
	cerr<<" [0]  显示目录                                  "<<endl;
	cerr<<"------------------------------------------------"<<endl;
	cerr<<" [1]  查询合约信息                              "<<endl;
	cerr<<" [2]  查询市场信息                              "<<endl;
	cerr<<" [3]  订阅行情                                  "<<endl;
	cerr<<" [10] 查询行情快照                              "<<endl;
	cerr<<" [20] 查询K线分钟                               "<<endl;
	cerr<<" [99]  Exit                                      "<<endl;
	cerr<<"------------------------------------------------"<<endl;
}

int main()
{
	char HqTypeCode[255];
	char ProdCode[255];
	char QueryDate[255];
	int CandlePeriod;
	char HSExchangeType[255];

	CHundsunH5sdkApi* lpUserApi = CHundsunH5sdkApi::CreateHundsunH5sdkApi();
	CH5sdkSpi* lpUserSpi = new CH5sdkSpi(lpUserApi);
	lpUserApi->RegisterSpi(lpUserSpi);
	lpUserApi->RegisterFront("112.124.211.5:9999");
	lpUserApi->Init();

	ShowList();

	while(1)
	{
		int cmd;  
		cin>>cmd;

		if(lpUserSpi->GetQueryStatus())
		{
			cerr<<"正在查询...别闹!"<<endl;
			continue;
		}

		switch(cmd){
			case 0:
				ShowList();
				break;
			case 1:
				lpUserSpi->ReqQryInstrument();
				break;
			case 2:
				lpUserSpi->ReqQryMarket();
				break;
			case 3:
				lpUserSpi->ReqH5sdkSubscribe();
				break;
			case 10:
				lpUserSpi->ReqH5sdkSnapshot();
				break;
			case 20:
				cerr<<" 警告：请不要在交易时间内导数据！ "<<endl;
				system_pause();

				cerr<<" 请输入交易所代码：HqTypeCode "<<endl;
				cerr<<"     证券市场：上证-->XSHG 深证-->XSHE"<<endl;
				cerr<<"     期货市场：中金-->CCFX 上期-->XSGE 大商-->XDCE 郑商-->XZCE"<<endl;
				cin>>HqTypeCode; 

				cerr<<" 请输入合约代码：ProdCode "<<endl;
				cin>>ProdCode; 

				cerr<<" 请输入K线类型：CandlePeriod "<<endl;
				cerr<<"     1分钟K线-->1 "<<endl;
				cerr<<"     5分钟K线-->2 "<<endl;
				cerr<<"     15分钟K线-->3 "<<endl;
				cerr<<"     30分钟K线-->4 "<<endl;
				cerr<<"     60分钟K线-->5 "<<endl;
				cerr<<"     日K线-->6 "<<endl;
				cerr<<"     周K线-->7 "<<endl;
				cerr<<"     月K线-->8 "<<endl;
				cerr<<"     年K线-->9 "<<endl;
				cin>>CandlePeriod; 

				cerr<<" 请输入开始查询日志：QueryDate（格式：19700101） "<<endl;
				cin>>QueryDate; 

				cerr<<" HqTypeCode = "<<HqTypeCode<<" ProdCode = "<<ProdCode<<" CandlePeriod = "<<CandlePeriod<<" QueryDate = "<<QueryDate<<endl;
				system_pause();

				lpUserSpi->SetHqTypeCode(HqTypeCode);
				lpUserSpi->SetProdCode(ProdCode);
				lpUserSpi->SetCandlePeriod(CandlePeriod);
				lpUserSpi->SetQueryDate(QueryDate);

				hs_strncpy(HSExchangeType, GetHSExchagneType(HqTypeCode), sizeof(HSExchangeType));

				cerr<<" 正在删除已有文件... "<<endl;
				char path[1024];
				mkdirFullPath("./Data");
				sprintf(path,"./Data/%s.txt", ProdCode);
				DeleteFile(path);

				cerr<<" 开始导数据... "<<endl;
				lpUserSpi->ReqH5sdkCandleByRange();
				lpUserSpi->SetQueryStatus(true);
				break;
			case 99: exit(0);
		}
	}

	lpUserApi->ReleaseApi();

	return 0;
}