
//  *****************************************************//
//      MyEncode.c
//
//  *****************************************************//


#include "MyAll.h"

#include "types.h"



int EncodeState;

pthread_t EncodeThreadId;

u8 EncodeFifo[ENCODE_FIFO_SIZE];


int EncodeInFileId;
char EncodeInFileName[128];
int EncodeInFileFd;
//FILE *EncodeInFileFp; 

int EncodeInFileReadOffset;
int EncodeInFileAimOffset;


int EncodeOutFileId;
char EncodeOutFileName[128];
//int EncodeOutFileFd;
//FILE *EncodeOutFileFp; 

int EncodeOutFileWriteOffset;


int EncodeRcdCtxIndex;



config_t config;
int cutoff;




//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////




void EncodeInit(void)
{
	int ret;

	
	Encode_recordmp3_Init();


	EncodeState = ENCODE_STATE_OFF;
	
	msleep(10);

	ret = pthread_create(&EncodeThreadId, NULL, (void *)EncodeThread, NULL);
	if(ret != 0)
	{
		printf("Create EncodeThread error! \n");
	
	}

}




void EncodeThread(void)
{

	while(1)
	{

		if(EncodeState == ENCODE_STATE_OFF)
		{
			EncodeWaiting();
		}
		else if(EncodeState == ENCODE_STATE_START)
		{
			EncodeStarting();

		}
		else if(EncodeState == ENCODE_STATE_ON)
		{
			if (L3_compress() != 0)
			{
				printf("recorder encode work failed \n"); 

				EncodeState = ENCODE_STATE_OFF;
				
			}

		}
		else if(EncodeState == ENCODE_STATE_STOP)
		{	
			EncodeStoping();
			
		}


		msleep(1);

	}


}



void EncodeWaiting(void)
{
	int i;
	
	for(i=0; i<RECORD_CONTEXT_NUM; i++)
	{
		if((RecordContext[i].Status == 1))
		{
			sprintf(EncodeOutFileName, RecordContext[i].Mp3FileName);

			EncodeRcdCtxIndex = i;

			EncodeState = ENCODE_STATE_START;

			return ;

		}

	}

}



void EncodeStarting(void)
{
	printf("EncodeOutFileName:%s\n", EncodeOutFileName);

	config.outfile = EncodeOutFileName; 

	RecordContextEncodeStart(EncodeRcdCtxIndex);
		
	printf("Start to encode.\n");
	
	EncodeState = ENCODE_STATE_ON;



}


int EncodeFifoRead(u8* pbuf, int len)
{
	int ret;	

	int i;	
	s16 *pdest;	
	s16 *psrc;
	
	ret = RecordContextEncodeRead(EncodeRcdCtxIndex, EncodeFifo, (len/2));

	if(ret == 0)
	{
//		msleep(1);

	}
	else if(ret < 0)
	{
		EncodeState = ENCODE_STATE_STOP;

	}
	else
	{	
		psrc = (s16*)EncodeFifo;		
		pdest = (s16*)pbuf;

		for(i=0; i<(len/4); i++)		
		{		
			*(pdest+(2*i)) = *(psrc+ i);	
			*(pdest+(2*i)+1) = *(psrc+ i);

		}

		ret = ret * 2;

	}

	return ret;

}







void EncodeStoping(void)
{

	RecordContextEncodeFinish(EncodeRcdCtxIndex);

	msleep(10);

	EncodeState = ENCODE_STATE_OFF;

	printf("Encode stoping.\n");

}







/*
 * set_defaults:
 * -------------
 */
static void set_defaults()
{
    config.mpeg.type = MPEG1;
    config.mpeg.layr = LAYER_3;
    config.mpeg.mode = MODE_DUAL_CHANNEL;
    config.mpeg.bitr = 128;   /*bitrate,32k-320k,default:128k*/
    config.mpeg.psyc = 0;
    config.mpeg.emph = 0;
    config.mpeg.crc  = 0;
    config.mpeg.ext  = 0;
    config.mpeg.mode_ext  = 0;
    config.mpeg.copyright = 0;
    config.mpeg.original  = 1;
    config.mpeg.channels = 2;
    config.mpeg.granules = 2;
    cutoff = 418; /* 16KHz @ 44.1Ksps */
    config.wave.samplerate = 44100;

}





static int  Record_wave_file()
{

	record_fd= open("/sddisk/audio/ls.wav", O_RDONLY);
	if (record_fd < 0)
	{
		perror("Cannot open /sddisk/audio/ls.wav");
		return -1;
		
	}

	lseek(record_fd, 44, SEEK_SET);


	return 0;


}




/*
 * find_samplerate_index:
 * ----------------------
 */
static int find_samplerate_index(long freq)
{
  static long sr[4][3] = {{11025, 12000,  8000},   /* mpeg 2.5 */
                          {    0,     0,     0},   /* reserved */
                          {22050, 24000, 16000},   /* mpeg 2 */
                          {44100, 48000, 32000}};  /* mpeg 1 */
  int i, j;

  for(j=0; j<4; j++)
    for(i=0; i<3; i++)
      if((freq == sr[j][i]) && (j != 1))
      {
        config.mpeg.type = j;
        return i;
      }

  printf("Invalid samplerate\n");    //hyh
  return -1;
}

/*
 * find_bitrate_index:
 * -------------------
 */
static int find_bitrate_index(int bitr)
{
  static long br[2][15] =
    {{0, 8,16,24,32,40,48,56, 64, 80, 96,112,128,144,160},   /* mpeg 2/2.5 */
     {0,32,40,48,56,64,80,96,112,128,160,192,224,256,320}};  /* mpeg 1 */
  int i;

  for(i=1; i<15; i++)
    if(bitr==br[config.mpeg.type & 1][i]) return i;

  printf("Invalid bitrate \n");   //hyh
  return -1;
}

int set_cutoff(void)
{
  static int cutoff_tab[3][2][15] =
  {
    { /* 44.1k, 22.05k, 11.025k */
      {100,104,131,157,183,209,261,313,365,418,418,418,418,418,418}, /* stereo */
      {183,209,261,313,365,418,418,418,418,418,418,418,418,418,418}  /* mono */
    },
    { /* 48k, 24k, 12k */
      {100,104,131,157,183,209,261,313,384,384,384,384,384,384,384}, /* stereo */
      {183,209,261,313,365,384,384,384,384,384,384,384,384,384,384}  /* mono */
    },
    { /* 32k, 16k, 8k */
      {100,104,131,157,183,209,261,313,365,418,522,576,576,576,576}, /* stereo */
      {183,209,261,313,365,418,522,576,576,576,576,576,576,576,576}  /* mono */
    }
  };

  return cutoff_tab[config.mpeg.samplerate_index]
                   [config.mpeg.mode == MODE_MONO]
                   [config.mpeg.bitrate_index];
}





/*
 * check_config:
 * -------------
 */
static int  check_config()
{
	const    static char *mode_names[4]    = { "stereo", "j-stereo", "dual-ch", "mono" };
	const    static char *layer_names[4]   = { "", "III", "II", "I" };
	const    static char *version_names[4] = { "MPEG 2.5", "", "MPEG 2", "MPEG 1" };
	const    static char *psy_names[3]     = { "none", "MUSICAM", "Shine" };
	const   static char *demp_names[4]    = { "none", "50/15us", "", "CITT" };
    
    if((config.mpeg.samplerate_index = find_samplerate_index(config.wave.samplerate)) < 0)
    {
        return (-1);
    }

    if((config.mpeg.bitrate_index = find_bitrate_index(config.mpeg.bitr)) < 0)
    {
        return (-2);
    }

    cutoff = set_cutoff();

  printf("%s layer %s, %s  Psychoacoustic Model: %s\n",
           version_names[config.mpeg.type],
           layer_names[config.mpeg.layr],
           mode_names[config.mpeg.mode],
           psy_names[config.mpeg.psyc] );
  printf("Bitrate=%d kbps  ",config.mpeg.bitr );
  printf("De-emphasis: %s   %s %s\n",
           demp_names[config.mpeg.emph],
           (config.mpeg.original) ? "Original" : "",
           (config.mpeg.copyright) ? "(C)" : "" );
 
    return 0;
}






int Encode_recordmp3_Init(void)
{

	set_defaults();

	config.wave.channels      = CHANNELS;
	config.wave.samplerate    = RATE;
	config.wave.bits          = SIZE;
	config.wave.total_samples = 48000;
	config.wave.length        = 30;


	/* prints mpeg (output) configuration */
	if(0 != check_config()) 
	{
		printf("configure recorder varibles error \n");    //hyh
		return  (-1);
	}

//	if(0 != Record_wave_file())
//	{
//		printf("set record DSP failed \n");    //hyh
//		return  (-3);
//	}
	
	config.outfile = "ss.mp3";   //for test

	return 0;

}






