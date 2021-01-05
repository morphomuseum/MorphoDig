# Set your files and resources here
SET (Helpers_SRCS
vtkOrientationHelperActor.cxx 
vtkGridActor.cxx 
vtkImageTransparencyFilter.cxx
vtkMDActor.cxx 
vtkMDVolume.cxx 
vtkLMActor.cxx 
vtkMDActorCollection.cxx 
vtkMDVolumeCollection.cxx 
vtkLMActorCollection.cxx 
vtkOrientationHelperWidget.cxx 
vtkMDInteractorStyle.cxx 
vtkUndoStack.cxx 
vtkUndoStackBuilder.cxx 
vtkProbeSource.cxx 
vtkBezierSurfaceSource.cxx
vtkBezierSurfaceWidget.cxx
vtkBezierCurveSource.cxx
vtkMDCylinderSource.cxx
vtkMDCubeSource.cxx
)

set( Helpers_HDRS 
vtkOrientationHelperActor.h 
vtkMDActor.h 
vtkMDVolume.h
vtkLMActor.h 
vtkMDActorCollection.h 
vtkMDVolumeCollection.h 
vtkLMActorCollection.h 
vtkGridActor.h 
vtkImageTransparencyFilter.h
vtkOrientationHelperWidget.h 
vtkMDInteractorStyle.h 
vtkUndoStack.h 
vtkUndoStackBuilder.h 
vtkUndoStackInternal.h
vtkProbeSource.h 
vtkBezierSurfaceSource.h
vtkBezierSurfaceWidget.h
vtkBezierCurveSource.h
vtkMDCylinderSource.h
vtkMDCubeSource.h
)

#Sorry but we need QT for the undo stack stuff... 
#So far, in the interactor style and in the core
#Something has to be improved


find_package(VTK
  COMPONENTS   
  ChartsCore 
  FiltersExtraction
  FiltersGeometry
  FiltersModeling
  FiltersStatistics
  GUISupportQt    
  InfovisCore
  ImagingGeneral
  ImagingStatistics
  ImagingStencil
  InteractionImage
  InteractionStyle
  InteractionWidgets
  IOGeometry
  IOImage
  IOLegacy
  IOPLY
  IOXML
  RenderingAnnotation
  RenderingFreeType
  RenderingImage
  RenderingOpenGL2 
  RenderingVolumeOpenGL2
  ViewsContext2D
  )

find_package(Qt5 COMPONENTS Widgets REQUIRED QUIET)
include_directories(${Qt5Widgets_INCLUDE_DIRS})


ADD_LIBRARY (MorphoDigHelpers ${Helpers_SRCS} ${Helpers_HDRS} ${Qt5_LIBRARIES})
include_directories ("${PROJECT_SOURCE_DIR}/Qt")

TARGET_LINK_LIBRARIES(MorphoDigHelpers  ${VTK_LIBRARIES} )
vtk_module_autoinit(TARGETS MorphoDigHelpers  MODULES ${VTK_LIBRARIES})


