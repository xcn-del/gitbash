@echo off
REM Build notepad.py into exe
REM Requirements: Python 3.6+ and PyInstaller

echo Building Notepad.exe...
echo.

REM Install PyInstaller if not already installed
python -m pip install pyinstaller

REM Build the exe
REM --onefile: creates a single exe file
REM --windowed: no console window
REM --name: output executable name
REM --icon: optional icon file
pyinstaller --onefile --windowed --name=Notepad --add-data "." notepad.py

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Build successful!
    echo The exe file is located at: dist\Notepad.exe
) else (
    echo.
    echo Build failed!
    pause
)

pause
