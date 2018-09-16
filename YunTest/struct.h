/*
#pragma pack(1)

struct ACC_CMDHEAD
{
	unsigned short wCmdType;
	unsigned short wAttr;
	unsigned int nLen;	
	unsigned int nExpandInfo;	//��չ��Ϣ��������������ݣ���ӦҪԭ������;
};


struct ACC_SERVERLOGIN
{
	char m_cValid[8];		//��������֤��;
	unsigned int nSerId;	//������id����id������ip��ʾ;
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
//������Э�鶨��  ���ݸ�ʽ:sub_head + data
//��Э������ͷ����
/*
typedef struct sub_tuisong_head
{
    unsigned short  sub_type;
    unsigned short  sub_attrs;          // ȡֵSubAttrsEnum
    unsigned short  sub_length;
    unsigned int    sub_extend;
};
*/

enum SubAttrsEnum
{
    SAE_SIMPLE = 0x0001,            // ��ʶ������������ֶ� ���á������塿��ʽ
    SAE_OVERTIME = 0x0002,          // ��ʶ����ʱ���� ����˷���
};

//====================================================================================
// ƽ̨���Ͷ���
enum PlatFormEnum
{
    J2ME = 1,                           // J2ME
    WP7 = 2,                            // WP7
    OPHONE_GPHONE = 3,                  // android
  //  IPHONE = 4,                         // IPHONE (6.21�汾2013��6��25����ǰʹ��)
    IPHONE = 14,                        // IPHONE (2013��6��26���Ժ�汾ʹ��)      ��2013��6��26���޸ģ�
    S60 = 5,                            // S60
    IPAD = 6,                           // IPAD��׼��
    IPAD_HIGH_END = 7,                  // IPAD�߶˰�
    GPAD = 8,                            // GPAD
    WP7CMCC = 9,                    // �ֻ�֤ȯWP7
    GPHONECMCC = 10,                  // �ֻ�֤ȯandroid�ֻ�
    IPHONECMCC = 11,                // �ֻ�֤ȯiphone    
    
    WIN8 = 12,                         // windows8ƽ̨
    WEB_CJ = 13,                       // web cjƽ̨
    
    SILVER_ANDROID = 15,               // ����android
    SILVER_IPHONE = 16,                // ����iphone
    SILVER_WP8 = 17,                   // ����wp8
    
    IPHONEJD = 18,                     // iphone�����
    ANDROIDJD = 19,                    // android�����
    
    IPHONE_CAIPIAO = 20,               // iphone��Ʊ������
};

//====================================================================================
// ��Ϣ���Ͷ���
enum MsgType
{
    PRICE_WARN = 0,                     // �ɼ�Ԥ��
    SELF_STK = 1,                       // ��ѡ����Ϣ
    PUB_NOTICE = 2,                     // ������Ϣ
    LOTTERY_MSG = 3,                    // ��Ʊ��Ϣ
    YUNCAOPAN_MSG = 4,                  // �Ʋ�����Ϣ
    IMATTEN_MSG = 5,                    // IM������Ϣ
};
#pragma pack(pop)

#pragma pack()

//���������ֶ�ö������
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


//ѹ�������б�
enum PressData
{
	PressConnSucc,
	PressConnFail,
	PressSendSucc,
	PressSendFail,
	PressRecvSucc,
	PressRecvFail
};

