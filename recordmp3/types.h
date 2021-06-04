/**********************************************************************
  Copyright (c), 2011-2014, SZBeiHais Rail Transit  & Technology  Co., Ltd.

  �ļ�����:  types.h
  �ļ�����: SZBeiHai ��˾mp3 encodec head file
                        
  �����б�:
  �޶���¼:
         1. ����: huangyuhui
            ����:  2011-7-11
            ����:  �����ļ�
            ��ע: mp3 ����ʵ��ͷ�ļ�

**********************************************************************/

#ifndef TYPES_H__
#define TYPES_H__

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>


#define MAX_RECORD_TIME     (30*60)      //���¼��ʱ��,1800��
#define RATE                             44100//16000       //����Ƶ��
#define SIZE                              16         //����λ��
#define CHANNELS                     2          //������Ŀ

enum{
RECORD_NONE,
RECORD_ON,
RECORD_OFF
};

extern int record_fd;      /*open(DSP)*/
extern volatile unsigned char recorder_onoff;  

extern pthread_cond_t    wait_record_mp3;            /*����¼������������*/
extern pthread_mutex_t  lock_wait_record_mp3;   /*���������Ļ�����*/

#define MPEG2_5 0
#define MPEG2   2
#define MPEG1   3

#define MODE_STEREO       0
#define MODE_MS_STEREO    1
#define MODE_DUAL_CHANNEL 2
#define MODE_MONO         3

#define LAYER_3 1
#define LAYER_2 2
#define LAYER_1 3
#define samp_per_frame  1152
#define samp_per_frame2 576
#define PI              3.14159265358979
#define HAN_SIZE        512
#define SBLIMIT         32

typedef struct {
    FILE *file;
    int  type;
    int  channels;
    int  bits;
    long samplerate;
    long total_samples;
    long length;
} wave_t;

typedef struct {
    FILE *file;
    int  type;
    int  layr;
    int  mode;
    int  bitr;
    int  psyc;
    int  emph;
    int  padding;
    long samples_per_frame;
    long bits_per_frame;
    long bits_per_slot;
    long total_frames;
    int  bitrate_index;
    int  samplerate_index;
    int crc;
    int ext;
    int mode_ext;
    int copyright;
    int original;
    int channels;
    int granules;
    int resv_limit;
} mpeg_t;

typedef struct {
    time_t start_time;

    char* infile;
    wave_t wave;

    char* outfile;
    mpeg_t mpeg;
} config_t;
extern config_t config;


#define HUFFBITS unsigned long int
#define HTN     34
#define MXOFF   250

struct huffcodetab {
  unsigned int xlen;    /*max. x-index+                         */
  unsigned int ylen;    /*max. y-index+                         */
  unsigned int linbits; /*number of linbits                     */
  unsigned int linmax;  /*max number to be stored in linbits    */
  HUFFBITS *table;      /*pointer to array[xlen][ylen]          */
  unsigned char *hlen;  /*pointer to array[xlen][ylen]          */
};

extern struct huffcodetab ht[HTN];/* global memory block                */
                                /* array of all huffcodtable headers    */
                                /* 0..31 Huffman code table 0..31       */
                                /* 32,33 count1-tables                  */

/* Side information */
typedef struct {
        unsigned part2_3_length;
        unsigned big_values;
        unsigned count1;
        unsigned global_gain;
        unsigned table_select[3];
        unsigned region0_count;
        unsigned region1_count;
        unsigned count1table_select;
        unsigned address1;
        unsigned address2;
        unsigned address3;
        int quantizerStepSize;
} gr_info;

typedef struct {
  int main_data_begin;
  struct
  {
    struct
    {
      gr_info tt;
    } ch[2];
  } gr[2];
  int resv_drain;
} L3_side_info_t;

/* function prototypes */

/* recorder.c */
//int record_init();
//void *audio_record_proc(void *arg);
void * record_file_setup_thread(void *param);



/* bitstream.c */
int open_bit_stream(char *bs_filenam);
void close_bit_stream(void);
void L3_format_bitstream(int l3_enc[2][2][samp_per_frame2],
                         L3_side_info_t  *l3_side);

/* l3loop.c */
void L3_iteration_loop(long            mdct_freq_org[2][2][samp_per_frame2],
                       L3_side_info_t *side_info,
                       int             l3_enc[2][2][samp_per_frame2],
                       int             mean_bits);


/* layer3.c */
//extern "C" 
int L3_compress(void);


/* wave.c */
//void wave_open(int raw, int mono_from_stereo);
//unsigned long int *wave_get(void);
//void wave_close(void);

/* coder.c */
void L3_window_filter_subband(unsigned long **buffer, long s[SBLIMIT],
                 int k);
void L3_mdct_sub(long sb_sample[2][3][18][SBLIMIT],
                 long mdct_freq[2][2][samp_per_frame2]);

/* utils.c */
long mul(int x, int y);
long muls(int x, int y);
long mulr(int x, int y);
long mulsr(int x, int y);

void Creat_local_socket(void);
#endif

