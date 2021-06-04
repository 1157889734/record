
//  *****************************************************//
//  MyAudio.h
  
//  *****************************************************//
  

#ifndef __MYAUDIO_H
#define __MYAUDIO_H


#include "MyAll.h"


#define  AUDIO_FIFO_BUFS			16
#define  AUDIO_FIFO_BUFSIZE		1024


typedef struct 
{	
	sem_t semaphore;

	u8 Buf[AUDIO_FIFO_BUFS][AUDIO_FIFO_BUFSIZE];
	int ProduceIndex;
	int ConsumeIndex;

	int BufsCanWrite;
	int BufsCanRead;

	int Timer;
	
}AudioFifoTypeDef;



extern AudioFifoTypeDef AudioFifoOutLeft;
extern AudioFifoTypeDef AudioFifoOutRight;

extern AudioFifoTypeDef AudioFifoInLeft;
extern AudioFifoTypeDef AudioFifoInRight;



int AudioInit(void);

void AudioOutThread(void);

void AudioInThread(void);


void AudioFifoInit(AudioFifoTypeDef* paf);
void AudioFifoClear(AudioFifoTypeDef* paf);
int AudioFifoWrite(AudioFifoTypeDef* paf, u8* pbuf);
int AudioFifoRead(AudioFifoTypeDef* paf, u8* pbuf);
int AudioFifoGetWriteFlag(AudioFifoTypeDef* paf);
int AudioFifoGetReadFlag(AudioFifoTypeDef* paf);






#endif 




