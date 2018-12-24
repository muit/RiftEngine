@echo off
call "Programs/Sharpmake/Sharpmake.Application.exe" /sources(@"RiftEngine.Solution.cs")
if errorlevel 0  (
    exit
)
pause
exit