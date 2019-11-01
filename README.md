# DetectorCalibration

This project provides tools to perform energy calibration of the various
detectors in the IRIS setup at TRIUMF. It consists of a shared library, which
can be linked to by user code or loaded into ROOT and used in macros, and an
executable which will perform the calibration automatically.

## Pre-requisites

To build this project you need a c++ compiler, a working installation of 
[ROOT](https://root.cern.ch) and cmake. Optionally, you need git to clone the
repository and doxygen to build the documentation. Except for ROOT, the
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

## Using the automatic calibration program

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

### The configuration file
The configuration file is, together with a data file, the only required input to
the calibration program. An example configuration file is provided in the
examples-folder. It could look like this:
```
DETECTOR
  TYPE YY1Array
  POSZ 90
  ROTY 180
  ROTZ 9.5

SOURCE
  TYPE Alpha
  POSZ 25
  PEAK 5.154
  PEAK 5.485
  PEAK 5.805
 
ALGORITHM
  TYPE TSpectrum
  SIGMA 4
  LOWTHRES 500
  HIGHTHRES 4000
  
OPTIONS
  TREE Iris
  ENERGY TYdEnergy
  CHANNEL TYdChannel
  OFFSET 196
  NBINS 4096
  MIN -0.5
  MAX 4095.5
```
The configuration specifies options for four 'classes': DETECTOR, SOURCE,
ALGORITHM and OPTIONS. When one of these keywords appear alone and un-indented
on a line it signals the program to create an instance of that particular class
and configure it depending on the following options. The options must be
indented by two spaces and contain the option name followed by a value.
Available options are listed below:
* DETECTOR
  * TYPE This must be the first option to appear in the DETECTOR definition.
         Possible values are YY1 or YY1Array. (required)
  * POSX X-position of the detector in mm.
  * POSY Y-position of the detector in mm.
  * POSZ Z-position of the detector in mm.
  * ROTX Rotate the detector around the x-axis by a given number of degrees.
  * ROTY Rotate the detector around the y-axis by a given number of degrees.
  * ROTZ Rotate the detector around the z-axis by a given number of degrees.
  * DEADLAYER Specify the dead layer thickness in nm Si equivalent.
* SOURCE
  * TYPE Specify the particle emitted by the source.
  * POSX X-position of the source in mm. 
  * POSY Y-position of the source in mm.
  * POSZ Z-position of the source in mm.
  * PEAK Define a peak energy of the source in MeV. (required)
* ALGORITHM
  * TYPE This must be the first option to appear in the ALGORITHM definition.
         Possible value is TSpectrum. (required)
  * SIGMA Expected width of the peaks to be searched for in no. of bins.
  * LOWTHRES The lower end of the spectrum to searched for peaks.
  * HIGHTHRES The upper end of the spectrum to be searched for peaks.
  * MINHEIGHT The minimum height of the peaks as a fraction of the maximum peak
              height.
* OPTIONS
  * TREE The name of the ROOT tree containing the data. (required)
  * ENERGY Name of the branch containing the energy signals. (required)
  * CHANNEL Name of the branch containing the channel numbers. (required)
  * NBINS Number of bins in the spectrum.
  * MIN Lower end of the spectrum.
  * MAX Upper end of the spectrum.
  * OFFSET Define an offset for the channel numbers listed in the output. If
           none is given, the channels will be numbered from zero.

### The pedestal file

A pedestal file can be given as an optional input. It should be a plain text
file following this format:
```
ch pedestal
0  21.453
1  19.923
2  23.286
.  .
.  .
```
where the pedestals define the ADC channel corresponding to zero energy.

### The output file

The output is a plain text file containing the calibration parameters found by
the program. The calibration is performed as a fit to E = b*(x-a), where a is
the pedestal(offset) and b is the gain. It follows a three-column format.
```
channel offset gain
196  21.453  0.00234
197  19.923  0.00175
198  23.286  0.00228
.  .
.  .
```

### The log file

It is recommended to specify a log file. The results of the calibration
procedure are stored in this file, and it is an easy way to check that the
program executed as expected. It is a ROOT file, and you can browse it using
the TBrowser from a ROOT session.

## Using in a ROOT macro

If the automatic calibration program doesn't quite fit your needs, there is a
good chance that you might still be able to benefit from some of the classes and
algorithms implemented in the project. An easy way to use the library is to load
it into a ROOT session and use the methods in a macro. To do this you simply
have to include the following lines in the top of your macro:
```
#include "/path/to/header/file.h"

void SuperDuperCalibrationMacro()
{
  gSystem->Load("/path/to/libDetectorCalibration");
  
  //The rest of your code.
  //...
}
```
