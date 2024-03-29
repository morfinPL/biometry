# Biometry ![CI](https://github.com/morfinPL/biometry/workflows/CI/badge.svg) [![codecov](https://codecov.io/gh/morfinPL/biometry/branch/master/graph/badge.svg)](https://codecov.io/gh/morfinPL/biometry)

## Pre-commit hook setup

Please install `cppcheck` in your system.

```powershell
python -m virtualenv .venv
.venv\Scripts\activate
python -m pip install --upgrade pip
pip install pre-commit lizard
pre-commit install
```

## Requirements
- [CMake](https://cmake.org/download/) greater than 3.25.2,
- [Catch v3.3.1](https://github.com/catchorg/Catch2/tree/v3.3.1),
- [OpenCV 4.7.0](https://github.com/opencv/opencv/tree/4.7.0) with [contribs](https://github.com/opencv/opencv_contrib/tree/4.7.0).

## Installation of requirements

### CMake

Just install by installer check if it is in path.

### Catch

Windows version:

```
git clone https://github.com/catchorg/Catch2.git
cd Catch2
git checkout v3.3.1
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 -D CMAKE_INSTALL_PREFIX:PATH=YOUR_CATCH_INSTALL_DIR ..
cmake --build . --config Debug
cmake --build . --config Debug --target INSTALL
cmake --build . --config Release
cmake --build . --config Release --target INSTALL
```
To be precise **-G** flag is a generator, **-A** is selected architecture. This is only example and you can choose some different [option](https://cmake.org/cmake/help/v3.25/manual/cmake-generators.7.html#cmake-generators). At the end you must set environmental variable **CATCH_DIR** to **YOUR_CATCH_INSTALL_DIR**. Now you can delete build and cloned repository.

### OpenCV

Windows version:

```
git clone https://github.com/opencv/opencv_contrib.git
cd opencv_contrib
git checkout 4.7.0
cd ..
git clone https://github.com/opencv/opencv.git
cd opencv
git checkout 4.7.0
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ^
-D CMAKE_INSTALL_PREFIX=YOUR_OPENCV_INSTALL_DIR ^
-D INSTALL_PYTHON_EXAMPLES=OFF ^
-D INSTALL_C_EXAMPLES=OFF ^
-D WITH_FFMPEG=ON ^
-D WITH_TBB=ON ^
-D INSTALL_PYTHON_EXAMPLES=OFF ^
-D BUILD_EXAMPLES=OFF ^
-D BUILD_DOCS=OFF ^
-D BUILD_PERF_TESTS=OFF ^
-D BUILD_TESTS=OFF ^
-D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules ..
cmake --build . --config Debug
cmake --build . --config Debug --target INSTALL
cmake --build . --config Release
cmake --build . --config Release --target INSTALL
```

At the end you must set environmental variable **OPENCV_DIR** to **YOUR_OPENCV_INSTALL_DIR**. You also should add **%OPENCV_DIR%\x64\vc17\bin** to path, but you must check **x64\vc17** part, it depends on Visual Studio version and selected architecture. This directory, in path, enables dynamic linking of OpenCV. If you use other architecture Now you can delete build and cloned repositories.

## CMake configuration of a project

To create project (in case of Visual Studio this command produce VS Solution), you should create directory **build** in your repository directory and use command:

```
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 `
    -D LINKAGE=SHARED `
    -D 3RDPARTY_LIBRARIES="Catch2;OpenCV" `
    -D 3RDPARTY_LIBRARIES_VERSIONS="3.3.1;4.7.0" `
    -S .. -B .
```

## Build a project

After successful configuration you can open Biometry.sln and work with Visual Studio or use command line:

```
cmake --build . --config=[Release|Debug]
```

## Test project

After successful build you can run tests:

```
ctest -C [Release|Debug] .
```

## Run FaceDetection

Our sample app makes classification of faces, implemented method is based on Local Binary Patterns Histograms(see [Face Recognition with OpenCV](https://docs.opencv.org/4.2.0/da/d60/tutorial_face_main.html)) and Nearest Neighbor Classification. To run it with Georgia Tech dataset available with our repository you must run this executable like this:
```
.\FaceDetection.exe --dataset ..\..\..\testData\GeorgiaTechDatabaseCropped --extension .jpg --testNumber 2 --debug 1
.\FaceDetection.exe --dataset ..\..\..\testData\yalefaces --extension .png --testNumber 2 --debug 1
```
With above configuration we get 78 % of successful classification!
For YaleFace dataset also shipped with our repo we get 90 %!

## Run BaseFrequencyDetector

Our sample app finds base frequencies in some wav file. To run it you must use the following command line:
```
.\BaseFrequencyDetector.exe --input ..\..\..\testData\sound\abrakadabra --window 1024
```
It prints out founded base frequencies in windows of given size of samples. It use two methods, autocorrelation and fourier analysis. Window size should be number which is some power of two, because of use dfft.
