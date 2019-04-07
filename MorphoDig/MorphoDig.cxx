



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
#include <vtkPiecewiseFunction.h>
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

#include <vtkRenderWindowInteractor.h>

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
	

	
	auto projectWindow = new QMainWindow(this->tabWidget);
	
	
	//this
	QWidget *projectTab = new QWidget();
	QHBoxLayout *projectLayout = new QHBoxLayout;
	projectLayout->addWidget(projectWindow);

	projectTab->setLayout(projectLayout);
	this->tabWidget->addTab(projectTab, "Project");


	
	//auto projectWindow = new QMainWindow();
	
	//this->mdiArea->addSubWindow(projectWindow);

	
	
#if VTK_MAJOR_VERSION<8	
	this->qvtkWidget2 = new QVTKWidget();
#elseif VTK_MAJOR_VERSION == 8 && VTK_MINOR_VERSION < 2
	this->qvtkWidget2 = new QVTKOpenGLWidget();	
#else
	this->qvtkWidget2 = new QVTKOpenGLNativeWidget();	
#endif


	qvtkWidget2->setObjectName(QStringLiteral("qvtkWidget"));
	QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	sizePolicy.setHorizontalStretch(1);
	sizePolicy.setVerticalStretch(1);
	sizePolicy.setHeightForWidth(qvtkWidget2->sizePolicy().hasHeightForWidth());
	qvtkWidget2->setSizePolicy(sizePolicy);
	qvtkWidget2->setMinimumSize(QSize(300, 300));

		
	projectWindow->setCentralWidget(qvtkWidget2);


	/*auto dock1 = new QDockWidget("3D viewer");
	dock1->setWidget(qvtkWidget2);
	dock1->setAllowedAreas(Qt::AllDockWidgetAreas);*/
	
	//projectWindow->setCentralWidget(dock1);
	


	//projectWindow->Maximise
	projectWindow->showMaximized();
	//auto mytree = new QTreeView(this->TabProject);
	/*auto mytree = new QTreeView(this->tabWidget);
	auto dock2 = new QDockWidget("Actors");
	dock2->setWidget(mytree);
	dock2->setAllowedAreas(Qt::AllDockWidgetAreas);
	projectWindow->addDockWidget(Qt::RightDockWidgetArea, dock2);*/

	projectWindow->setWindowModality(Qt::WindowModal);
	/*Qt::WindowFlags flags = windowFlags();
	Qt::WindowFlags closeFlag = Qt::WindowCloseButtonHint;
	flags = flags & (~closeFlag);
	projectWindow->setWindowFlags(flags);*/

	cout << "Try to set render window" << endl;
  auto window = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();

  cout << "Set render window done" << endl;
  

	vtkObject::GlobalWarningDisplayOff();
	this->MorphoDigCore =  new mqMorphoDigCore();
	this->MorphoDigCore->setQVTKWidget(this->qvtkWidget2);
	MorphoDigCore->SetProjectWindow(projectWindow);
	window->AddRenderer(this->MorphoDigCore->getRenderer());

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


			cout << "Add this map to !!" << endl;
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
	mqMorphoDigMenuBuilders::buildScalarsMenu(*this->menuScalars);
	mqMorphoDigMenuBuilders::buildRGBMenu(*this->menuRGB);
	mqMorphoDigMenuBuilders::buildEditMenu(*this->menuEdit);
	mqMorphoDigMenuBuilders::buildLandmarksMenu(*this->menuLandmarks);
	mqMorphoDigMenuBuilders::buildHelpMenu(*this->menuHelp);
	mqMorphoDigMenuBuilders::buildToolbars(*this);
	mqMorphoDigMenuBuilders::buildStatusBar(*this);
	mqMorphoDigMenuBuilders::buildProjectDocks(*projectWindow);
	cout << "OpenGL version:" << this->MorphoDigCore->GetOpenGLVersion().toStdString() << endl;
	cout << "About to build view Menu!" << endl;
	mqMorphoDigMenuBuilders::buildViewMenu(*this->menuView, *this, *projectWindow);
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

	//vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =		vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New(); //like paraview
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
	
 style->SetCurrentRenderer(this->MorphoDigCore->getRenderer());
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

  vtkSmartPointer<vtkBoxWidget> boxWidget = vtkSmartPointer<vtkBoxWidget>::New();
  boxWidget->SetInteractor(this->ui->qvtkWidget->GetRenderWindow()->GetInteractor());
  boxWidget->SetProp3D(actor);
  boxWidget->SetPlaceFactor(1.25);
  boxWidget->PlaceWidget();
  vtkSmartPointer<vtkMyCallback> callback = vtkSmartPointer<vtkMyCallback>::New();
  boxWidget->AddObserver(vtkCommand::InteractionEvent, callback);
  boxWidget->On();*/


  
  

  //EXAMPLE BEZIER SOURCE + BEZIER WIDGET
  
  unsigned int controlPointsX = 4;
  unsigned int controlPointsY = 4;
  
  // Create an arbitrary plane that we can use
  // to initialize the control points for the Bezier surface
  /*
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
void MorphoDig::dragEnterEvent(QDragEnterEvent *e)
{
	if (e->mimeData()->hasUrls()) {
		e->acceptProposedAction();
	}
}
void MorphoDig::dropEvent(QDropEvent *e)
{
	foreach(const QUrl &url, e->mimeData()->urls()) {
		QString fileName = url.toLocalFile();
		//this->MorphoDigCore->Open
		cout << "Dropped file:" << fileName.toStdString();
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
		std::size_t found3 = fileName.toStdString().find(VTKext3);
		std::size_t found4 = fileName.toStdString().find(VTKext4);
		if (found != std::string::npos || found2 != std::string::npos || found3 != std::string::npos || found4 != std::string::npos)
		{
			type = 1; //VTK
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


		if (type < 4)
		{
			mqMorphoDigCore::instance()->OpenMesh(fileName);
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
	settings.beginGroup("renderer_settings");
	settings.setValue("Anaglyph", this->MorphoDigCore->Getmui_Anaglyph());	
	settings.setValue("DisplayMode", this->MorphoDigCore->Getmui_DisplayMode());
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
				cout << "x" << j << "=" << curr << endl;
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



void MorphoDig::slotExit() {
	//maybe we should save the .ini files!
	qApp->exit();
}





