//*******************************************************//
//						MyMiscFunc.h								//
//*******************************************************//

#ifndef _MYMISCFUNC_H
#define  _MYMISCFUNC_H

#include "MyAll.h"


void Delay(u32 nCount);

char bcd_to_data(char bcd);

char data_to_bcd(char da);

int shifang(int n);
int shiliufang(int n);



int IndexAddn(int index, int max, int n);

int IndexAdd1(int index, int max);



int LineToWords(char *str, int str_len, char* words, int word_maxnum, int word_maxsize);

int StrSplitFrontChar(char *scr_str, char split_char, char *dest_str);

int StrSplitBehindChar(char *scr_str, char split_char, char *dest_str);

int StrToInt(char *str);
int IntToStr(char *outstr, int inda, int len, char headchar);

u32 Ip4ToIp32(u8* ip4);
u32 Ip44ToIp32(u8 ip1, u8 ip2, u8 ip3, u8 ip4);


void msleep(int ms);




#endif

