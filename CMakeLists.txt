cmake_minimum_required(VERSION 3.15)
project(vortex VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable compiler warnings for safety/learning
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Include directories
include_directories(include)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Standard Executable
add_executable(vortex_indexer ${SOURCES})

# --- Python Bindings (For IDE Support Only) ---
# Check if Python and pybind11 are available
find_package(Python3 COMPONENTS Interpreter Development)
execute_process(COMMAND "${Python3_EXECUTABLE}" -m pybind11 --cmake
                OUTPUT_VARIABLE pybind11_DIR
                OUTPUT_STRIP_TRAILING_WHITESPACE)

if (pybind11_DIR)
    list(APPEND CMAKE_PREFIX_PATH "${pybind11_DIR}")
    find_package(pybind11 CONFIG)
    
    if (pybind11_FOUND)
        pybind11_add_module(vortex_python src/bindings.cpp src/InvertedIndex.cpp)
        target_link_libraries(vortex_python PRIVATE vortex_indexer) # Link against main executable logic if static lib separated
    endif()
endif()
