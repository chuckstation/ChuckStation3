@echo off
rem
rem ChuckStation 3 - minimal Gradle wrapper for Windows.
rem Mirrors the bash gradlew script: uses gradle from PATH if available,
rem otherwise downloads the distribution specified in
rem gradle\wrapper\gradle-wrapper.properties.

setlocal enableextensions enabledelayedexpansion

set "SCRIPT_DIR=%~dp0"
set "SCRIPT_DIR=%SCRIPT_DIR:~0,-1%"
set "PROPS_FILE=%SCRIPT_DIR%\gradle\wrapper\gradle-wrapper.properties"

if not exist "%PROPS_FILE%" (
    echo ChuckStation3: missing %PROPS_FILE% 1>&2
    exit /b 1
)

rem Extract distributionUrl.
for /f "usebackq tokens=1,* delims==" %%a in ("%PROPS_FILE%") do (
    if "%%a"=="distributionUrl" set "DIST_URL=%%b"
)

if "%DIST_URL%"=="" (
    echo ChuckStation3: distributionUrl not set in %PROPS_FILE% 1>&2
    exit /b 1
)

rem Strip backslashes (Java .properties escaping).
set "DIST_URL=%DIST_URL:\=%"

if "%GRADLE_USER_HOME%"=="" set "GRADLE_USER_HOME=%USERPROFILE%\.gradle"
set "CACHE_DIR=%GRADLE_USER_HOME%\wrapper\dists"

rem Hash the URL (cheap, just for a stable dir name).
powershell -NoProfile -Command "Set-Content -Path '%TEMP%\cs3_url.txt' -Value '%DIST_URL%' -NoNewline"
for /f "delims=" %%i in ('certutil -hashfile "%TEMP%\cs3_url.txt" SHA1 ^| find /v ":" ^| find /v "CertUtil"') do set "DIST_HASH=%%i"
set "DIST_HASH=%DIST_HASH: =%"
del "%TEMP%\cs3_url.txt" 2>nul

for %%F in ("%DIST_URL%") do set "DIST_BASENAME=%%~nxF"
set "DIST_NAME=%DIST_BASENAME:-bin.zip=%"
set "INSTALL_DIR=%CACHE_DIR%\%DIST_HASH%\%DIST_NAME%"

if not exist "%INSTALL_DIR%\bin\gradle.bat" (
    echo ChuckStation3: downloading %DIST_BASENAME%... 1>&2
    set "ZIP_PATH=%CACHE_DIR%\%DIST_HASH%\%DIST_BASENAME%"
    if not exist "%CACHE_DIR%\%DIST_HASH%" mkdir "%CACHE_DIR%\%DIST_HASH%"
    powershell -NoProfile -Command "Invoke-WebRequest -Uri '%DIST_URL%' -OutFile '%CACHE_DIR%\%DIST_HASH%\%DIST_BASENAME%' -UseBasicParsing"
    powershell -NoProfile -Command "Expand-Archive -Path '%CACHE_DIR%\%DIST_HASH%\%DIST_BASENAME%' -DestinationPath '%CACHE_DIR%\%DIST_HASH%\tmp' -Force"
    if exist "%INSTALL_DIR%" rmdir /s /q "%INSTALL_DIR%"
    move "%CACHE_DIR%\%DIST_HASH%\tmp\gradle-*" "%INSTALL_DIR%" >nul
    rmdir /s /q "%CACHE_DIR%\%DIST_HASH%\tmp"
)

"%INSTALL_DIR%\bin\gradle.bat" %*
