@echo off
echo Building Vortex Indexer...

if not exist build mkdir build

g++ -std=c++20 -I include src/*.cpp -o build/vortex_indexer.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo [SUCCESS] Build complete.
    echo Run the indexer with: build\vortex_indexer.exe data
) else (
    echo.
    echo [ERROR] Build failed. Please check the errors above.
)
