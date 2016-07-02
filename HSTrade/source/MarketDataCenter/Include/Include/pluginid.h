///	框架插件
//	日志服务接口
#define SID_LOG				    	    "com.hundsun.fbase.log"
//	打包解报器接口(只有提供管理功能需要用到)
#define SID_F2PACKSVR				"com.hundsun.fbase.f2packsvr"
#define SID_F2PACKER				"com.hundsun.fbase.f2packer"
#define SID_F2UNPACKER				"com.hundsun.fbase.f2unpacker"
//	以下4个是配置服务接口
#define SID_RCFGOBJ			"com.hundsun.fbase.rcfgobj"
#define SID_WCFGOBJ			"com.hundsun.fbase.wcfgobj"
#define SID_CFGOBJMAKER		"com.hundsun.fbase.cfgobjmaker"
#define SID_CFGSVR			"com.hundsun.fbase.config"
//	容器接口(通过容器,可以获得所有的接口)
#define SID_H5_CORE  "com.hundsun.fbase.f2core"


////////////////	通用接口	/////////////////////
//	提供定时服务的插件接口名	动态库名:	fsc_timer
#define SID_TIMER	"com.hundsun.fbase.timer"
//	供定时器定时调用的接口
#define SID_TIMER_DEALER	"com.hundsun.fbase.timer_dealer"
//	消息服务接口(一般来说,所有服务类插件的主接口都是该接口,还可以额外提供其他接口提供特定服务)
#define SID_IHQMESSAGE_SERVICE	"com.hundsun.fbase.h5messageservice"
//	通信适配器	动态库名: fsc_hscomm
#define SID_HSCOMM_ADAPTER	"com.hundsun.fbase.commadapter"
#define SID_HSCOMM_SVR "com.hundsun.fbase.hscomm"				//	通信服务,该接口内部可以额外提供SID_HQCOMM_ADAPTER处理通信相关业务
//	消息工厂接口	库名:	fsc_hscommsg
#define SID_HSCOMM_MESSAGEFACTORY		"com.hundsun.fbase.hscommessagefactory"	



//	同步调用接口
#define SID_H5_SYN_CALL "com.hundsun.h5.syncall"
//      数据源初始化通知注册者
#define SID_H5D_INITNOTIFY_REGISTER "com.hundsun.h5ds.initnotify"

//	sid命名规则:
/*
	行情服务器和数据源服务器公用的插件:SID_H5_开头
	行情服务器:SID_H5HQ_	开头
	数据源:		 SID_H5DS_	开头
*/

////////////////	行情服务需要的插件	//////////////////
//	协议适配器接口	动态库名: h5_hsproto_adapter
#define SID_H5_PROTOCOL_ADAPTER	"com.hundsun.h5.protocoladapter"
// 恒生内部协议的解析服务
#define SID_H5_PROTO_SVR "com.hundsun.h5.protosvr"		//	额外提供SID_H5_PROTOCOL_ADAPTER接口

//	行情日志插件	库名: h5_quotelog
#define SID_H5_QUOTELOG	"com.hundsun.h5.quotelog"
//	行情日志插件接口	
#define SID_H5_QUOTELOG_INTERFACE	"com.hundsun.h5.quotelog.interface"


//	任务分派器接口	库名: h5_dispatcher
#define SID_H5_DISPATCHER	"com.hundsun.h5.dispatcher"
//	事件注册接口
#define SID_H5_REGISTER	"com.hundsun.h5.register"
//	缓存接口				库名: h5hq_cache
#define SID_H5HQ_CACHE "com.hundsun.h5hq.cache"

//	数据核心层接口	库名: h5_datacore
#define SID_H5_DATA_CORE		"com.hundsun.h5.data_core"

// L1 dcdp 					库名: h5hq_l1_dcdp
#define SID_H5HQ_L1_DCDP "com.hundsun.h5hq.l1_dcdp"
// L1 dcdp data query
#define SID_H5HQ_L1DCDP_QUERY "com.hundsun.h5hq.l1dcdp_query"
//L2  dcdp 					库名: h5hq_l2_dcdp
#define SID_H5HQ_L2_DCDP     "com.hundsun.h5hq.l2_dcdp"
//DDE dcdp
#define SID_H5HQ_DDE_DCDP     "com.hundsun.h5hq.dde_dcdp"
//DDE行情查询接口
#define SID_H5HQ_DDE_DCDP_QUERY   "com.hundsun.h5hq.Ddedcdp_query"		//add by hegh2011-11-10
//
#define SID_H5HQ_BLOCK_DCDP          "com.hundsun.h5hq.block_dcdp"
// 美股DCDP
#define SID_H5HQ_US_STOCK_DCDP   "com.hundsun.h5hq.usstock.dcdp"

//L2DCDP支持HQ4.0的行情查询接口
#define SID_H5HQ_L2_DCDP_QUERY   "com.hundsun.h5hq.l2dcdp_query"
//HKEx DCDP 库名：h5hq_hk_dcdp
#define SID_H5HQ_HK_DCDP	"com.hundsun.h5hq.hkex_dcdp"
//上海黄金DCDP
#define SID_H5HQ_SGE_DCDP	"com.hundsun.h5hq.sge_dcdp"
//HKEx 行情查询接口
#define SID_H5HQ_HK_DCDP_QUERY	"com.hundsun.h5hq.hkex_dcdp_query"
// 全球指数DCDP
#define SID_H5HQ_GLBINDEX_DCDP	"com.hundsun.h5hq.glbindex_dcdp"
// 外汇DCDP
#define SID_H5HQ_FX_DCDP	"com.hundsun.h5hq.fx_dcdp"
// 现货DCDP
#define SID_H5HQ_SPOT_DCDP	"com.hundsun.h5hq.spot_dcdp"

// 个股期权DCDP
#define SID_H5HQ_STOCK_OPTION_DCDP	"com.hundsun.h5hq.stock_option_dcdp"

// 期货期权DCDP
#define SID_H5HQ_FUTURES_OPTION_DCDP	"com.hundsun.h5hq.futures_option_dcdp"

// 银行间债券DCDP
#define SID_H5HQ_INTERBANK_DCDP	"com.hundsun.h5hq.interbank_dcdp"

//扩展指数市场
#define SID_H5HQ_EXT_DCDP	"com.hundsun.h5hq.ext_dcdp"

//外盘期货DCDP
#define SID_H5HQ_FRGNFUTURES_DCDP	"com.hundsun.h5hq.frgnfutures_dcdp"

//期货套利DCDP	by maxy
#define SID_H5HQ_ARBFUTURES_DCDP        "com.hundsun.h5hq.arbfutures_dcdp"	

// 短线精灵DCDP			库名:	h5hq_shortwizard_dcdp
#define SID_SHORTWIZARD_DCDP	"com.hundsun.h5hq.shortwizard_dcdp"

/// 每个行情服务器DCDP需要实现的接口
#define SID_H5HQ_DCDP_QUERY	"com.hundsun.h5hq.dcdp_query"


// BP插件内部提供业务处理的接口,	包括数据源BP也需要提供该接口					
#define SID_H5_BIZPROCESS	"com.hundsun.h5hq.bizprocess"
//	行情服务器的BP服务
#define SID_H5HQ_BIZ_SVR	"com.hundsun.h5hq.bizsvr"		//	库名	h5hq_bp

//	订阅接口				库名:	h5_subscribe
#define SID_H5_SUBSCRIBE	"com.hundsun.h5.subscribe"



// 监控协议适配器		库名:	h5_monitor
#define SID_H5_MONITOR "com.hundsun.h5.monitor"
// L2转L1适配器
#define SID_H5_L2TOL1_ADAPTER "com.hundsun.h5.l2tol1_adapter" // h5_l2tol1_adapter


// 权限处理插件			库名:	h5hq_auth
#define SID_H5HQ_AUTH "com.hundsun.h5hq.auth"		// 额外提供SID_H5HQ_AUTH_ADAPTER接口
// 权限处理适配接口
#define SID_H5HQ_AUTH_ADAPTER		"com.hundsun.h5hq.authadapter"

//数据源DCDP				库名:	h5ds_dcdp
#define SID_H5DS_DCDP    "com.hundsun.h5ds.dcdp"
//
#define SID_H5DS_DCDP_EXT    "com.hundsun.h5ds.dcdpext"


//数据源BP					库名:	h5ds_bp
#define SID_H5DS_BP                    "com.hundsun.h5ds.bizprocess"

//补线业务处理插件	库名:	h5hq_maintain_bp
#define SID_H5HQ_MAINTAIN_BP		"com.hundsun.h5hq.maintain_bp"

//补线数据加工处理插件	库名:	h5hq_maintain_dp
#define SID_H5HQ_MAINTAIN_DP		"com.hundsun.h5hq.maintain_dp"


///////	数据源各个适配器
//期货适配器插件 库名: h5ds_futures
#define SID_H5DS_FUTURES_ADAPTER "com.hundsun.h5ds.futures"					//	h5ds_futures
//期货适配器插件 库名: h5ds_futures
#define SID_H5DS_CFFEXL2_ADAPTER "com.hundsun.h5ds.cffexl2_adapter"	

//上海L1适配器插件
#define SID_H5DS_SHL1_ADAPTER    "com.hundsun.h5ds.shl1_adapter"		//	h5ds_shl1_adapter
//深圳L1适配器插件
#define SID_H5DS_SZL1_ADAPTER    "com.hundsun.h5ds.szl1_adapter"		//	h5ds_szl1_adapter
//上海L2适配器插件
#define SID_H5DS_SHL2_ADAPTER    "com.hundsun.h5ds.shl2_adapter"		//	h5ds_shl2_adapter
//深圳L2适配器插件
#define SID_H5DS_SZL2_ADAPTER    "com.hundsun.h5ds.szl2_adapter"		//	h5ds_szl2_adapter
//港股适配器
#define SID_H5DS_HK_ADAPTER      "com.hundsun.h5ds.hk_adapter"			//	h5ds_hk_adapter
//港股指数适配器
#define SID_H5DS_HKINDEX_JRJ_ADAPTER "com.hundsun.h5ds.hkindex_jrj_adapter"		//  h5ds_hkindex_jrj_adapter
//扩展行情适配器
#define SID_H5DS_EXTENDED_ADAPTER "com.hundsun.h5ds.extended_adapter"           //      h5ds_extended_adapter
//转发源适配器
#define SID_H5DS_FORWARD_ADAPTER  "com.hundsun.h5ds.forward_adapter"            //      h5ds_forward_adapter
//外盘期货适配器
#define SID_H5DS_FRGNFUTURES_ADAPTER "com.hundsun.h5ds.frgnfutures_adapter"		//  h5ds_frgnfutures_adapter
//OTC适配器
#define SID_H5DS_OTC_ADAPTER  "com.hundsun.h5ds.otc_adapter"

//深度分析插件
#define SID_DEPTH_ANALYSIS     "com.hundsun.h5.depth_analysis_adapter" 

// 期权适配器
#define SID_H5DS_OPTION_ADAPTER "com.hundsun.h5ds.option_adapter"

//DataFeed
#define SID_H5DS_DATA_FEED      "com.hundsun.h5ds.datafeed"

//TestPlugin
#define SID_TEST_PLUGIN      "com.hundsun.test_plugin"

// 银行间债券(inter-bank bond) 数据源 适配器
#define SID_H5DS_IBB_ADAPTER "com.hundsun.h5ds.ibb_adapter"

// 深圳文化产权交易所数据源适配器
#define SID_H5DS_CAE_ADAPTER "com.hundsun.h5ds.cae_adapter"

// 海西商品交易所数据源适配器
#define SID_H5DS_HAIXI_ADAPTER "com.hundsun.h5ds.haixi_adapter"

// 港交所 Orion Market Data Platform 数据源适配器
#define SID_H5DS_OMD_ADAPTER "com.hundsun.h5ds.omd_adapter"

//------------------- XINHUA08适配器SID ------ZXW---20110701---------
#define SID_H5DS_SHL1_ADAPTER_XH08      "com.hundsun.h5ds.shl1_adapter.xh08"
#define SID_H5DS_SZL1_ADAPTER_XH08      "com.hundsun.h5ds.szl1_adapter.xh08"
#define SID_H5DS_FURTURES_ADAPTER_XH08  "com.hundsun.h5ds.futures_adapter.xh08"
#define SID_H5DS_HK_ADAPTER_XH08        "com.hundsun.h5ds.hk_adapter.xh08"
#define SID_H5DS_GOLD_ADAPTER_XH08      "com.hundsun.h5ds.gold_adapter.xh08"
#define SID_H5DS_GLOBAL_INDEX_ADAPTER_XH08 "com.hundsun.h5ds.global_index_adapter.xh08"
#define SID_H5DS_FX_ADAPTER_XH08        "com.hundsun.h5ds.fx_adapter.xh08"

#define SID_H5DS_HONGHUI_ADAPTER        "com.hundsun.h5ds.honghui_adapter"
//现货适配器
#define SID_H5DS_SPOT_ADAPTER     "com.hundsun.h5ds.spot_adapter"


// DCDP插件	库名:	h5hq_dcdp_mngr
#define SID_H5HQ_DCDP_MANAGER "com.hundsun.h5hq.dcdp_manager"		//	h5hq_dcdpmanager 的主接口
#define SID_H5_DCDP_CORE	"com.hundsun.h5.dcdpcore"						//	供其他插件调用 各个DCDP 的功能
#define SID_H5_DCDP_DATA	"com.hundsun.h5.dcdpdata"						// dcdp容器数据查询接口接口	BP使用

// 行情中心
#define SID_H5HQ_QUOTA_CENTER "com.hundsun.h5hq.quota_center"
#define SID_H5HQ_QUOTA_SERVICE "com.hundsun.h5hq.quota_service"





//其他插件
//压力和性能测试插件 库名为:h5test_hqtest
#define SID_H5TEST_HQTEST "com.hundsun.h5test.hqtest"
//Level2数据挖掘插件（目前由何光红测试使用，当前的正式发布版本不包括此插件）
#define SID_H5_DATA_ANALYSE_DP          "com.hundsun.h5.analyse_dp"
#define SID_H5_DATA_ANALYSE_QUERY   "com.hundsun.hq5.analyse_dp_query"


