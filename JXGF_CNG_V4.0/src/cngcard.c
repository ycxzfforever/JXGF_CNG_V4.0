#include "kernel.h"

uint8_t SearchBlackList(uint8_t type,uint8_t *cardNo)
{
    uint32_t  addr,value;
    uint8_t   readAarray[8]; 
    int       low,mid,high,result;
    if(type==0x01)     //��ɾ������
    {
        addr=FlashDelBlkSADDR;
        value=sysparas.blackdelnum;
    }
    else if(type==0x02)//����������
    {
        addr=FlashAddBlkSADDR;
        value=sysparas.blackaddnum;
    }
    else if(type==0x03)//����������
    {
        addr=FlashBlkSADDR;
        value=sysparas.blacknum;
    }
    else if(type==0x04)//������
    {
        addr=FlashWhiteSADDR;
        value=sysparas.whitenum;
    }  
    low=0;
    mid=0;
    high=value-1;
    while(low<=high)
    {
        mid=(low+high)/2;
        memset(&readAarray[0],0,sizeof(readAarray));
        ReadFlash(0,addr+mid*8, &readAarray[0], 8);  
        result=memcmp(&cardNo[0],&readAarray[0],8);
        if(result>0)
        {
            low=mid+1;
        }
        else if(result<0)
        {
            high=mid-1;
        }
        else 
        {
            return 1;
        }
        if(high<0)
        {
            return 0;
        }
    }
    return 0;
}



/***************************************************************************
**�������ƣ�HuiIccard
**�������ܣ�����˿����ã��򽫿��ûҡ�
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void HuiIccard(void)
{
	uint8_t tmpbuffer[16];
    m1card.cardstate=2;//�ûҡ�
    m1card.gunnum=sysparas.gunnum;
    m1card.transnum=sysparas.transnum+1;
    memcpy(&m1card.laststationcode[0],&sysparas.stationcode[0],4);
	memcpy(&tmpbuffer[0],&m1card.cardstate,16);
    StrReverse(&tmpbuffer[2],1,4);;
    
    if(Mt318WriteM1Card(KEYB,&m1card.keyB[0],10,&tmpbuffer[0]))//д����
    {
        DispUI=FuelWork;
    }
    else
    {
        Fyd12864DispPrintf(2,1,"�û�ʧ�ܣ�");
        TaskMessage(10, DispFirst);// 1s�󷵻�������
    }
}

/***************************************************************************
**�������ƣ�ApplyVerif
**�������ܣ����̨��ѯ�˿��Ƿ���á�
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void ApplyVerif(void)
{
     if(m1card.cardtype == 2)//���˿���
     {
         if(m1card.cardbalance < sysparas.cardmaxyue)//�жϼ��˿�����Ƿ�������ޡ�
         {
             //m1card.checkflag = 1;
             DispUI=FuelReady;
             globalvar.displayonce = 1;
         }else
         {
             Fyd12864ClearLine(1); 
             Fyd12864DispPrintf(2,1,"���ޣ��븶��");
			 Fyd12864ClearLine(3);
			 Fyd12864ClearLine(4);
             TaskMessage(20, DispFirst);
         }
     }
     else if(m1card.cardtype == 1)//�����
     {
         if((m1card.cardbalance > sysparas.cardminyue) && (m1card.cardbalance <= sysparas.cardmaxyue))//�ж�����Ƿ���������ҵ������ޡ�
         {
             DispUI=FuelReady;
             globalvar.displayonce=1;
         }
		 else if(m1card.cardbalance<=sysparas.cardminyue)
         {
             Fyd12864ClearLine(1); 
             Fyd12864DispPrintf(2,1,"���㣬���ֵ");
			 Fyd12864ClearLine(3);
			 Fyd12864ClearLine(4);
             TaskMessage(20, DispFirst);
         }
		 else
		 {
			 Fyd12864ClearLine(1); 
             Fyd12864DispPrintf(2,1,"�����������");
			 Fyd12864ClearLine(3);
			 Fyd12864ClearLine(4);
             TaskMessage(20, DispFirst);
		 }
			 
     }
}
/***************************************************************************
**�������ƣ�IcCardLock
**�������ܣ������ɹ����жϿ������Ƿ�Ϊ0����Ϊ0����Ҫ�������롣
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void IcCardLock(void)
{
    uint8_t str[6];
    str[0]=((m1card.password[0]>>4)+0x30);   //��ѹ��BCD��ת��Ϊ�ַ���
    str[1]=((m1card.password[0]&0x0f)+0x30);
    str[2]=((m1card.password[1]>>4)+0x30);
    str[3]=((m1card.password[1]&0x0f)+0x30);
    str[4]=((m1card.password[2]>>4)+0x30);
    str[5]=((m1card.password[2]&0x0f)+0x30);
    m1card.key=atol((char *)str);         //���ַ�ת��Ϊ����
	if(m1card.key==0)                 //������Ϊ0��ֱ��������
	{
		DispUI=IcGas;
	}
	else                                    //�����벻Ϊ0�����������롣
	{
		DispUI=InputIccardLock;
	}
}

/***************************************************************************
**�������ƣ�InputIccardLock
**�������ܣ����뿨���롣
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void InputIccardLock(void)
{
     DispInputPwd("�����뿨����",CompareLock);
	 /*
     if(Mt318GetCardSerialNum()!=true)
     {
         DispUI=DispFirst;
     }
	*/
	if(m1card.escapeflag==0xaa)
	{
		DispUI=DispFirst;
	}
}

/***************************************************************************
**�������ƣ�CompareLock
**�������ܣ��жϿ���������
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void CompareLock(void)
{
    if(globalvar.U32Temp!=m1card.key)
    {
        Fyd12864DispPrintf(4,1,"�������");
        TaskAdd(KEY_CLR,InputIccardLock ,NULL);
        TaskAdd(KEY_RET,InputIccardLock ,NULL);
    }
    else
    {
        DispUI=IcGas;
    }
}

void  CardMesssageDisplay(void)
{
	uint8_t count1,count2;
	uint8_t tmparray[13];
	if(globalvar.displayonce==0x01)
	{
		globalvar.displayonce=0;
		Fyd12864DispPrintf(1,1,"���: %ld.%02d",m1card.cardbalance/100,m1card.cardbalance%100);
		Fyd12864DispPrintf(2,1,"����: %02x%02x%02x%02x",m1card.cardnum[4],m1card.cardnum[5],m1card.cardnum[6],m1card.cardnum[7]); 
		for(count1=0;count1<13;count1++)
		{
			if(m1card.customername[count1]!=0)
			{
				break;
			}
		}
		for(count2=0;count1<13;count2++,count1++)
		{
			tmparray[count2]=m1card.customername[count1];
		}
		tmparray[count2]='\0';
		Fyd12864DispPrintf(3,1,"����: %s",&tmparray[0]);
		for(count1=0;count1<9;count1++)
		{
			if(m1card.carnum[count1]!=0)
			{
				break;
			}
		}
		for(count2=0;count1<13;count2++,count1++)
		{
			tmparray[count2]=m1card.carnum[count1];
		}
		Fyd12864DispPrintf(4,1,"����: %s",&tmparray[0]);
	}
	if((syserr==0)&&(sysparas.yesnolock==0))
	{
		if(sysparas.offonwork==0x01)//�ϰ�
		{
			TaskAdd(KEY_START,ApplyVerif,NULL);
			TaskAdd(KEY_FIX,FixGas,NULL);                     //��������
			if(m1card.shiftflag==0xaa)
			{
				TaskAdd(KEY_SET, DispInputMenuPwd,NULL);          //���á�
				TaskAdd(KEY_COMB_8KEY,OffWork, NULL);             //ֹͣ+8�°� 
			}
		}
		else
		{
			if(m1card.shiftflag==0xaa)
			{
				TaskAdd(KEY_SET, DispInputMenuPwd,NULL);          //���á�
				TaskAdd(KEY_COMB_7KEY,OnWork,NULL);               //ֹͣ+7�ϰ�
			}
		}
	}
	if(m1card.escapeflag==0xaa)
	{
		DispUI=DispFirst;
	}
	/*
	if(Mt318GetCardSerialNum()!=true)
    {
        DispUI=DispFirst;
    }
	*/
}
/***************************************************************************
**�������ƣ�IcGas
**�������ܣ������ɹ����жϿ�����;��
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void IcGas(void)
{
	Fyd12864ClearLine(3);
    if(m1card.cardstate == 2)
	{
        m1card.graycard=1;
		Fyd12864DispPrintf(2,1,"��������...");
	}
	else
	{
		DispUI=CardValidQuery;
	}
	/*
	if(Mt318GetCardSerialNum()!=true)
    {
        DispUI=DispFirst;
    }
	*/
	if(m1card.escapeflag==0xaa)
	{
		DispUI=DispFirst;
	}
}

void GetCardMessageFail(void)
/***************************************************************************
**�������ƣ� GetCardMessageFail
**�������ܣ�����ʧ��
**��ڲ�������
**����ֵ���� 
**�����ߣ��ƺ���
**�������ڣ�2016-10-26 
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
{
	Fyd12864ClearLine(1);
	Fyd12864ClearLine(4);
	Fyd12864DispPrintf(2,1,"    ����ʧ��");
	Fyd12864DispPrintf(3,1,"    ��ο�..");
	TaskAdd(KEY_RET, DispFirst, NULL);
	/*
	if(Mt318GetCardSerialNum()!=true)
	{
		 DispUI=DispFirst;
	}
	*/
	if(m1card.escapeflag==0xaa)
	{
		DispUI=DispFirst;
	}
}

void  CardValidQueryOverTime(void)
{
	Fyd12864ClearLine(1);
	Fyd12864ClearLine(3);
	Fyd12864DispPrintf(2,1,"  �ڰ�������ѯ");
	Fyd12864DispPrintf(4,1,"    ��ѯ��ʱ");
	TaskAdd(KEY_RET, DispFirst, NULL);
	if(m1card.escapeflag==0xaa)
	{
		DispUI=DispFirst;
	}
	/*
	if(Mt318GetCardSerialNum()!=true)
	{
		 DispUI=DispFirst;
	}
	*/
}

void  CardInvalidDisplay(void)
{
	Fyd12864ClearLine(1);
	Fyd12864ClearLine(3);
	Fyd12864ClearLine(4);
	
	switch(m1card.checkresult)
	{
		case 0x01:
			Fyd12864DispPrintf(2,1,"���󿨺Ų�����");
			break;
		case 0x02:
			Fyd12864DispPrintf(2,1,"  ��  ��  ��");
			break;
		case 0x03:
			Fyd12864DispPrintf(2,1,"    ����쳣");
			break;
		case 0x04:
			Fyd12864DispPrintf(2,1,"    ����쳣");
			break;
		default:
			break;
	}
	TaskAdd(KEY_RET, DispFirst, NULL);
	/*
	if(Mt318GetCardSerialNum()!=true)
	{
		 DispUI=DispFirst;
	}	
	*/
	if(m1card.escapeflag==0xaa)
	{
		DispUI=DispFirst;
	}
}

void  CardValidQuery(void)
/***************************************************************************
**�������ƣ� CardValidQuery
**�������ܣ���ѯ������Ч�ԣ��ڰ�������
**��ڲ�������
**����ֵ���� 
**�����ߣ��ƺ���
**�������ڣ�2016-10-26 
**
**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
{
	uint8_t result;
	Fyd12864ClearLine(2);
	Fyd12864ClearLine(4);
	Fyd12864DispPrintf(1,1,"  ����Ч�Բ�ѯ");
	Fyd12864DispPrintf(3,1,"    ���Ե�..");
	m1card.checkflag=0x01;	
	if(m1card.checkresultflag==0x01)
	{
		if(m1card.checkresult==0)
		{
			if(m1card.Shiftfunction==0x01)
			{	
				result=0;
				result=SearchBlackList(0x04,&m1card.cardnum[0]);
				if(result==0x01)
				{
					m1card.shiftflag=0xaa;
				}
				globalvar.workstate=StaIdle;
				DispUI=CardMesssageDisplay; 
			}
			else
			{
				globalvar.workstate=StaIdle;
				DispUI=CardMesssageDisplay;  
			}
		}
		else
		{
			globalvar.workstate=StaIdle;
			DispUI=CardInvalidDisplay;
		}
	}
	TaskAdd(KEY_RET, DispFirst, NULL);
	/*
	if(Mt318GetCardSerialNum()!=true)
	{
		 DispUI=DispFirst;
	}	
	*/
	if(m1card.escapeflag==0xaa)
	{
		DispUI=DispFirst;
	}
}

/***************************************************************************
**�������ƣ�GetCardMessage
**�������ܣ������Կ�ȶԳɹ����ͻ�õ����ڲ���Ϣ��
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-7-25
**
**�޸��ߣ��ƺ���
**�޸����ڣ�20161026
**�޸����ݣ�1���޸ĺ�������
           2���޸ĺ�������
***************************************************************************/
void GetCardMessage(void)
{
    uint8_t tmp=0;                                             //������������
    uint8_t keystr[16];
	uint8_t tmpbuffer[16];
	uint8_t desresult[8];
	Fyd12864ClearLine(1);
	Fyd12864ClearLine(4);
	Fyd12864DispPrintf(2,1,"    ������..");
	Fyd12864DispPrintf(3,1,"    ���Ե�..");
	
    globalvar.KeyValue=0;                                      //���κΰ���������Ч��
    memset(&m1card,0,sizeof(M1carddata));                  //�忨���ݴ洢�ṹ�塣
    BufReverse2(&mt318reader.recbuf[6],4);                     //��ȡ��Կ��
    memcpy(&tmpbuffer[0], &mt318reader.recbuf[6],4);
    memcpy(&tmpbuffer[4], &mt318reader.recbuf[6],4);     //�õ��������кš�
    memcpy(&m1card.serialnumber[0],&mt318reader.recbuf[6],4);
	m1card.existflag=1;
	
    At24C512ReadNByte(LOCKVER_ADDR, keystr, 16);
    ThreeDES(&keystr[0],&tmpbuffer[0],&desresult[0],1);      //3DES��������õ�A�ܺ�B�ܡ� 
    memcpy(&m1card.keyA[0], &desresult[0],6);                            //A�ܡ�
    memcpy(&m1card.keyB[0], &desresult[2],6);                            //B�ܡ�       
    
    
    if(!Mt318ReadM1Card(KEYB,&m1card.keyB[0],1, &m1card.cardnum[0]))    //����1��
    {
       tmp++;
    }
    
    if(tmp==0)
	{
		if(!Mt318ReadM1Card(KEYB,&m1card.keyB[0],2, &m1card.stationcode[0]))    //����2��
		{
			tmp++;
		}
	}
    
	if(tmp==0)
	{
		if(!Mt318ReadM1Card(KEYB,&m1card.keyB[0],4, &m1card.password[0]))   //����4��
		{
			tmp++;
		}
	}
    
	if(tmp==0)
	{
		if(Mt318ReadM1Card(KEYB,&m1card.keyB[0],5,(uint8_t *)&m1card.balancecrc))   //����5��
		{
			BufReverse2((uint8_t *)&m1card.balancecrc+8,4);
			BufReverse2((uint8_t *)&m1card.balancecrc+12,4);
			//memcpy(&m1card.balancecrc, &tmpbuffer[0],16);
		}else{tmp++;}
	}
    
	if(tmp==0)
	{
		if(Mt318ReadM1Card(KEYB,&m1card.keyB[0],8,(uint8_t *)&m1card.sumtimes))    //����8��
		{	
			StrReverse((uint8_t *)&m1card.sumtimes,3,4);
			//StrReverse(&tmpbuffer[0],3,4);
			//memcpy(&m1card.sumtimes,&tmpbuffer[0],16);
		}else{tmp++;}
	}
    
	if(tmp==0)
	{
		if(Mt318ReadM1Card(KEYB,&m1card.keyB[0],9,(uint8_t *)&m1card.sumchargetimes))    //����9��
		{
			StrReverse((uint8_t *)&m1card.sumchargetimes,2,4);
			//StrReverse(&tmpbuffer[0],2,4);
			//memcpy((uint8_t *)&m1card.sumchargetimes, &tmpbuffer[0],16);
		}else{tmp++;}
	}
	if(tmp==0)
	{
		if(Mt318ReadM1Card(KEYB,&m1card.keyB[0],10,&m1card.cardstate))  //����10��
		{
			StrReverse(&m1card.cardstate+2,1,4);
			//StrReverse(&tmpbuffer[2],1,4);
			//memcpy(&m1card.cardstate, &tmpbuffer[0],16);
		}else{tmp++;}
	}
	
	if(tmp==0)
	{
		if(!Mt318ReadM1Card(KEYB,&m1card.keyB[0],12,&m1card.cylindersdate[0]))  //����12��
		{
			tmp++;
		}
	}
    
    if(tmp==0)                                                 //�����ɹ���
    {
        DispUI=IcCardLock;       
        globalvar.workstate=StaIdle;
    }
    else                                                       //����ʧ�ܡ�
    {
        DispUI=GetCardMessageFail;
    }
}

