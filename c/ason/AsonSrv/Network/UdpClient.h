// UdpClient.h: interface for the CUdpClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UDPCLIENT_H__6B8CB558_FC90_4769_A187_FBD94166DCC7__INCLUDED_)
#define AFX_UDPCLIENT_H__6B8CB558_FC90_4769_A187_FBD94166DCC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "YinSocket.h"

const char strNeSearchKey1[32] = "NESOFTECHOOHCETFOSEN";
const char strNeSearchKey2[32] = "~ES\x7FVTESXOOXSETV\x7FSE~";


class CUdpClient : public CYinSocket
{
public:
	CUdpClient();
	virtual ~CUdpClient();

	void CreateClient();

	void GetAllGipGroup(string strGIp, string& strIpGroup);
public:
	vector<string> m_vStrIpGroup;

protected:
	virtual void OnCltRecve(CHAR* pMsg, int iSize);
	virtual void OnErrorInfo(UINT iErrID);
	virtual void OnIdly();

	
};

#endif // !defined(AFX_UDPCLIENT_H__6B8CB558_FC90_4769_A187_FBD94166DCC7__INCLUDED_)
