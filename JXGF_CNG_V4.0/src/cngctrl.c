#include "kernel.h"

/***************************************************************************
**�������ƣ� JudgeStopConditions
**�������ܣ��ж�ͣ������
**��ڲ�������
**����ֵ����
**�����ߣ����
**�������ڣ�2016-5-10 13:06:28
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void JudgeStopConditions(void)
{
    if(globalvar.fix==1)//��������
    {
        if((fuelrecordinfo.volume+1)>=globalvar.fixnum)
        {
            fuelrecordinfo.volume=globalvar.fixnum;
            fpvariedpar.stopreason= 0x06;
        }
    }
    if(globalvar.fix==2)//����
    {
        if((fuelrecordinfo.money+1)>=globalvar.fixmoney)
        {
            fuelrecordinfo.money=globalvar.fixmoney;
            fpvariedpar.stopreason = 0x07;
        }
    }
    if(m1card.existflag==1)//������ÿ�������
    {
		if(m1card.escapeflag==0xaa)
		{
			fpvariedpar.stopreason=0x0d;
			globalvar.fix=0;
		}
		if(m1card.cardtype==2)//���˿���
        {
            if((fuelrecordinfo.money+m1card.cardbalance)>=sysparas.cardmaxyue)//���˿�������ޡ�
            {
                fpvariedpar.stopreason=0x0e;
				globalvar.fix=0;
            }
			
        }     
    }
}

/***************************************************************************
**�������ƣ�PowerDown
**�������ܣ�������ʾ������
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void PowerDown(void)
{
    PUMPCTRL_HIGH_OPEN();	//PLC
	PUMPCTRL_LOW_OPEN();	//PLC
	VCTRL_CLOSEKEY();
	VCTRL_HIGH_CLOSE() ;
	VCTRL_MID_CLOSE();
	VCTRL_LOW_CLOSE();                                             //ͣ����
    Fyd12864DispPrintf(1,1,"ϵͳ�Ѿ�����...");
    Fyd12864DispPrintf(4,1,"�����ѱ���");
    DispUI=PowerDown;
}

/***************************************************************************
**�������ƣ�PowerDownRealDate
**�������ܣ��������ݻָ�������
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void PowerDownRealDate(void)
{
    sysparas.transnum=realtime.transnum;                                //��ˮ�š�
    sysparas.totvolume+=realtime.volume;                                //ǹ��������
    sysparas.totmoney+=realtime.money;                                  //ǹ�۽�
    sysparas.shiftotvol+=realtime.volume;                               //����������
    sysparas.shiftotmon+=realtime.money;                                //����������
    sysparas.shiftfueltimes++;                                          //�����������
    
    fuelrecordinfo.transnum=realtime.transnum;                          //��ˮ�š�
    fuelrecordinfo.classnum=sysparas.shiftnum;                          //��š�;
    memcpy(&fuelrecordinfo.employeenum[0],&realtime.employeenum[0],8);  //Ա���š�
    fuelrecordinfo.transactiontype=realtime.transactiontype;            //�ӿ����ס�
    memcpy(&fuelrecordinfo.usernum[0],&realtime.cardnum[0],8);          //�û����š�
    fuelrecordinfo.cardtype=realtime.cardtype;                          //�����͡�
    memcpy(&fuelrecordinfo.carnum[0],&realtime.carnum[0],9);            //���ƺš�
    memcpy(&fuelrecordinfo.stationnum[0],&sysparas.stationcode[0],4);   //վ���롣
    memcpy(&fuelrecordinfo.starttime[0],&realtime.starttime[0],7);      //������ʼʱ�䡣
    memcpy(&fuelrecordinfo.endtime[0],&realtime.endtime[0],7);          //��������ʱ�䡣
   
    fuelrecordinfo.price=realtime.price;                                //���ۡ�
    fuelrecordinfo.volume=realtime.volume;                              //������
    fuelrecordinfo.money=realtime.money;                                //��
   
    fuelrecordinfo.reallymoney=realtime.reallymoney;                    //ʵ�ʿۿ��
    fuelrecordinfo.beforebalance=realtime.beforebalance;                //����ǰ��
    fuelrecordinfo.afterbalance=realtime.afterbalance;                  //��������
    fuelrecordinfo.allrechargetimes=realtime.allrechargetimes;          //�ۼƳ�ֵ������
    
   
    fuelrecordinfo.allrechargemoney=realtime.allrechargemoney;          //�ۼƳ�ֵ��
    
    fuelrecordinfo.allconsumptiontimes=realtime.allconsumptiontimes;    //�ۼ����Ѵ�����
    fuelrecordinfo.allconsumptionnum=realtime.allconsumptionnum;        //�ۼ�����������
    fuelrecordinfo.allconsumptionmoney=realtime.allconsumptionmoney;    //�ۼ����ѽ�
    fuelrecordinfo.allgunnum=sysparas.totvolume;                        //ǹ��������
    fuelrecordinfo.allgunmoney=sysparas.totmoney;                       //ǹ�۽�
    fuelrecordinfo.beforepressure=realtime.beforepressure;              //����ǰѹ����
    fuelrecordinfo.afterpressure=realtime.afterpressure;                //������ѹ����
    fuelrecordinfo.density=(uint32_t)(realtime.cur_density_g*10000+0.5);//�ܶȡ�
    fuelrecordinfo.curnum=realtime.curnum;                              //������
    fuelrecordinfo.devicetype=realtime.lcng;                            //��Ʒ���롣
    fuelrecordinfo.measuretype=realtime.measuretype;                    //������λ��
    fuelrecordinfo.stopreason=realtime.stopreason;                      //ͣ��ԭ��
    fuelrecordinfo.jhid=realtime.jhid;                                  //��������ˮ�š�
    fuelrecordinfo.classid=realtime.classid;                            //����ˮ�š�
    fuelrecordinfo.crc=CRC16Calculate((uint8_t *)(&fuelrecordinfo.transnum),sizeof(FuelRecordInfo)-2);
    
    
    SaveSysparas(0,(uint8_t*)&sysparas.head,sizeof(Sysparas));
    MsDelay(100);
    if(SaveFuelRecord(0))
    {
        Fyd12864DispPrintf(4,1,"�洢��ˮ�ɹ�"); 
    }
    else
    {
        Fyd12864DispPrintf(4,1,"�洢��ˮʧ��");  
    }
}

/***************************************************************************
**�������ƣ� FuelReady
**�������ܣ�����ǰ׼��
**��ڲ�������
**����ֵ����
**�����ߣ��ƺ���
**�������ڣ�2016-4-26 16:17:52
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
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
				Fyd12864DispPrintf(3,1,"  �������������");
				TaskMessage(20,DispFirst);
				return;
			}
		}												//����������
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
		Fyd12864DispPrintf(2,1,"  ��ȡ��������...");
		TaskAdd(KEY_STOP, DispFirst, NULL);
	}
	else  if(globalvar.pressure_error)
	{
		Fyd12864ClearLine(1);
		Fyd12864ClearLine(3);
		Fyd12864ClearLine(4);
		Fyd12864DispPrintf(2,1,"  ��ѹ��ʧ��");
		TaskMessage(30,DispFirst);
	}
	else
	{	
		memset(&fpvariedpar.ValvePreState,0,sizeof(struct FuelProcessVariedPar));
		fpvariedpar.stopreason=0xaa;//��ʼֵ
		//��ȡ����ϵ��
		if(sysparas.unit==0)		//������λ  ���������?
		{
			fpvariedpar.calfactor=(float)(sysparas.coeff/10000.00);	//����ϵ��������ϵ��������
		}
		else 
		{
			fpvariedpar.calfactor=((float)sysparas.coeff)/((float)sysparas.densty); //���	���� / �ܶ�
		}

		globalvar.fuelstep=1;                                                    //��������1��
		globalvar.workstate=StaFuel;                                              //����״̬��
		
		memset(&fuelrecordinfo,0,sizeof(FuelRecordInfo));                         //����ˮ�ṹ�塣
		fuelrecordinfo.classnum=sysparas.shiftnum;                                //��š�
		memcpy(&fuelrecordinfo.employeenum[0],&shiftrecordinfo.cardstaffnum[0],8);//Ա���š�
		memcpy(&fuelrecordinfo.usernum[0],&m1card.cardnum[0],8);              //�û����š�
		memcpy(&fuelrecordinfo.carnum[0],&m1card.carnum[0],9);                //���ƺš�
		fuelrecordinfo.cardtype=m1card.cardtype;                              //�����͡� 		
		memcpy(&fuelrecordinfo.stationnum[0],&sysparas.stationcode[0],4);         //վ���롣
		fuelrecordinfo.starttime[0]=0x20;
		memcpy(&fuelrecordinfo.starttime[1], &time.year, 6);                      //������ʼʱ�䡣		
		fuelrecordinfo.devicetype =sysparas.devicetype;                           //�豸����                                                 //��Ʒ���룬1�ֽڡ�0-CNG
		fuelrecordinfo.beforepressure=realtime.cur_pressure;                      //����ǰѹ����    
		fuelrecordinfo.beforebalance=m1card.cardbalance;                      //����ǰ��
		fuelrecordinfo.afterbalance=m1card.cardbalance;                       //��������
		fuelrecordinfo.allrechargemoney=m1card.sumchargemoney;                //�ۼƳ�ֵ���
		fuelrecordinfo.allrechargetimes=m1card.sumchargetimes;                //�ۼƳ�ֵ������
		fuelrecordinfo.allconsumptiontimes=m1card.sumtimes+1;                 //�ۼ����Ѵ�����
	   
		fpvariedpar.startgasvolume=realtime.cur_totalize_g;    					  //��¼��ʼ����
//		fpvariedpar.startgasvolume=(unsigned long)(fpvariedpar.startgasvolume*100); //����Ҫȡ��λС��
//		fpvariedpar.startgasvolume=(float)(fpvariedpar.startgasvolume/100.00);		//�������ֿ�ʼ�����ͳ���0.01�����
			
		realtime.powerdown=0;                                                     //δ�����־��
		if(m1card.existflag)
		{
			fuelrecordinfo.price=sysparas.price[m1card.pricelevel];               //�п����׼۸�
			realtime.price=sysparas.price[m1card.pricelevel];					  //�п���ǰʹ�õĵ���
			if(m1card.cardtype==1)//���  �������Ϊ������
			{
				availablemoney=m1card.cardbalance-sysparas.cardminyue; 
				if(globalvar.fix==1)//֮ǰ����������
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
			fuelrecordinfo.price=sysparas.price[0];              					 //���׼۸�
			realtime.price=sysparas.price[0];					  					//��ǰʹ�õĵ���
		}

		SaveFm25l04();                                                            //д���硣
		if(sysparas.simulation)
		{
			realtime.cur_flowrate_g = 9;
		}
		if((sysparas.gunnum==0)||(sysparas.gunnum>99))
		{
			
			Fyd12864ClearLine(1);
			Fyd12864ClearLine(3);
			Fyd12864ClearLine(4);
			Fyd12864DispPrintf(2,1,"  ������ǹ��");
			TaskMessage(20, DispFirst);

		}
		else if(realtime.price < 10 || realtime.price > 10000)
		{
			Fyd12864ClearLine(1);
			Fyd12864ClearLine(3);
			Fyd12864ClearLine(4);
			Fyd12864DispPrintf(2,1,"  ���۴���");
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

//��Һ��ʾ
//double    fpvariedpar.cngvolume=0;//������
/***************************************************************************
**�������ƣ� FuelWork
**�������ܣ���������
**��ڲ�������
**����ֵ���� 
**�����ߣ��ƺ���
**�������ڣ�2016-11-10 16:17:52
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void FuelWork(void)
{
	globalvar.displayonce = 1;
	globalvar.workstate = StaFuel;
    globalvar.fuelstep=2;//��������2
	if(globalvar.KeyValue==KEY_STOP)
	{
		globalvar.fix=0;
		fpvariedpar.stopreason=0;
	}
	
	if(sysparas.intnumflag==0x01)
	{
		if(globalvar.KeyValue==KEY_PT)//��С����ȡ����������
		{
			globalvar.fix=0x7;          //����
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
		fpvariedpar.cngvolume = fpvariedpar.cngvolume * fpvariedpar.calfactor;		//���ϱ���ϵ�� 		
    }
 
    fuelrecordinfo.volume=(uint32_t)(fpvariedpar.cngvolume*100);//��ǰ����������   
    fpvariedpar.cngvolume = (double)fuelrecordinfo.volume/100.0;
    fuelrecordinfo.money=(uint32_t)(fpvariedpar.cngvolume*realtime.price+0.5);//��ǰ���׽�
			
	if(!ValveSwitchByMassflow((uint32_t)(realtime.cur_flowrate_g*10)))		//�����з�
	{
		globalvar.fix=0;
		fpvariedpar.stopreason=0x08;	
	}
	
	if(!PressureWithFlow((uint16_t)realtime.cur_flowrate_g, realtime.cur_pressure))	//ѹ��ͣ��������ͣ��
	{
		globalvar.fix=0;
	}
    
    JudgeStopConditions();//�ж�ͣ��������
    
	realtime.volume=fuelrecordinfo.volume;
	realtime.money=fuelrecordinfo.money;    
	OsplcShow(fuelrecordinfo.volume,fuelrecordinfo.money,realtime.price,0,0);//������ʾ��
	
    Fyd12864DispPrintf(2,1,"����:%ld.%02d",fuelrecordinfo.volume/100,fuelrecordinfo.volume%100);
    Fyd12864DispPrintf(3,1,"���:%ld.%02d",fuelrecordinfo.money/100,fuelrecordinfo.money%100);
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
	if(fpvariedpar.stopreason!=0xaa)//��ʼֵ
	{
		globalvar.KeyValue=KEY_STOP;
	}
    TaskAdd(KEY_STOP,FuelEnd, NULL);    
    realtime.powerdown=0;          //δ�����־��
    SaveFm25l04();                 //д���硣
	
    if(PowerCheckIn!=0)            //�ж��Ƿ���硣 
    { 
        MsDelay(20);
        if(PowerCheckIn!=0)
        {
			fpvariedpar.stopreason=0x05;//����
            realtime.powerdown=1;      //�ѵ��硣
            SaveFm25l04();             //д���硣
            PowerDown();
        }
    }
	timer1.tick = 0;
}

void FuelEndDisplay(void)
/***************************************************************************
**�������ƣ� FuelEndDisplay
**�������ܣ����������洢�ʹ�ӡ
**��ڲ�������
**����ֵ���� 
**�����ߣ��ƺ���
**�������ڣ�2016-11-10 16:17:52
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
{
	if(globalvar.displayonce==1)
	{
		globalvar.displayonce=0;
		globalvar.workstate=StaIdle;
		if(SaveFuelRecord(0))
		{
			Fyd12864DispPrintf(1,1,"    ��������");
			Fyd12864ClearLine(2);
			Fyd12864DispPrintf(3,1,(char *)&stopreasonbuf[fuelrecordinfo.stopreason]);
			/*
			switch(fpvariedpar.stopreason)
			{
				case 0x00:
					Fyd12864DispPrintf(3,1,"�ֶ�ֹͣ");
					break;
				case 0x01:
					Fyd12864DispPrintf(3,1,"�����������쳣");
					break;
				case 0x02:
					Fyd12864DispPrintf(3,1,"Һ���������쳣");
					break;
				case 0x03:
					Fyd12864DispPrintf(3,1,"˲ʱ���ݴ���");
					break;
				case 0x04:
					Fyd12864DispPrintf(3,1,"ѹ���������쳣");
					break;
				case 0x05:
					Fyd12864DispPrintf(3,1,"  �����λ");
					break;
				case 0x06:
					Fyd12864DispPrintf(3,1,"    ������");
					break;
				case 0x07:
					Fyd12864DispPrintf(3,1,"    �����");
					break;
				case 0x08:
					Fyd12864DispPrintf(3,1,"    ������");
					break;
				case 0x09:
					Fyd12864DispPrintf(3,1,"    ������");
					break;
				case 0x0a:
					Fyd12864DispPrintf(3,1,"    �����ܶ�");
					break;
				case 0x0b:
					Fyd12864DispPrintf(3,1,"    ��������");
					break;
				case 0x0c:
					Fyd12864DispPrintf(3,1,"      ��ѹ");
					break;
				case 0x0d:
					Fyd12864DispPrintf(3,1,"      �ӿ�");
					break;
				case 0x0e:
					Fyd12864DispPrintf(3,1,"  ���˿�����");
					break;
				case 0x0f:
					Fyd12864DispPrintf(3,1,"���γ����򳬶�");
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
			Fyd12864DispPrintf(4,1,"�洢��ˮʧ��");  
		}
		if(sysparas.printmode==1)
		{
			Printid();
		}
		memset(&realtime.lcng,0,sizeof(RealTimeData));  //ycx����������ʵʱ���ݡ�
        Fm25l04Write(0,(uint8_t *)(&realtime.lcng),sizeof(RealTimeData));
	}
	TaskAdd(KEY_RET, DispFirst, NULL);
	TaskMessage(30, DispFirst);// 2s�󷵻�������
}
/***************************************************************************
**�������ƣ� FuelEnd
**�������ܣ���������
**��ڲ�������
**����ֵ���� 
**�����ߣ��ƺ���
**�������ڣ�2016-11-10 16:17:52
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
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
		timer1off(); //�����ƴ���Ͳ��ٶ�ȡ������
		fuelrecordinfo.money=fpvariedpar.moneybackup;
		fuelrecordinfo.volume=fpvariedpar.volumebackup;
		OsplcShow(fuelrecordinfo.volume,fuelrecordinfo.money,realtime.price,0,0);//������ʾ��
		Fyd12864DispPrintf(2,1,"����:%ld.%02d",fuelrecordinfo.volume/100,fuelrecordinfo.volume%100);
		Fyd12864DispPrintf(3,1,"���:%ld.%02d",fuelrecordinfo.money/100,fuelrecordinfo.money%100);
		Fyd12864DispPrintf(4,1,"V:%.02f P:%ld.%02d",realtime.cur_flowrate_g,realtime.cur_pressure/100,realtime.cur_pressure%100); 
		fpvariedpar.stoperrorflag=0;
		globalvar.fix=0xaa;
		timer1.tick=20;
	}
	else if(globalvar.fix==0x01)//������
	{
		fpvariedpar.cngvolume=(float)(globalvar.fixnum/100.00);
		fuelrecordinfo.volume=fpvariedpar.cngvolume*100+0.5;//��ǰ����������   
		fuelrecordinfo.money=(uint32_t)(fpvariedpar.cngvolume*realtime.price+0.5);//��ǰ���׽�
		OsplcShow(fuelrecordinfo.volume,fuelrecordinfo.money,realtime.price,0,0);//������ʾ��
		Fyd12864DispPrintf(2,1,"����:%ld.%02d",fuelrecordinfo.volume/100,fuelrecordinfo.volume%100);
		Fyd12864DispPrintf(3,1,"���:%ld.%02d",fuelrecordinfo.money/100,fuelrecordinfo.money%100);
		Fyd12864DispPrintf(4,1,"V:%.02f P:%ld.%02d",realtime.cur_flowrate_g,realtime.cur_pressure/100,realtime.cur_pressure%100); 
		globalvar.fix=0xaa;
		timer1.tick=20;
	}
	else if(globalvar.fix==0x02)//�����
	{
		fpvariedpar.cngvolume=(double)globalvar.fixmoney/realtime.price;	
		fuelrecordinfo.volume=fpvariedpar.cngvolume*100+0.5;//��ǰ��������
		fuelrecordinfo.money=globalvar.fixmoney;
		Fyd12864DispPrintf(2,1,"����:%ld.%02d",fuelrecordinfo.volume/100,fuelrecordinfo.volume%100);	
		Fyd12864DispPrintf(3,1,"���:%ld.%02d",fuelrecordinfo.money/100,fuelrecordinfo.money%100);
		Fyd12864DispPrintf(4,1,"V:%.02f P:%ld.%02d",realtime.cur_flowrate_g,realtime.cur_pressure/100,realtime.cur_pressure%100); 
		globalvar.fix=0xaa;
		timer1.tick=20;
	}
	else if((timer1.tick<20)&&(sysparas.simulation==0))
	{
		fpvariedpar.cngvolume=realtime.cur_totalize_g-fpvariedpar.startgasvolume;
		fpvariedpar.cngvolume = fpvariedpar.cngvolume * fpvariedpar.calfactor;		//���ϱ���ϵ�� 	
        fpvariedpar.cngvolume=(uint32_t)(fpvariedpar.cngvolume*100);//��ǰ��������
		fpvariedpar.cngvolume=(double)(fpvariedpar.cngvolume/100);//��ǰ��������
		fuelrecordinfo.volume=fpvariedpar.cngvolume*100;//��ǰ����������   
		fuelrecordinfo.money=(uint32_t)(fpvariedpar.cngvolume*realtime.price+0.5);//��ǰ���׽�
		OsplcShow(fuelrecordinfo.volume,fuelrecordinfo.money,realtime.price,0,0);//������ʾ��
		Fyd12864DispPrintf(2,1,"����:%ld.%02d",fuelrecordinfo.volume/100,fuelrecordinfo.volume%100);
		Fyd12864DispPrintf(3,1,"���:%ld.%02d",fuelrecordinfo.money/100,fuelrecordinfo.money%100);
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
			WHPrintf("����:%.02f\r\n",realtime.cur_totalize_g); 
			WHPrintf("��ʼ:%.02f\r\n",fpvariedpar.startgasvolume); 
			WHPrintf("����:%.02f\r\n",(float)fpvariedpar.cngvolume); 
//			WHPrinterFreeRow(3);
			SELECT_CARD();
		}
		fuelrecordinfo.stopreason=fpvariedpar.stopreason;
		globalvar.fuelstep=3;//��������3
		if(globalvar.displayonce==1)
		{

			globalvar.displayonce=0;
			fpvariedpar.cngvolume=0;                                 //��Һ�����������������㡣
			
			sysparas.transnum++;
			sysparas.totvolume += fuelrecordinfo.volume;//ǹ��������
			sysparas.totmoney += fuelrecordinfo.money;  //ǹ�ܽ�
			sysparas.shiftotvol+= fuelrecordinfo.volume;//���ۼ�������
			sysparas.shiftotmon+= fuelrecordinfo.money; //���ۼƽ�
			sysparas.shiftfueltimes++;                  //�����������
			fuelrecordinfo.transnum=sysparas.transnum;  //������ˮ�š�
			fuelrecordinfo.allconsumptionnum=m1card.sumvolme+fuelrecordinfo.volume;//�ۼ�����������
			if(m1card.existflag==1)
			{
				fuelrecordinfo.transactiontype=0xFF;                        //�������͡�
				fuelrecordinfo.reallymoney=0x00;                      //����ʵ�ʿۿ��
				fuelrecordinfo.allconsumptionmoney=m1card.summoney;     //�ۼ����ѽ�     
			}
			else
			{
				fuelrecordinfo.transactiontype=0;                           //�������͡�
				fuelrecordinfo.reallymoney=fuelrecordinfo.money;            //����ʵ�ʿۿ��
				fuelrecordinfo.allconsumptionmoney=fuelrecordinfo.money;    //�ۼ����ѽ�
			}
			fuelrecordinfo.endtime[0]=0x20;
			memcpy(&fuelrecordinfo.endtime[1], &time.year, 6);              //��������ʱ�䡣
			
			fuelrecordinfo.allgunnum=sysparas.totvolume;                    //ǹ�����ۼơ�
			fuelrecordinfo.allgunmoney=sysparas.totmoney;                   //ǹ����ۼơ�
			fuelrecordinfo.afterpressure=realtime.cur_pressure;             //������ѹ����
			fuelrecordinfo.curnum=sysparas.coeff;                           //������
			fuelrecordinfo.measuretype=sysparas.unit;                       //������λ��0-�귽��1-Kg��
			fuelrecordinfo.density=realtime.cur_density_g;                  //�ܶȡ�
			fuelrecordinfo.classid=sysparas.shiftransnum;                   //����ˮ�š�
			
			fuelrecordinfo.crc=CRC16Calculate((uint8_t *)(&fuelrecordinfo.transnum),sizeof(FuelRecordInfo)-2);
			OsplcShow(fuelrecordinfo.volume,fuelrecordinfo.money,realtime.price,0,1);
			
			SaveSysparas(0,(uint8_t*)&sysparas.head,sizeof(Sysparas));
			m1card.debittimes=100;                                       //�������100�οۿ
		}
		if((m1card.existflag==1)&&(PowerCheckIn==0)) 
		{
			globalvar.workstate=StaPay;
			Fyd12864DispPrintf(1,1,"���ڵȴ��ۿ�.."); 
			if(m1card.debittimes==0)
			{
				globalvar.displayonce=1;
				DispUI=FuelEndDisplay;
			}
		}
		else if(PowerCheckIn==0)//δ����
		{
			globalvar.displayonce=1;
			DispUI=FuelEndDisplay;
		}
		realtime.powerdown=0;          //δ�����־��
		SaveFm25l04();                 //д���硣
		if(PowerCheckIn!=0)            //�ж��Ƿ���硣 
		{ 
			MsDelay(20);
			if(PowerCheckIn!=0)
			{
				realtime.powerdown=1;  //�ѵ��硣
				SaveFm25l04();         //д���硣
				PowerDown();
			}
		}
	}
}

/***************************************************************************
**�������ƣ�FixGas
**�������ܣ�����ѡ������
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void FixGas(void)
{
	//if(globalvar.displayonce==0x01)
	{
		globalvar.displayonce=0;
		Fyd12864DispPrintf(1,1,"    ��������");
		Fyd12864DispPrintf(2,1,"1.������");
		Fyd12864DispPrintf(3,1,"2.�����");
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
**�������ƣ�FixAffirm
**�������ܣ�����ȷ��
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ��ƺ���
**�������ڣ�2016-11-14
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
{
	if(globalvar.fix==1)
	{
		Fyd12864DispPrintf(1,1,"������:%ld.%02d",globalvar.fixnum/100,globalvar.fixnum%100);
	}
	else if(globalvar.fix==2)
	{
		Fyd12864DispPrintf(1,1,"�����:%ld.%02d",globalvar.fixmoney/100,globalvar.fixmoney%100);
	}
	Fyd12864ClearLine(2);
	Fyd12864DispPrintf(3,1,"����    ��������");
	Fyd12864DispPrintf(4,1,"ȡ��    �����ء�");
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
**�������ƣ�FixNum
**�������ܣ�������������
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void FixNum(void)
{
    char buf[16]={0};
    ScreenInput.inputmaxlen=5;
    NumKeyHandler();
    globalvar.fix=1;
    sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
    globalvar.fixnum=atol(buf);
	Fyd12864DispPrintf(1,1,"    ��������");
    Fyd12864DispPrintf(2,1,"������:%ld.%02d",globalvar.fixnum/100,globalvar.fixnum%100);
	Fyd12864ClearLine(3);
    TaskAdd(KEY_RET, DispFirst, NULL); 
	TaskAdd(KEY_OK, FixAffirm, NULL);
}

/***************************************************************************
**�������ƣ�FixMoney
**�������ܣ���������
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void FixMoney(void)
{
    char buf[16]={0};
    ScreenInput.inputmaxlen=5;
    NumKeyHandler();
    globalvar.fix=2;
    sprintf(buf,"%f",atof((char *)ScreenInput.array)*100);
    globalvar.fixmoney=atol(buf);
	Fyd12864DispPrintf(1,1,"    �������");
    Fyd12864ClearLine(3);
    Fyd12864DispPrintf(2,1,"�����:%ld.%02d",globalvar.fixmoney/100,globalvar.fixmoney%100);
    TaskAdd(KEY_RET, DispFirst, NULL);
	TaskAdd(KEY_OK, FixAffirm, NULL);
}

/***************************************************************************
**�������ƣ�SaveFm25l04
**�������ܣ����籣�����ݺ�����
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void SaveFm25l04(void)
{
    realtime.lcng =sysparas.devicetype;                                                   //��Ʒ���롣
    if(globalvar.fuelstep==3)
    {
        realtime.transnum=sysparas.transnum;                            //��ǰ��ˮ�š�
    }else
    {
        realtime.transnum=sysparas.transnum+1;                          //��ǰ��ˮ�š�
    }
    realtime.cardtype=m1card.cardtype;                              //�����͡�  
    realtime.shiftnum=sysparas.shiftnum;                                //��š�
    memcpy(&realtime.employeenum[0],&shiftrecordinfo.cardstaffnum[0],8);//Ա���š�
    memcpy(&realtime.cardnum[0],&m1card.cardnum[0],8);              //�û����š�
    memcpy(&realtime.carnum[0],&m1card.carnum[0],9);                //���ƺš�
    memcpy(&realtime.starttime[0], &fuelrecordinfo.starttime[0], 7);    //������ʼʱ�䡣
	realtime.endtime[0]=0x20;
    memcpy(&realtime.endtime[1], &time.year, 6);                        //��������ʱ�䡣
    realtime.beforepressure=fuelrecordinfo.beforepressure;              //����ǰѹ����
    realtime.afterpressure=realtime.cur_pressure;                       //������ѹ����
    realtime.beforebalance=fuelrecordinfo.beforebalance;                //����ǰ��
    realtime.afterbalance=fuelrecordinfo.afterbalance;                  //��������
    realtime.allrechargetimes=fuelrecordinfo.allrechargetimes;          //�ۼƳ�ֵ������
    realtime.allrechargemoney=fuelrecordinfo.allrechargemoney;          //�ۼƳ�ֵ��
    realtime.allconsumptiontimes=fuelrecordinfo.allconsumptiontimes;    //�ۼ����Ѵ�����
    realtime.volume=fuelrecordinfo.volume;                              //������
    realtime.money=fuelrecordinfo.money;                                //��
    realtime.allconsumptionnum=fuelrecordinfo.volume+m1card.sumvolme;//�ۼ�����������
    realtime.allconsumptionmoney=m1card.summoney;//�ۼ����ѽ�

    if(m1card.existflag==1)
    {
        if((globalvar.fuelstep==1)||(globalvar.fuelstep==2))         //����ڼ�������1����2��
        {
            realtime.reallymoney=0;                           //ʵ�۽�
            realtime.transactiontype=0xFF;                             //�������͡�
        }
        else if(globalvar.fuelstep==3)                                //����ڼ�������3.
        {
            realtime.reallymoney=fuelrecordinfo.reallymoney;           //ʵ�۽�
            realtime.transactiontype=fuelrecordinfo.transactiontype;   //�������͡�
        }
    }else
    {
        realtime.reallymoney=fuelrecordinfo.money;                      //ʵ�۽�
        realtime.transactiontype=0x0;                                   //�������͡�
    }
    realtime.curnum=sysparas.coeff;                                     //������
    realtime.measuretype=sysparas.unit;                                 //������λ��
    realtime.stopreason=fpvariedpar.stopreason;                         //ͣ��ԭ��
    realtime.jhid=0;                                                    //��������ˮ�š�
    realtime.classid=sysparas.shiftransnum;                             //����ˮ�š�
    
    Fm25l04Write(0,(uint8_t *)(&realtime.lcng),sizeof(RealTimeData));   //�洢���硣

}

/***************************************************************************
**�������ƣ�DisPlayLock
**�������ܣ����ܰ���ʾ������
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void DisPlayLock(void)
{
    Fyd12864ClearLine(1);
    Fyd12864ClearLine(3);
    Fyd12864ClearLine(4);
    if(SI_INDATE==0)//����״̬��
    {
        Fyd12864DispPrintf(2,1,"������״̬");
    }else         
    {
        DispUI=DispFirst;
    }
}


void CirculateFueling(void)
{
	globalvar.fixnum=100+time.day+time.hour+time.minute+time.second;
	globalvar.fix=1;//����
	DispUI=ApplyVerif;
}
