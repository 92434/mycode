# Microsoft Developer Studio Project File - Name="YinAidanceKit" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=YinAidanceKit - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "YinAidanceKit.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "YinAidanceKit.mak" CFG="YinAidanceKit - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "YinAidanceKit - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "YinAidanceKit - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "YinAidanceKit - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Release"
# PROP Intermediate_Dir "..\Dump\Release\AidanceKit"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"..\Release/AsonKit.exe"
# SUBTRACT LINK32 /incremental:yes /debug

!ELSEIF  "$(CFG)" == "YinAidanceKit - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Release"
# PROP Intermediate_Dir "..\Dump\Debug\AidanceKit"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"..\Release/AsonKit.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "YinAidanceKit - Win32 Release"
# Name "YinAidanceKit - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AllCollecDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AllGwInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AssistantDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BottomDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BusinessDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CtrlNeStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MyEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\NeLinkImgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NePointDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputView.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\UserLoginDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\YinAidanceKit.cpp
# End Source File
# Begin Source File

SOURCE=.\YinAidanceKit.rc
# End Source File
# Begin Source File

SOURCE=.\YinAidanceKitDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\YinAidanceKitView.cpp
# End Source File
# Begin Source File

SOURCE=.\YinEvent.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AllCollecDlg.h
# End Source File
# Begin Source File

SOURCE=.\AllGwInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\AssistantDlg.h
# End Source File
# Begin Source File

SOURCE=.\BottomDlg.h
# End Source File
# Begin Source File

SOURCE=.\BusinessDlg.h
# End Source File
# Begin Source File

SOURCE=.\CtrlNeStatic.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MyEdit.h
# End Source File
# Begin Source File

SOURCE=.\NeLinkImgDlg.h
# End Source File
# Begin Source File

SOURCE=.\NePointDlg.h
# End Source File
# Begin Source File

SOURCE=.\OutputDlg.h
# End Source File
# Begin Source File

SOURCE=.\OutputView.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\UserLoginDlg.h
# End Source File
# Begin Source File

SOURCE=.\YinAidanceKit.h
# End Source File
# Begin Source File

SOURCE=.\YinAidanceKitDoc.h
# End Source File
# Begin Source File

SOURCE=.\YinAidanceKitView.h
# End Source File
# Begin Source File

SOURCE=.\YinEvent.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\imapclient.ico
# End Source File
# Begin Source File

SOURCE=.\res\ne.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\YinAidanceKit.ico
# End Source File
# Begin Source File

SOURCE=.\res\YinAidanceKit.rc2
# End Source File
# Begin Source File

SOURCE=.\res\YinAidanceKitDoc.ico
# End Source File
# End Group
# Begin Group "Network"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Network\ComNeMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\ComNeMsg.h
# End Source File
# Begin Source File

SOURCE=.\Network\NeClient.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\NeClient.h
# End Source File
# Begin Source File

SOURCE=.\Network\NeOper.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\NeOper.h
# End Source File
# Begin Source File

SOURCE=.\Network\YinSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\YinSocket.h
# End Source File
# End Group
# Begin Group "PublicModule"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PublicModule\AppConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\PublicModule\AppConfig.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
