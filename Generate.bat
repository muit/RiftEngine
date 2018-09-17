@echo off
call "Sharpmake/Sharpmake.Application.exe" /sources(@"FecoEngine.Solution.cs")
if errorlevel 1  (
    pause
)
exit