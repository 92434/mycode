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
		
		m_nPackMaxCount = HFCP_UPLOAD_USEDSTATES_COUNT;// ������Ͱ����ݶ�Ϊ8
		m_nPackSize = HFCP_PACKET_LEN;// ÿ�����ݳ���Ϊ800�ֽ�
		
		m_nInterval = HFCP_INTERVAL_TIME;	//5����û�յ���Ӧ֡���ط�
		m_nDataRetry = HFCP_DATA_RETRY;		//ÿ�����������������Ϊ10��
		m_nLastRetry = HFCP_FLASH_RETRY;	//���ڴ����ļ�дʱ��ʱʱ��Ҫ�ӳ�һЩ
		

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
	USHORT m_nPackMaxCount;	// ͬʱ���͵�������
    USHORT m_nPackSize;		// ÿ�����ݴ�С

	
    USHORT m_nInterval;		// �ط�ʱ����
    USHORT m_nDataRetry;	// �������Դ���
    USHORT m_nLastRetry;	// �������������Ӧ֡�����Դ���
	
	USHORT m_usPackNo;		//�������
	UINT   m_uFileSize;		//�ļ���С
	UINT   m_uPackCount;	//������
	UINT   m_ulSendCmd;		//��ǰ���͵�������

	USHORT m_usRspPackNo;	//���հ����

	char	m_byteStr[HFCP_PACKET_LEN];
};

#endif // !defined(AFX_HFCPDATACFG_H__9F7C80AC_D9C0_4692_8ACC_F2A7379CD192__INCLUDED_)
