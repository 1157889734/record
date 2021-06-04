
//  *****************************************************//
//      MyUdp.c
//
//  *****************************************************//


#include "MyAll.h"




int UdpTestSocket;

u8 UdpTestRcvBuf[UDP_PACK_MAXSIZE];
u8* pUdpTestRcvBuf;
int UdpTestRcvBufSize;




int UdpCommanderSocket;

int UdpControlRcvSocket;
int UdpControlSndSocket;

int UdpBroadcastRcvSocket;			//Audio Broadcast Data
int UdpBroadcastSndSocket;

int UdpTalkRcvSocket;
int UdpTalkSndSocket;

int UdpPlayclkRcvSocket;
int UdpPlayclkSndSocket;






int UdpInit(void)
{



	UdpTestInit();
	
	UdpCommanderInit();
	

	UdpControlRcvInit();
	UdpControlSndInit();

	UdpBroadcastRcvInit();
	UdpBroadcastSndInit();

	UdpTalkRcvInit();
	UdpTalkSndInit();

	UdpPlayclkRcvInit();
	UdpPlayclkSndInit();

	
	return 0;

}



////////////////////////////////////////////////////////////////////////


int UdpTestInit(void)
{
	int r;
	struct sockaddr_in local_sockaddr;
	
	
	UdpTestSocket = socket(AF_INET, SOCK_DGRAM,0);
	if(UdpTestSocket == -1)
	{
		perror("UdpTest socket");
		return -1;	
	}
	
	
	local_sockaddr.sin_family = AF_INET;
	local_sockaddr.sin_port = htons(UDP_PORT_TEST_L);
	local_sockaddr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(local_sockaddr.sin_zero), 8);
	
	r = bind(UdpTestSocket, (struct sockaddr *)&local_sockaddr, sizeof(struct sockaddr));
	if(r == -1)
	{
		perror("UdpTest bind");
		return -1;	
	}


	pUdpTestRcvBuf = UdpTestRcvBuf;
	
	
	return 0;
	
}


int UdpTestSend(void)
{
	int r;
	struct sockaddr_in remote_sockaddr;
	
	
	u8 outda[8];

	outda[0] = 0x20;
	outda[1] = 0x21;
	outda[2] = 0x22;
	outda[3] = 0x23;
	outda[4] = 0x24;
	outda[5] = 0x25;
	outda[6] = 0x26;
	outda[7] = 0x27;
	

	
	remote_sockaddr.sin_family = AF_INET;
	remote_sockaddr.sin_port = htons(UDP_PORT_TEST_R);
	remote_sockaddr.sin_addr.s_addr = inet_addr("192.168.101.187");
//	remote_sockaddr.sin_addr.s_addr = INADDR_BROADCAST;
	bzero(&(remote_sockaddr.sin_zero), 8);
	
	r = sendto(UdpTestSocket, outda, 8, 0, (struct sockaddr *)&remote_sockaddr, sizeof(struct sockaddr));
	if(r == -1)
	{
		perror("UdpTest sendto");
		
		return -1;	
	}
	
	
	return r;
	
}



int UdpTestReceive(void)
{
	struct sockaddr_in addr;
	int addr_len;


	recvfrom(UdpTestSocket, UdpTestRcvBuf, UDP_PACK_MAXSIZE, 0, 
			(struct sockaddr *)&addr, (socklen_t *)&addr_len);


	UdpTestSend();


	return 0;

}


////////////////////////////////////////////////////////////////////////



int UdpCommanderInit(void)
{
	int r;
	struct sockaddr_in local_sockaddr;
	
	
	UdpCommanderSocket = socket(AF_INET, SOCK_DGRAM,0);
	if(UdpCommanderSocket == -1)
	{
		perror("UdpCommander socket");
		return -1;	
	}
	
	
	local_sockaddr.sin_family = AF_INET;
	local_sockaddr.sin_port = htons(UDP_PORT_COMMANDER_L);
	local_sockaddr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(local_sockaddr.sin_zero), 8);
	
	r = bind(UdpCommanderSocket, (struct sockaddr *)&local_sockaddr, sizeof(struct sockaddr));
	if(r == -1)
	{
		perror("UdpCommander bind");
		return -1;	
	}
	
	
	return 0;
	
}




int UdpCommanderSend(u8* pbuf, int len)
{
	int r;
	struct sockaddr_in remote_sockaddr;
	
	
	remote_sockaddr.sin_family = AF_INET;
	remote_sockaddr.sin_port = htons(UDP_PORT_COMMANDER_R);
	remote_sockaddr.sin_addr.s_addr = inet_addr("192.168.101.187");
//	remote_sockaddr.sin_addr.s_addr = INADDR_BROADCAST;
	bzero(&(remote_sockaddr.sin_zero), 8);
	
	r = sendto(UdpCommanderSocket, pbuf, len, 0, (struct sockaddr *)&remote_sockaddr, sizeof(struct sockaddr));
	if(r == -1)
	{
		perror("UdpCommander sendto");
		
		return -1;	
	}
	
	
	return r;
	
}




int UdpCommanderReceive(u8* pbuf)
{
	struct sockaddr_in addr;
	int addr_len;

	int len;


	len = recvfrom(UdpCommanderSocket, pbuf, UDP_PACK_MAXSIZE, 0, 
			(struct sockaddr *)&addr, (socklen_t *)&addr_len);


	if(len < 0)
	{
		perror("UdpCommander recvfrom");


	}


	return len;

}



////////////////////////////////////////////////////////////////////////



int UdpControlRcvInit(void)
{
	int r;
	struct sockaddr_in local_sockaddr;
	
	
	UdpControlRcvSocket = socket(AF_INET, SOCK_DGRAM,0);
	if(UdpControlRcvSocket == -1)
	{
		perror("UdpControlRcv socket");
		return -1;	
	}
	
	
	local_sockaddr.sin_family = AF_INET;
	local_sockaddr.sin_port = htons(UDP_PORT_CONTROL_RCV_L);
	local_sockaddr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(local_sockaddr.sin_zero), 8);
	
	r = bind(UdpControlRcvSocket, (struct sockaddr *)&local_sockaddr, sizeof(struct sockaddr));
	if(r == -1)
	{
		perror("UdpControlRcv bind");
		return -1;	
	}

	
	return 0;
	
}


int UdpControlReceive(u8* pbuf)
{
	struct sockaddr_in addr;
	int addr_len;

	int len;


	len = recvfrom(UdpControlRcvSocket, pbuf, UDP_PACK_MAXSIZE, 0, 
			(struct sockaddr *)&addr, (socklen_t *)&addr_len);

	if(len < 0)
	{
		perror("UdpControlRcv recvfrom");

	}


	return len;

}




int UdpControlSndInit(void)
{
	int r;
	struct sockaddr_in local_sockaddr;

	int yes;
	int set;
	
	
	UdpControlSndSocket = socket(AF_INET, SOCK_DGRAM,0);
	if(UdpControlSndSocket == -1)
	{
		perror("UdpControlSnd socket");
		return -1;	
	}
	
	
	local_sockaddr.sin_family = AF_INET;
	local_sockaddr.sin_port = htons(UDP_PORT_CONTROL_SND_L);
	local_sockaddr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(local_sockaddr.sin_zero), 8);

	yes = 1;
	set = setsockopt(UdpControlSndSocket, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes));
	
	r = bind(UdpControlSndSocket, (struct sockaddr *)&local_sockaddr, sizeof(struct sockaddr));
	if(r == -1)
	{
		perror("UdpControlSnd bind");
		return -1;	
	}

	
	return 0;
	
}

int UdpControlSend(u8* pbuf, int len)
{
	int r;
	struct sockaddr_in remote_sockaddr;
	
	
	remote_sockaddr.sin_family = AF_INET;
	remote_sockaddr.sin_port = htons(UDP_PORT_CONTROL_SND_R);
	remote_sockaddr.sin_addr.s_addr = inet_addr("192.168.101.255");
//	remote_sockaddr.sin_addr.s_addr = INADDR_BROADCAST;
	bzero(&(remote_sockaddr.sin_zero), 8);
	
	r = sendto(UdpControlSndSocket, pbuf, len, 0, (struct sockaddr *)&remote_sockaddr, sizeof(struct sockaddr));
	if(r == -1)
	{
		perror("UdpControlSnd sendto");
		
		return -1;	
	}
	
	
	return r;
	
}



////////////////////////////////////////////////////////////////////////



int UdpBroadcastRcvInit(void)
{
	int r;
	struct sockaddr_in local_sockaddr;
	
	
	UdpBroadcastRcvSocket = socket(AF_INET, SOCK_DGRAM,0);
	if(UdpBroadcastRcvSocket == -1)
	{
		perror("UdpBroadcastRcv socket");
		return -1;	
	}
	
	
	local_sockaddr.sin_family = AF_INET;
	local_sockaddr.sin_port = htons(UDP_PORT_BROADCAST_RCV_L);
	local_sockaddr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(local_sockaddr.sin_zero), 8);
	
	r = bind(UdpBroadcastRcvSocket, (struct sockaddr *)&local_sockaddr, sizeof(struct sockaddr));
	if(r == -1)
	{
		perror("UdpBroadcastRcv bind");
		return -1;	
	}

	
	return 0;
	
}


int UdpBroadcastReceive(u8* pbuf)
{
	struct sockaddr_in addr;
	int addr_len;

	int len;


	len = recvfrom(UdpBroadcastRcvSocket, pbuf, UDP_PACK_MAXSIZE, 0, 
			(struct sockaddr *)&addr, (socklen_t *)&addr_len);

	if(len < 0)
	{
		perror("UdpBroadcastRcv recvfrom");

	}


	return len;

}




int UdpBroadcastSndInit(void)
{
	int r;
	struct sockaddr_in local_sockaddr;
	
	
	UdpBroadcastSndSocket = socket(AF_INET, SOCK_DGRAM,0);
	if(UdpBroadcastSndSocket == -1)
	{
		perror("UdpBroadcastSnd socket");
		return -1;	
	}
	
	
	local_sockaddr.sin_family = AF_INET;
	local_sockaddr.sin_port = htons(UDP_PORT_BROADCAST_SND_L);
	local_sockaddr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(local_sockaddr.sin_zero), 8);
	
	r = bind(UdpBroadcastSndSocket, (struct sockaddr *)&local_sockaddr, sizeof(struct sockaddr));
	if(r == -1)
	{
		perror("UdpBroadcastSnd bind");
		return -1;	
	}

	
	return 0;
	
}

int UdpBroadcastSend(u8* pbuf, int len)
{
	int r;
	struct sockaddr_in remote_sockaddr;
	
	
	remote_sockaddr.sin_family = AF_INET;
	remote_sockaddr.sin_port = htons(UDP_PORT_BROADCAST_SND_R);
	remote_sockaddr.sin_addr.s_addr = inet_addr("192.168.101.106");
//	remote_sockaddr.sin_addr.s_addr = INADDR_BROADCAST;
	bzero(&(remote_sockaddr.sin_zero), 8);
	
	r = sendto(UdpBroadcastSndSocket, pbuf, len, 0, (struct sockaddr *)&remote_sockaddr, sizeof(struct sockaddr));
	if(r == -1)
	{
		perror("UdpBroadcastSnd sendto");
		
		return -1;	
	}
	
	
	return r;
	
}


////////////////////////////////////////////////////////////////////////



int UdpTalkRcvInit(void)
{
	int r;
	struct sockaddr_in local_sockaddr;
	
	
	UdpTalkRcvSocket = socket(AF_INET, SOCK_DGRAM,0);
	if(UdpTalkRcvSocket == -1)
	{
		perror("UdpBroadcastRcv socket");
		return -1;	
	}
	
	
	local_sockaddr.sin_family = AF_INET;
	local_sockaddr.sin_port = htons(UDP_PORT_TALK_RCV_L);
	local_sockaddr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(local_sockaddr.sin_zero), 8);
	
	r = bind(UdpTalkRcvSocket, (struct sockaddr *)&local_sockaddr, sizeof(struct sockaddr));
	if(r == -1)
	{
		perror("UdpTalkRcv bind");
		return -1;	
	}

	
	return 0;
	
}


int UdpTalkReceive(u8* pbuf)
{
	struct sockaddr_in addr;
	int addr_len;

	int len;


	len = recvfrom(UdpTalkRcvSocket, pbuf, UDP_PACK_MAXSIZE, 0, 
			(struct sockaddr *)&addr, (socklen_t *)&addr_len);

	if(len < 0)
	{
//		perror("UdpTalkRcv recvfrom");

		printf("UdpTalkReceive len = %d", len);

	}


	return len;

}




int UdpTalkSndInit(void)
{
	int r;
	struct sockaddr_in local_sockaddr;
	
	
	UdpTalkSndSocket = socket(AF_INET, SOCK_DGRAM,0);
	if(UdpTalkSndSocket == -1)
	{
		perror("UdpTalkSnd socket");
		return -1;	
	}
	
	
	local_sockaddr.sin_family = AF_INET;
	local_sockaddr.sin_port = htons(UDP_PORT_TALK_SND_L);
	local_sockaddr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(local_sockaddr.sin_zero), 8);
	
	r = bind(UdpTalkSndSocket, (struct sockaddr *)&local_sockaddr, sizeof(struct sockaddr));
	if(r == -1)
	{
		perror("UdpTalkSnd bind");
		return -1;	
	}

	
	return 0;
	
}

int UdpTalkSend(u8* pbuf, int len)
{
	int r;
	struct sockaddr_in remote_sockaddr;
	
	
	remote_sockaddr.sin_family = AF_INET;
	remote_sockaddr.sin_port = htons(UDP_PORT_TALK_SND_R);
	remote_sockaddr.sin_addr.s_addr = inet_addr("192.168.101.187");
//	remote_sockaddr.sin_addr.s_addr = INADDR_BROADCAST;
	bzero(&(remote_sockaddr.sin_zero), 8);
	
	r = sendto(UdpTalkSndSocket, pbuf, len, 0, (struct sockaddr *)&remote_sockaddr, sizeof(struct sockaddr));
	if(r == -1)
	{
		perror("UdpTalkSnd sendto");
		
		return -1;	
	}
	
	
	return r;
	
}




////////////////////////////////////////////////////////////////////////



int UdpPlayclkRcvInit(void)
{
	int r;
	struct sockaddr_in local_sockaddr;
	
	
	UdpPlayclkRcvSocket = socket(AF_INET, SOCK_DGRAM,0);
	if(UdpPlayclkRcvSocket == -1)
	{
		perror("UdpPlayclkRcv socket");
		return -1;	
	}
	
	
	local_sockaddr.sin_family = AF_INET;
	local_sockaddr.sin_port = htons(UDP_PORT_PLAYCLK_RCV_L);
	local_sockaddr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(local_sockaddr.sin_zero), 8);
	
	r = bind(UdpPlayclkRcvSocket, (struct sockaddr *)&local_sockaddr, sizeof(struct sockaddr));
	if(r == -1)
	{
		perror("UdpPlayclkRcv bind");
		return -1;	
	}

	
	return 0;
	
}


int UdpPlayclkReceive(u8* pbuf)
{
	struct sockaddr_in addr;
	int addr_len;

	int len;


	len = recvfrom(UdpPlayclkRcvSocket, pbuf, UDP_PACK_MAXSIZE, 0, 
			(struct sockaddr *)&addr, (socklen_t *)&addr_len);

	if(len < 0)
	{
//		perror("UdpPlayclkRcv recvfrom");

	}


	return len;

}




int UdpPlayclkSndInit(void)
{
	int r;
	struct sockaddr_in local_sockaddr;
	
	
	UdpPlayclkSndSocket = socket(AF_INET, SOCK_DGRAM,0);
	if(UdpPlayclkSndSocket == -1)
	{
		perror("UdpPlayclkSnd socket");
		return -1;	
	}
	
	
	local_sockaddr.sin_family = AF_INET;
	local_sockaddr.sin_port = htons(UDP_PORT_PLAYCLK_SND_L);
	local_sockaddr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(local_sockaddr.sin_zero), 8);
	
	r = bind(UdpPlayclkSndSocket, (struct sockaddr *)&local_sockaddr, sizeof(struct sockaddr));
	if(r == -1)
	{
		perror("UdpPlayclkSnd bind");
		return -1;	
	}

	
	return 0;
	
}

int UdpPlayclkSend(u8* pbuf, int len)
{
	int r;
	struct sockaddr_in remote_sockaddr;
	
	
	remote_sockaddr.sin_family = AF_INET;
	remote_sockaddr.sin_port = htons(UDP_PORT_PLAYCLK_SND_R);
	remote_sockaddr.sin_addr.s_addr = inet_addr("192.168.101.187");
//	remote_sockaddr.sin_addr.s_addr = INADDR_BROADCAST;
	bzero(&(remote_sockaddr.sin_zero), 8);
	
	r = sendto(UdpPlayclkSndSocket, pbuf, len, 0, (struct sockaddr *)&remote_sockaddr, sizeof(struct sockaddr));
	if(r == -1)
	{
		perror("UdpPlayclkSnd sendto");
		
		return -1;	
	}
	
	
	return r;
	
}




//////////////////////////////////////////////////////////////////////










