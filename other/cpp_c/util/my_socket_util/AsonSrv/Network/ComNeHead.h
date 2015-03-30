/**************************************************************************************************
Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.
File name       :comnehead.h
Author: [李晓敏(107166)]      Version: 1.00       Date:[2008-1-28 9:47:48]
Description     :网元通讯数据包头信息
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

//协议命令字定义
const USHORT  LANCMD_INFO		= 0;		//网元响应信息类型 QX命令也是0
const USHORT  LANCMD_NELOOP	    = 1;        //网元环回数据包
const USHORT  LANCMD_OSLOOP  	= 2;        //环回帧命令
const USHORT  LANCMD_CMDLN		= 3;        //发送命令
const USHORT  LANCMD_TUONLY    	= 6;        //TU用户登录特殊标识
const USHORT  LANCMD_LIMIT		= 100;      //网元到达连接限制
const USHORT  LANCMD_CON_NEW	= 101;      //创建新连接
const USHORT  LANCMD_CON_OLD	= 102;      //使用老连接
const USHORT  LANCMD_CON_RSP	= 103;      //网元连接响应


class CComNeHead 
{  
protected:
	ULONG	m_ulDstNeId;        //目的网元地址
	ULONG	m_ulSrcNeId;        //源网元地址
	USHORT	m_usCmd;            //通信命令字
	USHORT	m_usLen;            //数据长度
	ULONG	m_ulReserved;       //保留字节

	
public:
	CComNeHead()
	{
		Clear();
	}

	~CComNeHead(){};
	void Clear()
	{
		m_ulDstNeId  = 0;       //目的网元地址
		m_ulSrcNeId  = 0;       //源网元地址
		m_usCmd      = 0;       //通信命令字
		m_usLen      = 0;       //数据长度
		m_ulReserved = 0;       //保留字节
	}

	void ToHostOrder()       // 转换成主机字节顺序
	{
		m_ulDstNeId  = ntohl(m_ulDstNeId); 
		m_ulSrcNeId  = ntohl(m_ulSrcNeId); 
		m_usCmd      = ntohs(m_usCmd);     
		m_usLen      = ntohs(m_usLen);     
		m_ulReserved = ntohl(m_ulReserved);
	}

	// 转换成网络字节顺序
	void ToNetOrder()
	{
		m_ulDstNeId  = htonl(m_ulDstNeId); 
		m_ulSrcNeId  = htonl(m_ulSrcNeId); 
		m_usCmd      = htons(m_usCmd);     
		m_usLen      = htons(m_usLen);     
		m_ulReserved = htonl(m_ulReserved);
	}      

	ULONG	GetDstNeId(){return m_ulDstNeId;};  // 获取目的网元ID
	void	SetDstNeId(ULONG ulDstNeId) { m_ulDstNeId = ulDstNeId;}  // 设置目的网元ID

	ULONG	GetSrcNeId(){return m_ulSrcNeId;};   // 获取源网元ID
	void	SetSrcNeId(ULONG ulSrcNeId) { m_ulSrcNeId = ulSrcNeId;}  // 设置源网元ID

	USHORT	GetCmd(){ return m_usCmd;};    // 获取命令字
	void	SetCmd(USHORT usCmd){m_usCmd = usCmd;}  // 设置命令字

	USHORT	GetLen(){return m_usLen;};     // 获取数据长度
	void	SetLen(USHORT usLen){m_usLen = usLen;}  // 设置数据长度
    
    ULONG   GetReserved(){return m_ulReserved;};
    void    SetReserved(ULONG ulReserved){m_ulReserved = ulReserved;}

	CComNeHead& operator = (CComNeHead& right);
};


#endif
