#include "kernel.h"

//struct FuelProcessFixPara  fpfixpar;		//�������̶̹�����
struct FuelProcessVariedPar fpvariedpar;	//�������̱仯����


void ValveInit(void)
{
	VCTRL_LOW_CLOSE();	//�صͷ�
	UsDelay(10);
	VCTRL_MID_CLOSE();	//��
	UsDelay(10);
	VCTRL_HIGH_CLOSE();	//��
	UsDelay(10);
	VCTRL_CLOSEKEY();	//������
	UsDelay(10);
}

uint8_t ValveSwitchByMassflow(uint32_t flowrate)
//���������л�
{
	
	uint32_t  currenttime;
	if(fpvariedpar.ValvePreState==fpvariedpar.ValveCurState) //����ǰ״̬�͵�ǰ״̬��ͬʱ
	{
		
	  
        fpvariedpar.ValveCurState += 1;
        switch(fpvariedpar.ValveCurState)
		{
		   
			case 0x01:
				VCTRL_OPENKEY();			//���ܷ�
				UsDelay(10);
				VCTRL_LOW_OPEN();			//���ͷ�
				fpvariedpar.ValveDelayTime = sysparas.valveswitchdelaytime;  //��ϵͳ�趨����ʱʱ�������ʱʱ��
				fpvariedpar.valvemassflow = sysparas.lowtomidflow /10;	//ϵͳ���õĵ�ת���з����ٸ�����
				Fyd12864DispPrintf(1,1,"    ��ѹ��");
				fpvariedpar.ValveSTime = Timer1GetTick();		//��ȡ��ʱ��2�ĵ�ǰ��ʱʱ����Ϊ��ʼֵ
				fpvariedpar.SwitchFilterStart = fpvariedpar.ValveSTime;//����ʼʱ���������ʼʱ��
				break;
			case 0x02:
				Fyd12864DispPrintf(1,1,"    ����ѹ��");
				VCTRL_MID_OPEN();									//���з�
				fpvariedpar.ValveDelayTime=sysparas.valveswitchdelaytime;
				fpvariedpar.valvemassflow=0xffff;					//������0xFFFF
				fpvariedpar.ValveSTime=Timer1GetTick();			//��ʼʱ��
				fpvariedpar.SwitchFilterStart=fpvariedpar.ValveSTime;//����ʼʱ��
				break;
			case 0x03:
				Fyd12864DispPrintf(1,1,"    ��ѹ��");
				VCTRL_LOW_CLOSE();								//�صͷ�
				fpvariedpar.ValveDelayTime=sysparas.valveswitchdelaytime;
				fpvariedpar.valvemassflow=sysparas.midtohighflow /10;	//ϵͳ�趨��ת�����ٸ������ɱ����?
				fpvariedpar.ValveSTime=Timer1GetTick();		//��ȡʱ��
				break;
			case 0x04:
				VCTRL_HIGH_OPEN();
				Fyd12864DispPrintf(1,1,"    �и�ѹ��");
				fpvariedpar.ValveDelayTime=sysparas.valveswitchdelaytime;
				fpvariedpar.valvemassflow=0xffff;
				fpvariedpar.ValveSTime=Timer1GetTick();
				fpvariedpar.SwitchFilterStart=fpvariedpar.ValveSTime;
				break;
			case 0x05:				
				VCTRL_MID_CLOSE();
				Fyd12864DispPrintf(1,1,"    ��ѹ��");
				fpvariedpar.ValveDelayTime=sysparas.valveswitchdelaytime;
				fpvariedpar.valvemassflow=sysparas.lowstopflow /10;
				fpvariedpar.ValveSTime=Timer1GetTick();
				break;
			case 0x06: 
				VCTRL_HIGH_CLOSE();		//�ظ�ѹ��
				VCTRL_CLOSEKEY();
				fpvariedpar.valvemassflow=0xFFFF;
				return 0;
		}
	}
	if(flowrate >= fpvariedpar.valvemassflow) 
	{
		fpvariedpar.ValveSTime = Timer1GetTick();
		return 1;
	}
	currenttime = Timer1GetTick(); 	
	
	if(sysparas.devicetype==JQJ)			//����Ǽ��������ͨ������ͣ��
	{
		if((currenttime-fpvariedpar.ValveSTime)>fpvariedpar.ValveDelayTime)	//�ж���ʱʱ��
		{
			fpvariedpar.ValvePreState = fpvariedpar.ValveCurState; //����ǰ״̬�޸�Ϊ��ǰ״̬�����ٴ��з�
		}
	}
	else									//������Ǽ��������ܽ���case��0x05����ͣ��
	{
		if(fpvariedpar.ValveCurState != 0x05)
		{
			if((currenttime-fpvariedpar.ValveSTime) > fpvariedpar.ValveDelayTime)	//�ж���ʱʱ��
			{
				fpvariedpar.ValvePreState = fpvariedpar.ValveCurState; //����ǰ״̬�޸�Ϊ��ǰ״̬�����ٴ��з�
			}
		}		
	}
	return 1;
}




uint8_t PressureWithFlow(uint16_t flowrate, uint32_t curpressure)
{
	
	uint32_t curtime;
	if(sysparas.devicetype == JQJ)
	{
		//����ǰ״̬Ϊ1,2,4  ������ʵ���ǽ��տ���ʱ���з���ʱ���ж�ѹ��������ͣ��
		if((fpvariedpar.ValveCurState==0x01)||(fpvariedpar.ValveCurState==0x02)||(fpvariedpar.ValveCurState==0x04))
		{
			curtime=Timer1GetTick();
			if((curtime - fpvariedpar.SwitchFilterStart) < sysparas.filtertime)
			{
				return true;
			}
		}
	}

	if(curpressure>=2450)				//20MP<-->2890
	{
		fpvariedpar.stopreason=0x0c;								//��ѹͣ��
		return false;
	}

	if(realtime.cur_flowrate_g>((float)sysparas.maxsafeflow)/100) 
    {
        fpvariedpar.stopreason=0x09;								//������ͣ��				
		return false;
    }
	
	if(sysparas.devicetype==JQJ)
	{
		if((curpressure>2000)&&(curpressure<2050))	//����ͣ��ѹ�����ж�ѹ�����������Ӧ�������Ƿ���
		{
			if(flowrate<sysparas.flow20_5)
			{
				fpvariedpar.stopreason=0x08;
				return false;
			}
		}
		else if((curpressure>=2050)&&(curpressure<2100))
		{
			if(flowrate<sysparas.flow21)
			{
				fpvariedpar.stopreason=0x08;
				return false;
			}
		}
		else if((curpressure>=2100)&&(curpressure<2150))
		{
			if(flowrate<sysparas.flow21_5)
			{
				fpvariedpar.stopreason=0x08;
				return false;
			}
		}
		else if((curpressure>=2150)&&(curpressure<2200))
		{
			if(flowrate<sysparas.flow22)
			{
				fpvariedpar.stopreason=0x08;
				return false;
			}
		}
		else if((curpressure>=2200)&&(curpressure<2250))
		{
			if(flowrate<sysparas.flow22_5)
			{
				fpvariedpar.stopreason=0x08;
				return false;
			}
		}
		else if((curpressure>=2250)&&(curpressure<2300))
		{
			if(flowrate<sysparas.flow23)
			{
				fpvariedpar.stopreason=0x08;
				return false;
			}
		}
		else if((curpressure>=2300)&&(curpressure<2350))
		{
			if(flowrate<sysparas.flow23_5)
			{
				fpvariedpar.stopreason=0x08;
				return false;
			}
		}
		else if((curpressure>=2350)&&(curpressure<2400))
		{
			if(flowrate<sysparas.flow24)
			{
				fpvariedpar.stopreason=0x08;
				return false;
			}
		}
		else if((curpressure>=2400)&&(curpressure<2450))
		{
			if(flowrate<sysparas.flow24_5)
			{
				fpvariedpar.stopreason=0x08;
				return false;
			}
		}
	}
	else if(sysparas.devicetype==JQZ)
	{
		if(curpressure<sysparas.stoppressure)			//С��ͣ��ѹ��
		{
			return true;
		}
		else
		{
			fpvariedpar.stopreason=12;
			return false;
		}
	}
	return true;
}
