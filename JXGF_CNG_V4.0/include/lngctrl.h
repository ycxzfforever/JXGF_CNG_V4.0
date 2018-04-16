#ifndef __LNGCTRL_H__
#define __LNGCTRL_H__


void    PumpSwitch(unsigned char state);
void    StartFueling(void);
void    StopFueling(void);
void    StartPrecooling(void);

void    GetCarddate(void);
void    IcGas(void);
void    HuiIccard(void);
void    YesOrNoCard(void);
uint8_t YesOrNoCard1(void);
void    ApplyVerif(void);
void    VerificationCard(void);
extern  RealTimeData    realtime;
void    ApplyHeiBai(void);
void    GetBlack(void);
void    GetWhite(void);

void SaveFm25l04(void);
void PowerDown(void);

void PowerDownRealDate(void);

void FixGas(void);
void FixNum(void);
void FixMoney(void);
void DisPlayLock(void);
uint8_t SeedBai(uint8_t cardnum[8]);
void BcdToStr(uint8_t bcd[],uint8_t str[]);

void CompareLock(void);
void InputIccardLock(void);
void IcCardLock(void);
void DisplayGP(void);
extern double    lngvolume;//½øÒºÁ¿
#endif

