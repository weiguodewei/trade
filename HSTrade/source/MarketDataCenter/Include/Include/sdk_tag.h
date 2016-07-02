/**
 *源程序名称:sdk_tag.h
 *软件著作权:恒生电子股份有限公司
 *系统名称:H5行情协议
 *模块名称:H5行情协议
 *功能说明:定义H5行情协议中的字段TAG
 *作    者: 彭小豪
 *开发日期: 2016-01-27 22:16:31
 *版 本 号: 1.0.0.1
 *备    注: 根据文档《H5行情服务协议(修订版).xls》生成
 */

#ifndef __H5_SDK_TAG_H__
#define __H5_SDK_TAG_H__

#include "hscomm_message_interface.h"

#define MESSAGE_TEMPLATE_VERSION	"1.0.0.1"


/*****************功能消息域定义*******************/
const int H5PROTO_HEAD_INFO = 0 ; // 
const int H5SDK_MSG_LOGIN = 1001 ; // 登入请求
const int H5SDK_MSG_LOGOUT = 1002 ; // 登出信息
const int H5SDK_MSG_HEARTBEAT = 1003 ; // 心跳
const int H5SDK_SERVER_INFO = 1004 ; // 请求服务器信息
const int H5SDK_MSG_BATCH = 1006 ; // 批处理
const int H5SDK_MSG_FILE = 2001 ; // 请求静态文件
const int H5SDK_MSG_MARKET_TYPES = 2002 ; // 市场分类信息
const int H5SDK_MSG_MARKET_REFERENCE = 2003 ; // 市场代码表
const int H5SDK_MSG_SNAPSHOT = 3001 ; // 行情快照
const int H5SDK_KEYBOARD_WIZARD = 3002 ; // 键盘精灵消息
const int H5SDK_MSG_SUBSCRIBE = 4001 ; // 行情快照订阅
const int H5SDK_MSG_SORT = 5001 ; // 排序请求
const int H5SDK_MSG_TREND = 6001 ; // 按指定的日期或偏移取分时数据
const int H5SDK_MSG_CANDLE_BY_OFFSET = 6002 ; // 按偏移取K线
const int H5SDK_MSG_CANDLE_BY_RANGE = 6003 ; // 按日期时间范围取K线
const int H5SDK_MSG_TICK_MIN = 6004 ; // 指定分钟的分笔
const int H5SDK_MSG_TICK_DIRECTION = 6005 ; // 按偏移取分笔
const int H5SDK_MSG_CANDLE_BY_RANGE_MAINTAIN = 6006 ; // 补线使用_分钟k线
const int H5SDK_MSG_DAY5_TREND = 6007 ; // 5日分时
const int H5SDK_MAINTAIN_RETURN = 7001 ; // 补线返回


/*****************系统域定义*******************/

/// 数据类型：uint32(枚举); 域名：业务prod_code,用以找合适的模板
const int H5PROTO_TAG_BUSINESS_ID = 1 ;
    const uint32 BIZ_SYSTEM = 0 ; // 系统模板
    const uint32 BIZ_H5HQ = 1 ; // 行情模板
    const uint32 BIZ_UFX = 2 ; // UFX模板
    const uint32 BIZ_H5PROTO = 3 ; // H5行情服务协议

/// 数据类型：uint32(枚举); 域名：报文类型
const int H5PROTO_TAG_PACKET_TYPE = 3 ;
    const uint32 REQUEST = 0 ; // 请求
    const uint32 ANSWER = 1 ; // 应答
    const uint32 PUSH = 2 ; // 主推报文

/// 数据类型：uint32; 域名：功能号
const int H5PROTO_TAG_FUNCTION_ID = 5 ;

/// 数据类型：rawdata; 域名：客户端KEY
const int H5PROTO_TAG_USER_KEY = 7 ;

/// 数据类型：rawdata; 域名：会话号
const int H5PROTO_TAG_SESSION_ID = 8 ;

/// 数据类型：uint32; 域名：错误号
const int H5PROTO_TAG_ERROR_NO = 20016 ;

/// 数据类型：bytevector; 域名：错误信息
const int H5PROTO_TAG_ERROR_INFO = 20017 ;

/// 数据类型：array; 数组项缺省类型：uint32; 域名：发送者的标识信息,表示信息从哪个适配器的哪个通道的哪个连接获得,一般通信适配器在接收到消息后,负责IDX_CONNECTID和IDX_CHANNEL_INDEX这两个成员的
const int H5PROTO_TAG_SEND_INFO_ARRAY = 100 ;

/// 数据类型：array; 数组项缺省类型：uint32; 域名：处理时间，计算异步返回时间
const int H5PROTO_TAG_TIME_STAMP_ARRAY = 52 ;

/// 数据类型：uint32; 域名：批处理号
const int H5PROTO_TAG_BATCH_NO = 21 ;

/// 数据类型：uint32; 域名：同步调用编号
const int H5PROTO_TAG_SYNC_NO = 22 ;

/// 数据类型：rawdata; 域名：流水信息
const int H5PROTO_TAG_SERIAL_INFO = 23 ;


/*****************消息域定义*******************/

/// 数据类型：string; 域名：用户名
const int H5SDK_TAG_USER_NAME = 20001 ;

/// 数据类型：string; 域名：密码
const int H5SDK_TAG_PASSWORD = 20002 ;

/// 数据类型：string; 域名：动态密码
const int H5SDK_TAG_DYNMIC_PASSWORD = 20003 ;

/// 数据类型：bytevector; 域名：原始数据
const int H5SDK_TAG_ORGINAL_DATA = 96 ;

/// 数据类型：uint32; 域名：心跳间隔
const int H5SDK_TAG_HEARTBEAT_INTERVAL = 108 ;

/// 数据类型：uint32; 域名：SDK版本
const int H5SDK_TAG_SDK_VERSION = 20005 ;

/// 数据类型：bytevector; 域名：操作系统版本
const int H5SDK_TAG_OS_VERSION = 20006 ;

/// 数据类型：bytevector; 域名：服务器名称
const int H5SDK_TAG_SERVER_NAME = 20007 ;

/// 数据类型：uint64; 域名：服务器时间。Unix时间戳
const int H5SDK_TAG_SERVER_TIME = 20008 ;

/// 数据类型：uint32; 域名：当前在线
const int H5SDK_TAG_CURR_ONLINE_COUNT = 20009 ;

/// 数据类型：uint32; 域名：最大在线
const int H5SDK_TAG_MAX_ONLINE_COUNT = 20010 ;

/// 数据类型：uint32(枚举); 域名：文件类型
const int H5SDK_TAG_HQ_FILE_TYPE = 20011 ;
    const uint32 H5SDK_ENUM_FT_COMMON_FILE = 0 ; // 普通文件。需要使用文件名、相对路径信息进行访问
    const uint32 H5SDK_ENUM_FT_FINDATA_FILE = 1 ; // 财务数据文件
    const uint32 H5SDK_ENUM_FT_EXRIGHT_FILE = 2 ; // 除权数据文件
    const uint32 H5SDK_ENUM_FT_INFO_CONFIG_FILE = 3 ; // 资讯配置文件
    const uint32 H5SDK_ENUM_FT_WELCOME_FILE = 4 ; // 欢迎文件
    const uint32 H5SDK_ENUM_FT_DYNAMIC_NEWS_FILE = 5 ; // 流动条信息文件
    const uint32 H5SDK_ENUM_FT_SYS_BLOCK_FILE = 6 ; // 系统板块文件
    const uint32 H5SDK_ENUM_FT_USR_BLOCK_FILE = 7 ; // 自定义板块文件
    const uint32 H5SDK_ENUM_FT_BLOCK_CODE_FILE = 8 ; // 板块组织关系文件
    const uint32 H5SDK_ENUM_FT_MARKET_MONITOR_CONFIG_FILE = 9 ; // 短线精灵配置文件
    const uint32 H5SDK_ENUM_FT_CALL_AUCTION_TIME_FILE = 10 ; // 集合竞价时段配置文件

/// 数据类型：bytevector; 域名：文件名称
const int H5SDK_TAG_HQ_FILE_NAME = 16001 ;

/// 数据类型：uint64; 域名：文件偏移
const int H5SDK_TAG_FILE_OFFSET = 20012 ;

/// 数据类型：uint32; 域名：文件长度
const int H5SDK_TAG_FILE_LENGTH = 20013 ;

/// 数据类型：uint32; 域名：成交时间
const int H5SDK_TAG_BUSINESS_TIME = 20014 ;

/// 数据类型：uint32; 域名：数据CRC
const int H5SDK_TAG_CRC = 20015 ;

/// 数据类型：uint32; 域名：错误号
const int H5SDK_TAG_ERROR_NO = 20016 ;

/// 数据类型：bytevector; 域名：错误描述字符串
const int H5SDK_TAG_ERROR_INFO = 20017 ;

/// 数据类型：bytevector; 域名：交易所代码
const int H5SDK_TAG_FINANCE_MIC = 20018 ;

/// 数据类型：bytevector; 域名：交易所名称
const int H5SDK_TAG_FINANCE_NAME = 20019 ;

/// 数据类型：uint32; 域名：市场日期
const int H5SDK_TAG_MARKET_DATE = 230 ;

/// 数据类型：uint32; 域名：交易日期
const int H5SDK_TAG_INIT_DATE = 75 ;

/// 数据类型：int32; 域名：时区
const int H5SDK_TAG_TIMEZONE = 20020 ;

/// 数据类型：uint8; 域名：夏令时标志
const int H5SDK_TAG_DST_FLAG = 20021 ;

/// 数据类型：sequence; 域名：类型重复组
const int H5SDK_TAG_TYPE_GRP = 20022 ;

/// 数据类型：sequence; 域名：排序类型重复组
const int H5SDK_TAG_SORT_TYPE_GRP = 20071 ;

/// 数据类型：bytevector; 域名：类型代码
const int H5SDK_TAG_HQ_TYPE_CODE = 167 ;

/// 数据类型：bytevector; 域名：类型名称
const int H5SDK_TAG_HQ_TYPE_NAME = 20023 ;

/// 数据类型：uint32; 域名：价格放大倍数
const int H5SDK_TAG_PRICE_SCALE = 20024 ;

/// 数据类型：sequence; 域名：交易时间段
const int H5SDK_TAG_TRADE_SECTION_GRP = 20025 ;

/// 数据类型：uint32; 域名：开市时间
const int H5SDK_TAG_OPEN_TIME = 20026 ;

/// 数据类型：uint32; 域名：闭市时间
const int H5SDK_TAG_CLOSE_TIME = 20027 ;

/// 数据类型：uint32; 域名：昨收价
const int H5SDK_TAG_PRECLOSE_PX = 140 ;

/// 数据类型：uint32; 域名：涨停价格
const int H5SDK_TAG_UP_PRICE = 20028 ;

/// 数据类型：uint32; 域名：跌停价格
const int H5SDK_TAG_DOWN_PRICE = 20029 ;

/// 数据类型：uint8(枚举); 域名：行情等级
const int H5SDK_TAG_HQ_LEVEL = 61 ;
    const uint8 H5SDK_ENUM_LEVEL_1 = 1 ; // 基础行情
    const uint8 H5SDK_ENUM_LEVEL_2 = 2 ; // 2级行情

/// 数据类型：sequence; 域名：股票集重复组
const int H5SDK_TAG_PROD_GRP = 20030 ;

/// 数据类型：sequence; 域名：排序股票集重复组
const int H5SDK_TAG_SORT_PROD_GRP = 20070 ;

/// 数据类型：bytevector; 域名：股票代码
const int H5SDK_TAG_PROD_CODE = 48 ;

/// 数据类型：string; 域名：行业代码
const int H5SDK_TAG_INDUSTRY_CODE = 20031 ;

/// 数据类型：string; 域名：货币
const int H5SDK_TAG_MONEY_TYPE = 20032 ;

/// 数据类型：uint32; 域名：时间戳
const int H5SDK_TAG_DATA_TIMESTAMP = 10178 ;

/// 数据类型：string; 域名：交易状态
const int H5SDK_TAG_TRADE_STATUS = 10135 ;

/// 数据类型：uint32; 域名：开盘价
const int H5SDK_TAG_OPEN_PX = 10018 ;

/// 数据类型：uint32; 域名：最新价
const int H5SDK_TAG_LAST_PX = 31 ;

/// 数据类型：uint32; 域名：最高价
const int H5SDK_TAG_HIGH_PX = 332 ;

/// 数据类型：uint32; 域名：最低价
const int H5SDK_TAG_LOW_PX = 333 ;

/// 数据类型：uint32; 域名：收盘价
const int H5SDK_TAG_CLOSE_PX = 10204 ;

/// 数据类型：uint32; 域名：平均价
const int H5SDK_TAG_AVG_PX = 49 ;

/// 数据类型：uint32; 域名：加权平均价
const int H5SDK_TAG_WAVG_PX = 50 ;

/// 数据类型：uint32; 域名：成交笔数
const int H5SDK_TAG_BUSINESS_COUNT = 8503 ;

/// 数据类型：uint64; 域名：成交量
const int H5SDK_TAG_BUSINESS_AMOUNT = 387 ;

/// 数据类型：uint64; 域名：成交额
const int H5SDK_TAG_BUSINESS_BALANCE = 8504 ;

/// 数据类型：uint64; 域名：现手
const int H5SDK_TAG_CURRENT_AMOUNT = 30 ;

/// 数据类型：uint64; 域名：内盘成交量
const int H5SDK_TAG_BUSINESS_AMOUNT_IN = 20033 ;

/// 数据类型：uint64; 域名：外盘成交量
const int H5SDK_TAG_BUSINESS_AMOUNT_OUT = 20034 ;

/// 数据类型：uint64; 域名：总委买量
const int H5SDK_TAG_TOTAL_BUY_AMOUNT = 10043 ;

/// 数据类型：uint64; 域名：总委卖量
const int H5SDK_TAG_TOTAL_SELL_AMOUNT = 10044 ;

/// 数据类型：uint32; 域名：加权平均委买价
const int H5SDK_TAG_WAVG_BID_PX = 10039 ;

/// 数据类型：uint32; 域名：加权平均委卖价
const int H5SDK_TAG_WAVG_OFFER_PX = 10040 ;

/// 数据类型：sequence; 域名：委买档位
const int H5SDK_TAG_BID_GRP = 10068 ;

/// 数据类型：sequence; 域名：委卖档位
const int H5SDK_TAG_OFFER_GRP = 10069 ;

/// 数据类型：uint32; 域名：价格
const int H5SDK_TAG_HQ_PRICE = 44 ;

/// 数据类型：uint64; 域名：委托量
const int H5SDK_TAG_TOTAL_ENTRUST_AMOUNT = 39 ;

/// 数据类型：sequence; 域名：委托单重复组
const int H5SDK_TAG_ENTRUST_GRP = 73 ;

/// 数据类型：uint32; 域名：委托价格
const int H5SDK_TAG_ENTRUST_PX = 20067 ;

/// 数据类型：uint32; 域名：委托单量
const int H5SDK_TAG_ENTRUST_AMOUNT = 38 ;

/// 数据类型：uint32; 域名：52周最低价
const int H5SDK_TAG_WEEK52_LOW_PX = 20035 ;

/// 数据类型：uint32; 域名：52周最高价
const int H5SDK_TAG_WEEK52_HIGH_PX = 20036 ;

/// 数据类型：int32; 域名：价格涨跌
const int H5SDK_TAG_PX_CHANGE = 20037 ;

/// 数据类型：int32; 域名：涨跌幅
const int H5SDK_TAG_PX_CHANGE_RATE = 20038 ;

/// 数据类型：uint32; 域名：盘前/盘后价格
const int H5SDK_TAG_POPC_PX = 20039 ;

/// 数据类型：uint64; 域名：当前阶段的成交量
const int H5SDK_TAG_SESSION_VOLUMUE = 20040 ;

/// 数据类型：uint32; 域名：日期: YYYYMMDD，0：表示当前日期
const int H5SDK_TAG_DATE = 20041 ;

/// 数据类型：int32; 域名：日期偏移天数
const int H5SDK_TAG_DATE_OFFSET = 20042 ;

/// 数据类型：sequence; 域名：分时数据重复组
const int H5SDK_TAG_TREND_GRP = 20043 ;

/// 数据类型：uint32(枚举); 域名：K线周期
const int H5SDK_TAG_CANDLE_PEROID = 20044 ;
    const uint32 H5SDK_ENUM_PEROID_1MIN = 1 ; // 一分钟
    const uint32 H5SDK_ENUM_PEROID_5MIN = 2 ; // 5分钟
    const uint32 H5SDK_ENUM_PEROID_15MIN = 3 ; // 15分钟
    const uint32 H5SDK_ENUM_PEROID_30MIN = 4 ; // 30分钟
    const uint32 H5SDK_ENUM_PEROID_60MIN = 5 ; // 60分钟
    const uint32 H5SDK_ENUM_PEROID_DAY = 6 ; // 日线
    const uint32 H5SDK_ENUM_PEROID_WEEK = 7 ; // 周线
    const uint32 H5SDK_ENUM_PEROID_MONTH = 8 ; // 月线
    const uint32 H5SDK_ENUM_PEROID_YEAR = 9 ; // 年线

/// 数据类型：uint8(枚举); 域名：K线搜索方向
const int H5SDK_TAG_DIRECTION = 20046 ;
    const uint8 H5SDK_ENUM_FORWARD = 1 ; // 向前
    const uint8 H5SDK_ENUM_BACKWARD = 2 ; // 向后

/// 数据类型：uint32; 域名：数据个数
const int H5SDK_TAG_DATA_COUNT = 20047 ;

/// 数据类型：uint32; 域名：起始日期,YYYYMMDD
const int H5SDK_TAG_START_DATE = 20048 ;

/// 数据类型：uint32; 域名：结束日期,YYYYMMDD
const int H5SDK_TAG_END_DATE = 20049 ;

/// 数据类型：sequence; 域名：K线重复组
const int H5SDK_TAG_CANDLE_GRP = 20050 ;

/// 数据类型：bytevector; 域名：访问TOKEN
const int H5SDK_TAG_HQ_TOKEN = 20051 ;

/// 数据类型：sequence; 域名：交易所重复组
const int H5SDK_TAG_FINANCE_GRP = 20052 ;

/// 数据类型：bytevector; 域名：退出原因
const int H5SDK_TAG_QUIT_REASON = 20053 ;

/// 数据类型：sequence; 域名：批
const int H5SDK_TAG_BATCH_GRP = 20055 ;

/// 数据类型：rawdata; 域名：子包
const int H5SDK_TAG_SUB_PACKET = 20056 ;

/// 数据类型：sequence; 域名：字段集重复组
const int H5SDK_TAG_FIELD_GRP = 20057 ;

/// 数据类型：bytevector; 域名：字段名称
const int H5SDK_TAG_FIELD_NAME = 20058 ;

/// 数据类型：bytevector; 域名：证券名称
const int H5SDK_TAG_PROD_NAME = 55 ;

/// 数据类型：uint32; 域名：委托单数
const int H5SDK_TAG_ENTRUST_COUNT = 10067 ;

/// 数据类型：uint32; 域名：交易分钟数
const int H5SDK_TAG_TRADE_MINS = 20059 ;

/// 数据类型：int32; 域名：交易阶段
const int H5SDK_TAG_TRADE_SECTION = 20060 ;

/// 数据类型：uint8(枚举); 域名：K线模式
const int H5SDK_TAG_CANDLE_MODE = 20061 ;
    const uint8 H5SDK_ENUM_CANDLE_ORIGINAL = 0 ; // 原始K线
    const uint8 H5SDK_ENUM_CANDLE_FORWARD = 1 ; // 前复权K线
    const uint8 H5SDK_ENUM_CANDLE_BACKWARD = 2 ; // 后复权K线

/// 数据类型：uint32; 域名：最大值
const int H5SDK_TAG_MAX_VALUE = 20062 ;

/// 数据类型：uint64; 域名：总股本
const int H5SDK_TAG_TOTAL_SHARES = 20063 ;

/// 数据类型：int32; 域名：市盈率
const int H5SDK_TAG_PE_RATE = 20064 ;

/// 数据类型：sequence; 域名：市场重复组
const int H5SDK_FINANCE_MIC_GRP = 30065 ;

/// 数据类型：uint32(枚举); 域名：订阅还是退订, 如果是退订,允许SubscriberKey为空,表示该连接的所有已订阅信息都取消
const int H5SDK_TAG_SUB_TYPE = 13 ;
    const uint32 HSUB_ENUM_SUB_OVER = 0 ; // 覆盖订阅(当前客户端的订阅模式)
    const uint32 HSUB_ENUM_SUB = 1 ; // 追加订阅
    const uint32 HSUB_ENUM_UNSUB = 2 ; // 退订

/// 数据类型：uint32; 域名：起始位置
const int H5SDK_TAG_START_POS = 50002 ;

/// 数据类型：bytevector; 域名：字段名称
const int H5SDK_TAG_SORT_FIELD_NAME = 20066 ;

/// 数据类型：int32; 域名：量比
const int H5SDK_TAG_VOL_RATIO = 34 ;

/// 数据类型：int32; 域名：振幅
const int H5SDK_TAG_AMPLITUDE = 46 ;

/// 数据类型：int64; 域名：持仓量
const int H5SDK_TAG_AMOUNT = 95 ;

/// 数据类型：int32; 域名：换手率
const int H5SDK_TAG_TURNOVER_RATIO = 97 ;

/// 数据类型：int32; 域名：委比
const int H5SDK_TAG_ENTRUST_RATE = 99 ;

/// 数据类型：int64; 域名：委差
const int H5SDK_TAG_ENTRUST_DIFF = 101 ;

/// 数据类型：int8(枚举); 域名：排序方式
const int H5SDK_TAG_SORT_TYPE = 102 ;
    const int8 HSUB_ENUM_SORT_ASC = 0 ; // 升序
    const int8 HSUB_ENUM_SORT_DESC = 1 ; // 降序

/// 数据类型：bytevector; 域名：MIC_ABBR
const int H5SDK_TAG_MIC_ABBR = 103 ;

/// 数据类型：uint32; 域名：起始分钟数
const int H5SDK_TAG_START_MIN = 104 ;

/// 数据类型：uint32; 域名：介绍分钟数
const int H5SDK_TAG_END_MIN = 105 ;

/// 数据类型：sequence; 域名：分笔数据重复组
const int H5SDK_TAG_TICK_GRP = 106 ;

/// 数据类型：uint32; 域名：分笔序号
const int H5SDK_TAG_BUSINESS_NO = 107 ;

/// 数据类型：int32; 域名：成交方向
const int HSSDK_TAG_BUSINESS_DIRECTION = 109 ;

/// 数据类型：int32; 域名：每手股数
const int HSSDK_TAG_SHARES_PER_HAND = 72 ;

/// 数据类型：int32; 域名：价格精度
const int HSSDK_TAG_PX_PRECISION = 71 ;

/// 数据类型：int64; 域名：流通股本
const int HSSDK_TAG_CIRCULATION_AMOUNT = 110 ;

/// 数据类型：int64; 域名：市值
const int HSSDK_TAG_MARKET_VALUE = 91 ;

/// 数据类型：int64; 域名：流通市值
const int HSSDK_TAG_CIRCULATION_VALUE = 111 ;

/// 数据类型：int32; 域名：每股股收益
const int HSSDK_TAG_EPS = 112 ;

/// 数据类型：int32; 域名：每股净资产
const int HSSDK_TAG_BPS = 94 ;

/// 数据类型：int32; 域名：市净率
const int HSSDK_TAG_DYN_PB_RATE = 45 ;

/// 数据类型：int32; 域名：财务季度
const int HSSDK_TAG_FIN_QUARTER = 113 ;

/// 数据类型：int32; 域名：财务截至日期
const int HSSDK_TAG_FIN_END_DATE = 114 ;

/// 数据类型：int8; 域名：数据是否获取完全
const int HSSDK_TAG_ALL_DATA_FLAG = 115 ;

/// 数据类型：int32; 域名：上涨家数
const int HSSDK_TAG_RISE_COUNT = 124 ;

/// 数据类型：int32; 域名：下跌家数
const int HSSDK_TAG_FALL_COUNT = 125 ;

/// 数据类型：int32; 域名：成员个数
const int HSSDK_TAG_MEMBER_COUNT = 126 ;

/// 数据类型：sequence; 域名：领涨股
const int H5SDK_TAG_RISE_FIRST_GRP = 1009 ;

/// 数据类型：sequence; 域名：领跌股
const int H5SDK_TAG_FALL_FIRST_GRP = 1010 ;

/// 数据类型：string; 域名：时区码
const int H5SDK_TAG_TIMEZONE_CODE = 128 ;

/// 数据类型：uint32; 域名：分钟时间
const int H5SDK_TAG_MIN_TIME = 10014 ;

/// 数据类型：uint32; 域名：排序字段ID
const int H5SDK_TAG_SORT_FIELD_ID = 20068 ;

/// 数据类型：uint32; 域名：字段id
const int H5SDK_TAG_FIELD_ID = 20069 ;

/// 数据类型：int64; 域名：昨持仓（期货、期权）
const int HSSDK_TAG_PREV_AMOUNT = 122 ;

/// 数据类型：int64; 域名：日增持（期货、期权）
const int HSSDK_TAG_AMOUNT_DELTA = 123 ;

/// 数据类型：int32; 域名：IOPV
const int HSSDK_TAG_IOPV = 29 ;

/// 数据类型：int32; 域名：国债基金净值
const int HSSDK_TAG_DEBT_FUND_VALUE = 90 ;

/// 数据类型：int32; 域名：昨结算
const int HSSDK_TAG_PREV_SETTLEMENT = 28 ;

/// 数据类型：int32; 域名：今结算价
const int HSSDK_TAG_SETTLEMENT = 27 ;

/// 数据类型：uint16; 域名：补线类型
const int H5SDK_TAG_SUB_KIND = 676 ;

/// 数据类型：int32; 域名：返回值
const int H5SDK_TAG_MAINTAIN_RETURN_VALUE = 767 ;

/// 数据类型：bytevector; 域名：应用名称
const int H5SDK_TAG_APP_NAME = 20072 ;

/// 数据类型：bytevector; 域名：手机号
const int H5SDK_TAG_PHONE_NUMBER = 20073 ;

/// 数据类型：bytevector; 域名：MAC地址，作为用户识别标志
const int H5SDK_TAG_MAC_ADDRESS = 20074 ;


#endif /* __H5_SDK_TAG_H__ */

