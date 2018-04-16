#include "kernel.h"

uint8_t SearchBlackList(uint8_t type,uint8_t *cardNo)
{
    uint32_t  addr,value;
    uint8_t   readAarray[8]; 
    int       low,mid,high,result;
    if(type==0x01)     //新删黑名单
    {
        addr=FlashDelBlkSADDR;
        value=sysparas.blackdelnum;
    }
    else if(type==0x02)//新增黑名单
    {
        addr=FlashAddBlkSADDR;
        value=sysparas.blackaddnum;
    }
    else if(type==0x03)//基础黑名单
    {
        addr=FlashBlkSADDR;
        value=sysparas.blacknum;
    }
    else if(type==0x04)//白名单
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
**函数名称：HuiIccard
**函数功能：如果此卡可用，则将卡置灰。
**入口参数：无
**返 回 值：无
**创 建 者：杨传坤
**创建日期：2016-7-25
**
**修改者：
**修改日期：
**修改内容：
***************************************************************************/
void HuiIccard(void)
{
	uint8_t tmpbuffer[16];
    m1card.cardstate=2;//置灰。
    m1card.gunnum=sysparas.gunnum;
    m1card.transnum=sysparas.transnum+1;
    memcpy(&m1card.laststationcode[0],&sysparas.stationcode[0],4);
	memcpy(&tmpbuffer[0],&m1card.cardstate,16);
    StrReverse(&tmpbuffer[2],1,4);;
    
    if(Mt318WriteM1Card(KEYB,&m1card.keyB[0],10,&tmpbuffer[0]))//写卡。
    {
        DispUI=FuelWork;
    }
    else
    {
        Fyd12864DispPrintf(2,1,"置灰失败！");
        TaskMessage(10, DispFirst);// 1s后返回主界面
    }
}

/***************************************************************************
**函数名称：ApplyVerif
**函数功能：向后台查询此卡是否可用。
**入口参数：无
**返 回 值：无
**创 建 者：杨传坤
**创建日期：2016-7-25
**
**修改者：
**修改日期：
**修改内容：
***************************************************************************/
void ApplyVerif(void)
{
     if(m1card.cardtype == 2)//记账卡。
     {
         if(m1card.cardbalance < sysparas.cardmaxyue)//判断记账卡余额是否高于上限。
         {
             //m1card.checkflag = 1;
             DispUI=FuelReady;
             globalvar.displayonce = 1;
         }else
         {
             Fyd12864ClearLine(1); 
             Fyd12864DispPrintf(2,1,"余额超限，请付账");
			 Fyd12864ClearLine(3);
			 Fyd12864ClearLine(4);
             TaskMessage(20, DispFirst);
         }
     }
     else if(m1card.cardtype == 1)//储蓄卡。
     {
         if((m1card.cardbalance > sysparas.cardminyue) && (m1card.cardbalance <= sysparas.cardmaxyue))//判断余额是否高于下限且低于上限。
         {
             DispUI=FuelReady;
             globalvar.displayonce=1;
         }
		 else if(m1card.cardbalance<=sysparas.cardminyue)
         {
             Fyd12864ClearLine(1); 
             Fyd12864DispPrintf(2,1,"余额不足，请充值");
			 Fyd12864ClearLine(3);
			 Fyd12864ClearLine(4);
             TaskMessage(20, DispFirst);
         }
		 else
		 {
			 Fyd12864ClearLine(1); 
             Fyd12864DispPrintf(2,1,"余额超过最大限制");
			 Fyd12864ClearLine(3);
			 Fyd12864ClearLine(4);
             TaskMessage(20, DispFirst);
		 }
			 
     }
}
/***************************************************************************
**函数名称：IcCardLock
**函数功能：读卡成功后，判断卡密码是否为0，不为0则需要输入密码。
**入口参数：无
**返 回 值：无
**创 建 者：杨传坤
**创建日期：2016-7-25
**
**修改者：
**修改日期：
**修改内容：
***************************************************************************/
void IcCardLock(void)
{
    uint8_t str[6];
    str[0]=((m1card.password[0]>>4)+0x30);   //将压缩BCD码转化为字符。
    str[1]=((m1card.password[0]&0x0f)+0x30);
    str[2]=((m1card.password[1]>>4)+0x30);
    str[3]=((m1card.password[1]&0x0f)+0x30);
    str[4]=((m1card.password[2]>>4)+0x30);
    str[5]=((m1card.password[2]&0x0f)+0x30);
    m1card.key=atol((char *)str);         //将字符转化为数字
	if(m1card.key==0)                 //卡密码为0，直接跳过。
	{
		DispUI=IcGas;
	}
	else                                    //卡密码不为0，则输入密码。
	{
		DispUI=InputIccardLock;
	}
}

/***************************************************************************
**函数名称：InputIccardLock
**函数功能：输入卡密码。
**入口参数：无
**返 回 值：无
**创 建 者：杨传坤
**创建日期：2016-7-25
**
**修改者：
**修改日期：
**修改内容：
***************************************************************************/
void InputIccardLock(void)
{
     DispInputPwd("请输入卡密码",CompareLock);
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
**函数名称：CompareLock
**函数功能：判断卡密码正误。
**入口参数：无
**返 回 值：无
**创 建 者：杨传坤
**创建日期：2016-7-25
**
**修改者：
**修改日期：
**修改内容：
***************************************************************************/
void CompareLock(void)
{
    if(globalvar.U32Temp!=m1card.key)
    {
        Fyd12864DispPrintf(4,1,"密码错误");
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
		Fyd12864DispPrintf(1,1,"余额: %ld.%02d",m1card.cardbalance/100,m1card.cardbalance%100);
		Fyd12864DispPrintf(2,1,"卡号: %02x%02x%02x%02x",m1card.cardnum[4],m1card.cardnum[5],m1card.cardnum[6],m1card.cardnum[7]); 
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
		Fyd12864DispPrintf(3,1,"姓名: %s",&tmparray[0]);
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
		Fyd12864DispPrintf(4,1,"车牌: %s",&tmparray[0]);
	}
	if((syserr==0)&&(sysparas.yesnolock==0))
	{
		if(sysparas.offonwork==0x01)//上班
		{
			TaskAdd(KEY_START,ApplyVerif,NULL);
			TaskAdd(KEY_FIX,FixGas,NULL);                     //定量加气
			if(m1card.shiftflag==0xaa)
			{
				TaskAdd(KEY_SET, DispInputMenuPwd,NULL);          //设置。
				TaskAdd(KEY_COMB_8KEY,OffWork, NULL);             //停止+8下班 
			}
		}
		else
		{
			if(m1card.shiftflag==0xaa)
			{
				TaskAdd(KEY_SET, DispInputMenuPwd,NULL);          //设置。
				TaskAdd(KEY_COMB_7KEY,OnWork,NULL);               //停止+7上班
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
**函数名称：IcGas
**函数功能：读卡成功后，判断卡得用途。
**入口参数：无
**返 回 值：无
**创 建 者：杨传坤
**创建日期：2016-7-25
**
**修改者：
**修改日期：
**修改内容：
***************************************************************************/
void IcGas(void)
{
	Fyd12864ClearLine(3);
    if(m1card.cardstate == 2)
	{
        m1card.graycard=1;
		Fyd12864DispPrintf(2,1,"申请解灰中...");
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
**函数名称： GetCardMessageFail
**函数功能：读卡失败
**入口参数：无
**返回值：无 
**创建者：黄豪民
**创建日期：2016-10-26 
**
**修改者：
**修改日期：
**修改内容：
***************************************************************************/
{
	Fyd12864ClearLine(1);
	Fyd12864ClearLine(4);
	Fyd12864DispPrintf(2,1,"    读卡失败");
	Fyd12864DispPrintf(3,1,"    请拔卡..");
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
	Fyd12864DispPrintf(2,1,"  黑白名单查询");
	Fyd12864DispPrintf(4,1,"    查询超时");
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
			Fyd12864DispPrintf(2,1,"请求卡号不存在");
			break;
		case 0x02:
			Fyd12864DispPrintf(2,1,"  黑  名  单");
			break;
		case 0x03:
			Fyd12864DispPrintf(2,1,"    余额异常");
			break;
		case 0x04:
			Fyd12864DispPrintf(2,1,"    余额异常");
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
**函数名称： CardValidQuery
**函数功能：查询卡的有效性（黑白名单）
**入口参数：无
**返回值：无 
**创建者：黄豪民
**创建日期：2016-10-26 
**
**修改者：
**修改日期：
**修改内容：
***************************************************************************/
{
	uint8_t result;
	Fyd12864ClearLine(2);
	Fyd12864ClearLine(4);
	Fyd12864DispPrintf(1,1,"  卡有效性查询");
	Fyd12864DispPrintf(3,1,"    请稍等..");
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
**函数名称：GetCardMessage
**函数功能：如果密钥比对成功，就会得到卡内部信息。
**入口参数：无
**返 回 值：无
**创 建 者：杨传坤
**创建日期：2016-7-25
**
**修改者：黄豪民
**修改日期：20161026
**修改内容：1、修改函数名称
           2、修改函数内容
***************************************************************************/
void GetCardMessage(void)
{
    uint8_t tmp=0;                                             //读块出错次数。
    uint8_t keystr[16];
	uint8_t tmpbuffer[16];
	uint8_t desresult[8];
	Fyd12864ClearLine(1);
	Fyd12864ClearLine(4);
	Fyd12864DispPrintf(2,1,"    读卡中..");
	Fyd12864DispPrintf(3,1,"    请稍等..");
	
    globalvar.KeyValue=0;                                      //按任何按键，皆无效。
    memset(&m1card,0,sizeof(M1carddata));                  //清卡内容存储结构体。
    BufReverse2(&mt318reader.recbuf[6],4);                     //读取密钥。
    memcpy(&tmpbuffer[0], &mt318reader.recbuf[6],4);
    memcpy(&tmpbuffer[4], &mt318reader.recbuf[6],4);     //得到连续序列号。
    memcpy(&m1card.serialnumber[0],&mt318reader.recbuf[6],4);
	m1card.existflag=1;
	
    At24C512ReadNByte(LOCKVER_ADDR, keystr, 16);
    ThreeDES(&keystr[0],&tmpbuffer[0],&desresult[0],1);      //3DES加密运算得到A密和B密。 
    memcpy(&m1card.keyA[0], &desresult[0],6);                            //A密。
    memcpy(&m1card.keyB[0], &desresult[2],6);                            //B密。       
    
    
    if(!Mt318ReadM1Card(KEYB,&m1card.keyB[0],1, &m1card.cardnum[0]))    //读块1。
    {
       tmp++;
    }
    
    if(tmp==0)
	{
		if(!Mt318ReadM1Card(KEYB,&m1card.keyB[0],2, &m1card.stationcode[0]))    //读块2。
		{
			tmp++;
		}
	}
    
	if(tmp==0)
	{
		if(!Mt318ReadM1Card(KEYB,&m1card.keyB[0],4, &m1card.password[0]))   //读块4。
		{
			tmp++;
		}
	}
    
	if(tmp==0)
	{
		if(Mt318ReadM1Card(KEYB,&m1card.keyB[0],5,(uint8_t *)&m1card.balancecrc))   //读块5。
		{
			BufReverse2((uint8_t *)&m1card.balancecrc+8,4);
			BufReverse2((uint8_t *)&m1card.balancecrc+12,4);
			//memcpy(&m1card.balancecrc, &tmpbuffer[0],16);
		}else{tmp++;}
	}
    
	if(tmp==0)
	{
		if(Mt318ReadM1Card(KEYB,&m1card.keyB[0],8,(uint8_t *)&m1card.sumtimes))    //读块8。
		{	
			StrReverse((uint8_t *)&m1card.sumtimes,3,4);
			//StrReverse(&tmpbuffer[0],3,4);
			//memcpy(&m1card.sumtimes,&tmpbuffer[0],16);
		}else{tmp++;}
	}
    
	if(tmp==0)
	{
		if(Mt318ReadM1Card(KEYB,&m1card.keyB[0],9,(uint8_t *)&m1card.sumchargetimes))    //读块9。
		{
			StrReverse((uint8_t *)&m1card.sumchargetimes,2,4);
			//StrReverse(&tmpbuffer[0],2,4);
			//memcpy((uint8_t *)&m1card.sumchargetimes, &tmpbuffer[0],16);
		}else{tmp++;}
	}
	if(tmp==0)
	{
		if(Mt318ReadM1Card(KEYB,&m1card.keyB[0],10,&m1card.cardstate))  //读块10。
		{
			StrReverse(&m1card.cardstate+2,1,4);
			//StrReverse(&tmpbuffer[2],1,4);
			//memcpy(&m1card.cardstate, &tmpbuffer[0],16);
		}else{tmp++;}
	}
	
	if(tmp==0)
	{
		if(!Mt318ReadM1Card(KEYB,&m1card.keyB[0],12,&m1card.cylindersdate[0]))  //读块12。
		{
			tmp++;
		}
	}
    
    if(tmp==0)                                                 //读卡成功。
    {
        DispUI=IcCardLock;       
        globalvar.workstate=StaIdle;
    }
    else                                                       //读卡失败。
    {
        DispUI=GetCardMessageFail;
    }
}

