# Set your files and resources here
SET (Helpers_SRCS
vtkOrientationHelperActor.cxx 
vtkGridActor.cxx 
vtkMDActor.cxx 
vtkLMActor.cxx 
vtkMDActorCollection.cxx 
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
vtkLMActor.h 
vtkMDActorCollection.h 
vtkLMActorCollection.h 
vtkGridActor.h 
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
find_package(Qt5 COMPONENTS Widgets REQUIRED QUIET)
include_directories(${Qt5Widgets_INCLUDE_DIRS})


ADD_LIBRARY (MorphoDigHelpers ${Helpers_SRCS} ${Helpers_HDRS} ${Qt5_LIBRARIES})
include_directories ("${PROJECT_SOURCE_DIR}/Qt")

TARGET_LINK_LIBRARIES(MorphoDigHelpers  ${VTK_LIBRARIES} )



