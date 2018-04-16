#include "kernel.h"

void DispSetControl1(void)
{
	InputInit();
	Fyd12864DispPrintf(1,1,"1.低转中切阀流速");
	Fyd12864DispPrintf(2,1,"2.中转高切阀流速");
	Fyd12864DispPrintf(3,1,"3.最低停机流速");
	Fyd12864DispPrintf(4,1,"4.最高安全流速");
	TaskAdd(KEY_RET,DispMenu_2,NULL);
	TaskAdd(KEY_1,ValveLowToMidFLow,NULL);
	TaskAdd(KEY_2,ValveMidToHighFLow,NULL);
	TaskAdd(KEY_3,ValveStopFLow,NULL);
	TaskAdd(KEY_4,ValveSafeFlow,NULL);
	TaskAdd(KEY_DOWN,DispControlSub1,NULL);
}
void ValveLowToMidFLow(void)
{
	char buf[16]={0};
	tmp_sysparas=sysparas;
	ScreenInput.inputmaxlen=4;
	sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
	tmp_sysparas.lowtomidflow=atol(buf);
	globalvar.paraindex=201;
	DispSet(2,sysparas.lowtomidflow,"低转中切阀流速");   
	TaskAdd(KEY_RET, DispSetControl1,NULL);
	TaskAdd(KEY_OK, DispOK,DispSetControl1);
}
void ValveMidToHighFLow(void)
{
	char buf[16]={0};
	tmp_sysparas=sysparas;
	ScreenInput.inputmaxlen=4;
	sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
	tmp_sysparas.midtohighflow=atol(buf);
	globalvar.paraindex=202;
	DispSet(2,sysparas.midtohighflow,"中转高切阀流速");   
	TaskAdd(KEY_RET, DispSetControl1,NULL);
	TaskAdd(KEY_OK, DispOK,DispSetControl1);
}
void ValveStopFLow(void)
{
	char buf[16]={0};
	tmp_sysparas=sysparas;
	ScreenInput.inputmaxlen=4;
	sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
	tmp_sysparas.lowstopflow=atol(buf);
	globalvar.paraindex=203;
	DispSet(2,sysparas.lowstopflow,"    停机流速");   
	TaskAdd(KEY_RET, DispSetControl1,NULL);
	TaskAdd(KEY_OK, DispOK,DispSetControl1);
}

void ValveSafeFlow(void)
{
	char buf[16]={0};
	tmp_sysparas=sysparas; 
	ScreenInput.inputmaxlen=4;
	sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
	tmp_sysparas.maxsafeflow=atol(buf);
	globalvar.paraindex=204;
	DispSet(2,sysparas.maxsafeflow,"    安全流速");   
	TaskAdd(KEY_RET, DispSetControl1,NULL);
	TaskAdd(KEY_OK, DispOK,DispSetControl1);
}

//////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void DispControlSub1(void)
{
	InputInit();
	tmp_sysparas=sysparas;
	Fyd12864DispPrintf(1,1,"1.设备类型");
	Fyd12864DispPrintf(2,1,"2.单次最大加气量");
	Fyd12864DispPrintf(3,1,"3.单次最大加气额");
	Fyd12864DispPrintf(4,1,"4.停机压力");
	TaskAdd(KEY_RET,DispMenu_2,NULL);
	TaskAdd(KEY_1,DeviceType,NULL);
	TaskAdd(KEY_2,MaxOneVolume,NULL);
	TaskAdd(KEY_3,MaxOnePrice,NULL);
	TaskAdd(KEY_4,StopFuelPressure,NULL);
	TaskAdd(KEY_UP,DispSetControl1,NULL);
	TaskAdd(KEY_DOWN,DispControlSub2,NULL);
}

void DeviceType(void)
{
	
	if((tmp_sysparas.devicetype<1)||(tmp_sysparas.devicetype>3))	//无符号数与小于0比较会报警
	{
		tmp_sysparas.devicetype = 1;
		globalvar.KeyValue = KEY_OK;	//如果加气机类型异常就强制设置为加气机
	}
	
	if(tmp_sysparas.devicetype == 1)
	{
		Fyd12864DispPrintf(1,1,"1.加气机    √");
		Fyd12864DispPrintf(2,1,"2.加气柱");
		Fyd12864DispPrintf(3,1,"3.泄气柱");
		tmp_sysparas.maxsafeflow = 2500;		//加气机高流速停机
	}
	if(tmp_sysparas.devicetype == 2)
	{
		Fyd12864DispPrintf(1,1,"1.加气机");
		Fyd12864DispPrintf(2,1,"2.加气柱    √");
		Fyd12864DispPrintf(3,1,"3.泄气柱");
		tmp_sysparas.maxsafeflow = 12000;		//泄气柱、加气柱高流速停机
	}
	if(tmp_sysparas.devicetype == 3)
	{
		Fyd12864DispPrintf(1,1,"1.加气机");
		Fyd12864DispPrintf(2,1,"2.加气柱");
		Fyd12864DispPrintf(3,1,"3.泄气柱    √");
		tmp_sysparas.maxsafeflow = 12000;		//泄气柱、加气柱高流速停机
	}
	globalvar.paraindex=88;
	Fyd12864DispPrintf(4,1,"按【确认】保存");
	TaskAdd(KEY_UP, SetDeviceUp,NULL);
	TaskAdd(KEY_DOWN, SetDeviceDown,NULL);
	TaskAdd(KEY_RET, DispControlSub1,NULL);
	if(globalvar.KeyValue==KEY_OK)
	{
		if(ScreenInput.inputlen>0)
		{
			DispOK();
			MsDelay(2000); 
			DispNextUI=NULL;
			SystemReStart();
		}
		else
		{
			TaskAdd(KEY_OK,DispOK,DispControlSub1);
		}
	}
}
void SetDeviceUp(void)
{
	if(tmp_sysparas.devicetype > 1)
	{
		ScreenInput.inputlen = 1;
		tmp_sysparas.devicetype--;
	}
	DispUI = DeviceType;
}
void SetDeviceDown(void)
{
	if(tmp_sysparas.devicetype < 3)
	{
		ScreenInput.inputlen = 1;
		tmp_sysparas.devicetype++;
	}
	DispUI = DeviceType;
}

void MaxOneVolume(void)
{
	char buf[16]={0};
	tmp_sysparas=sysparas;
	ScreenInput.inputmaxlen=9;
	sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
	tmp_sysparas.maxvolumeonce=atol(buf);
	globalvar.paraindex=86;
	DispSet(2,sysparas.maxvolumeonce,"单次最大加气气量");   
	TaskAdd(KEY_RET, DispControlSub1,NULL);
	TaskAdd(KEY_OK, DispOK,DispControlSub1);
}


void MaxOnePrice(void)
{
	char buf[16]={0};
	tmp_sysparas=sysparas;
	ScreenInput.inputmaxlen=5;
	sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
	tmp_sysparas.maxmoneyonce=atol(buf);
	globalvar.paraindex=87;
	DispSet(2,sysparas.maxmoneyonce,"单次最大加气金额");   
	TaskAdd(KEY_RET, DispControlSub1,NULL);
	TaskAdd(KEY_OK, DispOK,DispControlSub1);
}


void StopFuelPressure(void)
{
	char buf[16]={0};
	tmp_sysparas=sysparas;
	ScreenInput.inputmaxlen=4;
	sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
	tmp_sysparas.stoppressure=atol(buf);
	globalvar.paraindex=205;
	DispSet(2,sysparas.stoppressure,"    停机压力");   
	TaskAdd(KEY_RET, DispControlSub1,NULL);
	TaskAdd(KEY_OK, DispOK,DispControlSub1);
}
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
void DispControlSub2(void)
{
	InputInit();
	Fyd12864DispPrintf(1,1,"1.防过冲时间");
	Fyd12864DispPrintf(2,1,"2.阀切换延时时间");
	Fyd12864DispPrintf(3,1,"3.20.5区间流速");
	Fyd12864DispPrintf(4,1,"4.21.0区间流速");
	TaskAdd(KEY_RET,DispMenu_2,NULL);
	TaskAdd(KEY_1,ValveFilterTime,NULL);
	TaskAdd(KEY_2,ValveSwitchDelayTime,NULL);
	TaskAdd(KEY_3,TwentyPointFiveFlow,NULL);
	TaskAdd(KEY_4,TwentyOneFlow,NULL);
	TaskAdd(KEY_UP,DispControlSub1,NULL);
	TaskAdd(KEY_DOWN,DispControlSub3,NULL);
}

void ValveFilterTime(void)
{
	//char buf[16]={0};
	tmp_sysparas=sysparas;
	ScreenInput.inputmaxlen=5;
	//sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
	tmp_sysparas.filtertime=atol((char *)ScreenInput.array);
	globalvar.paraindex=207;
	DispSet(7,sysparas.filtertime,"  防过冲时间");   
	TaskAdd(KEY_RET, DispControlSub2,NULL);
	TaskAdd(KEY_OK, DispOK,DispControlSub2);
}

void ValveSwitchDelayTime(void)
{
	//char buf[16]={0};
	tmp_sysparas=sysparas;
	ScreenInput.inputmaxlen=5;
	//sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
	tmp_sysparas.valveswitchdelaytime=atol((char *)ScreenInput.array);
	globalvar.paraindex=206;
	DispSet(7,sysparas.valveswitchdelaytime,"  阀切换时间");   
	TaskAdd(KEY_RET, DispControlSub2,NULL);
	TaskAdd(KEY_OK, DispOK,DispControlSub2);
}

void TwentyPointFiveFlow(void)
{
	char buf[16]={0};
	tmp_sysparas=sysparas;
	ScreenInput.inputmaxlen=5;
	sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
	tmp_sysparas.flow20_5=atol(buf);
	globalvar.paraindex=208;
	DispSet(2,sysparas.flow20_5,"  20.5区间流速");   
	TaskAdd(KEY_RET, DispControlSub2,NULL);
	TaskAdd(KEY_OK, DispOK,DispControlSub2);
}
void TwentyOneFlow(void)
{
	char buf[16]={0};
	tmp_sysparas=sysparas;
	ScreenInput.inputmaxlen=5;
	sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
	tmp_sysparas.flow21=atol(buf);
	globalvar.paraindex=209;
	DispSet(2,sysparas.flow21,"  21.0区间流速");   
	TaskAdd(KEY_RET, DispControlSub2,NULL);
	TaskAdd(KEY_OK, DispOK,DispControlSub2);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void DispControlSub3(void)
{
	InputInit();
	Fyd12864DispPrintf(1,1,"1.21.5区间流速");
	Fyd12864DispPrintf(2,1,"2.22.0区间流速");
	Fyd12864DispPrintf(3,1,"3.22.5区间流速");
	Fyd12864DispPrintf(4,1,"4.23.0区间流速");
	TaskAdd(KEY_RET,DispMenu_2,NULL);
	TaskAdd(KEY_1,TwentyonePointFiveFlow,NULL);
	TaskAdd(KEY_2,TwentyTwoFlow,NULL);
	TaskAdd(KEY_3,TwentyTwoPointFive,NULL);
	TaskAdd(KEY_4,TwentyThreeFlow,NULL);
	TaskAdd(KEY_UP,DispControlSub2,NULL);
	TaskAdd(KEY_DOWN,DispControlSub4,NULL);
}

void TwentyonePointFiveFlow(void)
{
	char buf[16]={0};
	tmp_sysparas=sysparas;
	ScreenInput.inputmaxlen=5;
	sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
	tmp_sysparas.flow21_5=atol(buf);
	DispSet(2,sysparas.flow21_5,"  21.5区间流速");
    globalvar.paraindex=210;
	TaskAdd(KEY_RET, DispControlSub3,NULL);
	TaskAdd(KEY_OK, DispOK,DispControlSub3);
}

void TwentyTwoFlow(void)
{
	char buf[16]={0};
	tmp_sysparas=sysparas;
	ScreenInput.inputmaxlen=5;
	sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
	tmp_sysparas.flow22=atol(buf);
	DispSet(2,sysparas.flow22,"  22.0区间流速"); 
    globalvar.paraindex=211;	
	TaskAdd(KEY_RET, DispControlSub3,NULL);
	TaskAdd(KEY_OK, DispOK,DispControlSub3);
}

void TwentyTwoPointFive(void)
{
	char buf[16]={0};
	tmp_sysparas=sysparas;
	ScreenInput.inputmaxlen=5;
	sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
	tmp_sysparas.flow22_5=atol(buf);
	DispSet(2,sysparas.flow22_5,"  22.5区间流速"); 
    globalvar.paraindex=212;	
	TaskAdd(KEY_RET, DispControlSub3,NULL);
	TaskAdd(KEY_OK, DispOK,DispControlSub3);
}

void TwentyThreeFlow(void)
{
	char buf[16]={0};
	tmp_sysparas=sysparas;
	ScreenInput.inputmaxlen=5;
	sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
	tmp_sysparas.flow23=atol(buf);
	DispSet(2,sysparas.flow23,"  23.0区间流速"); 
    globalvar.paraindex=213;	
	TaskAdd(KEY_RET, DispControlSub3,NULL);
	TaskAdd(KEY_OK, DispOK,DispControlSub3);
	
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

void DispControlSub4(void)
{
	InputInit();
	Fyd12864DispPrintf(1,1,"1.23.5区间流速");
	Fyd12864DispPrintf(2,1,"2.24.0区间流速");
	Fyd12864DispPrintf(3,1,"3.24.5区间流速");
	Fyd12864DispPrintf(4,1,"4.泄气柱启动流速");
	TaskAdd(KEY_1,TwentyThreePointFiveFlow,NULL);
	TaskAdd(KEY_2,TwentyFourFlow,NULL);
	TaskAdd(KEY_3,TwentyFourPointFive,NULL);
	TaskAdd(KEY_4,XQZStartFlowRate,NULL);
	TaskAdd(KEY_RET,DispMenu_2,NULL);
	TaskAdd(KEY_UP,DispControlSub3,NULL);
}

void TwentyThreePointFiveFlow(void)
{
	char buf[16]={0};
	tmp_sysparas=sysparas;
	ScreenInput.inputmaxlen=5;
	sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
	tmp_sysparas.flow23_5=atol(buf);
	DispSet(2,sysparas.flow23_5,"  23.5区间流速"); 
    globalvar.paraindex=214;
	TaskAdd(KEY_RET, DispControlSub4,NULL);
	TaskAdd(KEY_OK, DispOK,DispControlSub4);
}
void TwentyFourFlow(void)
{
	char buf[16]={0};
	tmp_sysparas=sysparas;
	ScreenInput.inputmaxlen=5;
	sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
	tmp_sysparas.flow24=atol(buf);
	DispSet(2,sysparas.flow24,"  24.0区间流速"); 
    globalvar.paraindex=215;	
	TaskAdd(KEY_RET, DispControlSub4,NULL);
	TaskAdd(KEY_OK, DispOK,DispControlSub4);
}
void TwentyFourPointFive(void)
{
	char buf[16]={0};
	tmp_sysparas=sysparas;
	ScreenInput.inputmaxlen=5;
	sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
	tmp_sysparas.flow24_5=atol(buf);
	DispSet(2,sysparas.flow24_5,"  24.5区间流速"); 
    globalvar.paraindex=216;	
	TaskAdd(KEY_RET, DispControlSub4,NULL);
	TaskAdd(KEY_OK, DispOK,DispControlSub4);
}

void XQZStartFlowRate(void)
//泄气柱启动流速
{
	char buf[16]={0};
	tmp_sysparas=sysparas;
	ScreenInput.inputmaxlen=5;
	sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
	tmp_sysparas.xqzstartflowrate=atol(buf);
	DispSet(2,sysparas.xqzstartflowrate,"  泄气柱启动流速"); 
    globalvar.paraindex=217;	
	TaskAdd(KEY_RET, DispControlSub4,NULL);
	TaskAdd(KEY_OK, DispOK,DispControlSub4);
}

void BackShowPrint(void)
{
    Printid();
    DispUI=BackShow;
}

void BackShow(void)
{
	if(sysparas.transnum > 0)
	{
		if(globalvar.displayonce==1)
		{
			globalvar.displayonce=0;       
			ReadFuelRecord(sysparas.transnum-1,0);
    
			Fyd12864DispPrintf(1,1,"流水号:%ld",sysparas.transnum);
			Fyd12864DispPrintf(2,1,"单价:%d.%02d",fuelrecordinfo.price/100,fuelrecordinfo.price%100);
			Fyd12864DispPrintf(3,1,"气量:%ld.%02d",fuelrecordinfo.volume/100,fuelrecordinfo.volume%100);
			Fyd12864DispPrintf(4,1,"金额:%ld.%02d",fuelrecordinfo.money/100,fuelrecordinfo.money%100);
		}	
		TaskAdd(KEY_RET, DispFirst,NULL);
		TaskAdd(KEY_OK, BackShowPrint,NULL);
	}
	else
	{
		Fyd12864ClearLine(1);
		Fyd12864DispPrintf(2,1,"  无流水记录");
		Fyd12864ClearLine(3);
		Fyd12864ClearLine(4);
		TaskMessage(20, DispFirst);
	}
		
}
void PrinterCtrlParas(void)
{
    PrintCtrlParas();
    DispUI=DispCtrlParas1;
}
void PrintCtrlParas(void)
{
	WHPrinterInit();//初始化。
	if(WHPrintReady())
    {
        WHFuelCtrlParas();
    }
}
void WHFuelCtrlParas(void)
{
	SELECT_PRINT();

    WHPrinterHead();
    WHPrintf("         CNG控制参数\r\n");
    WHPrinterFreeRow(1);
    WHPrinterNormalCmd();
    if(sysparas.devicetype == 1)
		WHPrintf("设备类型：加气机\r\n");
	else if(sysparas.devicetype == 2)
		WHPrintf("设备类型：加气柱\r\n");
	else if(sysparas.devicetype == 3)
		WHPrintf("设备类型：泄气柱\r\n");
	else
		WHPrintf("设备类型：未设置\r\n");
    WHPrintf("停机流速:%d.%02d\r\n",sysparas.lowstopflow/100,sysparas.lowstopflow%100);
    WHPrintf("安全流速:%d.%02d\r\n",sysparas.maxsafeflow/100,sysparas.maxsafeflow%100);
    WHPrintf("最大气量:%d.%02d\r\n",sysparas.maxvolumeonce/100,sysparas.maxvolumeonce%100);
    WHPrintf("最大金额:%d.%02d\r\n",sysparas.maxmoneyonce/100,sysparas.maxmoneyonce%100);
    WHPrintf("停机压力:%d.%02d\r\n",sysparas.stoppressure/100,sysparas.stoppressure%100);
	WHPrintf("防过冲时间:%d\r\n",sysparas.filtertime);
	WHPrintf("低->中切阀:%d.%02d\r\n",sysparas.lowtomidflow/100,sysparas.lowtomidflow%100);
    WHPrintf("中->高切阀:%d.%02d\r\n",sysparas.midtohighflow/100,sysparas.midtohighflow%100);
	WHPrintf("20.5区间:%d.%02d\r\n",sysparas.flow20_5/100,sysparas.flow20_5%100);
	WHPrintf("21.0区间:%d.%02d\r\n",sysparas.flow21/100,sysparas.flow21%100);
	WHPrintf("21.5区间:%d.%02d\r\n",sysparas.flow21_5/100,sysparas.flow21_5%100);
	WHPrintf("22.0区间:%d.%02d\r\n",sysparas.flow22/100,sysparas.flow22%100);
	WHPrintf("22.5区间:%d.%02d\r\n",sysparas.flow22_5/100,sysparas.flow22_5%100);
	WHPrintf("23.0区间:%d.%02d\r\n",sysparas.flow23/100,sysparas.flow23%100);
	WHPrintf("23.5区间:%d.%02d\r\n",sysparas.flow23_5/100,sysparas.flow23_5%100);
	WHPrintf("24.0区间:%d.%02d\r\n",sysparas.flow24/100,sysparas.flow24%100);
	WHPrintf("24.5区间:%d.%02d\r\n",sysparas.flow24_5/100,sysparas.flow24_5%100);
	WHPrintf("泄气柱启动流速:%d.%02d\r\n",sysparas.xqzstartflowrate/100,sysparas.xqzstartflowrate%100);
 
    WHPrinterFreeRow(1);
    WHPrintf("     <请妥善保存此凭条>\r\n");//脚注 
    WHPrinterFreeRow(6); 
    SELECT_CARD();
}
void DispCtrlParas1(void)
{
	Fyd12864DispPrintf(1,1,"低->中切阀:%d.%02d",sysparas.lowtomidflow/100,sysparas.lowtomidflow%100);
	Fyd12864DispPrintf(2,1,"中->高切阀:%d.%02d",sysparas.midtohighflow/100,sysparas.midtohighflow%100);
	Fyd12864DispPrintf(3,1,"停机流速:%d.%02d",sysparas.lowstopflow/100,sysparas.lowstopflow%100);
	Fyd12864DispPrintf(4,1,"安全流速:%d.%02d",sysparas.maxsafeflow/100,sysparas.maxsafeflow%100);
	TaskAdd(KEY_DOWN,DispCtrlParas2, NULL);
    TaskAdd(KEY_RET, DispQuery1,NULL);
	TaskAdd(KEY_OK, PrinterCtrlParas,NULL);
}

void DispCtrlParas2(void)
{
	uint8_t *str;
	if(sysparas.devicetype == 1)
		str = "加气机";
	else if(sysparas.devicetype == 2)
		str = "加气柱";
	else if(sysparas.devicetype == 3)
		str = "泄气柱";
	else
		str = "未设置";
	
	Fyd12864DispPrintf(1,1,"设备类型：%s",str);
	Fyd12864DispPrintf(2,1,"最大气量:%d.%02d",sysparas.maxvolumeonce/100,sysparas.maxvolumeonce%100);
	Fyd12864DispPrintf(3,1,"最大金额:%d.%02d",sysparas.maxmoneyonce/100,sysparas.maxmoneyonce%100);
	Fyd12864DispPrintf(4,1,"停机压力:%d.%02d",sysparas.stoppressure/100,sysparas.stoppressure%100);
	TaskAdd(KEY_UP,DispCtrlParas1,NULL);
	TaskAdd(KEY_DOWN,DispCtrlParas3, NULL);	
    TaskAdd(KEY_RET, DispQuery1,NULL);
	TaskAdd(KEY_OK, PrinterCtrlParas,NULL);
}
void DispCtrlParas3(void)
{
	Fyd12864DispPrintf(1,1,"防过冲时间:%d",sysparas.filtertime);
	Fyd12864DispPrintf(2,1,"阀切换时间:%d",sysparas.valveswitchdelaytime);
	Fyd12864DispPrintf(3,1,"20.5区间:%d.%02d",sysparas.flow20_5/100,sysparas.flow20_5%100);
	Fyd12864DispPrintf(4,1,"21.0区间:%d.%02d",sysparas.flow21/100,sysparas.flow21%100);
	TaskAdd(KEY_UP,DispCtrlParas2,NULL);
	TaskAdd(KEY_DOWN,DispCtrlParas4, NULL);	
    TaskAdd(KEY_RET, DispQuery1,NULL);
	TaskAdd(KEY_OK, PrinterCtrlParas,NULL);
}
void DispCtrlParas4(void)
{
	Fyd12864DispPrintf(1,1,"21.5区间:%d.%02d",sysparas.flow21_5/100,sysparas.flow21_5%100);
	Fyd12864DispPrintf(2,1,"22.0区间:%d.%02d",sysparas.flow22/100,sysparas.flow22%100);
	Fyd12864DispPrintf(3,1,"22.5区间:%d.%02d",sysparas.flow22_5/100,sysparas.flow22_5%100);
	Fyd12864DispPrintf(4,1,"23.0区间:%d.%02d",sysparas.flow23/100,sysparas.flow23%100);
	TaskAdd(KEY_UP,DispCtrlParas3,NULL);
	TaskAdd(KEY_DOWN,DispCtrlParas5, NULL);	
    TaskAdd(KEY_RET, DispQuery1,NULL);
	TaskAdd(KEY_OK, PrinterCtrlParas,NULL);
}
void DispCtrlParas5(void)
{
	Fyd12864DispPrintf(1,1,"23.5区间:%d.%02d",sysparas.flow23_5/100,sysparas.flow23_5%100);
	Fyd12864DispPrintf(2,1,"24.0区间:%d.%02d",sysparas.flow24/100,sysparas.flow24%100);
	Fyd12864DispPrintf(3,1,"24.5区间:%d.%02d",sysparas.flow24_5/100,sysparas.flow24_5%100);
	Fyd12864DispPrintf(4,1,"启动流速:%d.%02d",sysparas.xqzstartflowrate/100,sysparas.xqzstartflowrate%100);
	TaskAdd(KEY_UP,DispCtrlParas4,NULL);
    TaskAdd(KEY_RET, DispQuery1,NULL);
	TaskAdd(KEY_OK, PrinterCtrlParas,NULL);
}


