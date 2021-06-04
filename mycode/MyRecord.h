
//  *****************************************************//
//  MyRecord.h
  
//  *****************************************************//
  

#ifndef __MYRECORD_H
#define __MYRECORD_H


#include "MyAll.h"



#define  RECORD_STREAM_NUM			8
#define  RECORD_STREAM_BUF_NUM		256
#define  RECORD_STREAM_BUF_SIZE		1024


#define  RECORD_TYPE_NULL		0
#define  RECORD_TYPE_SPEAK		1
#define  RECORD_TYPE_TALK_DD		2
#define  RECORD_TYPE_TALK_DC		3


#define  RECORD_STATE_OFF		0
#define  RECORD_STATE_ON			1
#define  RECORD_STATE_START		2
#define  RECORD_STATE_STOP		3



#define RECORD_CONTEXT_NUM	8



/////////////////////////////////////////////////////////


typedef struct 
{
	int State;

	int Timer;

	int Type;

	u8 SourceType;
	u8 SourceId;
	
	u8 Buf[RECORD_STREAM_BUF_NUM * RECORD_STREAM_BUF_SIZE];
	int BufProduceIndex;
	int BufConsumeIndex;
	int BufsCanRead;

	
}RecordStreamTypeDef;



typedef struct 
{
	int Status;

	char WavFileName[128];
	int WavFileFd;
	int WavFileWriteOffset;
	int WavFileReadOffset;
	int WavFileStatus;
	
	char Mp3FileName[128];
	int Mp3FileStatus;
	
	
}RecordContextTypeDef;







/////////////////////////////////////////////////////////


extern int RecordState;

extern int RecordType;

extern RecordContextTypeDef RecordContext[RECORD_CONTEXT_NUM];



/////////////////////////////////////////////////////////



void RecordInit(void);


void RecordThread(void);


void RecordStarting(void);

void RecordWorking(void);

void RecordStoping(void);

void RecordWaiting(void);



void RecordBufMix(u8* mix_buf, u8* src_buf);

void RecordBufSave(u8* mix_buf);

void Record100ms(void);


int RecordInput(u8* pbuf, int type, u8 src_type, u8 src_id);

void RecordStreamInit(void);


void RecordStream100ms(void);




void RecordContextInit(void);
int RecordContextStart(char* p_out_name);
int RecordContextWrite(int index, u8* pbuf, int len);
void RecordContextWriteFinish(int index);


void RecordContextEncodeStart(int index);
int RecordContextEncodeRead(int index, u8* pbuf, int len);
void RecordContextEncodeFinish(int index);




#endif 




