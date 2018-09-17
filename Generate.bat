@echo off
call "Source/ThirdParty/Sharpmake/Sharpmake.Application.exe" /sources(@"FecoEngine.Solution.cs")
if errorlevel 0  (
    exit
)
pause
exit