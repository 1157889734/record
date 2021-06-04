
//  *****************************************************//
//      MyApp.c
//
//  *****************************************************//


#include "MyAll.h"




pthread_t AudioWriteThreadId;
pthread_t AudioReadThreadId;

int AudioFd;

AudioFifoTypeDef AudioFifoOutLeft;
AudioFifoTypeDef AudioFifoOutRight;

AudioFifoTypeDef AudioFifoInLeft;
AudioFifoTypeDef AudioFifoInRight;



int AudioInit(void)
{
	int arg;
	int ret;


	AudioFifoInit(&AudioFifoOutLeft);
	AudioFifoInit(&AudioFifoOutRight);

	AudioFifoInit(&AudioFifoInLeft);
	AudioFifoInit(&AudioFifoInLeft);
	
		
	AudioFd = open("/dev/dsp", (O_WRONLY|O_SYNC));
	if(AudioFd < 0)
	{
		printf("Can't open /dev/dsp ! \n");
		return -1;
	}

	arg = 0;
	//ioctl(fd,SNDCTL_DSP_RESET,&arg);
	arg = 0;
	//ioctl(fd,SNDCTL_DSP_SYNC,&arg);

	arg = 44100;
	if(ioctl(AudioFd,SNDCTL_DSP_SPEED,&arg)<0) 	//sample rate
	{
		printf("AudioInit: sample_rate set failed.\n");
		return -1;
	}
	if(arg != 44100)
	{
		printf("AudioInit: sample_rate is %d\n", arg);
		return -1;
	}


	arg = AFMT_S16_LE;
	if (ioctl(AudioFd,SNDCTL_DSP_SETFMT,&arg)<0)	//bits format
	{
		printf("AudioInit: bits_format set failed.\n");
		return -1;
	}
	if(arg != AFMT_S16_LE)
	{
		printf("AudioInit: bits_format is %d\n", arg);
		return -1;
	}

	arg = 2;
	if((ioctl(AudioFd,SNDCTL_DSP_CHANNELS,&arg))<0)	//channels
	{
		printf("AudioInit: channels set failed.\n");
		return -1;
	}
	if(arg != 2)
	{
		printf("AudioInit: channels is %d\n", arg);
		return -1;
	}


//	ret = pthread_create(&AudioWriteThreadId, NULL, (void *)AudioOutThread, NULL);
	if(ret != 0)
	{
//		printf("Create AudioWriteThread error! \n");
//		return -2;
	}

	ret = pthread_create(&AudioReadThreadId, NULL, (void *)AudioInThread, NULL);
	if(ret != 0)
	{
		printf("Create AudioReadThread error! \n");
		return -3;
	}

	return 0;
  
}


void AudioOutThread(void)
{
	u8 buf[2048];
	u8 buf_left[1024];
	u8 buf_right[1024];
	
	s16* pbuf;
	s16* pbuf_left;
	s16* pbuf_right;

	int i;

	int ret;
	

	pbuf = (s16*)buf;
	pbuf_left = (s16*)(buf_left);
	pbuf_right = (s16*)(buf_right);

	while(1)
	{
		AudioFifoRead(&AudioFifoOutLeft, buf_left);
		AudioFifoRead(&AudioFifoOutRight, buf_right);
	
		for(i=0; i<512; i++)
		{
			*(pbuf + 2*i) = *(pbuf_left + i);
			*(pbuf + 2*i+i) = *(pbuf_right + i);

		}

		ret =write(AudioFd, buf,sizeof(buf));
		while(ret < 0)
		{
			msleep(1);
			ret =write(AudioFd, buf,sizeof(buf));
		}

		if(ret < 0)
		{
			msleep(1);
		}
		else if(ret < sizeof(buf))
		{
			msleep(1);
			printf("AudioOutThread: error.\n");
		}
		else
		{

		}
		
		msleep(1);
		
	}

}




void AudioInThread(void)
{

	while(1)
	{
			

		msleep(100);

		
	}



}



void AudioFifoInit(AudioFifoTypeDef* paf)
{
	sem_init(&(paf->semaphore), 0, 1);

 	memset((paf->Buf), 0, (sizeof(paf->Buf)));

	paf->ProduceIndex = 0;
	paf->ConsumeIndex = AUDIO_FIFO_BUFS/2;

	paf->BufsCanWrite = AUDIO_FIFO_BUFS/2;
	paf->BufsCanRead = AUDIO_FIFO_BUFS/2;

	paf->Timer = 0;

}

void AudioFifoClear(AudioFifoTypeDef* paf)
{
 	memset((paf->Buf), 0, (sizeof(paf->Buf)));

}


int AudioFifoWrite(AudioFifoTypeDef* paf, u8* pbuf)
{
	int result;
	u8* dest;

	result = 0;

	sem_wait(&(paf->semaphore));

	if(paf->BufsCanWrite < 2)
	{
		paf->ProduceIndex = IndexAddn(paf->ConsumeIndex, AUDIO_FIFO_BUFS, AUDIO_FIFO_BUFS/2);
		paf->BufsCanWrite = AUDIO_FIFO_BUFS/2;
		paf->BufsCanRead = AUDIO_FIFO_BUFS/2;
	
		result = -1;
	}
//	else
	{
//		Debuger = (u32)(&(paf->Buf[paf->ProduceIndex][0]));
		
//		printf("AudioFifoWrite: Debuger1 = %d. \n", Debuger);

//		Debuger = (u32)(paf->Buf[paf->ProduceIndex]);
		
//		printf("AudioFifoWrite: Debuger2 = %d. \n", Debuger);
	

		dest = paf->Buf[paf->ProduceIndex];
		
		memcpy(dest, pbuf, AUDIO_FIFO_BUFSIZE);

		paf->ProduceIndex = IndexAdd1(paf->ProduceIndex, AUDIO_FIFO_BUFS);
		
		(paf->BufsCanWrite)--;

		(paf->BufsCanRead)++;
		if((paf->BufsCanRead) >= AUDIO_FIFO_BUFS)
		{
			(paf->BufsCanRead) = AUDIO_FIFO_BUFS;
		}

		paf->Timer = 8;

	}

	sem_post(&(paf->semaphore));

	return result;
	
}


int AudioFifoRead(AudioFifoTypeDef* paf, u8* pbuf)
{
	int result;
	u8* src;

	result = 0;

	sem_wait(&(paf->semaphore));

	if(paf->BufsCanRead < 2)
	{
	
		result = -1;
	}
//	else
	{
		src = paf->Buf[paf->ConsumeIndex];
		
		memcpy(pbuf, src, AUDIO_FIFO_BUFSIZE);

		paf->ConsumeIndex = IndexAdd1(paf->ConsumeIndex, AUDIO_FIFO_BUFS);
		
		(paf->BufsCanWrite)++;
		if((paf->BufsCanWrite) >= AUDIO_FIFO_BUFS)
		{
			(paf->BufsCanWrite) = AUDIO_FIFO_BUFS;
		}

		(paf->BufsCanRead)--;

	}


	if(paf->Timer > 0)
	{
		paf->Timer --;
	}
	else if(paf->Timer == 0)
	{
		paf->Timer --;
		AudioFifoClear(paf);
	}
	else
	{
		paf->Timer = -1;
	}

	sem_post(&(paf->semaphore));

	return result;
	
}


int AudioFifoGetWriteFlag(AudioFifoTypeDef* paf)
{

	return 0;
}


int AudioFifoGetReadFlag(AudioFifoTypeDef* paf)
{

	return 0;
}






