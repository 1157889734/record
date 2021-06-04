
//  *****************************************************//
//  MyEncode.h
  
//  *****************************************************//
  

#ifndef __MYECODE_H
#define __MYECODE_H


#include "MyAll.h"



#define SAMPLES_PER_FRAME  		1152

#define ENCODE_FIFO_SIZE  		92160*2		//1152 * 80 = 1024 * 90

//#define ENCODE_FIFO_SIZE  		1179648*2		//1152 *1024

//#define ENCODE_FIFO_SIZE  		1179648*2		//1152 *1024



#define ENCODE_INBUF_SIZE  		9216			//1152 * 8 = 1024 * 9
#define ENCODE_OUTBUF_SIZE  		9216			//suppose to = in buf size

//#define ENCODE_INBUF_SIZE  	1152			
//#define ENCODE_OUTBUF_SIZE  	1152			//suppose to = in buf size


#define  ENCODE_STATE_OFF		0
#define  ENCODE_STATE_ON			1
#define  ENCODE_STATE_START		2		
#define  ENCODE_STATE_STOP		3



extern int EncodeState;




void EncodeInit(void);

void EncodeThread(void);


int EncodeStart(char* out_file_name);


void EncodeWaiting(void);
void EncodeStarting(void);

void EncodeStoping(void);


int EncodeFifoRead(u8* pbuf, int len);



int Encode_recordmp3_Init(void);



#endif 




