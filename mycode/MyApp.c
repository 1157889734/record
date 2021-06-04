
//  *****************************************************//
//      MyApp.c
//
//  *****************************************************//


#include "MyAll.h"




pthread_t AppThreadId;



u8 DevType;
u8 DevId;

u16 TrainId;


u8 AppVersion;
u8 AppVersion2;
u8 AppVersion3;
u8 AppVersion4;



u8 CurrentState;


u32 PlayClock;
int PlayClockRcvFlag;





void AppInit(void)
{
	int ret;

	DevType = PRTCL_DEVTYPE_RECORDER;

	DevId = 1;

	TrainId = 1;


	AppVersion = 1;
	AppVersion2 = 0;	
	AppVersion3 = 6;	
	AppVersion4 = 65;

	
	CurrentState = 0;


	StorageReadDevId();


	AddrInit();
	DevId = AddrRead();



	printf("DevId: %d.\n", DevId);



	ret = pthread_create(&AppThreadId, NULL, (void *)AppThread, NULL);
	if(ret != 0)
	{
		printf("Create AppThread error! \n");
		return;
	}


  
}


void AppThread(void)
{

	while(1)
	{
		
		AppLedControl();
			

		msleep(100);

		
	}



}




void App100ms(void)
{





}


void App500ms(void)
{

	NetControlSendInspection();



}


void App1000ms(void)
{





}



void AppLedControl(void)
{
	static int busy_flag;

	switch(RecordState)
	{
		case RECORD_STATE_ON:
		{
			if(RecordType == RECORD_TYPE_SPEAK)
			{
				LedRecordMicBlink100On();
				LedRecordTalkBlink100Off();
					
			}
			else
			{
				LedRecordMicBlink100Off();
				LedRecordTalkBlink100On();
					
			}
			
			break;
				
		}
		case RECORD_STATE_OFF:
		{
			LedRecordMicBlink100Off();
			LedRecordTalkBlink100Off();	
				
			break;
				
		}
			
		default :
		{


		}	
			
	}


	if(EncodeState != ENCODE_STATE_OFF)
	{
		busy_flag = 50;
	}

	busy_flag--;
	if(busy_flag == 20)
	{
		system("sync");
	}
	else if(busy_flag == 10)
	{
		system("sync");
	}
	else if(busy_flag < 0)
	{
		busy_flag = 0;
	}


	if(StorageSpaceFreePercent > 90)
	{
		if(busy_flag > 0)
		{
			LedSpace3Blink100On();
		}
		else
		{
			LedSpace3Blink100Off();
			LedSpace3On();
		}
		
		LedSpace2On();
		LedSpace1On();

	}
	else if(StorageSpaceFreePercent > 60)
	{
	
		LedSpace3Off();
		
		if(busy_flag > 0)
		{
			LedSpace2Blink100On();
		}
		else
		{
			LedSpace2Blink100Off();
			LedSpace2On();
		}
		
		LedSpace1On();

	}
	else if(StorageSpaceFreePercent > 30)
	{
	
		LedSpace3Off();
		LedSpace2Off();
		
		if(busy_flag > 0)
		{
			LedSpace1Blink100On();
		}
		else
		{
			LedSpace1On();
		}

	}
	else if(StorageSpaceFreePercent > 10)
	{
		LedSpace3Off();
		LedSpace2Off();
		
		if(busy_flag > 0)
		{
			LedSpace1Blink100On();
		}
		else
		{
			LedSpace1Blink100Off();
			LedSpace1Blink500On();
		}

	}
	else
	{
		LedSpace3Off();
		LedSpace2Off();
		LedSpace1Off();

//		LedSpace1Blink500Off();

	}




}




void AppSetTime(int year, int month, int day, int hour, int minute, int second)
{
	struct tm *timenow;
	time_t now;

	int flag;

	char cmd[64];


	time(&now);
	timenow = localtime(&now);


	flag = 0;

	if((timenow->tm_year+1900) != year)	flag = 1;
	if((timenow->tm_mon+1) != month) 	flag = 1;
	if((timenow->tm_mday) != day)		flag = 1;
	if((timenow->tm_hour) != hour)		flag = 1;
	if((timenow->tm_min) != minute)		flag = 1;
//	if((timenow->tm_sec) != second)		flag = 1;


	if(flag > 0)
	{
		sprintf(cmd, "date -s %04d.%02d.%02d-%02d:%02d:%02d &", year, month, day, hour, minute, second);

		system(cmd);
//		system("hwclock -w");

	}


}














