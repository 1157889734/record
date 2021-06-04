//*******************************************************//
//						MyStorage.h								//
//*******************************************************//

#ifndef _MYSTORAGE_H
#define  _MYSTORAGE_H

#include "MyAll.h"


extern int StorageSpaceTotal;		//unit is KB (Kilo Byte);
extern int StorageSpaceFree;		//unit is KB (Kilo Byte);
extern int StorageSpaceFreePercent;



void StorageInit(void);

void StorageThread(void);


void StorageGetSpaceInformation(void);


void StorageReadDevId(void);

void StorageReadLocalIp(void);


int StorageDeleteEarliestDir(void);



#endif


