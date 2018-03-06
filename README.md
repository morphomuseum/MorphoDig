# ISE-MeshTools
3D surface editor, designed for palaeontologists, palaeo-anthropologists and biologists

Visit the official web page at http://morphomuseum.com/meshtools


##Build/Install MeshTools on Linux
###Install on Ubuntu > 15.04:
Use the PPA: https://launchpad.net/~zarquon42/+archive/ubuntu/meshtools
    
To add the PPA and install MeshTools run:

    
    sudo apt-add-repository ppa:zarquon42/meshtools
    sudo apt-get update
    sudo apt-get install meshtools
    
    
### Build MeshTools

1. Get Dependencies: Devel versions of ```fltk1.3, VTK (>=6.1), glew, xinerama, cmake```. (On Ubuntu this would be the packages:   ```cmake, libfltk1.3-dev, libglew-dev, libvtk6-dev, libxinerama-dev  ```)


      * If your distribution comes with an older version of VTK, you need to build and install it yourself: http://www.vtk.org/Wiki/VTK/Configure_and_Build
* Get MeshTools sources: Download [tarball](https://github.com/morphomuseum/ISE-MeshTools/archive/master.zip) or clone the repo:
  
  ```git clone https://github.com/morphomuseum/ISE-MeshTools.git```
* Build it:
 
  ```
  mkdir MeshTools-build && cd MeshTools-build
  cmake [Path_To_MeshTools_source] -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_CXX_FLAGS='--std=c++0x'
  make 
  make install
   ```

##Install MeshTools binaries for Windows 
Windows binaries are downloadable at the following address : http://morphomuseum.com/meshtools 
  
##Build MeshTools on Windows
1.  install visual studio express for desktop.
2.  download and install glew (http://glew.sourceforge.net/). 
3.  download and install CMAKE (>=3.3.x) (http://cmake.org).
4.  download and build FLTK (1.3.x) (http://fltk.org) using CMAKE. 
5.  download and build VTK (>=6.1) (http://vtk.org) using CMAKE. Then add the directory where your .dll files have been created in your path.
6.  get ISE-MeshTools and build it (http://github.com/morphomuseum/ISE-MeshTools). In CMAKE, in order to build the visual studio related files, you will probably have to fill manually the paths towards GLEW include dir and the .lib files, and the paths towards different FLTK .lib files.

##Build MeshTools on OS X

1.  install xcode
2.  download, build  and install glew (http://glew.sourceforge.net/). 
3.  download, build and install CMAKE (>=3.3.x) (http://cmake.org).
4.  download, build and install FLTK (1.3.x) (http://fltk.org) using CMAKE. 
5.  download, build and install VTK (>=6.1) (http://vtk.org) using CMAKE.
6.  get ISE-MeshTools (https://github.com/morphomuseum/ISE-MeshTools).
7.  build it:

  ```
  mkdir MeshTools-build && cd MeshTools-build
  cmake [Path_To_MeshTools_source] -DCMAKE_INSTALL_PREFIX=/usr/local
  make 
  make install    
    
    
