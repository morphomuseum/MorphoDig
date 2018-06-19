# MorphoDig
3D surface editor, designed for palaeontologists, palaeo-anthropologists and biologists

Visit the official web page at http://morphomuseum.com/morphodig


##Build/Install MorphoDig on Linux
MorphoDig has not been compiled yet on Linux. Theoretically, MorphoDig should run on Linux without problem (very minor corrections may have to be applied to make it possible). Help would be appreciated to compile and distribute MorphoDig on Linux!

    
   
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
MorphoDig has not been compiled yet on Mac Os. Theoretically, MorphoDig should run on Mac without problem (very minor corrections may have to be applied though). Help would be appreciated to compile and distribute MorphoDig on Mac!
    
    
