
//  *****************************************************//
//      main.c
//
//  *****************************************************//
  


#include "MyAll.h"


u32 Debuger;



pthread_t ThreadTestId;

pthread_t ThreadUdpTestRcvId;

pthread_t ThreadCommanderRcvId;

pthread_t ThreadMyTimerId;







void ThreadTest(void)
{
	while(1)
	{
    		printf("Recorder: I'm Running.    ");

    		if(RecordState != RECORD_STATE_OFF)
    		{
			printf("Recording    ");

    		}

    		if(EncodeState != ENCODE_STATE_OFF)
    		{
			printf("Encoding    ");

    		}


    		printf("\n");
    	
		sleep(10);
    	
	}

}


void ThreadUdpTestRcv(void)
{
	while(1)
	{
    		UdpTestReceive();
    	
		usleep(10000);
    	
	}

}


void ThreadCommanderRcv(void)
{
	while(1)
	{
    		CommanderReceiveProcess();
    	
		usleep(10000);
    	
	}

}


void ThreadMyTimer(void)
{
	MyTimerThread();
	
}





int main()
{
	int ret;


	printf(" \n");
	printf("Hello 2014.04.10--13:52  \n");


	msleep(5000);
   

	BoardInit();

	MyTimerInit();

	AppInit();

	NetInit();

	RecordInit();

	EncodeInit();

	StorageInit();


	Httpd_Init();



	ret = pthread_create(&ThreadTestId, NULL, (void *)ThreadTest, NULL);
	if(ret != 0)
	{
		printf("Create ThreadTest error! \n");
		return -1;
	}

	ret = pthread_create(&ThreadUdpTestRcvId, NULL, (void *)ThreadUdpTestRcv, NULL);
	if(ret != 0)
	{
		printf("Create ThreadUdpTestRcv error! \n");
		return -1;
	}

	ret = pthread_create(&ThreadCommanderRcvId, NULL, (void *)ThreadCommanderRcv, NULL);
	if(ret != 0)
	{
		printf("Create ThreadCommanderRcv error! \n");
		return -1;
	}

	ret = pthread_create(&ThreadMyTimerId, NULL, (void *)ThreadMyTimer, NULL);
	if(ret != 0)
	{
		printf("Create ThreadMyTimer error! \n");
		return -1;
	}


	msleep(1000);

	CommanderInit();
    
    
	while(1)
	{

		
		msleep(100);
		
	}
    
    
	return 0;
  
}
