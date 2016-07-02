///恒生插件库接口定义
/*******************************************************
  源程序名称:plugin_interface.h
  软件著作权:恒生电子股份有限公司
  系统名称:  恒生插件库接口标准
  模块名称:  公共模块
  功能说明: 插件库通用接口定义
 *1、插件库以跨语言复用为目的，以动态库（共享库）的形式封装各类服务性功能的实现，统一定义动态库引出规范；<p>
 *2、插件库所封装的服务一般是通用的、基础的、非业务的功能；插件库一般运行在一个IoC的插件容器中，也可以脱离容器单独使用；<p>
 *3、服务接口定义采用C++纯虚基类，并从IKnown接口继承；IKnown接口中的QueryInterface方法，可以获得该服务的反转控制接口IIoC及在线管理接口IManged。<p>
 *   反转控制接口IIoC用于设置该服务实现所需要的依赖项，如配置信息、上下文环境、其他服务接口；<p>
 *   在线管理接口IManged用于实现对该服务进行运行时的在线监控与管理；<p>
 *4、为了便于跨语言复用，对接口定义作如下限定：<p>
 *   各接口函数所使用的参数类型限定为C++基本数据类型：int、char、char *、void *、double；<p>
 *   接口函数不允许重载；<p>
             
  作    者:  研发中心
  开发日期:  20071229
  备    注:  在windows下注意函数调用方式：__stdcall

  修改日期：20080310
  修改人员：zhangly
  修改说明：根据20080305评审结果，在一些同步处理函数接口上，增加传入设置下一步处理超时的回调函数 PFOnSetTimeOut

  修改日期：20080124
  修改人员：zhangly
  修改说明：修改了插件库引出函数原型定义,以支持多实例.PFgetxxxxDependService(),PFinitxxxx()方法都增加传入一个实例指针

  修改日期：20080116
  修改人员：zhangly
  修改说明：config.h修改了相关定义,如IConfigObject改为IRCfgObj

  修改日期：20080111
  修改人员：zhangly
  修改说明：根据20080110评审结果，修改IIoC接口,PFgetxxxxDependService定义
  
  修改日期：20080623
  修改人员：liangyong
  修改说明：修改业务包操作接口声明, IPacker --> IF2Packer
  									IUnPacker --> IF2UnPacker
*********************************************************/

#ifndef PluginInterface_h
#define PluginInterface_h
#include <Include/pluginid.h>
#include <stdio.h>

#ifdef _WIN32
	#if !defined( FUNCTION_CALL_MODE )
	#define FUNCTION_CALL_MODE		__stdcall
	#endif
#else
	#define FUNCTION_CALL_MODE
#endif

///PFgetxxxxInfo()返回I_OK表示按索引查到基础服务
#define I_OK 1
///PFgetxxxxInfo()返回I_NONE表示按索引查不到基础服务
#define I_NONE 0

///接口唯一标识的类型，由技总办统一命名规则，例如"com.hundsun.fbase.log" 即公司名.项目英文简称.接口名字
#define HS_SID  const char *

///反转控制器接口ID
#define SID_IOC  "com.hundsun.fbase.IoC"
///管理器接口ID
#define SID_MANAGER  "com.hundsun.fbase.Manager"

#ifndef HS_IKNOWN
#define HS_IKNOWN

///各类接口统一的查询与引用接口，所有插件提供的接口都要实现 IKnown (参照COM标准)
struct IKnown
{
	///接口查询
	/**查询与当前接口相关的其他接口，例如可以查到 IIoC, IManager 等
	 *@param HS_SID  iid  接口全局唯一标识
	 *@param IKnown **ppv 返回iid对应的接口指针
	 *@return I_OK 成功，I_NONE 未查到iid 相应接口
	 */
    virtual unsigned long  FUNCTION_CALL_MODE QueryInterface( HS_SID iid, IKnown **ppv ) = 0;
	///引用接口，引用计数加一(多线程引用时，方法实现代码里要对计数值加锁后修改)
    virtual unsigned long  FUNCTION_CALL_MODE AddRef() = 0;
	///释放接口，引用计数减一，计数为0时释放接口的实现对象(多线程引用时，方法实现代码里要对计数值加锁加锁后修改)
    virtual unsigned long  FUNCTION_CALL_MODE Release() =  0;
};


struct IRCfgObj; //树状配置对象接口（类似XML，通过此接口可遍历整个树）


/*
//插件配置项节点
#define CO_PLUGIN "plugin"

//公共配置项节点
#define CO_PUB  "pub"


//插件列表配置项节点
#define CO_PLUGINS "plugins"

*/





//插件配置项节点
#define CO_PLUGIN "plugin"

//公共配置项节点
#define CO_PUB  "pub"

//ESB定义的共公配置项节点下的应用信息子节点及属性
//@{
//公共配置项下AppInfo子节点，作为ESB每个进程在逻辑上的唯一标识
#define CO_APP_INFO  "AppInfo"
//公共配置项下AppInfo子节点，应用名(组名)属性
#define CO_ATTR_APP_NAME  "AppName"
//公共配置项下AppInfo子节点，应用编号(组内编号)属性，一般由命令行参数指定
#define CO_ATTR_APP_ID  "AppID"
//公共配置项下AppInfo子节点，服务进程名属性(逻辑上的服务名)
#define CO_ATTR_SVR_NAME  "SvrName"
//公共配置项下AppInfo子节点，服务进程编号属性，同一种服务进程因为fork操作有多个进程时，每个进程有一个编号
#define CO_ATTR_SVR_ID  "SvrID"
//公共配置项下Args子节点，存储命令行启动参数。存储形式如下：
//<Args opt="value"> opt为参数名，value为参数值。(无值参数不记录在这里)
//如：hsserver -start monsvr -s 1 -c存为<Args Exe="hsserver" start="monsvr" s="1">
#define CO_APP_ARGS  "Args"
//hsserver的应用程序名, 即Args中的Exe
#define CO_APP_ARGS_EXE "Exe"
//@}

//插件列表配置项节点
#define CO_PLUGINS "block"


///插件反转控制接口，通过此接口容器（应用）向该插件传递其所依赖的信息:配置信息(命令行参数等同于配置信息)、依赖的其他插件接口；
/**
 *若需要为插件设置依赖项，通过插件实现的IKnown接口中的QueryInterface(SID_IOC,&lpIoC)，可以获得本接口；<p>
 *一般插件容器会通过此接口为插件设置依赖项；若插件没有对外依赖项,则可以不提供此接口；
 */
struct IIoC : IKnown
{

	///传递配置信息
    /**注意插件运行环境下配置对象的生命期，一般会在系统启动时就结束，以免占用内存.
	 *@param const char * sPath  配置对象路径标识，合法取值： "pub"     公共配置对象,如命令行参数
	                                                          "plugin"  当前插件的配置对象
                                                              "plugins" 插件列表(只有IF2Core容器会依赖这个配置项)
     *@param IRCfgObj * lpConfig 配置信息
	 *@return I_OK 成功，I_NONE 失败.
     */
	virtual  int  FUNCTION_CALL_MODE SetConfig(const char * sPath,IRCfgObj * lpConfig)=0;

	///传递所依赖的其他插件接口
    /**
     *@param HS_SID iid 所依赖的插件ID
	 *@param HS_SID iid 所依赖的插件接口
	 *@return I_OK 成功，I_NONE 失败.
     */
	virtual  int  FUNCTION_CALL_MODE SetDependService(HS_SID iid,IKnown * lpService)=0;

};


struct IF2UnPacker;//解包器接口，一般用于传递请求参数
struct IF2Packer;//打包器接口，一般用于返回应答结果集

///设置下一步处理超时的回调函数类型，在插件以同步形式处理系统事件或进行初始化时，调用者会传入一个回调函数，以便插件通过调用该函数在同步处理过程中将处理进度通知调用者
/**该回调函数的调用者在调用时，传入下一步处理的超时时间(毫秒)<p>
 *相关方法：插件的PFinitxxxx()初始化方法,IManager.ProcSysEvent()系统事件处理方法<p> 
 *          插件容器的IF2Core.Load()插件加载与初始化方法，IF2Core.ProcSysEvent()系统事件处理方法<p>
 *@param int iMS  估计的处理超时时间(毫秒)
 */
typedef void (FUNCTION_CALL_MODE * PFOnSetTimeOut)( int iMS );

///消息处理函数有数据要发送
#define RET_PM_OK        0
///消息处理函数无数据要发送
#define RET_PM_NONE   	 1
///消息处理函数返回系统错误包(根据AR协议接口，必须定义为-1)
#define RET_PM_ERR       -1

///统一管理接口，实现本接口的插件，就可以被插件容器统一进行在线管理；
/**
 *若需要对插件进行在线管理，通过插件实现的IKnown接口的QueryInterface(SID_MANAGER,&lpMgr)，可以获得本接口；<p>
 *一般插件容器会维护所有插件提供的IManager接口，并向这些插件传递管理请求与系统事件；若插件不提供此接口，则无法进行在线管理；
 */
struct IManager : IKnown
{

	///处理管理请求
    /**
	 *@param IUnPacker * lpInput  输入请求包解包器接口，约定function_id字段表明不同的管理功能，取值由实现者自行确定，一般从1开始编号，function_id=0 默认表示取支持的管理功能号列表
     *@param IPacker * lpOutput   输出应答包打包器接口[输出参数]
	 *@return RET_PM_OK 成功，RET_PM_NONE 无数据，RET_PM_ERR 系统错误(输入的管理功能号不存在)，其他正整数表示业务性错误
     */
	virtual  int  FUNCTION_CALL_MODE ProcMngRequest(IF2UnPacker * lpInput,IF2Packer * lpOutput)=0;

	///处理管理应答
    /**被管理的对象如果向其他AS发送过管理请求，则收到应答时提交此函数处理；
	 *@param IUnPacker * lpInput  输入应答包解包器接口
	 *@return I_OK 成功，I_NONE 失败.
	 */
	virtual  int  FUNCTION_CALL_MODE ProcMngAnswer(IF2UnPacker * lpInput)=0;

	///处理系统事件
    /**主框架将一些系统事件统一通知给各被管理对象；
     *@param unsigned long dwMessageID       系统事件编号(参见MSG_XXXX定义)
	 *@param unsigned long dwWParam          消息参数1(由不同的dwMessageID来具体定义)
     *@param unsigned long dwLParam          消息参数2(由不同的dwMessageID来具体定义)
     *@param PFOnSetTimeOut pfOnSetTimeOut   设置超时的回调函数,缺省为NULL,用于通知调用者本插件估计还需要多少时间才能完成对该事件的处理或在指定时间后再次回调该函数
	 *@return I_OK 成功，I_NONE 失败.
     */
	virtual  int  FUNCTION_CALL_MODE ProcSysEvent(unsigned long dwMessageID,unsigned long dwWParam,unsigned long dwLParam,
		                                          PFOnSetTimeOut pfOnSetTimeOut = NULL ) = 0;

};

///系统开始处理消息(此时所有插件加载并已初始化完毕)
#define MSG_START         1
///系统停止处理消息（停止后可以再启动）
#define MSG_STOP          2   
///系统退出消息
#define MSG_EXIT          3 
///系统将开始Fork子进程的通知消息(此时加载级别为0的插件已加载并初始化完毕)
#define MSG_FORK          4

struct tagBaseServiceInfo;

///每种插件需要按以下原型提供(引出)三个函数（函数名自定），以便插件库脱离特定插件容器复用时，可以直接使用动态库引出函数;
//@{
///返回插件实例的接口指针，对于孤子模式的插件每次调用getxxxxInstance()返回相同的实例指针，对于支持多实例的插件每次调用getxxxxInstance()返回不同的实例指针；
/**
 * 在调用该插件的PFinitxxxx与PFgetxxxxDependService函数时，返回的接口指针会作为参数传入,以方便多实例情况下对这二个函数的实现。
 */
typedef IKnown * (FUNCTION_CALL_MODE * PFgetxxxxInstance)( );

///返回所依赖的其他插件接口ID串，所依赖的插件接口会在初始化前(PFinitxxxx被调用前)被注入
/**
 *@param   void * lp    插件实例，即getxxxxInstance()返回的实例指针
 *@return  char * lpIDs 返回依赖的其他插件接口ID列表串。
 *         HS_SID是字符串，列表最后一项的为空字符串,例如： "xxxxx\0yyyyy\0\0"，表示依赖xxxxx,yyyyy二个插件接口。
 */
typedef char * (FUNCTION_CALL_MODE * PFgetxxxxDependService)( void * );

///初始化函数指针（会在所有插件被动态加载后才调用，并根据初始化依赖顺序调用）
/**
 *@param   void * lp                     插件实例，即getxxxxInstance()返回的实例指针
 *@param PFOnSetTimeOut pfOnSetTimeOut   设置超时的回调函数,缺省为NULL,用于通知调用者本插件估计还需要多少时间才能完成对当前操作或在指定时间后再次回调该函数
 *@return  int                           I_OK 成功，I_NONE 失败.
 */
typedef int (FUNCTION_CALL_MODE * PFinitxxxx)( void * , PFOnSetTimeOut );
//@}


///一个插件库可以封装多个插件，必须引出一个插件信息查询接口，以便容器使用这个接口动态加载插件
//@{

///插件库信息查询，调用者从iIndex=0开始逐个轮询，直到返回I_NONE为止;
typedef int (FUNCTION_CALL_MODE * PFgetxxxxInfo)(int iIndex, tagBaseServiceInfo * ppv );

///插件信息(动态加载时使用)
struct tagBaseServiceInfo
{
	///统一命名规则的插件ID(插件实现中用常量串表示)
	HS_SID ServiceID;
	///插件版号信息yyyymmdd
	int iVersion;
	///插件初始化函数指针，插件容器通过此函数初始化插件
    PFinitxxxx  Init;
	///获取插件实例函数指针，插件容器通过此函数获得插件实例
    PFgetxxxxInstance  getInstance;
	///声明插件初始化时所依赖的其他插件ID(即其他插件必须先初始化好)，插件容器根据此函数的返回内容，在插件初始化前为插件注入其所依赖的其他插件接口
	PFgetxxxxDependService getDependServices;
	///插件附加描述信息，一般为编译日期
	const char * Caption;
};

//@}

#endif



#endif
