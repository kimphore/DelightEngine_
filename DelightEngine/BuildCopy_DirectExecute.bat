
echo "1.Find Engine Root Folder"
set RootBase=""
IF NOT EXIST "DelightEngine.sln" (
    set RootBase="../"
)
for  /f  "tokens=*  delims=  "  %%a  IN  ('dir %RootBase% /s /b ^| find /i "DelightEngine.sln" ')  do  set result= %%a

set RootPath=%result:DelightEngine.sln=%
set EnginePath=%RootPath%DelightEngine\
set ExternalPath=%RootPath%External\
set BinariesPath=%RootPath%Binaries\
set EngineHeaderPath=%RootPath%EngineHeaders\

echo %RootPath%


echo "2.Copy Engine Headers"
for  /f  "tokens=*  delims=  "  %%a  IN  ('dir %EnginePath% /s/ad/b')  do robocopy %%a %EngineHeaderPath% *.h /e

echo "3.Copy External library"
robocopy %ExternalPath%tbb\lib\vc14 %BinariesPath% *.lib /e
robocopy %ExternalPath%tbb\dll\vc14 %BinariesPath% *.dll /e