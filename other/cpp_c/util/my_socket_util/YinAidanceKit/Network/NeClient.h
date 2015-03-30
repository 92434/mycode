// NeClient.h: interface for the CNeClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NECLIENT_H__3017B821_D551_45D5_8D28_49A63D841506__INCLUDED_)
#define AFX_NECLIENT_H__3017B821_D551_45D5_8D28_49A63D841506__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "YinSocket.h"
#include "..\..\AsonSrv\Network\ComNeMsg.h"

class CNeClient : public CYinSocket
{
public:
	CNeClient();
	virtual ~CNeClient();
	
	UINT CreateConn(const char* pszIP, UINT iPort);
	UINT Send(const char* strMsg, USHORT usCmdType, UINT iLen);

	void WritePack(CComNeMsg* pNeMsg);
	void ReadAndPopPack(CComNeMsg** ppNeMsg);

	void CloseClt();

	BOOL m_bIsConnSrv;
private:
	CCriticalSection m_CsPack;
	CCriticalSection m_CsSend;
	queue<CComNeMsg*> m_qClientPack;

protected:
	virtual void OnCltRecve(CHAR* pMsg, int iSize);
	virtual void OnErrorInfo(UINT iErrID);
	virtual void OnIdly();

};

#endif // !defined(AFX_NECLIENT_H__3017B821_D551_45D5_8D28_49A63D841506__INCLUDED_)
