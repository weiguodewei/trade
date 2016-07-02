#ifndef _H5API_CONET_DEFINE_H__
#define _H5API_CONET_DEFINE_H__

#include "h5apistddef.h"

//	股票交易状态
const int TRADE_STOP	=0;		//	停牌
const int TRADE_RUNNING =1;		//	正常交易

/**
	应用在使用下述的定义值时,请采用本文件定义的宏,不要自行采用数字
	以免后续开发期间我们更改宏定义时,应用程序需要相应修改
**/
////////////////////////	这里给出市场类别定义	//////////////////////////////
const int MKT_SH_L1	= 0x101;		//	上海level1
const int MKT_SH_L2	= 0x201;		//	上海level2
const int MKT_SZ_L1	= 0x102;		//	深圳level1
const int MKT_SZ_L2	= 0x202;		//	深圳level2
const int MKT_QH_CFFE =  0x303;		//	中金所期货
const int MKT_QH_SHFE  = 0x304;		//	上期
const int MKT_QH_DLCE  = 0x305;		//	大连商品交易所
const int MKT_QH_ZZCE  = 0x306;		//	郑州商品交易所
const int MKT_QH_ZHHY  = 0x30c;		//	组合合约
const int MKT_NEEQ		= 0x10b;		//	新三板（股转）
//20131126	caich	添加个股期权
const int MKT_GGQQ  = 0xd01;		//	上海个股期权
const int MKT_GGQQ_SZ  = 0xd02;		//	深圳个股期权
const int MKT_QQ_CFFE	=0x1103;		//中金所期权（股指期权）
const int MKT_QQ_SHFE	=0x1104	;	//上期期权
const int MKT_QQ_DLCE	=0x1105	;	//大连商品交易所期权
const int MKT_QQ_ZZCE	=0x1106	;	//郑州商品交易所期权
//hk
const int MKT_HK_INDEX =0x501;
const int MKT_HK_MAIN =0x502;
const int MKT_HK_GEM =0x503;
const int MKT_HK_NASD =0x504;
const int MKT_HK_ETS =0x505;
//2014820	czh	添加沪港通
const int MKT_HGT = 0x506;			//沪港通

////////////////////////	这里给出所有元数据编号定义(暂时未完全整理)	元数据类似于投资赢家协议中定义的数据结构
//	最基本的实时信息和k线(基财目前只需要关注以下6类消息即可)
const int RESULT_MARKET_OVERVIEW=0;	//	市场概要
const int RESULT_MARKET_INIINFO=1;	//	市场初始化信息
const int RESULT_TREND=2;			//	基本分时
const int RESULT_SIMPLE_QUOTE=3;	//	基本报价
const int RESULT_KLINE=4;			//	基本K线
const int RESULT_BIDOFF=5;			//	买卖档位信息
const int RESULT_NEEQ_ZS_LEVEL=6;	//	股转做市投资者档位行情
const int RESULT_NEEQ_ZS_ORDER=7;	//	股转做市做市商申报行情
const int RESULT_NEEQ_XY_ORDER=8;	//	股转协议转让申报行情
const int RESULT_MAX_COUNT=9;		//	最大RESULT个数

#endif
