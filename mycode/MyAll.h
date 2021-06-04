
//  ******************************************************************************
//  * @file    MyAll.h
  
//  ******************************************************************************
  

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MYALL_H
#define __MYALL_H


//#include "MyAll.h"


#include <sys/types.h> 
#include <sys/stat.h> 
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>							
#include <netinet/in.h>	
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h> 
#include<sys/ioctl.h>


#include <pthread.h>

#include <sys/ipc.h>	
#include <semaphore.h>

#include<dirent.h>



#include "main.h"
#include "MyTimer.h"
#include "MyApp.h"
#include "MyMiscFunc.h"
#include "MyNet.h"
#include "MyUdp.h"
#include "MyProtocol.h"
#include "MyCommander.h"
#include "MyRecord.h"
#include "MyEncode.h"
#include "MyBoard.h"
#include "MyStorage.h"





#endif /* __MYALL_H */

