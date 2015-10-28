// stdafx.cpp : source file that includes just the standard includes
//	YinAidanceKit.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

CAppConfig		g_obAppCfg;


void Split(string& str, string strChars, vector<string>& vStr)
{
	const char* pStr = str.c_str();
	int iStrLen = str.length();
	int iCharsLen = strChars.length();

	if (iStrLen == 0)
	{
		vStr.clear();
		return;
	}
	
	if (iCharsLen > iStrLen)
	{
		vStr.push_back(str);
	}
	
	int iSign = 0;
	int i;
	for (i=0; i<iStrLen; i++)
	{
		string sTmp = str.substr(i, iCharsLen);
		if (strChars == sTmp)
		{
			if (iSign != i)
			{
				//string sfs = str.substr(iSign, i - iSign);
				vStr.push_back(str.substr(iSign, i - iSign));
				i = i + iCharsLen;
				iSign = i;
				i -= 1;
			}
			else
			{
				if(iSign == 0)
				{
					iSign += iCharsLen;
					i = iSign - 1;
				}
				else
				{
					vStr.push_back("");
					iSign += iCharsLen;
					i = iSign - 1;
				}
			}
		}
	}
	
	if (i - 1 >= iSign)
	{
		//string sfs = str.substr(iSign, iStrLen - iSign);
		vStr.push_back(str.substr(iSign, iStrLen - iSign));
	}
}

string TrimRight(const string& str, const string& strChar/* = " "*/)
{      
	string::size_type pos = str.find_last_not_of(strChar); 
	if (pos == string::npos)   
	{        
		return str;
	}  
	return str.substr(0, pos + 1);
}

string TrimLeft(const string& str, const string& strChar/* = " "*/)
{    
	string::size_type pos = str.find_first_not_of(strChar);
	if (pos == string::npos)   
	{        
		return str;   
	}
	return str.substr(pos);
}

string Trim(const string& str, const string& strChar/* = " "*/)
{
	string strTmp = TrimRight(str, strChar);
	return TrimLeft(strTmp, strChar);
}

//"0x000b0002"	>>> 0x000b0002
//"11-2"		>>> 0x000b0002
//"0"			>>> 0x00000000
BOOL Str2HexNeId(string strNeId, UINT& ulNeID)
{
	if (strNeId == "0")
	{
		ulNeID = 0;
		return TRUE;
	}

	if (strNeId.at(0)=='0' && strNeId.at(1)=='x')
	{
		string strNum = strNeId.substr(2);
		sscanf(strNum.c_str(), "%x", &ulNeID);
		return TRUE;
	}

	vector<string> vstrNeid;
	Split(strNeId, "-", vstrNeid);
	if (vstrNeid.size() != 2)
	{
		return FALSE;
	}
	int iNeIDHig = atoi(vstrNeid[0].c_str());
	int iNeIDLow = atoi(vstrNeid[1].c_str());
	ulNeID = (iNeIDHig<<16) | iNeIDLow;
	return TRUE;
}


//计算指定数据的校验和
//pszData :数据
//uLen:   数据长度
UINT CalcCheckSum(const char* pszData,UINT uLen)
{
    UINT uSum = 0;
	
    if (NULL == pszData)
    {
        return uSum;
    }
	
    for (UINT i = 0; i < uLen; i++)
    {
        uSum += (UINT)(*((UCHAR*)pszData + i));
    }
    return uSum;
}


