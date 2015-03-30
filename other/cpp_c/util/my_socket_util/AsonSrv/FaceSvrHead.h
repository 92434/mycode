///////////////////////////////////////////////////////////
//Author:		yinwenhui
//FileName:		FaceSvrHead.h
//CreateDate:	2012-05-26
///////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const USHORT  CMD_CONNECT_NE			= 0;
const USHORT  CMD_DISCONNECT_NE			= 1;
const USHORT  CMD_SEND_NE_CMD			= 2;
const USHORT  CMD_PY_SET_MOD_ARGS		= 3;
const USHORT  CMD_PY_EXE_MOD			= 4;
const USHORT  CMD_PY_EXE_MOD_RESULT		= 5;
const USHORT  CMD_NE_IS_CONN			= 6;
const USHORT  CMD_NE_IS_LOGIN			= 7;
const USHORT  CMD_HEART_THROB			= 8;
const USHORT  CMD_HOST_IP_GROUP			= 9;
const USHORT  CMD_NE_IP_GROUP			= 10;
const USHORT  CMD_EXIT_SERVER			= 11;
const USHORT  CMD_PY_DEBUG				= 12;
const USHORT  CMD_PY_PRINT				= 13;
const USHORT  CMD_NE_ID					= 14;

const USHORT  CMD_PY_DEBUG_CLEAR		= 15;
const USHORT  CMD_PY_PRINT_CLEAR		= 16;

const USHORT  CMD_FILE_DIALOG			= 17;
const USHORT  CMD_FOLDER_DIALOG			= 18;






#define  PACK_KEY_WORD	  "&YWH"


typedef struct FaceSvrHeader
{
    char   szHeader[4];         // 消息头 (固定为&YWH)
    USHORT usCmdType;           // 消息包类型
    UINT   uBufLen;             // 消息体长度
    UINT   uCheckSum;			// 消息体校验

	FaceSvrHeader()
	{
		memcpy(szHeader, PACK_KEY_WORD, 4);
		usCmdType = 0;
		uBufLen = 0;
		uCheckSum = 0;
	}

    void ToHostOrder()
    {
        usCmdType		= ntohs(usCmdType);
        uBufLen			= ntohl(uBufLen);
        uCheckSum       = ntohl(uCheckSum);
    };
    void ToNetOrder()
    {
		usCmdType		= htons(usCmdType);
        uBufLen			= htonl(uBufLen);
        uCheckSum       = htonl(uCheckSum);
    } 
} FACE_SVR_HEADER, *PFACE_SVR_HEADER;


typedef struct ConnStruct
{
	char strIP[16];
	UINT iPort;
	ULONG ulNeID;
} CONN_STRUCT,*PCONN_STRUCT;
