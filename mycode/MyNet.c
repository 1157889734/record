
//  *****************************************************//
//      MyNet.c
//
//  *****************************************************//


#include "MyAll.h"



pthread_t NetControlRcvThreadId;

pthread_t NetBroadcastSndThreadId;
pthread_t NetBroadcastRcvThreadId;

pthread_t NetTalkRcvThreadId;

pthread_t NetPlayclkRcvThreadId;


u8 NetLocalIp[4];
u8 NetRouterIp[4];
u8 NetMaskIp[4];





void NetInit(void)
{
	int ret;

	char cmd[128];


	NetLocalIp[0] = 192;
	NetLocalIp[1] = 168;
	NetLocalIp[2] = 101;
	NetLocalIp[3] = 10;

	NetRouterIp[0] = 192;
	NetRouterIp[1] = 168;
	NetRouterIp[2] = 101;
	NetRouterIp[3] = 1;

	NetMaskIp[0] = 255;
	NetMaskIp[1] = 255;
	NetMaskIp[2] = 255;
	NetMaskIp[3] = 0;


	StorageReadLocalIp();

	NetLocalIp[3] = 10 - 1 + DevId;
	
	
	sprintf(cmd, "ifconfig eth0 %d.%d.%d.%d ", NetLocalIp[0],  NetLocalIp[1],  NetLocalIp[2],  NetLocalIp[3]);
	
	system(cmd);

	msleep(100);
	

	UdpInit();

	ProtocolInit();


	msleep(10);



	ret = pthread_create(&NetControlRcvThreadId, NULL, (void *)NetControlRcvThread, NULL);
	if(ret != 0)
	{
		printf("Create NetControlRcvThread error! \n");
		
	}
	
//	ret = pthread_create(&NetBroadcastSndThreadId, NULL, (void *)NetBroadcastSndThread, NULL);
//	if(ret != 0)
//	{
//		printf("Create NetBroadcastSndThread error! \n");
		
//	}

	ret = pthread_create(&NetBroadcastRcvThreadId, NULL, (void *)NetBroadcastRcvThread, NULL);
	if(ret != 0)
	{
		printf("Create NetBroadcastRcvThread error! \n");
		
	}

	ret = pthread_create(&NetTalkRcvThreadId, NULL, (void *)NetTalkRcvThread, NULL);
	if(ret != 0)
	{
		printf("Create NetTalkRcvThread error! \n");
		
	}

	ret = pthread_create(&NetPlayclkRcvThreadId, NULL, (void *)NetPlayclkRcvThread, NULL);
	if(ret != 0)
	{
		printf("Create NetPlayclkRcvThread error! \n");
		
	}


}





u32 NetIp4toIp32(u8* ip4)
{
	u32 ip32;

	ip32 = ((*(ip4))<<24) | ((*(ip4+1))<<16) | ((*(ip4+2))<<8) | (*(ip4+3));
	

	return ip32;

}




int NetSetLocalIp4(u8 ip1, u8 ip2, u8 ip3, u8 ip4)
{

	if(ip1 < 128)
	{
		return -1;
	}
	if(ip1 > 247)
	{
		return -1;
	}
	if(ip2 > 255)
	{
		return -1;
	}
	if(ip3 > 255)
	{
		return -1;
	}
	if(ip4 > 255)
	{
		return -1;
	}
	

	NetLocalIp[0] = ip1;
	NetLocalIp[1] = ip2;
	NetLocalIp[2] = ip3;
	NetLocalIp[3] = ip4;


	return 0;

}

int NetSetLocalIp32(u32 ip)
{
	u8 ip1;
	u8 ip2;
	u8 ip3;
	u8 ip4;


	ip1 = (u8)(ip>>24);
	ip2 = (u8)(ip>>16);
	ip3 = (u8)(ip>>8);
	ip4 = (u8)(ip);
	
	
	if(ip1 < 128)
	{
		return -1;
	}
	if(ip1 > 247)
	{
		return -1;
	}
	if(ip2 > 255)
	{
		return -1;
	}
	if(ip3 > 255)
	{
		return -1;
	}
	if(ip4 > 255)
	{
		return -1;
	}
	

	NetLocalIp[0] = ip1;
	NetLocalIp[1] = ip2;
	NetLocalIp[2] = ip3;
	NetLocalIp[3] = ip4;
	

	return 0;

}









////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////


void NetControlRcvThread(void)
{
	u8 rcv_buf[128];
	u16 keyword;
	u8* pda;

	int type;
	u8 src_type;
	u8 src_id;

	while(1)
	{

		UdpControlReceive(rcv_buf);

		keyword = ProtocolRcvGetKeyword(rcv_buf);
		src_type = ProtocolRcvGetSrcDevType(rcv_buf);
		src_id = ProtocolRcvGetSrcDevId(rcv_buf);
		
		if(keyword == PRTCL_KEYWORD_TIME)
		{	
			if(RecordState == RECORD_STATE_OFF)
			{
				pda = ProtocolRcvGetDataPointer(rcv_buf);
				NetControlRcvTime(pda);
			}
			

		}	


	}
	

}


void NetControlRcvTime(u8* pbuf)
{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;

	year = (*pbuf) + (*(pbuf+1)*256);
	month = *(pbuf+2);
	day = *(pbuf+3);
	hour = *(pbuf+4);
	minute = *(pbuf+5);
	second = *(pbuf+6);

	if(year > 2099)	
		year = 2099;	
		
	if(year < 2000) 
		year = 2000;	
		
	if(month > 12) 
		month = 12;
		
	if(day > 31) 
		day = 31;
		
	if(hour > 60) 
		hour = 60;
		
	if(minute > 60) 
		minute = 60;
		
	if(second > 60) 
		second = 60;


	AppSetTime(year, month, day, hour, minute, second);

}




void NetControlSndThread(void)
{



}



int NetControlSendInspection(void)
{
	u8 send_buf[64];
	int send_len;


	ProtocolSndGetInspectionPack(send_buf, &send_len);

	UdpControlSend(send_buf, send_len);


	return 0;

}










//////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////



void NetBroadcastSndThread(void)
{
	int broadcast_fd;

	u8 wav_buf[1024];
	int read_len;

	u8 send_buf[1088];
	int send_len;



//	broadcast_fd = open("/home/resource/goodday.wav", O_CREAT|O_RDWR, 0666);
	
	broadcast_fd = open("/home/resource/audio/ls.wav", O_RDONLY, 0666);
	if(broadcast_fd < 0)
	{
		perror("broadcast_fd open:");

	}

	lseek(broadcast_fd, 44, SEEK_SET);   //Seek to the point where the audio data start


	while(1)
	{
	
		read_len = read(broadcast_fd, wav_buf, 1024);
		if(read_len < 0)
		{
			perror("broadcast_fd read:");

			return;

		}

		ProtocolSndGetBroadcastPack(send_buf, &send_len, wav_buf, 2);

		UdpBroadcastSend(send_buf, send_len);
		

		msleep(11);

	}


}




void NetBroadcastRcvThread(void)
{
	u8 rcv_buf[1088];
	u16 keyword;
	u8* pda;

	int type;
	u8 src_type;
	u8 src_id;

	while(1)
	{

		UdpBroadcastReceive(rcv_buf);

		keyword = ProtocolRcvGetKeyword(rcv_buf);
		
		if(keyword == PRTCL_KEYWORD_BROADCAST)
		{
			
			pda = ProtocolRcvGetDataPointer(rcv_buf);

//			printf("broadcast receive type :%d\n", *pda);
			

			if(*pda == 10) 		//broadcast type == speak
			{

				type = RECORD_TYPE_SPEAK;

				src_type = ProtocolRcvGetSrcDevType(rcv_buf);
				src_id = ProtocolRcvGetSrcDevId(rcv_buf);

//				RecordBufSave((pda+2));

				RecordInput((pda+2), type, src_type, src_id);

			}

		}	


	}



}





//////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////




void NetTalkRcvThread(void)
{

	u8 rcv_buf[1088];
	u16 keyword;
	u8* pda;

	int type;
	u8 src_type;
	u8 src_id;

	while(1)
	{

		UdpTalkReceive(rcv_buf);

		keyword = ProtocolRcvGetKeyword(rcv_buf);
		
		if(keyword == PRTCL_KEYWORD_TALK_DD)
		{

			pda = ProtocolRcvGetDataPointer(rcv_buf);

			type = RECORD_TYPE_TALK_DD;

			src_type = ProtocolRcvGetSrcDevType(rcv_buf);
			src_id = ProtocolRcvGetSrcDevId(rcv_buf);

//			RecordBufSave((pda));

			RecordInput(pda, type, src_type, src_id);

			
			//printf("receive dd\n");

		}
		else if(keyword == PRTCL_KEYWORD_TALK_DC)
		{

			pda = ProtocolRcvGetDataPointer(rcv_buf);

			type = RECORD_TYPE_TALK_DC;

			src_type = ProtocolRcvGetSrcDevType(rcv_buf);
			src_id = ProtocolRcvGetSrcDevId(rcv_buf);

//			RecordBufSave((pda));

			RecordInput(pda, type, src_type, src_id);
			

			//printf("receive dc\n");

		}	

//		usleep(10);

	}


}






//////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////




void NetPlayclkRcvThread(void)
{

	u8 playclk_buf[32];

	u32 keyword;

	u32* pda;


	while(1)
	{

		UdpPlayclkReceive(playclk_buf);

		if(ProtocolRcvHeadCheck(playclk_buf) != 0)
		{
			continue;

		}

		keyword = ProtocolRcvGetKeyword(playclk_buf);
		if(keyword == PRTCL_KEYWORD_PLAYCLK)
		{
			pda = (u32*)(ProtocolRcvGetDataPointer(playclk_buf));


			PlayClock = *pda;
			PlayClockRcvFlag = 1;


		}
		

		msleep(1);

	}



}


void NetPlayclkSndProcess(void)
{






}



//////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////








//////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////











