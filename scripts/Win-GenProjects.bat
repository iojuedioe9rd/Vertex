
pushd %~dp0\..\
echo %*
call vendor\premake\bin\premake5.exe vs2022
popd
IF %ERRORLEVEL% NEQ 0 (
  PAUSE
)

