/**************************************************************************************************
Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.
File name       :comnehead.h
Author: [������(107166)]      Version: 1.00       Date:[2008-1-28 9:47:48]
Description     :��ԪͨѶ���ݰ�ͷ��Ϣ
Others          :
Function List   :
     1: ...
History:         :
     Date             :
     Author           :
     Modification     :
     
**************************************************************************************************/
#include "YinSocket.h"

#ifndef __ComNEHead_H__
#define __ComNEHead_H__

//Э�������ֶ���
const USHORT  LANCMD_INFO		= 0;		//��Ԫ��Ӧ��Ϣ���� QX����Ҳ��0
const USHORT  LANCMD_NELOOP	    = 1;        //��Ԫ�������ݰ�
const USHORT  LANCMD_OSLOOP  	= 2;        //����֡����
const USHORT  LANCMD_CMDLN		= 3;        //��������
const USHORT  LANCMD_TUONLY    	= 6;        //TU�û���¼�����ʶ
const USHORT  LANCMD_LIMIT		= 100;      //��Ԫ������������
const USHORT  LANCMD_CON_NEW	= 101;      //����������
const USHORT  LANCMD_CON_OLD	= 102;      //ʹ��������
const USHORT  LANCMD_CON_RSP	= 103;      //��Ԫ������Ӧ


class CComNeHead 
{  
protected:
	ULONG	m_ulDstNeId;        //Ŀ����Ԫ��ַ
	ULONG	m_ulSrcNeId;        //Դ��Ԫ��ַ
	USHORT	m_usCmd;            //ͨ��������
	USHORT	m_usLen;            //���ݳ���
	ULONG	m_ulReserved;       //�����ֽ�

	
public:
	CComNeHead()
	{
		Clear();
	}

	~CComNeHead(){};
	void Clear()
	{
		m_ulDstNeId  = 0;       //Ŀ����Ԫ��ַ
		m_ulSrcNeId  = 0;       //Դ��Ԫ��ַ
		m_usCmd      = 0;       //ͨ��������
		m_usLen      = 0;       //���ݳ���
		m_ulReserved = 0;       //�����ֽ�
	}

	void ToHostOrder()       // ת���������ֽ�˳��
	{
		m_ulDstNeId  = ntohl(m_ulDstNeId); 
		m_ulSrcNeId  = ntohl(m_ulSrcNeId); 
		m_usCmd      = ntohs(m_usCmd);     
		m_usLen      = ntohs(m_usLen);     
		m_ulReserved = ntohl(m_ulReserved);
	}

	// ת���������ֽ�˳��
	void ToNetOrder()
	{
		m_ulDstNeId  = htonl(m_ulDstNeId); 
		m_ulSrcNeId  = htonl(m_ulSrcNeId); 
		m_usCmd      = htons(m_usCmd);     
		m_usLen      = htons(m_usLen);     
		m_ulReserved = htonl(m_ulReserved);
	}      

	ULONG	GetDstNeId(){return m_ulDstNeId;};  // ��ȡĿ����ԪID
	void	SetDstNeId(ULONG ulDstNeId) { m_ulDstNeId = ulDstNeId;}  // ����Ŀ����ԪID

	ULONG	GetSrcNeId(){return m_ulSrcNeId;};   // ��ȡԴ��ԪID
	void	SetSrcNeId(ULONG ulSrcNeId) { m_ulSrcNeId = ulSrcNeId;}  // ����Դ��ԪID

	USHORT	GetCmd(){ return m_usCmd;};    // ��ȡ������
	void	SetCmd(USHORT usCmd){m_usCmd = usCmd;}  // ����������

	USHORT	GetLen(){return m_usLen;};     // ��ȡ���ݳ���
	void	SetLen(USHORT usLen){m_usLen = usLen;}  // �������ݳ���
    
    ULONG   GetReserved(){return m_ulReserved;};
    void    SetReserved(ULONG ulReserved){m_ulReserved = ulReserved;}

	CComNeHead& operator = (CComNeHead& right);
};


#endif
