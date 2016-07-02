///	恒生消息接口定义
/*******************************************************
  源程序名称:hscomm_message_interface.h
  软件著作权:恒生电子股份有限公司
  系统名称:  恒生消息接口定义
  模块名称:  消息模块
  功能说明:  消息接口定义
             
  作    者:  maoyinjie
  开发日期:  20100801
  备    注:  在windows下注意函数调用方式：__stdcall

yinghh    20130408 给重复组增加接口，使其可以一次性添加N条空记录，方便业务层在记录条数已知的情况下优化性能
yinghh    20130508 增加消息的新接口Clone和Duplicate
*********************************************************/

#ifndef HSCOMM_MESSAGE_INTERFACE_H
#define HSCOMM_MESSAGE_INTERFACE_H

#include <stdio.h>
#include <stdlib.h>

//	定义跨平台的基本数据类型
#ifdef _WIN32
	typedef char				int8;
	typedef short				int16;
	typedef int					int32;
	typedef __int64				int64;
	typedef unsigned char		uint8;
	typedef unsigned short		uint16;
	typedef unsigned int		uint32;
	typedef unsigned __int64	uint64;
#else
	#include <sys/types.h>
	#if defined(__linux__)
		#include <stdint.h>
	#endif
	typedef int8_t				int8;
	typedef int16_t				int16;
	typedef int32_t				int32;
	typedef int64_t				int64;
	typedef uint8_t				uint8;
	typedef uint16_t			uint16;
	typedef uint32_t			uint32;
	typedef uint64_t			uint64;
#endif


#ifdef _WIN32
	#if !defined( FUNCTION_CALL_MODE )
		#define FUNCTION_CALL_MODE		__stdcall
	#endif
#else
	#define FUNCTION_CALL_MODE
#endif




//	数据类型定义
const uint8 TypeInt8=0;
const uint8 TypeuInt8=1;
const uint8 TypeInt16=2;
const uint8 TypeuInt16=3;
const uint8 TypeInt32=4;
const uint8 TypeuInt32=5;
const uint8 TypeInt64=6;
const uint8 TypeuInt64=7;
const uint8 TypeDouble=8;
const uint8 TypeString=9;
const uint8 TypeVector=10;
const uint8 TypeRaw=11;
const uint8 TypeIPV4=12;
const uint8 TypeIPV6=13;
const uint8 TypeMac=14;
const uint8 TypeArray=15;
const uint8 TypeSequence=16;
const uint8 TypeUnKnown=255;



#ifndef IHS_KNOWN
#define IHS_KNOWN

///	各类接口统一的查询与引用接口，所有插件提供的接口都要实现 IKnown (参照COM标准)
struct IHSKnown
{
	///接口查询
	/**查询与当前接口相关的其他接口，例如可以查到 IIoC, IManager 等
	 *@param HS_SID  iid  接口全局唯一标识
	 *@param IKnown **ppv 返回iid对应的接口指针
	 *@return I_OK 成功，I_NONE 未查到iid 相应接口
	 */
	virtual unsigned long  FUNCTION_CALL_MODE QueryInterface( const char * iid, IHSKnown **ppv ) = 0;
	///引用接口，引用计数加一(多线程引用时，方法实现代码里要对计数值加锁后修改)
	virtual unsigned long  FUNCTION_CALL_MODE AddRef() = 0;
	///释放接口，引用计数减一，计数为0时释放接口的实现对象(多线程引用时，方法实现代码里要对计数值加锁加锁后修改)
	virtual unsigned long  FUNCTION_CALL_MODE Release() =  0;
};
#endif

///	TAG的值域,支持数组形式
struct IHsCommTagItem : IHSKnown
{
	/////////////////////////////////////	获取值	////////////////////////////////////////
	///获得单字节整数形式的值	
	/**
	*@param int	iIndex:	需要获得数组内第几个元素的值
	*@return	域值
	**/
	virtual uint8	FUNCTION_CALL_MODE GetInt8(uint32 dwIndex=0) const=0;				//	0
	///获得双字节整数形式的值
	/**
	*@param int	iIndex:	需要获得数组内第几个元素的值
	*@return	域值
	**/
	virtual uint16	FUNCTION_CALL_MODE GetInt16(uint32 dwIndex=0) const=0;				//	0
	///获得4字节整数形式的值
	/**
	*@param int	iIndex:	需要获得数组内第几个元素的值
	*@return	域值
	**/
	virtual uint32	FUNCTION_CALL_MODE GetInt32(uint32 dwIndex=0) const=0;				//	0
	///获得64位整数形式的值
	/**
	*@param int	iIndex:	需要获得数组内第几个元素的值
	*@return	域值
	**/
	virtual uint64	FUNCTION_CALL_MODE GetInt64(uint32 dwIndex=0) const=0;				//	0
	///获得浮点形式的值
	/**
	*@param int	iIndex:	需要获得数组内第几个元素的值
	*@return	域值
	*Double		由于网络传输时,内部都会将double转换成int64或者int32(根据模板中定义的小数点位数)
				所以,处理double类型一方面速度慢,另一方面,不可避免可能会有数据失真,所以
				该接口尽量不要用
	**/
	virtual double	FUNCTION_CALL_MODE GetDouble(uint32 dwIndex=0) const=0;				//	0.0
	///获得rawdata的长度
	/**
	*@param int iIndex:			获取数组内第几个元素
	*@return	长度
	**/
	virtual int		FUNCTION_CALL_MODE 	GetRawDataLen(uint32 dwIndex=0) const=0;		//	0
	///获得rawdata的值
	/**
	*@param int* ilpRawDataLen:	返回rawdata的长度
	*@param int iIndex:			获取数组内第几个
	*@return	指向域值的指针
	**/
	virtual const void* FUNCTION_CALL_MODE GetRawData(int* ilpRawDataLen=NULL, uint32 dwIndex=0) const=0;		// "" 0
	///获得字串形式的值:
	/**
	*@param int	iIndex:	需要获得数组内第几个元素的值
	*@return	指向域值的指针
	**/
	virtual const char* FUNCTION_CALL_MODE GetString(uint32 dwIndex=0) const=0;			//	""
	

	/// 获得数组内成员个数 
	virtual int FUNCTION_CALL_MODE GetCount() const =0;
	/// 是否存在第iIndex个成员
	virtual bool FUNCTION_CALL_MODE IsExist(uint32 dwIndex=0) const =0;
	//	获得当前储存的数据类型
	virtual int  FUNCTION_CALL_MODE GetValueType(uint32 dwIndex=0) const=0;	


	///	设置值
	/**
	* 设置整数形式的值
	*@param int	iIndex:	需要设置数组内第几个元素的值
	*@param char cValue:	域值
	*@return	无
	**/
	virtual void FUNCTION_CALL_MODE SetInt8(uint8 byValue, int iIndex=0)=0;
	/**
	* 设置整数形式的值
	*@param int	iIndex:	需要设置数组内第几个元素的值
	*@param uint16 wValue:	域值
	*@return	无
	**/
	virtual void FUNCTION_CALL_MODE SetInt16(uint16 wValue, int iIndex=0)=0;
	/**
	* 设置整数形式的值
	*@param int	iIndex:	需要设置数组内第几个元素的值
	*@param int iValue:	域值
	*@return	无
	**/
	virtual void FUNCTION_CALL_MODE SetInt32(uint32 dwValue, int iIndex=0)=0;
	/**
	* 设置64位整数形式的值
	*@param int	iIndex:	需要设置数组内第几个元素的值
	*@param int64 i64Value:	域值
	*@return	无
	**/
	virtual void FUNCTION_CALL_MODE SetInt64(uint64 i64Value, int iIndex=0)=0;
	/**
	* 设置浮点形式的值
	*@param int	iIndex:	需要设置数组内第几个元素的值
	*@param double dfValue:	域值
	*@return	无
	*Double接口尽量不要用
	**/
	virtual void FUNCTION_CALL_MODE SetDouble(double dfValue, int iIndex=0)=0;	
	/**
	* 设置字串形式的值
	*@param int	iIndex:	需要设置数组内第几个元素的值
	*@param const char* lpValue:	域值
	*@return	无
	**/
	virtual void FUNCTION_CALL_MODE SetString(const char* lpValue, int iIndex=0)=0;
	/**
	* 设置rawdata形式的值
	*@param int	iIndex:	需要设置数组内第几个元素的值
	*@param const void* lpRawData:	域值
	*@param int iRawDataLen:	域值长度
	*@return	无
	**/
	virtual void FUNCTION_CALL_MODE SetRawData(const void* lpRawData, int iRawDataLen, int iIndex=0)=0;

	////////////////////////	遍历数组内有效成员,遍历完后可以用上述的GetXXX函数接口访问成员,其中iIndex值传入-1	//////////////////
	//	截止20100816	尚未实现
	/// 转到第一个有效成员
	/**
	*@return	0:无成员 	其他:还有成员
	**/
	virtual int FUNCTION_CALL_MODE First()=0;
	/// 到达下一个有效成员
	/**
	*@return	0:无成员 	其他:还有成员
	**/
	virtual int FUNCTION_CALL_MODE Next()=0;


	virtual bool FUNCTION_CALL_MODE IsDouble()=0;



	//	2011/04/19	新增	以下接口暂时未实现
	virtual void SetIPV4(const char* lpIPV4)=0;	//	类似	192.168.1.1	的地址
	virtual void SetIPV6(const char* lpIPV6)=0;	//	类似	fe80::226:55ff:fe1f:18cc	的地址
	virtual void SetMac(const char* lpMac)=0;		//	类似	00-1C-23-2A-A6-3A	的地址
	virtual char* GetIPV4()=0;
	virtual char* GetIPV6()=0;
	virtual char* GetMac()=0;

    // 获取在模板里定义的小数位数
    virtual int FUNCTION_CALL_MODE GetShift() = 0;
};


///	业务消息,可以是多记录的一个结果集,也可以是单记录,取决于具体消息的模板
struct  IBizResultSet : public IHSKnown		//	其本质是group
{
	/**
	*	获得某个域值的对象指针
	*@param int	iFieldTag:需要获得域值的TAG
	*@return	指向域值对象的指针(如果消息内不存在该TAG,则会返回一个缺省的对象,确保返回非空)
	**/
	virtual IHsCommTagItem*	FUNCTION_CALL_MODE GetItem(int iFieldTag) const =0;

	///判断是否为空
    /**@return 1 是，0 不是;
      */
    virtual int FUNCTION_CALL_MODE IsEmpty()const =0;	
    ///判断是否为结尾
    /**@return 1 是，0 不是;
      */
    virtual int FUNCTION_CALL_MODE IsEOF() const =0;
    ///取下一条记录
    /**@return 1 存在记录，0 已经到达最后
      */
    virtual int FUNCTION_CALL_MODE Next() =0;
	/**
	*@param int	iFieldTag:检查是否存在iFieldTag
	*@return	0:不存在	非0存在
	**/
	virtual int	FUNCTION_CALL_MODE IsExist(int iFieldTag) const  = 0;

	//	获得行数
	/**
	*@return	返回行数
	**/
	virtual int	FUNCTION_CALL_MODE GetLines() const = 0;

	/// 遍历获得Record中的普通字段
	/**
	*@param wTagID，本方法返回非NULL时的相应Item的TAG值[OUT]
	*@return NULL表示遍历结束，否则为遍历获得的Item
	*/
	virtual IHsCommTagItem* FUNCTION_CALL_MODE GetPureTag(uint16 &wTagID) const = 0;

    /// 在开始遍历记录中的普通字段时需要先调用一次本函数
	/**
	*/
	virtual void FUNCTION_CALL_MODE BeginTour() = 0;
};




//////////////////////////////////////////////////////////////////////////
///	消息是个递归的定义
struct  IGroup;
///	消息中的每条记录
struct IRecord : IHSKnown
{
	/**
	*	获得某个域值的对象指针
	*@param int	iFieldTag:需要获得域值的TAG
	*@return	指向域值对象的指针(如果消息内不存在该TAG,则创建对象,如果创建失败,则返回一个缺省对象,确保返回非空)
	**/
	virtual IHsCommTagItem*	FUNCTION_CALL_MODE GetItem(int iFieldTag) const=0;

	/**
	*@param int	iFieldTag:检查是否存在iFieldTag(普通字段和sequence都检查,如果用户明确知道
		自己需要检查的是普通字段还是sequence,请用IsExistPure和IsExistSequence检查,以提升性能)
	*@return	0:不存在	非0存在
	**/
	virtual bool	FUNCTION_CALL_MODE IsExist(int iFieldTag) const  = 0;

	/// 在消息中删除一个域
	/**
	*@param int	iFieldTag:需要删除TAG,仅删除普通域,如果该域是sequence,必须调用RemoveGroup来操作
	*@return 0:成功		其他:失败
	**/
	virtual int	FUNCTION_CALL_MODE RemoveTag(int iFieldTag) = 0;
	/// 往记录中增加一个重复组，如该重复组已存，则返回的是已存在重复组接口。
	/**
	*@param int iGroupTag	重复组(计数域)Tag(计数域作为一个重复组的TAG处理)
							可以唯一的标志该GROUP
	*@return  IGroup 结构指针,在资源紧张时,会返回NULL

	行为：	生成一个空的Group结构，将该空Group结构插入到记录中，同时返回
			刚生成的该Group结构指针。开发人员可以在获得该指针后，接下去
			对Group结构进行操作，可以在Group中插入多条记录。
	*/
	virtual IGroup* FUNCTION_CALL_MODE SetGroup( int iGroupTag ) = 0;

	/// 取指定的重复组
	/**
	 *@param int iGroupTag 重复组Tag，FIX协议中为计算域Tag
	 *@return IGroup 结构指针; 如果该重复组不存在，返回NULL;
	 */
	virtual IGroup * FUNCTION_CALL_MODE GetGroup( int iGroupTag ) const = 0;

	/// 从消息中删除一个重复组
	virtual void FUNCTION_CALL_MODE RemoveGroup( int iGroupTag ) = 0;

	/// 遍历获得Record中的普通字段
	/**
	*@param wTagID，本方法返回非NULL时的相应Item的TAG值[OUT]
	*@return NULL表示遍历结束，否则为遍历获得的Item
	*/
	virtual IHsCommTagItem* FUNCTION_CALL_MODE GetPureTag(uint16& wTagID) = 0;

	/**
	*@param int	iFieldTag:检查是否存在iFieldTag(只检查普通字段)
	*@return	0:不存在	非0存在
	**/
	virtual bool FUNCTION_CALL_MODE IsExistPure(int iFieldTag) const  = 0;
	/**
	*@param int	iFieldTag:检查是否存在iFieldTag(只sequence)
	*@return	0:不存在	非0存在
	**/
	virtual bool FUNCTION_CALL_MODE IsExistSequence(int iGroupTag) const  = 0;

    /// 在开始遍历记录中的普通字段时需要先调用一次本函数
	/**
	*/
	virtual void FUNCTION_CALL_MODE BeginTour() = 0;

	//	判断某个TAG是不是double类型
	virtual bool FUNCTION_CALL_MODE IsDouble(int iFieldTag) = 0;

    // 把record的数据复制到本记录里来
    // 注意！调用者要确保这2条记录的模板是一样的
    // 本方法目前只会返回0
    virtual int FUNCTION_CALL_MODE CopyFrom(IRecord *record) = 0;

    // 获取某个字段在模板里定义的小数位数
    virtual int FUNCTION_CALL_MODE GetShift(int iFieldTag) = 0;

    /// 在开始遍历记录中的字段时需要先调用一次本函数
	/**
	*/
	virtual void FUNCTION_CALL_MODE BeginTourEx() = 0;

    /// 遍历获得Record中的字段
	/**
	*@param wTagID，本方法返回非NULL时的相应Item或Group的TAG值[OUT]
    *@param isGroup，本方法返回非NULL时是否是Group[OUT]
	*@return NULL表示遍历结束，否则为遍历获得的Item和Group，需要强制类型转换
	*/
	virtual void * FUNCTION_CALL_MODE GetTagEx(uint16& wTagID, bool &isGroup) = 0;

    /**
	*	获得某个域值的对象指针
	*@param int	iFieldTag:需要获得域值的index
	*@return	指向域值对象的指针(如果不存在则返回NULL)
	**/
	virtual IHsCommTagItem * FUNCTION_CALL_MODE GetItemByIndex(int index) const = 0;

    /// 在消息中删除一个域
	/**
	*@param int	index:需要删除TAG的index,仅删除普通域,如果该域是sequence,必须调用RemoveGroup来操作
	*@return 0:成功		其他:失败
	**/
	virtual int	FUNCTION_CALL_MODE RemoveTagByIndex(int index) = 0;

    /// 遍历获得Record中的字段
	/**
	*@param wTagID，本方法返回非NULL时的相应Item或Group的TAG值[OUT]
    *@param isGroup，本方法返回非NULL时是否是Group[OUT]
    *@param index，本方法返回非NULL时的相应Item或Group的index值[OUT]
	*@return NULL表示遍历结束，否则为遍历获得的Item和Group，需要强制类型转换
	*/
	virtual void * FUNCTION_CALL_MODE GetTagEx2(uint16 &wTagID, bool &isGroup, int &index) = 0;
};

///重复组对象接口，表示多条记录的一个集合(DataSet)
struct  IGroup :public IHSKnown
{
	/// 返回重复组内记录个数
	virtual int FUNCTION_CALL_MODE GetRecordCount() const = 0;

	/// 取重复组中的某条记录
	/**
	 *@param int num 第几条记录(num从0开始)
	 *@return IRecord 结构指针
	 */
	virtual IRecord * FUNCTION_CALL_MODE GetRecord(int num) const = 0;

	/// 在重复组中追加一条空白记录
	/**
	 * @return IRecord 追加的记录指针,如果资源紧张,可能会返回NULL
	行为:	调用一次AddRecord操作,函数创建一个空的 IRecord,并且将该 IRecord 插入
			重复组,返回刚创建的 IRecord 指针,开发人员获得该指针后,可以往 IRecord
			中插入字段(普通字段或者又是一个重复组字段)

	*/
	virtual IRecord * FUNCTION_CALL_MODE AddRecord() = 0;

	/// 一次性插入count条空记录，后续调用者通过GetRecord来获取对应下标的记录
	/**
	*@param int count 需要插入的记录数
	*@return int 返回0表示成功，-1表示内存申请失败
	*/
    virtual int FUNCTION_CALL_MODE AddRecords(int count) = 0;

    /**
	*	根据Tag获得index
	*@param int	iFieldTag:需要获得下标的TAG
	*@return	Tag对应的index，如果找不到，返回255
	**/
	virtual int	FUNCTION_CALL_MODE GetIndexByFieldID(int iFieldTag) const = 0;
};

// 操作复杂消息时,利用 IRecord 和 IGroup 以实现递归的定义,但是,对于恒生交易系统使用的接口而言,采用IHsCommMessage和IBizResultSet即可处理
//	对消息的操作,最终体现为对消息里面的消息体的操作,如果,明确要操作消息头,必须通过GetHead拿到头后再操作
struct IHsCommMessage: IHSKnown
{
	/**
	*	获得消体某个域值的对象指针
	*@param int	iFieldTag:需要获得域值的TAG
	*@return	指向域值对象的指针(如果消息内不存在该TAG,则会返回一个缺省的对象,确保返回非空)
	**/
	virtual IHsCommTagItem * FUNCTION_CALL_MODE GetItem(int iFieldTag)=0;
	/**
	*	获得结果集的对象指针
	*@param int	iFieldTag:需要获得结果集的TAG
	*@return	指向结果集对象的指针
	*	注意:返回的结果集仅供读取值,不允许设置值
	**/
	virtual IBizResultSet * FUNCTION_CALL_MODE GetBizResultSet(int iFieldTag) = 0;

	/**
	*@param int	iFieldTag:检查是否存在iFieldTag
	*@return	0:不存在	非0存在
	**/
	virtual int			FUNCTION_CALL_MODE IsExist(int iFieldTag) const  = 0;

	///	根据报文生成消息对象内的各个域
	/**
	* 根据报文生成消息对象内的各个域
	*@param const void* lpMessageBuffer:	报文
	*@param int iMsgLen:					报文长度
	*@return 0:成功		其他:失败
	**/
	virtual int			FUNCTION_CALL_MODE SetBuffer(const void* lpMessageBuffer, int iMsgLen) = 0;
	/**
	* 将本对象内的各个域值对象最终封装成报文返回,(不封装协议内部的域值)
	*@param int* ilpMsgLen:	封装后报文的长度
	*@return 报文,如果失败,返回NULL
	**/
	virtual void*		FUNCTION_CALL_MODE GetBuffer(int* ilpMsgLen) = 0;

	/// 在消息中删除一个域
	/**
	*@param int	iFieldTag:需要删除TAG
	*@return 0:成功		其他:失败
	**/
	virtual int			FUNCTION_CALL_MODE RemoveTag(int iFieldTag) = 0;
	/// 往记录中增加一个重复组，如该重复组已存，则返回的是已存在重复组接口。
	/**
	*@param int iGroupTag	重复组(计数域)Tag(计数域作为一个重复组的TAG处理)
							可以唯一的标志该GROUP
	*@return  IGroup 结构指针

	行为：	生成一个空的Group结构，将该空Group结构插入到记录中，同时返回
			刚生成的该Group结构指针。开发人员可以在获得该指针后，接下去
			对Group结构进行操作，可以在Group中插入多条记录。
	*/
	virtual IGroup* FUNCTION_CALL_MODE SetGroup( int iGroupTag ) = 0;

	/// 取指定的重复组
	/**
	 *@param int iGroupTag 重复组Tag，FIX协议中为计算域Tag
	 *@return IGroup 结构指针; 如果该重复组不存在，返回NULL;
	 */
	virtual IGroup * FUNCTION_CALL_MODE GetGroup( int iGroupTag ) const = 0;


	//	以下接口仅仅是消息需要,但是消息内部的Record和Group不需要
	//	消息中加入时间戳
	virtual int	FUNCTION_CALL_MODE AddTimeStamp(char* lpText) = 0;
	//	留下痕迹(每个处理过的插件都要留下自己的痕迹)
	virtual int FUNCTION_CALL_MODE AddTrack(const char* lpID)=0;
	virtual int FUNCTION_CALL_MODE AddIntTrack(int iID)=0;
	
	//	获得头部信息,
	virtual IRecord * FUNCTION_CALL_MODE GetHead()=0;
	//	获得业务体
	virtual IRecord * FUNCTION_CALL_MODE GetBody()=0;


	//	以下的操作是直接操作消息头中的几个特殊TAG,以简化对消息头的操作
	virtual int FUNCTION_CALL_MODE GetFunction() const=0;
	virtual int FUNCTION_CALL_MODE GetPacketType()const=0;
	virtual int FUNCTION_CALL_MODE GetBizID()const=0;
	virtual int FUNCTION_CALL_MODE SetFunction(uint32 dwFuncID)=0;
	virtual int FUNCTION_CALL_MODE SetPacketType(uint32 dwPacketType)=0;
	virtual int FUNCTION_CALL_MODE SetBizID(uint32 dwBizID)=0;


	///	在打包解报失败时,可以获得为什么失败的原因
	virtual const char* FUNCTION_CALL_MODE GetLastErrInfo()=0;
	/// 清空消息,准备重新组织一个新的消息
	virtual void FUNCTION_CALL_MODE Clear()=0;

    /**
    *	复制一个和本消息一模一样的消息（注意！被复制出来的消息的初始引用计数为1）
    *   复制完成后，被复制出来的消息和原消息互相独立
    *@return	被复制出来的消息
    **/
    virtual IHsCommMessage * FUNCTION_CALL_MODE Clone() = 0;

    /**
    *	复制本消息的一个只读副本
    *   业务层可以这样来理解本方法的逻辑：增加本消息的引用计数，然后返回本消息
    *   业务层需要仔细区别本方法和Clone方法的区别
    *   当业务层需要修改这个副本时，可以调用副本的Clone方法来获取新消息，然后才能进行修改
    *@return    只读副本
    **/
    virtual IHsCommMessage * FUNCTION_CALL_MODE Duplicate() = 0;

    /**
    *   把本消息复制成hsMessage的一个拷贝
    *   要求本消息和hsMessage是同一种类型的消息
    **/
    virtual int FUNCTION_CALL_MODE CopyFrom(IHsCommMessage *hsMessage) = 0;

    /**
	* 将本对象内的各个域值对象最终封装成报文返回,(不封装协议内部的域值)
	*@param int* ilpMsgLen:	封装后报文的长度
	*@return 报文,如果失败,返回NULL
	**/
	virtual void*		FUNCTION_CALL_MODE GetBufferEx(int* ilpMsgLen, char *buffer, int size) = 0;
};

struct IHsComMessageFactory;

//	如果是为API用,则需要初始化
#ifdef EXPORT_API
	
#ifdef __cplusplus
extern "C" { 
#endif
	/// 初始化消息模板
	/**
	 *@param const char* lpTemplateFile 消息模板文件名
	 *@param int iBizModulID	需要初始化哪个业务(因为在一个模板文件中会存在多个业务,一般对应一个客户端只需要关注一个业务即可)
	 * @return 0成功  其他失败
	*/
	int				FUNCTION_CALL_MODE Init(IHSKnown* lpCfg, const char* lpTemplateFile, int iBizModulID);
	
    // 根据功能号来创建消息,因为不同功能有不同的消息模板
	IHsCommMessage* FUNCTION_CALL_MODE CreateMessage(int iFunc, int iPacketType);

    // 创建新的消息工厂
    IHsComMessageFactory * FUNCTION_CALL_MODE CreateMessageFactory();

	/// 根据fieldid获得fieldname
	/**
	 *@param int iFieldID		字段id
	 *@param int iBizModulID	操作时用到的业务模板
	 * @return fieldname, 如果失败,返回""
	*/
	const char* FUNCTION_CALL_MODE GetFieldNameByID(int iFieldID, int iBizModulID);
	/// 根据名字获得id
	/**
	 *@param const char* lpFieldName		字段名
	 *@param int iBizModulID	操作时用到的业务模板
	 * @return >=0字段ID	<0,说明没找到
	*/
	int FUNCTION_CALL_MODE GetFieldIDByName(const char* lpFieldName, int iBizModulID);
	const char* FUNCTION_CALL_MODE GetFuncNameByID(int iFuncD, int iPacketType, int iBizModulID);

	//	获得一个字段的取值类型
	int FUNCTION_CALL_MODE GetFieldType(int iBizModulID, int iTagID);
	//	获得某个array类型的字段内部,具体特定成员的取值类型
	int FUNCTION_CALL_MODE GetElementType(int iBizModulID, int iTagID, int iIndex);

	//	maoyinjie 20101230
	const char* FUNCTION_CALL_MODE GetFuncNote(int iFuncD, int iPacketType, int iBizModulID);
#ifdef __cplusplus
}
#endif	//	__cplusplus
#endif	//	EXPORT_API
#endif	//	HSCOMM_MESSAGE_INTERFACE_H
