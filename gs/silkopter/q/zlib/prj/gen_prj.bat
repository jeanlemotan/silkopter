@echo off 
if "%MAKETOOL%" == "" SET MAKETOOL=premake4.exe 
%MAKETOOL% vs2012 %1 %2 %3 %4 %5 %6 %7 %8 %9
%MAKETOOL% --platform=iphone xcode4 %1 %2 %3 %4 %5 %6 %7 %8 %9

