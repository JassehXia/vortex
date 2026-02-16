@echo off
if not exist build_cmake mkdir build_cmake

echo Configuring CMake...
"C:\Program Files\CMake\bin\cmake.exe" -G "MinGW Makefiles" -S . -B build_cmake

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Building...
    "C:\Program Files\CMake\bin\cmake.exe" --build build_cmake
    
    if %ERRORLEVEL% EQU 0 (
        echo.
        echo [SUCCESS] Build complete. Run: build_cmake\vortex_indexer.exe data
    )
) else (
    echo [ERROR] CMake configuration failed.
)
