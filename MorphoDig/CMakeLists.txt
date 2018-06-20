
cmake_minimum_required(VERSION 2.8.8)
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()
if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW) # CMake 3.0
endif()
if(POLICY CMP0043)
  cmake_policy(SET CMP0043 NEW) # CMake 3.0
endif()
if(POLICY CMP0053)
  cmake_policy(SET CMP0053 NEW) # CMake 3.1
endif()
if(POLICY CMP0071)
  cmake_policy(SET CMP0071 NEW) # CMake 3.10
endif()

project(MorphoDig)

file(STRINGS version.txt version_txt)

set (MORPHODIG_VERSION ${version_txt})


# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/MorphoDigVersion.h.in"
  "${PROJECT_BINARY_DIR}/MorphoDigVersion.h"
  )

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

if("${VTK_QT_VERSION}" STREQUAL "")
  message(FATAL_ERROR "VTK was not built with Qt")
endif()

# Use the include path and library for Qt that is used by VTK.
include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# Set your files and resources here

set( Srcs main.cxx MorphoDig.cxx)

set( Hdrs MorphoDig.h )

set( MOC_Hdrs MorphoDig.h )

set( UIs MorphoDig.ui )

set( QRCs Icons/icons.qrc )


find_package(Qt5Widgets)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# The rest should just work (sure...)
if(VTK_QT_VERSION VERSION_GREATER "4")
  # We have ui files, this will bring in the macro: qt5_wrap_ui
  find_package(Qt5 COMPONENTS Widgets REQUIRED QUIET)
  qt5_wrap_ui(UI_Srcs ${UIs})
  qt5_add_resources(QRC_Srcs ${QRCs} )

  source_group("Resources" FILES
    ${UIs}
    ${QRCs}
    ${EXE_ICON} # Not present
  )

  source_group("Generated" FILES
    ${UI_Srcs}
    ${MOC_Srcs}
    ${QRC_Srcs}
    ${QRC_Srcs}
  )

include_directories ("${PROJECT_SOURCE_DIR}/Helpers")
add_subdirectory (Helpers) 

include_directories ("${PROJECT_SOURCE_DIR}/Qt")
add_subdirectory (Qt) 



set (EXTRA_LIBRARIES ${EXTRA_LIBRARIES} 
MorphoDigHelpers
MorphoDigQt
)

  add_executable(MorphoDig MACOSX_BUNDLE
    ${Srcs} ${Hdrs} ${UI_Srcs} ${MOC_Hdrs} ${QRC_Srcs})
  qt5_use_modules(MorphoDig Core Gui Widgets)
  target_link_libraries(MorphoDig ${VTK_LIBRARIES} ${EXTRA_LIBRARIES} )
else()

  message(FATAL_ERROR "Qt 5 required")

endif()


IF (WIN32)
install(TARGETS MorphoDig	
        RUNTIME
        DESTINATION bin
        COMPONENT applications)
#set (QT5_BINARIES "C:/Qt/5.8/msvc2015_64/bin") 

file(TO_CMAKE_PATH "$ENV{QTDIR}/bin/Qt5Core.dll" qtcore)
file(TO_CMAKE_PATH "$ENV{QTDIR}/bin/Qt5Widgets.dll" qtwidgets)
file(TO_CMAKE_PATH "$ENV{QTDIR}/bin/Qt5GUI.dll" qtgui)
file(TO_CMAKE_PATH "$ENV{QTDIR}/plugins/platforms/qwindows.dll" qtwindows)
file(TO_CMAKE_PATH "$ENV{QTDIR}/plugins/imageformats/qico.dll" qtico)

install(
	FILES
 "${VTK_DIR}/bin/Release/vtkGUISupportQt-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
 "${VTK_DIR}/bin/Release/vtkRenderingOpenGL2-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkInteractionStyle-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkRenderingFreeType-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkRenderingCore-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkFiltersSources-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkFiltersGeneral-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkFiltersCore-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkIOGeometry-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkIOCore-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkCommonExecutionModel-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkCommonDataModel-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkCommonTransforms-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkCommonMath-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkCommonCore-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkCommonColor-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkInteractionWidgets-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkRenderingAnnotation-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkIOLegacy-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkIOPLY-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkRenderingContext2D-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkChartsCore-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkInfovisCore-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkViewsContext2D-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkViewsCore-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"

"${VTK_DIR}/bin/Release/vtkCommonComputationalGeometry-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkCommonMisc-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkCommonSystem-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"

"${VTK_DIR}/bin/Release/vtkFiltersGeometry-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkFiltersExtraction-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkFiltersStatistics-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkImagingFourier-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkImagingCore-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkalglib-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkImagingHybrid-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkglew-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkIOImage-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkpng-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkzlib-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtktiff-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkjpeg-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkmetaio-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkDICOMParser-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkfreetype-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkFiltersHybrid-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkImagingSources-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkFiltersModeling-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkImagingGeneral-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkImagingColor-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkRenderingVolume-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtksys-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtklz4-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkRenderingContextOpenGL2-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkRenderingGL2PSOpenGL2-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"
"${VTK_DIR}/bin/Release/vtkgl2ps-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}.dll"




		"${qtcore}"
		"${qtwidgets}"
		"${qtgui}"
	                
        DESTINATION bin
        COMPONENT applications)
install(
	FILES
		"${qtwindows}"                
        DESTINATION bin/platforms
        COMPONENT applications)


install(
	FILES
		"${qtico}"                
        DESTINATION bin/imageformats
        COMPONENT applications)
ELSE()
#TO DO FOR OTHER SYSTEMS
ENDIF()
