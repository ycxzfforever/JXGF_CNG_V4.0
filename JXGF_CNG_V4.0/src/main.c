#include "kernel.h"
GlobalVar        globalvar;                                     //全局变量
ShiftRecordInfo  shiftrecordinfo;                               //上下班记录信息
ShiftRecordInfo  shiftrecordinfo_tmp;
FuelRecordInfo   fuelrecordinfo;                                //加气流水记录信息
FuelRecordInfo   fuelrecordinfo_tmp;
Sysparas         sysparas;                                      //系统参数
Sysparas         tmp_sysparas;                                  //用户修改系统参数时，临时存储需要修改的参数
M1carddata       m1card;                                    //卡结构数据。

uint32_t         syserr;                                        //系统异常

uint8_t systemErrorArray[][16]={
	"Flash 1 检测失败",
	"Flash 2 检测失败",
	"    写主存错",
	"    读主存错",
	"	 写副存错",
	"    读副存错",
	"  写内部EE错误",
	"  读内部EE错误",
	"  写外部EE错误",
	"  读内部EE错误",
	"内部与EE内部差异",
	"  铁电测试失败",
	"  气相流量计错误",
	"  液相流量计错误",
	"  压力传感器错误",
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
//首界面显示
void DispFirst(void)
{
    globalvar.modbus_g_error=0;                                 //读气相流量计错次数清零
    timer1off();
    InputInit();

	VCTRL_CLOSEKEY();
	VCTRL_HIGH_CLOSE() ;
	VCTRL_MID_CLOSE();
	VCTRL_LOW_CLOSE();
	
//VCTRL_OPENKEY() ;
//VCTRL_HIGH_OPEN();
//    globalvar.workstate=StaIdle;                                //空闲。
    globalvar.fuelstep=0;                                      //加气步骤。
    globalvar.fix=0;                                            //定量标志。
    globalvar.displayonce=1;
    memset(&m1card.serialnumber[0],0,sizeof(m1card));                   //卡数据结构体清零
	memset(&fpvariedpar.ValvePreState,0,sizeof(struct FuelProcessVariedPar));
	
	if(syserr)
    {
        Fyd12864ClearLine(1);
        //Fyd12864DispPrintf(2,1,"系统错误:%ld",syserr);
		systemErrorDisplay();
		Fyd12864ClearLine(3);
        Fyd12864ClearLine(4);
		globalvar.workstate=StaFault;//故障
    }
    else
    {
		if(sysparas.yesnolock==1)//解锁
		{
			Fyd12864ClearLine(1);
			Fyd12864DispPrintf(2,1,"  加气机已锁!");
			Fyd12864DispPrintf(3,1,"  请解锁后使用");
			Fyd12864ClearLine(4);
			globalvar.workstate=Locked;//已锁
		}
		else
		{
			if(sysparas.offonwork==1)//上班
			{
				globalvar.workstate=StaIdle;                           //空闲。
				Fyd12864DispPrintf(1,1,"枪号:%02d   班号:%d",sysparas.gunnum,sysparas.shiftnum);
				Fyd12864DispPrintf(2,1,"气量:%ld.%02d",fuelrecordinfo.volume/100,fuelrecordinfo.volume%100);
				Fyd12864DispPrintf(3,1,"金额:%ld.%02d",fuelrecordinfo.money/100,fuelrecordinfo.money%100);
				if((sysparas.usecard==0)||(sysparas.usecard==2))
				{
					TaskAdd(KEY_START, FuelReady,NULL);   				//加气
					TaskAdd(KEY_FIX, FixGas,NULL);                      //定量加气
				}
				if(sysparas.offlinemode==0x01)			//脱机模式
				{
					TaskAdd(KEY_COMB_7KEY,OnWorkWithPwd,NULL);               //停止+7上班
					TaskAdd(KEY_COMB_8KEY,OffWorkWithPwd, NULL);             //停止+8下班
					if((globalvar.modbus_l_error==0xaa)&&(sysparas.devicetype==XQZ))
					{
						Fyd12864DispPrintf(4,1,"流量计通讯失败");
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
				else//在线模式
				{
					if(globalvar.comfailcount>10)                             //判断主板与后台是否断开连接。          
					{
						Fyd12864DispPrintf(4,1,"  加气机已掉线");
					}
					else if((globalvar.modbus_l_error==0xaa)&&(sysparas.devicetype==XQZ))
					{
						Fyd12864DispPrintf(4,1,"流量计通讯失败");
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
				if(sysparas.devicetype==XQZ)//泄气柱
				{			
					if(ModbusReadFlow(_MODBUS_G_,&realtime.cur_flowrate_g))	
					{
						globalvar.modbus_l_error=0;//这个变量原是LNG液相流量计使用，现临时作为
						                           //泄气柱时，流量计通信失败标志
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
				Fyd12864DispPrintf(2,1,"    下班中..");
				Fyd12864ClearLine(3);
				globalvar.workstate=StaOffDuty;                                //下班
				if(sysparas.offlinemode==1)			//脱机模式
				{
					TaskAdd(KEY_COMB_7KEY,OnWorkWithPwd,NULL);               //停止+7上班
					TaskAdd(KEY_COMB_8KEY,OffWorkWithPwd, NULL);             //停止+8下班
					Fyd12864ClearLine(4);
				}
				else
				{
					if(globalvar.comfailcount>10)                             //判断主板与后台是否断开连接。          
					{
						Fyd12864DispPrintf(4,1,"  加气机已掉线");
					}
					else
					{
						Fyd12864ClearLine(4);
					}
				}
			}
		}
    }
    TaskAdd(KEY_BACK, BackShow,NULL);       					//回显
    TaskAdd(KEY_QRY, DispQuery1,NULL);                          //查询。
    TaskAdd(KEY_SET, DispInputMenuPwd,NULL);                    //设置。
    TaskAdd(KEY_COMB_SBACK,DispInputQcPwd,NULL);                //停止+回显 修改质检参数    (还需根据质检模式修改)   
    TaskAdd(KEY_COMB_SQRY,DispInputTestPwd,NULL);               //停止+查询 硬件测试   
    TaskAdd(KEY_COMB_SSET,DispInputResetPwd, NULL);             //停止+菜单     
    if(globalvar.KeyValue == (KEY_STOP & KEY_OK))
		syserr = 0;
}


/*********************************************************************************************************
** 函数名称：main
** 函数描述：主函数,LPC1788模板
** 输入参数：无
** 返回值  ：系统返回
*********************************************************************************************************/
int main(void)
{

    MsDelay(200);                                                    //防止硬件电压还没准备好，导致初始化不成功
	SystemInit(); 
//SCB->VTOR  = 0x00001000 & 0x3FFFFF80;								//使能IAP需要用到
    KeyPinInit();                                                    //按键板初始化
	GPIOInit();                                                      //GPIO初始化
	Fyd12864Init();                                                  //主显屏初始化   
    I2C0Init(400000);                                                //i2c0初始化 最大400K
    I2C1Init(400000);                                                //i2c1初始化 最大400K
    EEPROMInit();                                                    //内部eeprom初始化
    At24c512Init();                                                  //外部eeprom初始化
    Fm25L04AndFlashSpiInit();	                                     //初始化spi0   
	ReadSysparas(0,(uint8_t*)&sysparas.head,sizeof(Sysparas));		 //获取系统参数
	UARTInit(1, sysparas.modbusbaud);     	                         //流量计
	UARTInit(3, sysparas.pcbaud);                                    //后台
	UARTInit(0, 9600);      	                                     //读卡器、打印机
	UARTInit(2, 9600);                                               //串口屏、DB9              
	UARTInit(4, 9600);      	                                     //电子标签	
    CloseUartRcvIRQ(3);                                              //关后台中断。
    WDDIR();                                                         //看门狗方向。
	timer0Init();                                                    //定时器0初始化
    timer1Init();                                                    //定时器1初始化
	
	Fm25l04Test();                                                                                    
    LockPortInit();                                                  //加密板set方向设置。
    PowerCheck();                                                    //掉电方向设置为输入。 
    DispUI=DispFirst;
    globalvar.comreceiveflag=0;                                           //后台有数据来临标志。
    timer0.flag_1s_circulatefue=0;                                   //循环加气时间控制。
    if(SSTFlashTest(0)) SetBit(syserr, SYSERR_FLASH_A_CHK);
    if(SSTFlashTest(1)) SetBit(syserr, SYSERR_FLASH_B_CHK); 
    MsDelay(100);
//    ReadSysparas(0,(uint8_t*)&sysparas.head,sizeof(Sysparas));
    MsDelay(100);
    Fm25l04Read(0,(uint8_t *)(&realtime.lcng),sizeof(RealTimeData)); //读铁电上次掉电保存数据。
    MsDelay(100);
    if((realtime.workstatus==StaFuel)||(realtime.workstatus==StaPay))//如果上次加气机正在加气或者扣费过程中掉电
    {
        PowerDownRealDate();
        MsDelay(1000);
    }else
    {
        if(sysparas.transnum>0) ReadFuelRecord(sysparas.transnum-1,0);
    }
    memset(&realtime.lcng,0,sizeof(RealTimeData));                   //清零实时数据。
    Fm25l04Write(0,(uint8_t *)(&realtime.lcng),sizeof(RealTimeData));//清铁电。   
    OsplcInit();                                                     //客显屏初始化   
    if(sysparas.shiftransnum>0) ReadFuelRecord(sysparas.shiftransnum-1,1);

	if(sysparas.offlinemode == 0)
	{
		OpenUartRcvIRQ(3);                                               //开后台串口中断。
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
			if(globalvar.comreceiveflag==1)                          //后代有命令来临标志。
            {
                BGCommunication(RecBack485Buf[4]);                   //后台命令解析。
                globalvar.comreceiveflag=0;
            }
        }
        if(timer0.flag_500ms==1)
        {	            
            timer0.flag_500ms=0;  
            Ds3232ReadTime();                                        //更新时间。
		    RunLed1();                                               //led1 500ms闪烁一次
		}          
        if(timer0.flag_1s==1)
        {
            timer0.flag_1s=0;
            FEEDDOG();                                               //喂狗。
            if((m1card.existflag==0)&&(sysparas.offlinemode!=1)&&(sysparas.usecard!=0x02))//无卡状态且在空闲时寻卡。
            {
				if((globalvar.workstate==StaIdle)||(globalvar.workstate==StaOffDuty))
				{
					if(Mt318GetCardSerialNum())                          //寻卡。
					{
					   DispUI=GetCardMessage;                               //读卡。
					}
				}
            }
			if(m1card.existflag==0x01)
			{
				if(!Mt318GetCardSerialNum()) //寻卡
				{
					m1card.escapeflag=0xaa;
				}
			}
        }
        if(globalvar.circulatefue==1)                              //是否循环加气。
		{
			if((globalvar.workstate==StaIdle)&&(m1card.existflag==0x01))//空闲
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
