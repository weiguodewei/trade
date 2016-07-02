#include "HundsunMdCenterApi.h"
#include "mdSpi.h"

int main()
{
	//H5 && Futu 一起上
	CHundsunMdCenterApi* lpUserMdApi = CHundsunMdCenterApi::CreateHundsunMdCenterApi();
	CMdSpi* lpUserMdSpi = new CMdSpi(lpUserMdApi);
	lpUserMdApi->RegisterSpi(lpUserMdSpi);

	//注册证券
	//lpUserMdApi->RegisterDBFPath("D:/dbf");
	lpUserMdApi->RegisterH5("h5apilicense.xml", "h5config_lv2.xml", true);

	//注册期货
	lpUserMdApi->RegisterFutu("license/yaqh_hs_sim_2.0/license.dat");
	lpUserMdApi->RegisterFront("115.238.53.140:9106");

	//初始化
	//lpUserMdApi->Init(HUNDSUN_FTDC_DBF);
	lpUserMdApi->Init(HUNDSUN_FTDC_H5);
	lpUserMdApi->Init(HUNDSUN_FTDC_Futu);

	char* Instrument[1];

	cerr<<"------------------------------------------------"<<endl;
	cerr<<" [1]  订阅证券                                  "<<endl;
	cerr<<" [2]  订阅期货                                  "<<endl;
	cerr<<" [0]  Exit                                      "<<endl;
	cerr<<"------------------------------------------------"<<endl;

	while(1)
	{
		int cmd;  
		cin>>cmd;
		switch(cmd){
		case 1:
			Instrument[0] = "600570";
			lpUserMdApi->SubscribeMarketData(HUNDSUN_FTDC_SH, Instrument, sizeof(Instrument)/sizeof(char*));
			break;
		case 2:
			Instrument[0] = "IF1511";
			lpUserMdApi->SubscribeMarketData(HUNDSUN_FTDC_CFFEX, Instrument, sizeof(Instrument)/sizeof(char*));
			break;
		case 0: exit(0);
		}
	}
	lpUserMdApi->ReleaseApi();

	return 0;
}