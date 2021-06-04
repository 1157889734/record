
//  ******************************************************************************
//  * @file    MyUdp.h
  
//  ******************************************************************************
  

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MYUDP_H
#define __MYUDP_H


#include "MyAll.h"



#define UDP_PACK_MAXSIZE	1500

#define UDP_PORT_TEST_L		15202
#define UDP_PORT_TEST_R		15202


#define UDP_PORT_COMMANDER_L		15200
#define UDP_PORT_COMMANDER_R		15201



#define UDP_PORT_CONTROL_SND_L		50151
#define UDP_PORT_CONTROL_SND_R		50152

#define UDP_PORT_CONTROL_RCV_L		50152
#define UDP_PORT_CONTROL_RCV_R		50151


#define UDP_PORT_BROADCAST_SND_L	51151
#define UDP_PORT_BROADCAST_SND_R	51152

#define UDP_PORT_BROADCAST_RCV_L	51152
#define UDP_PORT_BROADCAST_RCV_R	51151


#define UDP_PORT_TALK_SND_L			52151
#define UDP_PORT_TALK_SND_R			52152

#define UDP_PORT_TALK_RCV_L			52152
#define UDP_PORT_TALK_RCV_R			52151


#define UDP_PORT_PLAYCLK_SND_L		53151
#define UDP_PORT_PLAYCLK_SND_R		53152

#define UDP_PORT_PLAYCLK_RCV_L		53152
#define UDP_PORT_PLAYCLK_RCV_R		53151





int UdpInit(void);


int UdpTestInit(void);
int UdpTestSend(void);
int UdpTestReceive(void);



int UdpCommanderInit(void);
int UdpCommanderSend(u8* pbuf, int len);
int UdpCommanderReceive(u8* pbuf);



int UdpControlRcvInit(void);
int UdpControlReceive(u8* pbuf);
int UdpControlSndInit(void);
int UdpControlSend(u8* pbuf, int len);


int UdpBroadcastRcvInit(void);
int UdpBroadcastReceive(u8* pbuf);
int UdpBroadcastSndInit(void);
int UdpBroadcastSend(u8* pbuf, int len);


int UdpTalkRcvInit(void);
int UdpTalkReceive(u8* pbuf);
int UdpTalkSndInit(void);
int UdpTalkSend(u8* pbuf, int len);


int UdpPlayclkRcvInit(void);
int UdpPlayclkReceive(u8* pbuf);
int UdpPlayclkSndInit(void);
int UdpPlayclkSend(u8* pbuf, int len);







#endif 

