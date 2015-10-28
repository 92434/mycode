// HfcpDataCfg.h: interface for the CHfcpDataCfg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HFCPDATACFG_H__9F7C80AC_D9C0_4692_8ACC_F2A7379CD192__INCLUDED_)
#define AFX_HFCPDATACFG_H__9F7C80AC_D9C0_4692_8ACC_F2A7379CD192__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "HfcpDefine.h"

#include <math.h>

class CHfcpDataCfg  
{
public:
	CHfcpDataCfg()
	{
		Clear();
	}
	
	virtual ~CHfcpDataCfg(){}

	void Clear()
	{
		m_strNEFileAllName = "";
		m_strLocalFileAllName = "";
		
		m_nPackMaxCount = HFCP_UPLOAD_USEDSTATES_COUNT;// 多包发送包数暂定为8
		m_nPackSize = HFCP_PACKET_LEN;// 每包数据长度为800字节
		
		m_nInterval = HFCP_INTERVAL_TIME;	//5秒钟没收到响应帧则重发
		m_nDataRetry = HFCP_DATA_RETRY;		//每包数据最多重试数据为10次
		m_nLastRetry = HFCP_FLASH_RETRY;	//从内存向文件写时超时时间要延长一些
		

		m_usPackNo = 0;
		m_uFileSize = 0;
		m_uPackCount = 0;
		m_ulSendCmd = 0;
		m_usRspPackNo = 0;
	}

	void SetNeFileName(LPCTSTR lpFileName){ m_strNEFileAllName = lpFileName; }
	void SetLocalFileName(LPCTSTR lpFileName){ m_strLocalFileAllName = lpFileName; }
	
	string& GetNeFileName(){ return m_strNEFileAllName; }
	string& GetLocalFileName(){ return m_strLocalFileAllName; }
	
	USHORT GetPackMaxCount() { return m_nPackMaxCount; }
	
    USHORT GetPackSize() { return m_nPackSize; }
	
    USHORT GetInterval() { return m_nInterval; }
	
    USHORT GetDataRetry() { return m_nDataRetry; }
	
    USHORT GetLastRetry() { return m_nLastRetry; }

	void SetPackNo(USHORT usPackNo) { m_usPackNo = usPackNo; }
	USHORT GetPackNo() { return m_usPackNo; }

	void SetFileSize(UINT uFileSize) 
	{ 
		m_uFileSize = uFileSize; 
		m_uPackCount = GetPackCountPir ();
	}

	UINT GetFileSize() { return m_uFileSize; }
	UINT GetPackCount (){ return m_uPackCount; }
	void SetSendCmd(UINT uSendCmd) { m_ulSendCmd = uSendCmd; }
	UINT GetSendCmd() { return m_ulSendCmd; }
	char* GetMemFilePack()
	{
		return m_byteStr;
	}

private:
	UINT GetPackCountPir ()
	{
		UINT uCount = 0;
		if (0 == m_uFileSize)
		{
			return 1;
		}
		if (0 != m_nPackSize)
		{
			uCount = ceil( m_uFileSize / float(m_nPackSize) );
		}

		return uCount;
	}
	
public:
	string m_strNEFileAllName;
	string m_strLocalFileAllName;
	USHORT m_nPackMaxCount;	// 同时发送的最大包号
    USHORT m_nPackSize;		// 每包数据大小

	
    USHORT m_nInterval;		// 重发时间间隔
    USHORT m_nDataRetry;	// 数据重试次数
    USHORT m_nLastRetry;	// 最后批量结束响应帧的重试次数
	
	USHORT m_usPackNo;		//发包序号
	UINT   m_uFileSize;		//文件大小
	UINT   m_uPackCount;	//包总数
	UINT   m_ulSendCmd;		//当前发送的命令字

	USHORT m_usRspPackNo;	//接收包序号

	char	m_byteStr[HFCP_PACKET_LEN];
};

#endif // !defined(AFX_HFCPDATACFG_H__9F7C80AC_D9C0_4692_8ACC_F2A7379CD192__INCLUDED_)
