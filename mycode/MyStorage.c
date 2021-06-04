
//*****************************************************//
//		MyStorage.c
//*****************************************************//

#include "MyAll.h"

#include <sys/statfs.h>   



pthread_t StorageThreadId;


int StorageSpaceTotal;		//unit is KB (Kilo Byte);
int StorageSpaceFree;		//unit is KB (Kilo Byte);
int StorageSpaceFreePercent;




void StorageInit(void)
{
	int ret;
	

	StorageGetSpaceInformation();

	printf("SD Card total space is: %d KB.\n", StorageSpaceTotal);
	printf("SD Card free space is: %d KB.\n", StorageSpaceFree);
	printf("SD Card free space percent is: %d%%.\n", StorageSpaceFreePercent);



	ret = pthread_create(&StorageThreadId, NULL, (void *)StorageThread, NULL);
	if(ret != 0)
	{
		printf("Create StorageThread error! \n");
		return;
	}

	

}


void StorageThread(void)
{

	while(1)
	{
		msleep(500);

		StorageGetSpaceInformation();

		if(StorageSpaceFreePercent < 10)
		{	
			if(RecordState == RECORD_STATE_OFF)
			{
				StorageDeleteEarliestDir();

			}

		}

	}

}




void StorageGetSpaceInformation(void)
{
	struct statfs diskInfo;  

	int block_size;
	int total_blocks;
	int free_blocks;
	
      
	statfs("/sddisk/", &diskInfo);  

	block_size = diskInfo.f_bsize/1000;
	total_blocks = diskInfo.f_blocks;
	free_blocks = diskInfo.f_bfree;

//	printf("SD Card block size is: %d.\n", block_size);
//	printf("SD Card total blocks is: %d.\n", total_blocks);
//	printf("SD Card free blocks is: %d.\n", free_blocks);
	
	StorageSpaceTotal = total_blocks * block_size;
	StorageSpaceFree = free_blocks * block_size;
	StorageSpaceFreePercent = StorageSpaceFree / (StorageSpaceTotal / 100);
	

}



void StorageReadDevId(void)
{
	FILE *fp;

	char item[32];
	int value;

	int flag;

	int ret;
	

	fp = fopen("/sddisk/system/data/config", "r");
	if(fp == NULL)
	{
		printf("Can not open the config file.\n");

		return;

	}


	flag = 0;
	
	while(flag == 0)
	{
		ret = fscanf(fp, "%s", item);
		if(ret < 0)
		{
			printf("Read DevId item Error.\n");

			fclose(fp);

			return;

		}

//		printf("%s\n", item);

		if(strcmp(item, "devid:") == 0)
		{
			flag = 1;

		}

	}


	ret = fscanf(fp, "%d", &value);
	if(ret < 0)
	{
		printf("Read DevId value Error.\n");

		fclose(fp);

		return;

	}


	if((value>0) && (value < 64))
	{
		DevId = (u8)value;

	}
	else
	{
		printf("Read DevId value Error, value=%d.\n", value);

	}


	fclose(fp);
	

}



void StorageReadLocalIp(void)
{
	FILE *fp;

	char item[32];
	int value[4];

	int flag;

	int ret;
	int i;
	

	fp = fopen("/sddisk/system/data/config", "r");
	if(fp == NULL)
	{
		printf("Can not open the config file.\n");

		return;

	}


	flag = 0;
	
	while(flag == 0)
	{
		ret = fscanf(fp, "%s", item);
		if(ret < 0)
		{
			printf("Read LocalIp item Error.\n");

			fclose(fp);

			return;

		}

//		printf("%s\n", item);

		if(strcmp(item, "localip:") == 0)
		{
			flag = 1;

		}

	}


	ret = fscanf(fp, "%d.%d.%d.%d", &value[0], &value[1], &value[2], &value[3]);
	if(ret < 0)
	{
		printf("Read LocalIp value Error.\n");

		fclose(fp);

		return;

	}

	for(i=0; i<4; i++)
	{
		if((value[i]>0) && (value[i] < 256))
		{
			NetLocalIp[i] = (u8)(value[i]);

		}
		else
		{
			printf("Read LocalIp value Error, value=%d.\n", value[i]);

		}

	}

	fclose(fp);

}



int StorageDeleteEarliestDir(void)
{
	DIR *dirptr = NULL; 
	struct dirent *entry; 

	char dir_name[64];
	char dir_name_temp[64];

	int dir_num;
	int dir_num_temp;

	int year;
	int month;
	int day;

	char cmd[64];

	
	dir_num = 20991231;
	
	if((dirptr = opendir("/sddisk/")) == NULL) 
	{ 
		printf("open dir Error.\n");
		return -1; 
		
	} 
	else 
	{ 
		while ((entry = readdir(dirptr)) != NULL) 
		{ 
			printf("%s\n", entry->d_name);

			sprintf(dir_name_temp, entry->d_name);

			if((dir_name_temp[0] == '2') && (dir_name_temp[1] == '0'))
			{
				sscanf(dir_name_temp, "%d-%d-%d", &year, &month, &day);

				dir_num_temp = year*10000 + month*100 + day ;

				if(dir_num_temp < dir_num)
				{
					dir_num = dir_num_temp;

					sprintf(dir_name, dir_name_temp);

				}

			}	
			
		} 

		printf("The earliest dir to delete is %s \n", dir_name);

		sprintf(cmd, "rm -rf %s ", dir_name);
	
		system(cmd);

		msleep(100);
		
		
		closedir(dirptr); 
		
	} 

	
	return 0; 
	
}


















