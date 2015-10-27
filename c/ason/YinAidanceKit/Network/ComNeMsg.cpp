// ComNeMsg.cpp: implementation of the CComNeMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ComNeMsg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComNeMsg::CComNeMsg()
{
	m_ipos = 0;
	m_nBufLen = 0;
	m_pMsg = NULL;
	m_obComNeHead.Clear();
}

// CComNeMsg::CComNeMsg(const CComNeMsg& Right)
// {
// 	if(&Right != this)
// 	{
// 		m_nBufLen = 0;
// 		m_ipos = 0;
// 		m_pMsg = NULL;
// 		CComNeMsg* pRight = (CComNeMsg*)&Right;
// 		CreateBuf( pRight->GetMsgLen() );
// 		memcpy(&m_obComNeHead, &pRight->m_obComNeHead, sizeof(CComNeHead));
// 		AddMsg( pRight->GetMsg(), pRight->GetMsgLen());
// 		pRight->EmptyMsg();
// 	}
// }

CComNeMsg::~CComNeMsg()
{
	EmptyMsg();
}

void CComNeMsg::CreateBuf(int nDataLen)
{
	m_pMsg = new char[nDataLen];
	m_nBufLen = nDataLen;
}


void  CComNeMsg::AddMsg(const void* pData, USHORT usDataLen)
{
	memcpy(m_pMsg + m_ipos, pData, usDataLen);
	m_ipos += usDataLen;
}


void CComNeMsg::EmptyMsg()
{
	if ( NULL != m_pMsg )
	{
		delete[] m_pMsg;
	}
	m_nBufLen = 0;
	m_ipos = 0;
	m_pMsg = NULL;
	m_obComNeHead.Clear();
}


// 
// CComNeMsg& CComNeMsg::operator = (const CComNeMsg& Right) 
// {
// 	if(&Right != this)
// 	{
// 		m_nBufLen = 0;
// 		m_ipos = 0;
// 		m_pMsg = NULL;
// 		CComNeMsg* pRight = (CComNeMsg*)&Right;
// 		memcpy(&m_obComNeHead, &pRight->m_obComNeHead, sizeof(CComNeHead));
// 		CreateBuf( pRight->GetMsgLen() );
// 		AddMsg( pRight->GetMsg(), pRight->GetMsgLen());
// 		pRight->EmptyMsg();
// 	}
// 	return *this;
// }





