@echo off
echo Compiling project_two...

:: Set paths
set MINGW_PATH=C:\mingw64\bin
set SRC_DIR=.\src\
set OUT_FILE=.\build\project_two.exe

:: Clear previous build output
if exist "%OUT_FILE%" del "%OUT_FILE%"

:: Clear previous build directory
if exist ".\build" (
    rmdir /s /q ".\build"
)

mkdir ".\build"

@REM :: Check for clean command
@REM if "%1"=="clean" (
@REM     del /Q build\project_two.exe
@REM     echo 🧹 Cleaned build directory.
@REM     exit /b 0
@REM )

:: Compile the C++ source files
"%MINGW_PATH%\g++" ^
    "%SRC_DIR%\project_two.cpp" ^
    "%SRC_DIR%\CSVparser.cpp" ^
    -std=c++17 -Wall -o "%OUT_FILE%"

:: Check if compilation succeeded
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ❌ Build failed.
    pause
    exit /b %ERRORLEVEL%
) else (
    echo.
    echo ✅ Build succeeded! Output: %OUT_FILE%
)

:: Generate sample test data
echo Generating test CSV files...
if not exist tests mkdir tests

(
    echo CourseID,CourseName
    echo CS101,Intro to Computer Science
    echo CS102,Data Structures
) > tests\test.csv

(
    echo ID,Name,Age
    echo 1,Alice,20
    echo 2,Bob,22
) > tests\simple.csv

echo CourseID,CourseName > tests\header_only.csv

(
    echo CourseID;CourseName
    echo CS101;Intro to Computer Science
) > tests\semicolon.csv

echo Test CSV files created in /tests

echo.
