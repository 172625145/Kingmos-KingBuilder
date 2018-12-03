# Microsoft Developer Studio Project File - Name="KingBuilder" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=KingBuilder - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "KingBuilder.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "KingBuilder.mak" CFG="KingBuilder - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "KingBuilder - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "KingBuilder - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "KingBuilder - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"../Bin/KingBuilder.exe"

!ELSEIF  "$(CFG)" == "KingBuilder - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x404 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../mlClass/debug/Kingmosclass.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../bin/kingbuilder.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "KingBuilder - Win32 Release"
# Name "KingBuilder - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ActiveSoftWare.cpp
# End Source File
# Begin Source File

SOURCE=.\AmboConfigView.cpp
# End Source File
# Begin Source File

SOURCE=.\AmboDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\AmboFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\AmboMatterView.cpp
# End Source File
# Begin Source File

SOURCE=.\AmboOutBar.cpp
# End Source File
# Begin Source File

SOURCE=.\AmboView.cpp
# End Source File
# Begin Source File

SOURCE=.\AmboWorkBar.cpp
# End Source File
# Begin Source File

SOURCE=.\BSPConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\CommandBar.cpp
# End Source File
# Begin Source File

SOURCE=.\CPUAndBSPDirPage.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateSelectPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DirectoryList.cpp
# End Source File
# Begin Source File

SOURCE=.\EnvironmentSet.cpp
# End Source File
# Begin Source File

SOURCE=.\FileParse.cpp
# End Source File
# Begin Source File

SOURCE=.\FindInFiles.cpp
# End Source File
# Begin Source File

SOURCE=.\FindText.cpp
# End Source File
# Begin Source File

SOURCE=.\FormatEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\Grep.cpp
# End Source File
# Begin Source File

SOURCE=.\InputPage.cpp
# End Source File
# Begin Source File

SOURCE=.\KingBuilder.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\MessageView.cpp
# End Source File
# Begin Source File

SOURCE=.\mlControlBar.cpp
# End Source File
# Begin Source File

SOURCE=.\mlOutTab.cpp
# End Source File
# Begin Source File

SOURCE=.\mlViewTab.cpp
# End Source File
# Begin Source File

SOURCE=.\ModuleRead.cpp
# End Source File
# Begin Source File

SOURCE=.\ModuleSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\MyFileList.cpp
# End Source File
# Begin Source File

SOURCE=.\NewBSPPage.cpp
# End Source File
# Begin Source File

SOURCE=.\NewFolder.cpp
# End Source File
# Begin Source File

SOURCE=.\NewPlatform_Setp1.cpp
# End Source File
# Begin Source File

SOURCE=.\NewPlatform_Setp2.cpp
# End Source File
# Begin Source File

SOURCE=.\NewPlatform_Setp3.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputMessageEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputView.cpp
# End Source File
# Begin Source File

SOURCE=.\PlatformSet_Page1.cpp
# End Source File
# Begin Source File

SOURCE=.\PlatformSet_Page2.cpp
# End Source File
# Begin Source File

SOURCE=.\PlatformSet_page3.cpp
# End Source File
# Begin Source File

SOURCE=.\PlatformSet_page4.cpp
# End Source File
# Begin Source File

SOURCE=.\PlatformSet_page5.cpp
# End Source File
# Begin Source File

SOURCE=.\PlatformSet_page6.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectView.cpp
# End Source File
# Begin Source File

SOURCE=.\RecentStringList.cpp
# End Source File
# Begin Source File

SOURCE=.\ReplaceText.cpp
# End Source File
# Begin Source File

SOURCE=.\SetEnv.cpp
# End Source File
# Begin Source File

SOURCE=.\SetSysColor.cpp
# End Source File
# Begin Source File

SOURCE=.\SpecialEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\SpecialView.cpp
# End Source File
# Begin Source File

SOURCE=.\srcData.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SysOption.cpp
# End Source File
# Begin Source File

SOURCE=.\SysSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\TradeMarkFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\WorkSpaceInfo.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ActiveSoftWare.h
# End Source File
# Begin Source File

SOURCE=.\AmboConfigView.h
# End Source File
# Begin Source File

SOURCE=.\AmboDoc.h
# End Source File
# Begin Source File

SOURCE=.\AmboFrame.h
# End Source File
# Begin Source File

SOURCE=.\AmboMatterView.h
# End Source File
# Begin Source File

SOURCE=.\AmboOutBar.h
# End Source File
# Begin Source File

SOURCE=.\AmboView.h
# End Source File
# Begin Source File

SOURCE=.\AmboWorkBar.h
# End Source File
# Begin Source File

SOURCE=.\BSPConfig.h
# End Source File
# Begin Source File

SOURCE=.\build.tab.cpp.h
# End Source File
# Begin Source File

SOURCE=.\BuildView.h
# End Source File
# Begin Source File

SOURCE=.\CommandBar.h
# End Source File
# Begin Source File

SOURCE=.\CPUAndBSPDirPage.h
# End Source File
# Begin Source File

SOURCE=.\CreateInput.h
# End Source File
# Begin Source File

SOURCE=.\CreateInputPage.h
# End Source File
# Begin Source File

SOURCE=.\CreateSelectPage.h
# End Source File
# Begin Source File

SOURCE=.\CreateWorkSpaces.h
# End Source File
# Begin Source File

SOURCE=.\DirectoryList.h
# End Source File
# Begin Source File

SOURCE=.\EnvironmentSet.h
# End Source File
# Begin Source File

SOURCE=.\FileParse.h
# End Source File
# Begin Source File

SOURCE=.\FileView.h
# End Source File
# Begin Source File

SOURCE=.\FindInFiles.h
# End Source File
# Begin Source File

SOURCE=.\FindText.h
# End Source File
# Begin Source File

SOURCE=.\Grep.h
# End Source File
# Begin Source File

SOURCE=.\Infomation.h
# End Source File
# Begin Source File

SOURCE=.\InputPage.h
# End Source File
# Begin Source File

SOURCE=.\KingBuilder.h
# End Source File
# Begin Source File

SOURCE=.\MainFrame.h
# End Source File
# Begin Source File

SOURCE=.\MessageView.h
# End Source File
# Begin Source File

SOURCE=.\mlControlBar.h
# End Source File
# Begin Source File

SOURCE=.\mlOutCtrl.h
# End Source File
# Begin Source File

SOURCE=.\mlOutTab.h
# End Source File
# Begin Source File

SOURCE=.\mlViewTab.h
# End Source File
# Begin Source File

SOURCE=.\ModuleRead.h
# End Source File
# Begin Source File

SOURCE=.\ModuleSelect.h
# End Source File
# Begin Source File

SOURCE=.\MyFileList.h
# End Source File
# Begin Source File

SOURCE=.\NewBSPPage.h
# End Source File
# Begin Source File

SOURCE=.\NewFolder.h
# End Source File
# Begin Source File

SOURCE=.\NewPlatform_Setp1.h
# End Source File
# Begin Source File

SOURCE=.\NewPlatform_Setp2.h
# End Source File
# Begin Source File

SOURCE=.\NewPlatform_Setp3.h
# End Source File
# Begin Source File

SOURCE=.\OutputDebugMessage.h
# End Source File
# Begin Source File

SOURCE=.\OutputMessageEdit.h
# End Source File
# Begin Source File

SOURCE=.\OutputView.h
# End Source File
# Begin Source File

SOURCE=.\PlatformSet_Page1.h
# End Source File
# Begin Source File

SOURCE=.\PlatformSet_Page2.h
# End Source File
# Begin Source File

SOURCE=.\PlatformSet_page3.h
# End Source File
# Begin Source File

SOURCE=.\PlatformSet_page4.h
# End Source File
# Begin Source File

SOURCE=.\PlatformSet_page5.h
# End Source File
# Begin Source File

SOURCE=.\PlatformSet_page6.h
# End Source File
# Begin Source File

SOURCE=.\ProjectDoc.h
# End Source File
# Begin Source File

SOURCE=.\ProjectInfo.h
# End Source File
# Begin Source File

SOURCE=.\ProjectView.h
# End Source File
# Begin Source File

SOURCE=.\RecentStringList.h
# End Source File
# Begin Source File

SOURCE=.\ReplaceText.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SetEnv.h
# End Source File
# Begin Source File

SOURCE=.\SetSysColor.h
# End Source File
# Begin Source File

SOURCE=.\SpecialEdit.h
# End Source File
# Begin Source File

SOURCE=.\SpecialView.h
# End Source File
# Begin Source File

SOURCE=.\srcData.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SysOption.h
# End Source File
# Begin Source File

SOURCE=.\SysSetting.h
# End Source File
# Begin Source File

SOURCE=.\TradeMarkFrame.h
# End Source File
# Begin Source File

SOURCE=.\WorkSpaceInfo.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\1setup.bmp
# End Source File
# Begin Source File

SOURCE=.\res\AmboDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\configview.ico
# End Source File
# Begin Source File

SOURCE=..\mlClass\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\delete.bmp
# End Source File
# Begin Source File

SOURCE=.\res\directory.ico
# End Source File
# Begin Source File

SOURCE=.\res\downline.bmp
# End Source File
# Begin Source File

SOURCE=.\res\editbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\group.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\iconfo.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_ambo.ico
# End Source File
# Begin Source File

SOURCE=.\res\KingBuilder.ico
# End Source File
# Begin Source File

SOURCE=.\KingBuilder.rc
# End Source File
# Begin Source File

SOURCE=.\res\list.ico
# End Source File
# Begin Source File

SOURCE=.\matterview.ico
# End Source File
# Begin Source File

SOURCE=.\res\module.ico
# End Source File
# Begin Source File

SOURCE=.\res\setup.bmp
# End Source File
# Begin Source File

SOURCE=.\res\source.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tracemar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\upline.bmp
# End Source File
# End Group
# Begin Group "Parser Files"

# PROP Default_Filter "*.l;*.y"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
