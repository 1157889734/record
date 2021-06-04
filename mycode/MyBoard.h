
//  *****************************************************//
//  MyBoard.h
  
//  *****************************************************//
  

#ifndef __MYBOARD_H
#define __MYBOARD_H


#include "MyAll.h"




//ÄÚºËÎÄ¼þ:   linux-2.6.30.4/driver/char/sbc2440_my_gpio.c
#define IO_DEVICE_NAME	"/dev/my_gpio"





void BoardInit(void);



void LedsInit(void);

void Leds100ms(void);

void Leds500ms(void);




void LedRecordMicOn(void);
void LedRecordMicOff(void);
void LedRecordMicBlink100On(void);
void LedRecordMicBlink100Off(void);
void LedRecordMicBlink500On(void);
void LedRecordMicBlink500Off(void);


void LedRecordTalkOn(void);
void LedRecordTalkOff(void);
void LedRecordTalkBlink100On(void);
void LedRecordTalkBlink100Off(void);
void LedRecordTalkBlink500On(void);
void LedRecordTalkBlink500Off(void);


void LedSpace3On(void);
void LedSpace3Off(void);
void LedSpace3Blink100On(void);
void LedSpace3Blink100Off(void);
void LedSpace3Blink500On(void);
void LedSpace3Blink500Off(void);

void LedSpace2On(void);
void LedSpace2Off(void);
void LedSpace2Blink100On(void);
void LedSpace2Blink100Off(void);
void LedSpace2Blink500On(void);
void LedSpace2Blink500Off(void);

void LedSpace1On(void);
void LedSpace1Off(void);
void LedSpace1Blink100On(void);
void LedSpace1Blink100Off(void);
void LedSpace1Blink500On(void);
void LedSpace1Blink500Off(void);




void AddrInit(void);

int AddrRead(void);





#endif 




