#ifndef __CNGFUELREADY_H
#define __CNGFUELREADY_H
#include "kernel.h"
/*
struct FuelProcessFixPara
//加气过程中固定参数
{
  
	unsigned long switchdelay;
	unsigned long mincardmoney;//卡的最低消费限制
	unsigned long maxaccountmoney;//记账卡最大消费金额
	float defaultPrice;//默认单价
	float fixCofficient;//定气量、定金额修正系数
//	float calfactor;//加气过程比例系数
};
*/

struct FuelProcessVariedPar
//加气过程中变化参数
{
	uint8_t ValvePreState;//阀先前状态
	uint8_t ValveCurState;//阀当前状态
	uint32_t ValveDelayTime;//阀延迟时间
	uint32_t ValveSTime;//起始时间
	uint32_t valvemassflow;//流速
	uint32_t maxSafeFlowStart;//最大安全流速起始时间
	uint32_t SwitchFilterStart;//换阀起始时间
	uint32_t switchdelay;
	uint8_t  stoperrorflag;//流量计错误或瞬时数据错误
	uint32_t stopreason;  //停机原因
	        //0x00---手动停机
			//0x01---气相流量计异常（读流速或读流量）   
	        //0x02---液相流量计异常
	        //0x03---瞬时数据异常
			//0x04---读压力传感器错误
	        //0x05---掉电或复位
	        //0x06---定气量
	        //0x07---定金额
	        //0x08---低流速停机(高压阀时，流速低于设置流速；压力大于20MPa,流速小于区间值)
			//0x09---高流速
	        //0x0a---回气密度
	        //0x0b---回气增益 
	        //0x0c---超压（大于24.5MPa)
			//0x0d---逃卡     
            //0x0e---记账卡超额	
			//0x0f---单次超过最大加气量或最大加气金额
	        		
	uint32_t moneybackup;		//金额备份用于判断瞬时
	uint32_t volumebackup;		//气量备份用于判断瞬时
	double cngvolume;//加气量
	float calfactor;//加气过程比例系数
	float startgasvolume; //气象流量计开始总累
};

//extern struct FuelProcessFixPara  fpfixpar;
extern struct FuelProcessVariedPar fpvariedpar;

extern uint8_t ValveSwitchByMassflow(uint32_t flowrate);
extern uint8_t PressureWithFlow(uint16_t flowrate, uint32_t curpressure);
#endif
