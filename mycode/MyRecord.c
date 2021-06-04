
//  *****************************************************//
//      MyRecord.c
//
//  *****************************************************//


#include "MyAll.h"


int RecordState;

int RecordType;

char RecordFileName[128];


pthread_t RecordThreadId;


RecordStreamTypeDef RecordStream[RECORD_STREAM_NUM];


int RecordLinkFlag;

int RecordTempFd;



RecordContextTypeDef RecordContext[RECORD_CONTEXT_NUM];
int RecordContextCurrentIndex;



sem_t semRecordStream;
sem_t semRecordContextFd;



void RecordInit(void)
{
	int ret;

	RecordStreamInit();

	RecordContextInit();

	RecordLinkFlag = 0;


	msleep(10);

	ret = pthread_create(&RecordThreadId, NULL, (void *)RecordThread, NULL);
	if(ret != 0)
	{
		printf("Create ThreadRecord error! \n");
		return;
	}


	RecordTempFd = open("/sddisk/system/data/temp.wav", O_RDWR, 0666);
	if(RecordTempFd < 0)
	{
		printf("Open temp wav error.\n");

	}
	else
	{
		lseek(RecordTempFd, 44, SEEK_SET);

	}

	sem_init(&semRecordStream, 0, 1);
	sem_init(&semRecordContextFd, 0, 1);


}



void RecordThread(void)
{	


	while(1)
	{
		
		switch(RecordState)
		{
			case RECORD_STATE_START:
			{
				RecordStarting();

				msleep(1);
				
				break;
				
			}
			case RECORD_STATE_ON:
			{
				RecordWorking();
				
				break;
				
			}
			case RECORD_STATE_STOP:
			{
				RecordStoping();

				msleep(1);
				
				break;
				
			}
			case RECORD_STATE_OFF:
			{
				RecordWaiting();

				msleep(10);

				
				break;
				
			}
			
			default :
			{
				msleep(10);

			}	
			
		}
		

		msleep(1);

		
	}


}


void RecordStarting(void)
{
	DIR *dirptr = NULL; 
	
	struct tm *timenow;
	time_t now;

	char main_dir[] = "/sddisk/";
//	char main_dir[] = "record/";

	char target_dir[64];

	char type_str[4];

	char out_name[128];

	
	

	printf("Start to record.\n");


/*

	RecordTempFd = open("/sddisk/system/data/temp.wav", O_RDWR, 0666);
	if(RecordTempFd < 0)
	{
		printf("Open temp wav error.\n");

	}
	else
	{
		lseek(RecordTempFd, 44, SEEK_SET);

	}
	

*/


	time(&now);
	timenow = localtime(&now);


	sprintf(target_dir, "%s%04d-%02d-%02d/", main_dir, (timenow->tm_year+1900), (timenow->tm_mon+1), timenow->tm_mday);

	if((dirptr = opendir(target_dir)) == NULL)
	{
		printf("Create dir %s.\n", target_dir);

		if(mkdir(target_dir, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH) < 0)
		{
			printf("mkdir failed\n");
			
			return;
				
		}

	}
	else
	{
		closedir(dirptr);

	}

	if(RecordType == RECORD_TYPE_SPEAK)
	{
		sprintf(type_str, "a");

	}
	else if(RecordType == RECORD_TYPE_TALK_DD)
	{
		sprintf(type_str, "b");

	}
	else if(RecordType == RECORD_TYPE_TALK_DC)
	{
		sprintf(type_str, "c");

	}
	else
	{
		sprintf(type_str, "n");

	}
	
	sprintf(out_name, "%s%04d%02d%02d%02d%02d%02d_%s.mp3", target_dir, (timenow->tm_year+1900), (timenow->tm_mon+1), timenow->tm_mday, timenow->tm_hour, timenow->tm_min, timenow->tm_sec, type_str);

//	sprintf(RecordFileName, "%s%04d%02d%02d%02d%02d%02d.mp3", target_dir, (timenow->tm_year+1900), (timenow->tm_mon+1), timenow->tm_mday, timenow->tm_hour, timenow->tm_min, timenow->tm_sec);
	

	printf(out_name);
	printf("\n");

//	EncodeStart(RecordFileName);

//	msleep(100);


	RecordContextStart(out_name);



	RecordState = RECORD_STATE_ON;


}



void RecordWorking(void)
{

	int i;

	RecordStreamTypeDef * p_stream_on[RECORD_STREAM_NUM];
	int stream_on_index;
	int stream_on_num;
	int stream_ok_num;

	u8 mix_buf[RECORD_STREAM_BUF_SIZE];

	
	stream_on_index = 0;
	stream_on_num = 0;
	stream_ok_num = 0;

	for(i=0; i<RECORD_STREAM_NUM; i++)
	{
		if(RecordStream[i].State > 0)
		{
			p_stream_on[stream_on_index] = &RecordStream[i];

			stream_on_index++;
			stream_on_num++;

			if(RecordStream[i].BufsCanRead > 0)
			{
				stream_ok_num++;

			}

		}

	}


	if((stream_on_num > 0) && (stream_ok_num == stream_on_num))
	{

		memset(mix_buf, 0, RECORD_STREAM_BUF_SIZE);

		sem_wait(&semRecordStream);
	
		for(i=0; i<stream_on_num; i++)
		{
			RecordBufMix(mix_buf, &(p_stream_on[i]->Buf[(p_stream_on[i]->BufConsumeIndex)*1024]));

			p_stream_on[i]->BufConsumeIndex = IndexAdd1(p_stream_on[i]->BufConsumeIndex, RECORD_STREAM_BUF_NUM);
			p_stream_on[i]->BufsCanRead--;

//			printf("bufs can read = %d\n", p_stream_on[i]->BufsCanRead);

		}

		sem_post(&semRecordStream);

//		RecordBufSave(mix_buf);

		RecordContextWrite(RecordContextCurrentIndex, mix_buf, 1024);
		

//		printf("stream on num = %d\n", stream_on_num);
//		printf("bufs can read = %d\n", p_stream_on[i]->BufsCanRead);

		RecordLinkFlag = 20;

	}
	else
	{
		msleep(1);

	}
	


}


void RecordStoping(void)
{
			
//	msleep(20);		

//	close(RecordTempFd);

	msleep(10);	

	RecordState = RECORD_STATE_OFF;

	RecordContextWriteFinish(RecordContextCurrentIndex);

	RecordStreamInit();
			
	printf("Record stoped.\n");


}


void RecordWaiting(void)
{
	int i;

	int stream_on_num;

	int rcd_type;
	

	stream_on_num = 0;

	rcd_type = 0;
	

	for(i=0; i<RECORD_STREAM_NUM; i++)
	{
		if(RecordStream[i].State > 0)
		{
			stream_on_num++;

			rcd_type = RecordStream[i].Type;

		}

	}

	if(stream_on_num > 0)
	{
		RecordState = RECORD_STATE_START;

		RecordType = rcd_type;

	}


}





void RecordBufMix(u8* mix_buf, u8* src_buf)
{
	s16* pdest;
	s16* psrc;

	int i;


	pdest = (s16*)(mix_buf);

	psrc = (s16*)(src_buf);

	for(i=0; i<512; i++)
	{
		*(pdest + i) = *(pdest + i) + ((*(psrc + i)));

	}


}


void RecordBufSave(u8* mix_buf)
{
	int ret;

	ret = write(RecordTempFd, mix_buf, 1024);
	if(ret < 1024)
	{
		printf("Record temp wav write err.\n");

	}


}



void Record100ms(void)
{

	if(RecordState == RECORD_STATE_ON)
	{
		RecordLinkFlag --;

		if(RecordLinkFlag <= 0)
		{
			RecordLinkFlag = 0;

			RecordState = RECORD_STATE_STOP;


			printf("Record link is time out. %d\n", RecordLinkFlag);


		}

	}


}
   


int RecordInput(u8* pbuf, int type, u8 src_type, u8 src_id)
{
	int i;	


	if(RecordState == RECORD_STATE_STOP)
	{
		printf("RecordSream Input error 1.");
	
		return -1;
		
	}

	if(EncodeState == ENCODE_STATE_STOP)
	{
		printf("RecordSream Input error 2.");

		return -2;

	}
	
	
	for(i=0; i<RECORD_STREAM_NUM; i++)
	{
		if(RecordStream[i].State == 1)
		{
			if((type == RecordStream[i].Type) && (src_type == RecordStream[i].SourceType) && (src_id== RecordStream[i].SourceId))
			{
				sem_wait(&semRecordStream);
			
				memcpy(&(RecordStream[i].Buf[(RecordStream[i].BufProduceIndex)*1024]), pbuf, 1024);
		
				RecordStream[i].BufProduceIndex = IndexAdd1(RecordStream[i].BufProduceIndex, RECORD_STREAM_BUF_NUM);
				RecordStream[i].BufsCanRead++;
				if(RecordStream[i].BufsCanRead > RECORD_STREAM_BUF_NUM)
				{
					RecordStream[i].BufsCanRead = RECORD_STREAM_BUF_NUM;

					printf("RecordSream over write.");

				}

				RecordStream[i].Timer = 0;

				sem_post(&semRecordStream);

	//			printf("input1\n");

				return 0;
				
			}

		}
		
	}

	
	for(i=0; i<RECORD_STREAM_NUM; i++)
	{
		if(RecordStream[i].State == 0)
		{
			RecordStream[i].State = 1;
			
			RecordStream[i].Type = type;
			RecordStream[i].SourceType = src_type;
			RecordStream[i].SourceId = src_id;

			memcpy(&(RecordStream[i].Buf[(RecordStream[i].BufProduceIndex)*1024]), pbuf, 1024);
		
			RecordStream[i].BufProduceIndex = IndexAdd1(RecordStream[i].BufProduceIndex, RECORD_STREAM_BUF_NUM);
			RecordStream[i].BufsCanRead = 1;

			RecordStream[i].Timer = 0;

			printf("RecordStream %d is start\n", i);

			return 0;
			
		}

	}


	printf("RecordSream Input error.");
		

	return -1;


}



void RecordStreamInit(void)
{
	int i;

	for(i=0; i<RECORD_STREAM_NUM; i++)
	{
		RecordStream[i].State = 0;
		RecordStream[i].Timer = 0;
		RecordStream[i].BufProduceIndex = 0;
		RecordStream[i].BufConsumeIndex = 0;
		RecordStream[i].BufsCanRead = 0;

	}


}


void RecordStream100ms(void)
{
	int i;

	for(i=0; i<RECORD_STREAM_NUM; i++)
	{
		if(RecordStream[i].State == 1)
		{
			RecordStream[i].Timer++;

			if(RecordStream[i].Timer > 10)
			{
				RecordStream[i].State = 0;
				RecordStream[i].Timer = 0;
				RecordStream[i].BufProduceIndex = 0;
				RecordStream[i].BufConsumeIndex = 0;
				RecordStream[i].BufsCanRead = 0;

				printf("RecordStream %d is stop\n", i);

			}

		}

	}


}









void RecordContextInit(void)
{
	int i;


	for(i=0; i<RECORD_CONTEXT_NUM; i++)
	{
		RecordContext[i].Status = 0;

		sprintf(RecordContext[i].WavFileName, "/sddisk/system/data/record_%d.wav", i);

		RecordContext[i].WavFileFd= open(RecordContext[i].WavFileName, O_RDWR, 0666);
		if(RecordContext[i].WavFileFd < 0)
		{
			printf("Open RecordContext[i].WavFileName wav error.\n");

		}
		else
		{
			lseek(RecordContext[i].WavFileFd, 44, SEEK_SET);

		}

		RecordContext[i].WavFileWriteOffset = 44;
		RecordContext[i].WavFileReadOffset = 44;
		

	}


}



int RecordContextStart(char* p_out_name)
{
	int i;
	
	for(i=0; i<RECORD_CONTEXT_NUM; i++)
	{
		if(RecordContext[i].Status == 0)
		{
			sprintf(RecordContext[i].Mp3FileName, p_out_name);

			RecordContext[i].WavFileWriteOffset = 44;
			RecordContext[i].WavFileReadOffset = 44;

			RecordContext[i].Status = 1;
			RecordContext[i].WavFileStatus = 1;

			RecordContextCurrentIndex = i;

			return 0;

		}

	}

	return -1;

}



int RecordContextWrite(int index, u8* pbuf, int len)
{
	int ret;

	sem_wait(&semRecordContextFd);

	lseek(RecordContext[index].WavFileFd, RecordContext[index].WavFileWriteOffset, SEEK_SET);
	ret = write(RecordContext[index].WavFileFd, pbuf, len);
	if(ret < len)
	{
		printf("RecordContext wav write err.\n");

	}
	else
	{
		RecordContext[index].WavFileWriteOffset += len;

	}

	sem_post(&semRecordContextFd);

	return ret;

}



void RecordContextWriteFinish(int index)
{
	RecordContext[index].WavFileStatus = 0;

}


void RecordContextEncodeStart(int index)
{
	RecordContext[index].Mp3FileStatus = 1;

}



int RecordContextEncodeRead(int index, u8* pbuf, int len)
{
	int ret;

	RecordContextTypeDef* prc;

	prc = &(RecordContext[index]);

	if(prc->WavFileWriteOffset > (prc->WavFileReadOffset + len))
	{
		sem_wait(&semRecordContextFd);
	
		lseek(prc->WavFileFd, prc->WavFileReadOffset, SEEK_SET);
		ret = read(prc->WavFileFd, pbuf, len);
		if(ret < 0)
		{
			printf("RecordContext wav read err.\n");

		}

		prc->WavFileReadOffset += len;

		sem_post(&semRecordContextFd);

		return ret;

	}
	else if(prc->WavFileStatus > 0)
	{
		return 0;


	}
	else
	{
		return -1;

	}


}

void RecordContextEncodeFinish(int index)
{
	RecordContext[index].Status = 0;
	RecordContext[index].Mp3FileStatus = 0;

}






