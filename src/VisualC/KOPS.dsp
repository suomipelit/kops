# Microsoft Developer Studio Project File - Name="KOPS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=KOPS - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "KOPS.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "KOPS.mak" CFG="KOPS - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "KOPS - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "KOPS - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "KOPS - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /Ob2 /I "..\..\..\..\LIBS\SDL\SDL\include" /I "..\..\..\..\LIBS\fmod\api\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40b /d "NDEBUG"
# ADD RSC /l 0x417 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "KOPS - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\LIBS\SDL\SDL\include" /I "..\..\..\..\LIBS\fmod\api\inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40b /d "_DEBUG"
# ADD RSC /l 0x417 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"msvcrt.lib" /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "KOPS - Win32 Release"
# Name "KOPS - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "util Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\util\utilcore.c
# End Source File
# Begin Source File

SOURCE=..\util\utilfile.c
# End Source File
# End Group
# Begin Group "bot Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\kops-bot\reactive-agent\cannonfodder.c"
# End Source File
# Begin Source File

SOURCE="..\kops-bot\reactive-agent\kb-init.c"
# End Source File
# Begin Source File

SOURCE="..\kops-bot\reactive-agent\special-action.c"
# End Source File
# Begin Source File

SOURCE="..\kops-bot\reactive-agent\util.c"
# End Source File
# End Group
# Begin Source File

SOURCE=..\draw.c
# End Source File
# Begin Source File

SOURCE=..\game.c
# End Source File
# Begin Source File

SOURCE=..\gamefunc.c
# End Source File
# Begin Source File

SOURCE=..\init.c
# End Source File
# Begin Source File

SOURCE=..\kops.c
# End Source File
# Begin Source File

SOURCE=..\load.c
# End Source File
# Begin Source File

SOURCE=..\logo.c
# End Source File
# Begin Source File

SOURCE=..\menu.c
# End Source File
# Begin Source File

SOURCE=..\sound.c
# End Source File
# Begin Source File

SOURCE=..\spr.c
# End Source File
# Begin Source File

SOURCE=..\timer.c
# End Source File
# Begin Source File

SOURCE=..\weapon.c
# End Source File
# Begin Source File

SOURCE=..\wport.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "util Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\util\utilcore.h
# End Source File
# Begin Source File

SOURCE=..\util\utilfile.h
# End Source File
# Begin Source File

SOURCE=..\util\utilfile_sdl_wrapper.h
# End Source File
# Begin Source File

SOURCE=..\util\utilfile_wrapper.h
# End Source File
# Begin Source File

SOURCE=..\util\utilpic.h
# End Source File
# End Group
# Begin Group "bot Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\kops-bot\reactive-agent\cannonfodder.h"
# End Source File
# Begin Source File

SOURCE="..\kops-bot\reactive-agent\config.h"
# End Source File
# Begin Source File

SOURCE="..\kops-bot\reactive-agent\kb-init.h"
# End Source File
# Begin Source File

SOURCE="..\kops-bot\reactive-agent\util.h"
# End Source File
# End Group
# Begin Source File

SOURCE=..\draw.h
# End Source File
# Begin Source File

SOURCE=..\game.h
# End Source File
# Begin Source File

SOURCE=..\gamefunc.h
# End Source File
# Begin Source File

SOURCE=..\global.h
# End Source File
# Begin Source File

SOURCE=..\init.h
# End Source File
# Begin Source File

SOURCE=..\load.h
# End Source File
# Begin Source File

SOURCE=..\logo.h
# End Source File
# Begin Source File

SOURCE=..\menu.h
# End Source File
# Begin Source File

SOURCE=..\sound.h
# End Source File
# Begin Source File

SOURCE=..\spr.h
# End Source File
# Begin Source File

SOURCE=..\timer.h
# End Source File
# Begin Source File

SOURCE=..\version.h
# End Source File
# Begin Source File

SOURCE=..\wport.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\kops.rc
# End Source File
# End Group
# Begin Group "Library Files"

# PROP Default_Filter ""
# Begin Group "Debug"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\LIBS\SDL\SDL\VisualC\SDL\Debug\SDL.lib

!IF  "$(CFG)" == "KOPS - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "KOPS - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\SDL\SDL\VisualC\SDLmain\Debug\SDLmain.lib

!IF  "$(CFG)" == "KOPS - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "KOPS - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\ZLIB\Debug\zlib.lib

!IF  "$(CFG)" == "KOPS - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "KOPS - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Release"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\LIBS\SDL\SDL\VisualC\SDL\Release\SDL.lib

!IF  "$(CFG)" == "KOPS - Win32 Release"

!ELSEIF  "$(CFG)" == "KOPS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\SDL\SDL\VisualC\SDLmain\Release\SDLmain.lib

!IF  "$(CFG)" == "KOPS - Win32 Release"

!ELSEIF  "$(CFG)" == "KOPS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\ZLIB\Release\zlib.lib

!IF  "$(CFG)" == "KOPS - Win32 Release"

!ELSEIF  "$(CFG)" == "KOPS - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\LIBS\fmod\api\lib\fmodvc.lib
# End Source File
# End Group
# End Target
# End Project
