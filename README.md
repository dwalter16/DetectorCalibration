# DetectorCalibration

This project provides tools to perform energy calibration of the various
detectors in the IRIS setup at TRIUMF. It consists of a shared library, which
can be linked to by user code or loaded into ROOT and used in macros, and an
executable which will perform the calibration automatically.

## Pre-requisites

To build this project you need a c++ compiler, a working installation of 
[ROOT](https://root.cern.ch) and cmake. Optionally, you need git to clone the
respository and doxygen to build the documentation. Except for ROOT, the
remaining pre-requisites can be installed on an Ubuntu system by the command
```
sudo apt-get install g++ cmake git doxygen
```

## Getting started

First step is to clone the project from the git repository. Open a terminal and
go to the folder where you want to build the project. Do
```
git clone https://github.com/dwalter16/DetectorCalibration.git
```
or 
```
git clone git@github.com:dwalter16/DetectorCalibration.git
```
or simply download the project as a zip-file and unzip it.

Then go to the project folder, create a build folder, configure and build the
project
```
  mkdir build
  cd build
  cmake ..
  make
```
If you also want to build the documentation, you can additionally do
```
make documentation
```
which will build the documentation and place the output in build/documentation.
You can browse the html documentation by opening the 'index.html' in your
browser.

##Using the automatic calibration program

After compilation the build-folder should contain the 'calibrate' executable.
Instructions on how to use the program can be obtained by running it with the
-h option.
```
$ ./calibrate -h
Usage: ./calibrate [OPTIONS] ... [DATAFILE]
Detector energy calibration based on the data in [DATAFILE].
Available options:

  -c [FILE]      Load configuration from [FILE] (required).
  -h             Print this message.
  -l [FILE]      Store logging information in [FILE] (optional).
  -o [FILE]      Store calibration coefficients in [FILE] (optional).
  -p [FILE]      Load pedestals from [FILE] (optional).

Example:
./calibrate -c /config/file -o /output/file /data/file
```
