#include "kernel.h"
GlobalVar        globalvar;                                     //ȫ�ֱ���
ShiftRecordInfo  shiftrecordinfo;                               //���°��¼��Ϣ
ShiftRecordInfo  shiftrecordinfo_tmp;
FuelRecordInfo   fuelrecordinfo;                                //������ˮ��¼��Ϣ
FuelRecordInfo   fuelrecordinfo_tmp;
Sysparas         sysparas;                                      //ϵͳ����
Sysparas         tmp_sysparas;                                  //�û��޸�ϵͳ����ʱ����ʱ�洢��Ҫ�޸ĵĲ���
M1carddata       m1card;                                    //���ṹ���ݡ�

uint32_t         syserr;                                        //ϵͳ�쳣

uint8_t systemErrorArray[][16]={
	"Flash 1 ���ʧ��",
	"Flash 2 ���ʧ��",
	"    д�����",
	"    �������",
	"	 д�����",
	"    �������",
	"  д�ڲ�EE����",
	"  ���ڲ�EE����",
	"  д�ⲿEE����",
	"  ���ڲ�EE����",
	"�ڲ���EE�ڲ�����",
	"  �������ʧ��",
	"  ���������ƴ���",
	"  Һ�������ƴ���",
	"  ѹ������������",
};
void systemErrorDisplay(void)
{
	uint8_t index;
	if((syserr&0x7fff)!=0)
	{
		if(syserr&0x0001)
		{
			index=0;
		}
		else if(syserr&0x0002)
		{
			index=1;
		}
		else if(syserr&0x0004)
		{
			index=2;
		}
		else if(syserr&0x0008)
		{
			index=3;
		}
		else if(syserr&0x0010)
		{
			index=4;
		}
		else if(syserr&0x0020)
		{
			index=5;
		}
		else if(syserr&0x0040)
		{
			index=6;
		}
		else if(syserr&0x0080)
		{
			index=7;
		}
		else if(syserr&0x0100)
		{
			index=8;
		}
		else if(syserr&0x0200)
		{
			index=9;
		}
		else if(syserr&0x0400)
		{
			index=10;
		}
		else if(syserr&0x0800)
		{
			index=11;
		}
		else if(syserr&0x1000)
		{
			index=12;
		}
		else if(syserr&0x2000)
		{
			index=13;
		}
		else if(syserr&0x4000)
		{
			index=14;
		}
		Fyd12864DispPrintf(2,1,(char *)&systemErrorArray[index][0]);
	}
}
//�׽�����ʾ
void DispFirst(void)
{
    globalvar.modbus_g_error=0;                                 //�����������ƴ��������
    timer1off();
    InputInit();

	VCTRL_CLOSEKEY();
	VCTRL_HIGH_CLOSE() ;
	VCTRL_MID_CLOSE();
	VCTRL_LOW_CLOSE();
	
//VCTRL_OPENKEY() ;
//VCTRL_HIGH_OPEN();
//    globalvar.workstate=StaIdle;                                //���С�
    globalvar.fuelstep=0;                                      //�������衣
    globalvar.fix=0;                                            //������־��
    globalvar.displayonce=1;
    memset(&m1card.serialnumber[0],0,sizeof(m1card));                   //�����ݽṹ������
	memset(&fpvariedpar.ValvePreState,0,sizeof(struct FuelProcessVariedPar));
	
	if(syserr)
    {
        Fyd12864ClearLine(1);
        //Fyd12864DispPrintf(2,1,"ϵͳ����:%ld",syserr);
		systemErrorDisplay();
		Fyd12864ClearLine(3);
        Fyd12864ClearLine(4);
		globalvar.workstate=StaFault;//����
    }
    else
    {
		if(sysparas.yesnolock==1)//����
		{
			Fyd12864ClearLine(1);
			Fyd12864DispPrintf(2,1,"  ����������!");
			Fyd12864DispPrintf(3,1,"  �������ʹ��");
			Fyd12864ClearLine(4);
			globalvar.workstate=Locked;//����
		}
		else
		{
			if(sysparas.offonwork==1)//�ϰ�
			{
				globalvar.workstate=StaIdle;                           //���С�
				Fyd12864DispPrintf(1,1,"ǹ��:%02d   ���:%d",sysparas.gunnum,sysparas.shiftnum);
				Fyd12864DispPrintf(2,1,"����:%ld.%02d",fuelrecordinfo.volume/100,fuelrecordinfo.volume%100);
				Fyd12864DispPrintf(3,1,"���:%ld.%02d",fuelrecordinfo.money/100,fuelrecordinfo.money%100);
				if((sysparas.usecard==0)||(sysparas.usecard==2))
				{
					TaskAdd(KEY_START, FuelReady,NULL);   				//����
					TaskAdd(KEY_FIX, FixGas,NULL);                      //��������
				}
				if(sysparas.offlinemode==0x01)			//�ѻ�ģʽ
				{
					TaskAdd(KEY_COMB_7KEY,OnWorkWithPwd,NULL);               //ֹͣ+7�ϰ�
					TaskAdd(KEY_COMB_8KEY,OffWorkWithPwd, NULL);             //ֹͣ+8�°�
					if((globalvar.modbus_l_error==0xaa)&&(sysparas.devicetype==XQZ))
					{
						Fyd12864DispPrintf(4,1,"������ͨѶʧ��");
					}
					else
					{
						globalvar.U8Temp++;
						if(globalvar.U8Temp<0x05)
						{
							Fyd12864DispPrintf(4,1,"20%02X-%02X-%02X %02X %02X",
							time.year,time.month,time.day,time.hour,time.minute);
						}
						else if((globalvar.U8Temp>=0x05)&&(globalvar.U8Temp<10))
						{
							Fyd12864DispPrintf(4,1,"20%02X-%02X-%02X %02X:%02X",
							time.year,time.month,time.day,time.hour,time.minute);
						}
						else if(globalvar.U8Temp>=10)
						{
							globalvar.U8Temp=0;
						} 
					}
				}
				else//����ģʽ
				{
					if(globalvar.comfailcount>10)                             //�ж��������̨�Ƿ�Ͽ����ӡ�          
					{
						Fyd12864DispPrintf(4,1,"  �������ѵ���");
					}
					else if((globalvar.modbus_l_error==0xaa)&&(sysparas.devicetype==XQZ))
					{
						Fyd12864DispPrintf(4,1,"������ͨѶʧ��");
					}
					else
					{
						globalvar.U8Temp++;
						if(globalvar.U8Temp<0x05)
						{
							Fyd12864DispPrintf(4,1,"20%02X-%02X-%02X %02X %02X",
							time.year,time.month,time.day,time.hour,time.minute);
						}
						else if((globalvar.U8Temp>=0x05)&&(globalvar.U8Temp<10))
						{
							Fyd12864DispPrintf(4,1,"20%02X-%02X-%02X %02X:%02X",
							time.year,time.month,time.day,time.hour,time.minute);
						}
						else if(globalvar.U8Temp>=10)
						{
							globalvar.U8Temp=0;
						}
					}
				}
				if(sysparas.devicetype==XQZ)//й����
				{			
					if(ModbusReadFlow(_MODBUS_G_,&realtime.cur_flowrate_g))	
					{
						globalvar.modbus_l_error=0;//�������ԭ��LNGҺ��������ʹ�ã�����ʱ��Ϊ
						                           //й����ʱ��������ͨ��ʧ�ܱ�־
						if((uint32_t)(realtime.cur_flowrate_g*100)>sysparas.xqzstartflowrate)
						{
							DispUI=FuelReady;
						}
					}
					else
					{
						globalvar.modbus_l_error=0xaa;
					}
				}	
			}
			else
			{
				Fyd12864ClearLine(1);
				Fyd12864DispPrintf(2,1,"    �°���..");
				Fyd12864ClearLine(3);
				globalvar.workstate=StaOffDuty;                                //�°�
				if(sysparas.offlinemode==1)			//�ѻ�ģʽ
				{
					TaskAdd(KEY_COMB_7KEY,OnWorkWithPwd,NULL);               //ֹͣ+7�ϰ�
					TaskAdd(KEY_COMB_8KEY,OffWorkWithPwd, NULL);             //ֹͣ+8�°�
					Fyd12864ClearLine(4);
				}
				else
				{
					if(globalvar.comfailcount>10)                             //�ж��������̨�Ƿ�Ͽ����ӡ�          
					{
						Fyd12864DispPrintf(4,1,"  �������ѵ���");
					}
					else
					{
						Fyd12864ClearLine(4);
					}
				}
			}
		}
    }
    TaskAdd(KEY_BACK, BackShow,NULL);       					//����
    TaskAdd(KEY_QRY, DispQuery1,NULL);                          //��ѯ��
    TaskAdd(KEY_SET, DispInputMenuPwd,NULL);                    //���á�
    TaskAdd(KEY_COMB_SBACK,DispInputQcPwd,NULL);                //ֹͣ+���� �޸��ʼ����    (��������ʼ�ģʽ�޸�)   
    TaskAdd(KEY_COMB_SQRY,DispInputTestPwd,NULL);               //ֹͣ+��ѯ Ӳ������   
    TaskAdd(KEY_COMB_SSET,DispInputResetPwd, NULL);             //ֹͣ+�˵�     
    if(globalvar.KeyValue == (KEY_STOP & KEY_OK))
		syserr = 0;
}


/*********************************************************************************************************
** �������ƣ�main
** ����������������,LPC1788ģ��
** �����������
** ����ֵ  ��ϵͳ����
*********************************************************************************************************/
int main(void)
{

    MsDelay(200);                                                    //��ֹӲ����ѹ��û׼���ã����³�ʼ�����ɹ�
	SystemInit(); 
//SCB->VTOR  = 0x00001000 & 0x3FFFFF80;								//ʹ��IAP��Ҫ�õ�
    KeyPinInit();                                                    //�������ʼ��
	GPIOInit();                                                      //GPIO��ʼ��
	Fyd12864Init();                                                  //��������ʼ��   
    I2C0Init(400000);                                                //i2c0��ʼ�� ���400K
    I2C1Init(400000);                                                //i2c1��ʼ�� ���400K
    EEPROMInit();                                                    //�ڲ�eeprom��ʼ��
    At24c512Init();                                                  //�ⲿeeprom��ʼ��
    Fm25L04AndFlashSpiInit();	                                     //��ʼ��spi0   
	ReadSysparas(0,(uint8_t*)&sysparas.head,sizeof(Sysparas));		 //��ȡϵͳ����
	UARTInit(1, sysparas.modbusbaud);     	                         //������
	UARTInit(3, sysparas.pcbaud);                                    //��̨
	UARTInit(0, 9600);      	                                     //����������ӡ��
	UARTInit(2, 9600);                                               //��������DB9              
	UARTInit(4, 9600);      	                                     //���ӱ�ǩ	
    CloseUartRcvIRQ(3);                                              //�غ�̨�жϡ�
    WDDIR();                                                         //���Ź�����
	timer0Init();                                                    //��ʱ��0��ʼ��
    timer1Init();                                                    //��ʱ��1��ʼ��
	
	Fm25l04Test();                                                                                    
    LockPortInit();                                                  //���ܰ�set�������á�
    PowerCheck();                                                    //���緽������Ϊ���롣 
    DispUI=DispFirst;
    globalvar.comreceiveflag=0;                                           //��̨���������ٱ�־��
    timer0.flag_1s_circulatefue=0;                                   //ѭ������ʱ����ơ�
    if(SSTFlashTest(0)) SetBit(syserr, SYSERR_FLASH_A_CHK);
    if(SSTFlashTest(1)) SetBit(syserr, SYSERR_FLASH_B_CHK); 
    MsDelay(100);
//    ReadSysparas(0,(uint8_t*)&sysparas.head,sizeof(Sysparas));
    MsDelay(100);
    Fm25l04Read(0,(uint8_t *)(&realtime.lcng),sizeof(RealTimeData)); //�������ϴε��籣�����ݡ�
    MsDelay(100);
    if((realtime.workstatus==StaFuel)||(realtime.workstatus==StaPay))//����ϴμ��������ڼ������߿۷ѹ����е���
    {
        PowerDownRealDate();
        MsDelay(1000);
    }else
    {
        if(sysparas.transnum>0) ReadFuelRecord(sysparas.transnum-1,0);
    }
    memset(&realtime.lcng,0,sizeof(RealTimeData));                   //����ʵʱ���ݡ�
    Fm25l04Write(0,(uint8_t *)(&realtime.lcng),sizeof(RealTimeData));//�����硣   
    OsplcInit();                                                     //��������ʼ��   
    if(sysparas.shiftransnum>0) ReadFuelRecord(sysparas.shiftransnum-1,1);

	if(sysparas.offlinemode == 0)
	{
		OpenUartRcvIRQ(3);                                               //����̨�����жϡ�
	}
	
    while(1)
    {        
		realtime.workstatus=globalvar.workstate;
        KeyGetValue();  
        if(timer0.flag_100ms==1)
        {
            timer0.flag_100ms=0;
            if((globalvar.BeepCount--)==0){    BEEP_OFF();}
            if(globalvar.MsgTime>0)  globalvar.MsgTime--;   			
            DispUI();
			globalvar.KeyValue=0;
			RecBlckDataCheck();
			if(globalvar.comreceiveflag==1)                          //������������ٱ�־��
            {
                BGCommunication(RecBack485Buf[4]);                   //��̨���������
                globalvar.comreceiveflag=0;
            }
        }
        if(timer0.flag_500ms==1)
        {	            
            timer0.flag_500ms=0;  
            Ds3232ReadTime();                                        //����ʱ�䡣
		    RunLed1();                                               //led1 500ms��˸һ��
		}          
        if(timer0.flag_1s==1)
        {
            timer0.flag_1s=0;
            FEEDDOG();                                               //ι����
            if((m1card.existflag==0)&&(sysparas.offlinemode!=1)&&(sysparas.usecard!=0x02))//�޿�״̬���ڿ���ʱѰ����
            {
				if((globalvar.workstate==StaIdle)||(globalvar.workstate==StaOffDuty))
				{
					if(Mt318GetCardSerialNum())                          //Ѱ����
					{
					   DispUI=GetCardMessage;                               //������
					}
				}
            }
			if(m1card.existflag==0x01)
			{
				if(!Mt318GetCardSerialNum()) //Ѱ��
				{
					m1card.escapeflag=0xaa;
				}
			}
        }
        if(globalvar.circulatefue==1)                              //�Ƿ�ѭ��������
		{
			if((globalvar.workstate==StaIdle)&&(m1card.existflag==0x01))//����
			{	
				if((m1card.checkresultflag==1)&&(m1card.checkresult==0))
				{
					TaskMessage(50,CirculateFueling);
				}
			}
		}
    }
}


/*********************************************************************************************************
**                            End Of File
*********************************************************************************************************/
