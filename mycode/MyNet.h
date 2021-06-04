
//  ******************************************************************************
//    MyNet.h
  
//  ******************************************************************************
  

#ifndef __MYNET_H
#define __MYNET_H


#include "MyAll.h"




extern u8 NetLocalIp[4];
extern u8 NetRouterIp[4];
extern u8 NetMaskIp[4];





void NetInit(void);




u32 NetIp4toIp32(u8* ip4);

int NetSetLocalIp4(u8 ip1, u8 ip2, u8 ip3, u8 ip4);
int NetSetLocalIp32(u32 ip);





void NetControlRcvThread(void);

void NetControlRcvTime(u8* pbuf);


void NetControlSndThread(void);

int NetControlSendInspection(void);




void NetBroadcastSndThread(void);
void NetBroadcastRcvThread(void);




void NetTalkRcvThread(void);






void NetPlayclkRcvThread(void);

void NetPlayclkSndProcess(void);








#endif 




