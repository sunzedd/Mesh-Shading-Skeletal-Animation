@echo off
del *.sln
del *.vcxproj
del *.vcxproj.user
del *.vcxproj.filters
call premake5.exe vs2019
PAUSE
