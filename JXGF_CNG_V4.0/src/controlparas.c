/***************************************************************************
**文本：DispControl.c
**说明：用于CNG或者LNG中的"控制参数"菜单显示
**创建者 ：杨朝旭
**创建日期：2016-4-21 17:17:20

**修改者：
**修改日期：
**修改内容：
***************************************************************************/

#include "kernel.h"
//控制参数显示CNG和LNG需要分开编写此内容



//恢复出厂设置时，各个参数重置
void FactoryReset(void)
{
    uint8_t i;
    tmp_sysparas.head=0xFF;            //头
    tmp_sysparas.writeE2num=0;         //写eeprom次数
    
    tmp_sysparas.gunnum=1;             //枪号
    tmp_sysparas.transnum=0;           //交易流水号     
    tmp_sysparas.uptransnum=0;         //已传交易流水号     
    
    tmp_sysparas.totvolume=0;          //枪总气量    2位小数
    tmp_sysparas.totmoney=0;           //枪总金额    2位小数
	
	sysparas.price[0] = 400;

    for(i=0;i<21;i++) tmp_sysparas.price[i]=400;      //21个单价，单价0为默认单价，根据单价等级选择相应单价
    
    //班组信息
    tmp_sysparas.shiftnum=1;           //班号
    tmp_sysparas.shiftmaxnum=3;        //最大班号
    tmp_sysparas.shiftransnum=0;       //班流水号     
    tmp_sysparas.shiftuptransnum=0;    //已传班流水号     
    tmp_sysparas.shiftfueltimes=0;     //班加气次数
    tmp_sysparas.shiftstartvol=0;      //班开始枪累    2位小数
    tmp_sysparas.shiftstartmon=0;      //班开始金额    2位小数
    tmp_sysparas.shiftotvol=0;         //班累计气量    2位小数
    tmp_sysparas.shiftotmon=0;         //班累计金额    2位小数

    tmp_sysparas.shiftstarttime[0]=time.year;    //班开始时间    BCD
    tmp_sysparas.shiftstarttime[1]=time.month;
    tmp_sysparas.shiftstarttime[2]=time.day;
    tmp_sysparas.shiftstarttime[3]=time.hour;    
    tmp_sysparas.shiftstarttime[4]=time.minute;
    tmp_sysparas.shiftstarttime[5]=time.second;

    for(i=0;i<6;i++) tmp_sysparas.shiftendtime[i]=0;    //班结束时间    BCD
    
    //通用信息
    for(i=0;i<2;i++) tmp_sysparas.shengcode[i]=0;       //省编码
    for(i=0;i<4;i++) tmp_sysparas.stationcode[i]=0;     //站编码
    for(i=0;i<2;i++) tmp_sysparas.citycode[i]=0;        //地市代码
    for(i=0;i<4;i++) tmp_sysparas.parentcode[i]=0;      //上级单位代码

    //卡信息
    tmp_sysparas.cardmaxyue=100000000;   //卡余额上限    2位小数 默认100万
    tmp_sysparas.cardminyue=100;         //卡余额下限    2位小数 
    tmp_sysparas.cardpsamnum=1;          //psam卡卡座选择
    tmp_sysparas.cardmaxday=90;          //卡未使用最大间隔天数
    tmp_sysparas.cardcheck=0;            //是否检验钢瓶

    //波特率设置
    tmp_sysparas.modbusbaud=19200;       //流量计波特率
    tmp_sysparas.pcbaud=38400;           //后台波特率

    tmp_sysparas.fuelmode=0;             //加气模式
    tmp_sysparas.printmode=0;            //小票打印方式

    //版本编号设置
    tmp_sysparas.pricever=0;             //单价版本号
    tmp_sysparas.blackver=0;             //黑名单版本号
    tmp_sysparas.whitever=0;             //白名单版本号
    tmp_sysparas.addbkver=0;             //新增黑名单版本号
    tmp_sysparas.delbkver=0;             //新删黑名单版本号
    tmp_sysparas.comminfover=0;          //通用信息版本号

    //密码设置
    tmp_sysparas.menupwd=0;         //菜单进入密码
    tmp_sysparas.qcpwd=222222;           //质检密码
    tmp_sysparas.testpwd=234567;         //测试密码，如按键、电磁阀等测试
    tmp_sysparas.resetpwd=234567;        //恢复出厂密码
	tmp_sysparas.onoffworkpwd=0;	 //上下班密码
    for(i=0;i<6;i++) tmp_sysparas.pwdboard[i]=0;        //加密板数据
    tmp_sysparas.usepwdboard=0;          //0-不使用加密板，1-使用加密板
    
    tmp_sysparas.modbustype=1;           //流量计类型
    tmp_sysparas.unit=1;                 //计量单位
    tmp_sysparas.qcmode=0;               //质检模式设置

    tmp_sysparas.densty=6804;            //密度              4位小数
    tmp_sysparas.coeff=10000;            //修正系数  4位小数
    tmp_sysparas.pzero=820;              //压力零点  
    tmp_sysparas.pcoeff =105;            //压力系数  2位小数       
    
    tmp_sysparas.modnum   = 0;              //修改记录流水号     
    tmp_sysparas.upmodnum = 0;            //已传修改记录流水号  
	tmp_sysparas.offlinemode = 0;			//在线模式	
	//CNG控制参数
	tmp_sysparas.flow20_5 = 150;			//各区间流速（扩大100倍）
	tmp_sysparas.flow21   = 170;
	tmp_sysparas.flow21_5 = 190;
	tmp_sysparas.flow22   = 220;
	tmp_sysparas.flow22_5 = 290;
	tmp_sysparas.flow23   = 320;
	tmp_sysparas.flow23_5 = 350;
	tmp_sysparas.flow24   = 400;
	tmp_sysparas.flow24_5 = 420;
	tmp_sysparas.lowtomidflow = 350;		//低压转中压流速
	tmp_sysparas.midtohighflow = 320;		//中压转高压流速
	tmp_sysparas.lowstopflow = 100;			//低流速停机
	tmp_sysparas.maxsafeflow = 2500;		//加气机高流速停机
	tmp_sysparas.devicetype = 1;			//设备类型
	tmp_sysparas.maxvolumeonce = 999999;	//单次最大气量
	tmp_sysparas.maxmoneyonce = 999999;	//单次最大金额
	tmp_sysparas.stoppressure = 2000;		//停机压力（扩大100倍）
	tmp_sysparas.filtertime = 1;			//防过冲时间0.1秒
	tmp_sysparas.valveswitchdelaytime = 10;	//阀切换延时时间1秒	  
	tmp_sysparas.xqzstartflowrate = 50;
}

