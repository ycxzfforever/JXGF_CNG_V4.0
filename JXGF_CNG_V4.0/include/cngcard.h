#ifndef __CNGCARD_H
#define __CNGCARD_H

void    GetCardMessage(void);
void    IcGas(void);
void    HuiIccard(void);
void    YesOrNoCard(void);
void    ApplyVerif(void);
void CompareLock(void);
void InputIccardLock(void);
void IcCardLock(void);
void  CardValidQuery(void);
void  CardMesssageDisplay(void);
uint8_t SearchBlackList(uint8_t type,uint8_t *cardNo);
#endif
