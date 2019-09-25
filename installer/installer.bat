@rem this file is outdated and need to be revised
@rem
@rem $Id: installer.bat,v 1.1 2003/04/15 18:15:48 tonic Exp $

md p
call cc @files.txt p
cd p
call upx kops.exe
touch *
cd ..
"d:\program files\nsis\makensis" installer.nsi
rd /q /s p
dir s*.exe