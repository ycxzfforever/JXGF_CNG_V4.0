#include "kernel.h"

//struct FuelProcessFixPara  fpfixpar;		//加气过程固定参数
struct FuelProcessVariedPar fpvariedpar;	//加气过程变化参数


void ValveInit(void)
{
	VCTRL_LOW_CLOSE();	//关低阀
	UsDelay(10);
	VCTRL_MID_CLOSE();	//中
	UsDelay(10);
	VCTRL_HIGH_CLOSE();	//高
	UsDelay(10);
	VCTRL_CLOSEKEY();	//关所有
	UsDelay(10);
}

uint8_t ValveSwitchByMassflow(uint32_t flowrate)
//根据流速切换
{
	
	uint32_t  currenttime;
	if(fpvariedpar.ValvePreState==fpvariedpar.ValveCurState) //阀先前状态和当前状态相同时
	{
		
	  
        fpvariedpar.ValveCurState += 1;
        switch(fpvariedpar.ValveCurState)
		{
		   
			case 0x01:
				VCTRL_OPENKEY();			//开总阀
				UsDelay(10);
				VCTRL_LOW_OPEN();			//开低阀
				fpvariedpar.ValveDelayTime = sysparas.valveswitchdelaytime;  //将系统设定的延时时间给阀延时时间
				fpvariedpar.valvemassflow = sysparas.lowtomidflow /10;	//系统设置的低转中切阀流速给流速
				Fyd12864DispPrintf(1,1,"    低压阀");
				fpvariedpar.ValveSTime = Timer1GetTick();		//读取定时器2的当前计时时间作为起始值
				fpvariedpar.SwitchFilterStart = fpvariedpar.ValveSTime;//将起始时间给换阀起始时间
				break;
			case 0x02:
				Fyd12864DispPrintf(1,1,"    低中压阀");
				VCTRL_MID_OPEN();									//开中阀
				fpvariedpar.ValveDelayTime=sysparas.valveswitchdelaytime;
				fpvariedpar.valvemassflow=0xffff;					//流速置0xFFFF
				fpvariedpar.ValveSTime=Timer1GetTick();			//起始时间
				fpvariedpar.SwitchFilterStart=fpvariedpar.ValveSTime;//阀起始时间
				break;
			case 0x03:
				Fyd12864DispPrintf(1,1,"    中压阀");
				VCTRL_LOW_CLOSE();								//关低阀
				fpvariedpar.ValveDelayTime=sysparas.valveswitchdelaytime;
				fpvariedpar.valvemassflow=sysparas.midtohighflow /10;	//系统设定中转高流速给加气可变参数?
				fpvariedpar.ValveSTime=Timer1GetTick();		//获取时间
				break;
			case 0x04:
				VCTRL_HIGH_OPEN();
				Fyd12864DispPrintf(1,1,"    中高压阀");
				fpvariedpar.ValveDelayTime=sysparas.valveswitchdelaytime;
				fpvariedpar.valvemassflow=0xffff;
				fpvariedpar.ValveSTime=Timer1GetTick();
				fpvariedpar.SwitchFilterStart=fpvariedpar.ValveSTime;
				break;
			case 0x05:				
				VCTRL_MID_CLOSE();
				Fyd12864DispPrintf(1,1,"    高压阀");
				fpvariedpar.ValveDelayTime=sysparas.valveswitchdelaytime;
				fpvariedpar.valvemassflow=sysparas.lowstopflow /10;
				fpvariedpar.ValveSTime=Timer1GetTick();
				break;
			case 0x06: 
				VCTRL_HIGH_CLOSE();		//关高压阀
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
	
	if(sysparas.devicetype==JQJ)			//如果是加气机则可通过流速停机
	{
		if((currenttime-fpvariedpar.ValveSTime)>fpvariedpar.ValveDelayTime)	//判断延时时间
		{
			fpvariedpar.ValvePreState = fpvariedpar.ValveCurState; //阀先前状态修改为当前状态用以再次切阀
		}
	}
	else									//如果不是加气机则不能进入case：0x05进行停机
	{
		if(fpvariedpar.ValveCurState != 0x05)
		{
			if((currenttime-fpvariedpar.ValveSTime) > fpvariedpar.ValveDelayTime)	//判断延时时间
			{
				fpvariedpar.ValvePreState = fpvariedpar.ValveCurState; //阀先前状态修改为当前状态用以再次切阀
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
		//阀当前状态为1,2,4  这里其实就是将刚开阀时与切阀的时候不判断压力和流速停机
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
		fpvariedpar.stopreason=0x0c;								//超压停机
		return false;
	}

	if(realtime.cur_flowrate_g>((float)sysparas.maxsafeflow)/100) 
    {
        fpvariedpar.stopreason=0x09;								//高流速停机				
		return false;
    }
	
	if(sysparas.devicetype==JQJ)
	{
		if((curpressure>2000)&&(curpressure<2050))	//大于停机压力才判断压力所在区间对应的流速是否超限
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
		if(curpressure<sysparas.stoppressure)			//小于停机压力
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
