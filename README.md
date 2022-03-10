# MorphoDig
3D surface editor, designed for palaeontologists, palaeo-anthropologists and biologists

Visit the official web page at http://morphomuseum.com/morphodig


## Build/Install MorphoDig on Linux (Ubuntu)

Unfortunately, MorphoDig cannot be compiled using the VTK packages in the repositories but we have to compile them ourselves

0. Install the necessary build tools
`sudo apt install build-essential qtbase5-dev cmake-curses-gui`
1. Install the current VTK version (9.0)
```
wget https://www.vtk.org/files/release/9.0.1/VTK-9.0.1.tar.gz
tar -xzfv VTK-9.0.1.tar.gz`
mkdir vtk9.0-build
cd vtk9.0-build
cmake ../VTK-9.0.0 -DVTK_Group_Imaging=ON -DVTK_Group_Qt=ON -DVTK_Group_Rendering=ON -DVTK_Group_StandAlone=ON -DVTK_Group_Views=ON
make 
sudo make install
```
2. Install MorphoDig
``` 
mkdir build
cd build
cmake ../MorphoDig
make
```
3. Run MorphoDig (from the build directory)
```
./MorphoDig/MorphoDig
```
    
   
## Install morphodig binaries for Windows 

Windows binaries are downloadable at the following address : http://morphomuseum.com/morphodig 
  
## Build morphodig on Windows
1.  install visual studio 2019 express for desktop.
2.  install Qt 5
3. 	add Qt5_DIR to environment variables: {PATH TO QT}\5.x.x\msvc2019_64\lib\cmake
4.  download and install CMAKE (>=3.19.x) (http://cmake.org).
5.  download and build VTK (>=9.1) (http://vtk.org) using CMAKE with the following options:

* VTK_USE_LARGE_DATA : v
* VTK_Group_Enable_Qt : v (VTK should then find : QT5_DIR:  {PATH TO QT}\5.X\msvc2019_64\cmake\Qt5 )
* VTK_QT_VERSION : 5 
* VTK_MODULE_ENABLE_VTK_GUISupportQt: v
* VTK_MODULE_ENABLE_VTK_ImagingOpenGL2: YES
* VTK_MODULE_ENABLE_VTK_RenderingOpenGL2 : YES
* VTK_MODULE_ENABLE_VTK_RenderingVolumeOpenGL2 : YES

Add environment variable
* VTK_DIR to environment variables: {PATH TO VTK_BIN}

Append environment PATH : 
* {PATH TO QT}\5.X\msvc2017_64\bin
* {PATH TO VTK_BIN}\bin\Release (needed to access the .dll!)
* {PATH TO VTK_BIN}\lib\Release (needed to access the .lib when compiling!)
* {PATH TO VTK_BIN} (needed for CMAKE to find which version of VTK is present on the computer)


Load CMAKE after Path has been modified. Then add the directory where your .dll files have been created in your path.

6.  get MorphoDig and build it (http://github.com/morphomuseum/MorphoDig). 

## Build morphodig on OS X

MorphoDig has not been compiled yet on Mac Os. Theoretically, MorphoDig should run on Mac without problem (very minor corrections may have to be applied though). Help would be appreciated to compile and distribute MorphoDig on Mac!
    
    
