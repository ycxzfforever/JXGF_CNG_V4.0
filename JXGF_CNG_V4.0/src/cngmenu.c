#include "kernel.h"

void DispSetControl1(void)
{
	InputInit();
	Fyd12864DispPrintf(1,1,"1.��ת���з�����");
	Fyd12864DispPrintf(2,1,"2.��ת���з�����");
	Fyd12864DispPrintf(3,1,"3.���ͣ������");
	Fyd12864DispPrintf(4,1,"4.��߰�ȫ����");
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
	DispSet(2,sysparas.lowtomidflow,"��ת���з�����");   
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
	DispSet(2,sysparas.midtohighflow,"��ת���з�����");   
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
	DispSet(2,sysparas.lowstopflow,"    ͣ������");   
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
	DispSet(2,sysparas.maxsafeflow,"    ��ȫ����");   
	TaskAdd(KEY_RET, DispSetControl1,NULL);
	TaskAdd(KEY_OK, DispOK,DispSetControl1);
}

//////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void DispControlSub1(void)
{
	InputInit();
	tmp_sysparas=sysparas;
	Fyd12864DispPrintf(1,1,"1.�豸����");
	Fyd12864DispPrintf(2,1,"2.������������");
	Fyd12864DispPrintf(3,1,"3.������������");
	Fyd12864DispPrintf(4,1,"4.ͣ��ѹ��");
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
	
	if((tmp_sysparas.devicetype<1)||(tmp_sysparas.devicetype>3))	//�޷�������С��0�Ƚϻᱨ��
	{
		tmp_sysparas.devicetype = 1;
		globalvar.KeyValue = KEY_OK;	//��������������쳣��ǿ������Ϊ������
	}
	
	if(tmp_sysparas.devicetype == 1)
	{
		Fyd12864DispPrintf(1,1,"1.������    ��");
		Fyd12864DispPrintf(2,1,"2.������");
		Fyd12864DispPrintf(3,1,"3.й����");
		tmp_sysparas.maxsafeflow = 2500;		//������������ͣ��
	}
	if(tmp_sysparas.devicetype == 2)
	{
		Fyd12864DispPrintf(1,1,"1.������");
		Fyd12864DispPrintf(2,1,"2.������    ��");
		Fyd12864DispPrintf(3,1,"3.й����");
		tmp_sysparas.maxsafeflow = 12000;		//й������������������ͣ��
	}
	if(tmp_sysparas.devicetype == 3)
	{
		Fyd12864DispPrintf(1,1,"1.������");
		Fyd12864DispPrintf(2,1,"2.������");
		Fyd12864DispPrintf(3,1,"3.й����    ��");
		tmp_sysparas.maxsafeflow = 12000;		//й������������������ͣ��
	}
	globalvar.paraindex=88;
	Fyd12864DispPrintf(4,1,"����ȷ�ϡ�����");
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
	DispSet(2,sysparas.maxvolumeonce,"��������������");   
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
	DispSet(2,sysparas.maxmoneyonce,"�������������");   
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
	DispSet(2,sysparas.stoppressure,"    ͣ��ѹ��");   
	TaskAdd(KEY_RET, DispControlSub1,NULL);
	TaskAdd(KEY_OK, DispOK,DispControlSub1);
}
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
void DispControlSub2(void)
{
	InputInit();
	Fyd12864DispPrintf(1,1,"1.������ʱ��");
	Fyd12864DispPrintf(2,1,"2.���л���ʱʱ��");
	Fyd12864DispPrintf(3,1,"3.20.5��������");
	Fyd12864DispPrintf(4,1,"4.21.0��������");
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
	DispSet(7,sysparas.filtertime,"  ������ʱ��");   
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
	DispSet(7,sysparas.valveswitchdelaytime,"  ���л�ʱ��");   
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
	DispSet(2,sysparas.flow20_5,"  20.5��������");   
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
	DispSet(2,sysparas.flow21,"  21.0��������");   
	TaskAdd(KEY_RET, DispControlSub2,NULL);
	TaskAdd(KEY_OK, DispOK,DispControlSub2);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void DispControlSub3(void)
{
	InputInit();
	Fyd12864DispPrintf(1,1,"1.21.5��������");
	Fyd12864DispPrintf(2,1,"2.22.0��������");
	Fyd12864DispPrintf(3,1,"3.22.5��������");
	Fyd12864DispPrintf(4,1,"4.23.0��������");
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
	DispSet(2,sysparas.flow21_5,"  21.5��������");
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
	DispSet(2,sysparas.flow22,"  22.0��������"); 
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
	DispSet(2,sysparas.flow22_5,"  22.5��������"); 
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
	DispSet(2,sysparas.flow23,"  23.0��������"); 
    globalvar.paraindex=213;	
	TaskAdd(KEY_RET, DispControlSub3,NULL);
	TaskAdd(KEY_OK, DispOK,DispControlSub3);
	
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

void DispControlSub4(void)
{
	InputInit();
	Fyd12864DispPrintf(1,1,"1.23.5��������");
	Fyd12864DispPrintf(2,1,"2.24.0��������");
	Fyd12864DispPrintf(3,1,"3.24.5��������");
	Fyd12864DispPrintf(4,1,"4.й������������");
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
	DispSet(2,sysparas.flow23_5,"  23.5��������"); 
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
	DispSet(2,sysparas.flow24,"  24.0��������"); 
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
	DispSet(2,sysparas.flow24_5,"  24.5��������"); 
    globalvar.paraindex=216;	
	TaskAdd(KEY_RET, DispControlSub4,NULL);
	TaskAdd(KEY_OK, DispOK,DispControlSub4);
}

void XQZStartFlowRate(void)
//й������������
{
	char buf[16]={0};
	tmp_sysparas=sysparas;
	ScreenInput.inputmaxlen=5;
	sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
	tmp_sysparas.xqzstartflowrate=atol(buf);
	DispSet(2,sysparas.xqzstartflowrate,"  й������������"); 
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
    
			Fyd12864DispPrintf(1,1,"��ˮ��:%ld",sysparas.transnum);
			Fyd12864DispPrintf(2,1,"����:%d.%02d",fuelrecordinfo.price/100,fuelrecordinfo.price%100);
			Fyd12864DispPrintf(3,1,"����:%ld.%02d",fuelrecordinfo.volume/100,fuelrecordinfo.volume%100);
			Fyd12864DispPrintf(4,1,"���:%ld.%02d",fuelrecordinfo.money/100,fuelrecordinfo.money%100);
		}	
		TaskAdd(KEY_RET, DispFirst,NULL);
		TaskAdd(KEY_OK, BackShowPrint,NULL);
	}
	else
	{
		Fyd12864ClearLine(1);
		Fyd12864DispPrintf(2,1,"  ����ˮ��¼");
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
	WHPrinterInit();//��ʼ����
	if(WHPrintReady())
    {
        WHFuelCtrlParas();
    }
}
void WHFuelCtrlParas(void)
{
	SELECT_PRINT();

    WHPrinterHead();
    WHPrintf("         CNG���Ʋ���\r\n");
    WHPrinterFreeRow(1);
    WHPrinterNormalCmd();
    if(sysparas.devicetype == 1)
		WHPrintf("�豸���ͣ�������\r\n");
	else if(sysparas.devicetype == 2)
		WHPrintf("�豸���ͣ�������\r\n");
	else if(sysparas.devicetype == 3)
		WHPrintf("�豸���ͣ�й����\r\n");
	else
		WHPrintf("�豸���ͣ�δ����\r\n");
    WHPrintf("ͣ������:%d.%02d\r\n",sysparas.lowstopflow/100,sysparas.lowstopflow%100);
    WHPrintf("��ȫ����:%d.%02d\r\n",sysparas.maxsafeflow/100,sysparas.maxsafeflow%100);
    WHPrintf("�������:%d.%02d\r\n",sysparas.maxvolumeonce/100,sysparas.maxvolumeonce%100);
    WHPrintf("�����:%d.%02d\r\n",sysparas.maxmoneyonce/100,sysparas.maxmoneyonce%100);
    WHPrintf("ͣ��ѹ��:%d.%02d\r\n",sysparas.stoppressure/100,sysparas.stoppressure%100);
	WHPrintf("������ʱ��:%d\r\n",sysparas.filtertime);
	WHPrintf("��->���з�:%d.%02d\r\n",sysparas.lowtomidflow/100,sysparas.lowtomidflow%100);
    WHPrintf("��->���з�:%d.%02d\r\n",sysparas.midtohighflow/100,sysparas.midtohighflow%100);
	WHPrintf("20.5����:%d.%02d\r\n",sysparas.flow20_5/100,sysparas.flow20_5%100);
	WHPrintf("21.0����:%d.%02d\r\n",sysparas.flow21/100,sysparas.flow21%100);
	WHPrintf("21.5����:%d.%02d\r\n",sysparas.flow21_5/100,sysparas.flow21_5%100);
	WHPrintf("22.0����:%d.%02d\r\n",sysparas.flow22/100,sysparas.flow22%100);
	WHPrintf("22.5����:%d.%02d\r\n",sysparas.flow22_5/100,sysparas.flow22_5%100);
	WHPrintf("23.0����:%d.%02d\r\n",sysparas.flow23/100,sysparas.flow23%100);
	WHPrintf("23.5����:%d.%02d\r\n",sysparas.flow23_5/100,sysparas.flow23_5%100);
	WHPrintf("24.0����:%d.%02d\r\n",sysparas.flow24/100,sysparas.flow24%100);
	WHPrintf("24.5����:%d.%02d\r\n",sysparas.flow24_5/100,sysparas.flow24_5%100);
	WHPrintf("й������������:%d.%02d\r\n",sysparas.xqzstartflowrate/100,sysparas.xqzstartflowrate%100);
 
    WHPrinterFreeRow(1);
    WHPrintf("     <�����Ʊ����ƾ��>\r\n");//��ע 
    WHPrinterFreeRow(6); 
    SELECT_CARD();
}
void DispCtrlParas1(void)
{
	Fyd12864DispPrintf(1,1,"��->���з�:%d.%02d",sysparas.lowtomidflow/100,sysparas.lowtomidflow%100);
	Fyd12864DispPrintf(2,1,"��->���з�:%d.%02d",sysparas.midtohighflow/100,sysparas.midtohighflow%100);
	Fyd12864DispPrintf(3,1,"ͣ������:%d.%02d",sysparas.lowstopflow/100,sysparas.lowstopflow%100);
	Fyd12864DispPrintf(4,1,"��ȫ����:%d.%02d",sysparas.maxsafeflow/100,sysparas.maxsafeflow%100);
	TaskAdd(KEY_DOWN,DispCtrlParas2, NULL);
    TaskAdd(KEY_RET, DispQuery1,NULL);
	TaskAdd(KEY_OK, PrinterCtrlParas,NULL);
}

void DispCtrlParas2(void)
{
	uint8_t *str;
	if(sysparas.devicetype == 1)
		str = "������";
	else if(sysparas.devicetype == 2)
		str = "������";
	else if(sysparas.devicetype == 3)
		str = "й����";
	else
		str = "δ����";
	
	Fyd12864DispPrintf(1,1,"�豸���ͣ�%s",str);
	Fyd12864DispPrintf(2,1,"�������:%d.%02d",sysparas.maxvolumeonce/100,sysparas.maxvolumeonce%100);
	Fyd12864DispPrintf(3,1,"�����:%d.%02d",sysparas.maxmoneyonce/100,sysparas.maxmoneyonce%100);
	Fyd12864DispPrintf(4,1,"ͣ��ѹ��:%d.%02d",sysparas.stoppressure/100,sysparas.stoppressure%100);
	TaskAdd(KEY_UP,DispCtrlParas1,NULL);
	TaskAdd(KEY_DOWN,DispCtrlParas3, NULL);	
    TaskAdd(KEY_RET, DispQuery1,NULL);
	TaskAdd(KEY_OK, PrinterCtrlParas,NULL);
}
void DispCtrlParas3(void)
{
	Fyd12864DispPrintf(1,1,"������ʱ��:%d",sysparas.filtertime);
	Fyd12864DispPrintf(2,1,"���л�ʱ��:%d",sysparas.valveswitchdelaytime);
	Fyd12864DispPrintf(3,1,"20.5����:%d.%02d",sysparas.flow20_5/100,sysparas.flow20_5%100);
	Fyd12864DispPrintf(4,1,"21.0����:%d.%02d",sysparas.flow21/100,sysparas.flow21%100);
	TaskAdd(KEY_UP,DispCtrlParas2,NULL);
	TaskAdd(KEY_DOWN,DispCtrlParas4, NULL);	
    TaskAdd(KEY_RET, DispQuery1,NULL);
	TaskAdd(KEY_OK, PrinterCtrlParas,NULL);
}
void DispCtrlParas4(void)
{
	Fyd12864DispPrintf(1,1,"21.5����:%d.%02d",sysparas.flow21_5/100,sysparas.flow21_5%100);
	Fyd12864DispPrintf(2,1,"22.0����:%d.%02d",sysparas.flow22/100,sysparas.flow22%100);
	Fyd12864DispPrintf(3,1,"22.5����:%d.%02d",sysparas.flow22_5/100,sysparas.flow22_5%100);
	Fyd12864DispPrintf(4,1,"23.0����:%d.%02d",sysparas.flow23/100,sysparas.flow23%100);
	TaskAdd(KEY_UP,DispCtrlParas3,NULL);
	TaskAdd(KEY_DOWN,DispCtrlParas5, NULL);	
    TaskAdd(KEY_RET, DispQuery1,NULL);
	TaskAdd(KEY_OK, PrinterCtrlParas,NULL);
}
void DispCtrlParas5(void)
{
	Fyd12864DispPrintf(1,1,"23.5����:%d.%02d",sysparas.flow23_5/100,sysparas.flow23_5%100);
	Fyd12864DispPrintf(2,1,"24.0����:%d.%02d",sysparas.flow24/100,sysparas.flow24%100);
	Fyd12864DispPrintf(3,1,"24.5����:%d.%02d",sysparas.flow24_5/100,sysparas.flow24_5%100);
	Fyd12864DispPrintf(4,1,"��������:%d.%02d",sysparas.xqzstartflowrate/100,sysparas.xqzstartflowrate%100);
	TaskAdd(KEY_UP,DispCtrlParas4,NULL);
    TaskAdd(KEY_RET, DispQuery1,NULL);
	TaskAdd(KEY_OK, PrinterCtrlParas,NULL);
}


