
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
for  /f  "tokens=*  delims=  "  %%a  IN  ('dir %EnginePath% /s/ad/b')  do robocopy %%a %EngineHeaderPath% *.h /e /NFL /NDL /NJH /NJS >nul

echo "3.Copy External library"
robocopy %ExternalPath%tbb\lib\vc14 %BinariesPath% *.lib /e /NFL /NDL /NJH /NJS >nul
robocopy %ExternalPath%tbb\dll\vc14 %BinariesPath% *.dll /e /NFL /NDL /NJH /NJS >nul

robocopy %ExternalPath%assimp-master\lib\Debug %BinariesPath% *.lib /e /NFL /NDL /NJH /NJS >nul
robocopy %ExternalPath%assimp-master\bin\Debug %BinariesPath% *.dll /e /NFL /NDL /NJH /NJS >nul

robocopy %ExternalPath%assimp-master\lib\RelWithDebInfo %BinariesPath% *.lib /e /NFL /NDL /NJH /NJS >nul 
robocopy %ExternalPath%assimp-master\bin\RelWithDebInfo %BinariesPath% *.dll /e /NFL /NDL /NJH /NJS >nul

robocopy %ExternalPath%DirectXTex\DirectXTex\Bin\Desktop_2022_Win10\x64\Debug %BinariesPath% *.lib /e /NFL /NDL /NJH /NJS >nul 
robocopy %ExternalPath%DirectXTex\DirectXTex\Bin\Desktop_2022_Win10\x64\Debug *.dll /e /NFL /NDL /NJH /NJS >nul

robocopy %ExternalPath%DirectXTex\DirectXTex\Bin\Desktop_2022_Win10\x64\Release %BinariesPath% *.lib /e /NFL /NDL /NJH /NJS >nul 
robocopy %ExternalPath%DirectXTex\DirectXTex\Bin\Desktop_2022_Win10\x64\Release %BinariesPath% *.dll /e /NFL /NDL /NJH /NJS >nul

exit 0