@echo off

REM prepare the script to be run from another directory
pushd %~dp0\..

REM create obj and bin folders if non exiting, since
REM the development tools will not create them themselves
if not exist obj mkdir obj
if not exist bin mkdir bin

echo.
echo Import embedded data files
echo --------------------------
tiled2vircon assets\level1.tmx -o obj\ || goto :failed

echo.
echo Convert the PNG textures
echo --------------------------
png2vircon assets\pixel.png -o obj\pixel.vtex || goto :failed
png2vircon assets\walls.png -o obj\walls.vtex || goto :failed

echo.
echo Compile the C code
echo --------------------------
compile src\main.c -o obj\main.asm || goto :failed

echo.
echo Assemble the ASM code
echo --------------------------
assemble obj\main.asm -o obj\main.vbin || goto :failed

echo.
echo Pack the ROM
echo --------------------------
packrom Virconstein3D.xml -o bin\Virconstein3D.v32 || goto :failed
goto :succeeded

:failed
popd
echo.
echo BUILD FAILED
exit /b %errorlevel%

:succeeded
popd
echo.
echo BUILD SUCCESSFUL
exit /b

@echo on