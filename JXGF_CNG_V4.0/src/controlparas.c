/***************************************************************************
**�ı���DispControl.c
**˵��������CNG����LNG�е�"���Ʋ���"�˵���ʾ
**������ �����
**�������ڣ�2016-4-21 17:17:20

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/

#include "kernel.h"
//���Ʋ�����ʾCNG��LNG��Ҫ�ֿ���д������



//�ָ���������ʱ��������������
void FactoryReset(void)
{
    uint8_t i;
    tmp_sysparas.head=0xFF;            //ͷ
    tmp_sysparas.writeE2num=0;         //дeeprom����
    
    tmp_sysparas.gunnum=1;             //ǹ��
    tmp_sysparas.transnum=0;           //������ˮ��     
    tmp_sysparas.uptransnum=0;         //�Ѵ�������ˮ��     
    
    tmp_sysparas.totvolume=0;          //ǹ������    2λС��
    tmp_sysparas.totmoney=0;           //ǹ�ܽ��    2λС��
	
	sysparas.price[0] = 400;

    for(i=0;i<21;i++) tmp_sysparas.price[i]=400;      //21�����ۣ�����0ΪĬ�ϵ��ۣ����ݵ��۵ȼ�ѡ����Ӧ����
    
    //������Ϣ
    tmp_sysparas.shiftnum=1;           //���
    tmp_sysparas.shiftmaxnum=3;        //�����
    tmp_sysparas.shiftransnum=0;       //����ˮ��     
    tmp_sysparas.shiftuptransnum=0;    //�Ѵ�����ˮ��     
    tmp_sysparas.shiftfueltimes=0;     //���������
    tmp_sysparas.shiftstartvol=0;      //�࿪ʼǹ��    2λС��
    tmp_sysparas.shiftstartmon=0;      //�࿪ʼ���    2λС��
    tmp_sysparas.shiftotvol=0;         //���ۼ�����    2λС��
    tmp_sysparas.shiftotmon=0;         //���ۼƽ��    2λС��

    tmp_sysparas.shiftstarttime[0]=time.year;    //�࿪ʼʱ��    BCD
    tmp_sysparas.shiftstarttime[1]=time.month;
    tmp_sysparas.shiftstarttime[2]=time.day;
    tmp_sysparas.shiftstarttime[3]=time.hour;    
    tmp_sysparas.shiftstarttime[4]=time.minute;
    tmp_sysparas.shiftstarttime[5]=time.second;

    for(i=0;i<6;i++) tmp_sysparas.shiftendtime[i]=0;    //�����ʱ��    BCD
    
    //ͨ����Ϣ
    for(i=0;i<2;i++) tmp_sysparas.shengcode[i]=0;       //ʡ����
    for(i=0;i<4;i++) tmp_sysparas.stationcode[i]=0;     //վ����
    for(i=0;i<2;i++) tmp_sysparas.citycode[i]=0;        //���д���
    for(i=0;i<4;i++) tmp_sysparas.parentcode[i]=0;      //�ϼ���λ����

    //����Ϣ
    tmp_sysparas.cardmaxyue=100000000;   //���������    2λС�� Ĭ��100��
    tmp_sysparas.cardminyue=100;         //���������    2λС�� 
    tmp_sysparas.cardpsamnum=1;          //psam������ѡ��
    tmp_sysparas.cardmaxday=90;          //��δʹ�����������
    tmp_sysparas.cardcheck=0;            //�Ƿ�����ƿ

    //����������
    tmp_sysparas.modbusbaud=19200;       //�����Ʋ�����
    tmp_sysparas.pcbaud=38400;           //��̨������

    tmp_sysparas.fuelmode=0;             //����ģʽ
    tmp_sysparas.printmode=0;            //СƱ��ӡ��ʽ

    //�汾�������
    tmp_sysparas.pricever=0;             //���۰汾��
    tmp_sysparas.blackver=0;             //�������汾��
    tmp_sysparas.whitever=0;             //�������汾��
    tmp_sysparas.addbkver=0;             //�����������汾��
    tmp_sysparas.delbkver=0;             //��ɾ�������汾��
    tmp_sysparas.comminfover=0;          //ͨ����Ϣ�汾��

    //��������
    tmp_sysparas.menupwd=0;         //�˵���������
    tmp_sysparas.qcpwd=222222;           //�ʼ�����
    tmp_sysparas.testpwd=234567;         //�������룬�簴������ŷ��Ȳ���
    tmp_sysparas.resetpwd=234567;        //�ָ���������
	tmp_sysparas.onoffworkpwd=0;	 //���°�����
    for(i=0;i<6;i++) tmp_sysparas.pwdboard[i]=0;        //���ܰ�����
    tmp_sysparas.usepwdboard=0;          //0-��ʹ�ü��ܰ壬1-ʹ�ü��ܰ�
    
    tmp_sysparas.modbustype=1;           //����������
    tmp_sysparas.unit=1;                 //������λ
    tmp_sysparas.qcmode=0;               //�ʼ�ģʽ����

    tmp_sysparas.densty=6804;            //�ܶ�              4λС��
    tmp_sysparas.coeff=10000;            //����ϵ��  4λС��
    tmp_sysparas.pzero=820;              //ѹ�����  
    tmp_sysparas.pcoeff =105;            //ѹ��ϵ��  2λС��       
    
    tmp_sysparas.modnum   = 0;              //�޸ļ�¼��ˮ��     
    tmp_sysparas.upmodnum = 0;            //�Ѵ��޸ļ�¼��ˮ��  
	tmp_sysparas.offlinemode = 0;			//����ģʽ	
	//CNG���Ʋ���
	tmp_sysparas.flow20_5 = 150;			//���������٣�����100����
	tmp_sysparas.flow21   = 170;
	tmp_sysparas.flow21_5 = 190;
	tmp_sysparas.flow22   = 220;
	tmp_sysparas.flow22_5 = 290;
	tmp_sysparas.flow23   = 320;
	tmp_sysparas.flow23_5 = 350;
	tmp_sysparas.flow24   = 400;
	tmp_sysparas.flow24_5 = 420;
	tmp_sysparas.lowtomidflow = 350;		//��ѹת��ѹ����
	tmp_sysparas.midtohighflow = 320;		//��ѹת��ѹ����
	tmp_sysparas.lowstopflow = 100;			//������ͣ��
	tmp_sysparas.maxsafeflow = 2500;		//������������ͣ��
	tmp_sysparas.devicetype = 1;			//�豸����
	tmp_sysparas.maxvolumeonce = 999999;	//�����������
	tmp_sysparas.maxmoneyonce = 999999;	//���������
	tmp_sysparas.stoppressure = 2000;		//ͣ��ѹ��������100����
	tmp_sysparas.filtertime = 1;			//������ʱ��0.1��
	tmp_sysparas.valveswitchdelaytime = 10;	//���л���ʱʱ��1��	  
	tmp_sysparas.xqzstartflowrate = 50;
}

