# Microsoft Developer Studio Project File - Name="winconv" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=winconv - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "conv.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "conv.mak" CFG="winconv - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "winconv - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "winconv - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "winconv - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\..\LIBS\SDL\SDL\include" /I "..\..\..\..\LIBS\zlib" /D "ZLIB_DLL" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x40b /d "NDEBUG"
# ADD RSC /l 0x40b /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "winconv - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\LIBS\SDL\SDL\include" /I "..\..\..\..\LIBS\zlib" /D "ZLIB_DLL" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x40b /d "_DEBUG"
# ADD RSC /l 0x40b /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "winconv - Win32 Release"
# Name "winconv - Win32 Debug"
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
# Begin Source File

SOURCE=..\conv.c
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

SOURCE=..\util\util.h
# End Source File
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
# End Group
# Begin Group "zlib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\LIBS\ZLIB\adler32.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\ZLIB\compress.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\ZLIB\crc32.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\ZLIB\deflate.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\ZLIB\deflate.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\ZLIB\gzio.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\ZLIB\infblock.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\ZLIB\infblock.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\ZLIB\infcodes.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\ZLIB\infcodes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\ZLIB\inffast.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\ZLIB\inffast.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\ZLIB\inffixed.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\ZLIB\inflate.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\ZLIB\inftrees.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\ZLIB\inftrees.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\ZLIB\infutil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\ZLIB\infutil.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\ZLIB\trees.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\ZLIB\trees.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\ZLIB\uncompr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\ZLIB\zconf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\ZLIB\zlib.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\ZLIB\zutil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\ZLIB\zutil.h
# End Source File
# End Group
# Begin Group "Library Files"

# PROP Default_Filter ""
# Begin Group "Debug"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\LIBS\SDL\SDL\VisualC\SDL\Debug\SDL.lib

!IF  "$(CFG)" == "winconv - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winconv - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\SDL\SDL\VisualC\SDLmain\Debug\SDLmain.lib

!IF  "$(CFG)" == "winconv - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winconv - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\ZLIB\Debug\zlib.lib

!IF  "$(CFG)" == "winconv - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winconv - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Release"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\LIBS\SDL\SDL\VisualC\SDL\Release\SDL.lib

!IF  "$(CFG)" == "winconv - Win32 Release"

!ELSEIF  "$(CFG)" == "winconv - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\SDL\SDL\VisualC\SDLmain\Release\SDLmain.lib

!IF  "$(CFG)" == "winconv - Win32 Release"

!ELSEIF  "$(CFG)" == "winconv - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\LIBS\ZLIB\Release\zlib.lib

!IF  "$(CFG)" == "winconv - Win32 Release"

!ELSEIF  "$(CFG)" == "winconv - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# End Group
# End Target
# End Project
