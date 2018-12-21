@echo off
call "Programs/Sharpmake/Sharpmake.Application.exe" /sources(@"RiftEngine.Solution.cs")
if errorlevel 0  (
    pause
    exit
)
pause
exit