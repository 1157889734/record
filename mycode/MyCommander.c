
//*****************************************************//
//		MyCommander.c
//*****************************************************//

#include "MyAll.h"



char CommanderSendStr[UDP_PACK_MAXSIZE];


u8 CommanderRcvBuf[UDP_PACK_MAXSIZE];
int CommanderRcvBufSize;




void CommanderInit(void)
{

//	Debuger = sizeof(CommanderRcvBuf);
//	printf("Size of CommanderRcvBuf is %d \n", Debuger);
	

	memset(CommanderRcvBuf, 0, sizeof(CommanderRcvBuf));
	CommanderRcvBufSize = 0;


	char logo_str[] = "//**********  Beihai Communication  ***********//\n\n\
Digital_PIS  Recorder         \n\
Firmware version: 0.1  \n\n" ;
	
	CommanderSendString(logo_str);


	CommanderSendLocalIp();
	CommanderSendDeviceType();
	CommanderSendDeviceId();
	CommanderSendTrainId();
	

}


void CommanderSendString(char* pstr)
{

	UdpCommanderSend((u8*)pstr, strlen(pstr));


}


void Commander500ms(void)
{

//	if((AppStartFlag > 0) && (CommanderSendAdcValueFlag > 0))
//	{
		
//		CommanderSendAdcValue(AdcGetValue());


//	}
	

}




void CommanderReceiveProcess(void)
{

	char words[16][16];
	u8 rip[4];
	int rtemp;


	CommanderRcvBufSize = UdpCommanderReceive(CommanderRcvBuf);


	memset(words, 0, sizeof(words));
	
	LineToWords((char *)CommanderRcvBuf, 256, &words[0][0], 16, 16);


	if((strcmp(&words[0][0], "set") == 0) && (strcmp(&words[1][0], "localip") == 0))
	{
		rip[0] = (u8)StrToInt(&words[2][0]);
		rip[1] = (u8)StrToInt(&words[3][0]);
		rip[2] = (u8)StrToInt(&words[4][0]);
		rip[3] = (u8)StrToInt(&words[5][0]);

//		memset(OutStr, 0, sizeof(OutStr));		
//		sprintf(OutStr, "Ok, please wait...\n");
//		CommanderSendString(OutStr);

//		NetSetLocalIp4(rip[0], rip[1], rip[2], rip[3]);
		

	}
//	else if((strcmp(&words[0][0], "read") == 0) && (strcmp(&words[1][0], "localip") == 0))
//	{
//		CommanderSendLocalIp();
		
//	}
	else if((strcmp(&words[0][0], "read") == 0) && (strcmp(&words[1][0], "devtype") == 0))
	{
		CommanderSendDeviceType();
		
	}
	else if((strcmp(&words[0][0], "set") == 0) && (strcmp(&words[1][0], "devid") == 0))
	{
		rtemp = StrToInt(&words[2][0]);
//		SetDeviceId(rtemp);

	}
	else if((strcmp(&words[0][0], "read") == 0) && (strcmp(&words[1][0], "devid") == 0))
	{
		CommanderSendDeviceId();
		
	}
//	else if((strcmp(&words[0][0], "set") == 0) && (strcmp(&words[1][0], "trainid") == 0))
//	{
//		rtemp = StrToInt(&words[2][0]);
//		SetTrainId(rtemp);

//	}
	else if((strcmp(&words[0][0], "read") == 0) && (strcmp(&words[1][0], "trainid") == 0))
	{
		CommanderSendTrainId();
		
	}
	else if((strcmp(&words[0][0], "read") == 0) && (strcmp(&words[1][0], "information") == 0))
	{
		CommanderSendInformation();
		
	}
//	else if((strcmp(&words[0][0], "set") == 0) && (strcmp(&words[1][0], "volume") == 0))
//	{
//		rtemp = StrToInt(&words[2][0]);
//		AudioSetVolume(rtemp);

//	}


//	else if((strcmp(&words[0][0], "read") == 0) && (strcmp(&words[1][0], "adc") == 0))
//	{
//		if (strcmp(&words[2][0], "start") == 0)
//		{
//			CommanderSendAdcValueFlag = 1;

//		}
//		else if (strcmp(&words[2][0], "stop") == 0)
//		{
//			CommanderSendAdcValueFlag = 0;

//		}	

//	}

	
//	else if((strcmp(&words[0][0], "read") == 0) && (strcmp(&words[1][0], "addr") == 0))
//	{
//		CommanderSendAddr();
		
//	}
//	else if((strcmp(&words[0][0], "read") == 0) && (strcmp(&words[1][0], "ampstate") == 0))
//	{
//		CommanderSendAmpState();
		
//	}
	

	else
	{
		memset(CommanderSendStr, 0, sizeof(CommanderSendStr));		
		sprintf(CommanderSendStr, "Invalid Command !\n");
		CommanderSendString(CommanderSendStr);

	}



}



void CommanderSendLocalIp(void)
{

	memset(CommanderSendStr, 0, sizeof(CommanderSendStr));		
	sprintf(CommanderSendStr, "Local IP: %d.%d.%d.%d  \n", NetLocalIp[0], NetLocalIp[1],NetLocalIp[2], NetLocalIp[3]);
	CommanderSendString(CommanderSendStr);

}



void CommanderSendDeviceType(void)
{

	memset(CommanderSendStr, 0, sizeof(CommanderSendStr));		
	sprintf(CommanderSendStr, "Device Type: %d  \n", DevType);
	CommanderSendString(CommanderSendStr);

}


void CommanderSendDeviceId(void)
{

	memset(CommanderSendStr, 0, sizeof(CommanderSendStr));		
	sprintf(CommanderSendStr, "Device Id: %d  \n", DevId);
	CommanderSendString(CommanderSendStr);

}


void CommanderSendTrainId(void)
{

	memset(CommanderSendStr, 0, sizeof(CommanderSendStr));		
	sprintf(CommanderSendStr, "Train Id: %d  \n", TrainId);
	CommanderSendString(CommanderSendStr);

}


void CommanderSendAdcValue(int value)
{

	memset(CommanderSendStr, 0, sizeof(CommanderSendStr));		
	sprintf(CommanderSendStr, "Adc Value: %d  \n", value);
	CommanderSendString(CommanderSendStr);

}


void CommanderSendAddr(void)
{

//	memset(OutStr, 0, sizeof(OutStr));		
//	sprintf(OutStr, "Address: %d  \n", Addr);
//	CommanderSendString(OutStr);

}


void CommanderSendAmpState(void)
{

//	memset(OutStr, 0, sizeof(OutStr));	
	
//	sprintf(OutStr, "AmpState: %d  \n", AmpState);
//	CommanderSendString(OutStr);

}



void CommanderSendInformation(void)
{

	CommanderSendLocalIp();
	CommanderSendDeviceType();
	CommanderSendDeviceId();
	CommanderSendTrainId();

}












