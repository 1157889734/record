
//  *****************************************************//
//      MyTimer.c
//
//  *****************************************************//


#include "MyAll.h"

#include <sys/time.h>



struct timeval TimeValue;

struct timeval TimeValueLast;

u32 Time_s;
u32 Time_ms;

u32 TimeLast_s;
u32 TimeLast_ms;



int MyTimer;


int MyTmCnt10;
int MyTmCnt20;
int MyTmCnt50;
int MyTmCnt100;
int MyTmCnt500;
int MyTmCnt1000;
int MyTmCnt2000;



void MyTimerInit(void)
{
	MyTimer = 0;

//	gettimeofday(&TimeValueLast, NULL);

//	printf("TimeValueLast %u:%u\n",(u32)TimeValueLast.tv_sec,(u32)TimeValueLast.tv_usec);

//	TimeLast_s = (u32)(TimeValueLast.tv_sec);
//	TimeLast_ms = (u32)(TimeValueLast.tv_usec / 1000);

//	printf("TimeLast_s = %u\n", TimeLast_s);
//	printf("TimeLast_ms = %u\n", TimeLast_ms);


}



void MyTimerThread(void)
{
	
	

	while(1)
	{
//		gettimeofday(&TimeValue, NULL);

//		Time_s = (u32)(TimeValue.tv_sec);
//		Time_ms = (u32)(TimeValue.tv_usec / 1000);


//		if(Time_s < TimeLast_s)
//		{
//			gettimeofday(&TimeValueLast, NULL);

//		}
//		else if(((Time_s-TimeLast_s)*1000 + Time_ms -TimeLast_ms) >= 10)
		{
//			TimeValueLast = TimeValue;

//			TimeLast_s = (u32)(TimeValueLast.tv_sec);
//			TimeLast_ms = (u32)(TimeValueLast.tv_usec / 1000);
			

			MyTimer++;

			MyTmCnt10++;
			MyTmCnt20++;
			MyTmCnt50++;
			MyTmCnt100++;
			MyTmCnt500++;
			MyTmCnt1000++;
			MyTmCnt2000++;
		

			if(MyTmCnt10 >= 1)
			{
				MyTmCnt10 = 0;
				MyTimer10();
			}

			if(MyTmCnt20 >= 2)
			{
				MyTmCnt20 = 2;
				MyTimer20();
			}
		
			if(MyTmCnt50 >= 5)
			{
				MyTmCnt50 = 0;
				MyTimer50();
			}

			if(MyTmCnt100 >= 10)
			{
				MyTmCnt100 = 0;
				MyTimer100();
			}

			if(MyTmCnt500 >= 50)
			{
				MyTmCnt500 = 0;
				MyTimer500();
			}

			if(MyTmCnt1000 >= 100)
			{
				MyTmCnt1000 = 0;
				MyTimer1000();
			}

			if(MyTmCnt2000 >= 200)
			{
				MyTmCnt2000 = 0;
				MyTimer2000();
			}


		}


		usleep(9000);

	}


}




void MyTimer10(void)
{
	
	


}

void MyTimer20(void)
{
	
	

}

void MyTimer50(void)
{



}

void MyTimer100(void)
{

	App100ms();

	Leds100ms();

	RecordStream100ms();

	Record100ms();

}

void MyTimer500(void)
{
	App500ms();

	Leds500ms();


}


void MyTimer1000(void)
{
//	printf("mytimer = %d\n", MyTimer);




}


void MyTimer2000(void)
{

	

}








