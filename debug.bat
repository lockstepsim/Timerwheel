@echo off
setlocal EnableDelayedExpansion

set BUILD_CACHE=%~dp0\_build_cache.cmd

if exist "!BUILD_CACHE!" (
  rem cache file exists, so call it to set env variables very fast
  call "!BUILD_CACHE!"
) else (
  if not exist "%VS2019INSTALLDIR%\VC\Auxiliary\Build\vcvarsall.bat" (
    echo Visual Studio 2019 not found or installed version is to old.
    exit /b 1
  )
  rem cache file does not exist, get env variables in slow way
  call "%VS2019INSTALLDIR%\VC\Auxiliary\Build\vcvarsall.bat" amd64
  echo set PATH=!PATH!> "!BUILD_CACHE!"
  echo set INCLUDE=!INCLUDE!>> "!BUILD_CACHE!"
  echo set LIB=!LIB!>> "!BUILD_CACHE!"

  rem Depending on whether you are build .NET or other stuff, there are more
  rem env variables you might want to add to cache, like:
  rem Platform, FrameworkDir, NETFXSDKDir, WindowsSdkDir, WindowsSDKVersion, VCINSTALLDIR, ...
)

rem put your build commands here

devenv build\main.exe