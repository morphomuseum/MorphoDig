# MorphoDig
3D surface editor, designed for palaeontologists, palaeo-anthropologists and biologists

Visit the official web page at http://morphomuseum.com/morphodig


##Build/Install MorphoDig on Linux
###Install on Ubuntu > 15.04:
Use the PPA: https://launchpad.net/~zarquon42/+archive/ubuntu/morphodig
    
To add the PPA and install morphodig run:

    
    sudo apt-add-repository ppa:zarquon42/morphodig
    sudo apt-get update
    sudo apt-get install morphodig
    
    
### Build MorphoDig

1. Get Dependencies: Devel versions of ```fltk1.3, VTK (>=6.1), glew, xinerama, cmake```. (On Ubuntu this would be the packages:   ```cmake, libfltk1.3-dev, libglew-dev, libvtk6-dev, libxinerama-dev  ```)


      * If your distribution comes with an older version of VTK, you need to build and install it yourself: http://www.vtk.org/Wiki/VTK/Configure_and_Build
* Get morphodig sources: Download [tarball](https://github.com/morphomuseum/morphodig/archive/master.zip) or clone the repo:
  
  ```git clone https://github.com/morphomuseum/MorphoDig.git```
* Build it:
 
  ```
  mkdir morphodig-build && cd morphodig-build
  cmake [Path_To_morphodig_source] -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_CXX_FLAGS='--std=c++0x'
  make 
  make install
   ```

##Install morphodig binaries for Windows 
Windows binaries are downloadable at the following address : http://morphomuseum.com/morphodig 
  
##Build morphodig on Windows
1.  install visual studio 2015 express for desktop.
2.  install Qt 5
4.  download and install CMAKE (>=3.3.x) (http://cmake.org).
5.  download and build VTK (>=8.1) (http://vtk.org) using CMAKE with the following options
VTK_QT_VERSION : 5
VTK_Group_Qt : v
vtlGuiSupportQt: v
Module_vtkGUISupportQtOpenGL : v

Then d to environmentvariables QTDIR {PATH TO QT}\5.X\msvc2015_64
Append environment PATH : D:\Qt\5.X\msvc2015_64\bin
Append PATH {PATH TO VTK_BIN}\bin\Release (needed to access the .dll!)
Append PATH {PATH TO VTK_BIN} (needed for CMAKE to find which version of VTK is present on the computer)
Append PATH {PATH TO QT}\5.X\msvc2015_64\bin 

Load CMAKE after Path has been modified


Then add the directory where your .dll files have been created in your path.
6.  get MorphoDig and build it (http://github.com/morphomuseum/MorphoDig). 

##Build morphodig on OS X

1.  install xcode
2.  download, build and install CMAKE (>=3.3.x) (http://cmake.org).
3.  download and instal Qt (5.X)
3.  download, build and install VTK (>=8.1) (http://vtk.org) using CMAKE.
6.  get MorphoDig (https://github.com/morphomuseum/MorphoDig).
7.  build it:

  ```
  mkdir morphodig-build && cd morphodig-build
  cmake [Path_To_morphodig_source] -DCMAKE_INSTALL_PREFIX=/usr/local
  make 
  make install    
    
    
