#include "MorphoDig.h"


//#include <vtkImageStack.h>
#include <vtkImageAppend.h>
#include <vtkImageData.h>
#include <vtkTIFFReader.h>
#include <vtkPNGReader.h>
#include <vtkBMPReader.h>
#include <vtkDICOMImageReader.h>
#include "mqOpenDicomStackDialog.h"
#include "mqOpenTiff3DDialog.h"
#include "mqMorphoDigMenuBuilders.h"
#include "mqOpenRawDialog.h"
#include "mqCoreUtilities.h"
#include "mqOpenDataReaction.h"
#include "vtkBezierSurfaceWidget.h"
#include "vtkBezierSurfaceSource.h"
#include "vtkBezierCurveSource.h"
#include "vtkMDActor.h"
#include "vtkLMActor.h"
#include "mqMorphoDigCore.h"
#include "mqUndoStack.h"
#include "vtkMDInteractorStyle.h"
#include <vtkImageViewer2.h>


//#include "vtkMDLassoInteractorStyle.h"
#include "vtkMDActorCollection.h"
#if VTK_MAJOR_VERSION<8
#include <QVTKWidget.h>
#elseif VTK_MAJOR_VERSION == 8 && VTK_MINOR_VERSION < 2
#include <QVTKOpenGLWidget.h>
#else
#include <QVTKOpenGLNativeWidget.h>
#endif

//#include "vtkUndoStack.h"
//#include "vtkUndoSet.h"
//#include "vtkUndoElement.h"
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vtkLightCollection.h>
#include <vtkLight.h>
#include <vtkPiecewiseFunction.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkInteractorStyleDrawPolygon.h>
#include <vtkInteractorStyleRubberBand3D.h>
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
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkInteractorStyleRubberBandPick.h>
#include <vtkInteractorStyleSwitch.h>
#include <vtkCallbackCommand.h>
//#include <vtkAreaPicker.h>
#include <vtkRenderedAreaPicker.h>
#include <vtkLine.h>
#include <vtkProp3DCollection.h>

#include <QDragEnterEvent>
#include <QMimeData>
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

//#include <vtkRenderWindowInteractor.h>
//#include <QVTKInteractor.h>

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



/*
class vtkMyTextBoxCallback : public vtkCommand
{
public:
	static vtkMyTextBoxCallback *New() { return new vtkMyTextBoxCallback; }
	virtual void Execute(vtkObject *caller, unsigned long, void*)
	{    // Here we use the vtkBoxWidget to transform the underlying coneActor   
		 // (by manipulating its transformation matrix).  
		vtkSmartPointer<vtkTransform> t = vtkSmartPointer<vtkTransform>::New();
		vtkBoxWidget *widget = reinterpret_cast<vtkBoxWidget*>(caller);
		widget->GetTransform(t);    widget->GetProp3D()->SetUserTransform(t);
	}
};
*/

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

void lassoselect(vtkObject* caller,
	long unsigned int vtkNotUsed(eventId),
	void* vtkNotUsed(clientData),
	void* vtkNotUsed(callData))
{
	cout << "Lasso selection changed!" << endl;
	mqMorphoDigCore::instance()->stopLasso();

}

void rubberselect(vtkObject* caller,
	long unsigned int vtkNotUsed(eventId),
	void* vtkNotUsed(clientData),
	void* vtkNotUsed(callData))
{
	cout << "Rubber selection changed!" << endl;
	mqMorphoDigCore::instance()->stopRubber();

}



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
	int volume_actors_have_changed = 0;
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

		//if not "only select landmarks".
		if (mqMorphoDigCore::instance()->Getmui_MoveMode() != 0)
		{
			std::string str2("vtkMDActor");
			if (str2.compare(myprop3D->GetClassName()) == 0)
			{
				cout << "One surface actor found!" << endl;
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
			std::string str3("vtkMDVolume");
			if (str3.compare(myprop3D->GetClassName()) == 0)
			{
				cout << "One volume actor found!" << endl;
				vtkMDVolume *myVolume;
				myVolume = vtkMDVolume::SafeDownCast(myprop3D);


				if (myVolume->IsInsideFrustum(areaPicker->GetFrustum()))
				{
					if (myVolume->GetSelected() == 0)
					{
						myVolume->SaveState(Count);
						myVolume->SetChanged(1);
						myVolume->SetSelected(1);

					}
					else
					{
						myVolume->SaveState(Count);
						myVolume->SetChanged(1);
						myVolume->SetSelected(0);

					}
					volume_actors_have_changed = 1;

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
	if (volume_actors_have_changed == 1)
	{
		//cout << "Signal actor selection changed!" << endl;
		mqMorphoDigCore::instance()->signal_volumeSelectionChanged();
	}
	//cout << "Am I done ? " << endl;

}



#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

// Constructor
MorphoDig::MorphoDig(QWidget *parent) : QMainWindow(parent) {  

	setupUi(this); 
//MorphoDig::MorphoDig(){
	

	
	auto projectTabWindow = new QMainWindow(this->tabWidget);
	
	
	//this
	QWidget *projectTab = new QWidget();
	QHBoxLayout *projectLayout = new QHBoxLayout;
	projectLayout->addWidget(projectTabWindow);

	projectTab->setLayout(projectLayout);
	this->tabWidget->addTab(projectTab, "Project");

	QWidget *segmentationTab = new QWidget();
	QHBoxLayout *segmentationLayout = new QHBoxLayout;
	auto segmentationTabWindow = new QMainWindow(this->tabWidget);
	segmentationLayout->addWidget(segmentationTabWindow);
	segmentationTab->setLayout(segmentationLayout);
	//this->tabWidget->addTab(segmentationTab, "Segmentation");
	//segmentationTab->setDisabled(true);
	//auto projectTabWindow = new QMainWindow();
	
	//this->mdiArea->addSubWindow(projectTabWindow);

	
	
#if VTK_MAJOR_VERSION<8	
	this->qvtkWidget2 = new QVTKWidget();
	this->segView1 = new QVTKWidget();
	this->segView2 = new QVTKWidget();
	this->segView3 = new QVTKWidget();
	this->segView4 = new QVTKWidget();
#elseif VTK_MAJOR_VERSION == 8 && VTK_MINOR_VERSION < 2
	this->qvtkWidget2 = new QVTKOpenGLWidget();	
this->segView1 = new QVTKOpenGLWidget();
this->segView2 = new QVTKOpenGLWidget();
this->segView3 = new QVTKOpenGLWidget();
this->segView4 = new QVTKOpenGLWidget();
#else
	this->qvtkWidget2 = new QVTKOpenGLNativeWidget();		
	this->segView1= new QVTKOpenGLNativeWidget();
	this->segView2 = new QVTKOpenGLNativeWidget();
	this->segView3 = new QVTKOpenGLNativeWidget();
	this->segView4 = new QVTKOpenGLNativeWidget();
#endif
	

	qvtkWidget2->setObjectName(QStringLiteral("qvtkWidget"));
	QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	sizePolicy.setHorizontalStretch(1);
	sizePolicy.setVerticalStretch(1);
	sizePolicy.setHeightForWidth(qvtkWidget2->sizePolicy().hasHeightForWidth());
	qvtkWidget2->setSizePolicy(sizePolicy);
	qvtkWidget2->setMinimumSize(QSize(300, 300));
	//this->segView1->setSizePolicy(sizePolicy);
	this->segView1->setMinimumSize(QSize(200,200));
	
	projectTabWindow->setCentralWidget(qvtkWidget2);
	QWidget *segCentralWidget = new QWidget;

	segmentationTabWindow->setCentralWidget(segCentralWidget);
	QHBoxLayout *hLayout1 = new QHBoxLayout;
	QHBoxLayout *hLayout2 = new QHBoxLayout;
	QVBoxLayout *vLayout = new QVBoxLayout;
	//QGridLayout *gLayout = new QGridLayout;

	//segmentationLayout
	//segmentationLayout->addWidget(this->segView1);
	hLayout1->addWidget(this->segView1);
	hLayout1->addWidget(this->segView2);
	hLayout2->addWidget(this->segView3);
	hLayout2->addWidget(this->segView4);
	vLayout->addItem(hLayout1);
	vLayout->addItem(hLayout2);

	//segmentationTabWindow->setLayout(vLayout);
	
	segCentralWidget->setLayout(vLayout);
	

	//this->segView1->show();
	/*auto dock1 = new QDockWidget("3D viewer");
	dock1->setWidget(qvtkWidget2);
	dock1->setAllowedAreas(Qt::AllDockWidgetAreas);*/
	
	//projectTabWindow->setCentralWidget(dock1);
	


	//projectTabWindow->Maximise

	projectTabWindow->showMaximized();
	//segmentationTabWindow->showMaximized();
	
	//auto mytree = new QTreeView(this->TabProject);
	/*auto mytree = new QTreeView(this->tabWidget);
	auto dock2 = new QDockWidget("Actors");
	dock2->setWidget(mytree);
	dock2->setAllowedAreas(Qt::AllDockWidgetAreas);
	projectTabWindow->addDockWidget(Qt::RightDockWidgetArea, dock2);*/

	projectTabWindow->setWindowModality(Qt::WindowModal);
	//segmentationTabWindow->setWindowModality(Qt::WindowModal);
	/*Qt::WindowFlags flags = windowFlags();
	Qt::WindowFlags closeFlag = Qt::WindowCloseButtonHint;
	flags = flags & (~closeFlag);
	projectTabWindow->setWindowFlags(flags);*/

	cout << "Try to set render window" << endl;
  auto window = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
  auto segrenwindow1 = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
  auto segrenwindow2 = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
  auto segrenwindow3 = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();

  vtkSmartPointer< vtkImageViewer2> segViewer1 = vtkSmartPointer< vtkImageViewer2>::New();
  vtkSmartPointer< vtkImageViewer2> segViewer2 = vtkSmartPointer< vtkImageViewer2>::New();
  vtkSmartPointer< vtkImageViewer2> segViewer3 = vtkSmartPointer< vtkImageViewer2>::New();

  segViewer1->SetRenderWindow(segrenwindow1);
  segViewer2->SetRenderWindow(segrenwindow2);
  segViewer3->SetRenderWindow(segrenwindow3);
  segViewer1->GetRenderer()->SetBackground(0.3, 0, 0);
  segViewer2->GetRenderer()->SetBackground(0,0.3, 0);
  segViewer3->GetRenderer()->SetBackground(0, 0, 0.3);

 /* riw[i] = vtkSmartPointer< vtkResliceImageViewer >::New();
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    riw[i]->SetRenderWindow(renderWindow);*/


  cout << "Set render window done" << endl;
  

	vtkObject::GlobalWarningDisplayOff();
	this->MorphoDigCore =  new mqMorphoDigCore();
	this->MorphoDigCore->setQVTKWidget(this->qvtkWidget2);
	this->MorphoDigCore->setSegmentationView1(this->segView1);
	this->MorphoDigCore->setSegmentationView2(this->segView2);
	this->MorphoDigCore->setSegmentationView3(this->segView3);
	this->MorphoDigCore->setSegmentationView4(this->segView4);

	this->segView1->SetRenderWindow(segViewer1->GetRenderWindow());
	segViewer1->SetupInteractor(this->segView1->GetRenderWindow()->GetInteractor());

	this->segView2->SetRenderWindow(segViewer2->GetRenderWindow());
	segViewer2->SetupInteractor(this->segView2->GetRenderWindow()->GetInteractor());
	this->segView3->SetRenderWindow(segViewer3->GetRenderWindow());
	segViewer3->SetupInteractor(this->segView3->GetRenderWindow()->GetInteractor());

	MorphoDigCore->setSegViewer1(segViewer1);
	MorphoDigCore->setSegViewer2(segViewer2);
	MorphoDigCore->setSegViewer3(segViewer3);

	MorphoDigCore->SetProjectWindow(projectTabWindow);
	//MorphoDigCore->SetSegmentationWindow(segmentationTabWindow);
	//window->AddRenderer(this->MorphoDigCore->getRenderer());
	this->qvtkWidget2->SetRenderWindow(window);
	this->qvtkWidget2->GetRenderWindow()->AddRenderer(this->MorphoDigCore->getRenderer());

	this->MorphoDigCore->setCurrentCursor(0);
	//vtkUndoStack* undoStack = vtkUndoStack::New();

	//@@@
	//this->ui = new Ui_MorphoDig;
	//this->ui->setupUi(this);
	//@@@

	this->MorphoDigCore->SetMainWindow(this);
	this->MorphoDigCore->InitStatusBar();
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "MorphoMuseuM", "MorphoDig");
	cout<<".ini file path"<<  settings.fileName().toStdString()<<endl;
	settings.beginGroup("paths");
	this->MorphoDigCore->Setmui_LastUsedDir(settings.value("LastUsedDir", QDir::currentPath()).toString());
	settings.endGroup();
	settings.beginGroup("interaction_mode");
	this->MorphoDigCore->Setmui_MoveMode(settings.value("MoveMode",
		this->MorphoDigCore->Getmui_DefaultMoveMode()
	).toInt());
	settings.endGroup();

	settings.beginGroup("display_options");
	this->MorphoDigCore->Setmui_ShowGrid(settings.value("ShowGrid", 
		this->MorphoDigCore->Getmui_DefaultShowGrid()
		).toInt());
	this->MorphoDigCore->Setmui_ArrayVisibility(settings.value("ScalarVisibility",
		this->MorphoDigCore->Getmui_DefaultArrayVisibility()
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
	int defaultAmbient;
	int defaultDiffuse;
	int defaultSpecular;
	double defaultSpecularPower;
	
	defaultAmbient = this->MorphoDigCore->Getmui_DefaultAmbient();
	defaultDiffuse = this->MorphoDigCore->Getmui_DefaultDiffuse();
	defaultSpecular = this->MorphoDigCore->Getmui_DefaultSpecular();
	defaultSpecularPower = this->MorphoDigCore->Getmui_DefaultSpecularPower();
	
	this->MorphoDigCore->Setmui_Ambient(settings.value("Ambient",defaultAmbient).toInt());
	
	this->MorphoDigCore->Setmui_Diffuse(settings.value("Diffuse", defaultDiffuse).toInt());
	this->MorphoDigCore->Setmui_Specular(settings.value("Specular", defaultSpecular).toInt());
	this->MorphoDigCore->Setmui_SpecularPower(settings.value("SpecularPower", defaultSpecularPower).toDouble());

	
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

	int defaultFontSize = this->MorphoDigCore->Getmui_DefaultFontSize();
	this->MorphoDigCore->Setmui_FontSize(settings.value("FontSize", defaultFontSize).toInt());


	//cout << settings.value("AdjustScaleFactor",
	//	this->MorphoDigCore->Getmui_DefaultAdjustScaleFactor()
	//	).toDouble() << endl;
	settings.endGroup();
	settings.beginGroup("curve_settings");
	this->MorphoDigCore->Setmui_SegmentDecimation(
		settings.value("SegmentDecimation",
			this->MorphoDigCore->Getmui_DefaultSegmentDecimation()
		).toInt()
	);
	int nsegments = settings.value("nsegments", 0).toInt();
	if (nsegments > 0)
	{
		
		for (int i = 0; i < nsegments; i++)
		{			

			QString d = QString("d");
			d = d + QString::number(i);
			cout << "Segment " << i << ": decimation = " << settings.value(d, 0).toString().toStdString() << endl;

			int decimation = settings.value(d, 0).toInt();
			mqMorphoDigCore::instance()->Getmui_SegmentDecimations()->decimation.push_back(decimation);
			
		}
	}


	settings.endGroup();
	settings.beginGroup("volumes");
	long long int iniVolumeOutOfCoreThreshold = settings.value("VolumeOutOfCoreThreshold", 10).toLongLong();
	//cout << "iniVolumeOutOfCoreThreshold" << iniVolumeOutOfCoreThreshold << endl;
	//settings.setValue("VolumeOutOfCoreThreshold", this->MorphoDigCore->Getmui_VolumeOutOfCoreThreshold());
	long long int defaultVolumeOutOfCoreThreshold = this->MorphoDigCore->Getmui_DefaultVolumeOutOfCoreThreshold();
	this->MorphoDigCore->Setmui_VolumeOutOfCoreThreshold(settings.value("VolumeOutOfCoreThreshold", defaultVolumeOutOfCoreThreshold).toLongLong());
	
	
		
		int defaultVolumeDisplaySlice = this->MorphoDigCore->Getmui_DefaultVolumeDisplaySlice();

	this->MorphoDigCore->Setmui_VolumeDisplaySlice(settings.value("VolumeDisplaySlice", defaultVolumeDisplaySlice).toInt());
	
	//settings.setValue("VolumeDisplaySlice", this->MorphoDigCore->Getmui_VolumeDisplaySlice());
	settings.endGroup();

	settings.beginGroup("colormaps");
	cout<<"Number of custom colormaps:"<<settings.value("nr", 0).toInt()<<endl;
	int nr = settings.value("nr", 0).toInt();
	if (nr > 0)
	{
		for (int i = 0; i < nr; i++)
		{
			vtkSmartPointer<vtkDiscretizableColorTransferFunction> newSTC = vtkSmartPointer<vtkDiscretizableColorTransferFunction>::New();

			QString cm = QString("cm");
			cm = cm + QString::number(i);
			cout << "Color map" << i << ":" << settings.value(cm, "Colormap").toString().toStdString() << endl;
			
			QString name = settings.value(cm, "Colormap").toString();
			
			// Name : settings.value(cm, "Colormap").toString()
			QString cmnc = cm + "nc";
			int nc = settings.value(cmnc, 0).toInt();
			cout << "Color nodes:" << endl;
			//cm0discretize = 1
			//cm0discretizenr = 10
			//cm0enableopacity = 1
			QString cmdiscretize = cm + "discretize";
			QString cmdiscretizenr = cm + "discretizenr";
			QString cmenableopacity = cm + "enableopacity";

			
			newSTC->SetColorSpaceToRGB();


			int discretize = settings.value(cmdiscretize, 0).toInt();
			newSTC->SetDiscretize(discretize);

			int discretizenr = settings.value(cmdiscretizenr, 256).toInt();
			newSTC->SetNumberOfValues(discretizenr);
			int enableopacity = settings.value(cmenableopacity, 1).toInt();
			newSTC->SetEnableOpacityMapping(enableopacity);
			
			for (int j = 0; j < nc; j++)
			{
				QString cmcx = cm + "cx" + QString::number(j);
				QString cmr = cm + "r" + QString::number(j);
				QString cmg = cm + "g" + QString::number(j);
				QString cmb = cm + "b" + QString::number(j);
				double cx = settings.value(cmcx, 0).toDouble();
				double r = settings.value(cmr, 0).toDouble();
				double g = settings.value(cmg, 0).toDouble();
				double b = settings.value(cmb, 0).toDouble();
				cout << "cx" << j << ":" << settings.value(cmcx, 0).toDouble() << ",";
				cout << "rgb:"<< settings.value(cmr, 0).toDouble() << ",";
				cout  << settings.value(cmg, 0).toDouble() << ",";
				cout  << settings.value(cmb, 0).toDouble() << endl;
				newSTC->AddRGBPoint(cx, r, g, b);
				
			}

			vtkSmartPointer<vtkPiecewiseFunction> opacityfunction = vtkSmartPointer<vtkPiecewiseFunction>::New();
			QString cmno = cm + "no";
			int no = settings.value(cmno, 0).toInt();
			cout << "Opacity nodes:" << endl;
			for (int j = 0; j < no; j++)
			{
				QString cmox = cm + "ox" + QString::number(j);
				QString cmov = cm + "ov" + QString::number(j);
				double ox = settings.value(cmox, 0).toDouble();
				double ov = settings.value(cmov, 0).toDouble();
				cout << "ox" << j << ":" << settings.value(cmox, 0).toDouble() << ",";
				cout << "ov=" << j << ":" << settings.value(cmov, 0).toDouble() << endl;
				opacityfunction->AddPoint(ox, ov);
			}

			newSTC->SetScalarOpacityFunction(opacityfunction);
			newSTC->Build();


			cout << "Add this map to !!(morphodig constructor)" << endl;
			this->MorphoDigCore->Getmui_ExistingColorMaps()->Stack.push_back(ExistingColorMaps::Element(name, newSTC, 1));

		}
	}
	
	settings.endGroup();
	

	settings.beginGroup("tagmaps");
	cout << "Number of tagmaps:" << settings.value("nr", 0).toInt() << endl;
	 nr = settings.value("nr", 0).toInt();
	 int cpt = 0;
	 int custom = 0;
	if (nr > 0)
	{
		for (int i = 0; i < nr; i++)
		{
			vtkSmartPointer<vtkLookupTable> TagLut = vtkSmartPointer<vtkLookupTable>::New();
			std::vector<std::string> tagNames;
			if (i == 0)
			{
				TagLut= this->MorphoDigCore->GetTagLut();
			}
			QString tm = QString("tm");

			tm = tm + QString::number(i);
			cout << "Tagmap map" << i << ":" << settings.value(tm, "TagMap").toString().toStdString() << endl;

			QString TagMapName = settings.value(tm, "TagMap").toString();
			if (i == 0)
			{
				TagMapName = "TagMap";
			}

			// Name : settings.value(cm, "Colormap").toString()
			QString tmnt = tm + "nt";
			int nt = settings.value(tmnt, 0).toInt();
			cout << "Tags:" << endl;
			//cm0discretize = 1
			//cm0discretizenr = 10
			//cm0enableopacity = 1
			
			TagLut->SetNumberOfTableValues(nt);
			TagLut->Build();
			for (int j = 0; j < nt; j++)
			{
				QString tmt = tm + "t" + QString::number(j); 
				QString tmtr = tmt + "r";
				QString tmtg = tmt + "g";
				QString tmtb = tmt + "b";
				QString tmta = tmt + "a";
				QString Tag = "Tag"+ QString::number(j);
				QString tname = settings.value(tmt, Tag).toString();				
				double r = settings.value(tmtr, 0).toDouble();
				double g = settings.value(tmtg, 0).toDouble();
				double b = settings.value(tmtb, 0).toDouble();
				double	a = settings.value(tmta, 1.0).toDouble();

				cout << "Tag:" << j << ":" << tname.toStdString() << ",";
				cout << "rgb:" << r << ",";
				cout << g << ",";
				cout << b << ",";
				cout << a << endl;
				TagLut->SetTableValue(j, r, g,b, a);  //Grey
				tagNames.push_back(tname.toStdString());				
			}
			
			if (i > 0) { custom = 1; }

			this->MorphoDigCore->Getmui_ExistingTagMaps()->Stack.push_back(ExistingTagMaps::Element(TagMapName, TagLut, nt, tagNames, custom));
			if (i == 0)
			{
				this->MorphoDigCore->Getmui_ActiveTagMap()->TagMap = TagLut;
				cout << "Set Active color map2!" << endl;

				this->MorphoDigCore->Getmui_ActiveTagMap()->Name = TagMapName;
				this->MorphoDigCore->Getmui_ActiveTagMap()->numTags = nt;
				this->MorphoDigCore->Getmui_ActiveTagMap()->tagNames = tagNames;
			}

			

		}
	}
	else
	{
		// si rien dans le fichier .ini : 
		int tagnr = 25;
		vtkSmartPointer<vtkLookupTable> TagLut = vtkSmartPointer<vtkLookupTable>::New();		
		TagLut = this->MorphoDigCore->GetTagLut();	

		this->MorphoDigCore->GetTagLut()->SetNumberOfTableValues(tagnr);
		this->MorphoDigCore->GetTagLut()->Build();
		std::vector<std::string> tagNames;
		// Fill in a few known colors, the rest will be generated if needed

		//TagLut->SetTableValue(0, 0.66, 0.33, 1, 1);  //Violet
		for (int i = 0; i < tagnr; i++)
		{
			double rgba[4];
			cout << "i="<<i<< "try to get rgba" << endl;
			this->MorphoDigCore->GetDefaultTagColor(i, rgba);
			cout << "rgba:" << rgba[0] << ","<< rgba[1] << ","<< rgba[2] << "," <<rgba[3] << endl;
			TagLut->SetTableValue(i, rgba[0], rgba[1], rgba[2], rgba[3]);
			if (i == 0)
			{
				tagNames.push_back("Exterior");
			}
			else
			{
				QString TagName ="Tag" + QString::number(i);
				tagNames.push_back(TagName.toStdString());
			}
		}
		cout << "End i loop" << endl;
		QString TagMap = QString("TagMap");
		cout << "Try to set existing color maps!!" << endl;

		this->MorphoDigCore->Getmui_ExistingTagMaps()->Stack.push_back(ExistingTagMaps::Element(TagMap, this->MorphoDigCore->GetTagLut(), tagnr, tagNames, 0));
		this->MorphoDigCore->Getmui_ActiveTagMap()->TagMap = this->MorphoDigCore->GetTagLut();
		cout << "Set Active color map2!" << endl;

		this->MorphoDigCore->Getmui_ActiveTagMap()->Name = TagMap;
		this->MorphoDigCore->Getmui_ActiveTagMap()->numTags = tagnr;
		this->MorphoDigCore->Getmui_ActiveTagMap()->tagNames = tagNames;
	}
	/*
	
	



	
	*/


	
	
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
	settings.endGroup();
	settings.beginGroup("renderer_settings");
	this->MorphoDigCore->Setmui_Anaglyph(settings.value("Anaglyph", "0").toInt());
	//cout read ini file!
	this->MorphoDigCore->Setmui_DisplayMode(settings.value("DisplayMode", "0").toInt());
	cout << "In .ini file: Anaglyph = " << settings.value("Anaglyph", "0").toInt() << endl;
	cout << "In .ini file: DisplayMode = " << settings.value("DisplayMode", "0").toInt() << endl;

	
	double defaultRendererOcclusionRatio = this->MorphoDigCore->Getmui_DefaultRendererOcclusionRatio();
	int defaultRendererMaximalNumberOfPeels = this->MorphoDigCore->Getmui_DefaultRendererMaximalNumberOfPeels();
	this->MorphoDigCore->Setmui_RendererOcclusionRatio(settings.value("OcclusionRatio", defaultRendererOcclusionRatio).toDouble());
	this->MorphoDigCore->Setmui_RendererMaximalNumberOfPeels(settings.value("NumPeels", defaultRendererMaximalNumberOfPeels).toInt());
	

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
	mqMorphoDigMenuBuilders::buildTagsMenu(*this->menuTags);
	mqMorphoDigMenuBuilders::buildEditVolumesMenu(*this->menuVolumes);
	mqMorphoDigMenuBuilders::buildScalarsMenu(*this->menuScalars);
	mqMorphoDigMenuBuilders::buildRGBMenu(*this->menuRGB);
	mqMorphoDigMenuBuilders::buildEditMenu(*this->menuEdit);
	mqMorphoDigMenuBuilders::buildLandmarksMenu(*this->menuLandmarks);
	mqMorphoDigMenuBuilders::buildHelpMenu(*this->menuHelp);
	mqMorphoDigMenuBuilders::buildToolbars(*this);
	mqMorphoDigMenuBuilders::buildStatusBar(*this);
	mqMorphoDigMenuBuilders::buildProjectDocks(*projectTabWindow);
	cout << "OpenGL version:" << this->MorphoDigCore->GetOpenGLVersion().toStdString() << endl;
	cout << "About to build view Menu!" << endl;
	mqMorphoDigMenuBuilders::buildViewMenu(*this->menuView, *this, *projectTabWindow);
	cout << "View Menu built!" << endl;
	

	double myorigin[3];
	myorigin[0] = 0;
	myorigin[1] = 0;
	myorigin[2] = 0;

	this->MorphoDigCore->getGridActor()->SetOutlineMode(this->MorphoDigCore->Getmui_CameraCentreOfMassAtOrigin());
	


	//@@ rubber band selection!
	
	 vtkSmartPointer<vtkMDInteractorStyle> style =
    vtkSmartPointer<vtkMDInteractorStyle>::New();

	 /*vtkSmartPointer<vtkMDLassoInteractorStyle> lassostyle =
		 vtkSmartPointer<vtkMDLassoInteractorStyle>::New();*/

	 vtkSmartPointer<vtkInteractorStyleDrawPolygon> lassostyle =
		 vtkSmartPointer<vtkInteractorStyleDrawPolygon>::New();

	 vtkSmartPointer<vtkInteractorStyleRubberBand3D> rubberstyle =
		 vtkSmartPointer<vtkInteractorStyleRubberBand3D>::New();

	 style->SetActorCollection(this->MorphoDigCore->getActorCollection());
	 style->SetVolumeCollection(this->MorphoDigCore->getVolumeCollection());
	 style->SetNormalLandmarkCollection(this->MorphoDigCore->getNormalLandmarkCollection());
	 style->SetTargetLandmarkCollection(this->MorphoDigCore->getTargetLandmarkCollection());
	 style->SetNodeLandmarkCollection(this->MorphoDigCore->getNodeLandmarkCollection());
	 style->SetHandleLandmarkCollection(this->MorphoDigCore->getHandleLandmarkCollection());
	 style->SetFlagLandmarkCollection(this->MorphoDigCore->getFlagLandmarkCollection());

	/* lassostyle->SetActorCollection(this->MorphoDigCore->getActorCollection());
	 lassostyle->SetNormalLandmarkCollection(this->MorphoDigCore->getNormalLandmarkCollection());
	 lassostyle->SetTargetLandmarkCollection(this->MorphoDigCore->getTargetLandmarkCollection());
	 lassostyle->SetNodeLandmarkCollection(this->MorphoDigCore->getNodeLandmarkCollection());
	 lassostyle->SetHandleLandmarkCollection(this->MorphoDigCore->getHandleLandmarkCollection());
	 lassostyle->SetFlagLandmarkCollection(this->MorphoDigCore->getFlagLandmarkCollection());*/

	//vtkSmartPointer<vtkInteractorStyleTrackballCamera> style2 =		vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New(); //like paraview
	//vtkSmartPointer<vtkInteractorStyleTrackballActor> style =
	//	vtkSmartPointer<vtkInteractorStyleTrackballActor>::New();
	//vtkSmartPointer<vtkInteractorStyleSwitch> style =
	//	vtkSmartPointer<vtkInteractorStyleSwitch>::New();

	 vtkSmartPointer<vtkCallbackCommand> lassoselectionCallback =
		 vtkSmartPointer<vtkCallbackCommand>::New();
	 lassoselectionCallback->SetCallback(lassoselect);
	 lassostyle->AddObserver(vtkCommand::SelectionChangedEvent, lassoselectionCallback);

	 vtkSmartPointer<vtkCallbackCommand> rubberselectionCallback =
		 vtkSmartPointer<vtkCallbackCommand>::New();
	 rubberselectionCallback->SetCallback(rubberselect);
	 rubberstyle->AddObserver(vtkCommand::SelectionChangedEvent, rubberselectionCallback);

	 vtkSmartPointer<vtkCallbackCommand> pickCallback =
		 vtkSmartPointer<vtkCallbackCommand>::New();

	 pickCallback->SetCallback(RubberBandSelect);
	 this->AreaPicker =
		 vtkSmartPointer<vtkAreaPicker>::New();

		//  this->AreaPicker =
		// vtkSmartPointer<vtkRenderedAreaPicker>::New();
	 this->AreaPicker->AddObserver(vtkCommand::EndPickEvent, pickCallback);
	
 //style2->SetCurrentRenderer(this->MorphoDigCore->getRenderer());
  //this->qvtkWidget2->GetRenderWindow()->GetInteractor()->SetPicker(this->AreaPicker);
  //this->qvtkWidget2->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);
  
 //same thing !!!!
  window->GetInteractor()->SetPicker(this->AreaPicker);
  window->GetInteractor()->SetInteractorStyle(style);
  cout << "Set interaction styles (normal style, lassostyle and rubberband style)" << endl;
  // mqMorphoDigCore should be aware of the 2 coexisting interactor styles (so that we can switch between them
  mqMorphoDigCore::instance()->SetNormalInteractorStyle(style);
  mqMorphoDigCore::instance()->SetLassoInteractorStyle(lassostyle);
  mqMorphoDigCore::instance()->SetRubberInteractorStyle(rubberstyle);

  cout << "Bezier stuff" << endl;
  mqMorphoDigCore::instance()->getBezierCurveSource()->SetHandles(mqMorphoDigCore::instance()->getHandleLandmarkCollection());
  mqMorphoDigCore::instance()->getBezierCurveSource()->SetNodes(mqMorphoDigCore::instance()->getNodeLandmarkCollection());
  
  

  mqMorphoDigCore::instance()->getBezierCurveSource()->Update();

  cout << "Callbacks for node and handle landmarks" << endl;
  vtkSmartPointer<vtkMyNodeHandleCallBack> callback = vtkSmartPointer<vtkMyNodeHandleCallBack>::New();
  mqMorphoDigCore::instance()->getNodeLandmarkCollection()->AddObserver(vtkCommand::ModifiedEvent, callback);
  mqMorphoDigCore::instance()->getHandleLandmarkCollection()->AddObserver(vtkCommand::ModifiedEvent, callback);

  cout << "Adds Bezier NH actor, selected and normals actor" << endl;
  this->MorphoDigCore->getRenderer()->AddActor(mqMorphoDigCore::instance()->getBezierNHActor());
  this->MorphoDigCore->getRenderer()->AddActor(mqMorphoDigCore::instance()->getBezierSelectedActor());
  this->MorphoDigCore->getRenderer()->AddActor(mqMorphoDigCore::instance()->getBezierActor());

  cout << "Grid visibility, infos and orientation helper..." << endl;
  this->MorphoDigCore->SetGridVisibility();
  this->MorphoDigCore->SetGridInfos();
  this->MorphoDigCore->InitializeOrientationHelper(); // creates orientation helper...
  this->MorphoDigCore->SetOrientationHelperVisibility();
  //this->qvtkWidget2->SetRenderWindow(window);
  
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


  this->_boxWidget = vtkSmartPointer<vtkBoxWidget>::New();
  //
  this->_boxWidget->SetInteractor(this->qvtkWidget2->GetRenderWindow()->GetInteractor());
  //boxWidget->SetInteractor(this->ui->qvtkWidget->GetRenderWindow()->GetInteractor());
  this->_boxWidget->SetProp3D(actor);
  this->_boxWidget->SetPlaceFactor(1.25);
  this->_boxWidget->PlaceWidget();
  vtkSmartPointer<vtkMyCallback> Boxcallback = vtkSmartPointer<vtkMyCallback>::New();
  this->_boxWidget->AddObserver(vtkCommand::InteractionEvent, Boxcallback);
  this->_boxWidget->On();


  */
  

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
	  
  widget->SetInteractor(this->qvtkWidget2->GetRenderWindow()->GetInteractor());
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
  cout << "Accept Drops..." << endl;
  setAcceptDrops(true);
  cout << "OpenGL version:" << this->MorphoDigCore->GetOpenGLVersion().toStdString() << endl;
  //Activate this only for test purposes...
  //this->MorphoDigCore->TestVolume();

  
}




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
void MorphoDig::dragEnterEvent(QDragEnterEvent *e)
{
	if (e->mimeData()->hasUrls()) {
		e->acceptProposedAction();
	}
}
void MorphoDig::dropEvent(QDropEvent *e)
{
	cout << "drop event" << endl;
	int tiff_3D = 1;
	int cpt_png = 0;
	foreach(const QUrl &url, e->mimeData()->urls()) {
		QString fileName = url.toLocalFile();
		std::string PNGext(".png");
		std::string PNGext2(".PNG");
		
		std::size_t found = fileName.toStdString().find(PNGext);
		std::size_t found2 = fileName.toStdString().find(PNGext2);
		if (found != std::string::npos || found2 != std::string::npos )
		{
			cpt_png++;
			
			tiff_3D = 0;
		}

	}
	int cpt_bmp = 0;
	foreach(const QUrl &url, e->mimeData()->urls()) {
		QString fileName = url.toLocalFile();
		std::string BMPext(".bmp");
		std::string BMPext2(".BMP");

		std::size_t found = fileName.toStdString().find(BMPext);
		std::size_t found2 = fileName.toStdString().find(BMPext2);
		if (found != std::string::npos || found2 != std::string::npos)
		{
			cpt_bmp++;

			tiff_3D = 0;
		}

	}
	int cpt_tiff = 0;
	
	

	foreach(const QUrl &url, e->mimeData()->urls()) {
		QString fileName = url.toLocalFile();
		std::string TIFext(".tif");
		std::string TIFext2(".TIF");
		std::string TIFext3(".tiff");
		std::string TIFext4(".TIFF");
		std::size_t found = fileName.toStdString().find(TIFext);
		std::size_t found2 = fileName.toStdString().find(TIFext2);
		std::size_t found3 = fileName.toStdString().find(TIFext3);
		std::size_t found4 = fileName.toStdString().find(TIFext4);
		if (found != std::string::npos || found2 != std::string::npos || found3 != std::string::npos || found4 != std::string::npos)
		{
			cpt_tiff++;
			//Tif TIFF

		}

	}
	if (cpt_tiff > 1)
	{
		tiff_3D = 0;
		cout << "Should not open 3D tiff!" << endl;


	}
	int cpt_dcm = 0;
	foreach(const QUrl &url, e->mimeData()->urls()) {
		QString fileName = url.toLocalFile();
		std::string DCMext(".dcm");
		std::string DCMext2(".DCM");
		std::string DCMext3(".ima");
		std::string DCMext4(".IMA");
		std::size_t found = fileName.toStdString().find(DCMext);
		std::size_t found2 = fileName.toStdString().find(DCMext2);
		std::size_t found3 = fileName.toStdString().find(DCMext3);
		std::size_t found4 = fileName.toStdString().find(DCMext4);
		if (found != std::string::npos || found2 != std::string::npos || found3 != std::string::npos || found4 != std::string::npos)
		{
			cpt_dcm++;
			//dcm

		}

	}
	
	foreach(const QUrl &url, e->mimeData()->urls()) {
		QString fileName = url.toLocalFile();
		//this->MorphoDigCore->Open
		cout << "Dropped file:" << fileName.toStdString() << endl;
		std::string STLext(".stl");
		std::string STLext2(".STL");
		std::string VTKext(".vtk");
		std::string VTKext2(".VTK");
		std::string VTKext3(".vtp");
		std::string VTKext4(".VTP");
		std::string PLYext(".ply");
		std::string PLYext2(".PLY");
		std::string OBJext(".obj");
		std::string OBJext2(".OBJ");
		std::string NTWext(".ntw");
		std::string NTWext2(".NTW");
		std::string VERext(".ver");
		std::string VERext2(".VER");
		std::string CURext(".cur");
		std::string CURext2(".CUR");
		std::string FLGext(".flg");
		std::string FLGext2(".FLG");
		std::string LMKext(".lmk");
		std::string LMKext2(".LMK");		
		std::string TAGext(".tag");
		std::string TAGext2(".TAG");
		std::string TAGext3(".tgp");
		std::string TAGext4(".TGP");
		std::string STVext(".stv");
		std::string STVext2(".STV");
		std::string ORIext(".ori");
		std::string ORIext2(".ORI");
		std::string POSext(".pos");
		std::string POSext2(".POS");
		std::string MHAext(".mha");		
		std::string MHAext2(".MHA");
		std::string MHDext(".mhd");
		std::string MHDext2(".MHD");
		std::string VTIext(".vti");
		std::string VTIext2(".VTI");
		std::string TPSext(".tps");
		std::string TPSext2(".TPS");
		std::string PTSext(".pts");
		std::string PTSext2(".PTS");
		std::string RAWext(".raw");
		std::string RAWext2(".RAW");
		std::string TIFext(".tif");
		std::string TIFext2(".TIF");
		std::string TIFext3(".tiff");
		std::string TIFext4(".TIFF");
		std::string BMPext(".bmp");
		std::string BMPext2(".BMP");
		std::string PNGext(".png");
		std::string PNGext2(".PNG");

		std::string DCMext(".dcm");
		std::string DCMext2(".DCM");
		std::string DCMext3(".ima");
		std::string DCMext4(".IMA");

		int type = 0; //0 = stl, 1 = vtk,  2 = ply, 3 = ntw, 4 ver, 5 cur, 6 flg, 7 lmk, 8 tag, 9 stv, 10 ori, 11 pos
		std::size_t found = fileName.toStdString().find(STLext);
		std::size_t found2 = fileName.toStdString().find(STLext2);
		if (found != std::string::npos || found2 != std::string::npos)
		{
			type = 0;
			//STL
		}

		found = fileName.toStdString().find(VTKext);
		found2 = fileName.toStdString().find(VTKext2);
		if (found != std::string::npos || found2 != std::string::npos)
		{
			type = 1; //VTK
		}
		std::size_t found3 = fileName.toStdString().find(VTKext3);
		std::size_t found4 = fileName.toStdString().find(VTKext4);
		if (found3 != std::string::npos || found4 != std::string::npos)
		{
			type = 1; //VTP
		}
		//std::cout << "2Type= " <<type<< std::endl;
		found = fileName.toStdString().find(PLYext);
		found2 = fileName.toStdString().find(PLYext2);

		if (found != std::string::npos || found2 != std::string::npos)
		{
			type = 2; //PLY
		}

		//std::cout << "2Type= " <<type<< std::endl;
		found = fileName.toStdString().find(OBJext);
		found2 = fileName.toStdString().find(OBJext2);

		if (found != std::string::npos || found2 != std::string::npos)
		{
			type = 3; //OBJ
		}
		//0 = stl, 1 = vtk,  2 = ply, 3 = obj, 4 = ntw, 5 ver, 6 cur, 7 flg, 8 lmk
		found = fileName.toStdString().find(NTWext);
		found2 = fileName.toStdString().find(NTWext2);
		if (found != std::string::npos || found2 != std::string::npos)
		{
			type = 4; //NTW
		}

		//4 ver, 5 cur, 6 flg, 7 lmk
		found = fileName.toStdString().find(VERext);
		found2 = fileName.toStdString().find(VERext2);
		if (found != std::string::npos || found2 != std::string::npos)
		{
			type = 5; //VER
		}

		found = fileName.toStdString().find(CURext);
		found2 = fileName.toStdString().find(CURext2);
		if (found != std::string::npos || found2 != std::string::npos)
		{
			type = 6; //CUR
		}
		found = fileName.toStdString().find(FLGext);
		found2 = fileName.toStdString().find(FLGext2);
		if (found != std::string::npos || found2 != std::string::npos)
		{
			type = 7; //FLG
		}
		found = fileName.toStdString().find(LMKext);
		found2 = fileName.toStdString().find(LMKext2);
		if (found != std::string::npos || found2 != std::string::npos)
		{
			type = 8; //LMK
		}
		found = fileName.toStdString().find(TAGext);
		found2 = fileName.toStdString().find(TAGext2);
		found3 = fileName.toStdString().find(TAGext3);
		found4 = fileName.toStdString().find(TAGext4);
		if (found != std::string::npos || found2 != std::string::npos || found3 != std::string::npos || found4 != std::string::npos)		
		{
			type = 9; //TAG
		}
		found = fileName.toStdString().find(STVext);
		found2 = fileName.toStdString().find(STVext2);
		if (found != std::string::npos || found2 != std::string::npos)
		{
			type = 10; //STV
		}
		//8 tag, 9 stv, 10 ori, 11 pos
		found = fileName.toStdString().find(ORIext);
		found2 = fileName.toStdString().find(ORIext2);
		if (found != std::string::npos || found2 != std::string::npos)
		{
			type = 11; //ORI
		}
		found = fileName.toStdString().find(POSext);
		found2 = fileName.toStdString().find(POSext2);
		if (found != std::string::npos || found2 != std::string::npos)
		{
			type = 12; //POS
		}
		found = fileName.toStdString().find(MHAext);
		found2 = fileName.toStdString().find(MHAext2);
		if (found != std::string::npos || found2 != std::string::npos)
		{
			type = 13; //MHA MHD VTI
		}
		found = fileName.toStdString().find(MHDext);
		found2 = fileName.toStdString().find(MHDext2);
		if (found != std::string::npos || found2 != std::string::npos)
		{
			type = 13; //MHA MHD VTI
		}
		found = fileName.toStdString().find(VTIext);
		found2 = fileName.toStdString().find(VTIext2);
		if (found != std::string::npos || found2 != std::string::npos)
		{
			type = 13; //MHA MHD VTI
		}

		found = fileName.toStdString().find(PTSext);
		found2 = fileName.toStdString().find(PTSext2);
		if (found != std::string::npos || found2 != std::string::npos)
		{
			type = 14; //PTS
		}
		found = fileName.toStdString().find(TPSext);
		found2 = fileName.toStdString().find(TPSext2);
		if (found != std::string::npos || found2 != std::string::npos)
		{
			type = 15; //TPS
		}
		cout << "Test if raw:"  << endl;

		found = fileName.toStdString().find(RAWext);
		found2 = fileName.toStdString().find(RAWext2);
		if (found != std::string::npos || found2 != std::string::npos)
		{
			cout << "RAW" << endl;
			type = 16; //RAW
		}
		found = fileName.toStdString().find(TIFext);
		found2 = fileName.toStdString().find(TIFext2);
		found3 = fileName.toStdString().find(TIFext3);
		found4 = fileName.toStdString().find(TIFext4);
		std::size_t found5 = fileName.toStdString().find(BMPext);
		std::size_t found6 = fileName.toStdString().find(BMPext2);
		std::size_t found7 = fileName.toStdString().find(PNGext);
		std::size_t found8 = fileName.toStdString().find(PNGext2);
		if (found != std::string::npos || found2 != std::string::npos || found3 != std::string::npos || found4 != std::string::npos
			|| found5 != std::string::npos || found6 != std::string::npos || found7 != std::string::npos || found8 != std::string::npos
			)
		{
			type = 17;
			
			
		}
		found = fileName.toStdString().find(DCMext);
		found2 = fileName.toStdString().find(DCMext2);
		found3 = fileName.toStdString().find(DCMext3);
		found4 = fileName.toStdString().find(DCMext4);
		if (found != std::string::npos || found2 != std::string::npos || found3 != std::string::npos || found4 != std::string::npos)
		{
			type = 18;


		}
		if (type < 4)
		{
			int ok = mqMorphoDigCore::instance()->OpenMesh(fileName);
			if (ok ==1){ mqMorphoDigCore::instance()->CreateSurfaceUndoSet(1); }
			
		}
		else if (type == 4)
		{
			mqMorphoDigCore::instance()->OpenNTW(fileName);
		}
		else if (type == 5)
		{
			mqMorphoDigCore::instance()->OpenVER(fileName, 0);
		}
		else if (type == 6)
		{
			mqMorphoDigCore::instance()->OpenCUR(fileName);
		}
		else if (type == 7)
		{
			mqMorphoDigCore::instance()->OpenFLG(fileName);
		}
		else if (type == 8)
		{
			mqMorphoDigCore::instance()->OpenLMK(fileName, 0);
		}
		else if (type == 9)
		{
			mqMorphoDigCore::instance()->OpenTAGMAP(fileName);
		}
		else if (type == 10)
		{
			mqMorphoDigCore::instance()->OpenSTV(fileName);
		}
		else if (type == 11)
		{
			mqMorphoDigCore::instance()->OpenORI(fileName);
		}
		else if (type == 12)
		{
			mqMorphoDigCore::instance()->OpenPOS(fileName, 1);
		}
		else if (type == 13)
		{
			mqMorphoDigCore::instance()->OpenVolume(fileName);
		}
		else if (type == 14)
		{
			mqMorphoDigCore::instance()->OpenPTS(fileName, 0);
		}
		else if (type == 15)
		{
			mqMorphoDigCore::instance()->OpenTPS(fileName, 0);
		}
		else if (type == 16)
		{
			cout << "Drag and drop raw" << endl;
			mqOpenRawDialog OpenRaw_dialog(mqCoreUtilities::mainWidget());
			OpenRaw_dialog.setFileName(fileName);
			OpenRaw_dialog.exec(); 
		}
		else if (type == 17)
		{
			if (tiff_3D == 1)
			{

				mqOpenTiff3DDialog OpenTiff3D_dialog(mqCoreUtilities::mainWidget());
				OpenTiff3D_dialog.setFileName(fileName);
				OpenTiff3D_dialog.exec();
			}
			else
			{
				//do nothing!
			}
		}
		else if (type == 18)
		{
			
				//do nothing!
			
		}

	}
	// now the 2D tiff case
	cout << "here!!!!" << endl;
	if (cpt_tiff > 1 || cpt_png> 1 || cpt_bmp > 1)
	{
		int first_image = 1;
		int found_3Dtiff = 0;
		int wrong_dims_msg = 0;
		int dimX = 0;
		int dimY = 0;
		int dimZ = 0;
		//QString stackName = "";
		QString firstFileName = "";
		vtkSmartPointer<vtkImageAppend> imageAppend = vtkSmartPointer<vtkImageAppend>::New();
		imageAppend->SetAppendAxis(2);

		foreach(const QUrl &url, e->mimeData()->urls()) {
			QString fileName = url.toLocalFile();
			std::string TIFext(".tif");
			std::string TIFext2(".TIF");
			std::string TIFext3(".tiff");
			std::string TIFext4(".TIFF");
			std::string BMPext(".bmp");
			std::string BMPext2(".BMP");
			std::string PNGext(".png");
			std::string PNGext2(".PNG");

			std::size_t found = fileName.toStdString().find(TIFext);
			std::size_t found2 = fileName.toStdString().find(TIFext2);
			std::size_t found3 = fileName.toStdString().find(TIFext3);
			std::size_t found4 = fileName.toStdString().find(TIFext4);
			std::size_t found5 = fileName.toStdString().find(BMPext);
			std::size_t found6 = fileName.toStdString().find(BMPext2);
			std::size_t found7 = fileName.toStdString().find(PNGext);
			std::size_t found8 = fileName.toStdString().find(PNGext2);
			int file_format = 0; // tiff
			if (found5 != std::string::npos || found6 != std::string::npos)
			{ 
				file_format = 1; //bmp

			}
			if (found7 != std::string::npos || found8 != std::string::npos)
			{
				file_format = 2; //png

			}

			if (found != std::string::npos || found2 != std::string::npos || found3 != std::string::npos || found4 != std::string::npos 
				|| found5 != std::string::npos || found6 != std::string::npos || found7 != std::string::npos || found8 != std::string::npos)
			{

				QFile file(fileName);
				QString name = "";
				if (file.exists()) {
					dimZ++;

					name = file.fileName(); // Return only a file name		
					vtkSmartPointer<vtkImageData> input = vtkSmartPointer<vtkImageData>::New();
					if (file_format == 0)
					{
						vtkSmartPointer <vtkTIFFReader> tiffReader = vtkSmartPointer<vtkTIFFReader>::New();
						tiffReader->SetFileName(fileName.toLocal8Bit());
						//tiffReader->GetF
						tiffReader->Update();
						input = tiffReader->GetOutput();
					}
					else if (file_format == 1)
					{
						vtkSmartPointer <vtkBMPReader> bmpReader = vtkSmartPointer<vtkBMPReader>::New();
						bmpReader->SetAllow8BitBMP(true);
						bmpReader->SetFileName(fileName.toLocal8Bit());
						//tiffReader->GetF
						bmpReader->Update();
						input = bmpReader->GetOutput();
					}
					else
					{
						vtkSmartPointer <vtkPNGReader> pngReader = vtkSmartPointer<vtkPNGReader>::New();
						pngReader->SetFileName(fileName.toLocal8Bit());
						//tiffReader->GetF
						pngReader->Update();
						input = pngReader->GetOutput();
					}
					int dim[3];
					input->GetDimensions(dim);

					if (dim[2] != 1)
					{
						found_3Dtiff = 1;
						//mettre un message : pas possible de mettre un 3D tiff 
						QMessageBox msgBox;
						msgBox.setText("Error: please only drag and drop several 2D files or one single 3D TIFF volume. Do not mix 2D and 3D TIFF files.");
						msgBox.exec();
						return;
					}
					if (first_image == 1)
					{
						firstFileName = fileName;
						// now try to set dimX and dimY based on the first image.																							
						cout << "First image dimensions: " << dim[0] << "," << dim[1] << "," << dim[2] << endl;

						dimX = dim[0];
						dimY = dim[1];
						imageAppend->AddInputData(input);
						first_image = 0;
					}
					else
					{
					if (dimX != dim[0] || dimY != dim[1])
					{
						cout << "found an image of wrong dimensions" << endl;
						if (wrong_dims_msg == 0)
						{
							wrong_dims_msg = 1;
							QMessageBox msgBox;
							QString msg = "At lease one 2D image differs in dimensions from those of the first opened image(" + QString(dimX) + "," + QString(dimY) + "). These files will be ignored.";
							msgBox.setText(msg);
							msgBox.exec();
						}

					}
					else
					{
						imageAppend->AddInputData(input);
					}

					}
					file.close();
				}
				//Tif TIFF

			}

		}// end foreach
		imageAppend->Update();
		mqOpenTiff3DDialog OpenTiff3D_dialog(mqCoreUtilities::mainWidget());
		OpenTiff3D_dialog.setInputAsStack();
		OpenTiff3D_dialog.set2DStackInput(imageAppend->GetOutput());
		int dim[3];
		imageAppend->GetOutput()->GetDimensions(dim);
		OpenTiff3D_dialog.setDimensions(dim[0], dim[1], dim[2]);
		OpenTiff3D_dialog.setDataType(imageAppend->GetOutput()->GetScalarType());
		OpenTiff3D_dialog.setFileName(firstFileName);
		OpenTiff3D_dialog.exec();
	}

	// now the DICOM case

	if (cpt_dcm > 1)
	{
		//spacingZ est obtenu en ouvrant au moins 2 coupes. Soit ça marche avec getOrigin... sinon il faut arriver à ouvrir 2 coupes d'un coup!
		int first_image = 1;
		int second_image = 1;
		float origin_first[3];
		float origin_second[3];
			
		int found_3DDCM = 0;
		int wrong_dims_msg = 0;
		int dimX = 0;
		int dimY = 0;
		int dimZ = 0;
		double spacingX = 1;
		double spacingY = 1;
		double spacingZ = 1;
		//QString stackName = "";
		QString firstFileName = "";
		QString patientName = "";
		int frontToBack = 1;
		vtkSmartPointer<vtkImageAppend> imageAppend = vtkSmartPointer<vtkImageAppend>::New();
		imageAppend->SetAppendAxis(2);

		foreach(const QUrl &url, e->mimeData()->urls()) {
			QString fileName = url.toLocalFile();
			std::string DCMext(".dcm");
			std::string DCMext2(".DCM");
			std::string DCMext3(".ima");
			std::string DCMext4(".IMA");

			std::size_t found = fileName.toStdString().find(DCMext);
			std::size_t found2 = fileName.toStdString().find(DCMext2);
			std::size_t found3 = fileName.toStdString().find(DCMext3);
			std::size_t found4 = fileName.toStdString().find(DCMext4);

			if (found != std::string::npos || found2 != std::string::npos || found3 != std::string::npos || found4 != std::string::npos)
			{

				QFile file(fileName);
				QString name = "";
				if (file.exists()) {
					dimZ++;

					name = file.fileName(); // Return only a file name		
					vtkSmartPointer<vtkImageData> input = vtkSmartPointer<vtkImageData>::New();
					vtkSmartPointer <vtkDICOMImageReader> dcmReader = vtkSmartPointer<vtkDICOMImageReader>::New();
					dcmReader->SetFileName(fileName.toLocal8Bit());
					//tiffReader->GetF
					dcmReader->Update();
					input = dcmReader->GetOutput();
					int dim[3];
					double spacing[3];
					input->GetDimensions(dim);
					
					input->GetSpacing(spacing);

					if (dim[2] != 1 && dimZ > 1)
					{
						found_3DDCM = 1;
						//mettre un message : pas possible de mettre un 3D tiff 
						QMessageBox msgBox;
						msgBox.setText("Error: please only drag and drop several 2D DICOM files or one single 3D DICOM volume. Do not mix 2D and 3D DICOM files.");
						msgBox.exec();
						return;
					}
					else if (dim[2] == 1 && found_3DDCM == 1)
					{
						QMessageBox msgBox;
						msgBox.setText("Error: please only drag and drop several 2D DICOM files or one single 3D DICOM volume. Do not mix 2D and 3D DICOM files.");
						msgBox.exec();
						return;
					}
					if (first_image == 1)
					{
						patientName = dcmReader->GetPatientName();
						firstFileName = fileName;
						// now try to set dimX and dimY based on the first image.																							
						cout << "First image dimensions: " << dim[0] << "," << dim[1] << "," << dim[2] <<","<<spacing[0]<<","<<spacing[1]<<","<<spacing[2]<< endl;
						float *pos;
						pos = dcmReader->GetImagePositionPatient();
						origin_first[0] = pos[0];
						origin_first[1] = pos[1];
						origin_first[2] = pos[2];
						cout << "First image origin" << origin_first[0] << "," << origin_first[1] << "," << origin_first[2] << endl;
						dimX = dim[0];
						dimY = dim[1];
						spacingX = spacing[0];
						spacingY = spacing[1];
						//spacingZ = spacing[2];
						
						imageAppend->AddInputData(input);
						first_image = 0;
					}					
					else
					{
						if (first_image ==0&& second_image ==1){
							second_image = 0;
							//input->GetOrigin(origin_second);
							float *pos;
							pos = dcmReader->GetImagePositionPatient();
							origin_second[0] = pos[0];
							origin_second[1] = pos[1];
							origin_second[2] = pos[2];
							cout << "Second image origin" << origin_second[0] << "," << origin_second[1] << "," << origin_second[2] << endl;
							double diff = (double)(origin_first[2] - origin_second[2]);
							if (diff < 0) { frontToBack = 1; }
							else { frontToBack = 0; }
							spacingZ = abs(diff);


						}
						if (dimX != dim[0] || dimY != dim[1] 
							//||spacingX != spacing[0] || spacingY != spacing[1] || spacingZ != spacing[2]
							)
						{
							cout << "found an image of wrong dimensions" << endl;
							if (wrong_dims_msg == 0)
							{
								wrong_dims_msg = 1;
								QMessageBox msgBox;
								QString msg = "At lease one DICOM image differs in dimensions or voxel size from those of the first opened DICOM image(" + QString(dimX) + "," + QString(dimY) + "). These files will be ignored.";
								msgBox.setText(msg);
								msgBox.exec();
							}

						}
						else
						{
							imageAppend->AddInputData(input);
						}

					}
					file.close();
				}
				//Tif TIFF

			}

		}// end foreach
		imageAppend->Update();
		mqOpenDicomStackDialog OpenDicomStack_dialog(mqCoreUtilities::mainWidget());
		OpenDicomStack_dialog.setInputAsStack();
		OpenDicomStack_dialog.set2DStackInput(imageAppend->GetOutput());
		int dim[3];
		imageAppend->GetOutput()->GetDimensions(dim);
		
		OpenDicomStack_dialog.setDimensions(dim[0], dim[1], dim[2]);
		OpenDicomStack_dialog.setSpacing(spacingX, spacingY, spacingZ);
		cout << "frontToBack=" << frontToBack << endl;
		OpenDicomStack_dialog.setFrontToBack(frontToBack);
		OpenDicomStack_dialog.setDataType(imageAppend->GetOutput()->GetScalarType());
		OpenDicomStack_dialog.setFileName(firstFileName);
		OpenDicomStack_dialog.setPatientName(patientName);
		OpenDicomStack_dialog.exec();
	}
}


void MorphoDig::saveSettings()
{
	
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "MorphoMuseuM", "MorphoDig");
	//cout<<"try save settings:" << m_sSettingsFile.toStdString();

	settings.beginGroup("paths");
	settings.setValue("LastUsedDir", this->MorphoDigCore->Getmui_LastUsedDir());
	settings.endGroup();
	settings.beginGroup("interaction_mode");
	settings.setValue("MoveMode", this->MorphoDigCore->Getmui_MoveMode());
	settings.endGroup();
	settings.beginGroup("display_options");
	settings.setValue("ShowGrid", this->MorphoDigCore->Getmui_ShowGrid());
	settings.setValue("ScalarVisibility", this->MorphoDigCore->Getmui_ArrayVisibility());
	
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
	settings.setValue("Ambient", this->MorphoDigCore->Getmui_Ambient());
	settings.setValue("Diffuse", this->MorphoDigCore->Getmui_Diffuse());
	settings.setValue("Specular", this->MorphoDigCore->Getmui_Specular());
	settings.setValue("SpecularPower", this->MorphoDigCore->Getmui_SpecularPower());

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
	settings.setValue("FontSize", this->MorphoDigCore->Getmui_FontSize());

	settings.endGroup();
	
	
	settings.beginGroup("curve_settings");
	settings.setValue("SegmentDecimation", this->MorphoDigCore->Getmui_SegmentDecimation());
	size_t numsegs = this->MorphoDigCore->Getmui_SegmentDecimations()->decimation.size();
	settings.setValue("nsegments", numsegs);
	
	
	
	for (int i = 0; i < numsegs; i++)
	{
		int segdecimation = this->MorphoDigCore->Getmui_SegmentDecimations()->decimation.at(i);
		
		//2 set the name of colormap(cpt), and whether it's discretizable and opacity is enabled
		QString d = QString("d");
			d = d + QString::number(i);
			settings.setValue(d, segdecimation);
		
	}
	settings.endGroup();

	
	settings.beginGroup("renderer_settings");
	settings.setValue("Anaglyph", this->MorphoDigCore->Getmui_Anaglyph());	
	settings.setValue("DisplayMode", this->MorphoDigCore->Getmui_DisplayMode());
	settings.setValue("OcclusionRatio", this->MorphoDigCore->Getmui_RendererOcclusionRatio());
	settings.setValue("NumPeels", this->MorphoDigCore->Getmui_RendererMaximalNumberOfPeels());
	
	settings.endGroup();

	settings.beginGroup("volumes");
	settings.setValue("VolumeOutOfCoreThreshold", this->MorphoDigCore->Getmui_VolumeOutOfCoreThreshold());
	settings.setValue("VolumeDisplaySlice", this->MorphoDigCore->Getmui_VolumeDisplaySlice());
	settings.endGroup();

	settings.beginGroup("colormaps");

	
	ExistingColorMaps *colorMaps = this->MorphoDigCore->Getmui_ExistingColorMaps();
	size_t size = colorMaps->Stack.size();
	int cpt = 0;
	int nr = 0;
	for (int i = 0; i < size; i++)
	{
		if (colorMaps->Stack.at(i).isCustom == 1)
		{
			nr++;
		}
	}
	//1 total number of custom colormaps
	settings.setValue("nr", nr);
	for (int i = 0; i < size; i++)
	{
		if (colorMaps->Stack.at(i).isCustom == 1)
		{
			//2 set the name of colormap(cpt), and whether it's discretizable and opacity is enabled
			QString cm = QString("cm");
			cm = cm + QString::number(cpt);
			settings.setValue(cm, colorMaps->Stack.at(i).Name);
			
			QString cmdiscretize = cm + "discretize";
			QString cmdiscretizenr = cm + "discretizenr";
			QString cmenableopacity = cm + "enableopacity";
			int discretize = colorMaps->Stack.at(i).ColorMap->GetDiscretize();
			int discretizenr = colorMaps->Stack.at(i).ColorMap->GetNumberOfValues();
			int enableopacity = colorMaps->Stack.at(i).ColorMap->GetEnableOpacityMapping();
			settings.setValue(cmdiscretize, discretize);
			settings.setValue(cmdiscretizenr, discretizenr);
			settings.setValue(cmenableopacity, enableopacity);

			//3 set numer of colors
			QString cmnc = cm + "nc"; 
			int nc = colorMaps->Stack.at(i).ColorMap->GetSize();
			settings.setValue(cmnc, nc);

			//4 set number of opacity nodes
			int no = colorMaps->Stack.at(i).ColorMap->GetScalarOpacityFunction()->GetSize();
			QString cmno = cm + "no";
			settings.setValue(cmno, no);
			double *pts = colorMaps->Stack.at(i).ColorMap->GetDataPointer();
			//first : find max and min cx to reset cx range between 0 and 1
			double cx_min = DBL_MAX;
			double cx_max = -DBL_MAX;
			for (int j = 0; j < nc; j++)
			{
				double curr = pts[4 * j];
				//cout << "x" << j << "=" << curr << endl;
				if (curr < cx_min) { cx_min = curr; }
				if (curr > cx_max) { cx_max = curr; }

			}
			double c = 0;
			double mult = 1;
			if (cx_max > cx_min)
			{
				double range = cx_max - cx_min;				
				mult = 1/range;
				c = -cx_min/range;

			}

			for (int j = 0; j < nc; j++)
			{
				double cx = mult*pts[4 * j]+c;				
				double r = pts[4 * j + 1];
				double g = pts[4 * j + 2];
				double b = pts[4 * j + 3];
				QString cmcx = cm + "cx" + QString::number(j);
				QString cmr = cm + "r" + QString::number(j);
				QString cmg = cm + "g" + QString::number(j);
				QString cmb = cm + "b" + QString::number(j);
				settings.setValue(cmcx, cx);
				settings.setValue(cmr, r);
				settings.setValue(cmg, g);
				settings.setValue(cmb, b);
			}
			double *pts2 = colorMaps->Stack.at(i).ColorMap->GetScalarOpacityFunction()->GetDataPointer();

			double ox_min = DBL_MAX;
			double ox_max = -DBL_MAX;
			for (int j = 0; j < no; j++)
			{
				double curr = pts2[2 * j];
				//cout << "x" << j << "=" << curr << endl;
				if (curr < ox_min) { ox_min = curr; }
				if (curr > ox_max) { ox_max = curr; }

			}

			 c = 0;
			 mult = 1;
			if (ox_max > ox_min)
			{
				double range = ox_max - ox_min;
				mult = 1/range;
				c = -ox_min/range;

			}

			for (int j = 0; j < no; j++)
			{
				double ox = mult*pts2[2 * j]+c;
				double ov = pts2[2 * j + 1];
				QString cmox = cm + "ox" + QString::number(j);
				QString cmov = cm + "ov" + QString::number(j);
				settings.setValue(cmox, ox);
				settings.setValue(cmov, ov);
				
			}
			/*
			
			
			QString cmno = cm + "no";
			int no = settings.value(cmno, 0).toInt();
			cout << "Opacity nodes:" << endl;
			for (int j = 0; j < no; j++)
			{
				QString cmox = cm + "ox" + QString::number(j);
				QString cmov = cm + "ov" + QString::number(j);
				
				cout << "ox" << j << ":" << settings.value(cmox, 0).toDouble() << ",";
				cout << "ov=" << j << ":" << settings.value(cmov, 0).toDouble() << endl;
			}
		}*/

			cpt++;
			
		}
		
	}
	settings.endGroup();
	/*settings.beginGroup("tagmaps");
	cout << "Number of tagmaps:" << settings.value("nr", 0).toInt() << endl;
	 nr = settings.value("nr", 0).toInt();
	 int cpt = 0;
	 int custom = 0;
	if (nr > 0)
	{
		for (int i = 0; i < nr; i++)
		{
			vtkSmartPointer<vtkLookupTable> TagLut = vtkSmartPointer<vtkLookupTable>::New();
			std::vector<std::string> tagNames;
			if (i == 0)
			{
				TagLut= this->MorphoDigCore->GetTagLut();
			}
			QString tm = QString("tm");

			tm = tm + QString::number(i);
			cout << "Tagmap map" << i << ":" << settings.value(tm, "TagMap").toString().toStdString() << endl;

			QString TagMapName = settings.value(tm, "TagMap").toString();
			if (i == 0)
			{
				TagMapName = "TagMap";
			}

			// Name : settings.value(cm, "Colormap").toString()
			QString tmnt = tm + "nt";
			int nt = settings.value(tmnt, 0).toInt();
			cout << "Tags:" << endl;
			//cm0discretize = 1
			//cm0discretizenr = 10
			//cm0enableopacity = 1
			
			TagLut->SetNumberOfTableValues(nt);
			TagLut->Build();
			for (int j = 0; j < nt; j++)
			{
				QString tmt = tm + "t" + QString::number(j); 
				QString tmtr = tmt + "r";
				QString tmtg = tmt + "g";
				QString tmtb = tmt + "b";
				QString tmta = tmt + "a";
				QString Tag = "Tag"+ QString::number(j);
				QString tname = settings.value(tmt, Tag).toString();				
				double r = settings.value(tmtr, 0).toDouble();
				double g = settings.value(tmtg, 0).toDouble();
				double b = settings.value(tmtb, 0).toDouble();
				double	a = settings.value(tmta, 1.0).toDouble();

				cout << "Tag:" << j << ":" << tname.toStdString() << ",";
				cout << "rgb:" << r << ",";
				cout << g << ",";
				cout << b << ",";
				cout << a << endl;
				TagLut->SetTableValue(j, r, g,b, a);  //Grey
				tagNames.push_back(tname.toStdString());				
			}
			
			if (i > 0) { custom = 1; }

			this->MorphoDigCore->Getmui_ExistingTagMaps()->Stack.push_back(ExistingTagMaps::Element(TagMapName, TagLut, nt, tagNames, custom));
			if (i == 0)
			{
				this->MorphoDigCore->Getmui_ActiveTagMap()->TagMap = TagLut;
				cout << "Set Active color map2!" << endl;

				this->MorphoDigCore->Getmui_ActiveTagMap()->Name = TagMapName;
				this->MorphoDigCore->Getmui_ActiveTagMap()->numTags = nt;
				this->MorphoDigCore->Getmui_ActiveTagMap()->tagNames = tagNames;
			}

			

		}
	}*/
	settings.beginGroup("tagmaps");

	ExistingTagMaps *tagMaps = this->MorphoDigCore->Getmui_ExistingTagMaps();
	size = tagMaps->Stack.size();
	nr = (int)size;
	
	//total number of colormaps
	settings.setValue("nr", nr);
	for (int i = 0; i < size; i++)
	{
		
			//1 set the name of tagmap(i)
			QString tm = QString("tm");
			tm = tm + QString::number(i);
			settings.setValue(tm, tagMaps->Stack.at(i).Name);

		
			QString tmnt = tm + "nt";
			int nt = tagMaps->Stack.at(i).TagMap->GetNumberOfTableValues();
			settings.setValue(tmnt, nt);

		
			for (int j = 0; j < nt; j++)
			{
				QString tmt = tm + "t" + QString::number(j); 
				QString tmtr = tmt + "r";
				QString tmtg = tmt + "g";
				QString tmtb = tmt + "b";
				QString tmta = tmt + "a";
				
				QString tname = QString::fromStdString(tagMaps->Stack.at(i).tagNames.at(j));
				settings.setValue(tmt, tname);
				
				double r = tagMaps->Stack.at(i).TagMap->GetTableValue(j)[0];
				double g = tagMaps->Stack.at(i).TagMap->GetTableValue(j)[1];
				double b = tagMaps->Stack.at(i).TagMap->GetTableValue(j)[2];
				double a = tagMaps->Stack.at(i).TagMap->GetTableValue(j)[3];
				settings.setValue(tmtr, r);
				settings.setValue(tmtg, g);
				settings.setValue(tmtb, b);
				settings.setValue(tmta, a);

			}
									
	}
	settings.endGroup();



	//cout << "end save settings" << endl;
}





// problème
// aujourd'hui, c'est cette seule fonction qui est appelée lorsqu'on ouvre un nouveau maillage
// et lorsqu'on change entre "camera


void MorphoDig::ambientLightToWhite()
{
	vtkSmartPointer<vtkLightCollection> originalLights = vtkSmartPointer<vtkLightCollection>::New();
	originalLights = mqMorphoDigCore::instance()->getRenderer()->GetLights();
	std::cout << "Originally there are " << originalLights->GetNumberOfItems() << " lights." << std::endl;
	originalLights->InitTraversal();
	cout << "VTK_LIGHT_TYPE_CAMERA_LIGHT" << VTK_LIGHT_TYPE_CAMERA_LIGHT << endl;
	cout << "VTK_LIGHT_TYPE_HEADLIGHT" << VTK_LIGHT_TYPE_HEADLIGHT << endl;
	cout << "VTK_LIGHT_TYPE_HEADLIGHT" << VTK_LIGHT_TYPE_SCENE_LIGHT << endl;

	for (int i = 0; i < originalLights->GetNumberOfItems(); i++)
	{
		vtkLight *light = originalLights->GetNextItem();
		cout << "light type" << light->GetLightType() << endl;
		cout << "light ambient" << light->GetAmbientColor()[0] << "," << light->GetAmbientColor()[1] << "," << light->GetAmbientColor()[2] << endl;
		light->SetAmbientColor(1, 1, 1);

	}

	
}
void MorphoDig::slotExit() {
	//maybe we should save the .ini files!
	qApp->exit();
}





