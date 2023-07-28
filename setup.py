
import os
import sys
import subprocess
import multiprocessing

from pathlib import Path

from setuptools import setup, find_packages, Extension
from setuptools.command.build_ext import build_ext

__version__ = "0.2.21"
HERE = Path(__file__).resolve().parent


# A CMakeExtension needs a sourcedir instead of a file list.
# The name must be the _single_ output extension from the CMake build.
# If you need multiple extensions, see scikit-build.
class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=""):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))

        # required for auto-detection of auxiliary "native" libs
        if not extdir.endswith(os.path.sep):
            extdir += os.path.sep

        cfg = "Debug" if self.debug else "Release"

        cmake_args = [
            "-DDLPLAN_PYTHON=On",
            f"-DDLPLAN_VERSION_INFO={__version__}",
            f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={extdir}",
            "-DENABLE_TESTING:bool=false",
            f"-DPYTHON_EXECUTABLE={sys.executable}",
            f"-DCMAKE_BUILD_TYPE={cfg}",  # not used on MSVC, but no harm
        ]
        build_args = []
        build_args += ["--target", ext.name]

        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)

        subprocess.check_call(
            ["cmake", ext.sourcedir] + cmake_args, cwd=self.build_temp
        )
        subprocess.check_call(
            ["cmake", "--build", ".", f"-j{multiprocessing.cpu_count()}"] + build_args, cwd=self.build_temp
        )


# The information here can also be placed in setup.cfg - better separation of
# logic and declaration, and simpler if you include description/version in a file.
setup(
    name="dlplan",
    version=__version__,
    author="Dominik Drexler, Jendrik Seipp and Guillem Francès",
    author_email="dominik.drexler@liu.se",
    url="https://github.com/rleap-project/dlplan",
    description="A library for using description logics features in planning",
    long_description="",
    install_requires=["pybind11==2.10.4", "pybind11-global==2.10.4", "state_space_generator==0.1.8", "cmake>=3.16.3"],
    packages=find_packages(where="api/python/src"),
    package_dir={"": "api/python/src"},
    package_data={
        "": ["*.pyi"],
    },
    ext_modules=[CMakeExtension("_dlplan")],
    cmdclass={"build_ext": CMakeBuild},
    zip_safe=False,
    extras_require={
        'test': [
            'pytest',
        ],
    }
)
