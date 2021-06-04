
//  *****************************************************//
//      MyBoard.c
//
//  *****************************************************//


#include "MyAll.h"



int LedsFd;

int LedsState[5];

int LedsBlink100Flag[5];
int LedsBlink500Flag[5];


int AddrFd;


void BoardInit(void)
{

	LedsInit();

//	AddrInit();


}




void LedsInit(void)
{
	int i;
	

	system("echo 376 > /sys/class/gpio/export");
	system("echo 377 > /sys/class/gpio/export");
	system("echo 378 > /sys/class/gpio/export");
	system("echo 379 > /sys/class/gpio/export");
	system("echo 383 > /sys/class/gpio/export");

	system("echo out > /sys/class/gpio/gpio376/direction");
	system("echo out > /sys/class/gpio/gpio377/direction");
	system("echo out > /sys/class/gpio/gpio378/direction");
	system("echo out > /sys/class/gpio/gpio379/direction");
	system("echo out > /sys/class/gpio/gpio383/direction");

	msleep(200);

	system("echo 1 > /sys/class/gpio/gpio376/value");
	system("echo 1 > /sys/class/gpio/gpio377/value");
	system("echo 1 > /sys/class/gpio/gpio378/value");
	system("echo 1 > /sys/class/gpio/gpio379/value");
	system("echo 1 > /sys/class/gpio/gpio383/value");

		
	msleep(100);

	system("echo 0 > /sys/class/gpio/gpio376/value");
	msleep(100);

	system("echo 0 > /sys/class/gpio/gpio377/value");
	msleep(100);
		
	system("echo 0 > /sys/class/gpio/gpio378/value");
	msleep(100);

	system("echo 0 > /sys/class/gpio/gpio379/value");
	msleep(100);

	system("echo 0 > /sys/class/gpio/gpio383/value");
	msleep(100);
	

	system("echo 1 > /sys/class/gpio/gpio376/value");
	system("echo 1 > /sys/class/gpio/gpio377/value");
	system("echo 1 > /sys/class/gpio/gpio378/value");
	system("echo 1 > /sys/class/gpio/gpio379/value");
	system("echo 1 > /sys/class/gpio/gpio383/value");

	msleep(100);

	for(i=0; i<5; i++)
	{
		LedsState[i] = 0;

		LedsBlink100Flag[i] = 0;
		LedsBlink500Flag[i] = 0;

	}

  
}



void LedControl(int num, int state)
{
	if(num == 0)
	{
		if(state > 0)
		{
			system("echo 0 > /sys/class/gpio/gpio376/value");
		}
		else
		{
			system("echo 1 > /sys/class/gpio/gpio376/value");
		}
		
	}
	else if(num == 1)
	{
		if(state > 0)
		{
			system("echo 0 > /sys/class/gpio/gpio377/value");
		}
		else
		{
			system("echo 1 > /sys/class/gpio/gpio377/value");
		}
		
	}
	else if(num == 2)
	{
		if(state > 0)
		{
			system("echo 0 > /sys/class/gpio/gpio378/value");
		}
		else
		{
			system("echo 1 > /sys/class/gpio/gpio378/value");
		}
		
	}
	else if(num == 3)
	{
		if(state > 0)
		{
			system("echo 0 > /sys/class/gpio/gpio379/value");
		}
		else
		{
			system("echo 1 > /sys/class/gpio/gpio379/value");
		}
		
	}
	else if(num == 4)
	{
		if(state > 0)
		{
			system("echo 0 > /sys/class/gpio/gpio383/value");
		}
		else
		{
			system("echo 1 > /sys/class/gpio/gpio383/value");
		}
		
	}



}



void Leds100ms(void)
{
	int i;

	for(i=0; i<5; i++)
	{
		if(LedsBlink100Flag[i] > 0)
		{
			if(LedsState[i] > 0)
			{
				LedsState[i] = 0;
				LedControl(i, 0);

			}
			else
			{
				LedsState[i] = 1;
				LedControl(i, 1);

			}

		}
		
	}

}



void Leds500ms(void)
{
	int i;

	for(i=0; i<5; i++)
	{
		if(LedsBlink500Flag[i] > 0)
		{
			if(LedsState[i] > 0)
			{
				LedsState[i] = 0;
				LedControl(i, 0);

			}
			else
			{
				LedsState[i] = 1;
				LedControl(i, 1);

			}

		}
		
	}

}



void LedRecordMicOn(void)
{
	LedsState[0] = 1;
	LedControl(0, 1);

}

void LedRecordMicOff(void)
{
	LedsState[0] = 0;
	LedControl(0, 0);

}

void LedRecordMicBlink100On(void)
{
	LedsBlink100Flag[0] = 1;

}

void LedRecordMicBlink100Off(void)
{
	LedsBlink100Flag[0] = 0;
	LedRecordMicOff();

}

void LedRecordMicBlink500On(void)
{
	LedsBlink500Flag[0] = 1;

}

void LedRecordMicBlink500Off(void)
{
	LedsBlink500Flag[0] = 0;
	LedRecordMicOff();

}




void LedRecordTalkOn(void)
{
	LedsState[1] = 1;
	LedControl(1, 1);

}

void LedRecordTalkOff(void)
{
	LedsState[1] = 0;
	LedControl(1, 0);

}

void LedRecordTalkBlink100On(void)
{
	LedsBlink100Flag[1] = 1;

}

void LedRecordTalkBlink100Off(void)
{
	LedsBlink100Flag[1] = 0;
	LedRecordTalkOff();

}

void LedRecordTalkBlink500On(void)
{
	LedsBlink500Flag[1] = 1;

}

void LedRecordTalkBlink500Off(void)
{
	LedsBlink500Flag[1] = 0;
	LedRecordTalkOff();

}



void LedSpace3On(void)
{
	LedsState[2] = 1;
	LedsBlink100Flag[2] = 0;
	LedsBlink500Flag[2] = 0;
	LedControl(2, 1);

}

void LedSpace3Off(void)
{
	LedsState[2] = 0;
	LedsBlink100Flag[2] = 0;
	LedsBlink500Flag[2] = 0;
	LedControl(2, 0);

}

void LedSpace3Blink100On(void)
{
	LedsBlink100Flag[2] = 1;

}

void LedSpace3Blink100Off(void)
{
	LedsBlink100Flag[2] = 0;

}

void LedSpace3Blink500On(void)
{
	LedsBlink500Flag[2] = 1;

}

void LedSpace3Blink500Off(void)
{
	LedsBlink500Flag[2] = 0;

}


void LedSpace2On(void)
{
	LedsState[3] = 1;
	LedsBlink100Flag[3] = 0;
	LedsBlink500Flag[3] = 0;
	LedControl(3, 1);

}

void LedSpace2Off(void)
{
	LedsState[3] = 0;
	LedsBlink100Flag[3] = 0;
	LedsBlink500Flag[3] = 0;
	LedControl(3, 0);

}

void LedSpace2Blink100On(void)
{
	LedsBlink100Flag[3] = 1;

}

void LedSpace2Blink100Off(void)
{
	LedsBlink100Flag[3] = 0;

}

void LedSpace2Blink500On(void)
{
	LedsBlink500Flag[3] = 1;

}

void LedSpace2Blink500Off(void)
{
	LedsBlink500Flag[3] = 0;

}


void LedSpace1On(void)
{
	LedsState[4] = 1;
	LedsBlink100Flag[4] = 0;
	LedsBlink500Flag[4] = 0;
	LedControl(4, 1);

}

void LedSpace1Off(void)
{
	LedsState[4] = 0;
	LedsBlink100Flag[4] = 0;
	LedsBlink500Flag[4] = 0;
	LedControl(4, 0);

}


void LedSpace1Blink100On(void)
{
	LedsBlink100Flag[4] = 1;

}

void LedSpace1Blink100Off(void)
{
	LedsBlink100Flag[4] = 0;

}

void LedSpace1Blink500On(void)
{
	LedsBlink500Flag[4] = 1;

}

void LedSpace1Blink500Off(void)
{
	LedsBlink500Flag[4] = 0;

}




void AddrInit(void)
{
	int addr;


	
	AddrFd = open(IO_DEVICE_NAME, 0 );

	if(AddrFd < 0)
	{
		printf("Error: Open Addr Gpio Error. \n");

	}


	printf("Addr Gpio open. \n");

}


int AddrRead(void)
{
	int addr;

	int ret;
	int temp;


	ret = ioctl(AddrFd, 0x0d, &temp);
	if(ret < 0)
	{
		printf("Addr read error. \n");
	}

	if(temp > 0)
	{
		addr = 1;
	}
	else
	{
		addr = 2;
	}

	return addr;	

}



















