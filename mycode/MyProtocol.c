
//*****************************************************//
//		MyProtocol.c
//*****************************************************//

#include "MyAll.h"


ProtocolDeviceInfoTypeDef ProtocolLocalInfo;
ProtocolDeviceInfoTypeDef ProtocolBroadInfo;



int ProtocolInit(void)
{

	ProtocolLocalInfo.DevType = DevType;
	ProtocolLocalInfo.DevId =DevId;
	ProtocolLocalInfo.TrainId =TrainId;
	ProtocolLocalInfo.Ip32 = NetIp4toIp32(NetLocalIp);
	
	

	ProtocolBroadInfo.DevType = 0xff;
	ProtocolBroadInfo.DevId =0xff;
	ProtocolBroadInfo.TrainId =0xffff;
	ProtocolBroadInfo.Ip32= 0xffffffff;




	return 0;
	
}





int ProtocolRcvHeadCheck(u8* pbuf)
{

	ProtocolHeadTypeDef* phead;


	phead = (ProtocolHeadTypeDef*)(pbuf);


	if(phead->HeadMark != 0x7E)
	{
		return -1;
	}


	if((phead->DestInfo.DevType != DevType) && (phead->DestInfo.DevType != 0xFF))
	{
		return -1;
	}

	if((phead->DestInfo.DevId != DevId) && (phead->DestInfo.DevId  != 0xFF))
	{
		return -1;
	}

//	if((phead->DestInfo.Ip32 != NetIp4toIp32(NetLocalIp)) && (phead->DestInfo.Ip32!= 0xFFFFFFFF) && (phead->DestInfo.Ip32!= (NetIp4toIp32(NetLocalIp) | 0x000000FF)))
	{
//		return -1;
	}


	return 0;

}



u8 ProtocolRcvGetSrcDevType(u8* pbuf)
{
	u8 src_type;

	ProtocolHeadTypeDef* phead;
	

	phead = (ProtocolHeadTypeDef*)(pbuf);

	src_type = (phead->SrcInfo).DevType;

	return src_type;

}



u8 ProtocolRcvGetSrcDevId(u8* pbuf)
{
	u8 src_id;

	ProtocolHeadTypeDef* phead;
	

	phead = (ProtocolHeadTypeDef*)(pbuf);

	src_id = (phead->SrcInfo).DevId;

	return src_id;

}



u16 ProtocolRcvGetKeyword(u8* pbuf)
{
	u16 keyword;

	ProtocolPackTypeDef* ppack;


	ppack = (ProtocolPackTypeDef*)(pbuf);

	keyword = ppack->KeyWord;

	
	return(keyword);

}

int ProtocolRcvGetDataLength(u8* pbuf)
{
	int len;

	ProtocolPackTypeDef* ppack;


	ppack = (ProtocolPackTypeDef*)(pbuf);

	len = ppack->DataLength;

	
	return(len);

}

u8* ProtocolRcvGetDataPointer(u8* pbuf)
{
	u8* pdata;

	ProtocolPackTypeDef* ppack;


	ppack = (ProtocolPackTypeDef*)(pbuf);

	pdata = (u8*)(&(ppack->DataBuf));

	
	return(pdata);

}


u8 ProtocolRcvGetControlVolume(u8* pbuf, int len, u8 dev_type, u8 dev_id )
{
	u8 volume;

	u8* pdata;

	ProtocolPackTypeDef* ppack;

	ProtocolControlVolumeTypeDef* p_crl_volume;


	ppack = (ProtocolPackTypeDef*)(pbuf);

	pdata = (u8*)(&(ppack->DataBuf));

	p_crl_volume = (ProtocolControlVolumeTypeDef*)(pdata);

	if(dev_type == PRTCL_DEVTYPE_AMP)
	{
		volume = p_crl_volume->Amp[dev_id-1];
		
		return(volume);
		
	}
	else
	{
		return(255);

	}


}






ProtocolHeadTypeDef ProtocolHeadMake(ProtocolDeviceInfoTypeDef remote_info)
{
	ProtocolHeadTypeDef  head;
	

	ProtocolLocalInfo.DevType = DevType;
	ProtocolLocalInfo.DevId = DevId;
	ProtocolLocalInfo.TrainId = TrainId;
//	ProtocolLocalInfo.Ip32 = NetIp4toIp32(NetLocalIp);
	

	head.HeadMark = 0x7E;
	
	head.DestInfo = remote_info;

	head.SrcInfo = ProtocolLocalInfo;


	return head;
}




int ProtocolSndGetInspectionPack(u8* pbuf, int* plen)
{

	ProtocolPackTypeDef* ppack;

	u8* pdata;

	u8 space;


	space = 0;


	ppack= (ProtocolPackTypeDef *)pbuf;


	ppack->Head = ProtocolHeadMake(ProtocolBroadInfo);
	
	ppack->KeyWord = PRTCL_KEYWORD_INSPECTION;

	ppack->DataLength = 6;


	pdata = (u8*)(&(ppack->DataBuf));
	
	*pdata = space;
	*(pdata+1) = CurrentState;
	*(pdata+2) = AppVersion;
	*(pdata+3) = AppVersion2;
	*(pdata+4) = AppVersion3;
	*(pdata+5) = AppVersion4;


	*plen = PRTCL_HEADS_LEN + (ppack->DataLength);


	return 0;
	
}



int ProtocolSndGetLocalIpPack(u8* pbuf, int* plen, u32 localip)
{

	ProtocolPackTypeDef* ppack;

	u32* plocalip;


	ppack= (ProtocolPackTypeDef *)pbuf;


//	ppack->Head = ProtocolHeadMake(ProtocolStationBroadInfo);
	
//	ppack->KeyWord = PRTCL_KEYWORD_LOCALIP_SND;

	ppack->DataLength = 4;


	plocalip = (u32*)(&(ppack->DataBuf));
	
	*plocalip = localip;
	

	*plen = PRTCL_HEADS_LEN + (ppack->DataLength);


	return 0;
	
}


int ProtocolSndGetDevIdPack(u8* pbuf, int* plen, u8 inda)
{

	ProtocolPackTypeDef* ppack;

	u8* pda;


	ppack= (ProtocolPackTypeDef *)pbuf;


//	ppack->Head = ProtocolHeadMake(ProtocolStationBroadInfo);
	
//	ppack->KeyWord = PRTCL_KEYWORD_ID_SND;

	ppack->DataLength = 1;


	pda = (u8*)(&(ppack->DataBuf));
	
	*pda = inda;
	

	*plen = PRTCL_HEADS_LEN + (ppack->DataLength);


	return 0;
	
}




int ProtocolSndGetPlayclkPack(u8* pbuf, int* plen, u32 inda)
{

	ProtocolPackTypeDef* ppack;

	u32* pda;


	ppack= (ProtocolPackTypeDef *)pbuf;


	ppack->Head = ProtocolHeadMake(ProtocolBroadInfo);
	
	ppack->KeyWord = PRTCL_KEYWORD_PLAYCLK;

	ppack->DataLength = 4;


	pda = (u32*)(&(ppack->DataBuf));
	
	*pda = inda;
	

	*plen = PRTCL_HEADS_LEN + (ppack->DataLength);


	return 0;
	
}

int ProtocolSndGetVolumePack(u8* pbuf, int* plen, u8 inda)
{

	ProtocolPackTypeDef* ppack;

	u8* pda;


	ppack= (ProtocolPackTypeDef *)pbuf;


//	ppack->Head = ProtocolHeadMake(ProtocolStationBroadInfo);
	
//	ppack->KeyWord = PRTCL_KEYWORD_VOLUME_SND;

	ppack->DataLength = 2;


	pda = (u8*)(&(ppack->DataBuf));
	
//	*pda = Area;

	*(pda+1) = inda;
	

	*plen = PRTCL_HEADS_LEN + (ppack->DataLength);


	return 0;
	
}




int ProtocolSndGetBroadcastPack(u8* pbuf, int* plen, u8* inbuf, u8 priority)
{

	ProtocolPackTypeDef* ppack;

	u8* pda;


	ppack= (ProtocolPackTypeDef *)pbuf;


	ppack->Head = ProtocolHeadMake(ProtocolBroadInfo);
	
	ppack->KeyWord = PRTCL_KEYWORD_BROADCAST;

	ppack->DataLength = 1+1024;


	pda = (u8*)(&(ppack->DataBuf));


	*pda = priority;

	memcpy((pda+1), inbuf, 1024);

	

	*plen = PRTCL_HEADS_LEN + (ppack->DataLength);


	return 0;
	
	
}





int ProtocolSndGetTalkDcPack(u8* pbuf, int* plen, u8* inbuf, int inlen)
{

	ProtocolPackTypeDef* ppack;

	u8* pda;


	ppack= (ProtocolPackTypeDef *)pbuf;


	ppack->Head = ProtocolHeadMake(ProtocolBroadInfo);
	
	ppack->KeyWord = PRTCL_KEYWORD_TALK_DC;

	ppack->DataLength = 1024;


	pda = (u8*)(&(ppack->DataBuf));

	memcpy((pda), inbuf, inlen);

	

	*plen = PRTCL_HEADS_LEN + (ppack->DataLength);


	return 0;
	
	
}



int ProtocolSndGetTalkDdPack(u8* pbuf, int* plen, u8* inbuf, int inlen)
{

	ProtocolPackTypeDef* ppack;

	u8* pda;


	ppack= (ProtocolPackTypeDef *)pbuf;


	ppack->Head = ProtocolHeadMake(ProtocolBroadInfo);
	
	ppack->KeyWord = PRTCL_KEYWORD_TALK_DD;

	ppack->DataLength = 1024;


	pda = (u8*)(&(ppack->DataBuf));

	memcpy((pda), inbuf, inlen);

	

	*plen = PRTCL_HEADS_LEN + (ppack->DataLength);


	return 0;
	
	
}




int ProtocolSndGetTalkTestLeftPack(u8* pbuf, int* plen, u8* inbuf, int inlen)
{

	ProtocolPackTypeDef* ppack;

	u8* pda;


	ppack= (ProtocolPackTypeDef *)pbuf;


	ppack->Head = ProtocolHeadMake(ProtocolBroadInfo);
	
	ppack->KeyWord = PRTCL_KEYWORD_TALK_TEST_LEFT;

	ppack->DataLength = 1024;


	pda = (u8*)(&(ppack->DataBuf));

	memcpy((pda), inbuf, inlen);

	

	*plen = PRTCL_HEADS_LEN + (ppack->DataLength);


	return 0;
	
	
}


int ProtocolSndGetTalkTestRightPack(u8* pbuf, int* plen, u8* inbuf, int inlen)
{

	ProtocolPackTypeDef* ppack;

	u8* pda;


	ppack= (ProtocolPackTypeDef *)pbuf;


	ppack->Head = ProtocolHeadMake(ProtocolBroadInfo);
	
	ppack->KeyWord = PRTCL_KEYWORD_TALK_TEST_RIGHT;

	ppack->DataLength = 1024;


	pda = (u8*)(&(ppack->DataBuf));

	memcpy((pda), inbuf, inlen);

	

	*plen = PRTCL_HEADS_LEN + (ppack->DataLength);


	return 0;
	
	
}








