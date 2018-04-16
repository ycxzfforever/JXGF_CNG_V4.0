#ifndef _FM25L04_H_
#define _FM25L04_H_


/*************************************************************
** FM25L04Ƭѡ��
** ˵����FM25l04����С��ַ0x000������ַΪ0x1FF������洢ʱ����
**      0x1FF��ַ����ô��ַ����תΪ0x000��
*************************************************************/ 
#define     SPI0_CS_ENABLE()         SSP_SSELToggle(0, 3, 0)
#define     SPI0_CS_DISABLE()        SSP_SSELToggle(0, 3, 1)
/*************************************************************
** FM25L04�����
*************************************************************/                   
#define SPI_WREN  0x06   //дʹ�ܡ�
#define SPI_WRDI  0x04   //д���ܡ�
#define SPI_RDSR  0x05   //��״̬�Ĵ�����
#define SPI_WRSR  0x01   //д״̬�Ĵ�����
#define SPI_READ  0x03   //�����ݴ洢����
#define SPI_WRITE 0x02   //д���ݴ洢����


/***************************************************************************
**�������ƣ�FmSpiWriteByte
**�������ܣ�ͨ��spi0��FM25L04д��һ�����ݡ�
**��ڲ�����tem--Ҫд������ݡ�
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-4-12

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
uint8_t FmSpiWriteByte(uint8_t tem);

/***************************************************************************
**�������ƣ�FmSpiReadByte
**�������ܣ�ͨ��spi0��FM25L04����һ�����ݡ�
**��ڲ�������
**�� �� ֵ��tem--���������ݡ�
**�� �� �ߣ����
**�������ڣ�2016-4-12

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
uint8_t FmSpiReadByte(void);

/***************************************************************************
**�������ƣ�Fm25l04Write
**�������ܣ���FM25L04���ض���ַд��һ�����ȵ����ݡ�
**��ڲ�����Addr--�洢�׵�ַ��
            *Data--�洢���ݣ�
            Length--���ݳ��ȡ�
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-4-12

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void Fm25l04Write(uint16_t Addr,uint8_t *Data,uint16_t Length);

/***************************************************************************
**�������ƣ�Fm25l04Read
**�������ܣ���FM25L04���ض���ַ����һ�����ȵ����ݡ�
**��ڲ�����Addr--�洢�׵�ַ��
            *Data--�洢���ݣ�
            Length--���ݳ��ȡ�
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-4-12

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
void Fm25l04Read(uint16_t Addr,uint8_t *Data,uint16_t Length);

/***************************************************************************
**�������ƣ�Fm25l04Test
**�������ܣ���FM25L04д��ָ�������ݣ�������������Ƚ϶������ݣ��ж�����
**��ڲ�������
**�� �� ֵ����
**�� �� �ߣ����
**�������ڣ�2016-4-12

**�޸��ߣ�
**�޸����ڣ�
**�޸����ݣ�
***************************************************************************/
uint8_t Fm25l04Test(void);
#endif
