//*******************************************************//
//						MyCommander.h		             //
//*******************************************************//

#ifndef _MYCOMMANDER_H
#define  _MYCOMMANDER_H

#include "MyAll.h"






void CommanderInit(void);


void CommanderSendString(char* pstr);

void Commander500ms(void);



void CommanderReceiveProcess(void);



void CommanderSendLocalIp(void);
void CommanderSendDeviceType(void);
void CommanderSendDeviceId(void);
void CommanderSendTrainId(void);

void CommanderSendAdcValue(int value);


void CommanderSendAddr(void);

void CommanderSendAmpState(void);


void CommanderSendInformation(void);




#endif


