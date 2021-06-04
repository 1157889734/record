
//  *****************************************************//
//  MyApp.h
  
//  *****************************************************//
  

#ifndef __MYAPP_H
#define __MYAPP_H


#include "MyAll.h"




extern u8 DevType;
extern u8 DevId;

extern u16 TrainId;

extern u8 AppVersion;
extern u8 AppVersion2;
extern u8 AppVersion3;
extern u8 AppVersion4;



extern u8 CurrentState;


extern u32 PlayClock;
extern int PlayClockRcvFlag;



void AppInit(void);

void AppThread(void);



void App100ms(void);
void App500ms(void);
void App1000ms(void);

void AppLedControl(void);

void AppSetTime(int year, int month, int day, int hour, int minute, int second);



#endif 




