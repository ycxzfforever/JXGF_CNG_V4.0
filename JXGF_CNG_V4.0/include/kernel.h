#ifndef KERNEL_H_
#define KERNEL_H_

#pragma pack(1)


#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"
#include "LPC177x_8x.h"
#include "type.h"
#include "ctrl.h"
#include "ssp.h"
#include "mt318.h"
#include "uart.h"
#include "fm25l04.h"
#include "timer.h"
#include "sst25vf064c.h"
#include "keyboard.h"
#include "fyd12864.h"
#include "printer.h"
#include "modbus.h"
#include "threedes.h"
#include "delay.h"
#include "cngctrl.h"
#include "turn.h"
#include "splc.h"
#include "at24c512.h"
#include "ds3232.h"
#include "eeprom.h"
#include "i2c0.h"
#include "i2c1.h"
#include "rtc.h"
#include "adc.h"
#include "menu.h"
#include "stdbool.h"
#include "math.h"   
#include "controlparas.h"
#include "query.h"
#include "crc.h"
#include "devicetest.h"
#include "cngmenu.h"
#include "cngfuelready.h"

#include "getpassword.h"

#include "app.h"
#include "cngcard.h"



//λ����
#define BIT0		0X00000001
#define BIT1		0X00000002
#define BIT2		0X00000004
#define BIT3		0X00000008
#define BIT4		0X00000010
#define BIT5		0X00000020
#define BIT6		0X00000040
#define BIT7		0X00000080
#define BIT8		0X00000100
#define BIT9		0X00000200
#define BIT10		0X00000400
#define BIT11		0X00000800
#define BIT12		0X00001000
#define BIT13		0X00002000
#define BIT14		0X00004000
#define BIT15		0X00008000
#define BIT16		0X00010000
#define BIT17		0X00020000
#define BIT18		0X00040000
#define BIT19		0X00080000
#define BIT20		0X00100000
#define BIT21		0X00200000
#define BIT22		0X00400000
#define BIT23		0X00800000
#define BIT24		0X01000000
#define BIT25		0X02000000
#define BIT26		0X04000000
#define BIT27		0X08000000
#define BIT28		0X10000000
#define BIT29		0X20000000
#define BIT30		0X40000000
#define BIT31		0X80000000

#define GetBit(dat,i) (((dat)&(1ul<<(i)))?1:0)  //�ж�dat��iλ�Ƿ�Ϊ1
#define SetBit(dat,i) ((dat)|=(1ul<<(i)))        //��dat��iλ��1
#define ClrBit(dat,i) ((dat)&=(~(1ul<<(i))))    //��dat��iλ��0

#define PowerCheck()  LPC_GPIO2->DIR &= ~(1ul<<27) // ����-���롣
#define PowerCheckIn   ((LPC_GPIO2->PIN)&(1ul<<27))//����ܽ������ƽ��⡣

//������״̬����
#define StaOffDuty  0xA0    // �°�
#define StaIdle     0xA1    // ����
#define StaFuel     0xA4    // ������
#define StaPay      0xA5    // ���۷�
#define StaClose    0xA6    //  �ر�
#define Locked      0xF0    //����������
#define StaFault    0xF1    // �豸����

#define JQJ				1
#define JQZ				2
#define XQZ				3
	
#define VERSION  "CTY.V4.0171127"

//���¼��Ϣ
typedef struct ShiftRecordInfo
{   
    uint32_t  classid;                //���°��¼��ˮ�ţ�4�ֽڡ�
    uint8_t   transactiontype;        //�������ͣ�1�ֽڡ�
    uint8_t   classnum;               //��ţ�1�ֽڡ�
    uint8_t   cardstaffnum[8];        //Ա�����ţ�BCD�롣
    uint8_t   classstarttime[7];      //�࿪ʼʱ�䣬7�ֽڣ�BCD�롣
    uint8_t   classendtime[7];        //�����ʱ�䣬7�ֽڣ�BCD�롣
    uint32_t  allgunnum;              //ǹ�����ۼƣ�4�ֽڡ�
    uint32_t  allgunmoney;            //ǹ����ۼ�,4�ֽڡ�
    uint32_t  classgasnum;            //��������4�ֽڡ�
    uint32_t  classgasmoney;          //����,4�ֽڡ�
    uint32_t  classtimes;             //�����������4�ֽڡ�
    uint8_t   devicetype;             //�豸���룬1�ֽڣ�0-LNG��1-��������2-��������3-й����
    uint8_t   measuretype;            //������λ��1�ֽڣ�0-�귽��1-Kg��
    uint32_t  classiding;             //�ϰ��¼��ˮ�š�
	//����Ϊ���ݲ�����������һ���洢������
	//��Ҫɾ�����¶�Ӧ���ֽ������Ա�֤crc��ȷ
    uint8_t     backup[20];
    uint16_t  crc;                    //crc�洢У�顣
}ShiftRecordInfo;


//������ˮ��¼��Ϣ
typedef struct FuelRecordInfo
{   
    uint32_t  transnum;               //������ˮ�ţ�4�ֽڡ�
    uint8_t   classnum;               //��ţ�1�ֽڡ�
    uint8_t   employeenum[8];         //Ա�����룬10�ֽڣ�BCD�롣
    uint8_t   transactiontype;        //�������ͣ�1�ֽڡ�
    uint8_t   usernum[8];             //�û����ţ�10�ֽ�,BCD�롣
    uint8_t   cardtype;               //������ �� 1�ֽڡ�
    uint8_t   carnum[9];              //���ƺ��룬7�ֽڣ�ASCII�롣
    uint8_t   stationnum[4];          //����վ���룬4�ֽڣ�BCD�롣
    uint8_t   starttime[7];           //������ʼʱ�䣬7�ֽڣ�BCD�롣
    uint8_t   endtime[7];             //��������ʱ�䣬7�ֽڣ�BCD�롣
    uint16_t  price;                  //���μ�������,2�ֽڡ�
    uint32_t  volume;                 //��ǰ����������4�ֽڡ�
    uint32_t  money;                  //��ǰ������4�ֽڡ�
    uint32_t  reallymoney;            //����ʵ�ʿۿ��
    uint32_t  beforebalance;          //����ǰ��4�ֽڡ�
    uint32_t  afterbalance;           //��������4�ֽڡ�
    uint32_t  allrechargetimes;       //�ۼƳ�ֵ������4�ֽڡ�
    uint32_t  allrechargemoney;       //�ۼƳ�ֵ��4�ֽڡ�
    uint32_t  allconsumptiontimes;    //�ۼ����Ѵ�����4�ֽڡ�
    uint32_t  allconsumptionnum;      //�ۼ�����������4�ֽڡ�
    uint32_t  allconsumptionmoney;    //�ۼ����ѽ�4�ֽڡ�
    uint32_t  allgunnum;              //ǹ�����ۼƣ�4�ֽڡ�
    uint32_t  allgunmoney;            //ǹ����ۼ�,4�ֽڡ�
    uint16_t  beforepressure;         //����ǰѹ����2�ֽڡ�
    uint16_t  afterpressure;          //������ѹ����2�ֽڡ�
    uint32_t  density;                //�ܶȣ�4�ֽڡ�
    uint32_t  curnum;                 //������4�ֽڡ�����ϵ����
    uint8_t   devicetype;             //�豸���룬1�ֽڣ�0-LNG��1-��������2-��������3-й����
    uint8_t   measuretype;            //������λ��1�ֽڣ�0-�귽��1-Kg��
    uint8_t   stopreason;             //ͣ��ԭ�� 
    uint32_t  jhid;                   //��������ˮ�š�
    uint32_t  classid;                //����ˮ�š�
	
	uint32_t  backgas;                //������
	//����Ϊ���ݲ�����������һ���洢������
	//��Ҫɾ�����¶�Ӧ���ֽ������Ա�֤crc��ȷ
    uint8_t     backup[20];
    uint16_t    crc;                  //16λ����У��
}FuelRecordInfo;

//����������
typedef struct def_SysParas
{
	//                      �� �� 
    uint8_t     head;       // 0    ͷ		
    uint32_t    writeE2num; // 1 	дeeprom����
    
    uint8_t     gunnum;     // 2    ǹ��
    uint32_t    transnum;   // 3    ������ˮ��     
    uint32_t    uptransnum; // 4    �Ѵ�������ˮ��     
    
    uint32_t    totvolume;  // 5    ǹ������    2λС��
    uint32_t    totmoney;   // 6    ǹ�ܽ��    2λС��

    uint16_t    price[20];  // 7    20�����ۣ�����0ΪĬ�ϵ��ۣ����ݵ��۵ȼ�ѡ����Ӧ����
                            //26
	
    uint8_t     modbustype; //27    ���������ͣ�1-��˹���أ�2-E+H
    uint8_t     unit;       //28    ������λ
    uint8_t     qcmode;     //29    �ʼ�ģʽ���� 

    uint32_t    densty;     //30    �ܶ�              4λС��//����4������������16λ�޸�Ϊ32λ���ұ���Ҫ�ı� 
    uint32_t    coeff;      //31    ����ϵ��  4λС��
    uint32_t    pzero;      //32    ѹ�����  
    uint32_t    pcoeff;     //33    ѹ��ϵ��  2λС��      
    
    uint32_t    modnum;     //34    �޸ļ�¼��ˮ��     
    uint32_t    upmodnum;   //35    �Ѵ��޸ļ�¼��ˮ��        
    
    //������Ϣ
    uint8_t     shiftnum;           // 36  ���
    uint8_t     shiftmaxnum;        // 37  �����
    uint32_t    shiftransnum;       // 38  ����ˮ��     
    uint32_t    shiftuptransnum;    // 39  �Ѵ�����ˮ��     
    uint32_t    shiftfueltimes;     // 40  ���������
    uint32_t    shiftstartvol;      // 41  �࿪ʼǹ��    2λС��
    uint32_t    shiftstartmon;      // 42  �࿪ʼ���    2λС��
    uint32_t    shiftotvol;         // 43  ���ۼ�����    2λС��
    uint32_t    shiftotmon;         // 44  ���ۼƽ��    2λС��
    uint8_t     shiftstarttime[6];  // 45  �࿪ʼʱ��   BCD
    uint8_t     shiftendtime[6];    // 46  �����ʱ��    BCD

    //ͨ����Ϣ
    uint8_t     shengcode[2];       // 47  ʡ����
    uint8_t     stationcode[4];     // 48  վ����
    uint8_t     citycode[2];        // 49  ���д���
    uint8_t     parentcode[4];      // 50  �ϼ���λ����

    //����Ϣ
    uint32_t    cardmaxyue;         // 51  ����������    2λС��
    uint32_t    cardminyue;         // 52  ������������    2λС��
	uint32_t	accountcardmaxyue;	// 53  ���˿��������    2λС��
    uint8_t     cardpsamnum;        // 54  psam������ѡ��
    uint16_t    cardmaxday;         // 55  ��δʹ�����������
    uint8_t     cardcheck;          // 56  �Ƿ�����ƿ
    uint8_t     usecard;            // 57  �Ƿ��ÿ�
    //����������
    uint32_t    modbusbaud;         // 58  �����Ʋ�����
    uint32_t    pcbaud;             // 59  ��̨������

    uint8_t     fuelmode;           // 60  ����ģʽ
    uint8_t     printmode;          // 61  СƱ��ӡ��ʽ

    //�汾�������
    uint8_t     pricever;           // 62  ���۰汾��
    uint8_t     blackver;           // 63  �������汾��
    uint8_t     whitever;           // 64  �������汾��
    uint8_t     addbkver;           // 65  �����������汾��
    uint8_t     delbkver;           // 66  ��ɾ�������汾��
    uint8_t     comminfover;        // 67  ͨ����Ϣ�汾��
	uint8_t     lockver;            // 68  ��Կ�汾��
	uint32_t	blacknum;  	        // 69  ����������
	uint32_t    blackaddnum;        // 70  ��������������
	uint32_t	blackdelnum;        // 71  ��ɾ����������
	uint32_t	whitenum;	        // 72  ������������

    //��������
    uint32_t    menupwd;            // 73  �˵���������
    uint32_t    qcpwd;              // 74  �ʼ�����
    uint32_t    testpwd;            // 75  �������룬�簴������ŷ��Ȳ���
    uint32_t    resetpwd;           // 76  �ָ���������
	uint32_t	onoffworkpwd;       // 77
    uint8_t     pwdboard[6];        // 78  ���ܰ�����
    uint8_t     usepwdboard;        // 79  �Ƿ�ʹ�ü��ܰ�               

	uint8_t		offlinemode;		// 80  �ѻ�ģʽ 1���ѻ� 0������
	uint8_t     lcdtype;            // 81  ��Ļѡ��0������1����
	uint8_t     offonwork;          // 82  ���������°�״̬ 1�ϰ࣬0�°�
    uint8_t     yesnolock;          // 83  ������������־  1������0����
    uint8_t     simulation;         // 84  1:ģ�������0����������
	uint8_t     intnumflag;         // 85  �Ƿ�ȡ��
	uint32_t    maxvolumeonce;      // 86  ������������
	uint32_t    maxmoneyonce;       // 87  ���������
	uint8_t     devicetype;         // 88  �豸����(LNG��ʱδ��)
	uint8_t     backup1[20];
	//CNG���Ʋ���	
	uint16_t    lowtomidflow;         //201  ��ת���з�����
	uint16_t    midtohighflow; 		  //202  ��ת���з�����
	uint16_t    lowstopflow;          //203  ������ͣ��
    uint16_t    maxsafeflow;          //204  ������ͣ��,��ȫ����
	
	uint16_t    stoppressure;         //205  ͣ��ѹ��
	uint16_t    valveswitchdelaytime; //206  �����ӳ�ʱ��
	uint16_t    filtertime;           //207  ������ʱ��
	uint16_t    flow20_5;			  //208  20.5��������
	uint16_t    flow21;               //209
    uint16_t    flow21_5; //210             
    uint16_t    flow22;   //211
    uint16_t    flow22_5; //212
    uint16_t    flow23;   //213
    uint16_t    flow23_5; //214
    uint16_t    flow24;   //215
    uint16_t    flow24_5; //216
    uint16_t    xqzstartflowrate;  //217й������������
	
	//LNG���Ʋ���
	uint16_t    backgascalc;        //201�Ƿ����������0-��������1-����
	uint8_t     backup2[30];       //����
	/************************end**********************/
    uint16_t     crc;               //16λ����У��
}Sysparas; 

//ȫ�ֱ���
typedef struct GlobalVar
{
    uint8_t         U8Temp;         //��ʱ�洢u8����ֵ
	uint32_t        U32Temp;        //��ʱ�洢u32����ֵ
	
    uint32_t        KeyValue;       //����ֵ,ȫ�ֱ���
    uint32_t        KeyPreValue;    //��ǰ����ֵ            
	
    uint8_t         MsgTime;        //������ʾĳЩ��̵���Ϣ
    uint8_t         MsgFlag;        //����1ʱ��msgδ����
	uint8_t         displayonce;    //����1ʱ������ֻ��ʾһ�λ�ִֻ��һ��
	
	uint8_t         circulatefue;   //ѭ���������á�1-ѭ����0-ֹͣ
	uint8_t         fuelstep;       //��������	
	uint32_t        fixnum;         //��������
    uint32_t        fixmoney;       //����
    uint8_t         fix;            //������־��1-��������2-����
	
    uint8_t         workstate;      //����������״̬ 
    uint8_t         BeepCount;      //���������ʱ�� 100msΪ��λ
    uint8_t         eepromflag;     //0-����Ҫ������1-��Ҫ����
	
	uint16_t        paraindex;      //����������
    uint32_t        xgtransnum;     //�޸���ˮ�ű��������ڲ�ѯ
	uint32_t        uptransnum;     //�ϴ���ˮʱ���Ѵ���ˮ��
    uint8_t         UploadIdCount;  //�ϴ���ˮʧ�ܴ���
	uint8_t         comreceiveflag;  //��̨�������־
    uint8_t         comfailcount;    //��̨������ͨ���Ƿ�������־���������10sδ�յ���̨��Ϣ�����ʾ�ѵ���
	
    uint8_t         modbus_g_error; //����ʱ�����������Ƴ�������
	uint8_t         modbus_l_error; //��Һʱ��Һ�������Ƴ�������
	uint8_t         pressure_error; //����ʱ��ѹ������
	
    uint32_t        beforedex;      //�����޸�ǰ��ֵ��
    uint32_t        afterdex;       //�����޸ĺ��ֵ��
    uint8_t         pointnum;       //����С���������
	
	uint8_t			startstate;		//���ӱ�ǩ������״̬������0xffʱû�б�ǩ�������١�
	uint8_t         startways;      //���ӱ�ǩ��������ʽ��
	uint8_t         startnum[8];    //���ӱ�ǩ���������кš�
	uint8_t         handnum;		//���ӱ�ǩ���ֳֻ���š�
	
	uint8_t         readLorG;       // 0-��ȡҺ�������ƣ�1-��ȡ����������
	
	uint8_t         devicetest;     //����1ʱ�ڽ��е�ŷ�����
}GlobalVar;

typedef	struct	M1carddata
{
	uint8_t		serialnumber[4];		//�������к�	
    uint32_t    key;                    //������
	uint8_t     keyA[6];
	uint8_t     keyB[6];
	uint8_t     existflag;	            //�Ƿ��ÿ���־ 
	uint8_t     checkflag;              //�鿨
	uint8_t     checkresult;            //�鿨���
	uint16_t    debittimes;             //����ۿ����
	uint8_t     graycard;               //�ҿ����
	uint8_t     blackorwhite;           // 1-���Ǻ��������߰���������2-�Ǻ��������߰�����
	uint8_t     shiftflag;              //�����־ 
	
	uint8_t     checkresultflag;        //�鿨�����־
	uint8_t     escapeflag;             //�ӿ����
    //��1.
	uint8_t		cardnum[8];				//BCD ����
	uint8_t		cardtype;				//������
	uint8_t		pricelevel;				//���۵ȼ�
	uint8_t		companycode[4];			//BCD ��˾����
	uint8_t		Shiftfunction;			//���๦��
	uint8_t		jurisdiction;			//Ȩ��	
    //��2.
	uint8_t		stationcode[4];			//BCD վ����
	uint8_t		mode;					//���ѷ�ʽ
	uint8_t		createcardtime[7];		//����ʱ��YYYYMMDDHHMMSS
	uint8_t		reserve0[4];			//����	
	//��4
		
	uint8_t		password[3];			//BCD ����
	uint8_t		customername[13];		//�ͻ�������������Ҳ��ո�
    //��5	
	uint8_t		balancecrc;				//�����У��
	uint8_t		lastoptmode;			//���һ�β�����ʽ
	uint8_t		lasttradetime[6];		//BCD �����ʱ��YYMMDDHHMMSS
	uint32_t	trademoney;				//���׶� 2λС��		
	uint32_t	cardbalance;			//�����  2λС��	
    //��6.
	uint8_t		backupblock[16];		//���ݿ�	
	//��8.	
	uint32_t	sumtimes;				//�ۼ����Ѵ���	
	uint32_t	summoney;				//�ۼ����ѽ��  2λС��	
	uint32_t	sumvolme;				//�ۼ���������  2λС��
	uint8_t		reserve1[4];			//����
    //��9.
	uint32_t	sumchargetimes;			//�ۼƳ�ֵ����
	uint32_t	sumchargemoney;			//�ۼƳ�ֵ���
	uint8_t		reserve2[8];			//����
    //��10.
	uint8_t		cardstate;				//��״̬
	uint8_t		gunnum;					//���һ�μ���ǹ��
	uint32_t	transnum;				//���һ�μ�����ˮ��
	uint8_t		laststationcode[4];		//BCD���һ�μ���վ����
	uint8_t		reserve3[6];			//����
    //��12.
	uint8_t		cylindersdate[4];		//��ƿ��������
	uint8_t		carnum[9];				//���ƺ�
	uint8_t		reserve4[3];			//����
}M1carddata;

extern M1carddata       m1card;        //���ṹ���ݡ�
extern uint32_t         syserr;            //ϵͳ�쳣

extern GlobalVar        globalvar;         //ȫ�ֱ���
extern ShiftRecordInfo  shiftrecordinfo;   //���°��¼
extern ShiftRecordInfo  shiftrecordinfo_tmp;
extern FuelRecordInfo   fuelrecordinfo;    //��ˮ��¼��Ϣ
extern FuelRecordInfo   fuelrecordinfo_tmp;
extern Sysparas         sysparas;          //ϵͳ����
extern Sysparas         tmp_sysparas;      //�û��޸�ϵͳ����ʱ����ʱ�洢��Ҫ�޸ĵĲ���

#endif