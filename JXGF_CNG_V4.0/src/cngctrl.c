#include "kernel.h"

/***************************************************************************
**函数名称： JudgeStopConditions
**函数功能：判断停机条件
**入口参数：无
**返回值：无
**创建者：杨朝旭
**创建日期：2016-5-10 13:06:28
**
**修改者：
**修改日期：
**修改内容：
***************************************************************************/
void JudgeStopConditions(void)
{
    if(globalvar.fix==1)//定气量。
    {
        if((fuelrecordinfo.volume+1)>=globalvar.fixnum)
        {
            fuelrecordinfo.volume=globalvar.fixnum;
            fpvariedpar.stopreason= 0x06;
        }
    }
    if(globalvar.fix==2)//定金额。
    {
        if((fuelrecordinfo.money+1)>=globalvar.fixmoney)
        {
            fuelrecordinfo.money=globalvar.fixmoney;
            fpvariedpar.stopreason = 0x07;
        }
    }
    if(m1card.existflag==1)//如果是用卡加气。
    {
		if(m1card.escapeflag==0xaa)
		{
			fpvariedpar.stopreason=0x0d;
			globalvar.fix=0;
		}
		if(m1card.cardtype==2)//记账卡。
        {
            if((fuelrecordinfo.money+m1card.cardbalance)>=sysparas.cardmaxyue)//记账卡余额上限。
            {
                fpvariedpar.stopreason=0x0e;
				globalvar.fix=0;
            }
			
        }     
    }
}

/***************************************************************************
**函数名称：PowerDown
**函数功能：掉电显示函数。
**入口参数：无
**返 回 值：无
**创 建 者：杨传坤
**创建日期：2016-7-25
**
**修改者：
**修改日期：
**修改内容：
***************************************************************************/
void PowerDown(void)
{
    PUMPCTRL_HIGH_OPEN();	//PLC
	PUMPCTRL_LOW_OPEN();	//PLC
	VCTRL_CLOSEKEY();
	VCTRL_HIGH_CLOSE() ;
	VCTRL_MID_CLOSE();
	VCTRL_LOW_CLOSE();                                             //停机。
    Fyd12864DispPrintf(1,1,"系统已经掉电...");
    Fyd12864DispPrintf(4,1,"数据已保存");
    DispUI=PowerDown;
}

/***************************************************************************
**函数名称：PowerDownRealDate
**函数功能：掉电数据恢复函数。
**入口参数：无
**返 回 值：无
**创 建 者：杨传坤
**创建日期：2016-7-25
**
**修改者：
**修改日期：
**修改内容：
***************************************************************************/
void PowerDownRealDate(void)
{
    sysparas.transnum=realtime.transnum;                                //流水号。
    sysparas.totvolume+=realtime.volume;                                //枪累气量。
    sysparas.totmoney+=realtime.money;                                  //枪累金额。
    sysparas.shiftotvol+=realtime.volume;                               //班累气量。
    sysparas.shiftotmon+=realtime.money;                                //班累气量。
    sysparas.shiftfueltimes++;                                          //班加气次数。
    
    fuelrecordinfo.transnum=realtime.transnum;                          //流水号。
    fuelrecordinfo.classnum=sysparas.shiftnum;                          //班号。;
    memcpy(&fuelrecordinfo.employeenum[0],&realtime.employeenum[0],8);  //员工号。
    fuelrecordinfo.transactiontype=realtime.transactiontype;            //逃卡交易。
    memcpy(&fuelrecordinfo.usernum[0],&realtime.cardnum[0],8);          //用户卡号。
    fuelrecordinfo.cardtype=realtime.cardtype;                          //卡类型。
    memcpy(&fuelrecordinfo.carnum[0],&realtime.carnum[0],9);            //车牌号。
    memcpy(&fuelrecordinfo.stationnum[0],&sysparas.stationcode[0],4);   //站编码。
    memcpy(&fuelrecordinfo.starttime[0],&realtime.starttime[0],7);      //加气开始时间。
    memcpy(&fuelrecordinfo.endtime[0],&realtime.endtime[0],7);          //加气结束时间。
   
    fuelrecordinfo.price=realtime.price;                                //单价。
    fuelrecordinfo.volume=realtime.volume;                              //气量。
    fuelrecordinfo.money=realtime.money;                                //金额。
   
    fuelrecordinfo.reallymoney=realtime.reallymoney;                    //实际扣款金额。
    fuelrecordinfo.beforebalance=realtime.beforebalance;                //加气前余额。
    fuelrecordinfo.afterbalance=realtime.afterbalance;                  //加气后余额。
    fuelrecordinfo.allrechargetimes=realtime.allrechargetimes;          //累计充值次数。
    
   
    fuelrecordinfo.allrechargemoney=realtime.allrechargemoney;          //累计充值金额。
    
    fuelrecordinfo.allconsumptiontimes=realtime.allconsumptiontimes;    //累计消费次数。
    fuelrecordinfo.allconsumptionnum=realtime.allconsumptionnum;        //累计消费气量。
    fuelrecordinfo.allconsumptionmoney=realtime.allconsumptionmoney;    //累计消费金额。
    fuelrecordinfo.allgunnum=sysparas.totvolume;                        //枪累气量。
    fuelrecordinfo.allgunmoney=sysparas.totmoney;                       //枪累金额。
    fuelrecordinfo.beforepressure=realtime.beforepressure;              //加气前压力。
    fuelrecordinfo.afterpressure=realtime.afterpressure;                //加气后压力。
    fuelrecordinfo.density=(uint32_t)(realtime.cur_density_g*10000+0.5);//密度。
    fuelrecordinfo.curnum=realtime.curnum;                              //当量。
    fuelrecordinfo.devicetype=realtime.lcng;                            //气品代码。
    fuelrecordinfo.measuretype=realtime.measuretype;                    //计量单位。
    fuelrecordinfo.stopreason=realtime.stopreason;                      //停机原因。
    fuelrecordinfo.jhid=realtime.jhid;                                  //解灰相关流水号。
    fuelrecordinfo.classid=realtime.classid;                            //班流水号。
    fuelrecordinfo.crc=CRC16Calculate((uint8_t *)(&fuelrecordinfo.transnum),sizeof(FuelRecordInfo)-2);
    
    
    SaveSysparas(0,(uint8_t*)&sysparas.head,sizeof(Sysparas));
    MsDelay(100);
    if(SaveFuelRecord(0))
    {
        Fyd12864DispPrintf(4,1,"存储流水成功"); 
    }
    else
    {
        Fyd12864DispPrintf(4,1,"存储流水失败");  
    }
}

/***************************************************************************
**函数名称： FuelReady
**函数功能：加气前准备
**入口参数：无
**返回值：无
**创建者：黄豪民
**创建日期：2016-4-26 16:17:52
**
**修改者：
**修改日期：
**修改内容：
***************************************************************************/
void FuelReady(void)
{
	uint32_t  availablemoney;
	if(sysparas.simulation)
	{
		timer1on();
		globalvar.modbus_g_error = 0;
		globalvar.pressure_error=0;
	}
	else
	{
		ModbusReadTotal(_MODBUS_G_,&realtime.cur_totalize_g);
		if(realtime.cur_totalize_g > 90000)
		{
			if(!ModbusClearTotal(_MODBUS_G_))
			{
				Fyd12864ClearLine(1);
				Fyd12864ClearLine(2);
				Fyd12864ClearLine(4);
				Fyd12864DispPrintf(3,1,"  流量计清零错误");
				TaskMessage(20,DispFirst);
				return;
			}
		}												//清零流量计
		if(globalvar.displayonce==1)
		{
			globalvar.displayonce = 0;
			globalvar.modbus_g_error = 1;
			globalvar.pressure_error=1;
			timer1on();		
		}
	}
	if(globalvar.modbus_g_error)
	{
		Fyd12864ClearLine(1);
		Fyd12864ClearLine(3);
		Fyd12864ClearLine(4);
		Fyd12864DispPrintf(2,1,"  读取流量计中...");
		TaskAdd(KEY_STOP, DispFirst, NULL);
	}
	else  if(globalvar.pressure_error)
	{
		Fyd12864ClearLine(1);
		Fyd12864ClearLine(3);
		Fyd12864ClearLine(4);
		Fyd12864DispPrintf(2,1,"  读压力失败");
		TaskMessage(30,DispFirst);
	}
	else
	{	
		memset(&fpvariedpar.ValvePreState,0,sizeof(struct FuelProcessVariedPar));
		fpvariedpar.stopreason=0xaa;//初始值
		//获取比例系数
		if(sysparas.unit==0)		//计量单位  质量、体积?
		{
			fpvariedpar.calfactor=(float)(sysparas.coeff/10000.00);	//修正系数（比例系数）质量
		}
		else 
		{
			fpvariedpar.calfactor=((float)sysparas.coeff)/((float)sysparas.densty); //体积	质量 / 密度
		}

		globalvar.fuelstep=1;                                                    //加气步骤1。
		globalvar.workstate=StaFuel;                                              //加气状态。
		
		memset(&fuelrecordinfo,0,sizeof(FuelRecordInfo));                         //清流水结构体。
		fuelrecordinfo.classnum=sysparas.shiftnum;                                //班号。
		memcpy(&fuelrecordinfo.employeenum[0],&shiftrecordinfo.cardstaffnum[0],8);//员工号。
		memcpy(&fuelrecordinfo.usernum[0],&m1card.cardnum[0],8);              //用户卡号。
		memcpy(&fuelrecordinfo.carnum[0],&m1card.carnum[0],9);                //车牌号。
		fuelrecordinfo.cardtype=m1card.cardtype;                              //卡类型。 		
		memcpy(&fuelrecordinfo.stationnum[0],&sysparas.stationcode[0],4);         //站编码。
		fuelrecordinfo.starttime[0]=0x20;
		memcpy(&fuelrecordinfo.starttime[1], &time.year, 6);                      //加气开始时间。		
		fuelrecordinfo.devicetype =sysparas.devicetype;                           //设备类型                                                 //气品代码，1字节。0-CNG
		fuelrecordinfo.beforepressure=realtime.cur_pressure;                      //加气前压力。    
		fuelrecordinfo.beforebalance=m1card.cardbalance;                      //加气前余额。
		fuelrecordinfo.afterbalance=m1card.cardbalance;                       //加气后余额。
		fuelrecordinfo.allrechargemoney=m1card.sumchargemoney;                //累计充值金额
		fuelrecordinfo.allrechargetimes=m1card.sumchargetimes;                //累计充值次数。
		fuelrecordinfo.allconsumptiontimes=m1card.sumtimes+1;                 //累计消费次数。
	   
		fpvariedpar.startgasvolume=realtime.cur_totalize_g;    					  //记录起始气量
//		fpvariedpar.startgasvolume=(unsigned long)(fpvariedpar.startgasvolume*100); //不需要取两位小数
//		fpvariedpar.startgasvolume=(float)(fpvariedpar.startgasvolume/100.00);		//否则会出现开始气量就出现0.01的情况
			
		realtime.powerdown=0;                                                     //未掉电标志。
		if(m1card.existflag)
		{
			fuelrecordinfo.price=sysparas.price[m1card.pricelevel];               //有卡交易价格。
			realtime.price=sysparas.price[m1card.pricelevel];					  //有卡当前使用的单价
			if(m1card.cardtype==1)//储蓄卡  卡余额作为定金额处理
			{
				availablemoney=m1card.cardbalance-sysparas.cardminyue; 
				if(globalvar.fix==1)//之前按定量加气
				{
					if((globalvar.fixnum*fuelrecordinfo.price)>(availablemoney*100))
					{
						globalvar.fix=2;
						globalvar.fixmoney=availablemoney;
					}
				}
				else if(globalvar.fix==2)
				{
					if(globalvar.fixmoney>availablemoney)
					{
						globalvar.fixmoney=availablemoney; 
					}
				} 
				else
				{
					globalvar.fix=2;
					globalvar.fixmoney=availablemoney; 
				}
			}
		}
		else
		{
			fuelrecordinfo.price=sysparas.price[0];              					 //交易价格。
			realtime.price=sysparas.price[0];					  					//当前使用的单价
		}

		SaveFm25l04();                                                            //写铁电。
		if(sysparas.simulation)
		{
			realtime.cur_flowrate_g = 9;
		}
		if((sysparas.gunnum==0)||(sysparas.gunnum>99))
		{
			
			Fyd12864ClearLine(1);
			Fyd12864ClearLine(3);
			Fyd12864ClearLine(4);
			Fyd12864DispPrintf(2,1,"  请设置枪号");
			TaskMessage(20, DispFirst);

		}
		else if(realtime.price < 10 || realtime.price > 10000)
		{
			Fyd12864ClearLine(1);
			Fyd12864ClearLine(3);
			Fyd12864ClearLine(4);
			Fyd12864DispPrintf(2,1,"  单价错误");
			TaskMessage(20, DispFirst);
		}
		else
		{
			if(m1card.existflag)
			{

				HuiIccard();
			}
			else
			{
				DispUI=FuelWork;
			}
		}
	}
}

//加液显示
//double    fpvariedpar.cngvolume=0;//加气量
/***************************************************************************
**函数名称： FuelWork
**函数功能：加气结束
**入口参数：无
**返回值：无 
**创建者：黄豪民
**创建日期：2016-11-10 16:17:52
**
**修改者：
**修改日期：
**修改内容：
***************************************************************************/
void FuelWork(void)
{
	globalvar.displayonce = 1;
	globalvar.workstate = StaFuel;
    globalvar.fuelstep=2;//加气步骤2
	if(globalvar.KeyValue==KEY_STOP)
	{
		globalvar.fix=0;
		fpvariedpar.stopreason=0;
	}
	
	if(sysparas.intnumflag==0x01)
	{
		if(globalvar.KeyValue==KEY_PT)//按小数点取整按定金额处理
		{
			globalvar.fix=0x7;          //定金额。
			globalvar.fixmoney=((fuelrecordinfo.money+100)/100)*100;        
		}
	}
    if(sysparas.simulation)
    {
        fpvariedpar.cngvolume+=0.13;
        realtime.cur_flowrate_g -= 0.01;
        realtime.cur_pressure += 1;
        if(realtime.cur_pressure>1500){realtime.cur_pressure=1500;}
        if(realtime.cur_flowrate_g>400){realtime.cur_flowrate_g=400;}
    }
    else
    {
        fpvariedpar.cngvolume=realtime.cur_totalize_g-fpvariedpar.startgasvolume;
		fpvariedpar.cngvolume = fpvariedpar.cngvolume * fpvariedpar.calfactor;		//乘上比例系数 		
    }
 
    fuelrecordinfo.volume=(uint32_t)(fpvariedpar.cngvolume*100);//当前交易气量。   
    fpvariedpar.cngvolume = (double)fuelrecordinfo.volume/100.0;
    fuelrecordinfo.money=(uint32_t)(fpvariedpar.cngvolume*realtime.price+0.5);//当前交易金额。
			
	if(!ValveSwitchByMassflow((uint32_t)(realtime.cur_flowrate_g*10)))		//流速切阀
	{
		globalvar.fix=0;
		fpvariedpar.stopreason=0x08;	
	}
	
	if(!PressureWithFlow((uint16_t)realtime.cur_flowrate_g, realtime.cur_pressure))	//压力停机、流速停机
	{
		globalvar.fix=0;
	}
    
    JudgeStopConditions();//判断停机条件。
    
	realtime.volume=fuelrecordinfo.volume;
	realtime.money=fuelrecordinfo.money;    
	OsplcShow(fuelrecordinfo.volume,fuelrecordinfo.money,realtime.price,0,0);//蓝屏显示。
	
    Fyd12864DispPrintf(2,1,"气量:%ld.%02d",fuelrecordinfo.volume/100,fuelrecordinfo.volume%100);
    Fyd12864DispPrintf(3,1,"金额:%ld.%02d",fuelrecordinfo.money/100,fuelrecordinfo.money%100);
    Fyd12864DispPrintf(4,1,"V:%.02f P:%ld.%02d",realtime.cur_flowrate_g,realtime.cur_pressure/100,realtime.cur_pressure%100);  
	
	if(fuelrecordinfo.volume>sysparas.maxvolumeonce)
	{
		fpvariedpar.stopreason=0x0f;
		globalvar.fix=0;
	}
	if(fuelrecordinfo.money>sysparas.maxmoneyonce)
	{
		fpvariedpar.stopreason=0x0f;
		globalvar.fix=0;
	}
	if(fuelrecordinfo.money>(fpvariedpar.moneybackup + realtime.price/2))
	{
		fpvariedpar.stopreason=0x03;
		fpvariedpar.stoperrorflag=0xaa;
		globalvar.fix=0;
	}
	else
	{
		fpvariedpar.moneybackup = fuelrecordinfo.money;
		fpvariedpar.volumebackup = fuelrecordinfo.volume;
	}
	if(fpvariedpar.stopreason!=0xaa)//初始值
	{
		globalvar.KeyValue=KEY_STOP;
	}
    TaskAdd(KEY_STOP,FuelEnd, NULL);    
    realtime.powerdown=0;          //未掉电标志。
    SaveFm25l04();                 //写铁电。
	
    if(PowerCheckIn!=0)            //判断是否掉电。 
    { 
        MsDelay(20);
        if(PowerCheckIn!=0)
        {
			fpvariedpar.stopreason=0x05;//掉电
            realtime.powerdown=1;      //已掉电。
            SaveFm25l04();             //写铁电。
            PowerDown();
        }
    }
	timer1.tick = 0;
}

void FuelEndDisplay(void)
/***************************************************************************
**函数名称： FuelEndDisplay
**函数功能：加气结束存储和打印
**入口参数：无
**返回值：无 
**创建者：黄豪民
**创建日期：2016-11-10 16:17:52
**
**修改者：
**修改日期：
**修改内容：
***************************************************************************/
{
	if(globalvar.displayonce==1)
	{
		globalvar.displayonce=0;
		globalvar.workstate=StaIdle;
		if(SaveFuelRecord(0))
		{
			Fyd12864DispPrintf(1,1,"    加气结束");
			Fyd12864ClearLine(2);
			Fyd12864DispPrintf(3,1,(char *)&stopreasonbuf[fuelrecordinfo.stopreason]);
			/*
			switch(fpvariedpar.stopreason)
			{
				case 0x00:
					Fyd12864DispPrintf(3,1,"手动停止");
					break;
				case 0x01:
					Fyd12864DispPrintf(3,1,"气相流量计异常");
					break;
				case 0x02:
					Fyd12864DispPrintf(3,1,"液相流量计异常");
					break;
				case 0x03:
					Fyd12864DispPrintf(3,1,"瞬时数据错误");
					break;
				case 0x04:
					Fyd12864DispPrintf(3,1,"压力传感器异常");
					break;
				case 0x05:
					Fyd12864DispPrintf(3,1,"  掉电或复位");
					break;
				case 0x06:
					Fyd12864DispPrintf(3,1,"    定气量");
					break;
				case 0x07:
					Fyd12864DispPrintf(3,1,"    定金额");
					break;
				case 0x08:
					Fyd12864DispPrintf(3,1,"    低流速");
					break;
				case 0x09:
					Fyd12864DispPrintf(3,1,"    高流速");
					break;
				case 0x0a:
					Fyd12864DispPrintf(3,1,"    回气密度");
					break;
				case 0x0b:
					Fyd12864DispPrintf(3,1,"    回气增益");
					break;
				case 0x0c:
					Fyd12864DispPrintf(3,1,"      超压");
					break;
				case 0x0d:
					Fyd12864DispPrintf(3,1,"      逃卡");
					break;
				case 0x0e:
					Fyd12864DispPrintf(3,1,"  记账卡超额");
					break;
				case 0x0f:
					Fyd12864DispPrintf(3,1,"单次超量或超额");
					break;
			}
			*/
			Fyd12864ClearLine(4);
		}
		else
		{
			Fyd12864ClearLine(1);
			Fyd12864ClearLine(2);
			Fyd12864ClearLine(3);
			Fyd12864DispPrintf(4,1,"存储流水失败");  
		}
		if(sysparas.printmode==1)
		{
			Printid();
		}
		memset(&realtime.lcng,0,sizeof(RealTimeData));  //ycx加气后清零实时数据。
        Fm25l04Write(0,(uint8_t *)(&realtime.lcng),sizeof(RealTimeData));
	}
	TaskAdd(KEY_RET, DispFirst, NULL);
	TaskMessage(30, DispFirst);// 2s后返回主界面
}
/***************************************************************************
**函数名称： FuelEnd
**函数功能：加气结束
**入口参数：无
**返回值：无 
**创建者：黄豪民
**创建日期：2016-11-10 16:17:52
**
**修改者：
**修改日期：
**修改内容：
***************************************************************************/
void FuelEnd(void)
{
	//uint32_t tmp;
	VCTRL_CLOSEKEY();
	VCTRL_HIGH_CLOSE() ;
	VCTRL_MID_CLOSE();
	VCTRL_LOW_CLOSE();
	if(fpvariedpar.stoperrorflag==0xaa)
	{
		timer1off(); //流量计错误就不再读取流量计
		fuelrecordinfo.money=fpvariedpar.moneybackup;
		fuelrecordinfo.volume=fpvariedpar.volumebackup;
		OsplcShow(fuelrecordinfo.volume,fuelrecordinfo.money,realtime.price,0,0);//蓝屏显示。
		Fyd12864DispPrintf(2,1,"气量:%ld.%02d",fuelrecordinfo.volume/100,fuelrecordinfo.volume%100);
		Fyd12864DispPrintf(3,1,"金额:%ld.%02d",fuelrecordinfo.money/100,fuelrecordinfo.money%100);
		Fyd12864DispPrintf(4,1,"V:%.02f P:%ld.%02d",realtime.cur_flowrate_g,realtime.cur_pressure/100,realtime.cur_pressure%100); 
		fpvariedpar.stoperrorflag=0;
		globalvar.fix=0xaa;
		timer1.tick=20;
	}
	else if(globalvar.fix==0x01)//定气量
	{
		fpvariedpar.cngvolume=(float)(globalvar.fixnum/100.00);
		fuelrecordinfo.volume=fpvariedpar.cngvolume*100+0.5;//当前交易气量。   
		fuelrecordinfo.money=(uint32_t)(fpvariedpar.cngvolume*realtime.price+0.5);//当前交易金额。
		OsplcShow(fuelrecordinfo.volume,fuelrecordinfo.money,realtime.price,0,0);//蓝屏显示。
		Fyd12864DispPrintf(2,1,"气量:%ld.%02d",fuelrecordinfo.volume/100,fuelrecordinfo.volume%100);
		Fyd12864DispPrintf(3,1,"金额:%ld.%02d",fuelrecordinfo.money/100,fuelrecordinfo.money%100);
		Fyd12864DispPrintf(4,1,"V:%.02f P:%ld.%02d",realtime.cur_flowrate_g,realtime.cur_pressure/100,realtime.cur_pressure%100); 
		globalvar.fix=0xaa;
		timer1.tick=20;
	}
	else if(globalvar.fix==0x02)//定金额
	{
		fpvariedpar.cngvolume=(double)globalvar.fixmoney/realtime.price;	
		fuelrecordinfo.volume=fpvariedpar.cngvolume*100+0.5;//当前交易气量
		fuelrecordinfo.money=globalvar.fixmoney;
		Fyd12864DispPrintf(2,1,"气量:%ld.%02d",fuelrecordinfo.volume/100,fuelrecordinfo.volume%100);	
		Fyd12864DispPrintf(3,1,"金额:%ld.%02d",fuelrecordinfo.money/100,fuelrecordinfo.money%100);
		Fyd12864DispPrintf(4,1,"V:%.02f P:%ld.%02d",realtime.cur_flowrate_g,realtime.cur_pressure/100,realtime.cur_pressure%100); 
		globalvar.fix=0xaa;
		timer1.tick=20;
	}
	else if((timer1.tick<20)&&(sysparas.simulation==0))
	{
		fpvariedpar.cngvolume=realtime.cur_totalize_g-fpvariedpar.startgasvolume;
		fpvariedpar.cngvolume = fpvariedpar.cngvolume * fpvariedpar.calfactor;		//乘上比例系数 	
        fpvariedpar.cngvolume=(uint32_t)(fpvariedpar.cngvolume*100);//当前交易气量
		fpvariedpar.cngvolume=(double)(fpvariedpar.cngvolume/100);//当前交易气量
		fuelrecordinfo.volume=fpvariedpar.cngvolume*100;//当前交易气量。   
		fuelrecordinfo.money=(uint32_t)(fpvariedpar.cngvolume*realtime.price+0.5);//当前交易金额。
		OsplcShow(fuelrecordinfo.volume,fuelrecordinfo.money,realtime.price,0,0);//蓝屏显示。
		Fyd12864DispPrintf(2,1,"气量:%ld.%02d",fuelrecordinfo.volume/100,fuelrecordinfo.volume%100);
		Fyd12864DispPrintf(3,1,"金额:%ld.%02d",fuelrecordinfo.money/100,fuelrecordinfo.money%100);
		Fyd12864DispPrintf(4,1,"V:%.02f P:%ld.%02d",realtime.cur_flowrate_g,realtime.cur_pressure/100,realtime.cur_pressure%100); 
	}
	else
	{
		timer1off();
		if(globalvar.displayonce==1)
		{
			SELECT_PRINT();
			realtime.cur_totalize_g=(uint32_t)(realtime.cur_totalize_g*100);
			realtime.cur_totalize_g=(float)(realtime.cur_totalize_g/100.00);
			WHPrintf("结束:%.02f\r\n",realtime.cur_totalize_g); 
			WHPrintf("起始:%.02f\r\n",fpvariedpar.startgasvolume); 
			WHPrintf("气量:%.02f\r\n",(float)fpvariedpar.cngvolume); 
//			WHPrinterFreeRow(3);
			SELECT_CARD();
		}
		fuelrecordinfo.stopreason=fpvariedpar.stopreason;
		globalvar.fuelstep=3;//加气步骤3
		if(globalvar.displayonce==1)
		{

			globalvar.displayonce=0;
			fpvariedpar.cngvolume=0;                                 //加液结束后气量变量清零。
			
			sysparas.transnum++;
			sysparas.totvolume += fuelrecordinfo.volume;//枪总气量。
			sysparas.totmoney += fuelrecordinfo.money;  //枪总金额。
			sysparas.shiftotvol+= fuelrecordinfo.volume;//班累计气量。
			sysparas.shiftotmon+= fuelrecordinfo.money; //班累计金额。
			sysparas.shiftfueltimes++;                  //班加气次数。
			fuelrecordinfo.transnum=sysparas.transnum;  //交易流水号。
			fuelrecordinfo.allconsumptionnum=m1card.sumvolme+fuelrecordinfo.volume;//累计消费气量。
			if(m1card.existflag==1)
			{
				fuelrecordinfo.transactiontype=0xFF;                        //交易类型。
				fuelrecordinfo.reallymoney=0x00;                      //本次实际扣款金额。
				fuelrecordinfo.allconsumptionmoney=m1card.summoney;     //累计消费金额。     
			}
			else
			{
				fuelrecordinfo.transactiontype=0;                           //交易类型。
				fuelrecordinfo.reallymoney=fuelrecordinfo.money;            //本次实际扣款金额。
				fuelrecordinfo.allconsumptionmoney=fuelrecordinfo.money;    //累计消费金额。
			}
			fuelrecordinfo.endtime[0]=0x20;
			memcpy(&fuelrecordinfo.endtime[1], &time.year, 6);              //加气结束时间。
			
			fuelrecordinfo.allgunnum=sysparas.totvolume;                    //枪气量累计。
			fuelrecordinfo.allgunmoney=sysparas.totmoney;                   //枪金额累计。
			fuelrecordinfo.afterpressure=realtime.cur_pressure;             //加气后压力。
			fuelrecordinfo.curnum=sysparas.coeff;                           //当量。
			fuelrecordinfo.measuretype=sysparas.unit;                       //计量单位，0-标方，1-Kg。
			fuelrecordinfo.density=realtime.cur_density_g;                  //密度。
			fuelrecordinfo.classid=sysparas.shiftransnum;                   //班流水号。
			
			fuelrecordinfo.crc=CRC16Calculate((uint8_t *)(&fuelrecordinfo.transnum),sizeof(FuelRecordInfo)-2);
			OsplcShow(fuelrecordinfo.volume,fuelrecordinfo.money,realtime.price,0,1);
			
			SaveSysparas(0,(uint8_t*)&sysparas.head,sizeof(Sysparas));
			m1card.debittimes=100;                                       //最多申请100次扣款。
		}
		if((m1card.existflag==1)&&(PowerCheckIn==0)) 
		{
			globalvar.workstate=StaPay;
			Fyd12864DispPrintf(1,1,"正在等待扣款.."); 
			if(m1card.debittimes==0)
			{
				globalvar.displayonce=1;
				DispUI=FuelEndDisplay;
			}
		}
		else if(PowerCheckIn==0)//未掉电
		{
			globalvar.displayonce=1;
			DispUI=FuelEndDisplay;
		}
		realtime.powerdown=0;          //未掉电标志。
		SaveFm25l04();                 //写铁电。
		if(PowerCheckIn!=0)            //判断是否掉电。 
		{ 
			MsDelay(20);
			if(PowerCheckIn!=0)
			{
				realtime.powerdown=1;  //已掉电。
				SaveFm25l04();         //写铁电。
				PowerDown();
			}
		}
	}
}

/***************************************************************************
**函数名称：FixGas
**函数功能：定量选择函数。
**入口参数：无
**返 回 值：无
**创 建 者：杨传坤
**创建日期：2016-7-25
**
**修改者：
**修改日期：
**修改内容：
***************************************************************************/
void FixGas(void)
{
	//if(globalvar.displayonce==0x01)
	{
		globalvar.displayonce=0;
		Fyd12864DispPrintf(1,1,"    定量加气");
		Fyd12864DispPrintf(2,1,"1.定气量");
		Fyd12864DispPrintf(3,1,"2.定金额");
		globalvar.fixnum=0;
		globalvar.fixmoney=0;
		InputInit();
	}
	Fyd12864ClearLine(4);
    TaskAdd(KEY_RET, DispFirst, NULL);
    TaskAdd(KEY_1, FixNum, NULL);
    TaskAdd(KEY_2, FixMoney, NULL);
}

void FixAffirm(void)
/***************************************************************************
**函数名称：FixAffirm
**函数功能：定量确认
**入口参数：无
**返 回 值：无
**创 建 者：黄豪民
**创建日期：2016-11-14
**
**修改者：
**修改日期：
**修改内容：
***************************************************************************/
{
	if(globalvar.fix==1)
	{
		Fyd12864DispPrintf(1,1,"定气量:%ld.%02d",globalvar.fixnum/100,globalvar.fixnum%100);
	}
	else if(globalvar.fix==2)
	{
		Fyd12864DispPrintf(1,1,"定金额:%ld.%02d",globalvar.fixmoney/100,globalvar.fixmoney%100);
	}
	Fyd12864ClearLine(2);
	Fyd12864DispPrintf(3,1,"加气    【加气】");
	Fyd12864DispPrintf(4,1,"取消    【返回】");
	if(m1card.existflag==1)
    {
        TaskAdd(KEY_START, ApplyVerif, NULL);
    }
    else
    {
        TaskAdd(KEY_START, FuelReady, NULL);
    }  
	TaskAdd(KEY_RET, DispFirst, NULL);
}
/***************************************************************************
**函数名称：FixNum
**函数功能：定气量函数。
**入口参数：无
**返 回 值：无
**创 建 者：杨传坤
**创建日期：2016-7-25
**
**修改者：
**修改日期：
**修改内容：
***************************************************************************/
void FixNum(void)
{
    char buf[16]={0};
    ScreenInput.inputmaxlen=5;
    NumKeyHandler();
    globalvar.fix=1;
    sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
    globalvar.fixnum=atol(buf);
	Fyd12864DispPrintf(1,1,"    定量加气");
    Fyd12864DispPrintf(2,1,"定气量:%ld.%02d",globalvar.fixnum/100,globalvar.fixnum%100);
	Fyd12864ClearLine(3);
    TaskAdd(KEY_RET, DispFirst, NULL); 
	TaskAdd(KEY_OK, FixAffirm, NULL);
}

/***************************************************************************
**函数名称：FixMoney
**函数功能：定金额函数。
**入口参数：无
**返 回 值：无
**创 建 者：杨传坤
**创建日期：2016-7-25
**
**修改者：
**修改日期：
**修改内容：
***************************************************************************/
void FixMoney(void)
{
    char buf[16]={0};
    ScreenInput.inputmaxlen=5;
    NumKeyHandler();
    globalvar.fix=2;
    sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
    globalvar.fixmoney=atol(buf);
	Fyd12864DispPrintf(1,1,"    定额加气");
    Fyd12864ClearLine(3);
    Fyd12864DispPrintf(2,1,"定金额:%ld.%02d",globalvar.fixmoney/100,globalvar.fixmoney%100);
    TaskAdd(KEY_RET, DispFirst, NULL);
	TaskAdd(KEY_OK, FixAffirm, NULL);
}

/***************************************************************************
**函数名称：SaveFm25l04
**函数功能：掉电保存数据函数。
**入口参数：无
**返 回 值：无
**创 建 者：杨传坤
**创建日期：2016-7-25
**
**修改者：
**修改日期：
**修改内容：
***************************************************************************/
void SaveFm25l04(void)
{
    realtime.lcng =sysparas.devicetype;                                                   //气品代码。
    if(globalvar.fuelstep==3)
    {
        realtime.transnum=sysparas.transnum;                            //当前流水号。
    }else
    {
        realtime.transnum=sysparas.transnum+1;                          //当前流水号。
    }
    realtime.cardtype=m1card.cardtype;                              //卡类型。  
    realtime.shiftnum=sysparas.shiftnum;                                //班号。
    memcpy(&realtime.employeenum[0],&shiftrecordinfo.cardstaffnum[0],8);//员工号。
    memcpy(&realtime.cardnum[0],&m1card.cardnum[0],8);              //用户卡号。
    memcpy(&realtime.carnum[0],&m1card.carnum[0],9);                //车牌号。
    memcpy(&realtime.starttime[0], &fuelrecordinfo.starttime[0], 7);    //加气开始时间。
	realtime.endtime[0]=0x20;
    memcpy(&realtime.endtime[1], &time.year, 6);                        //加气结束时间。
    realtime.beforepressure=fuelrecordinfo.beforepressure;              //加气前压力。
    realtime.afterpressure=realtime.cur_pressure;                       //加气后压力。
    realtime.beforebalance=fuelrecordinfo.beforebalance;                //加气前余额。
    realtime.afterbalance=fuelrecordinfo.afterbalance;                  //加气后余额。
    realtime.allrechargetimes=fuelrecordinfo.allrechargetimes;          //累计充值次数。
    realtime.allrechargemoney=fuelrecordinfo.allrechargemoney;          //累计充值金额。
    realtime.allconsumptiontimes=fuelrecordinfo.allconsumptiontimes;    //累计消费次数。
    realtime.volume=fuelrecordinfo.volume;                              //气量。
    realtime.money=fuelrecordinfo.money;                                //金额。
    realtime.allconsumptionnum=fuelrecordinfo.volume+m1card.sumvolme;//累计消费气量。
    realtime.allconsumptionmoney=m1card.summoney;//累计消费金额。

    if(m1card.existflag==1)
    {
        if((globalvar.fuelstep==1)||(globalvar.fuelstep==2))         //如果在加气步骤1或者2。
        {
            realtime.reallymoney=0;                           //实扣金额。
            realtime.transactiontype=0xFF;                             //交易类型。
        }
        else if(globalvar.fuelstep==3)                                //如果在加气步骤3.
        {
            realtime.reallymoney=fuelrecordinfo.reallymoney;           //实扣金额。
            realtime.transactiontype=fuelrecordinfo.transactiontype;   //交易类型。
        }
    }else
    {
        realtime.reallymoney=fuelrecordinfo.money;                      //实扣金额。
        realtime.transactiontype=0x0;                                   //交易类型。
    }
    realtime.curnum=sysparas.coeff;                                     //当量。
    realtime.measuretype=sysparas.unit;                                 //计量单位。
    realtime.stopreason=fpvariedpar.stopreason;                         //停机原因。
    realtime.jhid=0;                                                    //解灰相关流水号。
    realtime.classid=sysparas.shiftransnum;                             //班流水号。
    
    Fm25l04Write(0,(uint8_t *)(&realtime.lcng),sizeof(RealTimeData));   //存储铁电。

}

/***************************************************************************
**函数名称：DisPlayLock
**函数功能：加密板显示函数。
**入口参数：无
**返 回 值：无
**创 建 者：杨传坤
**创建日期：2016-7-25
**
**修改者：
**修改日期：
**修改内容：
***************************************************************************/
void DisPlayLock(void)
{
    Fyd12864ClearLine(1);
    Fyd12864ClearLine(3);
    Fyd12864ClearLine(4);
    if(SI_INDATE==0)//设置状态。
    {
        Fyd12864DispPrintf(2,1,"非正常状态");
    }else         
    {
        DispUI=DispFirst;
    }
}


void CirculateFueling(void)
{
	globalvar.fixnum=100+time.day+time.hour+time.minute+time.second;
	globalvar.fix=1;//定量
	DispUI=ApplyVerif;
}
