# Microsoft Developer Studio Project File - Name="PPMD" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=PPMD - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PPMD.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PPMD.mak" CFG="PPMD - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PPMD - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PPMD - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PPMD - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PPMD_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PPMD_EXPORTS" /Yu"StdAfx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"C:\Program Files\7-ZIP\Compress\PPM\PPMd.dll" /opt:NOWIN98
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "PPMD - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PPMD_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PPMD_EXPORTS" /Yu"StdAfx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"C:\Program Files\7-ZIP\Compress\PPM\PPMd.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "PPMD - Win32 Release"
# Name "PPMD - Win32 Debug"
# Begin Group "Spec"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DllExports.cpp
# End Source File
# Begin Source File

SOURCE=.\PPMD.def
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Algo"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AriConst.h
# End Source File
# Begin Source File

SOURCE=.\Context.h
# End Source File
# Begin Source File

SOURCE=.\Decode.h
# End Source File
# Begin Source File

SOURCE=.\Decoder.cpp
# End Source File
# Begin Source File

SOURCE=.\Decoder.h
# End Source File
# Begin Source File

SOURCE=.\Encode.h
# End Source File
# Begin Source File

SOURCE=.\Encoder.cpp
# End Source File
# Begin Source File

SOURCE=.\Encoder.h
# End Source File
# Begin Source File

SOURCE=.\PPMDType.h
# End Source File
# Begin Source File

SOURCE=.\SubAlloc.h
# End Source File
# End Group
# Begin Group "Stream"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\Sdk\Stream\InByte.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Sdk\Stream\InByte.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Sdk\Stream\OutByte.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Sdk\Stream\OutByte.h
# End Source File
# End Group
# Begin Group "Interface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Interface\CompressInterface.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Sdk\Interface\ICoder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Sdk\Interface\IInOutStreams.h
# End Source File
# End Group
# Begin Group "Arithmetic"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\SDK\Compression\AriBitCoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Compression\AriBitCoder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Compression\AriPrice.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Compression\RangeCoder.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\resource.rc
# End Source File
# End Target
# End Project
