// AsonSrv.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SevFrame.h"


int main(int argc, char* argv[])
{
	g_obFileLog.CreateLog();

// 	CTcpServer::StartupWSA();
// 	CNeOperate ObTest;
// 	ObTest.Connect("129.9.68.35");
// 	Sleep(200);
// 	string strResult;
// 	ObTest.SendCmd(":lognew:\"yinwen\",\"@ywh12345\"", strResult);
// 	cout<<strResult.c_str()<<"\r\n";
// 	Sleep(800);
// 	ObTest.UploadFile("/ofs1/hwx/NESOFT.HWX", "D:\\zz.txt");
//                                                                                                          
// 	int a;
// 	cin>>a;
// 	CTcpServer::StopWSA();

// 	int aa = 0;
// 	while(aa<=1024 * 3000 + 5 )
// 	{
// 		g_obFileLog.WriteLog("g");
// 		aa++;
// 	}
// 	int a;
// 	cin>>a;
	//1.µÃµ½¶Ë¿Ú
	cout<<"argc:"<<argc<<"\r\n";
	if (2 > argc) return 0;
	char* pPort = argv[1];
	cout<<"PORT:"<<pPort<<"\r\n";
	UINT uPort = atoi(pPort);
  	CSevFrame obSevFrame;

	obSevFrame.CreateSrv(12500);//uPort
	obSevFrame.Run();

	obSevFrame.CloseSrv();
	return 0;
}


