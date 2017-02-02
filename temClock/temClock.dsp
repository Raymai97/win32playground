# Microsoft Developer Studio Project File - Name="temClock" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=temClock - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "temClock.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "temClock.mak" CFG="temClock - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "temClock - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "temClock - Win32 Release Win9x" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "temClock - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 msvcrt.lib kernel32.lib user32.lib gdi32.lib winmm.lib comdlg32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib /opt:nowin98
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "temClock - Win32 Release Win9x"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "temClock___Win32_Release_Win9x"
# PROP BASE Intermediate_Dir "temClock___Win32_Release_Win9x"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /FR /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 msvcrt.lib kernel32.lib user32.lib gdi32.lib winmm.lib comdlg32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib /opt:nowin98
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 msvcrt.lib kernel32.lib user32.lib gdi32.lib winmm.lib comdlg32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib /out:"Release/temClock_9x.exe" /opt:nowin98
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "temClock - Win32 Release"
# Name "temClock - Win32 Release Win9x"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DwmFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\MaiTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\Program.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DwmFrame.h
# End Source File
# Begin Source File

SOURCE=.\MaiTimer.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\default1.bin
# End Source File
# Begin Source File

SOURCE=.\res.rc
# End Source File
# End Group
# Begin Group "MyWinAPI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MyWinAPI\Create32bitBmp.c
# End Source File
# Begin Source File

SOURCE=.\MyWinAPI\Create32bitBmp.h
# End Source File
# Begin Source File

SOURCE=.\MyWinAPI\Dwm.c
# End Source File
# Begin Source File

SOURCE=.\MyWinAPI\Dwm.h
# End Source File
# Begin Source File

SOURCE=.\MyWinAPI\Uxtheme.c
# End Source File
# Begin Source File

SOURCE=.\MyWinAPI\Uxtheme.h
# End Source File
# End Group
# End Target
# End Project
