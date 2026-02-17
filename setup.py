import sys
from setuptools import setup, Extension
import pybind11

# Define the C++ extension module
ext_modules = [
    Extension(
        "vortex", # The name of the module Python will import
        sorted([
            "src/bindings.cpp", 
            "src/InvertedIndex.cpp"
        ]),
        include_dirs=[
            pybind11.get_include(),
            "include"
        ],
        language="c++",
        extra_compile_args=["/std:c++20"] if sys.platform == "win32" else ["-std=c++20"],
    ),
]

setup(
    name="vortex",
    version="0.1.0",
    description="High-performance C++ Search Engine Core",
    ext_modules=ext_modules,
    # This ensures zip_safe=False is set, which is required for C++ extensions
    zip_safe=False,
)
