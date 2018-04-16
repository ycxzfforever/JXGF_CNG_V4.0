#ifndef __LNGCTRL_H__
#define __LNGCTRL_H__

extern  RealTimeData    realtime;
extern double    lngvolume;//½øÒºÁ¿

void PowerDown(void);
void PowerDownRealDate(void);

void FuelReady(void);
void FuelWork(void);
void FuelEnd(void);

void FixGas(void);
void FixNum(void);
void FixMoney(void);

void SaveFm25l04(void);

void DisPlayLock(void);
void DisplayGP(void);

void CirculateFueling(void);

#endif

