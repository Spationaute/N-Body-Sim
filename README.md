# N-Body-Sim
NBody simulation using Armadillo and compatible with GPU acceleration (not on replit)

## Required Package
- Armadillo
- OpenMP
- JSONCPP

## Optional Package
- CUDAToolkit
  
## Compiling on your machine
This project use cmake. Simply create a file called build. If CUDA is installed, it will be compiled with GPU support.
```
mkdir build;
cd build;
cmake ..;
make;
mv NBody ..;
```

To Run
```
./NBody case.json;
```

# About Replit
## How to Use
The replit is configured to run the "case.json" file which contains all the information for the model. The base case is the solar system main bodies. Once you finish setting things up, just click run.

The results will be produced in a "results" folder and automatically archive in "results.zip" just download the zip. Be carefull: **DO NOT OPEN** the zip with replit!

## Notes
Don't forget that replit is limited in resources, so set "writeSteps" not to have too many files. If you compile on your machine, you can activate the GPU acceleration and have a really large number of elements! 

## Regarding the Results
Right now the results are in CSV form, enumerated with their steps. You can read and animate them with Python, Paraview or any other plotting software.

The current case is in 2D and use some data from the solar system. It obviously doesn't give good results, bit its still a somewhat good example. 

# Problems
## JSONCPP
The path for json change with the systems. so if you get an error with json change the line in "Settings.h" to the following:
```
#include <jsoncpp/json/json.h>
```

## NVBLAS.conf
Plea, e be sure the path for the blas libraries are well defined in "nvblas.conf" if you plan to use GPU acceleration.