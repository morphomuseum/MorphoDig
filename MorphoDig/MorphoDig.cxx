



#include "MorphoDig.h"



#include "mqMorphoDigMenuBuilders.h"
#include "mqOpenDataReaction.h"
#include "vtkBezierSurfaceWidget.h"
#include "vtkBezierSurfaceSource.h"
#include "vtkBezierCurveSource.h"
#include "vtkMDActor.h"
#include "vtkLMActor.h"
#include "mqMorphoDigCore.h"
#include "mqUndoStack.h"
#include "vtkMDInteractorStyle.h"
#include "vtkMDActorCollection.h"
#include <QVTKOpenGLWidget.h>
//#include "vtkUndoStack.h"
//#include "vtkUndoSet.h"
//#include "vtkUndoElement.h"
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iostream>

#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkPoints.h>
#include <vtkCylinderSource.h> 
#include <vtkPlaneSource.h>
#include <vtkTextProperty.h>
#include <vtkDataObjectToTable.h>
#include <vtkElevationFilter.h>
#include <vtkLookupTable.h>
#include <vtkBandedPolyDataContourFilter.h>
#include <vtkActor.h>
#include <vtkOpenGLActor.h>
#include <vtkMath.h>
#include <vtkCamera.h>
#include <vtkCenterOfMass.h>
#include <vtkProperty.h>
#include <vtkPolyDataMapper.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyDataReader.h>
#include <vtkRenderWindow.h>
#include <vtkVectorText.h>
#include <vtkImageData.h>
#include <vtkBillboardTextActor3D.h>
#include <vtkProperty2D.h>
#include <vtkAxesActor.h>
#include <vtkPolyDataNormals.h>
#include <vtkPLYReader.h>
#include <vtkSTLReader.h>
#include <vtkCleanPolyData.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include <vtkCellData.h>
#include <vtkPointData.h>
#include <vtkSmartPointer.h>
//
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkInteractorStyleRubberBandPick.h>
#include <vtkInteractorStyleSwitch.h>
#include <vtkCallbackCommand.h>
//#include <vtkAreaPicker.h>
#include <vtkRenderedAreaPicker.h>
#include <vtkLine.h>
#include <vtkProp3DCollection.h>


#include <QTemporaryFile>
#include <QSettings>
#include <QIcon>
#include <QFile>
#include <QTextStream>


#include <vtkTransform.h>
#include <vtkConeSource.h>
#include <vtkBoxWidget.h>
#include <vtkCommand.h>

#include <QMdiArea>
#include <QDockWidget>
//#include <QLabel>
#include <QTreeView>

//-----------------------------------------------------------------------------
//MorphoDig* MorphoDig::Instance = 0;

//-----------------------------------------------------------------------------
/*MorphoDig* MorphoDig::instance()
{
	return MorphoDig::Instance;
}
int MorphoDig::getTestInt()
{
	return MorphoDig::testint;
}*/


/*class vtkMyCallback : public vtkCommand
{
public:
	static vtkMyCallback *New() { return new vtkMyCallback; }
	virtual void Execute(vtkObject *caller, unsigned long, void*)
	{    // Here we use the vtkBoxWidget to transform the underlying coneActor   
		 // (by manipulating its transformation matrix).  
		vtkSmartPointer<vtkTransform> t = vtkSmartPointer<vtkTransform>::New();
		vtkBoxWidget *widget = reinterpret_cast<vtkBoxWidget*>(caller);
		widget->GetTransform(t);    widget->GetProp3D()->SetUserTransform(t);
	}
};*/


//When node landmarks or handle landmarks are modified : do this!
class vtkMyNodeHandleCallBack : public vtkCommand
{
public:
	static vtkMyNodeHandleCallBack *New() { return new vtkMyNodeHandleCallBack; }
	virtual void Execute(vtkObject *caller, unsigned long, void*)
	{  
		
		//vtkBezierSurfaceSource *surfacesource = reinterpret_cast<vtkBezierSurfaceSource*>(caller);
		vtkLMActorCollection *node_or_handle = reinterpret_cast<vtkLMActorCollection*>(caller);
		//cout << "node or handle modified!" << endl;
		mqMorphoDigCore::instance()->getBezierCurveSource()->Modified();
		//mqMorphoDigCore::instance()->getBezierCurveSource()->UpdateDataObject();
		//mqMorphoDigCore::instance()->RedrawBezierCurves();
		//mqMorphoDigCore::instance()->getBezierCurveSource()->Update();
		//surfacesource->Update();
	}
};


//Select meshes, landmarks and tags ... first try!
void RubberBandSelect(vtkObject* caller,
	long unsigned int vtkNotUsed(eventId),
	void* vtkNotUsed(clientData),
	void* vtkNotUsed(callData))
{
	//std::cout << "Pick." << std::endl;
	//vtkRenderedAreaPicker* areaPicker = static_cast<vtkRenderedAreaPicker*>(caller);
	
	vtkAreaPicker* areaPicker = static_cast<vtkAreaPicker*>(caller);
	//vtkPropCollection *pcoll = areaPicker->GetPickList();

	/*vtkSmartPointer<vtkPoints> myPoints = areaPicker->GetClipPoints();
	for (vtkIdType i = 0; i < myPoints->GetNumberOfPoints(); i++)
	{
		double pt[3];
		double ori[3] = { 0,0,1 };
		myPoints->GetPoint(i, pt);
		mqMorphoDigCore::instance()->CreateLandmark(pt, ori, 0);

	}*/
	
	//pcoll->GetNumberOfItems();
	
	vtkProp3DCollection* props = areaPicker->GetProp3Ds();
	//areaPicker->Get
	cout << "Props Number of items:" << props->GetNumberOfItems() << endl;
	
		
	//props->PrintSelf(cout, vtkIndent(2));
	
	std::string action = "Rubber band actor selection-unselection";
	int something_to_store = 0;
	if (props->GetNumberOfItems() > 0) { something_to_store = 1; }
	
	props->InitTraversal();
	int Count = 0;

	if (something_to_store == 1)
	{
		Count = BEGIN_UNDO_SET(action);
	}
	
			
		
	
	props->InitTraversal();
	int landmarks_have_changed = 0;
	int mesh_actors_have_changed = 0;
	for (vtkIdType i = 0; i < props->GetNumberOfItems(); i++)
	{
		
		
		vtkProp3D *myprop3D = props->GetNextProp3D();
		//std::cout << "Actor prop:  class name:" << myprop3D->GetClassName() << std::endl;

		
		std::string str1("vtkLMActor");
		if (str1.compare(myprop3D->GetClassName()) == 0)
		{
			vtkLMActor *myActor;
			myActor = vtkLMActor::SafeDownCast(myprop3D);
			

				if (myActor->GetSelected() == 0)
				{
					myActor->SaveState(Count);
					myActor->SetChanged(1);
					myActor->SetSelected(1);

				}
				else
				{
					myActor->SaveState(Count);
					myActor->SetChanged(1);
					myActor->SetSelected(0);

					
				}
				landmarks_have_changed = 1;
			
		}

		if (mqMorphoDigCore::instance()->Getmui_MoveAll() == 1)
		{
			std::string str2("vtkMDActor");
			if (str2.compare(myprop3D->GetClassName()) == 0)
			{
				cout << "One mesh actor found!" << endl;
				vtkMDActor *myActor;
				myActor = vtkMDActor::SafeDownCast(myprop3D);


				if (myActor->IsInsideFrustum(areaPicker->GetFrustum()))
				{
					if (myActor->GetSelected() == 0)
					{
						myActor->SaveState(Count);
						myActor->SetChanged(1);
						myActor->SetSelected(1);

					}
					else
					{
						myActor->SaveState(Count);
						myActor->SetChanged(1);
						myActor->SetSelected(0);

					}
					mesh_actors_have_changed = 1;

				}
			}
		}
		
	}
	if (something_to_store == 1)
	{
		END_UNDO_SET();
	}
	//cout << "now update landmarks and send signals..." << endl;
	
	//DIRTY
	if (landmarks_have_changed == 1)
	{
		mqMorphoDigCore::instance()->getNodeLandmarkCollection()->Modified();
		mqMorphoDigCore::instance()->getHandleLandmarkCollection()->Modified();
		mqMorphoDigCore::instance()->signal_lmSelectionChanged();
		
	}
	if (mesh_actors_have_changed == 1)
	{
		//cout << "Signal actor selection changed!" << endl;
		mqMorphoDigCore::instance()->signal_actorSelectionChanged();
	}
	//cout << "Am I done ? " << endl;

}



#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

// Constructor
MorphoDig::MorphoDig(QWidget *parent) : QMainWindow(parent) {  

	setupUi(this); 
//MorphoDig::MorphoDig(){
	/*
	auto subWindowWidget = new QMainWindow;
    mdi->addSubWindow(subWindowWidget);

    auto dock1 = new QDockWidget("Dock1");
    dock1->setWidget(new QLabel("Label1"));
    dock1->setAllowedAreas(Qt::AllDockWidgetAreas);
    subWindowWidget->setCentralWidget(dock1);

    auto dock2 = new QDockWidget("Dock2");
    dock2->setWidget(new QLabel("Label2"));
    dock2->setAllowedAreas(Qt::AllDockWidgetAreas);
    subWindowWidget->addDockWidget(Qt::BottomDockWidgetArea, dock2);

	
	*/
	//auto subWindowWidget = new QMainWindow(this->TabProject);
	
	
	//this
	




	auto subWindowWidget = new QMainWindow();
	this->mdiArea->addSubWindow(subWindowWidget);
	/*auto dock1 = new QDockWidget("3D viewer");
	dock1->setWidget(qvtkWidget2);
	dock1->setAllowedAreas(Qt::AllDockWidgetAreas);
	subWindowWidget->setCentralWidget(dock1);
	*/
	this->qvtkWidget2 = new QVTKOpenGLWidget();
	qvtkWidget2->setObjectName(QStringLiteral("qvtkWidget"));
	QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	sizePolicy.setHorizontalStretch(1);
	sizePolicy.setVerticalStretch(1);
	sizePolicy.setHeightForWidth(qvtkWidget2->sizePolicy().hasHeightForWidth());
	qvtkWidget2->setSizePolicy(sizePolicy);
	qvtkWidget2->setMinimumSize(QSize(300, 300));

		
	subWindowWidget->setCentralWidget(qvtkWidget2);


	//subWindowWidget->Maximise
	subWindowWidget->showMaximized();
	auto mytree = new QTreeView(this->TabProject);
	auto dock2 = new QDockWidget("Actors");
	dock2->setWidget(mytree);
	dock2->setAllowedAreas(Qt::AllDockWidgetAreas);
	subWindowWidget->addDockWidget(Qt::RightDockWidgetArea, dock2);

	cout << "Try to set render window" << endl;
  auto window = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
  
  cout << "Set render window done" << endl;
  

	vtkObject::GlobalWarningDisplayOff();
	this->MorphoDigCore =  new mqMorphoDigCore();
	window->AddRenderer(this->MorphoDigCore->getRenderer());
	//vtkUndoStack* undoStack = vtkUndoStack::New();

	//@@@
	//this->ui = new Ui_MorphoDig;
	//this->ui->setupUi(this);
	//@@@

	this->MorphoDigCore->SetMainWindow(this);
	
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "MorphoMuseuM", "MorphoDig");
	cout<<".ini file path"<<  settings.fileName().toStdString()<<endl;
	settings.beginGroup("paths");
	this->MorphoDigCore->Setmui_LastUsedDir(settings.value("LastUsedDir", QDir::currentPath()).toString());
	settings.endGroup();
	settings.beginGroup("interaction_mode");
	this->MorphoDigCore->Setmui_MoveAll(settings.value("MoveAll",
		this->MorphoDigCore->Getmui_DefaultMoveAll()
	).toInt());
	settings.endGroup();

	settings.beginGroup("display_options");
	this->MorphoDigCore->Setmui_ShowGrid(settings.value("ShowGrid", 
		this->MorphoDigCore->Getmui_DefaultShowGrid()
		).toInt());
	this->MorphoDigCore->Setmui_ScalarVisibility(settings.value("ScalarVisibility",
		this->MorphoDigCore->Getmui_DefaultScalarVisibility()
	).toInt());
	this->MorphoDigCore->Setmui_GridSpacing(settings.value("GridSpacing",
		this->MorphoDigCore->Getmui_DefaultGridSpacing()
	).toDouble());
	this->MorphoDigCore->Setmui_SizeUnit(settings.value("SizeUnit",
		this->MorphoDigCore->Getmui_DefaultSizeUnit()
	).toString());
	this->MorphoDigCore->Setmui_ShowOrientationHelper(settings.value("ShowOrientationHelper", 
		this->MorphoDigCore->Getmui_DefaultShowOrientationHelper()
		).toInt());

	
	this->MorphoDigCore->Setmui_X1Label(settings.value("X1Label", this->MorphoDigCore->Getmui_DefaultX1Label()).toString());
	this->MorphoDigCore->Setmui_X2Label(settings.value("X2Label", this->MorphoDigCore->Getmui_DefaultX2Label()).toString());
	this->MorphoDigCore->Setmui_Y1Label(settings.value("Y1Label", this->MorphoDigCore->Getmui_DefaultY1Label()).toString());
	this->MorphoDigCore->Setmui_Y2Label(settings.value("Y2Label", this->MorphoDigCore->Getmui_DefaultY2Label()).toString());
	this->MorphoDigCore->Setmui_Z1Label(settings.value("Z1Label", this->MorphoDigCore->Getmui_DefaultZ1Label()).toString());
	this->MorphoDigCore->Setmui_Z2Label(settings.value("Z2Label", this->MorphoDigCore->Getmui_DefaultZ2Label()).toString());

	this->MorphoDigCore->Setmui_CameraCentreOfMassAtOrigin(settings.value("CameraCentreOfMassAtOrigin", 
		this->MorphoDigCore->Getmui_DefaultCameraCentreOfMassAtOrigin()
		).toInt());
	this->MorphoDigCore->Setmui_CameraOrtho(settings.value("CameraOrtho", 
		this->MorphoDigCore->Getmui_DefaultCameraOrtho()
		).toInt());
	settings.endGroup();
	settings.beginGroup("color_settings");
	double defaultMeshColor[4];
	double defaultFlagColor[3];
	double defaultBackGroundColor[3];
	double defaultBackGroundColor2[3];

	this->MorphoDigCore->Getmui_DefaultMeshColor(defaultMeshColor);
	this->MorphoDigCore->Getmui_DefaultFlagColor(defaultFlagColor);
	this->MorphoDigCore->Getmui_DefaultBackGroundColor(defaultBackGroundColor);
	this->MorphoDigCore->Getmui_DefaultBackGroundColor2(defaultBackGroundColor2);
	//cout << "defaultMeshColor[3]=" << defaultMeshColor[3] << endl;
	this->MorphoDigCore->Setmui_MeshColor(
	settings.value("MeshRed", 
		defaultMeshColor[0]
		).toDouble(),
	settings.value("MeshGreen", 
		defaultMeshColor[1]
		).toDouble(),
	settings.value("MeshBlue", 
		defaultMeshColor[2]
		).toDouble(),
	settings.value("MeshAlpha", 
		defaultMeshColor[3]
		).toDouble()
		);
	this->MorphoDigCore->Setmui_FlagColor(
		settings.value("FlagRed",
			defaultFlagColor[0]
			).toDouble(),
		settings.value("FlagGreen",
			defaultFlagColor[1]
			).toDouble(),
		settings.value("FlagBlue",
			defaultFlagColor[2]
			).toDouble()
		);
	this->MorphoDigCore->Setmui_BackGroundColor(
		settings.value("BackGroundRed", 
			defaultBackGroundColor[0]
			).toDouble(),
		settings.value("BackGroundGreen", 
			defaultBackGroundColor[1]
			).toDouble(),
		settings.value("BackGroundBlue", 
			defaultBackGroundColor[2]
			).toDouble()		
		);

	this->MorphoDigCore->Setmui_BackGroundColor2(
		settings.value("BackGroundRed2", 
			defaultBackGroundColor2[0]
			).toDouble(),
		settings.value("BackGroundGreen2", 
			defaultBackGroundColor2[1]
			).toDouble(),
		settings.value("BackGroundBlue2",
			defaultBackGroundColor2[2]
			).toDouble()
		);
	settings.endGroup();

	settings.beginGroup("landmark_settings");
	this->MorphoDigCore->Setmui_LandmarkBodyType(
		settings.value("LandmarkBodyType",
			this->MorphoDigCore->Getmui_DefaultLandmarkBodyType()
			).toInt()
			);

	this->MorphoDigCore->Setmui_LandmarkMode(
		settings.value("LandmarkMode",
			this->MorphoDigCore->Getmui_DefaultLandmarkMode()
			).toInt()
		);

	this->MorphoDigCore->Setmui_FlagRenderingSize(
		settings.value("FlagRenderingSize",
			this->MorphoDigCore->Getmui_DefaultFlagRenderingSize()
			).toDouble()
		);

	this->MorphoDigCore->Setmui_LandmarkRenderingSize(
		settings.value("LandmarkRenderingSize",
			this->MorphoDigCore->Getmui_DefaultLandmarkRenderingSize()
			).toDouble()
		);
	

	this->MorphoDigCore->Setmui_AdjustScaleFactor(
		settings.value("AdjustScaleFactor",
			this->MorphoDigCore->Getmui_DefaultAdjustScaleFactor()
			).toDouble()
		);
	//cout << settings.value("AdjustScaleFactor",
	//	this->MorphoDigCore->Getmui_DefaultAdjustScaleFactor()
	//	).toDouble() << endl;
	settings.endGroup();

	

	

	


	
	
	// Place the table view in the designer form
	//this->ui->tableFrame->layout()->addWidget(this->TableView->GetWidget());
	
	this->qvtkWidget2->SetRenderWindow(window);

	//this->MorphoDigCore->SetRenderWindow(this->ui->qvtkWidget->GetRenderWindow());
	this->MorphoDigCore->SetRenderWindow(window);

	//this->ui->qvtkWidget->GetRenderWindow()->SetAlphaBitPlanes(1);
	window->SetAlphaBitPlanes(1);

	//this->ui->qvtkWidget->GetRenderWindow()->SetMultiSamples(0);
	window->SetMultiSamples(0);

	//this->ui->qvtkWidget->GetRenderWindow()->SetStereoTypeToAnaglyph();
	window->SetStereoTypeToAnaglyph();
	//this->ui->qvtkWidget->GetRenderWindow()->StereoCapableWindowOn();
	window->StereoCapableWindowOn();
	settings.beginGroup("renderer_settings");
	this->MorphoDigCore->Setmui_Anaglyph(settings.value("Anaglyph", "0").toInt());
	settings.endGroup();

	

	//this->ui->qvtkWidget->GetRenderWindow()->StereoRenderOn();
	

	// VTK/Qt wedded


	//this->ui->qvtkWidget->GetRenderWindow()->AddRenderer(this->MorphoDigCore->getRenderer());

	//window->AddRenderer(this->MorphoDigCore->getRenderer()); 

	this->MorphoDigCore->getRenderer()->GradientBackgroundOn();
	
	this->MorphoDigCore->getRenderer()->SetBackground(this->MorphoDigCore->Getmui_BackGroundColor());
	this->MorphoDigCore->getRenderer()->SetBackground2(this->MorphoDigCore->Getmui_BackGroundColor2());
	//actor->SetmColor(this->MorphoDigCore->Getmui_MeshColor());
	//cout<< "Peeling was used:"<< this->MorphoDigCore->getRenderer()->GetLastRenderingUsedDepthPeeling();

	

	// 448/120 seems to be a good ratio!!! 3.73
	double multfactor = 1 / tan(this->MorphoDigCore->getCamera()->GetViewAngle() *  vtkMath::Pi() / 360.0);

	this->MorphoDigCore->getCamera()->SetPosition(120* multfactor, 0, 0);
	this->MorphoDigCore->getCamera()->SetFocalPoint(0, 0, 0);
	this->MorphoDigCore->getCamera()->SetViewUp(0, 0, 1);
	//double *viewup;
	//viewup= this->MorphoDigCore->getCamera()->GetViewUp();
	//cout << "Initial view up:" << viewup[0] << "," << viewup[1] << "," << viewup[2] << endl;
	//this->MorphoDigCore->getCamera()->Azimuth(90);// > Roll(-90); // Around "z" (profondeur) viewing axis!
	//this->MorphoDigCore->getCamera()->Roll(90); // around "x" (horizontal) viewing axis
	//this->MorphoDigCore->getCamera()->Elevation(180); // around "y" (vertical) viewing axis
	
	this->MorphoDigCore->getCamera()->SetParallelScale(120); 
	this->MorphoDigCore->ResetCameraOrthoPerspective();
	//this->MorphoDigCore->getCamera()->ParallelProjectionOn();




	// Just a bit of Qt interest: Culling off the
	// point data and handing it to a vtkQtTableView
	// VTK_CREATE(vtkDataObjectToTable, toTable);
	//toTable->SetInputConnection(elevation->GetOutputPort());
	//toTable->SetFieldType(vtkDataObjectToTable::POINT_DATA);
	//toTable->Update();
	// Here we take the end of the VTK pipeline and give it to a Qt View
	//this->TableView->SetRepresentationFromInputConnection(toTable->GetOutputPort());

	// Set up action signals and slots
	//connect(this->ui->actionOpenMesh, SIGNAL(triggered()), this, SLOT(slotOpenMeshFile()));
	

	
	

	
	connect(this->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));

	mqMorphoDigMenuBuilders::buildFileMenu(*this->menuFile);
	mqMorphoDigMenuBuilders::buildEditSelectedSurfacesMenu(*this->menuEditSelectedSurfaces);
	mqMorphoDigMenuBuilders::buildEditMenu(*this->menuEdit);
	mqMorphoDigMenuBuilders::buildLandmarksMenu(*this->menuLandmarks);
	mqMorphoDigMenuBuilders::buildHelpMenu(*this->menuHelp);
	mqMorphoDigMenuBuilders::buildToolbars(*this);

	


	double myorigin[3];
	myorigin[0] = 0;
	myorigin[1] = 0;
	myorigin[2] = 0;

	this->MorphoDigCore->getGridActor()->SetOutlineMode(this->MorphoDigCore->Getmui_CameraCentreOfMassAtOrigin());
	


	//@@ rubber band selection!
	
	 vtkSmartPointer<vtkMDInteractorStyle> style =
    vtkSmartPointer<vtkMDInteractorStyle>::New();
	 style->SetActorCollection(this->MorphoDigCore->getActorCollection());
	 style->SetNormalLandmarkCollection(this->MorphoDigCore->getNormalLandmarkCollection());
	 style->SetTargetLandmarkCollection(this->MorphoDigCore->getTargetLandmarkCollection());
	 style->SetNodeLandmarkCollection(this->MorphoDigCore->getNodeLandmarkCollection());
	 style->SetHandleLandmarkCollection(this->MorphoDigCore->getHandleLandmarkCollection());
	 style->SetFlagLandmarkCollection(this->MorphoDigCore->getFlagLandmarkCollection());
	//vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =		vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New(); //like paraview
	//vtkSmartPointer<vtkInteractorStyleTrackballActor> style =
	//	vtkSmartPointer<vtkInteractorStyleTrackballActor>::New();
	//vtkSmartPointer<vtkInteractorStyleSwitch> style =
	//	vtkSmartPointer<vtkInteractorStyleSwitch>::New();
	 vtkSmartPointer<vtkCallbackCommand> pickCallback =
		 vtkSmartPointer<vtkCallbackCommand>::New();

	 pickCallback->SetCallback(RubberBandSelect);
	 this->AreaPicker =
		 vtkSmartPointer<vtkAreaPicker>::New();

		//  this->AreaPicker =
		// vtkSmartPointer<vtkRenderedAreaPicker>::New();
	 this->AreaPicker->AddObserver(vtkCommand::EndPickEvent, pickCallback);
	
 style->SetCurrentRenderer(this->MorphoDigCore->getRenderer());
  this->qvtkWidget2->GetRenderWindow()->GetInteractor()->SetPicker(this->AreaPicker);
  this->qvtkWidget2->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);
  
  window->GetInteractor()->SetPicker(this->AreaPicker);
  window->GetInteractor()->SetInteractorStyle(style);


  mqMorphoDigCore::instance()->getBezierCurveSource()->SetHandles(mqMorphoDigCore::instance()->getHandleLandmarkCollection());
  mqMorphoDigCore::instance()->getBezierCurveSource()->SetNodes(mqMorphoDigCore::instance()->getNodeLandmarkCollection());
  
  

  mqMorphoDigCore::instance()->getBezierCurveSource()->Update();


  vtkSmartPointer<vtkMyNodeHandleCallBack> callback = vtkSmartPointer<vtkMyNodeHandleCallBack>::New();
  mqMorphoDigCore::instance()->getNodeLandmarkCollection()->AddObserver(vtkCommand::ModifiedEvent, callback);
  mqMorphoDigCore::instance()->getHandleLandmarkCollection()->AddObserver(vtkCommand::ModifiedEvent, callback);

  this->MorphoDigCore->getRenderer()->AddActor(mqMorphoDigCore::instance()->getBezierNHActor());
  this->MorphoDigCore->getRenderer()->AddActor(mqMorphoDigCore::instance()->getBezierSelectedActor());
  this->MorphoDigCore->getRenderer()->AddActor(mqMorphoDigCore::instance()->getBezierActor());


  this->MorphoDigCore->SetGridVisibility();
  this->MorphoDigCore->SetGridInfos();
  this->MorphoDigCore->InitializeOrientationHelper(); // creates orientation helper...
  this->MorphoDigCore->SetOrientationHelperVisibility();
  this->qvtkWidget2->SetRenderWindow(window);
  
  //EXAMPLE vtkBoxWidget

  /*
  vtkSmartPointer<vtkConeSource> coneSource =
	  vtkSmartPointer<vtkConeSource>::New();
  coneSource->SetHeight(1.5);
  vtkSmartPointer<vtkPolyDataMapper> mapper =
	  vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(coneSource->GetOutputPort());
  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  //NEW

  //NEW
  //DNEW
  this->MorphoDigCore->getRenderer()->AddActor(actor);

  vtkSmartPointer<vtkBoxWidget> boxWidget = vtkSmartPointer<vtkBoxWidget>::New();
  boxWidget->SetInteractor(this->ui->qvtkWidget->GetRenderWindow()->GetInteractor());
  boxWidget->SetProp3D(actor);
  boxWidget->SetPlaceFactor(1.25);
  boxWidget->PlaceWidget();
  vtkSmartPointer<vtkMyCallback> callback = vtkSmartPointer<vtkMyCallback>::New();
  boxWidget->AddObserver(vtkCommand::InteractionEvent, callback);
  boxWidget->On();*/


  
  

  //EXAMPLE BEZIER SOURCE + BEZIER WIDGET
  /*
  unsigned int controlPointsX = 4;
  unsigned int controlPointsY = 4;
  
  // Create an arbitrary plane that we can use
  // to initialize the control points for the Bezier surface
  vtkSmartPointer<vtkPlaneSource> planeSource =
	  vtkSmartPointer<vtkPlaneSource>::New();
  planeSource->SetOrigin(0, 0, 0);
  planeSource->SetPoint2(1, 0, 0);
  planeSource->SetPoint1(0, 0, 1);
  planeSource->SetXResolution(controlPointsX - 1);
  planeSource->SetYResolution(controlPointsY - 1);
  planeSource->Update();

  // Create a transformation to be applied to the Bezier surface
  vtkSmartPointer<vtkTransform> transform =
	  vtkSmartPointer<vtkTransform>::New();
  transform->Scale(2, 1, 1);

  // Create the Bezier surface that matches the plane created above
  // and scaled by the transformation
  vtkSmartPointer<vtkBezierSurfaceSource> bezierSource =
	  vtkSmartPointer<vtkBezierSurfaceSource>::New();
  bezierSource->SetNumberOfControlPoints(controlPointsX, controlPointsY);

  vtkPolyData* planePoly = planeSource->GetOutput();

  unsigned int controlPointCount = 0;
  for (unsigned int i = 0; i < controlPointsX; ++i)
  {
	  for (unsigned int j = 0; j < controlPointsY; ++j)
	  {
		  double point[3];
		  planePoly->GetPoint(i*controlPointsY + j, point);
		  //          std::cout << "Point " << controlPointCount << ": "
		  //                    << point[0] << " " << point[1] << " " << point[2] << std::endl;
		  bezierSource->SetControlPoint(i, j, point);
		  controlPointCount++;
	  }
  }

  std::cout << "Created " << controlPointCount << " control points." << std::endl;

  bezierSource->SetTransform(transform);
  bezierSource->Update();
  
  vtkBezierSurfaceWidget *widget = vtkBezierSurfaceWidget::New();
	  
  widget->SetInteractor(this->ui->qvtkWidget->GetRenderWindow()->GetInteractor());
  widget->SetBezierSource(bezierSource);
  widget->SetHandleSize(0.1);
  widget->On();

  vtkSmartPointer<vtkPolyDataMapper> Bmapper =
	  vtkSmartPointer<vtkPolyDataMapper>::New();
  Bmapper->SetInputConnection(bezierSource->GetOutputPort());

  vtkSmartPointer<vtkActor> Bactor =
	  vtkSmartPointer<vtkActor>::New();
  Bactor->SetMapper(Bmapper);
  
  this->MorphoDigCore->getRenderer()->AddActor(Bactor);
  */

 

  //@@ end rubber band selection!


};



MorphoDig::~MorphoDig()
{
	/*if (MorphoDig::Instance == this)
	{
		MorphoDig::Instance = 0;
	}*/
	saveSettings();
	
	// The smart pointers should clean up for up
	//this->OrientationHelperWidget->Delete();
}


void MorphoDig::saveSettings()
{
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "MorphoMuseuM", "MorphoDig");
	//cout<<"try save settings:" << m_sSettingsFile.toStdString();

	settings.beginGroup("paths");
	settings.setValue("LastUsedDir", this->MorphoDigCore->Getmui_LastUsedDir());
	settings.endGroup();
	settings.beginGroup("interaction_mode");
	settings.setValue("MoveAll", this->MorphoDigCore->Getmui_MoveAll());
	settings.endGroup();
	settings.beginGroup("display_options");
	settings.setValue("ShowGrid", this->MorphoDigCore->Getmui_ShowGrid());
	settings.setValue("ScalarVisibility", this->MorphoDigCore->Getmui_ScalarVisibility());
	
	settings.setValue("GridSpacing", this->MorphoDigCore->Getmui_GridSpacing());
	settings.setValue("SizeUnit", this->MorphoDigCore->Getmui_SizeUnit());
	settings.setValue("ShowOrientationHelper", this->MorphoDigCore->Getmui_ShowOrientationHelper());
	settings.setValue("CameraCentreOfMassAtOrigin", this->MorphoDigCore->Getmui_CameraCentreOfMassAtOrigin());
	settings.setValue("CameraOrtho", this->MorphoDigCore->Getmui_CameraOrtho());

	
	settings.setValue("X1Label", this->MorphoDigCore->Getmui_X1Label());
	settings.setValue("X2Label", this->MorphoDigCore->Getmui_X2Label());
	settings.setValue("Y1Label", this->MorphoDigCore->Getmui_Y1Label());
	settings.setValue("Y2Label", this->MorphoDigCore->Getmui_Y2Label());
	settings.setValue("Z1Label", this->MorphoDigCore->Getmui_Z1Label());
	settings.setValue("Z2Label", this->MorphoDigCore->Getmui_Z2Label());
	settings.endGroup();	

	settings.beginGroup("color_settings");
	settings.setValue("BackGroundRed", this->MorphoDigCore->Getmui_BackGroundColor()[0]);
	settings.setValue("BackGroundGreen", this->MorphoDigCore->Getmui_BackGroundColor()[1]);
	settings.setValue("BackGroundBlue", this->MorphoDigCore->Getmui_BackGroundColor()[2]);

	settings.setValue("BackGroundRed2", this->MorphoDigCore->Getmui_BackGroundColor2()[0]);
	settings.setValue("BackGroundGreen2", this->MorphoDigCore->Getmui_BackGroundColor2()[1]);
	settings.setValue("BackGroundBlue2", this->MorphoDigCore->Getmui_BackGroundColor2()[2]);


	settings.setValue("MeshRed", this->MorphoDigCore->Getmui_MeshColor()[0]);
	settings.setValue("MeshGreen", this->MorphoDigCore->Getmui_MeshColor()[1]);
	settings.setValue("MeshBlue", this->MorphoDigCore->Getmui_MeshColor()[2]);
	settings.setValue("MeshAlpha", this->MorphoDigCore->Getmui_MeshColor()[3]);

	settings.setValue("FlagRed", this->MorphoDigCore->Getmui_FlagColor()[0]);
	settings.setValue("FlagGreen", this->MorphoDigCore->Getmui_FlagColor()[1]);
	settings.setValue("FlagBlue", this->MorphoDigCore->Getmui_FlagColor()[2]);
	

	

	settings.endGroup();
	
	settings.beginGroup("landmark_settings");
	settings.setValue("LandmarkBodyType", this->MorphoDigCore->Getmui_LandmarkBodyType());
	settings.setValue("LandmarkMode", this->MorphoDigCore->Getmui_LandmarkMode());
	settings.setValue("LandmarkRenderingSize", this->MorphoDigCore->Getmui_LandmarkRenderingSize());
	settings.setValue("FlagRenderingSize", this->MorphoDigCore->Getmui_FlagRenderingSize());
	
	settings.setValue("AdjustLandmarkRenderingSize", this->MorphoDigCore->Getmui_AdjustLandmarkRenderingSize());
	settings.setValue("AdjustScaleFactor", this->MorphoDigCore->Getmui_AdjustScaleFactor());
	
	settings.endGroup();
	settings.beginGroup("renderer_settings");
	settings.setValue("Anaglyph", this->MorphoDigCore->Getmui_Anaglyph());	
	settings.endGroup();
	//cout << "end save settings" << endl;
}





// problème
// aujourd'hui, c'est cette seule fonction qui est appelée lorsqu'on ouvre un nouveau maillage
// et lorsqu'on change entre "camera



void MorphoDig::slotExit() {
	//maybe we should save the .ini files!
	qApp->exit();
}





