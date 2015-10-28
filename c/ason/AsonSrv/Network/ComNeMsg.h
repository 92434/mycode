// ComNeMsg.h: interface for the CComNeMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMNEMSG_H__BA82B6DB_AB1D_40F2_B254_A90DB5722928__INCLUDED_)
#define AFX_COMNEMSG_H__BA82B6DB_AB1D_40F2_B254_A90DB5722928__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ComNeHead.h"
#include "../FaceSvrHead.h"

class CComNeMsg  
{
public:
	CComNeMsg();
//	CComNeMsg(const CComNeMsg& Right);
	virtual ~CComNeMsg();

	void CreateBuf(int nDataLen);
    // �����Ϣ������
	void  AddMsg(const void* pData, USHORT usDataLen);
	
    // ����ַ�������Ϣ������
//	void  AddStrToMsg(char* pData);
	void  EmptyMsg(); 
	
	char* GetMsg() { return m_pMsg;}
	
//    void Print();
	int GetMsgLen() { return m_nBufLen; }
	int GetTrulyLen() { return m_ipos; }
	//CComNeMsg& operator = (const CComNeMsg& Right);


public:
	CComNeHead m_obComNeHead;		// ��Ԫ��Ϣͷ
	FaceSvrHeader m_obFaceHeader;	// �ͻ������ͨ��
	
protected:
	char*	m_pMsg;	         // ָ����Ϣ��ָ��
	int		m_nBufLen;       // ��Ϣ�峤��
	int		m_ipos;
};

#endif // !defined(AFX_COMNEMSG_H__BA82B6DB_AB1D_40F2_B254_A90DB5722928__INCLUDED_)
