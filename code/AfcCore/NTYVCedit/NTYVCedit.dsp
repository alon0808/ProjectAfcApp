# Microsoft Developer Studio Project File - Name="NTYVCedit" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=NTYVCedit - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NTYVCedit.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NTYVCedit.mak" CFG="NTYVCedit - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NTYVCedit - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "NTYVCedit - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "NTYVCedit - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f NTYVCedit.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "NTYVCedit.exe"
# PROP BASE Bsc_Name "NTYVCedit.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f NTYVCedit.mak"
# PROP Rebuild_Opt "/a"
# PROP Target_File "NTYVCedit.exe"
# PROP Bsc_Name "NTYVCedit.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "NTYVCedit - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f NTYVCedit.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "NTYVCedit.exe"
# PROP BASE Bsc_Name "NTYVCedit.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "nmake /f "NTYVCedit.mak""
# PROP Rebuild_Opt "/a"
# PROP Target_File "NTYVCedit.exe"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "NTYVCedit - Win32 Release"
# Name "NTYVCedit - Win32 Debug"

!IF  "$(CFG)" == "NTYVCedit - Win32 Release"

!ELSEIF  "$(CFG)" == "NTYVCedit - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\CPUCard.c
# End Source File
# Begin Source File

SOURCE=..\CPUCARJTB.c
# End Source File
# Begin Source File

SOURCE=..\inputmisc\GPIOCtrl.c
# End Source File
# Begin Source File

SOURCE=..\inputmisc\GPIOCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\GprsSocket.c
# End Source File
# Begin Source File

SOURCE=\P2new\gps.c
# End Source File
# Begin Source File

SOURCE=..\inputmisc\IcCardCtrlApi.c
# End Source File
# Begin Source File

SOURCE=..\ICCardLib.c
# End Source File
# Begin Source File

SOURCE=..\SlDemo\LightColor.c
# End Source File
# Begin Source File

SOURCE=..\main.c
# End Source File
# Begin Source File

SOURCE=..\MYDES.c
# End Source File
# Begin Source File

SOURCE=..\inputmisc\PsamCard.c
# End Source File
# Begin Source File

SOURCE=..\PSAMLib.c
# End Source File
# Begin Source File

SOURCE=\P2new\QRCodeMain.c
# End Source File
# Begin Source File

SOURCE=..\RecordFile.c
# End Source File
# Begin Source File

SOURCE=..\SL8583_4GLinux.c
# End Source File
# Begin Source File

SOURCE=..\inputmisc\Uart.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\include\apparel.h
# End Source File
# Begin Source File

SOURCE="..\include\City-handan.h"
# End Source File
# Begin Source File

SOURCE=..\CPUCard.h
# End Source File
# Begin Source File

SOURCE=..\include\CPUCARJTB.h
# End Source File
# Begin Source File

SOURCE="..\include\Debug_ .h"
# End Source File
# Begin Source File

SOURCE=..\include\debug_.h
# End Source File
# Begin Source File

SOURCE=..\include\ev.h
# End Source File
# Begin Source File

SOURCE=..\inputmisc\GPIOCtrl.h
# End Source File
# Begin Source File

SOURCE=..\SlDemo\GPRS.h
# End Source File
# Begin Source File

SOURCE=..\GPRSdatatype.h
# End Source File
# Begin Source File

SOURCE=..\GprsSocket.h
# End Source File
# Begin Source File

SOURCE=\P2new\gps.h
# End Source File
# Begin Source File

SOURCE=..\inputmisc\IcCardCtrlApi.h
# End Source File
# Begin Source File

SOURCE=..\ICCardLib.h
# End Source File
# Begin Source File

SOURCE=..\include\InitSystem.h
# End Source File
# Begin Source File

SOURCE=..\include\kfifo.h
# End Source File
# Begin Source File

SOURCE=..\inputmisc\LightBeep.h
# End Source File
# Begin Source File

SOURCE=..\SlDemo\LightColor.h
# End Source File
# Begin Source File

SOURCE=..\include\LtyCommon.h
# End Source File
# Begin Source File

SOURCE=..\include\ltycommondatatype.h
# End Source File
# Begin Source File

SOURCE=..\include\ltycommonfunction.h
# End Source File
# Begin Source File

SOURCE=..\include\LtyError.h
# End Source File
# Begin Source File

SOURCE=..\include\LtyLog.h
# End Source File
# Begin Source File

SOURCE=..\include\LtyMessage.h
# End Source File
# Begin Source File

SOURCE=..\include\LtyTimer.h
# End Source File
# Begin Source File

SOURCE=..\include\LtyTypeDef.h
# End Source File
# Begin Source File

SOURCE=..\include\Main_City.h
# End Source File
# Begin Source File

SOURCE=..\MYDES.h
# End Source File
# Begin Source File

SOURCE=..\SlDemo\ProcCharacter.h
# End Source File
# Begin Source File

SOURCE=..\inputmisc\PsamCard.h
# End Source File
# Begin Source File

SOURCE=..\PSAMLib.h
# End Source File
# Begin Source File

SOURCE=\P2new\QRCodeMain.h
# End Source File
# Begin Source File

SOURCE=\P2new\QRScan.h
# End Source File
# Begin Source File

SOURCE=..\RecordFile.h
# End Source File
# Begin Source File

SOURCE=..\SL8583.h
# End Source File
# Begin Source File

SOURCE=..\include\SlzrError.h
# End Source File
# Begin Source File

SOURCE=..\include\SlzrTypeDef.h
# End Source File
# Begin Source File

SOURCE=..\include\StructDef.h
# End Source File
# Begin Source File

SOURCE=..\include\szct.h
# End Source File
# Begin Source File

SOURCE=..\inputmisc\Uart.h
# End Source File
# Begin Source File

SOURCE=..\inputmisc\WatchDog.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=..\Makefile
# End Source File
# End Target
# End Project
