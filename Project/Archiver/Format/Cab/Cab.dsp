# Microsoft Developer Studio Project File - Name="Cab" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Cab - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Cab.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Cab.mak" CFG="Cab - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Cab - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Cab - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Cab - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CAB_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CAB_EXPORTS" /Yu"StdAfx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"C:\Program Files\7-ZIP\Format\Cab.dll" /opt:NOWIN98
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Cab - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CAB_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CAB_EXPORTS" /Yu"StdAfx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"C:\Program Files\7-ZIP\Format\Cab.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Cab - Win32 Release"
# Name "Cab - Win32 Debug"
# Begin Group "Spec"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Cab.def
# End Source File
# Begin Source File

SOURCE=.\DllExports.cpp
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\resource.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\SDK\Common\NewHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Common\NewHandler.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Common\String.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Common\StringConvert.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Common\StringConvert.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Common\Vector.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Common\Vector.h
# End Source File
# End Group
# Begin Group "Windows"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\SDK\Windows\COMTry.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Windows\PropVariant.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Windows\PropVariant.h
# End Source File
# End Group
# Begin Group "Interface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\SDK\Interface\ProgressUtils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Interface\ProgressUtils.h
# End Source File
# End Group
# Begin Group "Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\SDK\Archive\Cab\DataInByte.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Archive\Cab\DataInByte.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Archive\Cab\DeflateConst.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Archive\Cab\DeflateExtConst.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Archive\Cab\Header.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Archive\Cab\Header.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Archive\Cab\InEngine.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Archive\Cab\InEngine.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Archive\Cab\ItemInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Archive\Cab\ItemInfoEx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Archive\Cab\LZXBitDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Archive\Cab\LZXConst.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Archive\Cab\LZXDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Archive\Cab\LZXDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Archive\Cab\LZXExtConst.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Archive\Cab\LZXi86Converter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Archive\Cab\LZXi86Converter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Archive\Cab\MSZipDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Archive\Cab\MSZipDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Archive\Cab\StoreDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Archive\Cab\StoreDecoder.h
# End Source File
# End Group
# Begin Group "Stream"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\SDK\Stream\InByte.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Stream\InByte.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Stream\LSBFDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Stream\LSBFDecoder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Stream\OutByte.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Stream\OutByte.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Stream\WindowOut.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Stream\WindowOut.h
# End Source File
# End Group
# Begin Source File

SOURCE=".\7-cab.ico"
# End Source File
# Begin Source File

SOURCE=.\Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Handler.h
# End Source File
# End Target
# End Project
