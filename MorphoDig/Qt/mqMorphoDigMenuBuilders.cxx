/*=========================================================================

   Program: MorphoDig
   Module:    mqMorphoDigMenuBuilders.cxx

========================================================================*/
#include "mqMorphoDigCore.h"
#include "mqAboutDialogReaction.h"
#include "mqChangeNodeReaction.h"
#include "mqEditAllFLGLengthDialogReaction.h"
#include "mqICPDialogReaction.h"
#include "mqEditAllFLGColorDialogReaction.h"
#include "mqSavePLYDialogReaction.h"
#include "mqSaveVTKDialogReaction.h"
#include "mqSaveSTLDialogReaction.h"
#include "mqColorDialogReaction.h"
#include "mqLandmarkDialogReaction.h"
#include "mqMorphoDigMenuBuilders.h"
#include "mqCameraControlsToolbar.h"
#include "mqMainControlsToolbar.h"
#include "mqInteractionControlsToolbar.h"
#include "mqDisplayControlsToolbar.h"
#include "mqLightControlsToolbar.h"
#include "mqActorTreePanel.h"
#include "mqObjectsControlsToolbar.h"
#include "mqScalarsControlsToolbar.h"
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
#include "mqSaveSTVDialogReaction.h"
#include "mqSaveMAPDialogReaction.h"
#include "mqSaveNTWDialogReaction.h"
#include "mqSelectLandmarkRangeDialogReaction.h"
#include "mqSmoothDialogReaction.h"
#include "mqDecimateDialogReaction.h"
#include "mqDensifyDialogReaction.h"
#include "mqFillHolesDialogReaction.h"
#include "mqTPSDialogReaction.h"
#include "mqDecomposeDialogReaction.h"
#include "mqScalarsThicknessDialogReaction.h"
#include "mqScalarsCurvatureDialogReaction.h"
#include "mqScalarsDistanceDialogReaction.h"
#include "mqScalarsThicknessBetweenDialogReaction.h"

#include "mqSetName.h"
#include "mqViewMenuManager.h"

//#include "ui_mqEditMenuBuilder.h" // no .ui Edit menu file yet
//#include "ui_mqFileMenuBuilder.h" // no .ui File menu yet


//#include "pqAboutDialogReaction.h"

#include <iostream>
#include <QDesktopServices>
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
  
  QMenu* submenuTagsAndFlags = menu.addMenu("Tags and Flags");
  //QMenu* submenuFileInfos = menu.addMenu("Save infos (surface, area, volume)");
  QMenu* submenuOrientationLabels = menu.addMenu("Orientation helper labels");
  QMenu* submenuMeasurements = menu.addMenu("Measurements");
  QMenu* submenuColorMaps = menu.addMenu("Color maps");

  new mqOpenDataReaction(submenuProject->addAction("Open Project") << mqSetName("actionOpenNTW"), 1);
  new mqSaveNTWDialogReaction(submenuProject->addAction("Save Project") << mqSetName("actionSaveNTW"));
  
  new mqOpenDataReaction(submenuSurface->addAction("Open surface") << mqSetName("actionOpenMesh"), 2);
  new mqSavePLYDialogReaction(submenuSurface->addAction("Save selected surfaces in one single .PLY file") << mqSetName("actionSavePLY"));
  new mqSaveVTKDialogReaction(submenuSurface->addAction("Save selected surfaces in one single VTK PolyData (.VTK or .VTP) file") << mqSetName("actionSaveVTP"));
  new mqSaveSTLDialogReaction(submenuSurface->addAction("Save selected surfaces in one single .STL file") << mqSetName("actionSaveSTL"));
  
  
  new mqOpenDataReaction(submenuLandmark->addAction("Open MorphoDig Landmark/Curve file (STV)") << mqSetName("actionOpenSTV"), 16);
  new mqOpenDataReaction(submenuLandmark->addAction("Open Landmarks") << mqSetName("actionOpenNormalLMK"), 3);
  new mqOpenDataReaction(submenuLandmark->addAction("Open Target Landmarks") << mqSetName("actionOpenNormalLMK"), 4);
  new mqOpenDataReaction(submenuColorMaps->addAction("Import color maps (MAP)") << mqSetName("actionOpenMAP"), 17);
  new mqSaveMAPDialogReaction(submenuColorMaps->addAction("Export color maps (MAP)") << mqSetName("actionSaveMAP"));
  

  new mqSaveSTVDialogReaction(submenuLandmark->addAction("Save MorphoDig Landmark/Curve file (STV)") << mqSetName("actionSaveSTV"));
  new mqSaveLandmarksDialogReaction(submenuLandmark->addAction("Save Normal Landmarks") << mqSetName("actionSaveNormalLMK"), 0);
  new mqSaveLandmarksDialogReaction(submenuLandmark->addAction("Save Target Landmarks") << mqSetName("actionSaveTargetLMK"), 1);
  
  
  
  new mqOpenDataReaction(submenuCurves->addAction("Open Curve (.CUR)") << mqSetName("actionOpenCUR"), 5);
  new mqOpenDataReaction(submenuCurves->addAction("Open MorphoDig Landmark/Curve file (STV)") << mqSetName("actionOpenSTV2"), 16);
  new mqOpenDataReaction(submenuCurves->addAction("Open Curve Node Landmarks") << mqSetName("actionOpenNodeLMK"), 14);
  new mqOpenDataReaction(submenuCurves->addAction("Open Curve Handle Landmarks") << mqSetName("actionOpenHandleLMK"), 15);
  new mqSaveCURDialogReaction(submenuCurves->addAction("Save .CUR File ") << mqSetName("actionSaveCURLMK"));
  new mqSaveSTVDialogReaction(submenuCurves->addAction("Save MorphoDig Landmark/Curve file (STV)") << mqSetName("actionSaveSTV2"));
  new mqSaveLandmarksDialogReaction(submenuCurves->addAction("Save Curve Nodes Landmarks") << mqSetName("actionSaveNodeLMK"), 2);
  new mqSaveLandmarksDialogReaction(submenuCurves->addAction("Save Curve Handle Landmarks") << mqSetName("actionSaveHandleLMK"), 3);
  new mqSaveCURasVERDialogReaction(submenuCurves->addAction("Export curve segments as landmark file") << mqSetName("actionExportCUR"));
  new mqSaveDataReaction(submenuCurves->addAction("Save curve infos (length per curve segment)") << mqSetName("actionSaveCURInfos"), 17);
  

  new mqOpenDataReaction(submenuPosition->addAction("Open position for selected surfaces") << mqSetName("actionOpenPOS"), 8);
  new mqOpenDataReaction(submenuPosition->addAction("Open transposed position for selected surfaces") << mqSetName("actionOpenPOS2"), 10);
  new mqOpenDataReaction(submenuPosition->addAction("Open position for selected landmarks") << mqSetName("actionOpenPOS3"), 11);
  new mqOpenDataReaction(submenuPosition->addAction("Open transposed position for selected landmarks") << mqSetName("actionOpenPOS4"), 12);
  new mqSaveDataReaction(submenuPosition->addAction("Save position for selected surface") << mqSetName("actionSavePOS"), 8);

  new mqOpenDataReaction(submenuTagsAndFlags->addAction("Open Flag") << mqSetName("actionOpenFLG"), 6);
  new mqSaveFlagsDialogReaction(submenuTagsAndFlags->addAction("SaveFlags") << mqSetName("actionSaveFLG"));
  new mqOpenDataReaction(submenuOrientationLabels->addAction("Open Orientation Labels") << mqSetName("actionOpenORI"), 7);
  new mqSaveDataReaction(submenuOrientationLabels->addAction("Save Orientation Labels") << mqSetName("actionSaveORI"), 7);
  
  new mqSaveDataReaction(submenuMeasurements->addAction("Save area and volume of selected surfaces") << mqSetName("actionSaveAV"), 18);
  new mqSaveDataReaction(submenuMeasurements->addAction("Save normalized shape index of selected surfaces") << mqSetName("actionSaveNSI"), 19);  
  new mqSaveDataReaction(submenuMeasurements->addAction("Save convex hull area ratio and normalized shape index of selected surfaces (warning: slow)") << mqSetName("actionSaveCHNSI"), 20);

  new mqOpenDataReaction(submenuTagsAndFlags->addAction("Open Tag") << mqSetName("actionOpenTAG"), 9);

  
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
  new mqColorDialogReaction(menu.addAction("Edit color options") << mqSetName("actionColor"));
  new mqGridSizeDialogReaction(menu.addAction("Edit size unit and grid spacing") << mqSetName("actionGridSize"));
  new mqLandmarkDialogReaction(menu.addAction("Edit landmark and flag rendering options") << mqSetName("actionLandmark"));
  new mqOrientationLabelsDialogReaction(menu.addAction("Edit orientation labels") << mqSetName("actionOrientationLabels"));
  //new pqUndoRedoReaction(ui.actionEditUndo, true);
  //new pqUndoRedoReaction(ui.actionEditRedo, false);
  //new pqCameraUndoRedoReaction(ui.actionEditCameraUndo, true);
 // new pqCameraUndoRedoReaction(ui.actionEditCameraRedo, false);
  
}

void mqMorphoDigMenuBuilders::buildEditSelectedSurfacesMenu(QMenu& menu)
{
	QString objectName = menu.objectName();
	std::cout << "Menu object name" << objectName.toStdString() << std::endl;
	menu.setObjectName(objectName);
	QMenu* submenuStructureModification = menu.addMenu("Structure modification");

	
	new mqSmoothDialogReaction(submenuStructureModification->addAction("Smooth each selected surface") << mqSetName("actionSmooth"));
	new mqDecimateDialogReaction(submenuStructureModification->addAction("Decimate each selected surface") << mqSetName("actionDecimate"));

	new mqDensifyDialogReaction(submenuStructureModification->addAction("Densify each selected surface") << mqSetName("actionDensify"));
	new mqFillHolesDialogReaction(submenuStructureModification->addAction("Fill holes of each selected surface") << mqSetName("actionFillHoles"));
	new mqTPSDialogReaction(submenuStructureModification->addAction("TPS deformation of each selected surface") << mqSetName("actionTPS"));
	new mqDecomposeDialogReaction(submenuStructureModification->addAction("Connectivity : decompose each selected surface into independent regions") << mqSetName("actionDecompose"));
	QAction *KeepLargest = submenuStructureModification->addAction("Connectivity : keep largest region for each selected surface");

	QAction *Invert = submenuStructureModification->addAction("Invert each selected surface");
	QAction *Mirror = submenuStructureModification->addAction("Mirror each selected surface along Y plane");
	QAction *ConvexHULL = submenuStructureModification->addAction("Create convex hull for each selected surface");
	QAction *Lasso = submenuStructureModification->addAction("Lasso cut: keep inside the selection");
	QAction *Lasso2 = submenuStructureModification->addAction("Lasso cut: keep outside the selection");
	
	
	QMenu* submenuAlignment = menu.addMenu("Surface alignment");
	new mqICPDialogReaction(submenuAlignment->addAction("Align 2 surfaces (iterative closest point algorithm)") << mqSetName("actionICP"));
	QMenu* submenuRenderingModification = menu.addMenu("Rendering modification");
	

	new mqEditAlphaDialogReaction(submenuRenderingModification->addAction("Change transparency") << mqSetName("actionEditAlpha"));

	QMenu* submenuChangeObjectColor = submenuRenderingModification->addMenu("Change object color");

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
	
	

	QAction::connect(KeepLargest, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotKeepLargest()));
	QAction::connect(Invert, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotInvert()));
	QAction::connect(Mirror, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotMirror()));
	QAction::connect(ConvexHULL, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotConvexHULL()));
	QAction::connect(Lasso, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotLassoCutKeepInside()));
	QAction::connect(Lasso2, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotLassoCutKeepOutside()));
	
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
	
	QMenu* submenuScalarModification = menu.addMenu("Scalars and colors");
	//new mqThicknessDialogReaction(submenuScalarModification->addAction("Compute thickness") << mqSetName("actionThickness"));
	//new mqCurvatureDialogReaction(submenuScalarModification->addAction("Compute curvature") << mqSetName("actionCurvature"));
	QAction *CameraDistance = submenuScalarModification->addAction("Compute distance map from camera");
	QAction::connect(CameraDistance, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotScalarsCameraDistance()));

	QAction *RGBFromCurrentColor = submenuScalarModification->addAction("Create/replace RGB scalar from current color");
	QAction::connect(RGBFromCurrentColor, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotScalarsRGB()));


	new mqScalarsThicknessDialogReaction(submenuScalarModification->addAction("Compute thickness") << mqSetName("actionThickess"));
	new mqScalarsThicknessBetweenDialogReaction(submenuScalarModification->addAction("Compute thickness map between two objects") << mqSetName("actionThickessBetween"));
	new mqScalarsDistanceDialogReaction(submenuScalarModification->addAction("Compute distance map between two objects") << mqSetName("actionDistanceBetween"));
	new mqScalarsCurvatureDialogReaction(submenuScalarModification->addAction("Compute curvature map") << mqSetName("actionCurvature"));
	QAction *GaussianBlur = submenuScalarModification->addAction("Smooth active scalars (gaussian blur)");
	QAction::connect(GaussianBlur, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotScalarsGaussianBlur()));

}
void mqMorphoDigMenuBuilders::buildLandmarksMenu(QMenu& menu)
{
	QString objectName = menu.objectName();
	std::cout << "Menu object name" << objectName.toStdString() << std::endl;
	//Ui::mqFileMenuBuilder ui;// we do not have a ui file yet for the menu!
	//ui.setupUi(&menu);
	// since the UI file tends to change the name of the menu.
	menu.setObjectName(objectName);
	// QAction *openNtw = menu.addAction("Open Project");

	//new mqOpenDataReaction(openNtw, 1);//1= open NTW 
	new mqSelectLandmarkRangeDialogReaction(menu.addAction("Select landmark range") << mqSetName("actionSelectLandmarkRange"));
	QAction *MoveUp = menu.addAction("Selected landmarks : move up (decrease landmark number)");
	QAction *MoveDown = menu.addAction("Selected landmarks : move down (increase landmark number)");
	QAction *PushBack = menu.addAction("Selected landmarks : push back on object surface");
	QAction *ReOrient = menu.addAction("Selected landmarks : change orientation accordin to surface normal");
	QAction::connect(MoveUp, SIGNAL(triggered()), mqMorphoDigCore::instance() , SLOT(slotLandmarkMoveUp()));
	QAction::connect(MoveDown, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotLandmarkMoveDown()));

	QAction::connect(PushBack, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotLandmarkPushBack()));
	QAction::connect(ReOrient, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotLandmarkReorient()));


	//actionLandmarskMoveUp


	QMenu* submenuLandmarksInvolved = menu.addMenu("Selected node and handle landmarks");
	new mqEditAllFLGColorDialogReaction(menu.addAction("Edit color of all selected flags") << mqSetName("actionEditAllSelectedFlagsColors"));
	new mqEditAllFLGLengthDialogReaction(menu.addAction("Edit length all selected flags") << mqSetName("actionEditAllSelectedFlagsLength"));
	QAction *UpdateAllSelectedFlagsColors = menu.addAction("Update all selected flags colours automatically");
	QAction::connect(UpdateAllSelectedFlagsColors, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotUpdateAllSelectedFlagsColors()));
	

	new mqChangeNodeReaction(submenuLandmarksInvolved->addAction("Move curve handles semi-automatically") << mqSetName("actionMoveHandles"), 4);
	new mqChangeNodeReaction(submenuLandmarksInvolved->addAction("Normal nodes (red): change as starting nodes (dark red)") << mqSetName("actionStartingNode"), 1);
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

	
	QToolBar* mainToolBar = new mqMainControlsToolbar(&projectWindow)
		<< mqSetName("MainControlsToolbar");
	mainToolBar->layout()->setSpacing(0);
	projectWindow.addToolBar(Qt::TopToolBarArea, mainToolBar);
	cout << "end create main tool bar" << endl;

	//cout << "create interaction tool bar" << endl;
	QToolBar* interactionToolBar = new mqInteractionControlsToolbar(&projectWindow)
		<< mqSetName("InteractionControlsToolbar");
	interactionToolBar->layout()->setSpacing(0);
	projectWindow.addToolBar(Qt::TopToolBarArea, interactionToolBar);

	//cout << "create display tool bar" << endl;
	QToolBar*displayToolBar = new mqDisplayControlsToolbar(&projectWindow)
		<< mqSetName("DisplayControlsToolbar");
	displayToolBar->layout()->setSpacing(0);
	projectWindow.addToolBar(Qt::TopToolBarArea, displayToolBar);
	
	
	//cout << "create light tool bar" << endl;
	QToolBar* lightToolBar = new mqLightControlsToolbar(&projectWindow)
		<< mqSetName("LightControlsToolbar");
	lightToolBar->layout()->setSpacing(0);
	projectWindow.addToolBar(Qt::BottomToolBarArea, lightToolBar);
	//cout << "create objects tool bar" << endl;

	QToolBar* ObjectsToolBar = new mqObjectsControlsToolbar(&projectWindow)
		<< mqSetName("ObjectsControlsToolbar");
	ObjectsToolBar->layout()->setSpacing(0);
	projectWindow.addToolBar(Qt::RightToolBarArea, ObjectsToolBar);

	//cout << "create scalars tool bar" << endl;
	QToolBar* scalarsToolBar = new mqScalarsControlsToolbar(&projectWindow)
		<< mqSetName("ScalarsControlsToolbar");
	scalarsToolBar->layout()->setSpacing(0);
	projectWindow.addToolBar(Qt::TopToolBarArea, scalarsToolBar);
	//cout << "create camera tool bar" << endl;
	QToolBar* cameraToolBar = new mqCameraControlsToolbar(&projectWindow)
		<< mqSetName("CameraControlsToolbar");
	cameraToolBar->layout()->setSpacing(0);
	projectWindow.addToolBar(Qt::LeftToolBarArea, cameraToolBar);

}

//-----------------------------------------------------------------------------
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

