# Microsoft Developer Studio Generated NMAKE File, Based on Ambo.dsp
!IF "$(CFG)" == ""
CFG=Ambo - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Ambo - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Ambo - Win32 Release" && "$(CFG)" != "Ambo - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Ambo.mak" CFG="Ambo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Ambo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Ambo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Ambo - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : "..\Bin\Ambo.exe"

!ELSE 

ALL : "mlClass - Win32 Release" "..\Bin\Ambo.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"mlClass - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\Ambo.obj"
	-@erase "$(INTDIR)\Ambo.pch"
	-@erase "$(INTDIR)\Ambo.res"
	-@erase "$(INTDIR)\AmboConfigView.obj"
	-@erase "$(INTDIR)\AmboDoc.obj"
	-@erase "$(INTDIR)\AmboFrame.obj"
	-@erase "$(INTDIR)\AmboMatterView.obj"
	-@erase "$(INTDIR)\AmboOutBar.obj"
	-@erase "$(INTDIR)\AmboView.obj"
	-@erase "$(INTDIR)\AmboWorkBar.obj"
	-@erase "$(INTDIR)\build.lex.obj"
	-@erase "$(INTDIR)\build.tab.obj"
	-@erase "$(INTDIR)\MainFrame.obj"
	-@erase "$(INTDIR)\mlBuildView.obj"
	-@erase "$(INTDIR)\mlOutCtrl.obj"
	-@erase "$(INTDIR)\mlOutTab.obj"
	-@erase "$(INTDIR)\ProjectDoc.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\Bin\Ambo.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\Ambo.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\Ambo.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Ambo.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\Ambo.pdb" /machine:I386 /out:"../Bin/Ambo.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Ambo.obj" \
	"$(INTDIR)\AmboDoc.obj" \
	"$(INTDIR)\AmboFrame.obj" \
	"$(INTDIR)\AmboMatterView.obj" \
	"$(INTDIR)\AmboOutBar.obj" \
	"$(INTDIR)\AmboView.obj" \
	"$(INTDIR)\AmboWorkBar.obj" \
	"$(INTDIR)\MainFrame.obj" \
	"$(INTDIR)\mlBuildView.obj" \
	"$(INTDIR)\mlOutCtrl.obj" \
	"$(INTDIR)\mlOutTab.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Ambo.res" \
	"$(INTDIR)\AmboConfigView.obj" \
	"$(INTDIR)\build.tab.obj" \
	"$(INTDIR)\build.lex.obj" \
	"$(INTDIR)\ProjectDoc.obj" \
	"..\mlClass\Release\mlClass.lib"

"..\Bin\Ambo.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Ambo - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\Bin\Ambo.exe" "$(OUTDIR)\Ambo.bsc"

!ELSE 

ALL : "mlClass - Win32 Debug" "..\Bin\Ambo.exe" "$(OUTDIR)\Ambo.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"mlClass - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\Ambo.obj"
	-@erase "$(INTDIR)\Ambo.pch"
	-@erase "$(INTDIR)\Ambo.res"
	-@erase "$(INTDIR)\Ambo.sbr"
	-@erase "$(INTDIR)\AmboConfigView.obj"
	-@erase "$(INTDIR)\AmboConfigView.sbr"
	-@erase "$(INTDIR)\AmboDoc.obj"
	-@erase "$(INTDIR)\AmboDoc.sbr"
	-@erase "$(INTDIR)\AmboFrame.obj"
	-@erase "$(INTDIR)\AmboFrame.sbr"
	-@erase "$(INTDIR)\AmboMatterView.obj"
	-@erase "$(INTDIR)\AmboMatterView.sbr"
	-@erase "$(INTDIR)\AmboOutBar.obj"
	-@erase "$(INTDIR)\AmboOutBar.sbr"
	-@erase "$(INTDIR)\AmboView.obj"
	-@erase "$(INTDIR)\AmboView.sbr"
	-@erase "$(INTDIR)\AmboWorkBar.obj"
	-@erase "$(INTDIR)\AmboWorkBar.sbr"
	-@erase "$(INTDIR)\build.lex.obj"
	-@erase "$(INTDIR)\build.lex.sbr"
	-@erase "$(INTDIR)\build.tab.obj"
	-@erase "$(INTDIR)\build.tab.sbr"
	-@erase "$(INTDIR)\MainFrame.obj"
	-@erase "$(INTDIR)\MainFrame.sbr"
	-@erase "$(INTDIR)\mlBuildView.obj"
	-@erase "$(INTDIR)\mlBuildView.sbr"
	-@erase "$(INTDIR)\mlOutCtrl.obj"
	-@erase "$(INTDIR)\mlOutCtrl.sbr"
	-@erase "$(INTDIR)\mlOutTab.obj"
	-@erase "$(INTDIR)\mlOutTab.sbr"
	-@erase "$(INTDIR)\ProjectDoc.obj"
	-@erase "$(INTDIR)\ProjectDoc.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Ambo.bsc"
	-@erase "$(OUTDIR)\Ambo.pdb"
	-@erase "..\Bin\Ambo.exe"
	-@erase "..\Bin\Ambo.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Ambo.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\Ambo.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Ambo.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Ambo.sbr" \
	"$(INTDIR)\AmboDoc.sbr" \
	"$(INTDIR)\AmboFrame.sbr" \
	"$(INTDIR)\AmboMatterView.sbr" \
	"$(INTDIR)\AmboOutBar.sbr" \
	"$(INTDIR)\AmboView.sbr" \
	"$(INTDIR)\AmboWorkBar.sbr" \
	"$(INTDIR)\MainFrame.sbr" \
	"$(INTDIR)\mlBuildView.sbr" \
	"$(INTDIR)\mlOutCtrl.sbr" \
	"$(INTDIR)\mlOutTab.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\AmboConfigView.sbr" \
	"$(INTDIR)\build.tab.sbr" \
	"$(INTDIR)\build.lex.sbr" \
	"$(INTDIR)\ProjectDoc.sbr"

"$(OUTDIR)\Ambo.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=../mlClass/Debug/mlClass.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\Ambo.pdb" /debug /machine:I386 /out:"../Bin/Ambo.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\Ambo.obj" \
	"$(INTDIR)\AmboDoc.obj" \
	"$(INTDIR)\AmboFrame.obj" \
	"$(INTDIR)\AmboMatterView.obj" \
	"$(INTDIR)\AmboOutBar.obj" \
	"$(INTDIR)\AmboView.obj" \
	"$(INTDIR)\AmboWorkBar.obj" \
	"$(INTDIR)\MainFrame.obj" \
	"$(INTDIR)\mlBuildView.obj" \
	"$(INTDIR)\mlOutCtrl.obj" \
	"$(INTDIR)\mlOutTab.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Ambo.res" \
	"$(INTDIR)\AmboConfigView.obj" \
	"$(INTDIR)\build.tab.obj" \
	"$(INTDIR)\build.lex.obj" \
	"$(INTDIR)\ProjectDoc.obj" \
	"..\mlClass\Debug\mlClass.lib"

"..\Bin\Ambo.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Ambo.dep")
!INCLUDE "Ambo.dep"
!ELSE 
!MESSAGE Warning: cannot find "Ambo.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Ambo - Win32 Release" || "$(CFG)" == "Ambo - Win32 Debug"
SOURCE=.\Ambo.cpp

!IF  "$(CFG)" == "Ambo - Win32 Release"


"$(INTDIR)\Ambo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ambo.pch"


!ELSEIF  "$(CFG)" == "Ambo - Win32 Debug"


"$(INTDIR)\Ambo.obj"	"$(INTDIR)\Ambo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ambo.pch"


!ENDIF 

SOURCE=.\AmboConfigView.cpp

!IF  "$(CFG)" == "Ambo - Win32 Release"


"$(INTDIR)\AmboConfigView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ambo.pch"


!ELSEIF  "$(CFG)" == "Ambo - Win32 Debug"


"$(INTDIR)\AmboConfigView.obj"	"$(INTDIR)\AmboConfigView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ambo.pch"


!ENDIF 

SOURCE=.\AmboDoc.cpp

!IF  "$(CFG)" == "Ambo - Win32 Release"


"$(INTDIR)\AmboDoc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ambo.pch"


!ELSEIF  "$(CFG)" == "Ambo - Win32 Debug"


"$(INTDIR)\AmboDoc.obj"	"$(INTDIR)\AmboDoc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ambo.pch"


!ENDIF 

SOURCE=.\AmboFrame.cpp

!IF  "$(CFG)" == "Ambo - Win32 Release"


"$(INTDIR)\AmboFrame.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ambo.pch"


!ELSEIF  "$(CFG)" == "Ambo - Win32 Debug"


"$(INTDIR)\AmboFrame.obj"	"$(INTDIR)\AmboFrame.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ambo.pch"


!ENDIF 

SOURCE=.\AmboMatterView.cpp

!IF  "$(CFG)" == "Ambo - Win32 Release"


"$(INTDIR)\AmboMatterView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ambo.pch"


!ELSEIF  "$(CFG)" == "Ambo - Win32 Debug"


"$(INTDIR)\AmboMatterView.obj"	"$(INTDIR)\AmboMatterView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ambo.pch"


!ENDIF 

SOURCE=.\AmboOutBar.cpp

!IF  "$(CFG)" == "Ambo - Win32 Release"


"$(INTDIR)\AmboOutBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ambo.pch"


!ELSEIF  "$(CFG)" == "Ambo - Win32 Debug"


"$(INTDIR)\AmboOutBar.obj"	"$(INTDIR)\AmboOutBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ambo.pch"


!ENDIF 

SOURCE=.\AmboView.cpp

!IF  "$(CFG)" == "Ambo - Win32 Release"


"$(INTDIR)\AmboView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ambo.pch"


!ELSEIF  "$(CFG)" == "Ambo - Win32 Debug"


"$(INTDIR)\AmboView.obj"	"$(INTDIR)\AmboView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ambo.pch"


!ENDIF 

SOURCE=.\AmboWorkBar.cpp

!IF  "$(CFG)" == "Ambo - Win32 Release"


"$(INTDIR)\AmboWorkBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ambo.pch"


!ELSEIF  "$(CFG)" == "Ambo - Win32 Debug"


"$(INTDIR)\AmboWorkBar.obj"	"$(INTDIR)\AmboWorkBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ambo.pch"


!ENDIF 

SOURCE=.\build.lex.cpp

!IF  "$(CFG)" == "Ambo - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\Ambo.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\build.lex.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ambo.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ambo - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\build.lex.obj"	"$(INTDIR)\build.lex.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\build.tab.cpp

!IF  "$(CFG)" == "Ambo - Win32 Release"


"$(INTDIR)\build.tab.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ambo.pch"


!ELSEIF  "$(CFG)" == "Ambo - Win32 Debug"


"$(INTDIR)\build.tab.obj"	"$(INTDIR)\build.tab.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ambo.pch"


!ENDIF 

SOURCE=.\MainFrame.cpp

!IF  "$(CFG)" == "Ambo - Win32 Release"


"$(INTDIR)\MainFrame.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ambo.pch"


!ELSEIF  "$(CFG)" == "Ambo - Win32 Debug"


"$(INTDIR)\MainFrame.obj"	"$(INTDIR)\MainFrame.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ambo.pch"


!ENDIF 

SOURCE=.\mlBuildView.cpp

!IF  "$(CFG)" == "Ambo - Win32 Release"


"$(INTDIR)\mlBuildView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ambo.pch"


!ELSEIF  "$(CFG)" == "Ambo - Win32 Debug"


"$(INTDIR)\mlBuildView.obj"	"$(INTDIR)\mlBuildView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ambo.pch"


!ENDIF 

SOURCE=.\mlOutCtrl.cpp

!IF  "$(CFG)" == "Ambo - Win32 Release"


"$(INTDIR)\mlOutCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ambo.pch"


!ELSEIF  "$(CFG)" == "Ambo - Win32 Debug"


"$(INTDIR)\mlOutCtrl.obj"	"$(INTDIR)\mlOutCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ambo.pch"


!ENDIF 

SOURCE=.\mlOutTab.cpp

!IF  "$(CFG)" == "Ambo - Win32 Release"


"$(INTDIR)\mlOutTab.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ambo.pch"


!ELSEIF  "$(CFG)" == "Ambo - Win32 Debug"


"$(INTDIR)\mlOutTab.obj"	"$(INTDIR)\mlOutTab.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ambo.pch"


!ENDIF 

SOURCE=.\ProjectDoc.cpp

!IF  "$(CFG)" == "Ambo - Win32 Release"


"$(INTDIR)\ProjectDoc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ambo.pch"


!ELSEIF  "$(CFG)" == "Ambo - Win32 Debug"


"$(INTDIR)\ProjectDoc.obj"	"$(INTDIR)\ProjectDoc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Ambo.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "Ambo - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\Ambo.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Ambo.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Ambo - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Ambo.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\Ambo.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Ambo.rc

"$(INTDIR)\Ambo.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\build.l

!IF  "$(CFG)" == "Ambo - Win32 Release"

!ELSEIF  "$(CFG)" == "Ambo - Win32 Debug"

InputPath=.\build.l
InputName=build

".\build.lex.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	flex -Pambo -t $(InputName).l > $(InputName).lex.cpp
<< 
	

!ENDIF 

SOURCE=.\build.y

!IF  "$(CFG)" == "Ambo - Win32 Release"

!ELSEIF  "$(CFG)" == "Ambo - Win32 Debug"

InputPath=.\build.y
InputName=build

".\build.tab.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	bison.exe --file-prefix=ambo --name-prefix=ambo -d -o $(InputName).tab.cpp $(InputName).y
<< 
	

!ENDIF 

!IF  "$(CFG)" == "Ambo - Win32 Release"

"mlClass - Win32 Release" : 
   cd "\Ambo\mlClass"
   $(MAKE) /$(MAKEFLAGS) /F .\mlClass.mak CFG="mlClass - Win32 Release" 
   cd "..\Builder"

"mlClass - Win32 ReleaseCLEAN" : 
   cd "\Ambo\mlClass"
   $(MAKE) /$(MAKEFLAGS) /F .\mlClass.mak CFG="mlClass - Win32 Release" RECURSE=1 CLEAN 
   cd "..\Builder"

!ELSEIF  "$(CFG)" == "Ambo - Win32 Debug"

"mlClass - Win32 Debug" : 
   cd "\Ambo\mlClass"
   $(MAKE) /$(MAKEFLAGS) /F .\mlClass.mak CFG="mlClass - Win32 Debug" 
   cd "..\Builder"

"mlClass - Win32 DebugCLEAN" : 
   cd "\Ambo\mlClass"
   $(MAKE) /$(MAKEFLAGS) /F .\mlClass.mak CFG="mlClass - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\Builder"

!ENDIF 


!ENDIF 

