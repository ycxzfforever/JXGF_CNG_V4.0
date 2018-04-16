#ifndef __CNGFUELREADY_H
#define __CNGFUELREADY_H
#include "kernel.h"
/*
struct FuelProcessFixPara
//���������й̶�����
{
  
	unsigned long switchdelay;
	unsigned long mincardmoney;//���������������
	unsigned long maxaccountmoney;//���˿�������ѽ��
	float defaultPrice;//Ĭ�ϵ���
	float fixCofficient;//�����������������ϵ��
//	float calfactor;//�������̱���ϵ��
};
*/

struct FuelProcessVariedPar
//���������б仯����
{
	uint8_t ValvePreState;//����ǰ״̬
	uint8_t ValveCurState;//����ǰ״̬
	uint32_t ValveDelayTime;//���ӳ�ʱ��
	uint32_t ValveSTime;//��ʼʱ��
	uint32_t valvemassflow;//����
	uint32_t maxSafeFlowStart;//���ȫ������ʼʱ��
	uint32_t SwitchFilterStart;//������ʼʱ��
	uint32_t switchdelay;
	uint8_t  stoperrorflag;//�����ƴ����˲ʱ���ݴ���
	uint32_t stopreason;  //ͣ��ԭ��
	        //0x00---�ֶ�ͣ��
			//0x01---�����������쳣�������ٻ��������   
	        //0x02---Һ���������쳣
	        //0x03---˲ʱ�����쳣
			//0x04---��ѹ������������
	        //0x05---�����λ
	        //0x06---������
	        //0x07---�����
	        //0x08---������ͣ��(��ѹ��ʱ�����ٵ����������٣�ѹ������20MPa,����С������ֵ)
			//0x09---������
	        //0x0a---�����ܶ�
	        //0x0b---�������� 
	        //0x0c---��ѹ������24.5MPa)
			//0x0d---�ӿ�     
            //0x0e---���˿�����	
			//0x0f---���γ����������������������
	        		
	uint32_t moneybackup;		//���������ж�˲ʱ
	uint32_t volumebackup;		//�������������ж�˲ʱ
	double cngvolume;//������
	float calfactor;//�������̱���ϵ��
	float startgasvolume; //���������ƿ�ʼ����
};

//extern struct FuelProcessFixPara  fpfixpar;
extern struct FuelProcessVariedPar fpvariedpar;

extern uint8_t ValveSwitchByMassflow(uint32_t flowrate);
extern uint8_t PressureWithFlow(uint16_t flowrate, uint32_t curpressure);
#endif
