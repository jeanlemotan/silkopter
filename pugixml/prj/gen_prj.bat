@echo off 
SET MAKETOOL=..\tools\premake\release\premake5.exe
%MAKETOOL% --arch=x86 --to=vs2013\x86 vs2013
