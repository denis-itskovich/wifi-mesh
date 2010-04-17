@echo off

set DstDir=.\Moc
set Target=%DstDir%\Mocs.cpp
set Mocs=%DstDir%\moc_*.cpp
set MocUtil=%QTDIR%\bin\moc.exe

echo Building MOC files...
echo // MOC files includer > %Target%
del /q %Mocs% 1> nul 2> nul
del %Target% 1> nul 2> nul
for /r %%f in (*.h) do %MocUtil% -nw -o %DstDir%\moc_%%~nf.cpp %%f
for %%f in (%Mocs%) do echo #include "%%~nf.cpp" >> %Target%
