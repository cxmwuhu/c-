/*
#pragma pack(1)

struct ACC_CMDHEAD
{
	unsigned short wCmdType;
	unsigned short wAttr;
	unsigned int nLen;	
	unsigned int nExpandInfo;	//扩展信息，如果是请求数据，响应要原样返回;
};


struct ACC_SERVERLOGIN
{
	char m_cValid[8];		//服务器验证串;
	unsigned int nSerId;	//服务器id，该id可以用ip标示;
};


struct LOGIN_HEAD
{
	ACC_CMDHEAD accCmdHead;
	ACC_SERVERLOGIN accServerLogin;
};

struct sub_head
{
	unsigned short	sub_type;
	unsigned short  sub_attrs;
	unsigned short  sub_length;
};

#pragma pack(pop)

*/

#pragma pack(1)

//===================================================================================
//数据子协议定义  数据格式:sub_head + data
//子协议数据头定义
/*
typedef struct sub_tuisong_head
{
    unsigned short  sub_type;
    unsigned short  sub_attrs;          // 取值SubAttrsEnum
    unsigned short  sub_length;
    unsigned int    sub_extend;
};
*/

enum SubAttrsEnum
{
    SAE_SIMPLE = 0x0001,            // 标识数据请求基本字段 采用【精简定义】格式
    SAE_OVERTIME = 0x0002,          // 标识处理超时返回 服务端返回
};

//====================================================================================
// 平台类型定义
enum PlatFormEnum
{
    J2ME = 1,                           // J2ME
    WP7 = 2,                            // WP7
    OPHONE_GPHONE = 3,                  // android
  //  IPHONE = 4,                         // IPHONE (6.21版本2013年6月25日以前使用)
    IPHONE = 14,                        // IPHONE (2013年6月26日以后版本使用)      （2013年6月26日修改）
    S60 = 5,                            // S60
    IPAD = 6,                           // IPAD标准版
    IPAD_HIGH_END = 7,                  // IPAD高端版
    GPAD = 8,                            // GPAD
    WP7CMCC = 9,                    // 手机证券WP7
    GPHONECMCC = 10,                  // 手机证券android手机
    IPHONECMCC = 11,                // 手机证券iphone    
    
    WIN8 = 12,                         // windows8平台
    WEB_CJ = 13,                       // web cj平台
    
    SILVER_ANDROID = 15,               // 白银android
    SILVER_IPHONE = 16,                // 白银iphone
    SILVER_WP8 = 17,                   // 白银wp8
    
    IPHONEJD = 18,                     // iphone经典版
    ANDROIDJD = 19,                    // android经典版
    
    IPHONE_CAIPIAO = 20,               // iphone彩票独立版
};

//====================================================================================
// 消息类型定义
enum MsgType
{
    PRICE_WARN = 0,                     // 股价预警
    SELF_STK = 1,                       // 自选股消息
    PUB_NOTICE = 2,                     // 公告消息
    LOTTERY_MSG = 3,                    // 彩票消息
    YUNCAOPAN_MSG = 4,                  // 云操盘消息
    IMATTEN_MSG = 5,                    // IM提醒消息
};
#pragma pack(pop)

#pragma pack()

//定义请求字段枚举类型
 enum ReqType
 {
	 req_string,
	 req_byte,
	 req_char,
	 req_short,
	 req_float,
	 req_int,
	 req_stringarray,
	 req_time,
	 req_long64,
	 req_char2,
	 req_char3,
	 req_customdata,
	 req_130data,
	 req_encrypass,
	 req_intarray,
	 req_shortarray,
	 req_account,
	 req_uint
 };


enum AnsType
{
	ans_stringarray,
	ans_unit64,
	ans_string,
	ans_byte,
	ans_char,
	ans_short,
	ans_int,
	ans_int24,
	ans_bytearray,
	ans_long,
	ans_long64,
	ans_intarray,
	ans_customdata,
	ans_130data
};


//压力参数列表
enum PressData
{
	PressConnSucc,
	PressConnFail,
	PressSendSucc,
	PressSendFail,
	PressRecvSucc,
	PressRecvFail
};

