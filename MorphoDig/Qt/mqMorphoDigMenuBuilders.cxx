/*=========================================================================

   Program: MorphoDig
   Module:    mqMorphoDigMenuBuilders.cxx

========================================================================*/
/*

#ifdef _WIN32
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#define DIV 1048576 
#define WIDTH 7
#endif

#ifdef linux
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#endif
*/

#include "mqMorphoDigCore.h"
#include "mqAboutDialogReaction.h"
#include "mqChangeNodeReaction.h"

#include "mqVolumeOptionsDialogReaction.h"
#include "mqRendererOptionsDialogReaction.h"
#include "mqEditACTORDialogReaction.h"
#include "mqEditVolumeDialogReaction.h"

#include "mqResliceDialogReaction.h"
#include "mqEditFLGDialogReaction.h"
#include "mqEditLMKDialogReaction.h"
#include "mqEditAllFLGLengthDialogReaction.h"
#include "mqEditTagsDialogReaction.h"
#include "mqICPDialogReaction.h"
#include "mqIsosurfaceDialogReaction.h"
#include "mqFlipSwapDialogReaction.h"
#include "mqSpacingDialogReaction.h"
#include "mqResampleDialogReaction.h"
#include "mqMedianFilterDialogReaction.h"
#include "mqGaussianFilterDialogReaction.h"
#include "mqEditAllFLGColorDialogReaction.h"
#include "mqEditAllSurfacesColorDialogReaction.h"
#include "mqSelectSmallObjectsDialogReaction.h"
#include "mqSelectSmallVolumesDialogReaction.h"
#include "mqShrinkWrapDialogReaction.h"
#include "mqShrinkWrapIterativeDialogReaction.h"
#include "mqBooleanOperationDialogReaction.h"
#include "mqSavePLYDialogReaction.h"
#include "mqSaveVTKDialogReaction.h"
#include "mqCylinderDialogReaction.h"
#include "mqCubeDialogReaction.h"
#include "mqIcosahedronDialogReaction.h"
#include "mqCameraDialogReaction.h"
#include "mqSaveMHDMHADialogReaction.h"
#include "mqSaveVTIDialogReaction.h"
#include "mqSaveSTLDialogReaction.h"
#include "mqMinimalDialogReaction.h"
#include "mqSaveOBJDialogReaction.h"
#include "mqColorDialogReaction.h"
#include "mqLandmarkDialogReaction.h"
#include "mqMorphoDigMenuBuilders.h"
#include "mqCameraControlsWidget.h"
#include "mqMainControlsWidget.h"
#include "mqInteractionControlsWidget.h"
#include "mqDisplayControlsWidget.h"
#include "mqLightControlsWidget.h"
#include "mqActorTreePanel.h"
#include "mqObjectsControlsWidget.h"
#include "mqScalarsControlsWidget.h"
#include "mqDesktopServicesReaction.h"
#include "mqOpenDataReaction.h"
#include "mqSaveDataReaction.h"
#include "mqSaveLandmarksDialogReaction.h"
#include "mqSaveFlagsDialogReaction.h"
#include "mqSaveCURDialogReaction.h"
#include "mqGridSizeDialogReaction.h"
#include "mqEditAlphaDialogReaction.h"
#include "mqSaveCURasVERDialogReaction.h"
#include "mqOrientationLabelsDialogReaction.h"
#include "mqExtractScalarRangeDialogReaction.h"
#include "mqEditScalarsDialogReaction.h"
#include "mqSaveSTVDialogReaction.h"
#include "mqSaveMAPDialogReaction.h"
#include "mqSaveTAGMAPDialogReaction.h"
#include "mqSaveNTWDialogReaction.h"
#include "mqSelectLandmarkRangeDialogReaction.h"
#include "mqSmoothDialogReaction.h"
#include "mqDecimateDialogReaction.h"
#include "mqDensifyDialogReaction.h"
#include "mqFillHolesDialogReaction.h"
#include "mqTPSDialogReaction.h"
#include "mqTagFromRGBDialogReaction.h"
#include "mqDecomposeDialogReaction.h"
#include "mqScalarsThicknessDialogReaction.h"
#include "mqScalarsComplexityDialogReaction.h"
#include "mqScalarsSmoothDialogReaction.h"
#include "mqScalarsCurvatureDialogReaction.h"
#include "mqScalarsNormalizationDialogReaction.h"
#include "mqScalarsDistanceDialogReaction.h"
#include "mqScalarsThicknessBetweenDialogReaction.h"
#include "mqScalarsInfosDialogReaction.h"
#include "mqScalarsVolumeDialogReaction.h"

#include "mqSurfaceTagInfosDialogReaction.h"
#include "mqExtractTagRangeDialogReaction.h"
#include "mqSetName.h"
#include "mqViewMenuManager.h"
#include "QRAMThread.h"

//#include "ui_mqEditMenuBuilder.h" // no .ui Edit menu file yet
//#include "ui_mqFileMenuBuilder.h" // no .ui File menu yet


//#include "pqAboutDialogReaction.h"

#include <iostream>
#include <QProgressBar>
#include <QLabel>
#include <QThread>
#include <QStatusBar>
#include <QDesktopServices>
#include <QFrame>
#include <QUrl>
#include <QDockWidget>
#include <QFileInfo>
#include <QKeySequence>
#include <QLayout>
#include <QPalette>
#include <QMainWindow>
#include <QMenu>



void mqMorphoDigMenuBuilders::buildViewMenu(QMenu& menu, QMainWindow& window, QMainWindow& projectwindow)
{
/*	QString objectName = menu.objectName();
	std::cout << "Menu object name" << objectName.toStdString() << std::endl;
	m_p_Act_Button1 = new QAction("Super Button 1", this);
	m_p_Act_Button1->setCheckable(true);
	m_p_Act_Button1->setChecked(true);
	connect(m_p_Act_Button1, SIGNAL(triggered()), this, SLOT(slot_SomethingChecked()));*/
	
	new mqViewMenuManager(&window, &menu, &projectwindow);

}
//-----------------------------------------------------------------------------
//void mqMorphoDigMenuBuilders::buildFileMenu(QMenu& menu, QMainWindow* mainWindow = 0)
void mqMorphoDigMenuBuilders::buildFileMenu(QMenu& menu)
{
  QString objectName = menu.objectName();
  std::cout << "Menu object name" << objectName.toStdString() << std::endl;
  //Ui::mqFileMenuBuilder ui;// we do not have a ui file yet for the menu!
  //ui.setupUi(&menu);
  // since the UI file tends to change the name of the menu.
  menu.setObjectName(objectName);
 // QAction *openNtw = menu.addAction("Open Project");
  //new mqOpenDataReaction(openNtw, 1);//1= open NTW 
  QMenu* submenuProject = menu.addMenu("Project");
  QMenu* submenuSurface = menu.addMenu("Surface");

  QMenu* submenuPosition = menu.addMenu("Position");
  QMenu* submenuLandmark = menu.addMenu("Landmarks");
  QMenu* submenuCurves = menu.addMenu("Curves");
  
  QMenu* submenuFlags = menu.addMenu("Flags");
  new mqOpenDataReaction(submenuFlags->addAction("Open Flags") << mqSetName("actionOpenFLG"), 6);
  new mqSaveFlagsDialogReaction(submenuFlags->addAction("Save Flags") << mqSetName("actionSaveFLG"));
  QMenu* submenuColorMaps = menu.addMenu("Color maps");
  QMenu* submenuTags = menu.addMenu("Tag maps");

  //QMenu* submenuFileInfos = menu.addMenu("Save infos (surface, area, volume)");
  QMenu* submenuOrientationLabels = menu.addMenu("Orientation helper labels");
  QMenu* submenuMeasurements = menu.addMenu("Measurements");
  
  
  new mqOpenDataReaction(submenuProject->addAction("Open Project") << mqSetName("actionOpenNTW"), 1);
  new mqSaveNTWDialogReaction(submenuProject->addAction("Save Project") << mqSetName("actionSaveNTW"));
  
  new mqOpenDataReaction(submenuSurface->addAction("Open surface") << mqSetName("actionOpenMesh"), 2);
  new mqSavePLYDialogReaction(submenuSurface->addAction("Save selected surfaces in one single .PLY file") << mqSetName("actionSavePLY"));
  new mqSaveVTKDialogReaction(submenuSurface->addAction("Save selected surfaces in one single VTK PolyData (.VTK or .VTP) file") << mqSetName("actionSaveVTP"));
  new mqSaveSTLDialogReaction(submenuSurface->addAction("Save selected surfaces in one single .STL file") << mqSetName("actionSaveSTL"));
  new mqSaveOBJDialogReaction(submenuSurface->addAction("Save selected surfaces in one single .OBJ file") << mqSetName("actionSaveOBJ"));


  new mqOpenDataReaction(submenuLandmark->addAction("Open MorphoDig Landmark/Curve file (.STV)") << mqSetName("actionOpenSTV"), 16);
  new mqOpenDataReaction(submenuLandmark->addAction("Open Normal Landmarks (VER, LMK, PTS, TPS)") << mqSetName("actionOpenNormalLMK"), 3);
  new mqOpenDataReaction(submenuLandmark->addAction("Open Target Landmarks (VER, LMK, PTS, TPS)") << mqSetName("actionOpenTargetLMK"), 4);
  new mqOpenDataReaction(submenuLandmark->addAction("Import Avizo/Amira Landmarks inside Normal/Target landmark lists") << mqSetName("actionImportAvizoLM"), 19);
  new mqOpenDataReaction(submenuColorMaps->addAction("Import color maps (.MAP)") << mqSetName("actionOpenMAP"), 17);
  new mqSaveMAPDialogReaction(submenuColorMaps->addAction("Export color maps (.MAP)") << mqSetName("actionSaveMAP"));
  
  new mqOpenDataReaction(submenuTags->addAction("Import tag maps (.TGP or .TAG)") << mqSetName("actionOpenTAGMAP"), 9);
  new mqSaveTAGMAPDialogReaction(submenuTags->addAction("Export tag maps (.TGP or .TAG)") << mqSetName("actionSaveTAGMAP"));
  


  new mqSaveSTVDialogReaction(submenuLandmark->addAction("Save MorphoDig Landmark/Curve file (.STV)") << mqSetName("actionSaveSTV"));
  new mqSaveLandmarksDialogReaction(submenuLandmark->addAction("Save Normal Landmarks (VER, LMK, PTS, TPS)") << mqSetName("actionSaveNormalLMK"), 0);
  new mqSaveLandmarksDialogReaction(submenuLandmark->addAction("Save Target Landmarks (VER, LMK, PTS, TPS)") << mqSetName("actionSaveTargetLMK"), 1);  
  new mqSaveDataReaction(submenuLandmark->addAction("Export Normal and Target Landmarks in Avizo/Amira format") << mqSetName("actionExportLM"), 1);
  new mqSaveDataReaction(submenuLandmark->addAction("Export Normal Landmarks as Flags") << mqSetName("actionExportNormalLMKAsFlags"), 2);
  
  new mqOpenDataReaction(submenuCurves->addAction("Open Curve (.CUR)") << mqSetName("actionOpenCUR"), 5);
  new mqOpenDataReaction(submenuCurves->addAction("Open MorphoDig Landmark/Curve file (.STV)") << mqSetName("actionOpenSTV2"), 16);
  new mqOpenDataReaction(submenuCurves->addAction("Open Curve Node Landmarks (VER, LMK, PTS, TPS)") << mqSetName("actionOpenNodeLMK"), 14);
  new mqOpenDataReaction(submenuCurves->addAction("Open Curve Handle Landmarks (VER, LMK, PTS, TPS)") << mqSetName("actionOpenHandleLMK"), 15);
  new mqSaveCURDialogReaction(submenuCurves->addAction("Save .CUR File ") << mqSetName("actionSaveCURLMK"));
  new mqSaveSTVDialogReaction(submenuCurves->addAction("Save MorphoDig Landmark/Curve file (.STV)") << mqSetName("actionSaveSTV2"));
  new mqSaveLandmarksDialogReaction(submenuCurves->addAction("Save Curve Node Landmarks (VER, LMK, PTS, TPS)") << mqSetName("actionSaveNodeLMK"), 2);
  new mqSaveLandmarksDialogReaction(submenuCurves->addAction("Save Curve Handle Landmarks (VER, LMK, PTS, TPS)") << mqSetName("actionSaveHandleLMK"), 3);
  new mqSaveCURasVERDialogReaction(submenuCurves->addAction("Export Curve Segments (and optionally normal and target landmarks) as landmark file (VER, LMK, PTS, TPS)") << mqSetName("actionExportCUR"));
  new mqSaveDataReaction(submenuCurves->addAction("Save curve infos (length per curve segment)") << mqSetName("actionSaveCURInfos"), 17);
  

  new mqOpenDataReaction(submenuPosition->addAction("Open position for selected surfaces or volumes") << mqSetName("actionOpenPOS"), 8);
  new mqOpenDataReaction(submenuPosition->addAction("Open transposed position for selected surfaces or volumes") << mqSetName("actionOpenPOS2"), 10);
  new mqOpenDataReaction(submenuPosition->addAction("Open position for selected landmarks/flags") << mqSetName("actionOpenPOS3"), 11);
  
  new mqOpenDataReaction(submenuPosition->addAction("Open transposed position for selected landmarks/flags") << mqSetName("actionOpenPOS4"), 12);
  new mqSaveDataReaction(submenuPosition->addAction("Save position for selected surface") << mqSetName("actionSavePOS"), 8);
  new mqSaveDataReaction(submenuPosition->addAction("Save position for selected volume") << mqSetName("actionSavePOS"), 9);
  
  new mqOpenDataReaction(submenuOrientationLabels->addAction("Open Orientation Labels") << mqSetName("actionOpenORI"), 7);
  new mqSaveDataReaction(submenuOrientationLabels->addAction("Save Orientation Labels") << mqSetName("actionSaveORI"), 7);
  
  //new mqSaveDataReaction(submenuMeasurements->addAction("Save area, volume, triangle number and vertex number of selected surfaces") << mqSetName("actionSaveAV"), 18);
  new mqSurfaceTagInfosDialogReaction(submenuMeasurements->addAction("Save area, volume triangle number of selected surfaces") << mqSetName("actionSaveSurfaceTagInfos"));

  new mqSaveDataReaction(submenuMeasurements->addAction("Surface global complexity: save normalized shape index of selected surfaces") << mqSetName("actionSaveNSI"), 19);  
  new mqSaveDataReaction(submenuMeasurements->addAction("Surface global complexity: save convex hull area ratio and normalized shape index of selected surfaces (warning: slow)") << mqSetName("actionSaveCHNSI"), 20);
  new mqSaveDataReaction(submenuMeasurements->addAction("Save size measurements (max length in xyz direction etc.) of selected surfaces") << mqSetName("actionSaveSize"), 21);
  //new mqSaveDataReaction(submenuMeasurements->addAction("Save active scalar infos (mean, median, variance ...) of selected surfaces") << mqSetName("actionSaveSCInfos"), 22);
  new mqScalarsInfosDialogReaction(submenuMeasurements->addAction("Save active scalar infos (mean, median, variance ...) of selected surfaces") << mqSetName("actionSaveSCInfos"));
  new mqSaveDataReaction(submenuMeasurements->addAction("Save scalar values of first selected surface") << mqSetName("actionSaveSCInfos"), 23);

  
  QMenu* submenuVolume = menu.addMenu("Volume");
  new mqOpenDataReaction(submenuVolume->addAction("Open MHD/MHA/VTI Volume") << mqSetName("actionOpenVolume"), 18);
  new mqOpenDataReaction(submenuVolume->addAction("Open Raw Volume") << mqSetName("actionOpenRawVolume"), 20);
  new mqOpenDataReaction(submenuVolume->addAction("Open 3D TIFF file") << mqSetName("actionOpenTiffVolume"), 21);
  new mqOpenDataReaction(submenuVolume->addAction("Open stack of 2D TIFF files") << mqSetName("actionOpenTiffStack"), 22);
  new mqOpenDataReaction(submenuVolume->addAction("Open DICOM 2D Stack") << mqSetName("actionOpenDicomStack"), 24);
  new mqOpenDataReaction(submenuVolume->addAction("Open BMP image stack") << mqSetName("actionOpenBMPstack"), 26);
  new mqOpenDataReaction(submenuVolume->addAction("Open PNG image stack") << mqSetName("actionOpenPNGStack"), 27);
  //new mqOpenDataReaction(submenuVolume->addAction("Open DICOM folder") << mqSetName("actionOpenDicomFolder"), 23);  
  //new mqOpenRawDialogReaction(submenuVolume->addAction("Open Raw Volume") << mqSetName("actionOpenRawVolume"));
  new mqOpenDataReaction(submenuVolume->addAction("Import MHD/MHA/VTI Mask and apply it to first selected volume") << mqSetName("actionImportMask"), 25);
//#include "mqOpenRawDialogReaction.h"
  new mqSaveMHDMHADialogReaction(submenuVolume->addAction("Save first selected volume as .MHD or .MHA file") << mqSetName("actionSaveMHDMHA"),0);
  new mqSaveVTIDialogReaction(submenuVolume->addAction("Save first selected volume as .VTI") << mqSetName("actionSaveVTI"));
  
  new mqSaveMHDMHADialogReaction(submenuVolume->addAction("Export mask of first selected volume as .MHD or .MHA file") << mqSetName("actionSaveMHDMHA"), 1);
  
  //new mqMinimalDialogReaction(menu.addAction("Minimal Window") << mqSetName("actionOpenMinimal"));
  
}

//-----------------------------------------------------------------------------
//void mqMorphoDigMenuBuilders::buildEditMenu(QMenu& menu, QMainWindow* mainWindow = 0)
void mqMorphoDigMenuBuilders::buildEditMenu(QMenu& menu)
{
	// About

  QString objectName = menu.objectName();
 /* Ui::pqEditMenuBuilder ui;
  ui.setupUi(&menu);*/ // no ui file yet

  // since the UI file tends to change the name of the menu.
  menu.setObjectName(objectName);
  new mqColorDialogReaction(menu.addAction("Edit color and lighting options") << mqSetName("actionColor"));
  new mqGridSizeDialogReaction(menu.addAction("Edit size unit, grid spacing and scale") << mqSetName("actionGridSize"));
  new mqLandmarkDialogReaction(menu.addAction("Edit landmark and flag rendering options") << mqSetName("actionLandmark"));
  new mqOrientationLabelsDialogReaction(menu.addAction("Edit orientation labels") << mqSetName("actionOrientationLabels"));
  new mqCameraDialogReaction(menu.addAction("Edit camera options") << mqSetName("actionOrientationCamera"));
  new mqVolumeOptionsDialogReaction(menu.addAction("Edit volume options") << mqSetName("actionVolumeOptions"));
  new mqRendererOptionsDialogReaction(menu.addAction("Edit renderer options (transparency rendering)") << mqSetName("actionRendererOptions"));
  //new pqUndoRedoReaction(ui.actionEditUndo, true);
  //new pqUndoRedoReaction(ui.actionEditRedo, false);
  //new pqCameraUndoRedoReaction(ui.actionEditCameraUndo, true);
 // new pqCameraUndoRedoReaction(ui.actionEditCameraRedo, false);
  
}

void mqMorphoDigMenuBuilders::buildScalarsMenu(QMenu& menu)
{

		new mqEditScalarsDialogReaction(menu.addAction("Open scalars window") << mqSetName("editScalarRenderingWindow"));

	QAction *CameraDistance = menu.addAction("Compute distance from camera for each selected surface");
	QAction::connect(CameraDistance, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotScalarsCameraDistance()));



	new mqScalarsThicknessDialogReaction(menu.addAction("Compute thickness within each selected surface ") << mqSetName("actionThickess"));
	new mqScalarsThicknessBetweenDialogReaction(menu.addAction("Compute thickness between two surfaces") << mqSetName("actionThickessBetween"));
	new mqScalarsDistanceDialogReaction(menu.addAction("Compute distance between two surfaces") << mqSetName("actionDistanceBetween"));
	new mqScalarsCurvatureDialogReaction(menu.addAction("Compute curvature for each selected surface") << mqSetName("actionCurvature"));
	new mqScalarsComplexityDialogReaction(menu.addAction("Compute complexity for each selected surface") << mqSetName("actionComplexity"));
	new mqScalarsVolumeDialogReaction(menu.addAction("Compute volume for each selected surface") << mqSetName("actionVolume"));
	new mqScalarsSmoothDialogReaction(menu.addAction("Smooth active scalars for each selected surface") << mqSetName("actionSmooth"));
	new mqScalarsNormalizationDialogReaction(menu.addAction("Normalize or rescale active scalars for each selected surface") << mqSetName("actionNormalize"));
	new mqExtractScalarRangeDialogReaction(menu.addAction("Extract scalar range for each selected surface") << mqSetName("actionExtractScalarRange"));
}
void mqMorphoDigMenuBuilders::buildTagsMenu(QMenu& menu)
{

	new mqEditTagsDialogReaction(menu.addAction("Open tags window") << mqSetName("editScalarRenderingWindow"));

	QAction *CreateNewTagArray = menu.addAction("Create new empty tag array for each selected surface");
	new mqTagFromRGBDialogReaction(menu.addAction("Create new tag array based on currently displayed colors for each selected surface") << mqSetName("ActrionTagFromRGB"));

	//là on va faire une fenêtre qui demande 2 choses : 
	// 1) EXACT color match est-ce qu'on se base sur la tag map actuelle (exact color match) => ce qui implique qu'on puisse y acceder sans avoir d'array tags ouvertes.... ce qui n'est pas un drame, notamment pour éditer les tag maps quand on n'a pas de tags ouverts
	// 2) si on 
	//1) on s'arrête à combien de couleurs ?
	// 

	//QAction::connect(CreateNewTagArrayColor, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotCreateTagArrayFromRGB()));
	QAction::connect(CreateNewTagArray, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotCreateTagArray()));
	QAction *CreateNewTagArrayConnectivity = menu.addAction("Create new tag array based on connectivity for each selected surface");
	QAction::connect(CreateNewTagArrayConnectivity, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotCreateTagArrayConnectivity()));
	QAction *extractActiveTag = menu.addAction("Extract active tag region for each selected surface");
	QAction::connect(extractActiveTag, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotExtractActiveTag()));
	
	new mqExtractTagRangeDialogReaction(menu.addAction("Extract tag region range for each selected surface") << mqSetName("actionExtractTagRange"));
	//QAction::connect(extractActiveTag, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotExtractActiveTag()));
	QAction *decomposeTag = menu.addAction("Extract each tag region as a new object, for all tag regions of all selected surfaces");
	QAction::connect(decomposeTag, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotDecomposeTag()));
	
}
void mqMorphoDigMenuBuilders::buildRGBMenu(QMenu& menu)
{

	QAction *RGBFromCurrentColor = menu.addAction("Create or replace an RGB array with current display color for each selected surface");
	QAction::connect(RGBFromCurrentColor, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotScalarsRGB()));

}
void mqMorphoDigMenuBuilders::buildEditVolumesMenu(QMenu& menu)
{
	new mqEditVolumeDialogReaction(menu.addAction("Edit first selected volume") << mqSetName("actionEditVolume"));
	new mqIsosurfaceDialogReaction(menu.addAction("Extract isosurface from first selected volume") << mqSetName("actionIsosurfaceVolume"));
	new mqFlipSwapDialogReaction(menu.addAction("Flip or Swap first selected volume") << mqSetName("actionFlipSwapVolume"));
	new mqSpacingDialogReaction(menu.addAction("Change voxel size of first selected volume (keep dimensions, no resampling)") << mqSetName("actionSpacingVolume"));
	new mqResampleDialogReaction(menu.addAction("Resample first selected volume (dimensions will be changed and voxel size as well)") << mqSetName("actionResampleVolume"));
	new mqResliceDialogReaction(menu.addAction("Reslice first selected volume") << mqSetName("actionResliceVolume"));
	new mqMedianFilterDialogReaction(menu.addAction("Median filter of first selected volume") << mqSetName("actionMedianFilterVolume"));
	new mqGaussianFilterDialogReaction(menu.addAction("Gaussian filter of first selected volume") << mqSetName("actionGaussianFilterVolume"));
	QAction *InvertV = menu.addAction("Invert first selected volume");
	QAction::connect(InvertV, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotInvertVolume()));

}

void mqMorphoDigMenuBuilders::buildEditSelectedSurfacesMenu(QMenu& menu)
{
	QString objectName = menu.objectName();
	std::cout << "Menu object name" << objectName.toStdString() << std::endl;
	menu.setObjectName(objectName);
	
	new mqEditACTORDialogReaction(menu.addAction("Edit first selected surface") << mqSetName("actionEditSurface"));
	QMenu* submenuStructureModification = menu.addMenu("Structure modification");

	
	new mqSmoothDialogReaction(submenuStructureModification->addAction("Smooth each selected surface") << mqSetName("actionSmooth"));
	new mqDecimateDialogReaction(submenuStructureModification->addAction("Decimate each selected surface") << mqSetName("actionDecimate"));

	new mqDensifyDialogReaction(submenuStructureModification->addAction("Densify each selected surface") << mqSetName("actionDensify"));
	new mqFillHolesDialogReaction(submenuStructureModification->addAction("Fill holes of each selected surface") << mqSetName("actionFillHoles"));
	new mqTPSDialogReaction(submenuStructureModification->addAction("TPS deformation of each selected surface") << mqSetName("actionTPS"));
	new mqDecomposeDialogReaction(submenuStructureModification->addAction("Connectivity : decompose each selected surface into independent regions") << mqSetName("actionDecompose"));
	QAction *KeepLargest = submenuStructureModification->addAction("Connectivity : keep largest region for each selected surface");

	QAction *Invert = submenuStructureModification->addAction("Invert each selected surface");
	QAction *MirrorX = submenuStructureModification->addAction("Mirror each selected surface along X plane");
	QAction *MirrorY = submenuStructureModification->addAction("Mirror each selected surface along Y plane");
	QAction *MirrorZ = submenuStructureModification->addAction("Mirror each selected surface along Z plane");
	
	/*QAction *Lasso = submenuStructureModification->addAction("Lasso cut: keep inside the selection");
	QAction *Lasso2 = submenuStructureModification->addAction("Lasso cut: keep outside the selection");*/
	QAction *Group = submenuStructureModification->addAction("Merge selected surfaces into one single surface");
	QMenu* submenuWrapping = menu.addMenu("Wrapping methods");
	QAction *ConvexHULL = submenuWrapping->addAction("Create a convex hull for each selected surface");

	new mqShrinkWrapDialogReaction(submenuWrapping->addAction("Shrink and wrap 1 surface over a 2nd surface") << mqSetName("actionShrinkWrap"));
	//new mqShrinkWrapIterativeDialogReaction(submenuWrapping->addAction("Iterative shrink and wrap 1 surface over a 2nd surface") << mqSetName("actionIterativeShrinkWrap"));


	QMenu* submenuBoolean = menu.addMenu("Boolean operations");
	new mqBooleanOperationDialogReaction(submenuBoolean->addAction("Boolean operations involving two surfaces") << mqSetName("actionBoolean"));

	QMenu* submenuAlignment = menu.addMenu("Surface alignment");
	new mqICPDialogReaction(submenuAlignment->addAction("Align 2 surfaces (iterative closest point algorithm)") << mqSetName("actionICP"));

	
	QMenu* submenuRenderingModification = menu.addMenu("Rendering modification");
	

	new mqEditAlphaDialogReaction(submenuRenderingModification->addAction("Change transparency") << mqSetName("actionEditAlpha"));

	QMenu* submenuChangeObjectColor = submenuRenderingModification->addMenu("Change object solid color");

	new mqEditAllSurfacesColorDialogReaction(submenuChangeObjectColor->addAction("Custom color") << mqSetName("actionEditAllSelectedSurfacesColors"));
	QAction *Grey = submenuChangeObjectColor->addAction("Grey");
	
	QAction *Yellow = submenuChangeObjectColor->addAction("Yellow");
	QAction *Red = submenuChangeObjectColor->addAction("Red");
	QAction *Pink = submenuChangeObjectColor->addAction("Pink");
	QAction *Blue = submenuChangeObjectColor->addAction("Blue");
	QAction *Violet = submenuChangeObjectColor->addAction("Violet");
	QAction *Bone = submenuChangeObjectColor->addAction("Bone");
	QAction *Green = submenuChangeObjectColor->addAction("Green");
	QAction *Darkred = submenuChangeObjectColor->addAction("Dark red");
	QAction *Darkblue = submenuChangeObjectColor->addAction("Dark blue");
	QAction *Darkgreen = submenuChangeObjectColor->addAction("Dark green");
	QAction *Orange = submenuChangeObjectColor->addAction("Orange");
	QAction *Brown = submenuChangeObjectColor->addAction("Brown");
	
	QMenu* submenuSupportMaterial = menu.addMenu("Create 3D primitives");
	
	new mqSelectSmallObjectsDialogReaction(menu.addAction("Select small objects") << mqSetName("actionSelectSmallObjects"));
	new mqSelectSmallVolumesDialogReaction(menu.addAction("Select small volumes") << mqSetName("actionSelectSmallVolumes"));
	new mqCylinderDialogReaction(submenuSupportMaterial->addAction("Create cylindric connective struts") << mqSetName("actionCylinders"));
	new mqCubeDialogReaction(submenuSupportMaterial->addAction("Create cubic/box-shaped connective struts") << mqSetName("actionCubes"));	
	new mqIcosahedronDialogReaction(submenuSupportMaterial->addAction("Create icosahedrons/spheres") << mqSetName("actionIcos"));
	
	QAction::connect(KeepLargest, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotKeepLargest()));
	QAction::connect(Invert, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotInvert()));
	QAction::connect(MirrorX, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotMirrorX()));
	QAction::connect(MirrorY, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotMirrorY()));
	QAction::connect(MirrorZ, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotMirrorZ()));
	QAction::connect(ConvexHULL, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotConvexHULL()));
	/*QAction::connect(Lasso, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotLassoCutKeepInside()));
	QAction::connect(Lasso2, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotLassoCutKeepOutside()));*/
	QAction::connect(Group, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotGroup()));

	QAction::connect(Grey, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotGrey()));
	QAction::connect(Yellow, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotYellow()));
	QAction::connect(Red, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotRed()));
	QAction::connect(Pink, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotPink()));
	QAction::connect(Blue, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotBlue()));
	QAction::connect(Violet, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotViolet()));
	QAction::connect(Bone, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotBone()));
	QAction::connect(Green, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotGreen()));
	QAction::connect(Darkred, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotDarkred()));
	QAction::connect(Darkblue, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotDarkblue()));
	QAction::connect(Darkgreen, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotDarkgreen()));
	QAction::connect(Orange, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotOrange()));
	QAction::connect(Brown, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotBrown()));
}
void mqMorphoDigMenuBuilders::buildLandmarksMenu(QMenu& menu)
{
	QString objectName = menu.objectName();
	std::cout << "Menu object name" << objectName.toStdString() << std::endl;
	//Ui::mqFileMenuBuilder ui;// we do not have a ui file yet for the menu!
	//ui.setupUi(&menu);
	// since the UI file tends to change the name of the menu.
	menu.setObjectName(objectName);
	new mqEditLMKDialogReaction(menu.addAction("Edit first selected landmark") << mqSetName("actionEditLMK"));
	new mqEditFLGDialogReaction(menu.addAction("Edit first selected flag") << mqSetName("actionEditFLG"));

	// QAction *openNtw = menu.addAction("Open Project");

	//new mqOpenDataReaction(openNtw, 1);//1= open NTW 
	new mqSelectLandmarkRangeDialogReaction(menu.addAction("Select landmark range") << mqSetName("actionSelectLandmarkRange"));
		QAction *MoveDown = menu.addAction("Selected landmarks:  increase landmark number (move down in list)");
		QAction *MoveUp = menu.addAction("Selected landmarks: decrease landmark number(move up in list)");
	QAction *PushBack = menu.addAction("Selected landmarks: push back on closest surface's vertex");
	QAction *ReOrient = menu.addAction("Selected landmarks: change orientation according to surface's normals");
	QAction *MirrorLMKX = menu.addAction("Selected landmarks/flags: mirror along X plane");
	QAction *MirrorLMKY = menu.addAction("Selected landmarks/flags: mirror along Y plane");
	QAction *MirrorLMKZ = menu.addAction("Selected landmarks/flags: mirror along Z plane");
	QAction::connect(MoveUp, SIGNAL(triggered()), mqMorphoDigCore::instance() , SLOT(slotLandmarkMoveUp()));
	QAction::connect(MoveDown, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotLandmarkMoveDown()));

	QAction::connect(PushBack, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotLandmarkPushBack()));
	QAction::connect(ReOrient, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotLandmarkReorient()));
	QAction::connect(MirrorLMKX, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotLandmarkMirrorX()));
	QAction::connect(MirrorLMKY, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotLandmarkMirrorY()));
	QAction::connect(MirrorLMKZ, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotLandmarkMirrorZ()));

	//actionLandmarskMoveUp


	QMenu* submenuLandmarksInvolved = menu.addMenu("Selected curve node and curve handle landmarks");
	new mqEditAllFLGColorDialogReaction(menu.addAction("Edit color of all selected flag landmarks") << mqSetName("actionEditAllSelectedFlagsColors"));
	new mqEditAllFLGLengthDialogReaction(menu.addAction("Edit length all selected flag landmarks") << mqSetName("actionEditAllSelectedFlagsLength"));
	QAction *UpdateAllSelectedFlagsColors = menu.addAction("Update all selected flag landmarks' color to that of the closest vertex");
	QAction::connect(UpdateAllSelectedFlagsColors, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotUpdateAllSelectedFlagsColors()));
	

	QAction *CreateCurveHandles = submenuLandmarksInvolved->addAction("Create curve handles for selected nodes");
	QAction::connect(CreateCurveHandles, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotCreateCurveHandles()));

	
	new mqChangeNodeReaction(submenuLandmarksInvolved->addAction("Move selected curve handles semi-automatically") << mqSetName("actionMoveHandles"), 4);
	
	new mqChangeNodeReaction(submenuLandmarksInvolved->addAction("Normal nodes (red): change as starting nodes (dark green)") << mqSetName("actionStartingNode"), 1);
	new mqChangeNodeReaction(submenuLandmarksInvolved->addAction("Normal nodes (red): connect to preceding starting nodes (cyan)") << mqSetName("actionConnectNode"), 3);
	new mqChangeNodeReaction(submenuLandmarksInvolved->addAction("Normal nodes (red): define as milestone nodes (blue)") << mqSetName("actionMilestoneNode"), 2);
	new mqChangeNodeReaction(submenuLandmarksInvolved->addAction("Reset selected nodes to Normal nodes (red)") << mqSetName("actionNormalNode"), 0);
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//void mqMorphoDigMenuBuilders::buildHelpMenu(QMenu& menu, QMainWindow* mainWindow = 0)
void mqMorphoDigMenuBuilders::buildHelpMenu(QMenu& menu)
{
	
	// MorphoDig Web Site
	new mqDesktopServicesReaction(QUrl("http://www.morphomuseum.com"),
		(menu.addAction("MorphoDig Web Site") << mqSetName("actionWebSite")));




	// MorphoDig tutorials
	new mqDesktopServicesReaction(QUrl("http://www.morphomuseum.com/tutorialsMorphoDig"),
		(menu.addAction("MorphoDig Tutorials") << mqSetName("actionMorphoDigTutorial")));



  

  // ParaView Release Notes
  

  // About
  new mqAboutDialogReaction(menu.addAction("About...") << mqSetName("actionAbout"));
}

void mqMorphoDigMenuBuilders::buildProjectDocks(QMainWindow& projectWindow)
{
	

	//auto dock3 = new QDockWidget(&projectWindow);
	
	
	//@@ ADD BACK ACTOR TREEE
	
	/*
	auto dock3 = new QDockWidget("Actors", &projectWindow);
	projectWindow.addDockWidget(Qt::RightDockWidgetArea, dock3);	
	auto window = new QMainWindow(0);	
	dock3->setAllowedAreas(Qt::AllDockWidgetAreas);	
	QWidget* actor_panel = new mqActorTreePanel(window);	
	window->setCentralWidget(actor_panel);
	dock3->setWidget(window);*/
	//@@
	
	auto dock1 = new QDockWidget("MainControlsToolbar", &projectWindow);
	QWidget* titleBarWidget1 = new QWidget;
	dock1->setTitleBarWidget(titleBarWidget1);

	dock1->titleBarWidget()->hide();
	dock1->setAllowedAreas(Qt::TopDockWidgetArea);


	QWidget* mainControlsWidget = new mqMainControlsWidget(&projectWindow);
	mainControlsWidget->layout()->setSpacing(0);	
	dock1->setWidget(mainControlsWidget);
	

	/*QToolBar* mainToolBar = new mqMainControlsToolbar(&projectWindow)
		<< mqSetName("MainControlsToolbar");
	mainToolBar->layout()->setSpacing(0);
	projectWindow.addToolBar(Qt::TopToolBarArea, mainToolBar);
	cout << "end create main tool bar" << endl;*/


	auto dock2 = new QDockWidget("InteractionControlsToolbar", &projectWindow);
	QWidget* titleBarWidget2 = new QWidget;
	dock2->setTitleBarWidget(titleBarWidget2);
	dock2->titleBarWidget()->hide(); 
	dock2->setAllowedAreas(Qt::AllDockWidgetAreas);
	//QToolBar* interactionToolBar = new mqInteractionControlsToolbar(&projectWindow);
	QWidget* interactionWidget = new mqInteractionControlsWidget(&projectWindow);
	interactionWidget->layout()->setSpacing(0);
	dock2->setWidget(interactionWidget);

	
	//cout << "create interaction tool bar" << endl;
	/*QToolBar* interactionToolBar = new mqInteractionControlsToolbar(&projectWindow)
		<< mqSetName("InteractionControlsToolbar");
	interactionToolBar->layout()->setSpacing(0);
	projectWindow.addToolBar(Qt::TopToolBarArea, interactionToolBar);*/


	auto dock3 = new QDockWidget("Display", &projectWindow);	
	QWidget* titleBarWidget3 = new QWidget;
	dock3->setTitleBarWidget(titleBarWidget3);
	dock3->titleBarWidget()->hide();
	dock3->setAllowedAreas(Qt::AllDockWidgetAreas);
/*	QToolBar*displayToolBar = new mqDisplayControlsToolbar(&projectWindow)
		<< mqSetName("DisplayControlsToolbar");
	displayToolBar->layout()->setSpacing(0);*/

	QWidget* displayWidget = new mqDisplayControlsWidget(&projectWindow);
	displayWidget->layout()->setSpacing(0);
	//displayToolBar->addWidget(displayWidget);

	dock3->setWidget(displayWidget);



	//dock3->setWidget(displayToolBar);
	//cout << "create display tool bar" << endl;

	/*QToolBar*displayToolBar = new mqDisplayControlsToolbar(&projectWindow)
		<< mqSetName("DisplayControlsToolbar");
	displayToolBar->layout()->setSpacing(0);
	projectWindow.addToolBar(Qt::TopToolBarArea, displayToolBar);*/

	auto dock4 = new QDockWidget("LightControlsToolbar", &projectWindow);
	//dock4->setTitleBarWidget(0);
	QWidget* titleBarWidget4 = new QWidget;
	dock4->setTitleBarWidget(titleBarWidget4);

	dock4->titleBarWidget()->hide();
	dock4->setAllowedAreas(Qt::AllDockWidgetAreas);
	//QToolBar* lightToolBar = new mqLightControlsToolbar(&projectWindow);
	//QWidget* lightWidget = new mqLightControlsWidget(&projectWindow);
	//lightWidget->layout()->setSpacing(0);
	//dock4->setWidget(lightWidget);
	
	
	//cout << "create light tool bar" << endl;
	/*QToolBar* lightToolBar = new mqLightControlsToolbar(&projectWindow)
		<< mqSetName("LightControlsToolbar");
	lightToolBar->layout()->setSpacing(0);
	projectWindow.addToolBar(Qt::BottomToolBarArea, lightToolBar);*/

	//cout << "create objects tool bar" << endl;

	auto dock5 = new QDockWidget("Objects",&projectWindow);
	
	dock5->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
	QWidget* titleBarWidget5 = new QWidget;
	dock5->setTitleBarWidget(titleBarWidget5);
	
	dock5->titleBarWidget()->hide();
	//QToolBar* ObjectsToolBar = new mqObjectsControlsToolbar(&projectWindow);
	QWidget* ObjectsWidget= new mqObjectsControlsWidget(&projectWindow);
	ObjectsWidget->layout()->setSpacing(0);
	dock5->setWidget(ObjectsWidget);
	
	//
	
	/*QToolBar* ObjectsToolBar = new mqObjectsControlsToolbar(&projectWindow)
		<< mqSetName("ObjectsControlsToolbar");
	ObjectsToolBar->layout()->setSpacing(0);
	projectWindow.addToolBar(Qt::RightToolBarArea, ObjectsToolBar);*/
	
	//<< mqSetName("ObjectsControlsToolbar");

	//projectWindow.addToolBar(Qt::RightToolBarArea, ObjectsToolBar);

	

	//cout << "create scalars tool bar" << endl;
	auto dock6 = new QDockWidget("Arrays (Scalars, Tags, Colors)", &projectWindow);

	dock6->setAllowedAreas(Qt::AllDockWidgetAreas);
	QWidget* titleBarWidget6 = new QWidget;
	dock6->setTitleBarWidget(titleBarWidget6);

	dock6->titleBarWidget()->hide();
	//QToolBar* scalarsToolBar = new mqScalarsControlsToolbar(&projectWindow);
	QWidget* scalarsWidget= new mqScalarsControlsWidget(&projectWindow);
	scalarsWidget->layout()->setSpacing(0);
	dock6->setWidget(scalarsWidget);
	

	/*
	QToolBar* scalarsToolBar = new mqScalarsControlsToolbar(&projectWindow)
		<< mqSetName("ScalarsControlsToolbar");
	scalarsToolBar->layout()->setSpacing(0);
	projectWindow.addToolBar(Qt::TopToolBarArea, scalarsToolBar);*/
	//cout << "create camera tool bar" << endl;

	auto dock7 = new QDockWidget("Camera", &projectWindow);

	dock7->setAllowedAreas(Qt::LeftDockWidgetArea);
	QWidget* titleBarWidget7 = new QWidget;
	dock7->setTitleBarWidget(titleBarWidget7);

	dock7->titleBarWidget()->hide();
	//QToolBar* cameraToolBar = new mqCameraControlsToolbar(&projectWindow);
	QWidget* cameraWidget = new mqCameraControlsWidget(&projectWindow);
	cameraWidget->layout()->setSpacing(0);
	dock7->setWidget(cameraWidget);
	

	//projectWindow.tabifyDockWidget(dock7, dock5);

	auto dock8 = new QDockWidget("", &projectWindow);

	dock8->setAllowedAreas(Qt::TopDockWidgetArea);
	QWidget* titleBarWidget8 = new QWidget;
	dock8->setTitleBarWidget(titleBarWidget8);

	dock8->titleBarWidget()->hide();
	QFrame* line = new QFrame();
	line->setFrameShape(QFrame::HLine);
	line->setFrameShadow(QFrame::Sunken);
	dock8->setWidget(line);

	/*mqLightControlsToolbar
	auto dock7 = new QDockWidget("Camera", &projectWindow);

	dock7->setAllowedAreas(Qt::LeftDockWidgetArea);
	QWidget* titleBarWidget7 = new QWidget;
	dock7->setTitleBarWidget(titleBarWidget7);

	dock7->titleBarWidget()->hide();


	QFrame* line = new QFrame();
line->setFrameShape(QFrame::HLine);
line->setFrameShadow(QFrame::Sunken);
	*/
	
	projectWindow.addDockWidget(Qt::TopDockWidgetArea, dock1); // main controls (open save undo redo)
	projectWindow.addDockWidget(Qt::TopDockWidgetArea, dock2);// interaction widget (cam obj land modes, norm targ curn curh flg)
	projectWindow.addDockWidget(Qt::TopDockWidgetArea, dock3); // display controls : show grid, orientation helper, anaglyph
	projectWindow.addDockWidget(Qt::TopDockWidgetArea, dock8); //line (=> pour le moment, ça ne fait que séparer des groupes d'icones sans mettre de ligne)

	projectWindow.addDockWidget(Qt::BottomDockWidgetArea, dock4); // light controls widget
	projectWindow.addDockWidget(Qt::RightDockWidgetArea, dock5); //objets controls
	projectWindow.addDockWidget(Qt::TopDockWidgetArea, dock6); // scalars widget
	projectWindow.addDockWidget(Qt::LeftDockWidgetArea, dock7); // camera controls
	
	/*QToolBar* cameraToolBar = new mqCameraControlsToolbar(&projectWindow)
		<< mqSetName("CameraControlsToolbar");
	cameraToolBar->layout()->setSpacing(0);
	projectWindow.addToolBar(Qt::LeftToolBarArea, cameraToolBar);*/

}

//-----------------------------------------------------------------------------
void mqMorphoDigMenuBuilders::buildStatusBar(QMainWindow& mainWindow)
{
	
	/*#ifdef _WIN32
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);

	
	QString mram = QString::number(statex.dwMemoryLoad);

	QStatusBar *statusBar = mainWindow.statusBar();

	QProgressBar *progressBar = new QProgressBar();
	QLabel *mode = new QLabel("My ram");
	QLabel *modified = new QLabel("  Y  ");
	QLabel *size = new QLabel("  999999kB  ");

	mode->setMinimumSize(mode->sizeHint());
	mode->setAlignment(Qt::AlignCenter);
	mode->setText(mram+"%");
	mode->setToolTip("Currently used ram.");

	statusBar->addPermanentWidget(mode);
	//RAMThread *mythread= new RAMThread();
	//nnect(mythread, &mythread::resultReady, this, &MyObject::handleResults);
	//statex.dwMemoryLoad
	//mythread->start();
#endif
	*/
	//QRAMThread*mythread = new QRAMThread();
	//nnect(mythread, &mythread::resultReady, this, &MyObject::handleResults);
	//statex.dwMemoryLoad
	//mythread->start();

}
void mqMorphoDigMenuBuilders::buildToolbars(QMainWindow& mainWindow)
{
	/*cout << "create main tool bar" << endl;
	QToolBar* mainToolBar = new mqMainControlsToolbar(&mainWindow)
		<< mqSetName("MainControlsToolbar");
	mainToolBar->layout()->setSpacing(0);
	mainWindow.addToolBar(Qt::TopToolBarArea, mainToolBar);
	cout << "end create main tool bar" << endl;*/


	/*

	QToolBar* lightToolBar = new mqLightControlsToolbar(&mainWindow)
		<< mqSetName("LightControlsToolbar");
	lightToolBar->layout()->setSpacing(0);
	mainWindow.addToolBar(Qt::BottomToolBarArea, lightToolBar);

	QToolBar* ObjectsToolBar = new mqObjectsControlsToolbar(&mainWindow)
		<< mqSetName("ObjectsControlsToolbar");
	ObjectsToolBar->layout()->setSpacing(0);
	mainWindow.addToolBar(Qt::RightToolBarArea, ObjectsToolBar);

	QToolBar* scalarsToolBar = new mqScalarsControlsToolbar(&mainWindow)
		<< mqSetName("ScalarsControlsToolbar");
	scalarsToolBar->layout()->setSpacing(0);
	mainWindow.addToolBar(Qt::BottomToolBarArea, scalarsToolBar);

	QToolBar* cameraToolBar = new mqCameraControlsToolbar(&mainWindow)
		<< mqSetName("CameraControlsToolbar");
	cameraToolBar->layout()->setSpacing(0);
	mainWindow.addToolBar(Qt::LeftToolBarArea, cameraToolBar);
	*/
}

