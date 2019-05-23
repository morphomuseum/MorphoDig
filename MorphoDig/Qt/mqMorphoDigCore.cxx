/*=========================================================================

  Program:   MorphoDig
  Module:    MorphoDigCore.cxx


=========================================================================*/
#include "mqMorphoDigCore.h"
#include "vtkMDActor.h"
#include "vtkMDVolume.h"
#include "vtkLMActor.h"
#include <vtkCallbackCommand.h>
#include <vtkCommand.h>
#include "vtkOrientationHelperActor.h"
#include <vtkMetaImageWriter.h>
#include <vtkXMLImageDataWriter.h>
#include "vtkOrientationHelperWidget.h"
#include "vtkBezierCurveSource.h"
#include <time.h>
#include <vtkAlgorithm.h>

#include<vtkBoxWidget.h>

#include <vtkLight.h>
#include <vtkTriangle.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkOpenGLGPUVolumeRayCastMapper.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolumeProperty.h>
#include <vtkVolume.h>
#include <vtkImageAccumulate.h>
#include <vtkImageHistogram.h>
#include <vtkBooleanOperationPolyDataFilter.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkLandmarkTransform.h>
#include <vtkTextProperty.h>
#include <vtkPlatonicSolidSource.h>
#include <vtkSphereSource.h>
#include <vtkImageData.h>
#include <vtkIterativeClosestPointTransform.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkReverseSense.h>
#include <vtkKdTreePointLocator.h>
#include <vtkFeatureEdges.h>
#include <vtkExtractEdges.h>
#include <vtkThreshold.h>
#include <vtkMaskFields.h>
#include <vtkActor.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkFillHolesFilter.h>
#include <vtkDensifyPolyData.h>
#include <vtkLinearSubdivisionFilter.h>
#include <vtkLoopSubdivisionFilter.h>
#include <vtkTriangleFilter.h>
#include <vtkButterflySubdivisionFilter.h>
#include <vtkInformation.h>
#include <vtkDecimatePro.h>
#include <vtkQuadricDecimation.h>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkReverseSense.h>
#include <vtkUnstructuredGrid.h>
#include <vtkReflectionFilter.h>
#include <vtkTransform.h>
#include <vtkPiecewiseFunction.h>
#include <vtkCellPicker.h>
#include <vtkProperty.h>
#include <vtkCurvatures.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkPolyDataMapper.h>
#include <vtkSTLWriter.h>
#include <vtkOBJWriter.h>
#include <vtkPLYWriter.h>
#include <vtkPolyDataWriter.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkThinPlateSplineTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkExtractSelection.h>
#include <vtkSmartPointer.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyDataReader.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkMetaImageReader.h>
#include <vtkXMLImageDataReader.h>
#include <vtkPolyDataNormals.h>
#include <vtkPLYReader.h>
#include <vtkMath.h>
#include <vtkSTLReader.h>
#include <vtkOBJReader.h>
#include <vtkCleanPolyData.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include <vtkMassProperties.h>

#include <vtkDelaunay3D.h>
#include <vtkQuadricDecimation.h>
#include <vtkCenterOfMass.h>
#include <vtkSphere.h>
#include <vtkDataSetSurfaceFilter.h>

#include <vtkGeometryFilter.h>

#include <vtkCubeAxesActor.h>
#include <vtkAppendPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkLookupTable.h>
#include <vtkDiscretizableColorTransferFunction.h>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QTextStream>
#include <QInputDialog>
#include <QFileDialog>
#include <QProgressDialog>
#include <QProgressBar>
#include <QStatusBar>
#include <QLabel>

#include "mqUndoStack.h"
#include "vtkMDCylinderSource.h"
#include "vtkMDCubeSource.h"

#define NORMAL_LMK 0
#define TARGET_LMK 1
#define NODE_LMK 2
#define HANDLE_LMK 3
#define FLAG_LMK 4

#define NORMAL_NODE 0
#define STARTING_NODE 1
#define MILESTONE_NODE 2
#define CONNECT_NODE 3

#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()
//-----------------------------------------------------------------------------
mqMorphoDigCore* mqMorphoDigCore::Instance = 0;




//-----------------------------------------------------------------------------
mqMorphoDigCore* mqMorphoDigCore::instance()
{
	return mqMorphoDigCore::Instance;
}


mqMorphoDigCore::mqMorphoDigCore()
{
	mqMorphoDigCore::Instance = this;
	/*VTK_CREATE(vtkMDActor, newactor);
	double defaultAmbient = newactor->GetProperty()->GetAmbient();
	double defaultDiffuse = newactor->GetProperty()->GetDiffuse();
	double defaultSpecular = newactor->GetProperty()->GetSpecular();
	double defaultSpecularPower = newactor->GetProperty()->GetSpecularPower();
	cout << "When creating an actor with VTK, ambient=" << defaultAmbient << ", diffuse=" << defaultDiffuse << ", specular=" << defaultSpecular << ", specularPower=" << defaultSpecularPower << endl;*/
	this->mui_DefaultSpecularPower = this->mui_SpecularPower = 1;
	this->mui_DefaultSpecular = this->mui_Specular= 0;
	this->mui_DefaultAmbient = this->mui_Ambient= 0;
	this->mui_DefaultDiffuse = this->mui_Diffuse = 100;
	this->mui_DefaultFontSize = this->mui_FontSize = 10;
	this->mui_DefaultDisplayLandmarkText = 1;
	this->mui_DisplayLandmarkText = 1;

	this->mui_OpenGL_minor_version = 0;
	this->mui_OpenGL_major_version = 0;
	this->mui_DisplayMode = 1; // point normals by defaults
	this->mui_TagModeActivated = 1;
	this->mui_TagTool = 0; //pencil by defaults
	this->qvtkWidget = NULL;
	this->Style = vtkSmartPointer<vtkMDInteractorStyle>::New();
	this->LassoStyle = vtkSmartPointer<vtkInteractorStyleDrawPolygon>::New();
	this->RubberStyle = vtkSmartPointer<vtkInteractorStyleRubberBand3D>::New();
	this->currentLassoMode = 0; // 
	this->currentRubberMode = 0; // 
	this->ScalarRangeMin = 0;
	this->ScalarRangeMax = 1;
	this->mui_ClippingPlane = 0; // no x=0 clipping plane by default
	this->mui_BackfaceCulling = 0; //no backface culling
	this->TagLut= vtkSmartPointer<vtkLookupTable>::New();
	this->ScalarRedLut = vtkSmartPointer<vtkDiscretizableColorTransferFunction>::New();
	this->ScalarRainbowLut = vtkSmartPointer<vtkDiscretizableColorTransferFunction>::New();
	
	this->mui_ActiveColorMap = new ActiveColorMap;
	this->mui_ExistingColorMaps = new ExistingColorMaps;
	this->mui_ActiveTagMap = new ActiveTagMap;
	this->mui_ActiveTag = 1;
	this->mui_PencilSize = 15;
	this->mui_PencilLimitAngle = 180;
	this->mui_PencilContiguous = 0;
	this->mui_ExistingTagMaps = new ExistingTagMaps;
	this->InitLuts();
	this->ActorCollection = vtkSmartPointer<vtkMDActorCollection>::New();
	this->VolumeCollection = vtkSmartPointer<vtkMDVolumeCollection>::New();
	cout << "try to create mui_ActiveArrays" << endl;
	this->mui_ActiveArray = new ActiveArray;
	this->mui_ExistingArrays = new ExistingArrays;
	this->mui_ArrayList = new ExistingArrays;
	this->ScalarBarActor = vtkSmartPointer<vtkScalarBarActor>::New();
	this->ScalarBarActor->SetOrientationToHorizontal();
	this->ScalarBarActor->SetHeight(0.1);

	this->TagScalarBarActor = vtkSmartPointer<vtkScalarBarActor>::New();
	this->TagScalarBarActor->SetOrientationToVertical();
	this->TagScalarBarActor->SetHeight(0.8);
	//this->TagScalarBarActor->GetLabelTextProperty()->set
	//this->TagScalarBarActor->SetLabelFormat("%d");
	//this->TagScalarBarActor->SetWidth(0.8);
	//this->TagScalarBarActor->SetTitleRatio(10);

	cout << "mui_ActiveArray created" << endl;
	QString none = QString("Solid color");
	this->Setmui_ActiveArray(none, -1, 0);
	cout << "mui_ActiveArray instantiated" << endl;
	this->Addmui_ExistingArrays(this->mui_ActiveArray->Name, this->mui_ActiveArray->DataType, this->mui_ActiveArray->NumComp);

	this->MainWindow = NULL;
	this->OrientationHelperWidget = vtkOrientationHelperWidget::New();
	this->mui_DefaultLandmarkMode = this->mui_LandmarkMode = 0;

	this->mui_LandmarkBodyType = this->mui_DefaultLandmarkBodyType = 0;
	this->mui_LandmarkRenderingSize=this->mui_DefaultLandmarkRenderingSize=1;
	this->mui_AdjustLandmarkRenderingSize= this->mui_DefaultAdjustLandmarkRenderingSize=1;
	this->mui_FlagRenderingSize= this->mui_DefaultFlagRenderingSize=5;
	this->mui_AdjustScaleFactor = this->mui_DefaultAdjustScaleFactor = 1;


	this->mui_ArrayVisibility = this->mui_DefaultArrayVisibility = 1;


	this->mui_Anaglyph = this->mui_DefaultAnaglyph = 0;
	this->mui_DisplayMode = this->mui_DefaultDisplayMode = 0;
	this->mui_ShowGrid = this->mui_DefaultShowGrid = 1;
	this->mui_GridSpacing = this->mui_DefaultGridSpacing = 10;
	this->mui_SizeUnit = this->mui_DefaultSizeUnit = "mm";

	this->mui_MoveMode = this->mui_DefaultMoveMode = 1;
	this->mui_FlagColor[0] = this->mui_DefaultFlagColor[0] = 0;
	this->mui_FlagColor[1] = this->mui_DefaultFlagColor[1] = 0.7;
	this->mui_FlagColor[2] = this->mui_DefaultFlagColor[2] = 0.7;

	this->mui_MeshColor[0] = this->mui_DefaultMeshColor[0] = 0.631373;
	this->mui_MeshColor[1] = this->mui_DefaultMeshColor[1] = 0.572549;
	this->mui_MeshColor[2] = this->mui_DefaultMeshColor[2] = 0.372549;
	this->mui_MeshColor[3] = this->mui_DefaultMeshColor[3] = 1;
	
	this->mui_BackGroundColor2[0] = this->mui_DefaultBackGroundColor2[0] = 0;
	this->mui_BackGroundColor2[1] = this->mui_DefaultBackGroundColor2[1] = 0;
	this->mui_BackGroundColor2[2] = this->mui_DefaultBackGroundColor2[2] = 0;

	this->mui_BackGroundColor[0] = this->mui_DefaultBackGroundColor[0] = 0.5;
	this->mui_BackGroundColor[1] = this->mui_DefaultBackGroundColor[1] = 0.5;
	this->mui_BackGroundColor[2] = this->mui_DefaultBackGroundColor[2] = 1;

	this->mui_ShowOrientationHelper = this->mui_DefaultShowOrientationHelper = 1;

	this->mui_X1Label = this->mui_DefaultX1Label = "Anterior";
	this->mui_X2Label = this->mui_DefaultX2Label = "Posterior";

	this->mui_Y1Label = this->mui_DefaultY1Label = "Left";
	this->mui_Y2Label = this->mui_DefaultY2Label = "Right";
	this->mui_Z1Label = this->mui_DefaultZ1Label = "Dorsal";
	this->mui_Z2Label = this->mui_DefaultZ2Label = "Ventral";

	this->mui_CameraOrtho = this->mui_DefaultCameraOrtho = 1;
	this->mui_CameraCentreOfMassAtOrigin = this->mui_DefaultCameraCentreOfMassAtOrigin = 0;
	//this->UndoStack = vtkSmartPointer<vtkUndoStack>::New();
	mqUndoStack* undoStack = new mqUndoStack();
	this->setUndoStack(undoStack);
	//this->mUndoStack = undoStack;
	//MorphoDig::testint = 10;
	//MorphoDig::Instance = this;
	//this->SetUndoCount(0);
	//vtkUndoStack* undoStack = vtkUndoStack::New();
	//vtkUndoSet* undoSet = vtkUndoSet::New();
	//vtkUndoElement* undoElement = vtkUndoElement::New();
	//undoStack->Push("Test", undoSet);

	/*
	vtkSMSession* session = vtkSMSession::New();
	vtkSMSessionProxyManager* pxm = session->GetSessionProxyManager();

	vtkSMProxy* sphere = pxm->NewProxy("sources", "SphereSource");
	sphere->UpdateVTKObjects();
	QVERIFY(sphere != NULL);
	QCOMPARE(vtkSMPropertyHelper(sphere, "Radius").GetAsDouble(), 0.5);

	vtkSMUndoStack* undoStack = vtkSMUndoStack::New();
	vtkUndoSet* undoSet = vtkUndoSet::New();
	vtkSMRemoteObjectUpdateUndoElement* undoElement = vtkSMRemoteObjectUpdateUndoElement::New();
	undoElement->SetSession(session);

	vtkSMMessage before;
	before.CopyFrom(*sphere->GetFullState());
	vtkSMPropertyHelper(sphere, "Radius").Set(1.2);
	sphere->UpdateVTKObjects();
	vtkSMMessage after;
	after.CopyFrom(*sphere->GetFullState());
	undoElement->SetUndoRedoState(&before, &after);

	undoSet->AddElement(undoElement);
	undoElement->Delete();
	undoStack->Push("ChangeRadius", undoSet);
	undoSet->Delete();

	QVERIFY(static_cast<bool>(undoStack->CanUndo()) == true);
	undoStack->Undo();
	QVERIFY(static_cast<bool>(undoStack->CanUndo()) == false);
	sphere->UpdateVTKObjects();
	QCOMPARE(vtkSMPropertyHelper(sphere, "Radius").GetAsDouble(), 0.5);

	QVERIFY(static_cast<bool>(undoStack->CanRedo()) == true);
	undoStack->Redo();
	sphere->UpdateVTKObjects();
	QCOMPARE(vtkSMPropertyHelper(sphere, "Radius").GetAsDouble(), 1.2);
	QVERIFY(static_cast<bool>(undoStack->CanRedo()) == false);

	undoStack->Delete();

	sphere->Delete();
	session->Delete();
	*/
	//this->RenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	this->RenderWindow = NULL;
	this->Renderer = vtkSmartPointer<vtkRenderer>::New();
	

	this->ActorCollection->SetRenderer(this->Renderer);
	this->VolumeCollection->SetRenderer(this->Renderer);
	this->BezierCurveSource = vtkSmartPointer<vtkBezierCurveSource>::New();
	this->BezierMapper=vtkSmartPointer<vtkPolyDataMapper>::New();
	this->BezierSelectedMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	BezierMapper->SetInputConnection(this->BezierCurveSource->GetOutputPort(0));
	BezierSelectedMapper->SetInputConnection(this->BezierCurveSource->GetOutputPort(1));
	BezierSelectedActor = vtkSmartPointer<vtkActor>::New();
	this->BezierActor=vtkSmartPointer<vtkActor>::New();

	this->BezierActor->SetMapper(this->BezierMapper);
	this->BezierActor->GetProperty()->SetColor(0, 0.5, 0);
	this->BezierActor->GetProperty()->SetLineWidth(2.0);

	this->BezierSelectedActor->SetMapper(this->BezierSelectedMapper);
	this->BezierSelectedActor->GetProperty()->SetColor(0.8, 0.2, 0);
	this->BezierSelectedActor->GetProperty()->SetLineWidth(3.0);

	this->BezierNHMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	this->BezierNHMapper->SetInputConnection(this->BezierCurveSource->GetOutputPort(2));
	this->BezierNHActor = vtkSmartPointer<vtkActor>::New();

	this->BezierNHActor->SetMapper(this->BezierNHMapper);
	this->BezierNHActor->GetProperty()->SetColor(0, 1, 1);

	

	//vtkSmartPointer<vtkBezierCurveSource> bezierCurve =
	this->NormalLandmarkCollection = vtkSmartPointer<vtkLMActorCollection>::New();
	this->NormalLandmarkCollection->SetRenderer(this->Renderer);
	this->TargetLandmarkCollection = vtkSmartPointer<vtkLMActorCollection>::New();
	this->TargetLandmarkCollection->SetRenderer(this->Renderer);
	this->NodeLandmarkCollection = vtkSmartPointer<vtkLMActorCollection>::New();
	this->NodeLandmarkCollection->SetRenderer(this->Renderer);
	this->HandleLandmarkCollection = vtkSmartPointer<vtkLMActorCollection>::New();
	this->HandleLandmarkCollection->SetRenderer(this->Renderer);

	this->FlagLandmarkCollection = vtkSmartPointer<vtkLMActorCollection>::New();
	this->FlagLandmarkCollection->SetRenderer(this->Renderer);

	this->Renderer->SetUseDepthPeeling(1);
	this->Renderer->SetMaximumNumberOfPeels(100);
	this->Renderer->SetOcclusionRatio(0.1);
	this->Camera = this->Renderer->GetActiveCamera();
	this->GridActor = vtkSmartPointer<vtkGridActor>::New();
	this->GridActor->SetGridSpacing(this->Getmui_GridSpacing());
	this->GridActor->SetGridType(2);	

	//TEST OPEN ONE SURFACE AND RENDER IT!!! NICE TRIANGLE NORMALS

	/*
	vtkSmartPointer<vtkPolyData> input1;
	auto reader1 =	vtkSmartPointer<vtkPolyDataReader>::New();
	//reader1->SetFileName("test.vtk");
	reader1->SetFileName("ear_scalar.vtk");
	//reader1->SetFileName("test_pinkRGB.vtk");
	reader1->Update();
	input1 = reader1->GetOutput();
   auto clean1 =	  vtkSmartPointer<vtkCleanPolyData>::New();
   clean1->SetInputData(input1);
   clean1->Update();

  auto mapper =   vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputData(clean1->GetOutput());
  mapper->Update();
  //mapper->SetInputConnection(clean1->GetOutputPort());
 

  auto actor =	  vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
   this->Renderer->AddActor(actor);
   */
  
	//

	cornerAnnotation= vtkSmartPointer<vtkCornerAnnotation>::New();

	this->Renderer->AddViewProp(cornerAnnotation);
	this->Renderer->AddActor(this->GridActor);
	this->Renderer->TwoSidedLightingOn();
	connect(this, SIGNAL(zoomChanged()), this, SLOT(slotEditGridInfos()));
	connect(this, SIGNAL(projectionModeChanged()), this, SLOT(slotEditGridInfos()));
}

mqMorphoDigCore::~mqMorphoDigCore()
{
	//this->ActorCollection->Delete();
	this->mui_ExistingArrays->Stack.clear();
	delete this->mui_ExistingArrays;
	this->mui_ArrayList->Stack.clear();
	delete this->mui_ArrayList;
	delete this->mui_ActiveArray;

	this->mui_ExistingColorMaps->Stack.clear();
	delete this->mui_ExistingColorMaps;
	delete this->mui_ActiveColorMap;
	
	for (int i = 0; i < this->mui_ExistingTagMaps->Stack.size(); i++)
	{
		this->mui_ExistingTagMaps->Stack.at(i).tagNames.clear();
	}
	this->mui_ExistingTagMaps->Stack.clear();
	delete this->mui_ExistingTagMaps;
	
	this->mui_ActiveTagMap->tagNames.clear();
	delete this->mui_ActiveTagMap;

	if (mqMorphoDigCore::Instance == this)
	{
		mqMorphoDigCore::Instance = 0;
	}
}

#if VTK_MAJOR_VERSION<8	  	
	void mqMorphoDigCore::setQVTKWidget(QVTKWidget *mqvtkWidget)
#elseif VTK_MAJOR_VERSION == 8 && VTK_MINOR_VERSION < 2
	void mqMorphoDigCore::setQVTKWidget(QVTKOpenGLWidget *mqvtkWidget)
#else
	void mqMorphoDigCore::setQVTKWidget(QVTKOpenGLNativeWidget *mqvtkWidget)
#endif
{
	this->qvtkWidget = mqvtkWidget;
}

#if VTK_MAJOR_VERSION<8	  	
	QVTKWidget* mqMorphoDigCore::getQVTKWidget()
#elseif VTK_MAJOR_VERSION == 8 && VTK_MINOR_VERSION < 2
	QVTKOpenGLWidget* mqMorphoDigCore::getQVTKWidget()
#else
	QVTKOpenGLNativeWidget* mqMorphoDigCore::getQVTKWidget()
#endif
{ return this->qvtkWidget; }
void mqMorphoDigCore::SetNormalInteractorStyle(vtkSmartPointer<vtkMDInteractorStyle> mStyle)
{
	this->Style = mStyle;
}
void mqMorphoDigCore::SetLassoInteractorStyle(vtkSmartPointer<vtkInteractorStyleDrawPolygon> mLassoStyle)
{
	this->LassoStyle = mLassoStyle;
}
void mqMorphoDigCore::SetRubberInteractorStyle(vtkSmartPointer<vtkInteractorStyleRubberBand3D> mRubbertyle)
{
	this->RubberStyle = mRubbertyle;
}
QString mqMorphoDigCore::GetVTKVersion() 
{
	QString myVersion = QString::number(VTK_MAJOR_VERSION) + "." + QString::number(VTK_MINOR_VERSION)+ "." + QString::number(VTK_BUILD_VERSION);
	return myVersion;
}

QString mqMorphoDigCore::GetOpenGLVersion()
{
	vtkSmartPointer<vtkGenericOpenGLRenderWindow> openGL = vtkGenericOpenGLRenderWindow::SafeDownCast(this->RenderWindow);
	int min, maj;
	openGL->GetOpenGLVersion(maj, min);
	this->SetOpenGLVersion(maj, min);
	QString myVersion = QString::number(this->mui_OpenGL_major_version) + "." + QString::number(this->mui_OpenGL_minor_version);
	//QString myVersion = QString::number(maj) + "." + QString::number(min);
	return myVersion;
}
void mqMorphoDigCore::TestVolume()
{
	//only used at startup fo open a volume and rendering it!
	vtkSmartPointer <vtkMetaImageReader> metaReader = vtkSmartPointer<vtkMetaImageReader>::New();
	vtkSmartPointer<vtkImageData> input = vtkSmartPointer<vtkImageData>::New();
	metaReader->SetFileName("head.mha");
	metaReader->Update();
	input = metaReader->GetOutput();
	int dim[3];
	input->GetDimensions(dim);
	int numcells = input->GetNumberOfCells();
	input->GetScalarTypeAsString();
	//input->Get
	cout << "Read Volume: dim=" << dim[0] << ", " << dim[1] << ", " << dim[2] << "numcells=" << numcells << endl;
	cout << "Dim0*Dim1*Dim2:" << dim[0] * dim[1] * dim[2] << endl;
	cout << "Image type:" << input->GetScalarTypeAsString() << endl;
	cout << "Image type int:" << input->GetScalarType() << "=" << VTK_UNSIGNED_SHORT << "?" << endl;
	cout << "Number of scalar components:" << input->GetNumberOfScalarComponents() << endl;
	

	cout << "Range min:" << input->GetScalarRange()[0] << ", Range max:" << input->GetScalarRange()[1] << endl;

	if (dim[0] < 2 ||
		dim[1] < 2 ||
		dim[2] < 2)
	{
		return;
	}
	else
	{
		cout << "Try visualize!!!" << endl;

		vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
		vtkSmartPointer<vtkSmartVolumeMapper> mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
		//vtkSmartPointer <vtkOpenGLGPUVolumeRayCastMapper> mapper = vtkSmartPointer<vtkOpenGLGPUVolumeRayCastMapper>::New();
		//vtkSmartPointer <vtkGPUVolumeRayCastMapper> mapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
		vtkSmartPointer<vtkDiscretizableColorTransferFunction> TF = vtkSmartPointer<vtkDiscretizableColorTransferFunction>::New();
		//vtkSmartPointer<vtkColorTransferFunction> colorFun = vtkSmartPointer <vtkColorTransferFunction>::New();
		vtkSmartPointer<vtkPiecewiseFunction> opacityFun = vtkSmartPointer<vtkPiecewiseFunction>::New();
		vtkSmartPointer<vtkImageAccumulate> histogram =
			vtkSmartPointer<vtkImageAccumulate>::New();
		//mapper->SetRequestedRenderModeToDefault();


		histogram->SetInputData(input);
		if (input->GetScalarType() == VTK_UNSIGNED_SHORT)
		{
			histogram->SetComponentExtent(VTK_UNSIGNED_SHORT_MIN, VTK_UNSIGNED_SHORT_MAX, 0, 0, 0, 0);
			histogram->SetComponentOrigin(0, 0, 0);
		}
		if (input->GetScalarType() == VTK_SHORT)
		{
			//cout << "signed shorts !" << endl;
			//cout << "VTK SHORT MIN = " << VTK_SHORT_MIN << endl;
			histogram->SetComponentExtent(VTK_SHORT_MIN, VTK_SHORT_MAX, 0, 0, 0, 0);
			histogram->SetComponentOrigin(VTK_SHORT_MIN, 0, 0);
		}

		int bin_spacing = 100;
		histogram->SetComponentSpacing(bin_spacing, 0, 0);
		histogram->Update();
		// faire plutôt une liste avec push.

		std::vector<int> peaks;
		std::vector<int> peaksT;
		std::vector<int> peakVals;
		std::vector<int> lows;
		std::vector<int> lowsT;
		std::vector<int> lowVals;

		int dims[3];
		histogram->GetOutput()->GetDimensions(dims);
		cout << "Histogram (max) dims=" << dims[0] << ", " << dims[1] << ", " << dims[2]  << endl;
		vtkIdType used_bins = (vtkIdType)(dims[0] / bin_spacing);
		cout << "Histogram (used) dims=" << used_bins  << endl;
		int prevbin = 0;


		int p_or_l = 1; //1 search peak //2 search low
		int p_i = 0;
		int l_i = -1;

		peaks.push_back(0);
		peaksT.push_back(0);
		peakVals.push_back(0);
		for (vtkIdType bin = 0; bin < used_bins; ++bin)
		{
			int binT;
			if (input->GetScalarType() == VTK_UNSIGNED_SHORT) {
				binT = VTK_UNSIGNED_SHORT_MIN + bin * bin_spacing;
			}
			if (input->GetScalarType() == VTK_SHORT) {
				binT = VTK_SHORT_MIN + bin * bin_spacing;
			}
			int curbin = *(static_cast<int*>(histogram->GetOutput()->GetScalarPointer(bin, 0, 0)));
			//histogram->GetOutput()->GetPointData()->GetScalars()->GetTuple1(bin);
			if (p_or_l == 1)//search peak
			{
				if (curbin >= prevbin)
				{
					if (curbin > 0)
					{
						peaks.at(p_i) = bin;
						peaksT.at(p_i) = binT;
						peakVals.at(p_i) = curbin;
					}
				}
				else
				{
					// starts do decrease: 
					p_or_l = 0;
					lows.push_back(bin);
					lowsT.push_back(binT);
					lowVals.push_back(curbin);
					l_i++; //cout << "l_i" << l_i << endl;
				}
			}
			if (p_or_l == 0)//search low
			{
				if (curbin < prevbin)
				{
					lows.at(l_i) = bin;
					lowsT.at(l_i) = binT;
					lowVals.at(l_i) = curbin;
				}
				else
				{
					// starts do re-increase: 
					p_or_l = 1;
					peaks.push_back(bin);
					peaksT.push_back(binT);
					peakVals.push_back(curbin);
					p_i++;
				}
			}

			//cout <<"bin="<<bin<<"|"<< *(static_cast<int*>(histogram->GetOutput()->GetScalarPointer(bin, 0, 0))) << " ";
		//	cout << histogram->GetOutput()->GetPointData()->GetScalars()->GetTuple1(bin) << endl;				
			prevbin = curbin;
		}

		for (int i = 0; i <= p_i; i++)
		{
			//cout << "p" << i <<":"<< peaksT.at(i) << ", val=" << peakVals.at(i) << endl;
		}
		for (int i = 0; i <= l_i; i++)
		{
			//cout << "l" << i << ":" << lowsT.at(i) << ", val=" << lowVals.at(i) << endl;
		}

		// Create the property and attach the transfer functions
		vtkSmartPointer < vtkVolumeProperty> property = vtkSmartPointer <vtkVolumeProperty>::New();
		property->SetIndependentComponents(true);
		property->SetColor(TF);
		property->SetScalarOpacity(opacityFun);
		property->SetInterpolationTypeToLinear();
		//mapper->SetInputData(input);
		mapper->SetInputConnection(metaReader->GetOutputPort());
		// connect up the volume to the property and the mapper
		volume->SetProperty(property);
		volume->SetMapper(mapper);

		int first_point = 0; // as first low... a
		int last_point = 0; //somme pondérée autres peaks,  puis chercher s'il y a un low après cette valeur. Si oui, moyenne des deux. sinon on garde la moyenne pondérée.

		if (input->GetScalarType() == VTK_UNSIGNED_SHORT) {
			first_point = VTK_UNSIGNED_SHORT_MIN;
			last_point = VTK_UNSIGNED_SHORT_MAX;
		}
		else
		{
			first_point = VTK_SHORT_MIN;
			last_point = VTK_SHORT_MAX;

		}
		if (l_i >= 0)
		{
			first_point = lowsT.at(0);
			if (p_i > 0) {
				first_point = (int)(0.5*(lowsT.at(0) + peaksT.at(1)));
			}
		}

		//now search last point
		int avg_peaks = 0;

		int low_after_avgpeaks = 0;
		if (p_i > 0)
		{
			double sum_peaks = 0;
			double sum_peakVals = 0;
			for (int i = 1; i <= p_i; i++)
			{
				//peaksT.at(i) peakVals.at(i) ;
				//cout << "peaksT.at(i)="<<peaksT.at(i) << endl;
				//cout << "peakVals.at(i)=" << peakVals.at(i) << endl;
				double mult = (double)peaksT.at(i)*(double)peakVals.at(i);
				//cout << "mult=" << mult << endl;
				sum_peakVals += (double)peakVals.at(i);
				sum_peaks += mult;
				//cout << "sum_peaks=" << sum_peaks << endl;
				//cout << "sum_peakVals=" << sum_peakVals << endl;

			}
			if (sum_peakVals > 0)
			{
				avg_peaks = (int)(sum_peaks / sum_peakVals);
				//cout << "avg_peaks="<<avg_peaks << endl;
				// search if a low exists after avg_peaks
				int exists = 0;
				int i_low = 0;
				if (l_i >= 0)
				{
					for (int i = 0; i <= l_i; i++)
					{
						if (lowsT.at(i) > avg_peaks)
						{
							if (exists == 0) { i_low = i; }
							exists = 1;

						}
					}
					if (exists == 1)
					{
						low_after_avgpeaks = lowsT.at(i_low);
						last_point = (int)((low_after_avgpeaks + avg_peaks) / 2);
					}
				}
				else
				{
					last_point = avg_peaks;
				}
			}
		}
		//let's put first point a little bit further!
		first_point = (int)(first_point + 0.2*(last_point - first_point));

		int second_point = (int)(first_point + 0.2*(last_point - first_point));
		int third_point = (int)(first_point + 0.4*(last_point - first_point));

		TF->AddRGBPoint(first_point, 0, 0, 0, 0.5, 0);
		TF->AddRGBPoint(second_point, 0.73, 0, 0, 0.5, 0);
		TF->AddRGBPoint(third_point, .90, .82, .56, .5, 0);
		TF->AddRGBPoint(last_point, 1, 1, 1, .5, 0);

		opacityFun->AddPoint(first_point, 0, 0.5, 0);
		opacityFun->AddPoint(second_point, 0.5, .5, 0);
		opacityFun->AddPoint(third_point, 0.8, .5, 0);
		opacityFun->AddPoint(last_point, 0.99, 0.5, 0);
		TF->SetEnableOpacityMapping(true);
		TF->SetScalarOpacityFunction(opacityFun);
		TF->Build();
		mapper->SetBlendModeToComposite();


		//property->set
		property->SetAmbient(0.2);
		property->SetDiffuse(1.0);
		property->SetSpecular(0.2);
		property->SetSpecularPower(14);
		property->SetInterpolationTypeToLinear();
		property->ShadeOn();

		double SOUD = 0.89;
		//cout << "Scalar Opacity Unit Distance:" << bblength << "/500=" << SOUD << endl;
		if (SOUD == 0) { SOUD = 0.89; }

		property->SetScalarOpacityUnitDistance(SOUD); // Ca doit être fonction de la taille des spécimens, sinon ça va pas... 
		mapper->Update();
		
		volume->Update();

	
		this->getRenderer()->AddVolume(volume);
	}
}
void mqMorphoDigCore::SetOpenGLVersion(int major, int minor)
{
	this->mui_OpenGL_major_version = major;
	this->mui_OpenGL_minor_version = minor;
	
}
void mqMorphoDigCore::ActivateClippingPlane()
{
	if (this->Getmui_ClippinPlane() == 1)
	{

		double cr[2];
		double cameracentre[3];
		double camerafocalpoint[3];
		
		this->getRenderer()->GetActiveCamera()->GetPosition(cameracentre);
		this->getRenderer()->GetActiveCamera()->GetFocalPoint(camerafocalpoint);
		

		double dist = sqrt(vtkMath::Distance2BetweenPoints(cameracentre, camerafocalpoint));
		this->getRenderer()->GetActiveCamera()->GetClippingRange(cr);
		
		this->getRenderer()->GetActiveCamera()->SetClippingRange(dist, cr[1]);

	}
}
void mqMorphoDigCore::ActivateBackfaceCulling() {

	this->ActorCollection->InitTraversal();

	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMDActor * myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		
			if (this->mui_BackfaceCulling == 0)
			{
				myActor->GetProperty()->BackfaceCullingOff();
			}
			else
			{
				myActor->GetProperty()->BackfaceCullingOn();
			}
					
	}

}
void mqMorphoDigCore::ChangeBackfaceCulling() {
	if (this->mui_BackfaceCulling == 0) { this->mui_BackfaceCulling = 1; }
	else { this->mui_BackfaceCulling = 0; }
	this->ActivateBackfaceCulling();
}

int mqMorphoDigCore::Getmui_BackfaceCulling() { return this->mui_BackfaceCulling; }
void mqMorphoDigCore::Setmui_BackfaceCulling(int on_off) {
	if (on_off == 0 || on_off == 1) { this->mui_BackfaceCulling = on_off; }
}

void mqMorphoDigCore::Setmui_TagTool(int tool)
{
	this->mui_TagTool = tool;
	//0 : pencil, 1, paintbucket
}
int mqMorphoDigCore::Getmui_TagTool()
{
	return this->mui_TagTool;
}

void mqMorphoDigCore::Setmui_TagModeActivated(int activated) {
	this->mui_TagModeActivated = activated;
	/*	
	If off : delete existing  KdTrees for all actors.
	*/
	if (activated == 0)
	{
		this->ActorCollection->InitTraversal();

		for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
		{
			vtkMDActor * myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
			myActor->FreeKdTree();
			myActor->FreeConnectivityFilter();
		}
	}
}

int mqMorphoDigCore::Already_Listed(vtkIdType ve, vtkSmartPointer<vtkIdList> ptList)
{
	int already = 0;
	for (vtkIdType i = 0; i<ptList->GetNumberOfIds(); i++)
	{

		if (ve == ptList->GetId(i))
		{
			already = 1;
		}
	}
	return already;
}

void mqMorphoDigCore::PropagateVertices(
	vtkSmartPointer<vtkPolyData> mesh, 
	vtkSmartPointer<vtkFloatArray> norms, 
	double vn[3], 
	double min_cos,
	vtkSmartPointer<vtkIdList> propagationPointsList, 
	vtkSmartPointer<vtkIdList> outputPtsList, 
	vtkSmartPointer<vtkIdList> ptsToInvestigate, 
	vtkSmartPointer<vtkIdList> alreadyInvestigatedPtsList,
	
	int *list_changed
)
{
	// this function updates an outputPtsList (list of vertices Id).
	// it looks for the input surface all the vertices within a list (ptsToInvestigate) and includes them in outputPtsList if 
	// it their normal meet some criterion defined by vn[3] min_cos (angle between vn and that of investigated points should be smaller than a limit angle defined by min_cos)

	// for each vertex of ptsToInvestigate.
	//		if point meets requirements:		
	//			a- normal*vn > min_cos
	//          b- not already in outputPtsList
	//			c- within propagationPointsList
	//		then: 
	//			a: include it in outputPtsList
	//          b: construct neighbors point list of this newly inserted point	
	//          c: for all neigbor points	
	//				if neighbor point meets requirements:	
	//					1- not already in futureptsToInvestigate
	//					2- not already in outputPtsList
	//					3- not already in alreadyInvestigatedPtsList
	//					4- within propagationPointsList
	//			    then include neihbor point in futureptsToInvestigate
	
	vtkSmartPointer<vtkIdList> futureptsToInvestigate = vtkSmartPointer<vtkIdList>::New();
	int bchanged = 0;
	// for each vertex of ptsToInvestigate.
	for (vtkIdType i = 0; i<ptsToInvestigate->GetNumberOfIds(); i++)
	{		
		vtkIdType investigatedPt = ptsToInvestigate->GetId(i);
		alreadyInvestigatedPtsList->InsertNextId(investigatedPt);

		//		if point meets requirements:		
		//          a- normal*vn > min_cos
		double *vn2;
		int ok = 1;
		vn2 = norms->GetTuple(investigatedPt);
		double curr_cos = vn[0] * vn2[0] + vn[1] * vn2[1] + vn[2] * vn2[2];
		if (curr_cos<min_cos)
		{
			ok = 0;
		}
		//          b- not already in outpuPtsList
		if (ok == 1 && this->Already_Listed(investigatedPt, outputPtsList) == 1)
		{
			ok = 0;
		}
		//			c- within propagationPointsList
		if (ok == 1 && this->Already_Listed(investigatedPt, propagationPointsList) == 0)
		{
			ok = 0;
		}

		//        then:
		if (ok == 1)
		{
			//			a- include it in outputPtsList
			outputPtsList->InsertNextId(investigatedPt);
			//          b: construct neighbors point list of this newly inserted point	
			vtkSmartPointer<vtkIdList> neighborPts = vtkSmartPointer<vtkIdList>::New();
			vtkSmartPointer<vtkIdList> cellPts = vtkSmartPointer<vtkIdList>::New();
			vtkSmartPointer<vtkIdList> ptCells = vtkSmartPointer<vtkIdList>::New();
			mesh->GetPointCells(investigatedPt, ptCells);
			for (vtkIdType j = 0; j<ptCells->GetNumberOfIds(); j++)
			{
				mesh->GetCellPoints(ptCells->GetId(j), cellPts);
				for (vtkIdType k = 0; k < cellPts->GetNumberOfIds(); k++)
				{
					if (this->Already_Listed(cellPts->GetId(k), neighborPts) == 0)
					{
						neighborPts->InsertNextId(cellPts->GetId(k));
					}

				}
			}
			//          c: for all neigbor points	
			for (vtkIdType j = 0; j < neighborPts->GetNumberOfIds(); j++)
			{
				ok = 1;
				vtkIdType neighborPt = neighborPts->GetId(j);
				//				if neighbor point meets requirements:	

				//					1- not already in futureptsToInvestigate				
				if (this->Already_Listed(neighborPt, futureptsToInvestigate) == 1)
				{
					ok = 0;
				}
				//					2- not already in outputPtsList
				if (ok == 1 && this->Already_Listed(neighborPt, outputPtsList) == 1)
				{
					ok = 0;
				}
				//					3- not already in alreadyInvestigatedPtsList
				if (ok == 1 && this->Already_Listed(neighborPt, alreadyInvestigatedPtsList) == 1)
				{
					ok = 0;
				}
				//					4- within propagationPointsList						
				if (ok == 1 && this->Already_Listed(neighborPt, propagationPointsList) == 0)
				{
					ok = 0;
				}

				//				then include it in futurepts to investigate
				if (ok == 1)
				{
					futureptsToInvestigate->InsertNextId(neighborPt);
				}
				
			}
			
		}



		if (futureptsToInvestigate->GetNumberOfIds() > 0)
		{
			bchanged = 1;
		}
		
	}
	

	
	ptsToInvestigate->Initialize();
	ptsToInvestigate->DeepCopy(futureptsToInvestigate);
	*list_changed = bchanged;

}
vtkSmartPointer<vtkIdTypeArray> mqMorphoDigCore::Get_Tag_Region_Sizes(vtkIntArray *Tags)
{
	// Constructs an array containing number of vertices held by each tag.
	vtkSmartPointer<vtkIdTypeArray> region_sizes = vtkSmartPointer<vtkIdTypeArray>::New();
	// get min and max ranges.
	vtkIdType Tag_Min = VTK_INT_MAX;
	vtkIdType Tag_Max = 0;
	//double current_tag;
	vtkIdType current_tag;
	vtkIdType act_ve_nr;

	
	if (Tags != NULL)
	{
		for (vtkIdType i = 0; i<Tags->GetNumberOfTuples(); i++)	// for each vertex 
		{
			current_tag = (vtkIdType)Tags->GetTuple(i)[0];
			if (current_tag > Tag_Max) { Tag_Max = current_tag; }
			if (current_tag < Tag_Min) { Tag_Min = current_tag; }
		}
		region_sizes->SetNumberOfComponents(1);
		region_sizes->SetNumberOfValues(Tag_Max + 1);
		region_sizes->SetNumberOfTuples(Tag_Max + 1);
		for (vtkIdType i = 0; i<region_sizes->GetNumberOfTuples(); i++)	// for each vertex 
		{
			region_sizes->SetValue(i, 0);
		}

		for (vtkIdType i = 0; i<Tags->GetNumberOfTuples(); i++)	// for each vertex 
		{
			current_tag = (vtkIdType)Tags->GetTuple(i)[0];
			if (current_tag >= 0)
			{
				act_ve_nr = region_sizes->GetValue(current_tag);
				act_ve_nr++;
				region_sizes->SetValue(current_tag, act_ve_nr);
			}

		}
	}
	return region_sizes;
}

void mqMorphoDigCore::Decompose_Tag(int tag_min, int tag_max)
{
	// dirty hack:
	//ActiveArrays *myActiveArrays = mqMorphoDigCore::instance()->Getmui_ActiveArray();
	cout << "Call Decompose Tag" << endl;
	vtkSmartPointer<vtkMDActorCollection> newcoll = vtkSmartPointer<vtkMDActorCollection>::New();
	
	this->ActorCollection->InitTraversal();
	vtkIdType num = this->ActorCollection->GetNumberOfItems();
	cout << "Here are num" << num << "Actors" << endl;
	int modified = 0;
	for (vtkIdType i = 0; i < num; i++)
	{
		cout << "try to get next actor:" << i << endl;
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		cout << "ok?" << i << endl;
		if (myActor->GetSelected() == 1)
		{
			//myActor->SetSelected(0); we don't unselect after a cut


			vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());

			if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
			{
				vtkPolyData *myPD = vtkPolyData::SafeDownCast(mymapper->GetInput());



				vtkSmartPointer<vtkFloatArray> newScalars =
					vtkSmartPointer<vtkFloatArray>::New();

				newScalars->SetNumberOfComponents(1); //3d normals (ie x,y,z)
				newScalars->SetNumberOfTuples(myPD->GetNumberOfPoints());

				vtkIntArray *currentTag;
				currentTag = vtkIntArray::SafeDownCast(myPD->GetPointData()->GetScalars());
				cout << "Got current Tag array... " << endl;
				if (currentTag != NULL)
				{
					for (vtkIdType i = 0; i < myPD->GetNumberOfPoints(); i++)	// for each vertex 
					{
						//std::cout<<"vertex"<<i<<", current region:"<<currentRegions->GetTuple(i)[0]<<std::endl;
						newScalars->InsertTuple1(i, currentTag->GetTuple(i)[0]);
					}
					newScalars->SetName("TMP");

					myPD->GetPointData()->RemoveArray("TMP");
					myPD->GetPointData()->AddArray(newScalars);
					//myPD->GetPointData()->SetActiveScalars("TMP");
					vtkSmartPointer<vtkIdTypeArray> region_sizes = vtkSmartPointer<vtkIdTypeArray>::New();
					cout << "Try to call Get_Tag_Region_Sizes"<<endl;
					region_sizes = this->Get_Tag_Region_Sizes(currentTag);
					cout << "Number of regions: " << region_sizes->GetNumberOfTuples()<< endl;
					for (vtkIdType i = 0; i < region_sizes->GetNumberOfTuples(); i++)
					{
						cout << "region_sizes->GetTuple((vtkIdType)"<<i<<")[0]" << region_sizes->GetTuple((vtkIdType)i)[0] << endl;
						if (region_sizes->GetTuple((vtkIdType)i)[0] >= (vtkIdType)4) // no region smaller than 4... could be parameterized somewhere though
						{

							vtkSmartPointer<vtkThreshold> selector =
								vtkSmartPointer<vtkThreshold>::New();
							vtkSmartPointer<vtkMaskFields> scalarsOff =
								vtkSmartPointer<vtkMaskFields>::New();
							vtkSmartPointer<vtkGeometryFilter> geometry =
								vtkSmartPointer<vtkGeometryFilter>::New();
							selector->SetInputData((vtkPolyData*)myPD);
							selector->SetInputArrayToProcess(0, 0, 0,
								vtkDataObject::FIELD_ASSOCIATION_CELLS,
								vtkDataSetAttributes::SCALARS);
							selector->SetAllScalars(1);
							selector->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "TMP");
							selector->ThresholdBetween((double)(i), (double)(i));

							selector->Update();
							std::cout << "\n Extract_Scalar_Range new Number of points:" << selector->GetOutput()->GetNumberOfPoints() << std::endl;
							std::cout << "\nExtract_Scalar_Range new Number of cells:" << selector->GetOutput()->GetNumberOfCells() << std::endl;

							scalarsOff->SetInputData(selector->GetOutput());
							scalarsOff->CopyAttributeOff(vtkMaskFields::POINT_DATA, vtkDataSetAttributes::SCALARS);
							scalarsOff->CopyAttributeOff(vtkMaskFields::CELL_DATA, vtkDataSetAttributes::SCALARS);
							scalarsOff->Update();
							geometry->SetInputData(scalarsOff->GetOutput());
							geometry->Update();




							vtkSmartPointer<vtkPolyData> MyObj = vtkSmartPointer<vtkPolyData>::New();


							MyObj = geometry->GetOutput();
							MyObj->GetPointData()->RemoveArray("TMP");
							std::cout << "\nExtract array new Number of points:" << MyObj->GetNumberOfPoints() << std::endl;
							std::cout << "\nExtract array new Number of cells:" << MyObj->GetNumberOfCells() << std::endl;

							if (MyObj->GetNumberOfPoints() > 10)
							{
								VTK_CREATE(vtkMDActor, newactor);
								if (this->mui_BackfaceCulling == 0)
								{
									newactor->GetProperty()->BackfaceCullingOff();
								}
								else
								{
									newactor->GetProperty()->BackfaceCullingOn();
								}


								//VTK_CREATE(vtkDataSetMapper, newmapper);
								VTK_CREATE(vtkPolyDataMapper, newmapper);
								//VTK_CREATE(vtkSmartPointer<vtkDataSetMapper>
								//newmapper->ImmediateModeRenderingOn();
								newmapper->SetColorModeToDefault();

								if (
									(this->mui_ActiveArray->DataType == VTK_INT || this->mui_ActiveArray->DataType == VTK_UNSIGNED_INT)
									&& this->mui_ActiveArray->NumComp == 1
									)
								{
									newmapper->SetScalarRange(0, this->Getmui_ActiveTagMap()->numTags - 1);
									newmapper->SetLookupTable(this->Getmui_ActiveTagMap()->TagMap);

								}
								else
								{
									newmapper->SetLookupTable(this->Getmui_ActiveColorMap()->ColorMap);
								}


								newmapper->ScalarVisibilityOn();

								cout << "extract object" << MyObj->GetNumberOfPoints() << endl;
								//newmapper->SetInputConnection(delaunay3D->GetOutputPort());
								newmapper->SetInputData(MyObj);

								//VTK_CREATE(vtkActor, actor);

								int num = 2;

								vtkSmartPointer<vtkMatrix4x4> Mat = myActor->GetMatrix();
								vtkTransform *newTransform = vtkTransform::New();
								newTransform->PostMultiply();

								newTransform->SetMatrix(Mat);
								newactor->SetPosition(newTransform->GetPosition());
								newactor->SetScale(newTransform->GetScale());
								newactor->SetOrientation(newTransform->GetOrientation());
								newTransform->Delete();


								double color[4] = { 0.5, 0.5, 0.5, 1 };
								myActor->GetmColor(color);
								/*double trans = 100* color[3];
								if (trans >= 0 && trans <= 100)
								{
								color[3] = (double)((double)trans / 100);


								}*/

								newactor->SetmColor(color);

								newactor->SetMapper(newmapper);
								newactor->SetSelected(0);

								//std::string newname = this->CheckingName(myActor->GetName());
								newactor->SetName(myActor->GetName() + "_tag_"+std::to_string((int)i));
								//newactor->SetName(newname);
								cout << "try to add new actor=" << endl;
								newcoll->AddTmpItem(newactor);
								modified = 1;
							}
						}

					}
					myPD->GetPointData()->RemoveArray("TMP");
				}
			}
		}
	}
	if (modified == 1)
	{
		newcoll->InitTraversal();
		vtkIdType num = newcoll->GetNumberOfItems();
		for (vtkIdType i = 0; i < num; i++)
		{
			cout << "try to get next actor from newcoll:" << i << endl;
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(newcoll->GetNextActor());
			
			myActor->SetColorProperties(this->mui_Ambient, this->mui_Diffuse, this->mui_Specular, this->mui_SpecularPower);
			myActor->SetDisplayMode(this->mui_DisplayMode);
			this->getActorCollection()->AddItem(myActor);
			emit this->actorsMightHaveChanged();
			std::string action = "Object extracted from array: " + myActor->GetName();
			int mCount = BEGIN_UNDO_SET(action);
			this->getActorCollection()->CreateLoadUndoSet(mCount, 1);
			END_UNDO_SET();


		}
		//cout << "camera and grid adjusted" << endl;
		cout << "new actor(s) added" << endl;
		this->Initmui_ExistingArrays();

		cout << "Set actor collection changed" << endl;
		this->getActorCollection()->SetChanged(1);
		cout << "Actor collection changed" << endl;

		this->AdjustCameraAndGrid();
		cout << "Camera and grid adjusted" << endl;

		if (this->Getmui_AdjustLandmarkRenderingSize() == 1)
		{
			this->UpdateLandmarkSettings();
		}
		//this->Setmui_ActiveArray(myActiveArray->Name, myActiveArray->DataType, myActiveArray->NumComp);
		this->Render();
	}


}

void mqMorphoDigCore::Extract_Array_Range(double array_min, int array_max)
{
	// dirty hack:
	//ActiveArray *myActiveArray = mqMorphoDigCore::instance()->Getmui_ActiveArray();
	
	vtkSmartPointer<vtkMDActorCollection> newcoll = vtkSmartPointer<vtkMDActorCollection>::New();
	this->ActorCollection->InitTraversal();
	vtkIdType num = this->ActorCollection->GetNumberOfItems();
	int modified = 0;
	QString Sauv_Active_Array_Name = this->Getmui_ActiveArray()->Name;
	for (vtkIdType i = 0; i < num; i++)
	{
		cout << "try to get next actor:" << i << endl;
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			//myActor->SetSelected(0); we don't unselect after a cut


			vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());

			if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
			{
				vtkPolyData *myPD = vtkPolyData::SafeDownCast(mymapper->GetInput());

				vtkSmartPointer<vtkFloatArray> newScalars =
					vtkSmartPointer<vtkFloatArray>::New();

				newScalars->SetNumberOfComponents(1); //3d normals (ie x,y,z)
				newScalars->SetNumberOfTuples(myPD->GetNumberOfPoints());

				vtkFloatArray *currentScalar;
				currentScalar = (vtkFloatArray*)myPD->GetPointData()->GetScalars();

				if (currentScalar != NULL)
				{
					for (vtkIdType i = 0; i < myPD->GetNumberOfPoints(); i++)	// for each vertex 
					{
						//std::cout<<"vertex"<<i<<", current region:"<<currentRegions->GetTuple(i)[0]<<std::endl;
						newScalars->InsertTuple1(i, currentScalar->GetTuple(i)[0]);
					}
					newScalars->SetName("TMP");

					myPD->GetPointData()->RemoveArray("TMP");
					myPD->GetPointData()->AddArray(newScalars);
					//myPD->GetPointData()->SetActiveScalars("TMP");


					vtkSmartPointer<vtkThreshold> selector =
						vtkSmartPointer<vtkThreshold>::New();
					vtkSmartPointer<vtkMaskFields> scalarsOff =
						vtkSmartPointer<vtkMaskFields>::New();
					vtkSmartPointer<vtkGeometryFilter> geometry =
						vtkSmartPointer<vtkGeometryFilter>::New();
					selector->SetInputData((vtkPolyData*)myPD);
					selector->SetInputArrayToProcess(0, 0, 0,
						vtkDataObject::FIELD_ASSOCIATION_CELLS,
						vtkDataSetAttributes::SCALARS);
					selector->SetAllScalars(1);
					selector->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "TMP");
					selector->ThresholdBetween(array_min, array_max);
					selector->Update();
					std::cout << "\n Extract_Scalar_Range new Number of points:" << selector->GetOutput()->GetNumberOfPoints() << std::endl;
					std::cout << "\nExtract_Scalar_Range new Number of cells:" << selector->GetOutput()->GetNumberOfCells() << std::endl;

					scalarsOff->SetInputData(selector->GetOutput());
					scalarsOff->CopyAttributeOff(vtkMaskFields::POINT_DATA, vtkDataSetAttributes::SCALARS);
					scalarsOff->CopyAttributeOff(vtkMaskFields::CELL_DATA, vtkDataSetAttributes::SCALARS);
					scalarsOff->Update();
					geometry->SetInputData(scalarsOff->GetOutput());
					geometry->Update();




					vtkSmartPointer<vtkPolyData> MyObj = vtkSmartPointer<vtkPolyData>::New();


					MyObj = geometry->GetOutput();
					myPD->GetPointData()->RemoveArray("TMP");
					MyObj->GetPointData()->RemoveArray("TMP");
					std::cout << "\nExtract array new Number of points:" << MyObj->GetNumberOfPoints() << std::endl;
					std::cout << "\nExtract array new Number of cells:" << MyObj->GetNumberOfCells() << std::endl;

					if (MyObj->GetNumberOfPoints() > 10)
					{
						VTK_CREATE(vtkMDActor, newactor);
						if (this->mui_BackfaceCulling == 0)
						{
							newactor->GetProperty()->BackfaceCullingOff();
						}
						else
						{
							newactor->GetProperty()->BackfaceCullingOn();
						}


						//VTK_CREATE(vtkDataSetMapper, newmapper);
						VTK_CREATE(vtkPolyDataMapper, newmapper);
						//VTK_CREATE(vtkSmartPointer<vtkDataSetMapper>
						//newmapper->ImmediateModeRenderingOn();
						newmapper->SetColorModeToDefault();

						if (
							(this->mui_ActiveArray->DataType == VTK_INT || this->mui_ActiveArray->DataType == VTK_UNSIGNED_INT)
							&& this->mui_ActiveArray->NumComp == 1
							)
						{
							newmapper->SetScalarRange(0, this->Getmui_ActiveTagMap()->numTags - 1);
							newmapper->SetLookupTable(this->Getmui_ActiveTagMap()->TagMap);

						}
						else
						{
							newmapper->SetLookupTable(this->Getmui_ActiveColorMap()->ColorMap);
						}


						newmapper->ScalarVisibilityOn();
						MyObj->GetPointData()->SetActiveScalars(Sauv_Active_Array_Name.toStdString().c_str());

						cout << "extract object" << MyObj->GetNumberOfPoints() << endl;
						//newmapper->SetInputConnection(delaunay3D->GetOutputPort());
						newmapper->SetInputData(MyObj);

						//VTK_CREATE(vtkActor, actor);

						int num = 2;

						vtkSmartPointer<vtkMatrix4x4> Mat = myActor->GetMatrix();
						vtkTransform *newTransform = vtkTransform::New();
						newTransform->PostMultiply();

						newTransform->SetMatrix(Mat);
						newactor->SetPosition(newTransform->GetPosition());
						newactor->SetScale(newTransform->GetScale());
						newactor->SetOrientation(newTransform->GetOrientation());
						newTransform->Delete();


						double color[4] = { 0.5, 0.5, 0.5, 1 };
						myActor->GetmColor(color);
						/*double trans = 100* color[3];
						if (trans >= 0 && trans <= 100)
						{
						color[3] = (double)((double)trans / 100);


						}*/

						newactor->SetmColor(color);

						newactor->SetMapper(newmapper);
						newactor->SetSelected(0);

						//std::string newname = this->CheckingName(myActor->GetName());
						if (array_min != array_max)
						{
							newactor->SetName(myActor->GetName() + "_ar");
						}
						else
						{
							newactor->SetName(myActor->GetName() + QString::number(array_min).toStdString());
						}
						//newactor->SetName(newname);
						cout << "try to add new actor=" << endl;
						newcoll->AddTmpItem(newactor);
						modified = 1;
					}

				}
			}
		}
	}
	
	if (modified == 1)
	{
		newcoll->InitTraversal();
		vtkIdType num = newcoll->GetNumberOfItems();
		for (vtkIdType i = 0; i < num; i++)
		{
			cout << "try to get next actor from newcoll:" << i << endl;
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(newcoll->GetNextActor());
			myActor->SetColorProperties(this->mui_Ambient, this->mui_Diffuse, this->mui_Specular, this->mui_SpecularPower);
			myActor->SetDisplayMode(this->mui_DisplayMode);
			this->getActorCollection()->AddItem(myActor);
			emit this->actorsMightHaveChanged();
			std::string action = "Object extracted from array: " + myActor->GetName();
			int mCount = BEGIN_UNDO_SET(action);
			this->getActorCollection()->CreateLoadUndoSet(mCount, 1);
			END_UNDO_SET();


		}
		//cout << "camera and grid adjusted" << endl;
		cout << "new actor(s) added" << endl;
		this->Initmui_ExistingArrays();
			
		cout << "Set actor collection changed" << endl;
		this->getActorCollection()->SetChanged(1);
		cout << "Actor collection changed" << endl;

		this
				
				
			->AdjustCameraAndGrid();
		cout << "Camera and grid adjusted" << endl;

		if (this->Getmui_AdjustLandmarkRenderingSize() == 1)
		{
			this->UpdateLandmarkSettings();
		}
		//this->Setmui_ActiveArray(myActiveArray->Name, myActiveArray->DataType, myActiveArray->NumComp);
		this->Render();
	}
	

}
void mqMorphoDigCore::Extract_Tag_Range(int tag_min, int tag_max)
{
	// checks wether active array is tag array... 	
	ActiveArray *myActiveArray = mqMorphoDigCore::instance()->Getmui_ActiveArray();
	int ok = 0;
	if ((myActiveArray->DataType == VTK_INT || myActiveArray->DataType == VTK_UNSIGNED_INT) && myActiveArray->NumComp == 1)
	{
		ok = 1;
	}
	if (ok == 0)
	{
		QMessageBox msgBox;
		msgBox.setText("A Tag array must be active to use this option.");
		msgBox.exec();
		return;
	}
	

	std::cout << "tag_min=" << tag_min << ", tag_max=" << tag_max << std::endl;
	
	this->Extract_Array_Range((double)tag_min, (double)tag_max);
	

}
void mqMorphoDigCore::Extract_Scalar_Range(double scalar_min, double scalar_max)
{
	// checks wether active array is scalar array... 	
	ActiveArray *myActiveArray = this->Getmui_ActiveArray();
	int ok = 0;
	if ((myActiveArray->DataType == VTK_FLOAT || myActiveArray->DataType == VTK_DOUBLE) && myActiveArray->NumComp == 1)
	{
		ok = 1;
	}
	if (ok == 0)
	{
		QMessageBox msgBox;
		msgBox.setText("A Scalar array must be active to use this option.");
		msgBox.exec();
		return;
	}


	std::cout << "scalar_min=" << scalar_min << ", scalar_max=" << scalar_max << std::endl;

	this->Extract_Array_Range(scalar_min, scalar_max);


}
void mqMorphoDigCore::TagAt(vtkIdType pickid, vtkMDActor *myActor, int toverride)
{

	cout << "Tag At " << pickid << endl;
	//1 check if myActor is not null
	//2 now check if tagModeActivated is on
	if (this->Getmui_TagModeActivated() == 1 && myActor !=NULL)
	{
		//3 if on, check if actor has a scalar list
		vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
		if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
		{
			QString sActiveArray = this->Getmui_ActiveArray()->Name;
			vtkIntArray *currentTags = vtkIntArray::SafeDownCast(mymapper->GetInput()->GetPointData()->GetScalars(sActiveArray.toStdString().c_str()));
			//4 if current tags exist, retrieve what tool is active
			if (currentTags != NULL)
			{
				std::string action = "Tag ";
				action.append(myActor->GetName().c_str());
				int Count = BEGIN_UNDO_SET(action);

				std::string mArrayName = sActiveArray.toStdString(); 

				myActor->SaveState(Count, QString(mArrayName.c_str()), 1);
				vtkPolyData *myPD = vtkPolyData::SafeDownCast(mymapper->GetInput());
				//todo : retrieve what tool is active
				//5 retrieve list of point ids which are selected by the currently active tool
				if (myActor->GetKdTree() == nullptr)
				{
					cout << "Try to build kdtree!" << endl;
					myActor->BuildKdTree();
					cout << "KdTree built" << endl;
				}
				if (myActor->GetConnectivityFilter() == nullptr)
				{
					cout << "Try to build connectivity filter!" << endl;
					myActor->BuildConnectivityFilter();
					cout << "Connectivity filter built" << endl;
				}
				double ve[3];

				//double vn[3];
				myPD->GetPoint(pickid, ve);
				double *vn;
				//vn = norms->GetTuple((vtkIdType)i);
				vtkSmartPointer<vtkFloatArray> norms = myActor->GetPointNormals();
				vn= norms->GetTuple(pickid);
				double mvn[3];
				mvn[0] = vn[0];
				mvn[1] = vn[1];
				mvn[2] = vn[2];
				int curTag = currentTags->GetTuple1(pickid);
				cout << "pickid :" << pickid << ", tagid:" << curTag << endl;
				cout << "vn[0] :" << vn[0] << ", vn[1]:" << vn[1] << ", vn[2]:" << vn[2] << endl;
				int do_override;
				
				if (toverride==1) // if user explicitly asks to override OR current picked vertex is tagged with 0 (exterior), we do the override
				{
					do_override = 1;
				}
				else
				{
					do_override = 0;
				}

				int tool = this->Getmui_TagTool();
				cout << "Tool=" << tool<<endl;
				int activeTag = this->Getmui_ActiveTag();
				if (tool == 0) // pencil
				{
					

					vtkSmartPointer<vtkIdList> observedNeighbours = vtkSmartPointer<vtkIdList>::New();
					double Radius = this->GetHundredPxSU()*this->Getmui_PencilSize()/100;
					cout <<"Radius = " << Radius << endl;
					myActor->GetKdTree()->FindPointsWithinRadius(Radius, ve, observedNeighbours);
					
					// two cases:
					// 1 angle limit = 180°
					// in that cases the observed Neighbours I am interested in is the WHOLE list
					// 1 angle limit < 180°
					// in that case propagate from picked_id through observedneighbours to construct a new vtkIdList
					vtkSmartPointer<vtkPolyData> mesh = vtkPolyData::SafeDownCast(myActor->GetMapper()->GetInput());
					
					vtkSmartPointer<vtkIdList> ids =
						vtkSmartPointer<vtkIdList>::New();
					
					
					double min_cos = cos((double)(this->Getmui_PencilLimitAngle())*vtkMath::Pi() / 180);
					
					
					if (this->Getmui_PencilContiguous() == 0) // no propagation: tag all vertices matching angle condition within radius
					{
						if (this->Getmui_PencilLimitAngle() < 180)
						{
							cout << "Min cos = " << min_cos << endl;
							for (vtkIdType j = 0; j < observedNeighbours->GetNumberOfIds(); j++)
							{
								vtkIdType investigatedPt = observedNeighbours->GetId(j);								
								double *vn2;
								vn2 = norms->GetTuple(investigatedPt);
								double curr_cos = mvn[0] * vn2[0] + mvn[1] * vn2[1] + mvn[2] * vn2[2];
								if (curr_cos > min_cos)
								{

									ids->InsertNextId(investigatedPt);
								}

							}
						}
						else
						{
							cout << "Ids = observedNeighbours" << endl;
							ids = observedNeighbours;
						}

						
					}
					else
					{
						vtkSmartPointer<vtkIdList> toInvestigatePtsList =
							vtkSmartPointer<vtkIdList>::New();
						vtkSmartPointer<vtkIdList> alreadyinvestigatedPtsList =
							vtkSmartPointer<vtkIdList>::New();
						
						toInvestigatePtsList->InsertNextId(pickid);
						int cpt = 0;
						int list_changed = 1;
						while (list_changed == 1)
						{
						//	cout << "cpt=" << cpt << endl;
						//	cout << "observedNeighbours->GetNumberOfIds()=" << observedNeighbours->GetNumberOfIds() << endl;
						//	cout << "ids->GetNumberOfIds()=" << ids->GetNumberOfIds() << endl;
						//	cout << "toInvestigatePtsList->GetNumberOfIds()=" << toInvestigatePtsList->GetNumberOfIds() << endl;
						//	cout << "min_cos=" << min_cos << endl;

							this->PropagateVertices(
								mesh,
								norms,
								mvn,
								min_cos,
								observedNeighbours,
								ids,
								toInvestigatePtsList,
								alreadyinvestigatedPtsList,
								&list_changed
							);
							
							cpt++;
							std::cout << "Tag magic wand level " << cpt << ": ids=" << ids->GetNumberOfIds() << std::endl;
						}

						
						//cout << " ids->GetNumberOfIds()=" << ids->GetNumberOfIds() << endl;

					}

					// mismatch observedneighbours
					for (vtkIdType j = 0; j < ids->GetNumberOfIds(); j++)
					{

						
						vtkIdType observedConnectedVertex = ids->GetId(j);

						int mTag = currentTags->GetTuple1(observedConnectedVertex);
						if (j < 10) {
							std::cout << "neighbour id: " << observedConnectedVertex << ", tagid:" << mTag << endl;


						}
						if (do_override == 1 || (do_override == 0 && mTag == curTag))
						{
							if (j < 10)
							{
								cout << "change tag value" << endl;
							}
							currentTags->SetTuple1(observedConnectedVertex, activeTag);
						}

					}
				}
				else// paint bucket
				{
					cout << "Start non pencil Tool=" << tool << endl;
					vtkIdType corrpickedId = myActor->GetCorrPickedId(pickid);
					int pickedRegion = myActor->GetConnectivityRegions()->GetTuple1(corrpickedId);
					cout << "picked region:" << pickedRegion << endl;
					cout << "small check: " << myPD->GetNumberOfPoints() << "=" << myActor->GetConnectivityRegions()->GetNumberOfTuples() << "?" << endl;
					for (vtkIdType j = 0; j < myPD->GetNumberOfPoints(); j++)
					{
						vtkIdType corrj = myActor->GetCorrPickedId(j);
						int jRegion = myActor->GetConnectivityRegions()->GetTuple1(corrj);
						
						if (jRegion == pickedRegion)
						{
							int mTag = currentTags->GetTuple1(j);

							if (do_override == 1 || (do_override == 0 && mTag == curTag))
							{
								currentTags->SetTuple1(j, activeTag);
								

								
							}
						}
					}
				
				}
				//mymapper->GetLookupTable()->
				currentTags->Modified();
				//mymapper->Update();
				END_UNDO_SET();
			}
			else
			{
				cout << "Can not tag, could not find tags" << endl;
			}
			
			
			//6 update list depending on override on/off and of color of active tag.
			//7 retrieve currently active tag number
			//8 update activeTag array with active tag number for all selected point ids.
		}
		else
		{
			cout << "Can not tag, no mapper / polydata" << endl;
		}
	}
	else
	{
		cout << "Can not tag : Tag Mode is deactivated" << endl;
	}


}

int mqMorphoDigCore::Getmui_TagModeActivated() {
	return this->mui_TagModeActivated;

}

void mqMorphoDigCore::ChangeClippingPlane() 
{
	if (this->mui_ClippingPlane == 0) { this->mui_ClippingPlane = 1; }
	else{ this->mui_ClippingPlane = 0; }
	this->ActivateClippingPlane();

}
int mqMorphoDigCore::Getmui_ClippinPlane() 
{
	return this->mui_ClippingPlane;
}
void mqMorphoDigCore::Setmui_ClippinPlane(int on_off) 
{
	if (on_off == 0 || on_off == 1) { this->mui_ClippingPlane = on_off; }
}

vtkSmartPointer<vtkDiscretizableColorTransferFunction> mqMorphoDigCore::GetScalarRainbowLut()
{
	return this->ScalarRainbowLut;
}
vtkSmartPointer<vtkDiscretizableColorTransferFunction> mqMorphoDigCore::GetScalarRedLut()
{
	return this->ScalarRedLut;
}
double mqMorphoDigCore::GetVolumeRangeMin()
{

	//return this->ScalarRangeMin;
	double my_min;
	double my_currmin;

	my_min = DBL_MAX;
	my_currmin = DBL_MAX;

	this->VolumeCollection->InitTraversal();

	for (vtkIdType i = 0; i < this->VolumeCollection->GetNumberOfItems(); i++)
	{
		vtkMDVolume * myVolume = vtkMDVolume::SafeDownCast(this->VolumeCollection->GetNextVolume());
		my_currmin = myVolume->GetImageData()->GetScalarRange()[0];
		if (my_currmin < my_min)
		{
			my_min = my_currmin;
		}
	}
	if (my_min == VTK_DOUBLE_MAX )
	{
		cout << "Strange!!!" << endl;
		return 0;
	}
	else
	{
		return my_min;
	}
}
double mqMorphoDigCore::GetVolumeRangeMax()
{

	//return this->ScalarRangeMin;
	double my_max;
	double my_currmax;

	my_max = -DBL_MAX;
	my_currmax = -DBL_MAX;

	this->VolumeCollection->InitTraversal();

	for (vtkIdType i = 0; i < this->VolumeCollection->GetNumberOfItems(); i++)
	{
		vtkMDVolume * myVolume = vtkMDVolume::SafeDownCast(this->VolumeCollection->GetNextVolume());
		my_currmax = myVolume->GetImageData()->GetScalarRange()[1];
		if (my_currmax > my_max)
		{
			my_max = my_currmax;
		}
	}
	if (my_max == VTK_DOUBLE_MIN)
	{
		cout << "Strange!!!" << endl;
		return 1;
	}
	else
	{
		return my_max;
	}
}
double mqMorphoDigCore::GetScalarRangeMin()
{

	//return this->ScalarRangeMin;
	double my_min;
	double my_currmin;

	my_min = DBL_MAX;
	my_currmin = DBL_MAX;

	this->ActorCollection->InitTraversal();

	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMDActor * myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		vtkPolyDataMapper *mapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());

		if (mapper != NULL && vtkPolyData::SafeDownCast(mapper->GetInput()) != NULL)
		{
			vtkPolyData *myPD = vtkPolyData::SafeDownCast(mapper->GetInput());
			if (myPD->GetPointData()->GetScalars(this->Getmui_ActiveArray()->Name.toStdString().c_str()) != NULL
				&& (
					this->Getmui_ActiveArray()->DataType == VTK_FLOAT ||
					this->Getmui_ActiveArray()->DataType == VTK_DOUBLE

					)
				&& this->Getmui_ActiveArray()->NumComp == 1
				)
			{
				if (this->Getmui_ActiveArray()->DataType == VTK_FLOAT)
				{
					vtkFloatArray *currentScalars = (vtkFloatArray*)myPD->GetPointData()->GetScalars(this->Getmui_ActiveArray()->Name.toStdString().c_str());
					if (currentScalars != NULL)
					{
						my_currmin = DBL_MAX;
						my_currmin = (double)currentScalars->GetTuple(0)[0];


						std::vector<float> vals;
						for (int i = 0; i < myPD->GetNumberOfPoints(); i++)
						{
							vals.push_back(currentScalars->GetTuple(i)[0]);
						}

						std::sort(vals.begin(), vals.end());

						//int iQ = (int)(0.05*myPD->GetNumberOfPoints());
						my_currmin = (double)vals.at(0);

						if (my_currmin < my_min) { my_min = my_currmin; }
					}
				}
				else
				{
					vtkDoubleArray *currentScalars = (vtkDoubleArray*)myPD->GetPointData()->GetScalars(this->Getmui_ActiveArray()->Name.toStdString().c_str());
					if (currentScalars != NULL)
					{
						my_currmin = DBL_MAX;
						my_currmin = currentScalars->GetTuple(0)[0];


						std::vector<double> vals;
						for (int i = 0; i < myPD->GetNumberOfPoints(); i++)
						{
							vals.push_back(currentScalars->GetTuple(i)[0]);
						}

						std::sort(vals.begin(), vals.end());

						//int iQ = (int)(0.05*myPD->GetNumberOfPoints());
						my_currmin = vals.at(0);

						if (my_currmin < my_min) { my_min = my_currmin; }
					}

				}

			}
		}
	}
	if (my_min == VTK_DOUBLE_MAX || my_min == VTK_FLOAT_MAX)
	{
		cout << "Strange!!!" << endl;
		return 0;
	}
	else
	{
		return my_min;
	}
}

int mqMorphoDigCore::GetTagRangeMin(QString TagArray)
{
	// A GLOBAL range min (for all actors)
	// warning: calls a Ini Traversal...
		QString mTagArray;
	if (TagArray.length() == 0)
	{
		mTagArray = this->Getmui_ActiveArray()->Name; // we hope this active scalar is a Tag one...

	}
	else
	{
		mTagArray = TagArray;
	}

	//return this->ScalarRangeMin;
	int my_min;
	int my_currmin;

	my_min = INT_MAX;
	my_currmin = INT_MAX;

	this->ActorCollection->InitTraversal();

	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMDActor * myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		vtkPolyDataMapper *mapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());

		if (mapper != NULL && vtkPolyData::SafeDownCast(mapper->GetInput()) != NULL)
		{
			vtkPolyData *myPD = vtkPolyData::SafeDownCast(mapper->GetInput());
			
				vtkIntArray *currentTags = vtkIntArray::SafeDownCast(myPD->GetPointData()->GetScalars(mTagArray.toStdString().c_str()));
					if (currentTags != NULL)
					{
						my_currmin = INT_MAX;
						my_currmin = currentTags->GetTuple(0)[0];


						std::vector<int> vals;
						for (int i = 0; i < myPD->GetNumberOfPoints(); i++)
						{
							vals.push_back(currentTags->GetTuple(i)[0]);
						}

						std::sort(vals.begin(), vals.end());

						//int iQ = (int)(0.05*myPD->GetNumberOfPoints());
						my_currmin = (int)vals.at(0);

						if (my_currmin < my_min) { my_min = my_currmin; }
					}
				
				

		}
		
	}
	if (my_min == VTK_INT_MAX)
	{
		cout << "Strange!!!" << endl;
		return 0;
	}
	else
	{
		return my_min;
	}
}
int mqMorphoDigCore::GetTagRangeMax(QString TagArray)
{ 
	// A GLOBAL range max (for all actors)
	//warning: calls a Ini Traversal...
	QString mTagArray;
	if (TagArray == "")
	{
		mTagArray = this->Getmui_ActiveArray()->Name; // we hope this active scalar is a Tag one...
		
	}
	else
	{
		mTagArray = TagArray;
	}
	cout << "mTagArray=" << mTagArray.toStdString() << endl;
	//return this->ScalarRangeMin;
	int my_max;
	int my_currmax;

	my_max = 0;
	my_currmax = 0;

	this->ActorCollection->InitTraversal();

	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMDActor * myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		vtkPolyDataMapper *mapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());

		if (mapper != NULL && vtkPolyData::SafeDownCast(mapper->GetInput()) != NULL)
		{
			vtkPolyData *myPD = vtkPolyData::SafeDownCast(mapper->GetInput());
			
				vtkIntArray *currentTags = vtkIntArray::SafeDownCast(myPD->GetPointData()->GetScalars(mTagArray.toStdString().c_str()));
				if (currentTags != NULL)
				{
					my_currmax = 0;
					my_currmax = currentTags->GetTuple(0)[0];


					std::vector<int> vals;
					for (int i = 0; i < myPD->GetNumberOfPoints(); i++)
					{
						vals.push_back(currentTags->GetTuple(i)[0]);
					}

					std::sort(vals.begin(), vals.end());

					//int iQ = (int)(0.05*myPD->GetNumberOfPoints());
					my_currmax= (int)vals.at(myPD->GetNumberOfPoints()-1);

					if (my_currmax > my_max) { my_max = my_currmax; }
				}
			


		}

	}
	if (my_max == 0)
	{
		cout << "Odd... Tag max =0" << endl;
		return 0;
	}
	else
	{
		return my_max;
	}
}

double mqMorphoDigCore::GetScalarRangeMax()
{

	//return this->ScalarRangeMax;
	double my_max;
	double my_currmax;

	my_max = -DBL_MAX;

	my_currmax = -DBL_MAX;

	this->ActorCollection->InitTraversal();

	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMDActor * myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		vtkPolyDataMapper *mapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());

		if (mapper != NULL && vtkPolyData::SafeDownCast(mapper->GetInput()) != NULL)
		{
			vtkPolyData *myPD = vtkPolyData::SafeDownCast(mapper->GetInput());
			if (myPD->GetPointData()->GetScalars(this->Getmui_ActiveArray()->Name.toStdString().c_str()) != NULL
				&& (
					this->Getmui_ActiveArray()->DataType == VTK_FLOAT ||
					this->Getmui_ActiveArray()->DataType == VTK_DOUBLE

					)
				&& this->Getmui_ActiveArray()->NumComp == 1
				)
			{
				if (this->Getmui_ActiveArray()->DataType == VTK_FLOAT)
				{
					vtkFloatArray *currentScalars = (vtkFloatArray*)myPD->GetPointData()->GetScalars(this->Getmui_ActiveArray()->Name.toStdString().c_str());
					if (currentScalars != NULL)
					{

						my_currmax = (double)currentScalars->GetTuple(0)[0];

						std::vector<float> vals;
						for (int i = 0; i < myPD->GetNumberOfPoints(); i++)
						{
							vals.push_back(currentScalars->GetTuple(i)[0]);
						}

						std::sort(vals.begin(), vals.end());

						int iQ = (int)(myPD->GetNumberOfPoints()-1);

						my_currmax = (double)vals.at(iQ);
						if (my_currmax > my_max) { my_max = my_currmax; }

					}
				}
				else
				{
					vtkDoubleArray *currentScalars = (vtkDoubleArray*)myPD->GetPointData()->GetScalars(this->Getmui_ActiveArray()->Name.toStdString().c_str());
					if (currentScalars != NULL)
					{

						my_currmax = currentScalars->GetTuple(0)[0];


						std::vector<double> vals;
						for (int i = 0; i < myPD->GetNumberOfPoints(); i++)
						{
							vals.push_back(currentScalars->GetTuple(i)[0]);
						}

						std::sort(vals.begin(), vals.end());

						int iQ = (int)(myPD->GetNumberOfPoints()-1);
						my_currmax = vals.at(iQ);

						if (my_currmax > my_max) { my_max = my_currmax; }
					}

				}

			}
		}
	}
	if (my_max == VTK_DOUBLE_MIN || my_max == VTK_FLOAT_MIN)
	{
		return 1;
	}
	else
	{
		cout << "my_max=" << my_max << endl;
		return my_max;
	}
}
double mqMorphoDigCore::GetSuggestedVolumeRangeMin(int cutMin, int onlyselected )
{
	// cutMin (is a percentage, between 0 and 50).
		// get scalar range "cutMin"% smallest value 
		// example1: cutMin = 0 => will return minimal scalar value (for all voxels of all opened volumes or for only selected volumes, depending on onlyselected 0,1)
		// example2 : cutMin = 5 => will return rangeMin + 5% * (rangeMax - rangeMin)
		// here we do not allow cutMin to exceed 50% because this function is usually used in association with GetSuggestedVolumeRangeMax, and we do not want the returned value to be greater than GetSuggestedVolumeRangeMax.
	double my_min, my_max;
	double my_currmin, my_currmax;

	my_min = DBL_MAX;
	my_max = -DBL_MAX;
	my_currmin = DBL_MAX;
	my_currmax = -DBL_MAX;
	double suggested_min = 0;
	int totalVol = 0;
	this->VolumeCollection->InitTraversal();
		
	for (vtkIdType i = 0; i < this->VolumeCollection->GetNumberOfItems(); i++)
	{
		vtkMDVolume * myVolume = vtkMDVolume::SafeDownCast(this->VolumeCollection->GetNextVolume());
		vtkSmartPointer<vtkImageData> data = myVolume->GetImageData();  


		if (onlyselected == 0 || myVolume->GetSelected() == 1)
		{
			 totalVol++;
			 my_currmin = data->GetScalarRange()[0];
			 my_currmax = data->GetScalarRange()[1];
			 if (my_currmin < my_min) { my_min = my_currmin; }
			 if (my_currmax > my_max) { my_max = my_currmax; }

		}
	}
	

	if (totalVol > 0)
	{
		
		double cut = 0;
		if (cutMin >= 0 && cutMin < 50)
		{
			cut = (double)cutMin;
			cut /= 100;
		}

		suggested_min = my_min + cut * (my_max - my_min);
		
	}
	if (suggested_min == VTK_DOUBLE_MAX || suggested_min == VTK_FLOAT_MAX)
	{
		cout << "Strange!!!" << endl;
		return 0;
	}
	else
	{
		return suggested_min;
	}
}

double mqMorphoDigCore::GetSuggestedVolumeRangeMax(int cutMax, int onlyselected)
{
	// cutMax (is a percentage, between 0 and 50).
		// get scalar range "cutMax"% highest value 
		// example1: cutMax = 0 => will return maximal scalar value (for all voxels of all opened volumes or for only selected volumes, depending on onlyselected 0,1)
		// example2 : cutMax = 5 => will return rangeMax - 5% * (rangeMax - rangeMin)
		// here we do not allow cutMin to exceed 50% because this function is usually used in association with GetSuggestedVolumeRangeMin, and we do not want the returned value to be smaller than GetSuggestedVolumeRangeMin.
	double my_min, my_max;
	double my_currmin, my_currmax;

	my_min = DBL_MAX;
	my_max = -DBL_MAX;
	my_currmin = DBL_MAX;
	my_currmax = -DBL_MAX;
	double suggested_max = 1;
	int totalVol = 0;
	this->VolumeCollection->InitTraversal();
	
	for (vtkIdType i = 0; i < this->VolumeCollection->GetNumberOfItems(); i++)
	{
		vtkMDVolume * myVolume = vtkMDVolume::SafeDownCast(this->VolumeCollection->GetNextVolume());
		vtkSmartPointer<vtkImageData> data = myVolume->GetImageData();


		if (onlyselected == 0 || myVolume->GetSelected() == 1)
		{
			totalVol++;
			my_currmin = data->GetScalarRange()[0];
			my_currmax = data->GetScalarRange()[1];
			if (my_currmin < my_min) { my_min = my_currmin; }
			if (my_currmax > my_max) { my_max = my_currmax; }

		}
		
	}


	if (totalVol > 0)
	{

		double cut = 0;
		if (cutMax >= 0 && cutMax < 50)
		{
			cut = (double)cutMax;
			cut /= 100;
		}

		suggested_max = my_max - cut * (my_max - my_min);

	}
	if (suggested_max == VTK_DOUBLE_MIN || suggested_max == VTK_FLOAT_MIN)
	{
		cout << "Strange!!!" << endl;
		return 1;
	}
	else
	{
		return suggested_max;
	}
}


double mqMorphoDigCore::GetSuggestedScalarRangeMin(int cutMin, int onlyselected)
{
	// cutMin (is a percentage, between 0 and 50).
	// sorts scalar and takes the "cutMin"% smallest value 
	// example1: cutMin = 0 => will return minimal scalar value (for all vertices of all opened objects)
	// example2 : cutMin = 5 => will return scalar's Q5 (5% of the vertices have scalar values smaller than the returned value).
	// here we do not allow cutMin to exceed 50% because this function is usually used in association with GetSuggestedScalarRangeMax, and we do not want the returned value to be greater than GetSuggestedScalarRangeMax.
	double my_min;
	double my_currmin;

	my_min = DBL_MAX;
	my_currmin = DBL_MAX;

	this->ActorCollection->InitTraversal();
	vtkIdType totalVeNr = 0;
	std::vector<double> vals;
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMDActor * myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		vtkPolyDataMapper *mapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());

		if (mapper != NULL && vtkPolyData::SafeDownCast(mapper->GetInput()) != NULL && (onlyselected==0 || myActor->GetSelected()==1))
		{
			vtkPolyData *myPD = vtkPolyData::SafeDownCast(mapper->GetInput());
			if (myPD->GetPointData()->GetScalars(this->Getmui_ActiveArray()->Name.toStdString().c_str()) != NULL
				&& (
					this->Getmui_ActiveArray()->DataType == VTK_FLOAT ||
					this->Getmui_ActiveArray()->DataType == VTK_DOUBLE

					)
				&& this->Getmui_ActiveArray()->NumComp == 1
				)
			{
				
				vtkDataArray *currentScalars=NULL;
				currentScalars = vtkDoubleArray::SafeDownCast(myPD->GetPointData()->GetScalars(this->Getmui_ActiveArray()->Name.toStdString().c_str()));
				if (currentScalars == NULL)
				{
					currentScalars = vtkFloatArray::SafeDownCast(myPD->GetPointData()->GetScalars(this->Getmui_ActiveArray()->Name.toStdString().c_str()));

				}

				if (currentScalars != NULL)
				{
					totalVeNr += myPD->GetNumberOfPoints();
					
					for (int i = 0; i < myPD->GetNumberOfPoints(); i++)
					{
						vals.push_back((double)currentScalars->GetTuple(i)[0]);
					}
						
				}
			}				

		}
	}
	cout << "try to sort vals Min at cutMin=" <<cutMin<< endl;
	cout << "totalVeNr==" << totalVeNr << endl;
	cout << "vals nr of values:" << vals.size() << endl;

	if (totalVeNr > 0)
	{
		std::sort(vals.begin(), vals.end());
		double cut = 0;
		if (cutMin >= 0 && cutMin < 50)
		{
			cut = (double)cutMin;
			cut /= 100;
		}

		size_t iQ = (size_t)(cut*totalVeNr);
		if (iQ >= vals.size()) { iQ = vals.size() - 1; }
		if (iQ < 0) { iQ = 0; }
		cout << "iQ=" << iQ << endl;
		my_currmin = (double)vals.at(iQ);
		cout << "my_currmin=" << my_currmin << endl;
		if (my_currmin < my_min) { my_min = my_currmin; }
	}
	if (my_min == VTK_DOUBLE_MAX || my_min == VTK_FLOAT_MAX)
	{
		cout << "Strange!!!" << endl;
		return 0;
	}
	else
	{
		return my_min;
	}

}

double mqMorphoDigCore::GetSuggestedScalarRangeMax( int cutMax, int onlyselected)
{
	// cutMax (is a percentage, between 0 and 50).
	// sorts scalar and takes the "cutMax"% maximal value
	// example1: cutMax = 0 => will return maximal scalar value (for all vertices of all opened objects)
	// example2 : cutMin = 5 => will return scalar's Q95 (only 5% of the vertices have scalar values higher than the returned value).
	// here we do not allow cutMax to exceed 50% because this function is usually used in association with GetSuggestedScalarRangeMin, and we do not want the returned value to be smaller than GetSuggestedScalarRangeMin.

	double my_max;
	double my_currmax;

	my_max = -DBL_MAX;
	
	my_currmax = -DBL_MAX;

	this->ActorCollection->InitTraversal();

	vtkIdType totalVeNr = 0;
	std::vector<double> vals;
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMDActor * myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		vtkPolyDataMapper *mapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());

		if (mapper != NULL && vtkPolyData::SafeDownCast(mapper->GetInput()) != NULL && (onlyselected == 0 || myActor->GetSelected() == 1))
		{
			vtkPolyData *myPD =vtkPolyData::SafeDownCast(mapper->GetInput());
			if (myPD->GetPointData()->GetScalars(this->Getmui_ActiveArray()->Name.toStdString().c_str()) != NULL
				&& (
					this->Getmui_ActiveArray()->DataType ==VTK_FLOAT ||
					this->Getmui_ActiveArray()->DataType == VTK_DOUBLE

					)
				&& this->Getmui_ActiveArray()->NumComp == 1
				)
			{

				vtkDataArray *currentScalars = NULL;
				currentScalars = vtkDoubleArray::SafeDownCast(myPD->GetPointData()->GetScalars(this->Getmui_ActiveArray()->Name.toStdString().c_str()));
				if (currentScalars == NULL)
				{
					currentScalars = vtkFloatArray::SafeDownCast(myPD->GetPointData()->GetScalars(this->Getmui_ActiveArray()->Name.toStdString().c_str()));

				}

				if (currentScalars != NULL)
				{
						totalVeNr += myPD->GetNumberOfPoints();											
						for (int i = 0; i < myPD->GetNumberOfPoints(); i++)
						{
							vals.push_back((double)currentScalars->GetTuple(i)[0]);
						}
					
						
					}
				}		
		}
									
	}
	cout << "try to sort vals Max at cutMax=" << cutMax << endl;
	cout << "totalVeNr==" << totalVeNr << endl;
	cout << "vals nr of values:" << vals.size() << endl;

	if (totalVeNr > 0)
	{
		std::sort(vals.begin(), vals.end());
		double cut = 1;
		if (cutMax >= 0 && cutMax < 50)
		{
			cut = 100 - (double)cutMax;
			cut /= 100;
		}
		size_t iQ = (size_t)(cut*totalVeNr);
		if (iQ >= vals.size()) { iQ = vals.size() - 1; }
		if (iQ < 0) { iQ = 0; }
		cout << "iQ=" << iQ << endl;
		my_currmax = (double)vals.at(iQ);
		cout << "my_currmax=" << my_currmax << endl;
		if (my_currmax > my_max) { my_max = my_currmax; }
	}
	if (my_max == VTK_DOUBLE_MIN || my_max == VTK_FLOAT_MIN)
	{
		return 1;
	}
	else 
	{
		cout << "suggested range my_max=" << my_max << endl;
		return my_max;
	}
	
	
}

vtkDiscretizableColorTransferFunction* mqMorphoDigCore::GetOneColorMap()
{
	if (this->mui_ExistingColorMaps->Stack.size() > 0)
	{
		cout <<"GetOneColorMap returns "<< this->mui_ExistingColorMaps->Stack.at(0).Name.toStdString()<< endl;
		return this->mui_ExistingColorMaps->Stack.at(0).ColorMap;
	}
	else
	{
		cout << "GetOneColorMap returns NULL" << endl;
		return NULL;
	}

}
void mqMorphoDigCore::UpdateLookupTablesRanges(double min, double max)
{
	cout << "UpdateLookupTablesRanges inside MqMorphoDigCore" << endl;
	for (int i = 0; i < this->mui_ExistingColorMaps->Stack.size(); i++)
	{
		vtkSmartPointer<vtkDiscretizableColorTransferFunction> CM = this->mui_ExistingColorMaps->Stack.at(i).ColorMap;
		
		double *pts = CM->GetDataPointer();
		
		int numnodes = CM->GetSize();
		cout << this->mui_ExistingColorMaps->Stack.at(i).Name.toStdString() << ": num nodes = " << numnodes << endl;
		double old_min = DBL_MAX;
		double old_max = -DBL_MAX;
		for (int j = 0; j < numnodes; j++)
		{
			double curr = pts[4 * j];
			//cout << "x" << j << "=" << curr << endl;
			if (curr < old_min) { old_min = curr; }
			if (curr > old_max) { old_max = curr; }

		}
		//cout << "old max:" << old_max << ", old min:" << old_min << endl;
		if (old_max > old_min)
		{
			double old_range = old_max - old_min;
			double new_range = max - min;
			double mult = new_range / old_range;
			double c = min - old_min*mult;
			for (int k = 0; k < numnodes; k++)
			{
				pts[4 * k] = pts[4 * k] * mult + c;
				//cout << "nx" << k << "=" << pts[4 * k] << endl;
			}
			CM->FillFromDataPointer(numnodes, pts);

		}
		vtkPiecewiseFunction* OF = CM->GetScalarOpacityFunction();
		int numnodes2 = OF->GetSize();
		double *pts2 = OF->GetDataPointer();
		//cout << this->mui_ExistingColorMaps->Stack.at(i).Name.toStdString() << ": OF num nodes = " << numnodes2 << endl;
		double old_min2 = DBL_MAX;
		double old_max2 = -DBL_MAX;
		for (int j = 0; j < numnodes2; j++)
		{
			double curr = pts2[2*j];
			//cout << "x" << j << "=" << curr << endl;
			if (curr < old_min2) { old_min2 = curr; }
			if (curr > old_max2) { old_max2 = curr; }

		}
		if (old_max2 > old_min2)
		{
			double old_range = old_max2 - old_min2;
			double new_range = max - min;
			double mult = new_range / old_range;
			double c = min - old_min2*mult;
			for (int k = 0; k < numnodes2; k++)
			{
				pts2[2*k] = pts2[2*k] * mult + c;
				//cout << "nx" << k << "=" << pts2[2*k] << endl;
			}
			OF->FillFromDataPointer(numnodes2, pts2);

		}
	}
	this->Render();

}
void mqMorphoDigCore::UpdateLookupTablesToData()
{
	
	double min; double max;
	min = DBL_MAX;
	max = -DBL_MAX;
	
	min = this->GetScalarRangeMin();
	cout << "min=" << min << endl;
	max = this->GetScalarRangeMax();
	
	cout << "max=" << max << endl;
	if (min <DBL_MAX && max>-DBL_MAX && min < max)
	{
		cout << "Update look up tables ranges"  << endl;
		this->UpdateLookupTablesRanges(min, max);
	}
	

}
void mqMorphoDigCore::createCustomTagMap(QString name)
{
	//copy Active Tag Maps inside a new tag map.

	vtkSmartPointer<vtkLookupTable> newTagMap = vtkSmartPointer<vtkLookupTable>::New();
	newTagMap->DeepCopy(this->Getmui_ActiveTagMap()->TagMap);

	std::vector<std::string> tagNames = this->Getmui_ActiveTagMap()->tagNames;
	int numTags = this->Getmui_ActiveTagMap()->numTags;


	this->mui_ActiveTagMap->TagMap= newTagMap;
	this->mui_ActiveTagMap->numTags= numTags;
	this->mui_ActiveTagMap->tagNames = tagNames;
	this->mui_ActiveTagMap->Name = name;	

	cout << "Add this map to !! create custom tag map" << endl;
	this->mui_ExistingTagMaps->Stack.push_back(ExistingTagMaps::Element(name, newTagMap, numTags, tagNames, 1));
	emit this->tagMapsChanged();

}
void mqMorphoDigCore::createCustomColorMap(QString name, vtkDiscretizableColorTransferFunction *STC)
{
//@@ TO DO!
	vtkSmartPointer<vtkDiscretizableColorTransferFunction> newSTC = vtkSmartPointer<vtkDiscretizableColorTransferFunction>::New();
	newSTC->DeepCopy(STC);
	

	vtkSmartPointer<vtkPiecewiseFunction> opacityfunction = vtkSmartPointer<vtkPiecewiseFunction>::New();
	opacityfunction->DeepCopy(STC->GetScalarOpacityFunction());
	

	newSTC->SetScalarOpacityFunction(opacityfunction);
	newSTC->Build();


	this->mui_ActiveColorMap->ColorMap = newSTC;
	cout << "Active color map = newSTC!" << endl;
	this->mui_ActiveColorMap->Name = name;

	cout << "Add this map to create custom colormap!!" << endl;
	this->mui_ExistingColorMaps->Stack.push_back(ExistingColorMaps::Element(name, newSTC, 1));

	emit colorMapsChanged();

}

void mqMorphoDigCore::invertRGB(vtkDiscretizableColorTransferFunction *STC)
{
	if (STC != NULL)
	{
		double *pts = STC->GetDataPointer();

		int numnodes = STC->GetSize();
		//cout << ": num nodes = " << numnodes << endl;
		double min = DBL_MAX;
		double max = -DBL_MAX;
		for (int j = 0; j < numnodes; j++)
		{
			double curr = pts[4 * j];
			//cout << "x" << j << "=" << curr << endl;
			if (curr < min) { min = curr; }
			if (curr > max) { max = curr; }

		}
		//cout << "max:" << max << ", old min:" << min << endl;
		if (max > min)
		{
			double mult = -1;
			double c = max+min;
			for (int k = 0; k < numnodes; k++)
			{
				pts[4 * k] = pts[4 * k] * mult + c;
				//cout << "nx" << k << "=" << pts[4 * k] << endl;
			}
			STC->FillFromDataPointer(numnodes, pts);

		}
	}
}
void mqMorphoDigCore::invertOpacity(vtkDiscretizableColorTransferFunction *STC)
{
	if (STC != NULL)
	{
		vtkPiecewiseFunction* OF = STC->GetScalarOpacityFunction();
		int numnodes2 = OF->GetSize();
		double *pts2 = OF->GetDataPointer();
		//cout << this->mui_ExistingColorMaps->Stack.at(i).Name.toStdString() << ": OF num nodes = " << numnodes2 << endl;
		double min2 = DBL_MAX;
		double max2 = -DBL_MAX;
		for (int j = 0; j < numnodes2; j++)
		{
			double curr = pts2[2 * j];
			//cout << "x" << j << "=" << curr << endl;
			if (curr < min2) { min2 = curr; }
			if (curr > max2) { max2 = curr; }

		}
		if (max2 > min2)
		{
			double mult = -1;
			double c = min2+max2;
			for (int k = 0; k < numnodes2; k++)
			{
				pts2[2 * k] = pts2[2 * k] * mult + c;
				//cout << "nx" << k << "=" << pts2[2*k] << endl;
			}
			OF->FillFromDataPointer(numnodes2, pts2);

		}
	}
}
void mqMorphoDigCore::InitLuts()
{
	cout << "Start Init LUTS!" << endl;
	/*this->TagLut->SetNumberOfTableValues(10);
	this->TagLut->Build();
	std::vector<std::string> tagNames;
	// Fill in a few known colors, the rest will be generated if needed

	//TagLut->SetTableValue(0, 0.66, 0.33, 1, 1);  //Violet
	TagLut->SetTableValue(0, 0.8, 0.8, 0.8, 1);  //Grey
	tagNames.push_back("Exterior");
	TagLut->SetTableValue(1, 0.8900, 0.8100, 0.3400, 1); // Banana
	tagNames.push_back("Tag1");
	TagLut->SetTableValue(2, 1.0000, 0.3882, 0.2784, 0.2); // Tomato
	tagNames.push_back("Tag2");
	TagLut->SetTableValue(3, 0.9608, 0.8706, 0.7020, 1); // Wheat
	tagNames.push_back("Tag3");
	TagLut->SetTableValue(4, 0.9020, 0.9020, 0.9804, 1); // Lavender
	tagNames.push_back("Tag4");
	TagLut->SetTableValue(5, 1.0000, 0.4900, 0.2500, 1); // Flesh
	tagNames.push_back("Tag5");
	TagLut->SetTableValue(6, 0.5300, 0.1500, 0.3400, 1); // Raspberry
	tagNames.push_back("Tag6");
	TagLut->SetTableValue(7, 0.9804, 0.5020, 0.4471, 1); // Salmon
	tagNames.push_back("Tag7");
	TagLut->SetTableValue(8, 0.7400, 0.9900, 0.7900, 1); // Mint
	tagNames.push_back("Tag8");
	TagLut->SetTableValue(9, 0.2000, 0.6300, 0.7900, 1);
	tagNames.push_back("Tag9");
	
	

	QString TagMap = QString("TagMap");
	cout << "Try to set existing color maps!!" << endl;

	this->mui_ExistingTagMaps->Stack.push_back(ExistingTagMaps::Element(TagMap, TagLut,10, tagNames, 0));
	this->mui_ActiveTagMap->TagMap= this->TagLut;
		cout << "Set Active color map2!" << endl;
	
	this->mui_ActiveTagMap->Name = TagMap;
	this->mui_ActiveTagMap->numTags = 10;
	this->mui_ActiveTagMap->tagNames = tagNames;
	*/
	this->ScalarRainbowLut->DiscretizeOff();
	this->ScalarRainbowLut->SetColorSpaceToRGB();
	//this->ScalarRainbowLut->EnableOpacityMappingOn();

	
	this->ScalarRainbowLut->AddRGBPoint(0.0, 1.0, 0.0, 1.0); //# purple
	this->ScalarRainbowLut->AddRGBPoint(0.2, 0.0, 0.0, 1.0); //# blue
	this->ScalarRainbowLut->AddRGBPoint(0.4, 0.0, 1.0, 1.0); //# purple
	this->ScalarRainbowLut->AddRGBPoint(0.6, 0.0, 1.0, 0.0); //# green
	this->ScalarRainbowLut->AddRGBPoint(0.8, 1.0, 1.0, 0.0); //# yellow
	this->ScalarRainbowLut->AddRGBPoint(1.0, 1.0, 0.0, 0.0); //# red
	this->ScalarRainbowLut->GetNumberOfValues();
	vtkSmartPointer<vtkPiecewiseFunction> opacityRfunction = vtkSmartPointer<vtkPiecewiseFunction>::New();
	
	opacityRfunction->AddPoint(0, 0);
	opacityRfunction->AddPoint(0.2, 0.6);
	opacityRfunction->AddPoint(0.8, 0.8);
	opacityRfunction->AddPoint(1, 1);
	//opacityRfunction->AddPoint(1.0001, 0);
	cout << "Rainbow scalar has opacity function!!!" << endl;
	this->ScalarRainbowLut->SetScalarOpacityFunction(opacityRfunction);
	this->ScalarRainbowLut->EnableOpacityMappingOn();
	this->ScalarRainbowLut->Build();
	
    this->ScalarRedLut->SetColorSpaceToRGB();
	this->ScalarRedLut->EnableOpacityMappingOn();
	/*TF->AddRGBPoint(first_point, 0, 0, 0, 0.5, 0);
			TF->AddRGBPoint(second_point, 0.73, 0, 0, 0.5, 0);
			TF->AddRGBPoint(third_point, .90, .82, .56, .5, 0);
			TF->AddRGBPoint(last_point, 1, 1, 1, .5, 0);*/
	this->ScalarRedLut->AddRGBPoint(0.0, 0.0, 0.0, 0.0); //# black
	this->ScalarRedLut->AddRGBPoint(0.2, 0.73, 0, 0.0); //# reddish
	this->ScalarRedLut->AddRGBPoint(0.4, 0.9, 0.82, 0.56);// # yellow
	this->ScalarRedLut->AddRGBPoint(1.0, 1.0, 1.0, 1.0);// # white

	// 	vtkPiecewiseFunction
	//cmap->SetScalarOpacityFunction();
	
	

	vtkSmartPointer<vtkPiecewiseFunction> opacityfunction =  vtkSmartPointer<vtkPiecewiseFunction>::New();
	

	opacityfunction->AddPoint(0, 0);
	opacityfunction->AddPoint(0.2, 0.5);
	opacityfunction->AddPoint(0.4, 0.8);
	opacityfunction->AddPoint(1, 1);
	
	this->ScalarRedLut->SetScalarOpacityFunction(opacityfunction);
	this->ScalarRedLut->Build();

	/*this->mui_ActiveColorMap->ColorMap = this->ScalarRedLut;
	this->mui_ActiveColorMap->Name = QString("Black-Red-White_Alpha");*/
	cout << "Set Active color map!" << endl;
	this->mui_ActiveColorMap->ColorMap = this->ScalarRainbowLut;
	cout << "Set Active color map2!" << endl;
	QString Rainbow = QString("Rainbow");
	this->mui_ActiveColorMap->Name = Rainbow;

	cout << "Try to set existing color maps!!" << endl;
	this->mui_ExistingColorMaps->Stack.push_back(ExistingColorMaps::Element(Rainbow, this->ScalarRainbowLut, 0));

	cout << "Try to set existing color maps 2!!" << endl;
	QString BRWA = QString("Black-Red-White");

	this->mui_ExistingColorMaps->Stack.push_back(ExistingColorMaps::Element(BRWA,this->ScalarRedLut, 0));
	cout << "Try to set existing color maps 3!!" << endl;


	vtkSmartPointer<vtkFloatArray> scalarValues = vtkSmartPointer<vtkFloatArray>::New();
	scalarValues->SetNumberOfComponents(1);
	scalarValues->SetNumberOfTuples(256);
	for (int i = 0; i < 256; i++)
	{

		const float val = (float)i / 255;
		//		scalarValues->SetTupleValue(i, &val);
		scalarValues->SetTypedTuple(i, &val);
	}
	
	//cmap->
	//cmap->MapScalars(scalarValues, 0, -1);
	//this->ScalarRedLut = vtkLookupTable::SafeDownCast(); 
	//this->ScalarRainbowLut = vtkLookupTable::SafeDownCast(cmap->MapScalars(scalarValues, 0, 1));
	
	/*for (int i = 0; i < 256; i++)
	{
		double* machin= scalarValues->GetTuple(i);
		//scalarValues->GetTupleGetTupleValue(i, &machin);

		//cout << "tuple i=" << machin[0] << endl;
	}*/

	/*
	
> for i in xrange(256) :
>     scalarValues.SetTupleValue(i, [i / 255.0])
>
> table = cmap.MapScalars(scalarValues, 0, -1)
>
> for i in xrange(table.GetNumberOfTuples()) :
>     print table.GetTuple(i)*/
	
	

}
void mqMorphoDigCore::deleteColorMap(int i)
{
	ExistingColorMaps *colorMaps = this->Getmui_ExistingColorMaps();
	size_t size = colorMaps->Stack.size();
	if (i < size && i>=0)
	{
		colorMaps->Stack.erase(colorMaps->Stack.begin() + i);
		if (colorMaps->Stack.size() > 0)
		{
			this->Setmui_ActiveColorMap(colorMaps->Stack.at(0).Name, colorMaps->Stack.at(0).ColorMap);
		}
	}
}


void mqMorphoDigCore::addTagToTagMap(int i) 
{
	ExistingTagMaps *tagMaps = this->Getmui_ExistingTagMaps();
	size_t size = tagMaps->Stack.size();
	if (i < size && i >= 0)
	{
		//to change if mod than one "preset" tagmap
		//only TagMap 1
		
		vtkSmartPointer<vtkLookupTable> mTagLut = vtkSmartPointer<vtkLookupTable>::New();

		mTagLut = this->Getmui_ActiveTagMap()->TagMap;
		int tagnr = mTagLut->GetNumberOfTableValues();
		
		int newtag = tagnr;
		tagnr++;
		mTagLut->Build();
		mTagLut->SetNumberOfTableValues(tagnr);
		//mTagLut->Build();
		double rgba[4];		
		this->GetDefaultTagColor(newtag, rgba);
		mTagLut->SetTableValue(newtag, rgba[0], rgba[1], rgba[2], rgba[3]);
		/*for (int i = 0; i < tagnr; i++)
		{
			double rgba[4];
			cout << "i=" << i << "try to get rgba" << endl;

			if (i == newtag)
			{
				this->GetDefaultTagColor(i, rgba);
			
			}
			
		}*/
		//mTagLut->SetTableValue(newtag, rgba);
		QString TagName = "Tag" + QString::number(newtag);								
		this->Getmui_ActiveTagMap()->tagNames.push_back(TagName.toStdString());
		//this->Getmui_ActiveTagMap()->numTags = tagnr;
		

		this->Getmui_ExistingTagMaps()->Stack.at(i).numTags = tagnr;
		this->Getmui_ExistingTagMaps()->Stack.at(i).tagNames = this->Getmui_ActiveTagMap()->tagNames;
		this->Getmui_ExistingTagMaps()->Stack.at(i).TagMap = mTagLut;

		this->Setmui_ActiveTagMap(this->Getmui_ActiveTagMap()->Name, tagnr, this->Getmui_ActiveTagMap()->tagNames, mTagLut);
		mTagLut->Build();
	}
	emit this->tagMapsChanged();
}
void mqMorphoDigCore::matchTagMapToActorCollection()
{
	int activeTagMapNr = this->Getmui_ActiveTagMap()->numTags; 
	int maxTagNr = this->highestTagInActorCollection();// 0 1 2 3 ... maxTagNr
	cout << "activeTagMapNr =" << activeTagMapNr << endl;
	cout << "maxTagNr =" << maxTagNr << endl;
	
	if (maxTagNr>=activeTagMapNr)
	{
		QMessageBox msgBox;
		msgBox.setText("Some tagged vertices (max tagged value found:" + QString::number(maxTagNr)+") are above active Tag Map highest value ("+ QString::number(activeTagMapNr-1)+"). Extend current tag map?");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setDefaultButton(QMessageBox::No);
		int ret = msgBox.exec();

		if (ret == QMessageBox::Yes) { this->increaseTagNumberTo(maxTagNr+1);}
		
		
	}

}

void mqMorphoDigCore::increaseTagNumberTo(int newtagnr)
{	
	int i = this->getActiveTagMapId();
	ExistingTagMaps *tagMaps = this->Getmui_ExistingTagMaps();
	size_t size = tagMaps->Stack.size();
	if (i < size && i >= 0)
	{
		vtkSmartPointer<vtkLookupTable> mTagLut = vtkSmartPointer<vtkLookupTable>::New();

		mTagLut = this->Getmui_ActiveTagMap()->TagMap;
		int tagnr = mTagLut->GetNumberOfTableValues();
		if (newtagnr > tagnr)
		{
			mTagLut->SetNumberOfTableValues(newtagnr);

			mTagLut->Build();
			for (int j = tagnr; j < newtagnr; j++)
			{
				double rgba[4];
				this->GetDefaultTagColor(j, rgba);
				mTagLut->SetTableValue(j, rgba[0], rgba[1], rgba[2], rgba[3]);
				QString TagName = "Tag" + QString::number(j);
				this->Getmui_ActiveTagMap()->tagNames.push_back(TagName.toStdString());
			}
			this->Getmui_ExistingTagMaps()->Stack.at(i).numTags = newtagnr;
			this->Getmui_ExistingTagMaps()->Stack.at(i).tagNames = this->Getmui_ActiveTagMap()->tagNames;
			this->Getmui_ExistingTagMaps()->Stack.at(i).TagMap = mTagLut;

			this->Setmui_ActiveTagMap(this->Getmui_ActiveTagMap()->Name, newtagnr, this->Getmui_ActiveTagMap()->tagNames, mTagLut);
			mTagLut->Build();

			emit this->tagMapsChanged();
		}


	}


	
}
int mqMorphoDigCore::getTagMapId(QString tagMapName)
{
	int tagMapId = -1;
	for (int i = 0; i <this->Getmui_ExistingTagMaps()->Stack.size(); i++)
	{
		QString myExisingTagMapName = this->Getmui_ExistingTagMaps()->Stack.at(i).Name;
		if (tagMapName == myExisingTagMapName)
		{

			tagMapId = i;
		}
	}
	return tagMapId;
}
int mqMorphoDigCore::getActiveTagMapId()
{
	int activeTagMapId = 0;
	QString ActiveTagMap = this->Getmui_ActiveTagMap()->Name;
	for (int i = 0; i <this->Getmui_ExistingTagMaps()->Stack.size(); i++)
	{
		QString myExisingTagMapName = this->Getmui_ExistingTagMaps()->Stack.at(i).Name;
		if (ActiveTagMap == myExisingTagMapName )
		{

			activeTagMapId = i;
		}
	}
	return activeTagMapId;
}
int mqMorphoDigCore::tagAlreadyExists(int tagnr)
{
	int exists = 0;
	int modified = 0;
	this->ActorCollection->InitTraversal();
	vtkIdType num = this->ActorCollection->GetNumberOfItems();
	for (vtkIdType i = 0; i < num; i++)
	{
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		//if (myActor->GetSelected() == 1)
		//{

		//	myActor->SetSelected(0);
		vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
		if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
		{
			vtkSmartPointer<vtkPolyData> mPD = vtkSmartPointer<vtkPolyData>::New();
			mPD = mymapper->GetInput();
			double numvert = mPD->GetNumberOfPoints();
			vtkIntArray *currentTags = (vtkIntArray*)mPD->GetPointData()->GetScalars(this->Getmui_ActiveArray()->Name.toStdString().c_str());
			if (currentTags != NULL)
			{
				for (vtkIdType j = 0; j < mymapper->GetInput()->GetNumberOfPoints(); j++)
				{
					if (currentTags->GetTuple1(j) == tagnr)
					{
						exists = 1; return 1;
					}
				}
				cout << "Finished loop and found no tag matching " << tagnr << endl;
			}
			else
			{

				cout << "current tags is null!" << endl;
			}
		}
		//}
	}
	return exists;
}
void mqMorphoDigCore::clearTag(int tagnr)
{
		cout << "Clean tag " << tagnr << endl;
		int modified = 0;
		this->ActorCollection->InitTraversal();
		vtkIdType num = this->ActorCollection->GetNumberOfItems();
		for (vtkIdType i = 0; i < num; i++)
		{
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
			//if (myActor->GetSelected() == 1)
			//{

			//	myActor->SetSelected(0);
				vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
				if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
				{
					vtkSmartPointer<vtkPolyData> mPD = vtkSmartPointer<vtkPolyData>::New();
					mPD = mymapper->GetInput();
					double numvert = mPD->GetNumberOfPoints();					
					vtkIntArray *currentTags = (vtkIntArray*)mPD->GetPointData()->GetScalars(this->Getmui_ActiveArray()->Name.toStdString().c_str());
					if (currentTags != NULL)
					{
						for (vtkIdType j = 0; j < mymapper->GetInput()->GetNumberOfPoints(); j++)
						{
							if (currentTags->GetTuple1(j)==tagnr)
							{
								currentTags->SetTuple1(j, 0);
							}							
						}
					}					
				}
			//}
		}
		

	/*int exists = 0;

	return exists;*/
}
int mqMorphoDigCore::highestTagInActorCollection()
{
	int max = 0;
	this->ActorCollection->InitTraversal();
	vtkIdType num = this->ActorCollection->GetNumberOfItems();
	cout << "Number of actors : " << num << endl;
	for (vtkIdType i = 0; i < num; i++)
	{
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		
		vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
		if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
		{
			vtkSmartPointer<vtkPolyData> mPD = vtkSmartPointer<vtkPolyData>::New();
			mPD = mymapper->GetInput();
			double numvert = mPD->GetNumberOfPoints();
			vtkIntArray *currentTags = (vtkIntArray*)mPD->GetPointData()->GetScalars(this->Getmui_ActiveArray()->Name.toStdString().c_str());			
			if (currentTags != NULL)
			{
				for (vtkIdType j = 0; j < mymapper->GetInput()->GetNumberOfPoints(); j++)
				{
					if (currentTags->GetTuple1(j) >max)
					{
						max = currentTags->GetTuple1(j);
					}
				}
			}
			
		}
		//}
	}
	return max;
}
void mqMorphoDigCore::removeTagFromTagMap(int i)
{
	ExistingTagMaps *tagMaps = this->Getmui_ExistingTagMaps();
	size_t size = tagMaps->Stack.size();
	if (i < size && i >= 0)
	{
		//to change if mod than one "preset" tagmap
		//only TagMap 1

		vtkSmartPointer<vtkLookupTable> mTagLut = vtkSmartPointer<vtkLookupTable>::New();

		mTagLut = this->Getmui_ActiveTagMap()->TagMap;
		int tagnr = mTagLut->GetNumberOfTableValues();
		int tag_to_remove = tagnr - 1;
		if (tagnr > 1)
		{
			int remove = 1;
			int exists= this->tagAlreadyExists(tag_to_remove);
			if (exists==1)
			{
				cout << "tag exists!"<< endl;
				QMessageBox msgBox;
				msgBox.setText("Some opened objets have vertices tagged with value " + QString::number(tag_to_remove) + ". Reset corresponding vertices to 0 ?");
				msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
				msgBox.setDefaultButton(QMessageBox::No);
				int ret = msgBox.exec();
				
				if (ret == QMessageBox::Yes) { this->clearTag(tag_to_remove); remove = 1; }
				else
				{
					remove = 0;
					QMessageBox msgBox2;
					msgBox2.setText("Remove last line from tag map anyway?");
					msgBox2.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
					msgBox2.setDefaultButton(QMessageBox::No);
					int ret2 = msgBox2.exec();
					if (ret2 == QMessageBox::Yes) { remove = 1; }
				}

			}
			if (remove)			
			{
				int newtag = tagnr;
				tagnr--;

				mTagLut->SetNumberOfTableValues(tagnr);
				//mTagLut->Build();


				this->Getmui_ActiveTagMap()->tagNames.pop_back();
				

				this->Getmui_ExistingTagMaps()->Stack.at(i).numTags = tagnr;
				this->Getmui_ExistingTagMaps()->Stack.at(i).tagNames = this->Getmui_ActiveTagMap()->tagNames;
				this->Getmui_ExistingTagMaps()->Stack.at(i).TagMap = mTagLut;
				this->Setmui_ActiveTagMap(this->Getmui_ActiveTagMap()->Name, tagnr, this->Getmui_ActiveTagMap()->tagNames, mTagLut);
			}
		}
		//this->Setmui_ActiveTagMap(TagMap, tagnr, tagNames, mTagLut);
	}
	emit this->tagMapsChanged();
}


void mqMorphoDigCore::reinitializeTagMap(int i)
{
	ExistingTagMaps *tagMaps = this->Getmui_ExistingTagMaps();
	size_t size = tagMaps->Stack.size();
	if (i < size && i >= 0 && tagMaps->Stack.at(i).isCustom==0)
	{
		//to change if mod than one "preset" tagmap
		//only TagMap 1
		int tagnr = 25;
		vtkSmartPointer<vtkLookupTable> mTagLut = vtkSmartPointer<vtkLookupTable>::New();

		mTagLut = this->GetTagLut();		
		mTagLut->SetNumberOfTableValues(tagnr);
		mTagLut->Build();
		std::vector<std::string> tagNames;
		for (int i = 0; i < tagnr; i++)
		{
			double rgba[4];
			cout << "i=" << i << "try to get rgba" << endl;
			this->GetDefaultTagColor(i, rgba);
			mTagLut->SetTableValue(i, rgba[0], rgba[1], rgba[2], rgba[3]);
			if (i == 0)
			{
				tagNames.push_back("Exterior");
			}
			else
			{
				QString TagName = "Tag" + QString::number(i);
				tagNames.push_back(TagName.toStdString());
			}
		}
		cout << "End i loop" << endl;
		QString TagMap = QString("TagMap");
		cout << "Try to set existing color maps!!" << endl;

		this->Getmui_ExistingTagMaps()->Stack.at(i).numTags = tagnr;
		this->Getmui_ExistingTagMaps()->Stack.at(i).tagNames = tagNames;
		this->Getmui_ExistingTagMaps()->Stack.at(i).TagMap = mTagLut;						
		this->Setmui_ActiveTagMap(TagMap, tagnr, tagNames, mTagLut);
	}	
	emit this->tagMapsChanged();
}

void mqMorphoDigCore::reinitializeColorMap(int i)
{
	ExistingColorMaps *colorMaps = this->Getmui_ExistingColorMaps();
	size_t size = colorMaps->Stack.size();
	if (i < size && i >= 0 && colorMaps->Stack.at(i).isCustom == 0)
	{
		//to change if mod than one "preset" colorMap
		vtkSmartPointer<vtkDiscretizableColorTransferFunction> STC = vtkSmartPointer<vtkDiscretizableColorTransferFunction>::New();
		vtkSmartPointer<vtkPiecewiseFunction> opacityRfunction = vtkSmartPointer<vtkPiecewiseFunction>::New();
		
		
		STC->DiscretizeOff();
		STC->SetColorSpaceToRGB();
		//this->ScalarRainbowLut->EnableOpacityMappingOn();
		QString Name;
		if (i == 0)
		{
			STC = this->ScalarRainbowLut;
			STC->RemoveAllPoints();
			Name = QString("Rainbow");
			STC->AddRGBPoint(0.0, 1.0, 0.0, 1.0); //# purple
			STC->AddRGBPoint(0.2, 0.0, 0.0, 1.0); //# blue
			STC->AddRGBPoint(0.4, 0.0, 1.0, 1.0); //# purple
			STC->AddRGBPoint(0.6, 0.0, 1.0, 0.0); //# green
			STC->AddRGBPoint(0.8, 1.0, 1.0, 0.0); //# yellow
			STC->AddRGBPoint(1.0, 1.0, 0.0, 0.0); //# red
			opacityRfunction->AddPoint(0, 0.3);
			opacityRfunction->AddPoint(0.2, 0.6);
			opacityRfunction->AddPoint(0.8, 0.8);
			opacityRfunction->AddPoint(1, 1);
			STC->SetScalarOpacityFunction(opacityRfunction);
			STC->EnableOpacityMappingOn();
			STC->Build();
		}
		else 
		{
			Name = QString("Black-Red-White");
			STC = this->ScalarRedLut;
			STC->RemoveAllPoints();
			STC->SetColorSpaceToRGB();
			STC->EnableOpacityMappingOn();
			STC->AddRGBPoint(0.0, 0.0, 0.0, 0.0); //# black
			STC->AddRGBPoint(0.2, 0.73, 0, 0.0); //# reddish
			STC->AddRGBPoint(0.4, 0.9, 0.82, 0.56);// # yellow
			STC->AddRGBPoint(1.0, 1.0, 1.0, 1.0);// # white
			opacityRfunction->AddPoint(0, 0.3);
			opacityRfunction->AddPoint(0.2, 0.5);
			opacityRfunction->AddPoint(0.4, 0.8);
			opacityRfunction->AddPoint(1, 1);
			STC->SetScalarOpacityFunction(opacityRfunction);
			STC->EnableOpacityMappingOn();
			STC->Build();
		}
		
		
		this->mui_ActiveColorMap->ColorMap =STC;
		this->mui_ActiveColorMap->Name = Name;
		this->mui_ExistingColorMaps->Stack.at(i).ColorMap = STC;
		this->mui_ExistingColorMaps->Stack.at(i).Name = Name;

	
		cout << "Try to set existing color maps 3!!" << endl;

	}
	cout << "colorMapsChanged reinitializeColorMap" << endl;
	emit this->colorMapsChanged();
}
void mqMorphoDigCore::deleteTagMap(int i)
{
	ExistingTagMaps *tagMaps = this->Getmui_ExistingTagMaps();
	size_t size = tagMaps->Stack.size();
	if (i < size && i >= 0)
	{
		tagMaps->Stack.erase(tagMaps->Stack.begin() + i);
		if (tagMaps->Stack.size() > 0)
		{
			this->Setmui_ActiveTagMap(tagMaps->Stack.at(0).Name, tagMaps->Stack.at(0).numTags, tagMaps->Stack.at(0).tagNames, tagMaps->Stack.at(0).TagMap);
		}
	}
	emit this->colorMapsChanged();
}

int mqMorphoDigCore::tagMapNameAlreadyExists(QString proposed_name)
{
	ExistingTagMaps *tagMaps = this->Getmui_ExistingTagMaps();
	size_t size = tagMaps->Stack.size();
	int exists = 0;
	for (int i = 0; i < size; i++)
	{
		if (tagMaps->Stack.at(i).Name == proposed_name)
		{
			exists = 1;
		}
	}
	return exists;

}
int mqMorphoDigCore::colorMapNameAlreadyExists(QString proposed_name)
{
	ExistingColorMaps *colorMaps = this->Getmui_ExistingColorMaps();
	size_t size = colorMaps->Stack.size();
	int exists = 0;
	for (int i = 0; i < size; i++)
	{
		if (colorMaps->Stack.at(i).Name == proposed_name)
		{
			exists = 1;
		}
	}
	return exists;

}

void mqMorphoDigCore::ComputeSelectedNamesLists()
{
	
		g_selected_names.clear();
		g_distinct_selected_names.clear();
	
		this->ActorCollection->InitTraversal();

	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMDActor * myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			g_selected_names.push_back(myActor->GetName());

			int already = 0;

			for (int j = 0; j<g_distinct_selected_names.size(); j++)
			{
				//std::cout<<"i"<<i<<std::endl;
				std::size_t found = g_distinct_selected_names.at(j).find(myActor->GetName());
				size_t length1 = myActor->GetName().length();
				size_t length2 = g_distinct_selected_names.at(j).length();
				if (length1 == length2 && found != std::string::npos)
				{
					already = 1;
				}
			}
			if (already == 0)
			{
				g_distinct_selected_names.push_back(myActor->GetName());
			}


		}
	}
	this->VolumeCollection->InitTraversal();

	for (vtkIdType i = 0; i < this->VolumeCollection->GetNumberOfItems(); i++)
	{
		vtkMDVolume * myVolume = vtkMDVolume::SafeDownCast(this->VolumeCollection->GetNextVolume());
		if (myVolume->GetSelected() == 1)
		{
			g_selected_names.push_back(myVolume->GetName());

			int already = 0;

			for (int j = 0; j < g_distinct_selected_names.size(); j++)
			{
				//std::cout<<"i"<<i<<std::endl;
				std::size_t found = g_distinct_selected_names.at(j).find(myVolume->GetName());
				size_t length1 = myVolume->GetName().length();
				size_t length2 = g_distinct_selected_names.at(j).length();
				if (length1 == length2 && found != std::string::npos)
				{
					already = 1;
				}
			}
			if (already == 0)
			{
				g_distinct_selected_names.push_back(myVolume->GetName());
			}


		}
	}
	
}

int mqMorphoDigCore::context_file_exists(QString path, QString ext, QString postfix)
{
	// used by the save NTW function : 
	//looks whether a non-mesh related file = context file (.ori, .flg, .tag, .ver, .cur, .stv) constructed with only a postfix + extension (project name) already exists 
	QString filename;
	int exists = 0;

	filename = path;
	if (postfix.length()>0)
	{
		filename = filename + postfix;
	}
	filename += ext;
	ifstream file(filename.toLocal8Bit());
	if (file)
	{
		file.close();
		exists = 1;
	}
	return exists;
}

int mqMorphoDigCore::selected_file_exists(QString path, QString ext, QString postfix)
{
	// used by the save NTW function : 
	//looks whether mesh related files (vtk, vtp, stl, ply, pos) constructed with a prefix (object name) and a postfix (project name) already exist 
	QString filename;
	int exists = 0;
	for (int i = 0; i<g_distinct_selected_names.size(); i++)
	{
		filename = path;
		filename.append(g_distinct_selected_names.at(i).c_str());
		if (postfix.length()>0)
		{
			filename += postfix;
		}
		filename += ext;;
		ifstream file(filename.toLocal8Bit());
		if (file)
		{
			file.close();
			exists = 1;
			break;
		}
	}
	//cout << "file " << path << " " << ext <<  postfix<< " exists" << exists;
	return exists;

}

void mqMorphoDigCore::SaveORI(QString fileName)
{

	QString X1, X2, Y1, Y2, Z1, Z2;

	std::string ORIext = ".ori";
	std::string ORIext2 = ".ORI";
	std::size_t found = fileName.toStdString().find(ORIext);
	std::size_t found2 = fileName.toStdString().find(ORIext2);
	if (found == std::string::npos && found2 == std::string::npos)
	{
		fileName.append(".ori");
	}

	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);

		stream << this->Getmui_Z1Label() << endl;
		stream << this->Getmui_Z2Label() << endl;
		stream << this->Getmui_Y1Label() << endl;
		stream << this->Getmui_Y2Label() << endl;
		stream << this->Getmui_X1Label() << endl;
		stream << this->Getmui_X2Label() << endl;


	}
	file.close();





}

void mqMorphoDigCore::GetDefaultTagColor(int tagnr, double rgba[4])
{
	cout << "GetDefaultTagColor(" << tagnr <<")"<< endl;
	std::vector<double> r;
	
	std::vector<double> g;
	std::vector<double> b;
	r.push_back(0.7); g.push_back(0.8); b.push_back(0.8);
	r.push_back(0.64); g.push_back(0); b.push_back(0.8);
	r.push_back(1); g.push_back(0); b.push_back(0);
	r.push_back(0); g.push_back(1); b.push_back(0);
	r.push_back(0); g.push_back(0); b.push_back(1);
	r.push_back(0); g.push_back(1); b.push_back(1);
	r.push_back(1); g.push_back(0); b.push_back(1);
	r.push_back(1); g.push_back(1); b.push_back(0);
	r.push_back(1); g.push_back(0.5); b.push_back(0.5);
	r.push_back(0.5); g.push_back(1); b.push_back(0.5);
	r.push_back(0.5); g.push_back(0.5); b.push_back(1);
	r.push_back(1); g.push_back(0); b.push_back(0.5);
	r.push_back(0); g.push_back(1); b.push_back(0.5);
	r.push_back(0); g.push_back(0.5); b.push_back(1);
	r.push_back(1); g.push_back(0.5); b.push_back(0);
	r.push_back(0.5); g.push_back(1); b.push_back(0);
	r.push_back(0.5); g.push_back(0); b.push_back(1);
	r.push_back(0.5); g.push_back(1); b.push_back(1);
	r.push_back(1); g.push_back(0.5); b.push_back(1);
	r.push_back(1); g.push_back(1); b.push_back(0.5);
	r.push_back(0.5); g.push_back(0); b.push_back(0);
	r.push_back(0); g.push_back(0.5); b.push_back(0);
	r.push_back(0); g.push_back(0); b.push_back(0.5);
	r.push_back(0); g.push_back(0.5); b.push_back(0.5);
	r.push_back(0.5); g.push_back(0); b.push_back(0.5);
	r.push_back(0.5); g.push_back(0.5); b.push_back(0);
	r.push_back(0.5); g.push_back(0.25); b.push_back(0);
	r.push_back(0.25); g.push_back(0.5); b.push_back(0);
	r.push_back(0.25); g.push_back(0); b.push_back(0.5);
	r.push_back(0.5); g.push_back(0); b.push_back(0.25);
	r.push_back(0); g.push_back(0.5); b.push_back(0.25);
	r.push_back(0); g.push_back(0); b.push_back(0.5);
	r.push_back(0.25); g.push_back(0.5); b.push_back(0.5);
	r.push_back(0.5); g.push_back(0.25); b.push_back(0.5);
	r.push_back(0.5); g.push_back(0.5); b.push_back(0.25);
	r.push_back(1); g.push_back(0.25); b.push_back(1);
	r.push_back(1); g.push_back(1); b.push_back(0.25);
	r.push_back(1); g.push_back(0.25); b.push_back(0.5);
	r.push_back(0.25); g.push_back(1); b.push_back(0.5);
	r.push_back(0.25); g.push_back(0.5); b.push_back(1);
	r.push_back(1); g.push_back(0.5); b.push_back(0.25);
	r.push_back(0.5); g.push_back(1); b.push_back(0.25);
	r.push_back(0.5); g.push_back(0.25); b.push_back(1);
	r.push_back(1); g.push_back(0.25); b.push_back(0);
	r.push_back(0.25); g.push_back(1); b.push_back(0);
	r.push_back(0.25); g.push_back(0); b.push_back(1);
	r.push_back(0.25); g.push_back(1); b.push_back(1);
	r.push_back(1); g.push_back(0); b.push_back(0.25);
	r.push_back(0); g.push_back(1); b.push_back(0.25);
	r.push_back(0); g.push_back(0.25); b.push_back(1);
	
	if (tagnr < r.size())
	{
		cout << "r.size()=" << r.size() << endl;
		rgba[0] = r.at(tagnr);
		rgba[1] = g.at(tagnr);
		rgba[2] = b.at(tagnr);
		rgba[3] = 0.8;

	}
	else
	{
		cout << "Try rand..." << endl;
		rgba[0] = ((double)rand() / (double)RAND_MAX);
		rgba[1] = ((double)rand() / (double)RAND_MAX);
		rgba[2] = ((double)rand() / (double)RAND_MAX);
		rgba[3] = 0.8;
	}
}
void mqMorphoDigCore::UpdateAllSelectedFlagsColors()
{
	vtkIdType num_flg = 0;
	num_flg = mqMorphoDigCore::instance()->getFlagLandmarkCollection()->GetNumberOfSelectedActors();
	if (num_flg == 0) {
		QMessageBox msgBox;
		msgBox.setText("No flag landmark selected. Please select at least one flag to use this option.");
		msgBox.exec();
		return;
	}
	double r1, g1, b1;
	vtkIdType selectedflags = this->getFlagLandmarkCollection()->GetNumberOfSelectedActors();
	if (selectedflags>0)
	{

		vtkSmartPointer<vtkLMActorCollection> myColl = vtkSmartPointer<vtkLMActorCollection>::New();
		myColl = this->FlagLandmarkCollection;
		
		std::string action = "Update color of all selected flags";
		int mCount = BEGIN_UNDO_SET(action);
		
		
		myColl->InitTraversal();
		for (vtkIdType k = 0; k < myColl->GetNumberOfItems(); k++)
		{
			int ok = 0;
			vtkLMActor *myFlag = vtkLMActor::SafeDownCast(myColl->GetNextActor());
			double min_dist = DBL_MAX;
			if (myFlag->GetSelected() == 1)
			{
				double flpos[3];
				
				myFlag->GetLMOrigin(flpos);
				double closest[3] = { flpos[0] , flpos[1],flpos[2] };
				cout << "Current flag :" << flpos[0] << "," << flpos[1] << "," << flpos[2] << endl;

				this->ActorCollection->InitTraversal();
				for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
				{
					vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
					
					if (myActor->GetSelected() == 1) { myActor->SetSelected(0); }
					vtkPolyDataMapper *mapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
					if (mapper != NULL && vtkPolyData::SafeDownCast(mapper->GetInput()) != NULL)
					{
						vtkSmartPointer<vtkPolyData> myPD = vtkSmartPointer<vtkPolyData>::New();
						myPD->DeepCopy(vtkPolyData::SafeDownCast(mapper->GetInput()));
						double ve_init_pos[3];;
						double ve_final_pos[3];
						vtkSmartPointer<vtkMatrix4x4> Mat = myActor->GetMatrix();

						vtkIdType id_min=-1; 
						for (vtkIdType j = 0; j < myPD->GetNumberOfPoints(); j++) 
						{
							// for every triangle 
							myPD->GetPoint(j, ve_init_pos);
							mqMorphoDigCore::TransformPoint(Mat, ve_init_pos, ve_final_pos);

							double curr_dist = (ve_final_pos[0] - flpos[0])*(ve_final_pos[0] - flpos[0]) +
								(ve_final_pos[1] - flpos[1])*(ve_final_pos[1] - flpos[1]) +
								(ve_final_pos[2] - flpos[2])*(ve_final_pos[2] - flpos[2]);
							if (min_dist>curr_dist)
							{
								id_min = j;
								min_dist = curr_dist;
								//now get current color!
								
							}
							
						}
						if (id_min != -1) // means that mesh i could be the mesh that contains the closest vertex of flag k
						{
							//now get current color of point id_min of mesh i!
							//@TODO! => On va faire 1 variable globale de type G_Current_Active_Scalar => Ce premier if sera changé par 
							// if (visibility ==0 OU GetScalar(G_Current_Active_Scalar)==NULL)
							QString none = QString("Solid color");
							if (this->Getmui_ArrayVisibility() == 0 || this->mui_ActiveArray->Name== none||
							myPD->GetPointData()->GetScalars(this->mui_ActiveArray->Name.toStdString().c_str()) == NULL )
								
							{
								myActor->GetProperty()->GetColor(r1, g1, b1);
								cout << "Mesh PLAIN color " <<i<<"("<< myActor->GetName() << "): " << "r="<<r1 << ", g=" << g1<< ", b="<<b1 << endl;
								ok = 1;
							}
							else
							{
								if (
									(this->mui_ActiveArray->DataType == VTK_INT || this->mui_ActiveArray->DataType == VTK_UNSIGNED_INT)
									&& this->mui_ActiveArray->NumComp==1
									)
								{ 
								// Tag-like scalar!!!!
									vtkIntArray *currentTagsLike;
									currentTagsLike = (vtkIntArray*)myPD->GetPointData()->GetScalars(this->mui_ActiveArray->Name.toStdString().c_str());
									int mytag = currentTagsLike->GetTuple(id_min)[0];
									vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
									lut = vtkLookupTable::SafeDownCast(mapper->GetLookupTable());
									double rgb[3];
									lut->GetColor((double)mytag, rgb);
									r1 = rgb[0];
									g1 = rgb[1];
									b1 = rgb[2];
									cout << "Mesh TAG-like color " << i << "(" << myActor->GetName() << "): " << "r=" << r1 << ", g=" << g1 << ", b=" << b1 << endl;
									ok = 1;								
								}
								else if (
									(this->mui_ActiveArray->DataType == VTK_UNSIGNED_CHAR)
									&& (this->mui_ActiveArray->NumComp == 3 || this->mui_ActiveArray->NumComp == 4)
									)
								{
									//RGB-Like Arrays
									int numcomp = this->mui_ActiveArray->NumComp;
									vtkSmartPointer<vtkUnsignedCharArray> colors =
										vtkSmartPointer<vtkUnsignedCharArray>::New();
									colors->SetNumberOfComponents(numcomp);
									colors = (vtkUnsignedCharArray*)myPD->GetPointData()->GetScalars(this->mui_ActiveArray->Name.toStdString().c_str());
									double cur_r = (double)colors->GetTuple(id_min)[0];
									double cur_g = (double)colors->GetTuple(id_min)[1];
									double cur_b = (double)colors->GetTuple(id_min)[2];
									r1 = cur_r / 255;
									g1 = cur_g / 255;
									b1 = cur_b / 255;
									cout << "Mesh RGB-like color " << i << "(" << myActor->GetName() << "): " << "r=" << r1 << ", g=" << g1 << ", b=" << b1 << endl;
									ok = 1;

								}
								else if (
									(this->mui_ActiveArray->DataType == VTK_DOUBLE || this->mui_ActiveArray->DataType == VTK_FLOAT)
									&& this->mui_ActiveArray->NumComp == 1
									)
								{
									if (this->mui_ActiveArray->DataType == VTK_FLOAT)
									{
										// Tag-like scalar!!!!
										vtkFloatArray *currentScalars;
										currentScalars = (vtkFloatArray*)myPD->GetPointData()->GetScalars(this->mui_ActiveArray->Name.toStdString().c_str());
										float myscalar = currentScalars->GetTuple(id_min)[0];
										vtkSmartPointer<vtkLookupTable> lut = vtkLookupTable::SafeDownCast(mapper->GetLookupTable());
										double rgb[3];
										lut->GetColor(myscalar, rgb);
										r1 = rgb[0];
										g1 = rgb[1];
										b1 = rgb[2];
										cout << "Mesh Float-Scalar color " << i << "(" << myActor->GetName() << "): " << "r=" << r1 << ", g=" << g1 << ", b=" << b1 << endl;
										ok = 1;
									}
									else
									{
										// Tag-like scalar!!!!
										vtkDoubleArray *currentScalars;
										currentScalars = (vtkDoubleArray*)myPD->GetPointData()->GetScalars(this->mui_ActiveArray->Name.toStdString().c_str());
										
										double myscalar = currentScalars->GetTuple(id_min)[0];
										vtkSmartPointer<vtkLookupTable> lut = vtkLookupTable::SafeDownCast(mapper->GetLookupTable());
										double rgb[3];
										lut->GetColor(myscalar, rgb);
										r1 = rgb[0];
										g1 = rgb[1];
										b1 = rgb[2];
										cout << "Mesh Double-Scalar color " << i << "(" << myActor->GetName() << "): " << "r=" << r1 << ", g=" << g1 << ", b=" << b1 << endl;
										ok = 1;

									}
								}
								
									
							}

						}
					}

				}
				
				
				myFlag->SetSelected(0);
				if (ok == 1)
				{
					myFlag->SaveState(mCount);
					myFlag->SetmColor(r1,g1,b1,0.5);

				}
				this->UpdateLandmarkSettings(myFlag);



			}

		}
		END_UNDO_SET();

		this->Render();
	}
}

void mqMorphoDigCore::UpdateAllSelectedFlagsColors(double flagcolor[4])
{
	vtkIdType selectedflags = this->getFlagLandmarkCollection()->GetNumberOfSelectedActors();
	if (selectedflags>0)
	{
		
		vtkSmartPointer<vtkLMActorCollection> myColl = vtkSmartPointer<vtkLMActorCollection>::New();
		myColl = this->FlagLandmarkCollection;


		myColl->InitTraversal();
		for (vtkIdType i = 0; i < myColl->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(myColl->GetNextActor());
			if (myActor->GetSelected() == 1 )
			{

				myActor->SetmColor(flagcolor);
				myActor->SetSelected(0);
				this->UpdateLandmarkSettings(myActor);
				


			}

		}


	}

}
void mqMorphoDigCore::UpdateAllSelectedFlagsLengths(double flag_rendering_size)
{
	vtkIdType selectedflags = this->getFlagLandmarkCollection()->GetNumberOfSelectedActors();
	if (selectedflags>0)
	{

		vtkSmartPointer<vtkLMActorCollection> myColl = vtkSmartPointer<vtkLMActorCollection>::New();
		myColl = this->FlagLandmarkCollection;


		myColl->InitTraversal();
		for (vtkIdType i = 0; i < myColl->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(myColl->GetNextActor());
			if (myActor->GetSelected() == 1)
			{

				myActor->SetLMSize(flag_rendering_size);
				myActor->SetSelected(0);
				this->UpdateLandmarkSettings(myActor);



			}

		}


	}

}

void mqMorphoDigCore::OpenFLG(QString fileName)
{
	//cout << "OpenFLG " << fileName.toStdString() << endl;
	double  x, y, z, nx, ny, nz, flength, r, g, b;

	QString FLGName;

	//Open a landmark file!


	int  length;


	length = fileName.length();

	int done = 0;
	if (length>0)
	{
		int file_exists = 1;
		ifstream file(fileName.toLocal8Bit());
		if (file)
		{
			//std::cout<<"file:"<<filename.c_str()<<" exists."<<std::endl;
			file.close();
		}
		else
		{

			//std::cout << "file:" << fileName.toLocal8Bit() << " does not exists." << std::endl;
			file_exists = 0;
		}

		if (file_exists == 1)
		{

			std::string FLGext(".flg");
			std::string FLGext2(".FLG");


			int type = 0; // 0 = .POS Ascii File //1 = .MAT binary File or simple .MAT file

			std::size_t found = fileName.toStdString().find(FLGext);
			std::size_t found2 = fileName.toStdString().find(FLGext2);
			if (found != std::string::npos || found2 != std::string::npos)
			{
				type = 1;
				//FLG
			}



			if (type == 1)
			{


				//filein = fopen(fileName.toLocal8Bit(), "rt");
				QFile inputFile(fileName);
				int ok = 0;
				int cpt_created = 0;
				if (inputFile.open(QIODevice::ReadOnly))
				{
					QTextStream in(&inputFile);
					int cpt = 1;
					while (!in.atEnd())
					{
						QString line = in.readLine();
						QTextStream myteststream(&line);
						if (cpt % 2 == 0)
						{

							myteststream >> x >> y >> z >> nx >> ny >> nz >> flength >> r >> g >> b;
							double coord[3] = { x,y,z };
							double ncoord[3] = { nx,ny,nz };
							double ori[3];

							double length = nx*nx + ny*ny + nz*nz;
							if (length == 1)
							{
								ori[0] = ncoord[0];
								ori[1] = ncoord[1];
								ori[2] = ncoord[2];
							}
							else
							{
								vtkMath::Subtract(ncoord, coord, ori);
								vtkMath::Normalize(ori);
							}
							this->CreateLandmark(coord, ori, 4);
							vtkLMActor *myLastFlag = this->GetLastLandmark(4);
							myLastFlag->SetLMType(4);
							myLastFlag->SetmColor(r, g, b, 0.5);
							myLastFlag->SetLMText(FLGName.toStdString());
							//cout << "Set LM Size:" << flength << endl;
							myLastFlag->SetLMSize(flength);
							myLastFlag->SetChanged(1);
							cpt_created ++;
						}
						else
						{
							FLGName = line;
						}
						cpt++;

					}
					/**/

					inputFile.close();
					this->CreateLandmarkUndoSet(4, cpt_created);
					this->UpdateLandmarkSettings();

				}
				this->Render();
			}//fin if																		

		}//file exists...
	}

}
void mqMorphoDigCore::OpenPOS(QString fileName, int mode)
{
	// mode : 0 for last inserted surface
	// mode : 1 for all selected surfaces AND volumes
	// mode : 2 for all selected landmarks/flags
	// mode : 3 for last inserted volume	
	

	//Open a position file!

	int i, j, l;
	int  length;


	length = fileName.length();

	union {
		float f;
		char c[4];
	} u; // holds one float or 4 characters (bytes)



	int done = 0;
	if (length>0)
	{
		int file_exists = 1;
		ifstream file(fileName.toLocal8Bit());
		if (file)
		{
			//std::cout<<"file:"<<filename.c_str()<<" exists."<<std::endl;
			file.close();
		}
		else
		{

			//std::cout << "file:" << fileName.toLocal8Bit() << " does not exists." << std::endl;
			file_exists = 0;
		}

		if (file_exists == 1)
		{

			std::string MAText(".mat");
			std::string MAText2(".MAT");
			std::string POSext(".pos");
			std::string POSext2(".POS");

			int type = 0; // 0 = .POS Ascii File //1 = .MAT binary File or simple .MAT file

			std::size_t found = fileName.toStdString().find(MAText);
			std::size_t found2 = fileName.toStdString().find(MAText2);
			if (found != std::string::npos || found2 != std::string::npos)
			{
				type = 1;
				//MAT
			}

			found = fileName.toStdString().find(POSext);
			found2 = fileName.toStdString().find(POSext2);
			if (found != std::string::npos || found2 != std::string::npos)
			{
				type = 0; //POS
			}




			int Ok = 1;
			vtkSmartPointer<vtkMatrix4x4> Mat = vtkSmartPointer<vtkMatrix4x4>::New();



			if (type == 1)
			{
				FILE	*filein;									// Filename To Open
				filein = fopen(fileName.toLocal8Bit(), "rb");
				for (i = 0; i<4; i++)
					for (j = 0; j<4; j++)
					{

						for (l = 3; l >= 0; l--)
						{
							u.c[l] = fgetc(filein);
						}
						//Mat1[j][i] = u.f;
						//My_Obj->Mat1[i][j] = u.f;
					}


				for (i = 0; i<4; i++)
					for (j = 0; j<4; j++)
					{
						for (l = 3; l >= 0; l--)
						{
							u.c[l] = fgetc(filein);
						}
						Mat->SetElement(j, i, double(u.f));

					}

			}
			else
			{
				//filein = fopen(fileName.toLocal8Bit(), "rt");
				QFile inputFile(fileName);
				int ok = 0;

				if (inputFile.open(QIODevice::ReadOnly))
				{
					QTextStream in(&inputFile);

					// first matrix is useless (for the moment)	
					for (i = 0; i < 4; i++)
					{
						QString line = in.readLine();

					}
					//
					for (i = 0; i < 4; i++)
					{
						QString line = in.readLine();
						double n1, n2, n3, n4;
						QTextStream myteststream(&line);
						myteststream >> n1 >> n2 >> n3 >> n4;

						Mat->SetElement(0, i, n1);
						Mat->SetElement(1, i, n2);
						Mat->SetElement(2, i, n3);
						Mat->SetElement(3, i, n4);


					}
					inputFile.close();

				}
			}//fin if	



			 //cout << "call MorphoDig apply mat" << &Mat << endl;
			this->ApplyMatrix(Mat, mode);
			this->AdjustCameraAndGrid();

		}//file exists...
	}	//length

}

void mqMorphoDigCore::OpenPOSTrans(QString fileName, int mode)
{
	// mode : 0 for last inserted surface
	// mode : 1 for all selected surfaces AND volumes
	// mode : 2 for all selected landmarks/flags
	// mode : 3 for last inserted volume	
	//Open a position file!

	int i, j, l;
	int  length;


	length = fileName.length();

	union {
		float f;
		char c[4];
	} u; // holds one float or 4 characters (bytes)



	int done = 0;
	if (length>0)
	{
		int file_exists = 1;
		ifstream file(fileName.toLocal8Bit());
		if (file)
		{
			//std::cout<<"file:"<<filename.c_str()<<" exists."<<std::endl;
			file.close();
		}
		else
		{

			//std::cout << "file:" << fileName.toStdString().c_str() << " does not exists." << std::endl;
			file_exists = 0;
		}

		if (file_exists == 1)
		{

			std::string MAText(".mat");
			std::string MAText2(".MAT");
			std::string POSext(".pos");
			std::string POSext2(".POS");

			int type = 0; // 0 = .POS Ascii File //1 = .MAT binary File or simple .MAT file

			std::size_t found = fileName.toStdString().find(MAText);
			std::size_t found2 = fileName.toStdString().find(MAText2);
			if (found != std::string::npos || found2 != std::string::npos)
			{
				type = 1;
				//MAT
			}

			found = fileName.toStdString().find(POSext);
			found2 = fileName.toStdString().find(POSext2);
			if (found != std::string::npos || found2 != std::string::npos)
			{
				type = 0; //POS
			}




			int Ok = 1;
			vtkSmartPointer<vtkMatrix4x4> Mat = vtkSmartPointer<vtkMatrix4x4>::New();



			if (type == 1)
			{
				FILE	*filein;									// Filename To Open


				filein = fopen(fileName.toLocal8Bit(), "rb");
				for (i = 0; i<4; i++)
					for (j = 0; j<4; j++)
					{

						for (l = 3; l >= 0; l--)
						{
							u.c[l] = fgetc(filein);
						}
						//My_Obj->Mat1[i][j] = u.f;
					}


				for (i = 0; i<4; i++)
					for (j = 0; j<4; j++)
					{
						for (l = 3; l >= 0; l--)
						{
							u.c[l] = fgetc(filein);
						}
						Mat->SetElement(i, j, double(u.f));

					}

			}
			else
			{
				//filein = fopen(fileName.toLocal8Bit(), "rt");
				QFile inputFile(fileName);
				int ok = 0;

				if (inputFile.open(QIODevice::ReadOnly))
				{
					QTextStream in(&inputFile);

					// first matrix is useless (for the moment)	
					for (i = 0; i < 4; i++)
					{
						QString line = in.readLine();

					}
					//
					for (i = 0; i < 4; i++)
					{
						QString line = in.readLine();
						double n1, n2, n3, n4;
						QTextStream myteststream(&line);
						myteststream >> n1 >> n2 >> n3 >> n4;

						Mat->SetElement(i, 0, n1);
						Mat->SetElement(i, 1, n2);
						Mat->SetElement(i, 2, n3);
						Mat->SetElement(i, 3, n4);


					}
					inputFile.close();

				}
			}//fin if		

			double N1, N2, N3;
			N1 = -(Mat->GetElement(3, 0) * Mat->GetElement(0, 0) +
				Mat->GetElement(3, 1) * Mat->GetElement(0, 1)
				+ Mat->GetElement(3, 2) * Mat->GetElement(0, 2));



			Mat->SetElement(0, 3, N1);



			N2 = -(Mat->GetElement(3, 0) * Mat->GetElement(1, 0) +
				Mat->GetElement(3, 1) * Mat->GetElement(1, 1)
				+ Mat->GetElement(3, 2) * Mat->GetElement(1, 2));

			Mat->SetElement(1, 3, N2);

			N3 = -(Mat->GetElement(3, 0) * Mat->GetElement(2, 0) +
				Mat->GetElement(3, 1) * Mat->GetElement(2, 1)
				+ Mat->GetElement(3, 2) * Mat->GetElement(2, 2));

			Mat->SetElement(2, 3, N3);


			Mat->SetElement(3, 0, 0);
			Mat->SetElement(3, 1, 0);
			Mat->SetElement(3, 2, 0);

			//cout << "call MorphoDig apply mat" << &Mat << endl;
			this->ApplyMatrix(Mat, mode);

			//ApplyChanges()
			this->AdjustCameraAndGrid();
		}//file exists...
	}	//length

}
void mqMorphoDigCore::ImportAvizoLandmarks(QString fileName)
{
	double  x, y, z;
	//QString OneLine;
	QString Word1, Word2, Word3, Word4, Word5;
	int num_lmk = 0;
	int num_sets=1;
	QString start1;
	QString start2;

	//Open a landmark file!

	cout << "Import an Avizo/Amira file" << endl;
	size_t  length;


	length = fileName.toStdString().length();
	int li = 1;

	int done = 0;
	if (length > 0)
	{
		int file_exists = 1;
		ifstream file(fileName.toLocal8Bit());
		if (file)
		{
			//std::cout<<"file:"<<filename.c_str()<<" exists."<<std::endl;
			file.close();
		}
		else
		{

			//std::cout << "file:" << fileName.toLocal8Bit() << " does not exists." << std::endl;
			file_exists = 0;
		}
		if (file_exists == 1)
		{			

			//filein = fopen(fileName.toLocal8Bit(), "rt");
			QFile inputFile(fileName);
			int ok = 0;

			if (inputFile.open(QIODevice::ReadOnly))
			{
				QTextStream in(&inputFile);
				QString line = in.readLine(); // # Avizo 3D ASCII 3.0
				QTextStream mystream(&line);
				mystream >> Word1;
				cout << "line_" << li << ":" << line.toStdString().c_str() << endl;
				
				int ok = 0;
				while (!in.atEnd() && ok==0)
				{ 
					li++;
					QString line = in.readLine(); // Empty line?
					QTextStream myteststream(&line);					
					cout << "line_"<<li<<":"<< line.toStdString().c_str()<<endl;
					if (line.length() > 0)
					{
						//define Markers 3
						//QString::compare(ArrayName, newArrayName, Qt::CaseInsensitive) ==0
						if (line.contains("define Markers", Qt::CaseInsensitive))
						{
							cout << "Contains define Markers" << endl;
							myteststream >> Word1 >> Word2 >> num_lmk;
						}
						ok = 1;

					}
				}
				cout << "line_" << li << ":num_lmk=" << num_lmk << endl;
				// Now I want get NumSets 
				ok = 0;
				while (!in.atEnd() && ok == 0)
				{
					li++;
					QString line = in.readLine();
					QTextStream myteststream(&line);
					cout << "line_" << li << ":" << line.toStdString().c_str() << endl;
					if (line.length() > 0)
					{
						//define Markers 3
						//QString::compare(ArrayName, newArrayName, Qt::CaseInsensitive) ==0
						if (line.contains("NumSets",  Qt::CaseInsensitive))
						{
							cout << "Contains NumSets" << endl;
							myteststream >> Word1 >> num_sets;
							ok = 1;
						}
						

					}
				}
				cout << "line_" << li << ":num_sets=" << num_sets << endl;
				// Now make sure that this or these sets LandmarkSet.
				ok = 0;
				while (!in.atEnd() && ok == 0)
				{
					li++;
					QString line = in.readLine(); 
					QTextStream myteststream(&line);
					cout << "line_" << li << ":" << line.toStdString().c_str() << endl;
					if (line.length() > 0)
					{					
						if (line.contains("LandmarkSet", Qt::CaseInsensitive))
						{							
							ok = 1;
						}
					}
				}
				// if we are not at the end of the file, search for @1 @2 and place them in start1 start2
				ok = 0;
				int curr_set = 1;
				while (!in.atEnd() && ok==0)
				{
					li++;
					QString line = in.readLine();
					cout << "line_" << li << ":" << line.toStdString().c_str() << endl;
					QTextStream myteststream(&line);
					if (line.length() > 0 && line.contains("Markers", Qt::CaseInsensitive) && curr_set ==1)
					{
						myteststream >> Word1 >> Word2 >> Word3 >> Word4 >> Word5 >> start1;
						cout << "start1=" << start1.toStdString().c_str() << endl;
						if (num_sets == curr_set)
						{
							ok = 1;
						}
						else
						{
							curr_set++;
						}
					}
					else if (line.length() > 0 && line.contains("Markers", Qt::CaseInsensitive) && curr_set == 2)
					{
						myteststream >> Word1 >> Word2 >> Word3 >> Word4 >> Word5 >> start2;
						cout << "start2=" << start2.toStdString().c_str() << endl;
						ok = 1;
					}
					cout << "line_" << li << ":" << Word1.toStdString().c_str() << "|"<<Word2.toStdString().c_str() << "|" << Word3.toStdString().c_str() << endl;			

				}
				//at this stage we should have start1 (and optionnally start2)
				// Now make sure that this or these sets LandmarkSet.
				ok = 0;
				while (!in.atEnd() && ok == 0)
				{
					li++;
					QString line = in.readLine();
					QTextStream myteststream(&line);
					cout << "line_" << li << ":" << line.toStdString().c_str() << endl;
					if (line.length() > 0 && line.contains(start1))
					{
							ok = 1;
					}
					
				}
				int cpt = 0;

				ok = 0;
				//first landmark set
				while (!in.atEnd() && cpt < num_lmk)
				{
					li++;
					QString line = in.readLine();
					QTextStream myteststream(&line);
					myteststream >>  x >> y >> z;
					double coord[3] = { x,y,z };
					double ori[3];

					ori[0] = 0;
					ori[1] = 0;
					ori[2] = 1;

					this->CreateLandmark(coord, ori, 0);
					cpt++;
				}
				this->CreateLandmarkUndoSet(0, cpt);
				if (num_sets > 1)
				{
					ok = 0;
					while (!in.atEnd() && ok == 0)
					{
						li++;
						QString line = in.readLine();
						QTextStream myteststream(&line);
						cout << "line_" << li << ":" << line.toStdString().c_str() << endl;
						if (line.length() > 0 && line.contains(start2))
						{
							ok = 1;
						}

					}
					cpt = 0;
					ok = 0;
					//first landmark set
					while (!in.atEnd() && cpt < num_lmk)
					{
						li++;
						QString line = in.readLine();
						QTextStream myteststream(&line);
						myteststream >> x >> y >> z;
						double coord[3] = { x,y,z };
						double ori[3];

						ori[0] = 0;
						ori[1] = 0;
						ori[2] = 1;

						this->CreateLandmark(coord, ori, 1);
						cpt++;
					}
					this->CreateLandmarkUndoSet(1, cpt);

				}

				/**/

				inputFile.close();


			}

																			

		}//file exists...
	}	//length
	this->Render();


}
void mqMorphoDigCore::OpenLMK(QString fileName, int mode)
{// mode : 0 for normal landmarks
 // mode : 1 for target landmarks
 // mode : 2 for curve node landmarks
 // mode : 3 for curve handle landmarks
	double  x, y, z;
	QString LMKName;
	//Open a landmark file!

	cout << "Open a lmk file" << endl;
	size_t  length;


	length = fileName.toStdString().length();

	int done = 0;
	if (length>0)
	{
		int file_exists = 1;
		ifstream file(fileName.toLocal8Bit());
		if (file)
		{
			//std::cout<<"file:"<<filename.c_str()<<" exists."<<std::endl;
			file.close();
		}
		else
		{

			//std::cout << "file:" << fileName.toLocal8Bit() << " does not exists." << std::endl;
			file_exists = 0;
		}

		if (file_exists == 1)
		{

			std::string LMKext(".lmk");
			std::string LMKext2(".LMK");

			std::size_t found = fileName.toStdString().find(LMKext);
			std::size_t found2 = fileName.toStdString().find(LMKext2);
			if (found != std::string::npos || found2 != std::string::npos)
			{

				//filein = fopen(fileName.toLocal8Bit(), "rt");
				QFile inputFile(fileName);
				int ok = 0;

				if (inputFile.open(QIODevice::ReadOnly))
				{
					QTextStream in(&inputFile);
					int cpt = 0;
					while (!in.atEnd())
					{

						QString line = in.readLine();
						QTextStream myteststream(&line);
						myteststream >> LMKName >> x >> y >> z;
						double coord[3] = { x,y,z };
						double ori[3];


						ori[0] = 0;
						ori[1] = 0;
						ori[2] = 1;

						this->CreateLandmark(coord, ori, mode);
						cpt++;
					}
					this->CreateLandmarkUndoSet(mode, cpt);
					/**/

					inputFile.close();


				}

			}//fin if																		

		}//file exists...
	}	//length
	this->Render();

}
void mqMorphoDigCore::OpenPTS(QString fileName, int mode)
{// mode : 0 for normal landmarks
 // mode : 1 for target landmarks
 // mode : 2 for curve node landmarks
 // mode : 3 for curve handle landmarks
	double  x, y, z;
	QString LMKName;
	//Open a landmark file!

	cout << "Open a pts file" << endl;
	size_t  length;


	length = fileName.toStdString().length();

	int done = 0;
	if (length > 0)
	{
		int file_exists = 1;
		ifstream file(fileName.toLocal8Bit());
		if (file)
		{
			//std::cout<<"file:"<<filename.c_str()<<" exists."<<std::endl;
			file.close();
		}
		else
		{

			//std::cout << "file:" << fileName.toLocal8Bit() << " does not exists." << std::endl;
			file_exists = 0;
		}

		if (file_exists == 1)
		{

			std::string PTSext(".pts");
			std::string PTSext2(".PTS");

			std::size_t found = fileName.toStdString().find(PTSext);
			std::size_t found2 = fileName.toStdString().find(PTSext2);
			if (found != std::string::npos || found2 != std::string::npos)
			{

				//filein = fopen(fileName.toLocal8Bit(), "rt");
				QFile inputFile(fileName);
				int ok = 0;

				if (inputFile.open(QIODevice::ReadOnly))
				{
					QTextStream in(&inputFile);
					QString line0 = in.readLine(); // nothing interested
					line0 = in.readLine();
					QTextStream numlmk(&line0);
					vtkIdType num_lmk;
					numlmk >> num_lmk;
					vtkIdType cpt_lmk=0;
					//(two first lines are useless)
					while (!in.atEnd() && cpt_lmk < num_lmk)
					{

						QString line = in.readLine();
						QTextStream myteststream(&line);
						myteststream >> LMKName >> x >> y >> z;
						double coord[3] = { x,y,z };
						double ori[3];


						ori[0] = 0;
						ori[1] = 0;
						ori[2] = 1;

						this->CreateLandmark(coord, ori, mode);
						cpt_lmk++;
					}
					this->CreateLandmarkUndoSet(mode, cpt_lmk);
					/**/

					inputFile.close();


				}

			}//fin if																		

		}//file exists...
	}	//length
	this->Render();

}

void mqMorphoDigCore::OpenTPS(QString fileName, int mode)
{// mode : 0 for normal landmarks
 // mode : 1 for target landmarks
 // mode : 2 for curve node landmarks
 // mode : 3 for curve handle landmarks
	double  x, y, z;
	QString LMKName;
	//Open a landmark file!

	cout << "Open a tps file" << endl;
	size_t  length;


	length = fileName.toStdString().length();

	int done = 0;
	if (length > 0)
	{
		int file_exists = 1;
		ifstream file(fileName.toLocal8Bit());
		if (file)
		{
			//std::cout<<"file:"<<filename.c_str()<<" exists."<<std::endl;
			file.close();
		}
		else
		{

			//std::cout << "file:" << fileName.toLocal8Bit() << " does not exists." << std::endl;
			file_exists = 0;
		}

		if (file_exists == 1)
		{

			std::string TPSext(".tps");
			std::string TPSext2(".TPS");

			std::size_t found = fileName.toStdString().find(TPSext);
			std::size_t found2 = fileName.toStdString().find(TPSext2);
			if (found != std::string::npos || found2 != std::string::npos)
			{

				//filein = fopen(fileName.toLocal8Bit(), "rt");
				QFile inputFile(fileName);
				int ok = 0;

				if (inputFile.open(QIODevice::ReadOnly))
				{
					QTextStream in(&inputFile);
					QString line0 = in.readLine(); // first line is not interesting
					int cpt = 0;
					while (!in.atEnd())
					{

						QString line = in.readLine();
						QTextStream myteststream(&line);
						myteststream >> x >> y >> z;
						double coord[3] = { x,y,z };
						double ori[3];

						ori[0] = 0;
						ori[1] = 0;
						ori[2] = 1;

						this->CreateLandmark(coord, ori, mode);
						cpt++;
					}
					this->CreateLandmarkUndoSet(mode, cpt);
					/**/

					inputFile.close();


				}

			}//fin if																		

		}//file exists...
	}	//length
	this->Render();

}

void mqMorphoDigCore::OpenVER(QString fileName, int mode)
{// mode : 0 for normal landmarks
 // mode : 1 for target landmarks
 // mode : 2 for curve nodes
 // mode : 3 for curve handles
	double  x, y, z, nx, ny, nz;
	QString LMKName;
	//Open a landmark file!


	size_t  length;


	length = fileName.toStdString().length();

	int done = 0;
	if (length>0)
	{
		int file_exists = 1;
		ifstream file(fileName.toLocal8Bit());
		if (file)
		{
			//std::cout<<"file:"<<filename.c_str()<<" exists."<<std::endl;
			file.close();
		}
		else
		{

			std::cout << "file:" << fileName.toStdString().c_str() << " does not exists." << std::endl;
			file_exists = 0;
		}

		if (file_exists == 1)
		{

			std::string VERext(".ver");
			std::string VERext2(".VER");
			std::string LMKext(".LMK");
			std::string LMKext2(".LMK");

			int type = 0; // 0 = .POS Ascii File //1 = .MAT binary File or simple .MAT file

			std::size_t found = fileName.toStdString().find(LMKext);
			std::size_t found2 = fileName.toStdString().find(LMKext2);
			if (found != std::string::npos || found2 != std::string::npos)
			{
				type = 1;
				//LMK
			}

			found = fileName.toStdString().find(VERext);
			found2 = fileName.toStdString().find(VERext2);
			if (found != std::string::npos || found2 != std::string::npos)
			{
				type = 0; //VER
			}



			if (type == 1)
			{


			}
			else
			{
				//filein = fopen(fileName.toLocal8Bit(), "rt");
				QFile inputFile(fileName);
				int ok = 0;

				if (inputFile.open(QIODevice::ReadOnly))
				{
					QTextStream in(&inputFile);
					int cpt = 0;
					while (!in.atEnd())
					{

						QString line = in.readLine();
						QTextStream myteststream(&line);
						myteststream >> LMKName >> x >> y >> z >> nx >> ny >> nz;
						double coord[3] = { x,y,z };
						double ncoord[3] = { nx,ny,nz };
						double ori[3];

						double length = nx*nx + ny*ny + nz*nz;
						if (length == 1)
						{
							ori[0] = ncoord[0];
							ori[1] = ncoord[1];
							ori[2] = ncoord[2];
						}
						else
						{
							vtkMath::Subtract(ncoord, coord, ori);
							vtkMath::Normalize(ori);
						}
						this->CreateLandmark(coord, ori, mode);
						cpt++;

					}
					this->CreateLandmarkUndoSet(mode);
					/**/

					inputFile.close();


				}
			}//fin if																		

		}//file exists...
	}	//length
	this->Render();

}

void mqMorphoDigCore::OpenVolume(QString fileName)
{
	int file_exists = 1;
	QFile file(fileName);
	QString name = "";
	if (file.exists()) {
		// Message
		name = file.fileName(); // Return only a file name		
		file.close();
	}
	else
	{
		file_exists = 0;


	}


	if (file_exists == 1)
	{
		std::string MHDext(".mhd");
		std::string MHDext2(".MHD");
		std::string MHAext(".mha");
		std::string MHAext2(".MHA");
		std::string VTIext(".vti");
		std::string VTIext2(".VTI");

		int type = 0; //0 = mhd, 1 = mha, 2 = vti,
		std::size_t found = fileName.toStdString().find(MHDext);
		std::size_t found2 = fileName.toStdString().find(MHDext2);
		if (found != std::string::npos || found2 != std::string::npos)
		{
			type = 0;
			//MHD
		}

		
		found = fileName.toStdString().find(MHAext);
		found2 = fileName.toStdString().find(MHAext2);
		if (found != std::string::npos || found2 != std::string::npos )
		{
			type = 1; //MHA
		}

		
		found = fileName.toStdString().find(VTIext);
		found2 = fileName.toStdString().find(VTIext2);
		if (found != std::string::npos || found2 != std::string::npos)
		{
			type = 2; //VTI
		}
		vtkSmartPointer<vtkImageData> input = vtkSmartPointer<vtkImageData>::New();
		vtkSmartPointer<vtkAlgorithm> reader = vtkSmartPointer <vtkAlgorithm>::New();

		if (type < 2)
		{
			vtkSmartPointer <vtkMetaImageReader> metaReader = vtkSmartPointer<vtkMetaImageReader>::New();
			 metaReader->SetFileName(fileName.toLocal8Bit());
		     metaReader->Update();
		     input = metaReader->GetOutput();
		     reader = metaReader;		

		}
		else
		{ 
			vtkSmartPointer <vtkXMLImageDataReader> xmlReader = vtkSmartPointer<vtkXMLImageDataReader>::New();
			xmlReader->SetFileName(fileName.toLocal8Bit());
			xmlReader->Update();
			input = xmlReader->GetOutput();
			reader = xmlReader;

		
		}

		int dim[3];
		double spacing[3];
		input->GetDimensions(dim);
		input->GetSpacing(spacing);
		int numcells = input->GetNumberOfCells();
		input->GetScalarTypeAsString();
		//input->Get
		cout << "Read Volume: dim=" << dim[0] << ", " << dim[1] << ", " << dim[2] << "numcells="<<numcells<< endl;
		cout << "Dim0*Dim1*Dim2:" << dim[0]* dim[1]* dim[2] << endl;
		cout << "Spacing0*Spacing1*Spacing2:" << spacing[0] * spacing[1] * spacing[2] << endl;
		cout << "Image type:" << input->GetScalarTypeAsString() << endl;
		cout << "Image type int:" << input->GetScalarType() << "="<<VTK_UNSIGNED_SHORT<< "?"<<endl;
		cout << "Number of scalar components:" << input->GetNumberOfScalarComponents() << endl;
		
		
		cout << "Range min:" << input->GetScalarRange()[0] << ", Range max:" << input->GetScalarRange()[1] << endl;

	
		
		
		if (dim[0] < 2 ||
		        dim[1] < 2 ||
		        dim[2] < 2)
		{
			return;
		}
		else 
		{
			cout << "Try visualize!!!" << endl;

			vtkSmartPointer<vtkMDVolume> volume = vtkSmartPointer<vtkMDVolume>::New();
			vtkSmartPointer<vtkSmartVolumeMapper> mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
			//vtkSmartPointer <vtkOpenGLGPUVolumeRayCastMapper> mapper = vtkSmartPointer<vtkOpenGLGPUVolumeRayCastMapper>::New();
			//vtkSmartPointer <vtkGPUVolumeRayCastMapper> mapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New(); //NOthing works... 
			vtkSmartPointer<vtkDiscretizableColorTransferFunction> TF = vtkSmartPointer<vtkDiscretizableColorTransferFunction>::New();
			//vtkSmartPointer<vtkColorTransferFunction> colorFun = vtkSmartPointer <vtkColorTransferFunction>::New();
			vtkSmartPointer<vtkPiecewiseFunction> opacityFun = vtkSmartPointer<vtkPiecewiseFunction>::New();
			vtkSmartPointer<vtkImageAccumulate> histogram =
				  vtkSmartPointer<vtkImageAccumulate>::New();
			//mapper->SetRequestedRenderModeToDefault();
			
						
			
			volume->GetOutline()->SetInputData(input);

			histogram->SetInputData(input);
			double bin_spacing = 1;
			
			/*if (input->GetScalarType() ==  VTK_UNSIGNED_SHORT)
			{
				histogram->SetComponentExtent(VTK_UNSIGNED_SHORT_MIN, VTK_UNSIGNED_SHORT_MAX, 0, 0, 0, 0);
				histogram->SetComponentOrigin(0, 0, 0);
				
			}else if (input->GetScalarType() == VTK_SHORT)
			{

				histogram->SetComponentExtent(VTK_SHORT_MIN, VTK_SHORT_MAX, 0, 0, 0, 0);
				histogram->SetComponentOrigin(VTK_SHORT_MIN, 0, 0);
				
			}
			else if (input->GetScalarType() == VTK_CHAR)
			{				
				histogram->SetComponentExtent(VTK_CHAR_MIN, VTK_CHAR_MAX, 0, 0, 0, 0);
				histogram->SetComponentOrigin(VTK_CHAR_MIN, 0, 0);
				
			}
			else if (input->GetScalarType() == VTK_UNSIGNED_CHAR)
			{
				
				histogram->SetComponentExtent(VTK_UNSIGNED_CHAR_MIN, VTK_UNSIGNED_CHAR_MAX, 0, 0, 0, 0);
				histogram->SetComponentOrigin(VTK_UNSIGNED_CHAR_MIN, 0, 0);
				
			}
			else if (input->GetScalarType() == VTK_SIGNED_CHAR)
			{
				
				histogram->SetComponentExtent(VTK_SIGNED_CHAR_MIN, VTK_SIGNED_CHAR_MAX, 0, 0, 0, 0);
				histogram->SetComponentOrigin(VTK_SIGNED_CHAR_MIN, 0, 0);
				
			}
			else if (input->GetScalarType() == VTK_FLOAT)
			{
				
				//input->GetScalarRange()[0] input->GetScalarRange()[1]
				//histogram->SetComponentExtent(VTK_FLOAT_MIN, VTK_FLOAT_MAX, 0, 0, 0, 0);
				histogram->SetComponentOrigin(VTK_FLOAT_MIN, 0, 0);
				
			}
			else if (input->GetScalarType() == VTK_DOUBLE)
			{
				
				//histogram->SetComponentExtent(VTK_DOUBLE_MIN, VTK_DOUBLE_MAX, 0, 0, 0, 0);
				histogram->SetComponentOrigin(VTK_DOUBLE_MIN, 0, 0);
				
			}*/
			

			// bin_spacing :
			

			
			histogram->SetComponentExtent(input->GetScalarRange()[0], input->GetScalarRange()[1], 0, 0, 0, 0);
			histogram->SetComponentOrigin(input->GetScalarRange()[0], 0, 0);
			bin_spacing = (double)(input->GetScalarRange()[1] - input->GetScalarRange()[0]) / 100;
			//
			//histogram->SetComponentExtent(6544, 30540, 0, 0, 0, 0);
			//histogram->SetComponentOrigin(6544, 0, 0);
			//bin_spacing = (double)(30540 - 6544) / 100;

			//
			histogram->SetComponentSpacing(bin_spacing, 0, 0);
			cout << "Open volume: bin spacing = " << bin_spacing << endl;
			histogram->Update();
			// faire plutôt une liste avec push.
			/*
			std::vector<int> peaks;
			std::vector<int> peaksT;
			std::vector<int> peakVals;
			std::vector<int> lows;
			std::vector<int> lowsT;
			std::vector<int> lowVals;
			
			int dims[3];
			histogram->GetOutput()->GetDimensions(dims);
			cout << "Histogram (max) dims=" << dims[0] << ", " << dims[1] << ", " << dims[2]  << endl;
			vtkIdType used_bins = (vtkIdType)(dims[0] / bin_spacing);
			cout << "Histogram (used) bins=" << used_bins  << endl;
			int prevbin = 0;
		

			int p_or_l=1; //1 search peak //2 search low
			int p_i = 0;
			int l_i = -1;

			peaks.push_back(0);
			peaksT.push_back(0);
			peakVals.push_back(0);
			for (vtkIdType bin = 0; bin < used_bins; bin++)
			{
				int binT;
				if (input->GetScalarType() == VTK_UNSIGNED_SHORT) {
					binT = VTK_UNSIGNED_SHORT_MIN + bin * bin_spacing;}
				if (input->GetScalarType() == VTK_SHORT) {
					binT = VTK_SHORT_MIN + bin * bin_spacing;
				}
				int curbin = *(static_cast<int*>(histogram->GetOutput()->GetScalarPointer(bin, 0, 0)));
				cout << "bin:" << bin << ", curbin=" << curbin << endl;
					//histogram->GetOutput()->GetPointData()->GetScalars()->GetTuple1(bin);
				if (p_or_l == 1)//search peak
				{
					if (curbin >= prevbin)
					{
						if (curbin > 0)
						{
							peaks.at(p_i) = bin;
							peaksT.at(p_i) = binT;
							peakVals.at(p_i) = curbin;
						}
					}
					else
					{
						// starts do decrease: 
						p_or_l = 0;
						lows.push_back(bin);
						lowsT.push_back(binT);
						lowVals.push_back(curbin);
						l_i++; //cout << "l_i" << l_i << endl;
					}
				}
				if (p_or_l == 0)//search low
				{
					if (curbin < prevbin)
					{
						lows.at(l_i) = bin;
						lowsT.at(l_i) = binT;
						lowVals.at(l_i) = curbin;
					}
					else
					{
						// starts do re-increase: 
						p_or_l = 1;
						peaks.push_back(bin);
						peaksT.push_back(binT);
						peakVals.push_back(curbin);
						p_i++;
					}
				}
				
					//cout <<"bin="<<bin<<"|"<< *(static_cast<int*>(histogram->GetOutput()->GetScalarPointer(bin, 0, 0))) << " ";
				//	cout << histogram->GetOutput()->GetPointData()->GetScalars()->GetTuple1(bin) << endl;				
				prevbin = curbin;
			}
			
			for (int i = 0; i <= p_i;  i++)
			{
				//cout << "p" << i <<":"<< peaksT.at(i) << ", val=" << peakVals.at(i) << endl;
			}
			for (int i = 0; i <= l_i; i++)
			{
				//cout << "l" << i << ":" << lowsT.at(i) << ", val=" << lowVals.at(i) << endl;
			}
		
		*/
			  // Create the property and attach the transfer functions
			vtkSmartPointer < vtkVolumeProperty> property = vtkSmartPointer <vtkVolumeProperty>::New();
			property->SetIndependentComponents(true);
			volume->SetCtf(TF);
			property->SetColor(TF);
			property->SetScalarOpacity(opacityFun);
			property->SetInterpolationTypeToLinear();
			//mapper->SetInputData(input);
			mapper->SetInputConnection(reader->GetOutputPort());
			// connect up the volume to the property and the mapper
			volume->SetProperty(property);
			volume->SetMapper(mapper);
			volume->SetImageData(input);
			volume->SetHist(histogram);

			


			double first_point = 0; // as first low... a
			double last_point = 0; //somme pondérée autres peaks,  puis chercher s'il y a un low après cette valeur. Si oui, moyenne des deux. sinon on garde la moyenne pondérée.

			/*
			if (input->GetScalarType() == VTK_UNSIGNED_SHORT) {
				first_point = VTK_UNSIGNED_SHORT_MIN;
				last_point = VTK_UNSIGNED_SHORT_MAX;
			}
			else
			{
				first_point = VTK_SHORT_MIN;
				last_point = VTK_SHORT_MAX;
				
			}
			if (l_i >= 0) 
			{ 
				first_point = lowsT.at(0); 
				if (p_i > 0) {
					first_point = (int)(0.5*(lowsT.at(0) + peaksT.at(1)));
				}
			}

			//now search last point
			int avg_peaks= 0;

			int low_after_avgpeaks = 0;
			if (p_i>0)
			{ 
				double sum_peaks=0;
				double sum_peakVals=0;
				for (int i = 1; i <= p_i; i++)
				{
					//peaksT.at(i) peakVals.at(i) ;
					//cout << "peaksT.at(i)="<<peaksT.at(i) << endl;
					//cout << "peakVals.at(i)=" << peakVals.at(i) << endl;
					double mult = (double)peaksT.at(i)*(double)peakVals.at(i);
					//cout << "mult=" << mult << endl;
					sum_peakVals += (double)peakVals.at(i);
					sum_peaks += mult;
					//cout << "sum_peaks=" << sum_peaks << endl;
					//cout << "sum_peakVals=" << sum_peakVals << endl;

				}
				if (sum_peakVals>0)
				{
					avg_peaks = (int)(sum_peaks / sum_peakVals);
					//cout << "avg_peaks="<<avg_peaks << endl;
					// search if a low exists after avg_peaks
					int exists = 0;
					int i_low=0;
					if (l_i>=0)
					{ 
						for (int i = 0; i <= l_i; i++)
						{
							if (lowsT.at(i) > avg_peaks)
							{
								if (exists==0){ i_low = i; }
								exists = 1;
								
							}
						}
						if (exists == 1)
						{
							low_after_avgpeaks = lowsT.at(i_low);
							last_point = (int)((low_after_avgpeaks + avg_peaks) / 2);
						}
					}
					else
					{
						last_point = avg_peaks;
					}
				}
			}
			//let's put first point a little bit further!
			first_point = (int)(first_point + 0.2*(last_point - first_point));
			cout << "Range based on peaks and lows :" << first_point << "," << last_point << endl;
			*/
			
			//let's try something else

			double width = input->GetScalarRange()[1] - input->GetScalarRange()[0];


			first_point = input->GetScalarRange()[0] + 0.15*width;
			last_point = input->GetScalarRange()[1] - 0.3*width;
			cout << "Range based on 15p min and 5p low:" << first_point << "," << last_point << endl;
			cout << "EH???" << endl;

			double second_point = first_point + 0.2*(last_point - first_point);
			double third_point = first_point + 0.4*(last_point - first_point);
		
			TF->AddRGBPoint(first_point, 0, 0, 0, 0.5, 0);
			TF->AddRGBPoint(second_point, 0.73, 0, 0, 0.5, 0);
			TF->AddRGBPoint(third_point, .90, .82, .56, .5, 0);
			TF->AddRGBPoint(last_point, 1, 1, 1, .5, 0);

			opacityFun->AddPoint(first_point, 0, 0.5, 0);
			opacityFun->AddPoint(second_point, 0.5, .5, 0);
			opacityFun->AddPoint(third_point, 0.8, .5, 0);
			opacityFun->AddPoint(last_point, 1, 0.5, 0);
			TF->SetEnableOpacityMapping(true);
			TF->SetScalarOpacityFunction(opacityFun);
			TF->Build();
				mapper->SetBlendModeToComposite();
				  
				  
				  //property->set
				  property->SetAmbient(this->mui_Ambient);
			      property->SetDiffuse(this->mui_Diffuse);
			      property->SetSpecular(this->mui_Specular);
			      property->SetSpecularPower(this->mui_SpecularPower);
				  property->SetInterpolationTypeToLinear();
				  property->ShadeOn();
				  double bblength = volume->GetBoundingBoxLength();
				  double SOUD = bblength / 500;
				  cout << "Scalar Opacity Unit Distance:" << bblength << "/500=" << SOUD << endl;
				  if (SOUD == 0) { SOUD = 0.89; }
				 
			     property->SetScalarOpacityUnitDistance(SOUD); // Ca doit être fonction de la taille des spécimens, sinon ça va pas... 
				 mapper->Update();
				 volume->Update();
				 volume->SetSelected(1);
				 volume->SetScalarOpacityUnitDistance(SOUD);
				 volume->SetScalarDisplayMin((double)first_point);
				 volume->SetScalarDisplayMax((double)last_point);
				 volume->UpdateLookupTableRange();

				 QFileInfo fileInfo(fileName);
				 QString onlyfilename(fileInfo.fileName());
				 std::string only_filename = onlyfilename.toStdString();
				 std::string newname = only_filename.c_str();
				 size_t nPos = newname.find_last_of(".");
				 if (nPos > 0)
				 {

					 newname = newname.substr(0, nPos);
				 }

				 //@@TODO! 

				 newname = this->CheckingName(newname);
				 cout << "Volume Name= " << newname << endl;
				 //volume->SetOrientation(0, 0, 180);
				 volume->SetName(newname);
				 volume->SetColorProperties(this->mui_Ambient, this->mui_Diffuse, this->mui_Specular, this->mui_SpecularPower);
				 volume->InitCenter();
				 //this->getRenderer()->AddVolume(volume);
				 this->getVolumeCollection()->AddItem(volume);
				 //emit this->actorsMightHaveChanged();
				 emit this->volumesMightHaveChanged();
				 this->Initmui_ExistingArrays();
				 std::string action = "Load volume";
				 int mCount = BEGIN_UNDO_SET(action);
				 this->getVolumeCollection()->CreateLoadUndoSet(mCount, 1);
				 END_UNDO_SET();



				 this->getVolumeCollection()->SetChanged(1);

				 cout << "try adjust camera grid" << endl;
				 this->AdjustCameraAndGrid();
				 cout << "camera and grid adjusted" << endl;
				 
				 //cout << "camera and grid adjusted" << endl;

				 if (this->Getmui_AdjustLandmarkRenderingSize() == 1)
				 {
					 this->UpdateLandmarkSettings();
				 }
				 //this->getRenderer()->AddVolume(volume);
			

			
			this->Render();

		}
	}

}
int mqMorphoDigCore::OpenMesh(QString fileName)
{

	int file_exists = 1;
	int ok = 0;
	QFile file(fileName);
	QString name = "";
	if (file.exists()) {
		// Message
		name = file.fileName(); // Return only a file name		
		file.close();
	}
	else
	{
		file_exists = 0;


	}


	if (file_exists == 1)
	{
		std::string STLext(".stl");
		std::string STLext2(".STL");
		std::string VTKext(".vtk");
		std::string VTKext2(".VTK");
		std::string VTKext3(".vtp");
		std::string VTKext4(".VTP");
		std::string OBJext(".obj");
		std::string OBJext2(".OBJ");
		std::string PLYext(".ply");
		std::string PLYext2(".PLY");

		int type = 0; //0 = stl, 1 = vtk, 2 = obj, 3 = ply, 4: vtp (XML vtkPolyData)
		std::size_t found = fileName.toStdString().find(STLext);
		std::size_t found2 = fileName.toStdString().find(STLext2);
		if (found != std::string::npos || found2 != std::string::npos)
		{
			type = 0;
			//STL
		}

		//std::cout << "0Type= " <<type<< std::endl;
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
			type = 4; //VTP
		}
		//std::cout << "2Type= " <<type<< std::endl;
		found = fileName.toStdString().find(PLYext);
		found2 = fileName.toStdString().find(PLYext2);
		if (found != std::string::npos || found2 != std::string::npos)
		{
			type = 2; //PLY
		}

		//std::cout << "3Type= " <<type<< std::endl;
		found = fileName.toStdString().find(OBJext);
		found2 = fileName.toStdString().find(OBJext2);
		if (found != std::string::npos || found2 != std::string::npos)
		{
			type = 3; //OBJ
		}
		// Read and display for verification

		vtkSmartPointer<vtkPolyData> MyPolyData = vtkSmartPointer<vtkPolyData>::New();

		if (type == 0)
		{

			vtkSmartPointer<vtkSTLReader> reader =
				vtkSmartPointer<vtkSTLReader>::New();

			reader->SetFileName(fileName.toLocal8Bit());
			reader->Update();
			MyPolyData = reader->GetOutput();
		}

		else if (type == 1)
		{

			vtkSmartPointer<vtkPolyDataReader> reader =
				vtkSmartPointer<vtkPolyDataReader>::New();
			reader->SetFileName(fileName.toLocal8Bit());
			cout << "inside open mesh:"<<fileName.toStdString().c_str() << endl;
			//reader->SetFileName(fileName.toUtf8());
			reader->Update();
			MyPolyData = reader->GetOutput();
		}
		else if (type ==2)
		{

			vtkSmartPointer<vtkPLYReader> reader =
				vtkSmartPointer<vtkPLYReader>::New();
			reader->SetFileName(fileName.toLocal8Bit());
			reader->Update();
			MyPolyData = reader->GetOutput();
		}
		else if (type == 3)
		{
			vtkSmartPointer<vtkOBJReader> reader =
				vtkSmartPointer<vtkOBJReader>::New();
			reader->SetFileName(fileName.toLocal8Bit());
			reader->Update();
			MyPolyData = reader->GetOutput();
		}
		else if (type == 4)
		{
			vtkSmartPointer<vtkXMLPolyDataReader> reader =
				vtkSmartPointer<vtkXMLPolyDataReader>::New();
			reader->SetFileName(fileName.toLocal8Bit());
			reader->Update();
			MyPolyData = reader->GetOutput();
		}
		//std::cout << "\nNumber of points 1:" << MyPolyData->GetNumberOfPoints() << std::endl;
		//std::cout << "\nNumber of cells 1:" << MyPolyData->GetNumberOfCells() << std::endl;

	

		
		vtkSmartPointer<vtkCleanPolyData> cleanPolyDataFilter = vtkSmartPointer<vtkCleanPolyData>::New();
		//cleanPolyDataFilter->SetInputData(ObjNormals->GetOutput());
		cleanPolyDataFilter->SetInputData(MyPolyData);
		cleanPolyDataFilter->PieceInvariantOff();
		cleanPolyDataFilter->ConvertLinesToPointsOff();
		cleanPolyDataFilter->ConvertPolysToLinesOff();
		cleanPolyDataFilter->ConvertStripsToPolysOff();
		cleanPolyDataFilter->SetAbsoluteTolerance(0);
		cleanPolyDataFilter->PointMergingOn();
		
		cleanPolyDataFilter->Update();
		//Surface can contain a few "non-manifold" edges
		vtkSmartPointer<vtkFeatureEdges> featureEdges = vtkSmartPointer<vtkFeatureEdges>::New();
		featureEdges->SetInputData(cleanPolyDataFilter->GetOutput());
		featureEdges->BoundaryEdgesOff();
		featureEdges->FeatureEdgesOff();
		featureEdges->ManifoldEdgesOff();
		featureEdges->NonManifoldEdgesOn();
		
		featureEdges->Update();
		vtkIdType num_nonmanifold_pts = featureEdges->GetOutput()->GetNumberOfPoints();
		vtkIdType num_nonmanifold_cells = featureEdges->GetOutput()->GetNumberOfCells();
		cout << "NON MANIFOLD pts:" << num_nonmanifold_pts << " , cells:" << num_nonmanifold_cells << endl;

		MyPolyData = cleanPolyDataFilter->GetOutput();
		//Too long for he moment! 
		/*if (num_nonmanifold_pts > 0)
		{
			//UGLY STUFF TO REMOVE NON MANIFOLD PTS STARTS (I am so sorry...)
			// try to build a list of pts 
			vtkSmartPointer<vtkPolyData> Features = vtkSmartPointer<vtkPolyData>::New();
			
			vtkSmartPointer<vtkIdTypeArray> ids =
				vtkSmartPointer<vtkIdTypeArray>::New();
			ids->SetNumberOfComponents(1);
			Features = featureEdges->GetOutput();
			//featureEdges->Get
			//this thing is too long!
			for (vtkIdType i = 0; i < num_nonmanifold_pts; i++)
			{
				double fpt[3];
				double *mfpt;
				mfpt = Features->GetPoint(i);
				fpt[0] = mfpt[0]; fpt[1] = mfpt[1]; fpt[2] = mfpt[2];
				int found = 0;
				for (vtkIdType j = 0; j < MyPolyData->GetNumberOfPoints(); j++)
				{
					double pt[3];
					double *mpt;
					mpt = MyPolyData->GetPoint(j);
					pt[0] = mpt[0]; pt[1] = mpt[1]; pt[2] = mpt[2];
					if (pt[0] == fpt[0] && pt[1] == fpt[1] && pt[2] == fpt[2])
					{
						//cout << "Id=" << j << endl;
						ids->InsertNextValue(j);
					}
				}
			}

			//ids are in "selection nodes"
			vtkSmartPointer<vtkSelectionNode> selectionNode =
				vtkSmartPointer<vtkSelectionNode>::New();
			selectionNode->SetFieldType(vtkSelectionNode::POINT);
			selectionNode->SetContentType(vtkSelectionNode::INDICES);
			selectionNode->SetSelectionList(ids);
			selectionNode->GetProperties()->Set(vtkSelectionNode::CONTAINING_CELLS(), 1);
			selectionNode->GetProperties()->Set(vtkSelectionNode::INVERSE(), 1); // to select all but these points

			vtkSmartPointer<vtkSelection> selection =
				vtkSmartPointer<vtkSelection>::New();
			selection->AddNode(selectionNode);

			vtkSmartPointer<vtkExtractSelection> extractSelection =
				vtkSmartPointer<vtkExtractSelection>::New();

			extractSelection->SetInputData(0, MyPolyData);
			extractSelection->SetInputData(1, selection);

			extractSelection->Update();
			vtkSmartPointer<vtkUnstructuredGrid> notSelected =
				vtkSmartPointer<vtkUnstructuredGrid>::New();
			notSelected->ShallowCopy(extractSelection->GetOutput());
			vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter =
				vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
#
			surfaceFilter->SetInputData(notSelected);
			surfaceFilter->Update();

			
			MyPolyData = surfaceFilter->GetOutput(); 
		
		}
		*/
		//MyPolyData->SetActive
		cout << "\nNumber of points:" << MyPolyData->GetNumberOfPoints() << std::endl;
		cout << "\nNumber of cells:" << MyPolyData->GetNumberOfCells() << std::endl;

		MyPolyData->GetCellData();

		vtkFloatArray* norms = vtkFloatArray::SafeDownCast(MyPolyData->GetCellData()->GetNormals());
		//	cout << "Safe cell downcast done ! " << endl;
		if (norms)
		{

			cout << "There are here " << norms->GetNumberOfTuples()
				<< " Float Cell normals in norms" << endl;
		}
		else
		{
			cout << "FloatNorms CELL is null " << endl;
		}

		norms = vtkFloatArray::SafeDownCast
		(MyPolyData->GetPointData()->GetNormals());
		//cout << "Safe point downcast done ! " << endl;
		if (norms)
		{

			cout << "There are  " << norms->GetNumberOfTuples()
				<< " Float POINT normals in norms" << endl;
		}
		else
		{
			cout << "FloatNorms POINTS is null " << endl;
		}

		if (MyPolyData->GetNumberOfPoints() > 3)
		{
			cout << "More than 3 points!" << endl;
			VTK_CREATE(vtkMDActor, actor);
			if (this->mui_BackfaceCulling == 0)
			{
				actor->GetProperty()->BackfaceCullingOff();
			}
			else
			{
				actor->GetProperty()->BackfaceCullingOn();
			}
			

			QFileInfo fileInfo(fileName);
			QString onlyfilename(fileInfo.fileName());
			std::string only_filename = onlyfilename.toStdString();
			std::string newname = only_filename.c_str();
			size_t nPos = newname.find_last_of(".");
			if (nPos > 0)
			{

				newname = newname.substr(0, nPos);
			}

			//@@TODO! 
			
			newname = this->CheckingName(newname);
			cout << "Object Name= " << newname << endl;
			
			/*if ((vtkUnsignedCharArray*)MyPolyData->GetPointData()->GetScalars("RGB") != NULL)
			{
				QString RGB = QString("RGB");
				this->Addmui_ExistingArrays(RGB);
				//	MyPolyData->GetPointData()->SetScalars(NULL);
					cout << "found RGB colours! " << endl;
			}*/
			cout << "Current active scalar=" << this->mui_ActiveArray->Name.toStdString().c_str() << endl;

			if (MyPolyData->GetPointData()->GetScalars(this->mui_ActiveArray->Name.toStdString().c_str()) != NULL)
			{

				

				MyPolyData->GetPointData()->SetActiveScalars(this->mui_ActiveArray->Name.toStdString().c_str());
			}
			//MyPolyData->GetPointData()->SetActiveScalars(NULL);
			/*
			vtkSmartPointer<vtkUnsignedCharArray> newcolors =
			vtkSmartPointer<vtkUnsignedCharArray>::New();
			newcolors->SetNumberOfComponents(4);
			newcolors->SetNumberOfTuples(numpoints);
			//ici init_RGB ou RGB_i
			if ((vtkUnsignedCharArray*)MyObj->GetPointData()->GetScalars("RGB") != NULL) {
			newcolors->DeepCopy((vtkUnsignedCharArray*)MyObj->GetPointData()->GetScalars("RGB"));

			for (int i = 0; i < numpoints; i++)
			{
			if (i < 100)
			{
			cout << newcolors->GetComponent(i, 0) << "," << newcolors->GetComponent(i, 1)
			<< "," << newcolors->GetComponent(i, 2) << std::endl;
			}
			//newcolors->SetComponent(i, 3, 255.);

			}

			cout << "found RGB colours: ";
			newcolors->SetName("Init_RGB");
			My_Obj->GetPointData()->AddArray(newcolors);
			}

			*/

			// THIS PIECE OF CODE WORKS!
			//auto mapper2 = vtkSmartPointer<vtkPolyDataMapper>::New();
			/*mapper2->SetInputData(MyPolyData);
			mapper2->Update();
			//mapper->SetInputConnection(clean1->GetOutputPort());


			auto actor2 = vtkSmartPointer<vtkActor>::New();
			actor2->SetMapper(mapper2);
			this->Renderer->AddActor(actor2);*/
			//END THIS PIECE OF CODE WORKS

			// Mapper
			VTK_CREATE(vtkPolyDataMapper, mapper);
			
			//mapper->ImmediateModeRenderingOn();
			//mapper->SetColorModeToDirectScalars();
			mapper->SetColorModeToDefault();

			
			
			// Decide which lut should be set!
			if (
				(this->mui_ActiveArray->DataType == VTK_INT || this->mui_ActiveArray->DataType == VTK_UNSIGNED_INT)
				&& this->mui_ActiveArray->NumComp == 1
				)
			{
				
				mapper->SetScalarRange(0, this->Getmui_ActiveTagMap()->numTags - 1);
				mapper->SetLookupTable(this->Getmui_ActiveTagMap()->TagMap);
			}
			else
			{

				//mapper->SetScalarRange(0,200);								
				
				mapper->SetLookupTable(this->Getmui_ActiveColorMap()->ColorMap);
				//mapper->UseLookupTableScalarRangeOff();
				//cout << "Set color map" << this->Getmui_ActiveColorMap()->Name.toStdString() << endl;
			}

			
			
			//mapper->ScalarVisibilityOn();
			//mapper->ScalarVisibilityOff();
			mapper->SetInputData(MyPolyData);
			//VTK_CREATE(vtkActor, actor);

			int num = 2;

			actor->SetmColor(this->Getmui_MeshColor());
			
			actor->SetMapper(mapper);
			actor->SetSelected(1);
			actor->SetName(newname);
			actor->SetColorProperties(this->mui_Ambient, this->mui_Diffuse, this->mui_Specular, this->mui_SpecularPower);
			actor->SetDisplayMode(this->mui_DisplayMode);
			//actor->SetOrientation(0, 0, 180);
			//actor->GetProperty()->SetRepresentationToPoints();
			this->getActorCollection()->AddItem(actor);
			ok = 1;
			
			/*emit this->actorsMightHaveChanged(); 
			this->Initmui_ExistingArrays();
			std::string action = "Load surface file";
			int mCount = BEGIN_UNDO_SET(action);
			this->getActorCollection()->CreateLoadUndoSet(mCount, 1);
			END_UNDO_SET();*/



			this->getActorCollection()->SetChanged(1);

			//double BoundingBoxLength = MyPolyData->GetLength();
			/*this->AdjustCameraAndGrid();
			//cout << "camera and grid adjusted" << endl;

			if (this->Getmui_AdjustLandmarkRenderingSize() == 1)
			{
				this->UpdateLandmarkSettings();
			}*/
			/*
			double bounds[6];
			MyPolyData->GetBounds(bounds);
			vtkSmartPointer<vtkElevationFilter> elevation =
			vtkSmartPointer<vtkElevationFilter>::New();
			elevation->SetInputData(MyPolyData);
			elevation->SetLowPoint(0, bounds[2], 0);
			elevation->SetHighPoint(0, bounds[3], 0);
			elevation->Update();
			vtkSmartPointer<vtkBandedPolyDataContourFilter> bcf =
			vtkSmartPointer<vtkBandedPolyDataContourFilter>::New();
			bcf->SetInputConnection(elevation->GetOutputPort());
			bcf->SetScalarModeToValue();
			bcf->GenerateContourEdgesOn();

			bcf->GenerateValues(10, elevation->GetScalarRange());

			bcf->Update();
			//bcf->GetNumberOfContours();
			vtkSmartPointer<vtkPolyDataMapper> contourLineMapper =
			vtkSmartPointer<vtkPolyDataMapper>::New();
			contourLineMapper->SetInputData(bcf->GetContourEdgesOutput());

			cout<<"Number of contours:"<< bcf->GetNumberOfContours();

			contourLineMapper->SetScalarRange(elevation->GetScalarRange());
			contourLineMapper->SetResolveCoincidentTopologyToPolygonOffset();
			contourLineMapper->Update();

			vtkSmartPointer<vtkMDActor> contourLineActor =
			vtkSmartPointer<vtkMDActor>::New();
			contourLineActor->SetMapper(contourLineMapper);
			contourLineActor->GetProperty()->SetColor(0.5, 0.5, 1.0);
			this->getRenderer()->AddActor(contourLineActor);

			vtkSmartPointer<vtkPolyDataMapper> mapper2 =
			vtkSmartPointer<vtkPolyDataMapper>::New();
			mapper2->SetInputConnection(bcf->GetOutputPort());
			mapper2->SetScalarModeToUseCellData();
			//mapper2->SetScalarRange(0, 1);
			vtkSmartPointer<vtkMDActor> actor2=
			vtkSmartPointer<vtkMDActor>::New();
			actor2->SetMapper(mapper2);
			this->getRenderer()->AddActor(actor2);
			*/
			/*if (this->Getmui_CameraCentreOfMassAtOrigin() == 0)
			{
			double globalcenterofmass[3];
			this->GetGlobalCenterOfMass(globalcenterofmass);
			cout << "Center of mass of all opened surfaces is " << globalcenterofmass[0] << " " << globalcenterofmass[1] << " " << globalcenterofmass[2] << endl;

			double GlobalBoundingBoxLength = this->GetGlobalBoundingBoxLength();
			cout << "Global Bounding Box length is " << GlobalBoundingBoxLength << " mm" << endl;

			double campos[3];
			this->getCamera()->GetPosition(campos);
			double camfocalpoint[3];
			this->getCamera()->GetFocalPoint(camfocalpoint);
			double camscale = this->getCamera()->GetParallelScale();

			double movex, movey, movez;
			movex = (campos[0] - camfocalpoint[0])*GlobalBoundingBoxLength / camscale;
			movey = (campos[1] - camfocalpoint[1])*GlobalBoundingBoxLength / camscale;
			movez = (campos[2] - camfocalpoint[2])*GlobalBoundingBoxLength / camscale;
			this->getCamera()->SetPosition
			(globalcenterofmass[0] + movex,
			globalcenterofmass[1] + movey,
			globalcenterofmass[2] + movez);
			//this->getCamera()->SetPosition(center[0] + GlobalBoundingBoxLength, center[1], center[2]);
			this->getCamera()->SetFocalPoint(globalcenterofmass[0], globalcenterofmass[1], globalcenterofmass[2]);
			this->getCamera()->SetParallelScale(GlobalBoundingBoxLength);
			}*/
			//this->getCamera()->ParallelProjectionOn();


			//this->UpdateRenderer();

			//My_Obj = Cont_Mesh.Mesh_PDcontainerload(MyObj, (char*)newname.c_str());

			/*My_Obj->Set_Active_Scalar();
			int numpoints = My_Obj->GetNumberOfPoints();
			int numtriangles = My_Obj->GetNumberOfCells();
			std::cout << "Number of points:" << numpoints << std::endl;
			std::cout << "Number of cells:" << numtriangles << std::endl;

			//std::cout << "2 Mean x:"<<My_Obj->mean[0]<< "Mean y:"<<My_Obj->mean[1]<< "Mean z:"<<My_Obj->mean[2]<< std::endl;

			//std::cout << "3 Mean x:"<<My_Obj->mean[0]<< "Mean y:"<<My_Obj->mean[1]<< "Mean z:"<<My_Obj->mean[2]<< std::endl;

			My_Obj->selected = 1;


			cout << "color init: ";
			vtkSmartPointer<vtkUnsignedCharArray> newcolors =
			vtkSmartPointer<vtkUnsignedCharArray>::New();
			newcolors->SetNumberOfComponents(4);
			newcolors->SetNumberOfTuples(numpoints);
			//ici init_RGB ou RGB_i
			if ((vtkUnsignedCharArray*)MyObj->GetPointData()->GetScalars("RGB") != NULL) {
			newcolors->DeepCopy((vtkUnsignedCharArray*)MyObj->GetPointData()->GetScalars("RGB"));

			for (int i = 0; i < numpoints; i++)
			{
			if (i < 100)
			{
			cout << newcolors->GetComponent(i, 0) << "," << newcolors->GetComponent(i, 1)
			<< "," << newcolors->GetComponent(i, 2) << std::endl;
			}
			//newcolors->SetComponent(i, 3, 255.);

			}

			cout << "found RGB colours: ";
			newcolors->SetName("Init_RGB");
			My_Obj->GetPointData()->AddArray(newcolors);
			}
			cout << "ok." << endl;

			My_Obj->color[0] = color_obj[0];
			My_Obj->color[1] = color_obj[1];
			My_Obj->color[2] = color_obj[2];
			My_Obj->color[3] = 1;

			My_Obj->bool_init_buf = 0;
			// Only update RGB if not exists!

			vtkUnsignedCharArray* test = (vtkUnsignedCharArray*)My_Obj->GetPointData()->GetScalars("RGB");
			if (test == NULL)
			{
			My_Obj->Update_RGB();
			}


			//std::cout << "4 Mean x:"<<My_Obj->mean[0]<< "Mean y:"<<My_Obj->mean[1]<< "Mean z:"<<My_Obj->mean[2]<< std::endl;


			//Move object at center of mass only in some cases
			if (g_move_cm == 1)
			{
			My_Obj->Mat2[3][0] = -My_Obj->mean[0];
			My_Obj->Mat2[3][1] = -My_Obj->mean[1];
			My_Obj->Mat2[3][2] = -My_Obj->mean[2];
			}

			this->Compute_Global_Mean(0);
			if (g_landmark_auto_rendering_size)
			{
			this->Adjust_landmark_rendering_size();
			}
			this->Compute_Global_Scalar_List();

			}
			cout << "Reinitialize camera" << endl;
			rollinit_camera();
			cout << "G_zoom after initialization:" << g_zoom << endl;
			this->redraw();
			cout << "G_zoom after redraw:" << g_zoom << endl;*/

		}

	}
	else
	{
		return 0;
	}


	
	//this->MainWindow->vtkWidgetUpdate();
	//cout << "call matchTagMapToActorCollection" << endl;
	return ok;

	/*this->Render();
	this->matchTagMapToActorCollection();*/

}
void mqMorphoDigCore::OpenNTW(QString fileName)
{


	int file_exists = 1;
	int cpt_surfaces = 0;
	int i = 0;
	QFile file(fileName);
	QFileInfo fileInfo(fileName);
	QString onlyfilename(fileInfo.fileName());
	if (file.exists()) {
		// Message
		file.close();
	}
	else
	{
		file_exists = 0;

	}


	if (file_exists == 1)
	{
		cout << "found file!!!!" << endl;


		std::string only_filename = onlyfilename.toStdString();;
		std::string path = fileName.toStdString().substr(0, (fileName.toStdString().length() - only_filename.length()));
		cout << "only_filename" << only_filename << endl;
		cout << "path" << path << endl;
		this->UnselectAll(-1);





		QFile inputFile(fileName);
		int ok = 0;

		if (inputFile.open(QIODevice::ReadOnly))
		{
			QTextStream in(&inputFile);
			in.setCodec("UTF-8");
			int surface_file = 0;
			int volume_file = 0;
			while (!in.atEnd())
			{
				QString line = in.readLine();
				cout << "Line:" << line.toStdString() << endl;
				//sscanf(line.toStdString().c_str(), "%[^\n]s", param1);
				//cout << "param1" << param1 << endl;
				//std::string myline = param1;
				std::string myline = line.toStdString();
				cout << "My line:" << myline << endl;
				std::string FLGext(".flg");
				std::string FLGext2(".FLG");
				std::string VERext(".ver");
				std::string VERext2(".VER");
				std::string CURext(".cur");
				std::string CURext2(".CUR");
				std::string STVext(".stv");
				std::string STVext2(".STV");
				std::string TAGext(".tag");
				std::string TAGext2(".TAG");
				std::string TAGext3(".tgp");
				std::string TAGext4(".TGP");
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

				int lmk_file = 0; // top lines
				
				std::size_t found = myline.find(FLGext);
				std::size_t found2 = myline.find(FLGext2);
				if (found != std::string::npos || found2 != std::string::npos)
				{
					lmk_file = 1;
					// Now open flag file!
					QFileInfo flgfileInfo(line);
					QString onlyflgfilename(flgfileInfo.fileName());
					std::string flgfilename = onlyflgfilename.toStdString();
					if (myline.length() == flgfilename.length())
					{
						myline = path.c_str();
						myline.append(flgfilename.c_str());
					}
					std::cout << "Try to load flag file :<<" << myline.c_str() << std::endl;

					QString flgfile(myline.c_str());
					this->OpenFLG(flgfile);

				}

				found = myline.find(VERext);
				found2 = myline.find(VERext2);
				if (found != std::string::npos || found2 != std::string::npos)
				{
					lmk_file = 1;
					int landmark_mode = 0;
					// Now open ver file!
					QFileInfo verfileInfo(line);
					QString onlyverfilename(verfileInfo.fileName());
					std::string verfilename = onlyverfilename.toStdString();
					if (myline.length() == verfilename.length())
					{
						myline = path.c_str();
						myline.append(verfilename.c_str());
					}
					std::cout << "Try to load landmark file :<<" << myline.c_str() << std::endl;

					QString verfile(myline.c_str());
					this->OpenVER(verfile, landmark_mode);


				}

				found = myline.find(CURext);
				found2 = myline.find(CURext2);
				if (found != std::string::npos || found2 != std::string::npos)
				{
					lmk_file = 1;
					// Now open cur file!
					QFileInfo curfileInfo(line);
					QString onlycurfilename(curfileInfo.fileName());
					std::string curfilename = onlycurfilename.toStdString();
					if (myline.length() == curfilename.length())
					{
						myline = path.c_str();
						myline.append(curfilename.c_str());
					}
					std::cout << "Try to load CUR curve file :<<" << myline.c_str() << std::endl;
					QString curfile(myline.c_str());
					this->OpenCUR(curfile);

				}

				found = myline.find(STVext);
				found2 = myline.find(STVext2);
				if (found != std::string::npos || found2 != std::string::npos)
				{
					lmk_file = 1;
					// Now open STV file!
					QFileInfo stvfileInfo(line);
					QString onlystvfilename(stvfileInfo.fileName());
					std::string stvfilename = onlystvfilename.toStdString();
					if (myline.length() == stvfilename.length())
					{
						myline = path.c_str();
						myline.append(stvfilename.c_str());
					}
					std::cout << "Try to load STV curve file :<<" << myline.c_str() << std::endl;
					QString stvfile(myline.c_str());
					this->OpenSTV(stvfile);


				}
				found = myline.find(ORIext);
				found2 = myline.find(ORIext2);
				if (found != std::string::npos || found2 != std::string::npos)
				{
					lmk_file = 1;
					// Now open ORI file!
					QFileInfo orifileInfo(line);
					QString onlyorifilename(orifileInfo.fileName());
					std::string orifilename = onlyorifilename.toStdString();
					if (myline.length() == orifilename.length())
					{
						myline = path.c_str();
						myline.append(orifilename.c_str());
					}
					std::cout << "Try to load orientation file :<<" << myline.c_str() << std::endl;
					QString orifile(myline.c_str());
					this->OpenORI(orifile);

				}

				found = myline.find(TAGext);
				found2 = myline.find(TAGext2);
				std::size_t found3 = myline.find(TAGext3);
				std::size_t found4 = myline.find(TAGext4);
				 
				if (found != std::string::npos || found2 != std::string::npos || found3 != std::string::npos || found4 != std::string::npos
					)
				{
					lmk_file = 1;
					// Now open TAG file!
					QFileInfo tagfileInfo(line);
					QString onlytagfilename(tagfileInfo.fileName());
					std::string tagfilename = onlytagfilename.toStdString();
					if (myline.length() == tagfilename.length())
					{
						myline = path.c_str();
						myline.append(tagfilename.c_str());
					}
					std::cout << "Try to load tag map file :<<" << myline.c_str() << std::endl;
					QString tagfile(myline.c_str());
					this->OpenTAGMAP(tagfile);


				}
				found = myline.find(STLext);
				found2 = myline.find(STLext2);
				if (found != std::string::npos || found2 != std::string::npos)
				{
					surface_file = 1; volume_file = 0;
					
				}

				found = myline.find(VTKext);
				found2 = myline.find(VTKext2);
				found3 = myline.find(VTKext3);
				found4 = myline.find(VTKext4);
				if (found != std::string::npos || found2 != std::string::npos || found3 != std::string::npos || found4 != std::string::npos)
				{
					surface_file = 1; volume_file = 0;
				}

				found = myline.find(PLYext);
				found2 = myline.find(PLYext2);

				if (found != std::string::npos || found2 != std::string::npos)
				{
					surface_file = 1; volume_file = 0; 
				}

				//std::cout << "2Type= " <<type<< std::endl;
				found = myline.find(OBJext);
				found2 = myline.find(OBJext2);
				if (found != std::string::npos || found2 != std::string::npos)
				{
					surface_file = 1; volume_file = 0;
				}
				found = myline.find(MHAext);
				found2 = myline.find(MHAext2);
				if (found != std::string::npos || found2 != std::string::npos)
				{
					std::cout << "found a MHA file!:<<" << myline.c_str() << std::endl;
					surface_file = 0; volume_file = 1;
				}
				found = myline.find(MHDext);
				found2 = myline.find(MHDext2);
				if (found != std::string::npos || found2 != std::string::npos)
				{
					surface_file = 0; volume_file = 1;
					std::cout << "found a MHD file!:<<" << myline.c_str() <<", volume_file ="<< volume_file << std::endl;
				
				}
				else
				{
					//cout << "no mhd????" << myline << endl;
				}
				found = myline.find(VTIext);
				found2 = myline.find(VTIext2);
				if (found != std::string::npos || found2 != std::string::npos)
				{
					std::cout << "found a VTI file!:<<" << myline.c_str() << std::endl;
					surface_file = 0; volume_file = 1;
				}

				//NOW THE SURFACES and VOLUMES!!!
				//SURFACES STRUCTURE : 
						// i= 0 surface file
						// i= 1 pos file
						// i= 2 color and opacity.

						//VOLUME STRUCTURE : 
						// i= 0 volume file
						// i= 1 pos file
						// i= 2 map file.

				if (lmk_file == 0)
				{
					cout << "Definitely not a landmark file. Ok status:" <<ok<< endl;
					if (surface_file == 1)
					{
						cout << "A surface file or related with surfaces" << endl;
						if (i == 0)
						{
							//length=(int)strlen(oneline);						
							//strncpy(param1, oneline, length-1);
							std::string meshname = line.toStdString();
							QFileInfo meshfileInfo(line);
							QString onlymeshfilename(meshfileInfo.fileName());
							std::string meshfilename = onlymeshfilename.toStdString();

							if (meshname.length() == meshfilename.length())
							{
								meshname = path.c_str();
								meshname.append(meshfilename.c_str());
							}
							QString meshfile(meshname.c_str());


							int totoche = this->OpenMesh(meshfile);
							//@@@
							//if (totoche == 1) { cpt_surfaces++; }
							vtkMDActor* actor = this->GetLastActor();

							if (actor != NULL && actor->GetNumberOfPoints() > 10)
							{

								ok = 1; cpt_surfaces++;
								cout << "Object has more than 10 points <<" << endl;
							}
							else
							{
								ok = 0;
							}


						}
						if (i == 1)
						{


							if (ok)
							{

								//length= (int)strlen(oneline);						
								//strncpy(param1, oneline, length-1);
								std::string posfile = line.toStdString();
								// Now open TAG file!
								QFileInfo posfileInfo(line);
								QString onlyposfilename(posfileInfo.fileName());
								std::string posfilename = onlyposfilename.toStdString();

								if (posfile.length() == posfilename.length())
								{
									posfile = path.c_str();
									posfile.append(posfilename.c_str());
								}
								std::cout << "Try to load position :<<" << posfile.c_str() << std::endl;
								QString qposfile(posfile.c_str());
								this->OpenPOS(qposfile, 0);
								//@@TODO!
								//this->Open_POS_File(posfile, My_Obj);
								//std::cout <<"Object <<"<<My_Obj->name.c_str()<<">> position loaded"<< std::endl;
								//My_Obj->selected = 0;
							}
						}
						if (i == 2)
						{
							if (ok)
							{
								vtkMDActor *actor = this->GetLastActor();
								double r, g, b, a;
								QTextStream myteststream(&line);
								myteststream >> r >> g >> b >> a;
								if (r > 1 || g > 1 || b > 1) {
									r /= 255; g /= 255;
									b /= 255;
								}


								actor->SetmColor(r, g, b, a);
								actor->SetSelected(0);
								actor->SetChanged(1);

								this->getActorCollection()->SetChanged(1);
								/*sscanf(oneline, "%f %f %f %f\n", &color1, &color2, &color3, &color4);
								//std::cout <<"color 1"<<color1<<",color 2"<<color3<<",color 3"<<color3<<",color 4"<<color4<< std::endl;
								My_Obj->color[0] = color1; My_Obj->color[1] = color2; My_Obj->color[2] = color3; My_Obj->color[3] = color4;
								My_Obj->blend = color4;
								My_Obj->Update_RGB();*/
							}
						}
						i++;
						if (i > 2)
						{
							i = 0;
						}
					}
					else if (volume_file == 1)
					{
						cout << "A volume file or related with volume" << endl;
						if (i == 0)
						{
							cout << "i=" << i << endl;
							//length=(int)strlen(oneline);						
							//strncpy(param1, oneline, length-1);
							std::string volname = line.toStdString();
							QFileInfo volfileInfo(line);
							QString onlyvolumefilename(volfileInfo.fileName());
							std::string volumefilename = onlyvolumefilename.toStdString();

							if (volname.length() == volumefilename.length())
							{
								volname = path.c_str();
								volname.append(volumefilename.c_str());
							}
							QString volumefile(volname.c_str());


							cout << "Open volume!" << endl;
							this->OpenVolume(volumefile);
							cout << "Open volume done!" << endl;
							vtkMDVolume* volume = this->GetLastVolume();

							if (volume != NULL)
							{

								ok = 1;								
							}
							else
							{
								ok = 0;
							}
						}
						if (i == 1)
						{
							cout << "i =1" << endl;
							if (ok)
							{

								//length= (int)strlen(oneline);						
								//strncpy(param1, oneline, length-1);
								std::string posfile = line.toStdString();
								// Now open TAG file!
								QFileInfo posfileInfo(line);
								QString onlyposfilename(posfileInfo.fileName());
								std::string posfilename = onlyposfilename.toStdString();

								if (posfile.length() == posfilename.length())
								{
									posfile = path.c_str();
									posfile.append(posfilename.c_str());
								}
								std::cout << "Try to load position vor a volume <<" << posfile.c_str() << std::endl;
								QString qposfile(posfile.c_str());
								this->OpenPOS(qposfile, 3);								
							}
						}
						if (i == 2)
						{
							cout << "i=" << i << endl;
							if (ok)
							{

								//length= (int)strlen(oneline);						
								//strncpy(param1, oneline, length-1);
								std::string mapfile = line.toStdString();
								// Now open TAG file!
								QFileInfo mapfileInfo(line);
								QString onlymapfilename(mapfileInfo.fileName());
								std::string mapfilename = onlymapfilename.toStdString();

								if (mapfile.length() == mapfilename.length())
								{
									mapfile = path.c_str();
									mapfile.append(mapfilename.c_str());
								}
								std::cout << "Try to load ColorTransform Function .map .. and apply it to a volume!<<" << mapfile.c_str() << std::endl;
								QString qmapfile(mapfile.c_str());
								this->OpenMAP(qmapfile, 1);
							}
						}
						if (i == 3)
						{
							cout << "i=" << i << endl;
							if (ok)
							{
								vtkMDVolume *volume = this->GetLastVolume();
								double Scalar_opacity, Scalar_min, Scalar_max;
								QTextStream myteststream(&line);
								myteststream >> Scalar_opacity >> Scalar_min >> Scalar_max;
								cout << "Scalar_opacity =" << Scalar_opacity << endl;
								cout << "Min and Max:" << Scalar_min <<","<< Scalar_max<< endl;
								volume->SetScalarOpacityUnitDistance(Scalar_opacity);
								volume->SetScalarDisplayMin(Scalar_min);
								volume->SetScalarDisplayMax(Scalar_max);
								volume->UpdateLookupTableRange();
								volume->SetSelected(0);
								volume->SetChanged(1);

								this->getVolumeCollection()->SetChanged(1);
							}
						}
						i++;
						if (i > 3)
						{
							i = 0;
						}
					}//volume_file == 1
				}//if lmk_file == 0

			}
			inputFile.close();
		}
		if (cpt_surfaces > 0) { this->CreateSurfaceUndoSet(cpt_surfaces); }
		this->Render();
	}
}

void mqMorphoDigCore::OpenORI(QString fileName)
{

	QString X1, X2, Y1, Y2, Z1, Z2;





	size_t  length;


	length = fileName.toStdString().length();

	int done = 0;
	if (length>0)
	{
		int file_exists = 1;
		ifstream file(fileName.toLocal8Bit());
		if (file)
		{
			//std::cout<<"file:"<<filename.c_str()<<" exists."<<std::endl;
			file.close();
		}
		else
		{

			std::cout << "file:" << fileName.toStdString().c_str() << " does not exists." << std::endl;
			file_exists = 0;
		}

		if (file_exists == 1)
		{

			std::string ORIext(".ori");
			std::string ORIext2(".ORI");


			int type = 0; // 0 = .POS Ascii File //1 = .MAT binary File or simple .MAT file

			std::size_t found = fileName.toStdString().find(ORIext);
			std::size_t found2 = fileName.toStdString().find(ORIext2);
			if (found != std::string::npos || found2 != std::string::npos)
			{
				type = 1;
				//ORI
			}

			int cpt = 0;


			if (type == 1)
			{

				//filein = fopen(fileName.toLocal8Bit(), "rt");
				QFile inputFile(fileName);
				int ok = 0;

				if (inputFile.open(QIODevice::ReadOnly))
				{
					QTextStream in(&inputFile);

					while (!in.atEnd())
					{

						QString line = in.readLine();
						if (cpt == 0) { Z1 = line; }
						if (cpt == 1) { Z2 = line; }
						if (cpt == 2) { Y1 = line; }
						if (cpt == 3) { Y2 = line; }
						if (cpt == 4) { X1 = line; }
						if (cpt == 5) { X2 = line; }
						cpt++;

					}


					inputFile.close();
					this->Setmui_X1Label(X1);
					this->Setmui_X2Label(X2);
					this->Setmui_Y1Label(Y1);
					this->Setmui_Y2Label(Y2);
					this->Setmui_Z1Label(Z1);
					this->Setmui_Z2Label(Z2);
					this->ResetOrientationHelperLabels();


				}
			}//fin if																		

		}//file exists...
	}	//length*/
	this->Render();
}

void mqMorphoDigCore::OpenCUR(QString fileName)

{
	double  xn, yn, zn, xh, yh, zh;// coordinates of curve nodes and curve handles
	int node_type;
	QString LMKName;
	//Open a landmark file!


	size_t  length;


	length = fileName.toStdString().length();

	int done = 0;
	if (length>0)
	{
		int file_exists = 1;
		ifstream file(fileName.toLocal8Bit());
		if (file)
		{
			//std::cout<<"file:"<<filename.c_str()<<" exists."<<std::endl;
			file.close();
		}
		else
		{

			std::cout << "file:" << fileName.toStdString().c_str() << " does not exists." << std::endl;
			file_exists = 0;
		}

		if (file_exists == 1)
		{

			std::string CURext(".cur");
			std::string CURext2(".CUR");


			int type = 0;

			std::size_t found = fileName.toStdString().find(CURext);
			std::size_t found2 = fileName.toStdString().find(CURext2);
			if (found != std::string::npos || found2 != std::string::npos)
			{
				type = 1;
				//CUR
			}



			if (type == 1)
			{

				//filein = fopen(fileName.toLocal8Bit(), "rt");
				QFile inputFile(fileName);
				int ok = 0;

				if (inputFile.open(QIODevice::ReadOnly))
				{
					QTextStream in(&inputFile);
					int cpt_node = 0;
					int cpt_handle = 0;
					while (!in.atEnd())
					{

						QString line = in.readLine();
						QTextStream myteststream(&line);
						myteststream >> LMKName >> xn >> yn >> zn >> xh >> yh >> zh >> node_type;
						double coordn[3] = { xn,yn,zn };
						double coordh[3] = { xh,yh,zh };
						double ori[3];


						ori[0] = 0;
						ori[1] = 0;
						ori[2] = 1;

						this->CreateLandmark(coordn, ori, 2, node_type);
						cpt_node++;
						this->CreateLandmark(coordh, ori, 3);
						cpt_handle++;

					}
					int cpt = cpt_node + cpt_handle;
					this->CreateLandmarkUndoSet(-1, cpt, 0 , 0, cpt_node, cpt_handle);
					/**/

					inputFile.close();


				}
				this->Render();
			}//fin if																		

		}//file exists...
	}	//length

		
}
void mqMorphoDigCore::OpenSTV(QString fileName)
{
	double  x, y, z, nx, ny, nz;
	QString LMKName;
	//Open a STV file!


	size_t  length;

	int type = 1;
	length = fileName.toStdString().length();

	int done = 0;
	if (length>0)
	{
		int file_exists = 1;
		ifstream file(fileName.toLocal8Bit());
		if (file)
		{
			//std::cout<<"file:"<<filename.c_str()<<" exists."<<std::endl;
			file.close();
		}
		else
		{

			std::cout << "file:" << fileName.toStdString().c_str() << " does not exists." << std::endl;
			file_exists = 0;
		}

		if (file_exists == 1)
		{


			std::string STVext(".stv");
			std::string STVext2(".STV");


			std::size_t found = fileName.toStdString().find(STVext);
			std::size_t found2 = fileName.toStdString().find(STVext2);
			if (found != std::string::npos || found2 != std::string::npos)
			{
				type = 1;
				//STV
			}

			if (type == 1)
			{
				//filein = fopen(fileName.toLocal8Bit(), "rt");
				QFile inputFile(fileName);
				int ok = 0;
				if (inputFile.open(QIODevice::ReadOnly))
				{
					QTextStream in(&inputFile);
					int lmk_type= 0;
					int number = 0;
					int cpt_line = 0;
					int cpt_norm = 0;
					int cpt_targ = 0;
					int cpt_node = 0;
					int cpt_handle = 0;
					while (!in.atEnd())
					{

						QString line = in.readLine();
						QTextStream myteststream(&line);
						if (cpt_line == 0)
						{
							myteststream >> lmk_type >> number;
						}
						else
						{
							// To do : type = 2 => information 
							int node_type = -1;
							if (lmk_type == NODE_LMK)// curve node!
							{
								myteststream >> LMKName >> x >> y >> z >> nx >> ny >> nz >> node_type;
								//cout << "lmtype!" << lmtype << endl;
								//lmtype: 1 curve starting point
								//lmtype: 0 normal node
								//lmtype: 2 curve milestone
								//lmtype: 3 connect to preceding starting point
							}
							else
							{
								myteststream >> LMKName >> x >> y >> z >> nx >> ny >> nz;
							}
							double coord[3] = { x,y,z };
							double ncoord[3] = { nx,ny,nz };
							double ori[3];

							double length = nx*nx + ny*ny + nz*nz;
							if (length == 1)
							{
								ori[0] = ncoord[0];
								ori[1] = ncoord[1];
								ori[2] = ncoord[2];
							}
							else
							{
								vtkMath::Subtract(ncoord, coord, ori);
								vtkMath::Normalize(ori);
							}
							/*if (lmtype != 0)
							{
							cout << "landmark_mode: " << landmark_mode << " lmtype: " << lmtype << endl;
							}*/
							this->CreateLandmark(coord, ori, lmk_type, node_type);
							if (lmk_type == NORMAL_LMK)
							{
								cpt_norm++;
							}
							else if (lmk_type == TARGET_LMK)
							{
								cpt_targ++;
							}
							else if (lmk_type == NODE_LMK)
							{
								cpt_node++;
							}
							else if (lmk_type == HANDLE_LMK)
							{
								cpt_handle++;
							}
								
						}
						cpt_line++;
						if (cpt_line == number + 1) {
							cpt_line = 0;

						}
					}
					int cpt_tot = cpt_norm + cpt_targ + cpt_node + cpt_handle;
					this->CreateLandmarkUndoSet(-1, cpt_tot, cpt_norm, cpt_targ, cpt_node, cpt_handle);
					/**/

					inputFile.close();


				}
				this->Render();
			}//fin if																		

		}//file exists...
	}	//length



		/*float  param2, param3, param4, param5, param6, param7;
		float m_ve[3], m_ven[3], leng;
		char param1[50];
		FILE	*filein;// Filename To Open
		char	oneline[255];
		int landmark_mode;


		int file_exists = 1;
		ifstream file(filename.c_str());

		if (file)
		{
		file.close();
		}
		else
		{
		cout << "file:" << filename.c_str() << " does not exists." << std::endl;
		file_exists = 0;
		}

		if (file_exists == 1)
		{
		std::string STVext(".stv");
		std::string STVext2(".STV");

		int type = 1; //VER

		filein = fopen(filename.c_str(), "r");
		readstr(filein, oneline);
		feof(filein);
		std::cout << "Try open landmark file " << std::endl;
		std::cout << "feof(filein)" << feof(filein) << std::endl;
		int ind = 0;
		vtkSmartPointer<vtkFloatArray> param_list = vtkSmartPointer<vtkFloatArray>::New();
		param_list->SetNumberOfComponents(1);
		int number = 0;
		int cpt_line = 0;
		while (!feof(filein))
		{
		if (cpt_line == 0) {
		sscanf(oneline, "%d %d\n", &landmark_mode, &number);
		}
		else {
		sscanf(oneline, "%s %f %f %f %f %f %f %d\n", param1, &param2, &param3, &param4, &param5, &param6, &param7, &ind);
		param_list->InsertNextTuple1(param2);
		param_list->InsertNextTuple1(param3);
		param_list->InsertNextTuple1(param4);
		param_list->InsertNextTuple1(param5);
		param_list->InsertNextTuple1(param6);
		param_list->InsertNextTuple1(param7);

		create_landmarks(landmark_mode, param_list, type);

		param_list = vtkSmartPointer<vtkFloatArray>::New();
		param_list->SetNumberOfComponents(1);

		}
		readstr(filein, oneline); //read next line
		cpt_line++;

		if (cpt_line == number + 1 && landmark_mode == 0) {
		cpt_line = 0;
		landmark_mode++;
		}
		}//While scanff...
		fclose(filein);
		}
		*/

}
void mqMorphoDigCore::OpenMAP(QString fileName, int mode)
{
	//mode 0: add map to Existing color maps... (for vtkPolyData objects)
	// mode 1: add map to Last Volume (for Volume objects)
	// mode 2: add map to All selected Volumes (for Volume objects)
	int nr, discretize, discretizenr, enableopacity, nc, no; 
	cout << "Call open map in mode" << mode << endl;
	double  cx, r, g, b, ox, ov;
	QString ColorMapName;
	QString SomeText;
	//Open a STV file!

	//cout << "start: " << this->Getmui_ExistingColorMaps()->Stack.size() << "Color maps inside the stack!" << endl;
	size_t  length;

	int type = 1;
	length = fileName.toStdString().length();

	int done = 0;
	if (length>0)
	{
		int file_exists = 1;
		ifstream file(fileName.toLocal8Bit());
		if (file)
		{
			//std::cout<<"file:"<<filename.c_str()<<" exists."<<std::endl;
			file.close();
		}
		else
		{

			std::cout << "file:" << fileName.toStdString().c_str() << " does not exists." << std::endl;
			file_exists = 0;
		}

		if (file_exists == 1)
		{


			std::string MAPext(".map");
			std::string MAPext2(".MAP");


			std::size_t found = fileName.toStdString().find(MAPext);
			std::size_t found2 = fileName.toStdString().find(MAPext2);
			if (found != std::string::npos || found2 != std::string::npos)
			{
				type = 1;
				//MAP
				cout << "Open Map: Type 1!" << endl;
			}
			if (type == 1)
			{
				//filein = fopen(fileName.toLocal8Bit(), "rt");
				QFile inputFile(fileName);
				int ok = 0;
				cout << "Open Map: Try open file!" << fileName.toStdString()<< endl;
				if (inputFile.open(QIODevice::ReadOnly))
				{
					QTextStream in(&inputFile);
					int cpt_line = 0;
					QString line = in.readLine();
					QTextStream myteststream(&line);

					myteststream >> SomeText >> nr;
					
					if (nr > 0)
					{
						for (int i = 0; i < nr; i++)
						{
							vtkSmartPointer<vtkDiscretizableColorTransferFunction> newSTC = vtkSmartPointer<vtkDiscretizableColorTransferFunction>::New();

							QString cm = QString("cm");
							cm = cm + QString::number(i);
							
							line = in.readLine();
							myteststream.setString(&line); 
							myteststream >> SomeText >> ColorMapName;
							cout << "Found a name: " << ColorMapName.toStdString() << endl;							

							line = in.readLine();
							myteststream.setString(&line);
							myteststream >> SomeText >> discretize;
							cout << "Discretize: " << discretize << endl;
							
							line = in.readLine();
							myteststream.setString(&line);
							myteststream >> SomeText >> discretizenr;
							cout << "Discretizenr: " << discretizenr << endl;
							
							line = in.readLine();
							myteststream.setString(&line);							
							myteststream >> SomeText >> enableopacity;
							cout << "enableopacity: " << enableopacity << endl;
							
							line = in.readLine();
							myteststream.setString(&line);
							
							myteststream >> SomeText >> nc;
							cout << "nc: " << nc << endl;

							line = in.readLine();
							myteststream.setString(&line);
							
							myteststream >> SomeText >> no;
							cout << "no: " << no << endl;
							
							

							newSTC->SetColorSpaceToRGB();						
							newSTC->SetDiscretize(discretize);							
							newSTC->SetNumberOfValues(discretizenr);							
							newSTC->SetEnableOpacityMapping(enableopacity);

							for (int j = 0; j < nc; j++)
							{
								line = in.readLine();
								myteststream.setString(&line);
								
								myteststream >> SomeText >> cx >> r >> g >> b;
								
								cout << "cx" << j << ":" << cx << ",";
								cout << "rgb:" << r << ",";
								cout << g << ",";
								cout << b << endl;
								newSTC->AddRGBPoint(cx, r, g, b);

							}

							vtkSmartPointer<vtkPiecewiseFunction> opacityfunction = vtkSmartPointer<vtkPiecewiseFunction>::New();														
							
							for (int j = 0; j < no; j++)
							{
								line = in.readLine();
								myteststream.setString(&line);
								
								myteststream >> SomeText >> ox >> ov;
								cout << "ox" << j << ":" << ox << ",";
								cout << "ov=" << j << ":" << ov << endl;
								opacityfunction->AddPoint(ox, ov);
							}

							newSTC->SetScalarOpacityFunction(opacityfunction);
							newSTC->Build();


							cout << "Add this map to open map!!" << endl;
							int num = 2;
							if (this->colorMapNameAlreadyExists(ColorMapName) == 1)
							{
								int exists = 1;
								while (exists == 1)
								{
									QString NewColorMapName = ColorMapName + "_" + QString::number(num);
									if (this->colorMapNameAlreadyExists(NewColorMapName) == 0)
									{
										exists = 0;
										ColorMapName = NewColorMapName;
									}
									else
									{
										num++;
									}
								}
							}
							
							//cout << "here: " << this->Getmui_ExistingColorMaps()->Stack.size() << " Color maps inside the stack!" << endl;
							if (mode == 0)
							{
								this->Getmui_ExistingColorMaps()->Stack.push_back(ExistingColorMaps::Element(ColorMapName, newSTC, 1));
							}
							else if (mode == 1 && i == 0)
							{
								//mode 1: sets the MAP to the last inserted volume
								// can only add one STC to a gve
								vtkMDVolume *myVolume = this->GetLastVolume();
								cout << "Tried to get last volume for map i==" << i << endl;
								if (myVolume != NULL)
								{
									cout << "Set Ctf!!!!" << i << endl;
									myVolume->SetCtf(newSTC);
								}
							}
							else if (mode == 2 && i == 0)
							{
								//mode 2: sets the MAP to the all selected volumes.
								// can only add one STC to a gve
								cout << "Tried to import map ==" << i << " to all selected volumes"<<endl;
								this->getVolumeCollection()->InitTraversal();
								for (vtkIdType i = 0; i < this->getVolumeCollection()->GetNumberOfItems(); i++)
								{

									vtkVolume *vol = this->getVolumeCollection()->GetNextVolume();
									if (vol != NULL)
									{
										std::string str1("vtkMDVolume");
										if (str1.compare(vol->GetClassName()) == 0)
										{

											vtkMDVolume *myVolume = vtkMDVolume::SafeDownCast(vol);
											if (myVolume->GetSelected() == 1) { myVolume->SetCtf(newSTC); }
										}
									}

								}

							}
							//cout << "and now: " << this->Getmui_ExistingColorMaps()->Stack.size() << " Color maps inside the stack!" << endl;
							

						}
					
						if (mode == 0)
						{
							cout << "have Just Imported " << nr << " ColorMaps" << endl;
							cout << "now " << this->Getmui_ExistingColorMaps()->Stack.size() << "Color maps inside the stack!" << endl;
							this->signal_colorMapsChanged();// emit this->colorMapsChanged();
						}
					}

				}
				else
				{
				cout << "Failed to open Map!" << endl;
				 }
					/**/

					inputFile.close();


			}
		}																	

	}
}	


int mqMorphoDigCore::SaveNTWFile(QString fileName, int save_ori, int save_tag, int save_surfaces_format, int save_volumes_format, int compression, int apply_position_to_surfaces)
{
	//save_surfaces_format: 0:VTK 1:PLY 2:STL
	// save_surfaces_format 0 : stl
	// save_surfaces_format 1 : vtk
	// save_surfaces_format 2 : ply
	// save_surfaces_format 4  vtp

	// save_volumes_format 0 : mhd
	// save_volumes_format 1 : mha
	// save_volumes_format 2 : vti
	cout << "apply_position_to_surfaces=" << apply_position_to_surfaces << endl;
	std::string NTWext = ".ntw";
	std::string NTWext2 = ".NTW";
	std::size_t found = fileName.toStdString().find(NTWext);
	std::size_t found2 = fileName.toStdString().find(NTWext2);
	if (found == std::string::npos && found2 == std::string::npos)
	{
		fileName.append(".ntw");
	}

	QFileInfo fileInfo(fileName);
	QString onlyfilename(fileInfo.fileName());
	QString onlypath = fileName.left(fileName.length() - onlyfilename.length());
	QString projectname = onlyfilename.left(onlyfilename.lastIndexOf("."));
	cout << "QString onlyfilename: " << onlyfilename.toStdString() << endl;;
	cout << "QString onlypath: " << onlypath.toStdString() << endl;;
	cout << "QString projectname: " << projectname.toStdString() << endl;;
	QFile file(fileName);
	QTextStream stream(&file);
	this->ComputeSelectedNamesLists();
	
	//stream.setCodec("UTF-8");
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		//std::string only_filename = onlyfilename.toStdString();
		//std::string project_name = only_filename.c_str();
		//size_t nPos = project_name.find_last_of(".");
		//if (nPos > 0 && nPos <= project_name.length())
		//{
		//	project_name = project_name.substr(0, nPos);
		//}
		//std::string path = fileName.toStdString().substr(0, (fileName.toStdString().length() - only_filename.length()));
		QString posExt = ".pos";
		QString vtpExt = ".vtp";
		QString vtkExt = ".vtk";
		QString plyExt = ".ply";
		QString stlExt = ".stl";
		QString tagExt = ".tag";
		QString tagExt2 = ".tgp";
		QString oriExt = ".ori";
		QString flgExt = ".flg";
		QString verExt = ".ver";
		QString stvExt = ".stv";
		QString curExt = ".cur";
		QString mhdExt = ".mhd";
		QString mhaExt = ".mha";
		QString rawExt = ".raw";
		QString zrawExt = ".zraw";
		QString vtiExt = ".vti";
		QString mapExt = ".map";

		int overwrite_pos = 1;
		int overwrite_mesh = 1;
		//int overwrite_pos = 1;
		int overwrite_vol = 1;
		int overwrite_map = 1;
		int overwrite_ori = 1;
		int overwrite_tag = 1;
		int overwrite_flg = 1;
		int overwrite_ver = 1;
		int overwrite_cur = 1;
		int overwrite_stv = 1;

		

		QString postfix = "_";
		postfix.append(projectname.toStdString().c_str());
		QString no_postfix = "";
		int pos_exists = 0;
		int pos_special = 0;
		if (g_distinct_selected_names.size() == 1 && projectname.toStdString().compare(g_distinct_selected_names.at(0)) == 0)
		{
			pos_special = 1;
			pos_exists = this->selected_file_exists(onlypath, posExt, no_postfix);
		}
		else
		{
			pos_exists = this->selected_file_exists(onlypath, posExt, postfix);
		}
		int map_exists = 0;


		int map_special = 0;
		if (g_distinct_selected_names.size() == 1 && projectname.toStdString().compare(g_distinct_selected_names.at(0)) == 0)
		{
			map_special = 1;
			map_exists = this->selected_file_exists(onlypath, mapExt, no_postfix);
		}
		else
		{
			map_exists = this->selected_file_exists(onlypath, mapExt, postfix);
		}

		int mesh_exists = 0;
		if (save_surfaces_format == 1)//VTK
		{
			mesh_exists = this->selected_file_exists(onlypath, vtkExt, no_postfix);
		}
		else if (save_surfaces_format == 4)//VTP
		{
			mesh_exists = this->selected_file_exists(onlypath, vtpExt, no_postfix);
		}
		else if (save_surfaces_format == 2)//PLY
		{
			mesh_exists = this->selected_file_exists(onlypath, plyExt, no_postfix);
		}
		else //0 STL
		{
			mesh_exists = this->selected_file_exists(onlypath, stlExt, no_postfix);
		}

		int vol_exists = 0;
		cout << "Save volume format =" << save_volumes_format << endl;
		if (save_volumes_format == 0)//MHD
		{
			vol_exists = this->selected_file_exists(onlypath, mhdExt, no_postfix);
			if (vol_exists ==0 && compression==0)
			{
				vol_exists = this->selected_file_exists(onlypath, rawExt, no_postfix);
			}
			if (vol_exists == 0 && compression ==1)
			{
				vol_exists = this->selected_file_exists(onlypath, zrawExt, no_postfix);
			}
		}
		else if (save_volumes_format == 1)//MHA
		{
			vol_exists = this->selected_file_exists(onlypath, mhaExt, no_postfix);
		}
		else //2 VTI
		{
			//cout << "checks if vti file already exists" << endl;
			vol_exists = this->selected_file_exists(onlypath, vtiExt, no_postfix);
			//cout << "vol_exists:" << vol_exists << endl;
		}

		
	
		
		

		if (save_ori == 1)
		{
			QString _ori_fullpath;
			QString _ori_file;
			int ori_exists = this->context_file_exists(onlypath, oriExt, projectname);
			if (ori_exists == 1)
			{
				QMessageBox msgBox;
				msgBox.setText("Ori file already exists: overwrite existing orientation file ?");
				msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
				msgBox.setDefaultButton(QMessageBox::Yes);
				int ret = msgBox.exec();
				if (ret == QMessageBox::Cancel) { overwrite_ori=0; }

			}
			

			/*_ori_file = projectname.toStdString().c_str();
			_ori_file.append(".ori");
			_ori_fullpath = onlypath.toStdString.c_str();
			_ori_fullpath.append(_ori_file.c_str());*/
			_ori_file = projectname + oriExt;
			_ori_fullpath = onlypath + _ori_file;
			int write = 1;
			//QString qori(_ori_fullpath.c_str());
			
			if (overwrite_ori == 1)
			{
				//write or overwrite ORI file without further question (0)
				this->SaveORI(_ori_fullpath);
			}
			stream << _ori_file.toStdString().c_str() << endl;

		}
		if (mesh_exists == 1)
		{
			QMessageBox msgBox;
			msgBox.setText("At least one surface file already exists: update existing surface files?");
			msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Yes);
			int ret = msgBox.exec();
			if (ret == QMessageBox::Cancel) { overwrite_mesh = 0; }

			
		}
		if (vol_exists == 1)
		{
			QMessageBox msgBox;
			msgBox.setText("At least one volume file already exists: update existing volume files?");
			msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Yes);
			int ret = msgBox.exec();
			if (ret == QMessageBox::Cancel) { overwrite_vol = 0; }


		}
		if (map_exists == 1)
		{
			QMessageBox msgBox;
			msgBox.setText("At least one Color Transfer Function file (.map) already exists: update existing Color Transfer Function ?");
			msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Yes);
			int ret = msgBox.exec();
			if (ret == QMessageBox::Cancel) { overwrite_map = 0; }


		}
		if (pos_exists == 1)
		{
			QMessageBox msgBox;
			msgBox.setText("At least one position file already exists: update existing position files?");
			msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Yes);
			int ret = msgBox.exec();
			if (ret == QMessageBox::Cancel) { overwrite_pos = 0; }

			
		}

		if (save_tag == 1)
		{
			QString _tag_fullpath;
			QString _tag_file;
			
			int tag_exists2 = this->context_file_exists(onlypath, tagExt2, projectname);
			
			if (tag_exists2 ==1)
			{
				QMessageBox msgBox;
				msgBox.setText("Tag file already exists: overwrite existing tag colours and labels file ?");
				msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
				msgBox.setDefaultButton(QMessageBox::Yes);
				int ret = msgBox.exec();
				if (ret == QMessageBox::Cancel) { overwrite_tag = 0; }
				
			}

			_tag_file = projectname;
			
			_tag_file.append(".tgp");
			_tag_fullpath = onlypath;
			_tag_fullpath += _tag_file;
			int write = 1;
			
			if (overwrite_tag == 1)
			{
				//write or overwrite TAG file without further question (0)
				//@@ TODO!
				this->SaveTAGMAPFile(_tag_fullpath, 0);
			}
			stream << _tag_file.toLocal8Bit() << endl;
			

		}
		
		//flags 
		vtkIdType selectedflags = this->getFlagLandmarkCollection()->GetNumberOfSelectedActors();
		if (selectedflags>0)
		{
			QString _flg_fullpath;
			QString _flg_file;
			int flg_exists = this->context_file_exists(onlypath, flgExt, projectname);
			if (flg_exists == 1)
			{
				QMessageBox msgBox;
				msgBox.setText("Flag file already exists: overwrite existing flag file ?");
				msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
				msgBox.setDefaultButton(QMessageBox::Yes);
				int ret = msgBox.exec();
				if (ret == QMessageBox::Cancel) { overwrite_flg = 0; }
				
			}

			_flg_file = projectname;
			_flg_file.append(".flg");
			_flg_fullpath = onlypath;
			_flg_fullpath+=_flg_file;
			if (overwrite_flg == 1)			
			{
				//cout << "Try to save FLG file" << endl;
				//write or overwrite FLG file without further question (0)
				//QString qflg(_flg_fullpath.c_str());
				this->SaveFlagFile(_flg_fullpath, 1);
			}
			stream << _flg_file.toStdString().c_str() << endl;

		}

		int nselnor = this->NormalLandmarkCollection->GetNumberOfSelectedActors();
		int nseltar = this->TargetLandmarkCollection->GetNumberOfSelectedActors();
		int nselnod = this->NodeLandmarkCollection->GetNumberOfSelectedActors();
		int nselhan = this->HandleLandmarkCollection->GetNumberOfSelectedActors();
		if (nselnor > 0 || nseltar > 0|| nselnod > 0|| nselhan > 0)
		{
			QString _stv_fullpath;
			QString _stv_file;
			int stv_exists = this->context_file_exists(onlypath, stvExt, projectname);
			if (stv_exists == 1)
			{
				QMessageBox msgBox;
				msgBox.setText("Landmark/Curve file already exists: overwrite existing STV file ?");
				msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
				msgBox.setDefaultButton(QMessageBox::Yes);
				int ret = msgBox.exec();
				if (ret == QMessageBox::Cancel) { overwrite_stv = 0; }
				
			}

			_stv_file = projectname;
			_stv_file.append(".stv");
			_stv_fullpath = onlypath+_stv_file;
			int write = 1;
			if (overwrite_stv == 1)
			{
				//write or overwrite stv file without further question (0)
				//QString qstv(_stv_fullpath.c_str());
				this->SaveSTVFile(_stv_fullpath,1);
			}
			stream << _stv_file.toStdString().c_str() << endl;
			
		}

		this->ActorCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
		{
			vtkMDActor * myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
			if (myActor->GetSelected() == 1)
			{
				QString _mesh_fullpath;
				QString _pos_fullpath;
				QString _mesh_file;
				QString _pos_file;
				_mesh_file.append(myActor->GetName().c_str());
				_pos_file.append(myActor->GetName().c_str());
				if (save_surfaces_format == 1)
				{
					_mesh_file.append(".vtk");
				}
				else if (save_surfaces_format == 4)
				{
					_mesh_file.append(".vtp");
				}
				else if (save_surfaces_format == 2)
				{
					_mesh_file.append(".ply");
				}
				else
				{
					_mesh_file.append(".stl");
				}
				if (pos_special==0)
				{
					_pos_file+=postfix;
				}
				_pos_file.append(".pos");

				_mesh_fullpath = onlypath;
				_mesh_fullpath+=_mesh_file;

				_pos_fullpath = onlypath;
				_pos_fullpath+=_pos_file;

				int write = 1;
				if (overwrite_mesh == 0)
				{
					// in that case, check if file exists...								
					ifstream file2(_mesh_fullpath.toLocal8Bit());
					if (file2)
					{
						write = 0;
						file2.close();
					}
				}
				if (write == 1)
				{
					

					std::vector<std::string> mscalarsToBeRemoved; // no scalar to be removed here for the moment...
					int RGBopt = 0; //now: keep current RGB color
					this->SaveSurfaceFile(_mesh_fullpath ,0 , apply_position_to_surfaces, save_surfaces_format, mscalarsToBeRemoved, RGBopt, 0, myActor);
				}

				

				write = 1;
				if (overwrite_pos == 0)
				{
					// in that case, check if file exists...								
					ifstream file2(_pos_fullpath.toLocal8Bit());
					if (file2)
					{
						write = 0;
						file2.close();
					}
				}

				if (write == 1)
				{
					if (apply_position_to_surfaces == 0)
					{
						vtkSmartPointer<vtkMatrix4x4> Mat = myActor->GetMatrix();
						this->SavePOS(Mat, _pos_fullpath);
					}
					else
					{
						vtkSmartPointer<vtkMatrix4x4> Mat = vtkSmartPointer<vtkMatrix4x4>::New();
						Mat->Identity();
						this->SavePOS(Mat,_pos_fullpath);

					}
				}

				stream << _mesh_file.toStdString().c_str() << endl;
				stream << _pos_file.toStdString().c_str() << endl;
				
				double colors[4];
				myActor->GetmColor(colors);
				stream << colors[0]<<" "<< colors[1]<<" "<< colors[2]<<" "<< colors[3] << endl;

					
			}

				

		}
		this->VolumeCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->VolumeCollection->GetNumberOfItems(); i++)
		{
			vtkMDVolume * myVolume = vtkMDVolume::SafeDownCast(this->VolumeCollection->GetNextVolume());
			if (myVolume->GetSelected() == 1)
			{
				QString _vol_fullpath;
				QString _pos_fullpath;
				QString _map_fullpath;
				QString _vol_file;
				QString _pos_file;
				QString _map_file;
				_vol_file.append(myVolume->GetName().c_str());
				_pos_file.append(myVolume->GetName().c_str());
				_map_file.append(myVolume->GetName().c_str());

				if (save_volumes_format == 0)
				{
					_vol_file.append(".mhd");
				}
				else if (save_volumes_format == 1)
				{
					_vol_file.append(".mha");
				}
				else
				{
					_vol_file.append(".vti");
				}

				if (pos_special == 0)
				{
					_pos_file += postfix;
				}
				if (map_special == 0)
				{
					_map_file += postfix;
				}
				_pos_file.append(".pos");
				_map_file.append(".map");


				_vol_fullpath = onlypath;
				_vol_fullpath += _vol_file;

				_pos_fullpath = onlypath;
				_pos_fullpath += _pos_file;

				_map_fullpath = onlypath;
				_map_fullpath += _map_file;

				int write = 1;
				if (overwrite_vol == 0)
				{
					// in that case, check if file exists...								
					ifstream file2(_vol_fullpath.toLocal8Bit());
					if (file2)
					{
						write = 0;
						file2.close();
					}
				}
				if (write == 1)
				{
														
					this->SaveVolume(_vol_fullpath, save_volumes_format, compression, myVolume);
				}

				write = 1;
				if (overwrite_pos == 0)
				{
					// in that case, check if file exists...								
					ifstream file2(_pos_fullpath.toLocal8Bit());
					if (file2)
					{
						write = 0;
						file2.close();
					}
				}

				if (write == 1)
				{
					
						vtkSmartPointer<vtkMatrix4x4> Mat = myVolume->GetMatrix();
						this->SavePOS(Mat, _pos_fullpath);					
				}
				write = 1;
				if (overwrite_map == 0)
				{
					// in that case, check if file exists...								
					ifstream file2(_map_fullpath.toLocal8Bit());
					if (file2)
					{
						write = 0;
						file2.close();
					}
				}

				if (write == 1)
				{
					
					vtkSmartPointer<vtkDiscretizableColorTransferFunction> ctf = myVolume->GetCtf();
					cout << "SaveNTW: call SaveMAP in volume mode" << endl;
					//this->SaveMAP(_map_fullpath, myVolume->GetName().c_str(), ctf, 1);
					this->SaveMAPFile(_map_fullpath, 1, _map_file, myVolume);
					
				}
				stream << _vol_file.toStdString().c_str() << endl;
				stream << _pos_file.toStdString().c_str() << endl;
				stream << _map_file.toStdString().c_str() << endl;
				stream << myVolume->GetScalarOpacityUnitDistance() << " " << myVolume->GetScalarDisplayMin() << " "<< myVolume->GetScalarDisplayMax() << endl;
				


			}



		}

				
	}
	file.close();
	return 1;
}

int mqMorphoDigCore::Getmui_Specular() {
	return this->mui_Specular;
}
double mqMorphoDigCore::Getmui_SpecularPower(){
	return this->mui_SpecularPower;
}
int mqMorphoDigCore::Getmui_DefaultSpecular(){
	return this->mui_DefaultSpecular;
}
double mqMorphoDigCore::Getmui_DefaultSpecularPower(){
	return this->mui_DefaultSpecularPower;
}
void mqMorphoDigCore::Setmui_Specular(int specular){
	this->mui_Specular = specular;
	this->UpdateColorProperties();
}
void mqMorphoDigCore::Setmui_SpecularPower(double specularPower){
	this->mui_SpecularPower = specularPower;
	this->UpdateColorProperties();
}

int mqMorphoDigCore::Getmui_Diffuse(){
	return this->mui_Diffuse;
}
int mqMorphoDigCore::Getmui_DefaultDiffuse(){
	return this->mui_DefaultDiffuse;
}
void mqMorphoDigCore::Setmui_Diffuse(int diffuse){

	this->mui_Diffuse = diffuse;
	this->UpdateColorProperties();
}

int mqMorphoDigCore::Getmui_DisplayLandmarkText()
{
	// Display LM Actors text (LM number or flag text)
	return this->mui_DisplayLandmarkText;
}
int mqMorphoDigCore::Getmui_DefaultDisplayLandmarkText() { 
	// Display LM Actors text (LM number or flag text)
	return this->mui_DefaultDisplayLandmarkText;
}
void mqMorphoDigCore::Setmui_DisplayLandmarkText(int display)
{
	// Display LM Actors text (LM number or flag text)
	this->mui_DisplayLandmarkText = display;
	cout << "displayLM=" << display<<endl;
	this->UpdateLandmarkSettings();
}

int mqMorphoDigCore::Getmui_FontSize() {
	return this->mui_FontSize;
}
int mqMorphoDigCore::Getmui_DefaultFontSize() {
	return this->mui_DefaultFontSize;
}
void mqMorphoDigCore::Setmui_FontSize(int fontSize) {

	this->mui_FontSize = fontSize;
	this->UpdateLandmarkSettings();
}

int mqMorphoDigCore::Getmui_Ambient(){
	return this->mui_Ambient;

}
void mqMorphoDigCore::Setmui_Ambient(int ambient){
	this->mui_Ambient = ambient;
	this->UpdateColorProperties();
}
int mqMorphoDigCore::Getmui_DefaultAmbient(){

	return this->mui_DefaultAmbient;
}
void mqMorphoDigCore::UpdateColorProperties()
{
	if (this->ActorCollection && this->ActorCollection->GetNumberOfItems()>0)
	{
		this->ActorCollection->UpdateColorProperties(this->mui_Ambient, this->mui_Diffuse, this->mui_Specular, this->mui_SpecularPower);
		this->Render();
	}
	if (this->VolumeCollection && this->VolumeCollection->GetNumberOfItems() > 0)
	{
		this->VolumeCollection->UpdateColorProperties(this->mui_Ambient, this->mui_Diffuse, this->mui_Specular, this->mui_SpecularPower);
		this->Render();
	}
}

void mqMorphoDigCore::OpenCAM(QString fileName)
{
	double  cNear, cFar, cx, cy, cz, fx, fy, fz, ux, uy, uz, parallel;
	QString someText;
	//Open a camera file!


	size_t  length;


	length = fileName.toStdString().length();

	int done = 0;
	if (length>0)
	{
		int file_exists = 1;
		ifstream file(fileName.toLocal8Bit());
		if (file)
		{
			file.close();
		}
		else
		{

			std::cout << "file:" << fileName.toStdString().c_str() << " does not exists." << std::endl;
			file_exists = 0;
		}

		if (file_exists == 1)
		{
			
			
				//filein = fopen(fileName.toLocal8Bit(), "rt");
				QFile inputFile(fileName);
				int ok = 0;
				if (inputFile.open(QIODevice::ReadOnly))
				{
					QTextStream in(&inputFile);					
					QString line = in.readLine();
					QTextStream myteststream(&line);				
					// 1st line
					myteststream >> someText >> cNear >> cFar ;
					line = in.readLine();
					myteststream.setString(&line);
					myteststream >> someText >> cx>>cy>>cz;
					line = in.readLine();
					myteststream.setString(&line);
					myteststream >> someText >> fx >> fy >> fz;

					line = in.readLine();
					myteststream.setString(&line);
					myteststream >> someText >> ux >> uy >> uz;
					
					line = in.readLine();
					myteststream.setString(&line);
					myteststream >> someText >> parallel;
					
					inputFile.close();
					this->SetCAM(cNear, cFar, cx, cy, cz, fx, fy, fz, ux, uy, uz, parallel);

				}																	

		}//file exists...
	}	//length


}


double  mqMorphoDigCore::GetCAMParameters(double cp[2], double position[3], double focal[3], double up[3])
{
	this->getRenderer()->GetActiveCamera()->GetClippingRange(cp);
	this->getRenderer()->GetActiveCamera()->GetPosition(position);
	this->getRenderer()->GetActiveCamera()->GetFocalPoint(focal);
	this->getRenderer()->GetActiveCamera()->GetViewUp(up);
	return this->getRenderer()->GetActiveCamera()->GetParallelScale();
}
void mqMorphoDigCore::SetCAM(double cNear, double cFar, double cX, double cY, double cZ, double fX, double fY, double fZ, double uX, double uY, double uZ, double parallel)
{
	if (this->Getmui_CameraOrtho() == 1)
	{

		this->getCamera()->SetParallelScale(parallel);
		this->DollyCameraForPerspectiveMode();
		this->getRenderer()->ResetCameraClippingRange();
		this->SetGridInfos();

	}
	this->SetCAM(cNear, cFar, cX, cY, cZ, fX, fY, fZ, uX, uY, uZ);

}
void mqMorphoDigCore::SetCAM(double cNear, double cFar, double cX, double cY, double cZ, double fX, double fY, double fZ, double uX, double uY, double uZ)
{

	this->getRenderer()->GetActiveCamera()->SetClippingRange(cNear, cFar);
	this->getRenderer()->GetActiveCamera()->SetPosition(cX, cY, cZ);
	this->getRenderer()->GetActiveCamera()->SetFocalPoint(fX, fY, fZ);
	this->getRenderer()->GetActiveCamera()->SetViewUp(uX, uY, uZ);



	this->Render();

}

void mqMorphoDigCore::SaveCAM(QString fileName, double cNear, double cFar, double cX, double cY, double cZ, double fX, double fY, double fZ, double uX, double uY, double uZ)
{
	// this value is not correct when in perspective mode et qu'on a fait des zoom en orthographic.
	// quand on fait des zoom en parallel, tout va bien.
	double parallel = this->getRenderer()->GetActiveCamera()->GetParallelScale();
	std::string CAMext = ".cam";
	std::string CAMext2 = ".CAM";
	std::size_t found = fileName.toStdString().find(CAMext);
	std::size_t found2 = fileName.toStdString().find(CAMext2);
	if (found == std::string::npos && found2 == std::string::npos)
	{
		fileName.append(".cam");
	}

	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);

		stream << "Clipping: " <<  cNear << " " << cFar<<endl;
		stream << "Center: " << cX << " " << cY << " " << cZ << endl;
		stream << "Focal: " << fX << " " << fY << " " << fZ << endl;
		stream << "Up: " << uX << " " << uY << " " << uZ << endl;
		stream << "Parallel: " << parallel << endl;
		/*
		if (mqMorphoDigCore::instance()->Getmui_CameraOrtho() == 1)
	{
		double currentScale = mqMorphoDigCore::instance()->getCamera()->GetParallelScale();
		double currentHundredPxSU = mqMorphoDigCore::instance()->GetHundredPxSU();
		double newHundredPxSu = this->Ui->hundredpxsu->value();
		if (currentHundredPxSU>0 && newHundredPxSu>0)
		{
			double newScale = currentScale*newHundredPxSu / currentHundredPxSU;
			mqMorphoDigCore::instance()->getCamera()->SetParallelScale(newScale);
			// now change camera position
			mqMorphoDigCore::instance()->DollyCameraForPerspectiveMode();
			mqMorphoDigCore::instance()->getRenderer()->ResetCameraClippingRange();
			//change grid infos
			mqMorphoDigCore::instance()->SetGridInfos();
			mqMorphoDigCore::instance()->Render();
		}
		
		
		
	}
		*/

	}
	file.close();



}
void mqMorphoDigCore::SavePOS(vtkSmartPointer<vtkMatrix4x4> Mat, QString fileName)
{

	std::string POSext = ".pos";
	std::string POSext2 = ".POS";
	std::size_t found = fileName.toStdString().find(POSext);
	std::size_t found2 = fileName.toStdString().find(POSext2);
	if (found == std::string::npos && found2 == std::string::npos)
	{
		fileName.append(".pos");
	}

	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);

		stream << "1 0 0 0" << endl;
		stream << "0 1 0 0" << endl;
		stream << "0 0 1 0" << endl;
		stream << "0 0 0 1" << endl;


		stream << Mat->GetElement(0, 0) << " " << Mat->GetElement(1, 0) << " " << Mat->GetElement(2, 0) << " " << Mat->GetElement(3, 0) << endl;
		stream << Mat->GetElement(0, 1) << " " << Mat->GetElement(1, 1) << " " << Mat->GetElement(2, 1) << " " << Mat->GetElement(3, 1) << endl;
		stream << Mat->GetElement(0, 2) << " " << Mat->GetElement(1, 2) << " " << Mat->GetElement(2, 2) << " " << Mat->GetElement(3, 2) << endl;
		stream << Mat->GetElement(0, 3) << " " << Mat->GetElement(1, 3) << " " << Mat->GetElement(2, 3) << " " << Mat->GetElement(3, 3) << endl;


	}
	file.close();



}
//should only be done after main window is initialized.
int mqMorphoDigCore::SaveFlagFile(QString fileName, int save_only_selected)
{

	
		std::string FLGext = ".flg";
		std::string FLGext2 = ".FLG";
		std::size_t found = fileName.toStdString().find(FLGext);
		std::size_t found2 = fileName.toStdString().find(FLGext2);
		if (found == std::string::npos && found2 == std::string::npos)
		{
			fileName.append(".flg");
		}
	

	

	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);

		vtkSmartPointer<vtkLMActorCollection> myColl = vtkSmartPointer<vtkLMActorCollection>::New();
		 myColl = this->FlagLandmarkCollection; 
		

		myColl->InitTraversal();
		for (vtkIdType i = 0; i < myColl->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(myColl->GetNextActor());
			if (myActor->GetSelected() == 1 || save_only_selected == 0)
			{

				double lmpos[3];
				myActor->GetLMOrigin(lmpos);
				double ori[3];
				myActor->GetLMOrientation(ori);
				double lmori[3] = { lmpos[0] + ori[0],lmpos[1] + ori[1] ,lmpos[2] + ori[2] };
				
				stream << myActor->GetLMLabelText()<< endl;
				stream << lmpos[0] << " " << lmpos[1] << " " << lmpos[2] << " " 
					<< lmori[0] << " " << lmori[1] << " " << lmori[2] << " " << 
					myActor->GetLMSize()<<" "<<
					myActor->GetmColor()[0] << " "<<
					myActor->GetmColor()[1] << " " <<
					myActor->GetmColor()[2] << " " <<
					endl;
				
					
				

			}

		}




	}
	file.close();
	return 1;

}
void mqMorphoDigCore::OpenTAGMAP(QString fileName) {
	// tgp files: when map name already exists : update existing map.
	// tag files : always replace "this->TagMap" by .tag file content.

	int ntagmap;
	
	cout << "start: " << this->Getmui_ExistingTagMaps()->Stack.size() << " Tag maps inside the stack!" << endl;
	size_t  length;

	int type = 1;
	length = fileName.toStdString().length();

	int done = 0;
	if (length>0)
	{
		int file_exists = 1;
		ifstream file(fileName.toLocal8Bit());
		if (file)
		{
			
			file.close();
		}
		else
		{

			std::cout << "file:" << fileName.toStdString().c_str() << " does not exists." << std::endl;
			file_exists = 0;
		}

		if (file_exists == 1)
		{

			std::string TAGext = ".tgp";
			std::string TAGext2 = ".TGP";
			std::string TAGext3 = ".tag";
			std::string TAGext4 = ".TAG";
			std::size_t found = fileName.toStdString().find(TAGext);
			std::size_t found2 = fileName.toStdString().find(TAGext2);
			std::size_t found3 = fileName.toStdString().find(TAGext3);
			std::size_t found4 = fileName.toStdString().find(TAGext4);
			int mode = 0; //tgp

			if (found == std::string::npos && found2 == std::string::npos && found3 == std::string::npos && found4 == std::string::npos)
			{
				mode = 0;
			}
			else if (found3 != std::string::npos || found4 != std::string::npos)
			{
				mode = 1; //TAG! Old meshtools style
			}
								
			if (mode == 0) // tgp extension
			{
				//filein = fopen(fileName.toLocal8Bit(), "rt");
				QFile inputFile(fileName);
				int ok = 0;
				if (inputFile.open(QIODevice::ReadOnly))
				{
					

					QTextStream in(&inputFile);
					int cpt_line = 0;
					QString line = in.readLine();
					QTextStream myteststream(&line);
					QString SomeText;

					myteststream >> SomeText >> ntagmap;

					if (ntagmap > 0)
					{
						for (int i = 0; i < ntagmap; i++)
						{
							int tagnr;
							vtkSmartPointer<vtkLookupTable> NewMap = vtkSmartPointer<vtkLookupTable>::New();
							std::vector<std::string> tagNames;
							double  r, g, b, a;
							QString TagMapName;
							QString Tag;
						
						
							line = in.readLine();
							myteststream.setString(&line);
							myteststream >> SomeText >> TagMapName;
							cout << "Found a name: " << TagMapName.toStdString() << endl;

							line = in.readLine();
							myteststream.setString(&line);
							myteststream >> SomeText >> tagnr;
							cout << "Tagnr: " << tagnr << endl;
							NewMap->SetNumberOfTableValues(tagnr);
							NewMap->Build();

							for (int j = 0; j < tagnr; j++)
							{
								line = in.readLine();
								myteststream.setString(&line);
								myteststream >> SomeText >> Tag;
								//tagNames.push_back(line.toStdString().c_str());
								QString AllTag = line.remove(0, SomeText.length()+1);
								tagNames.push_back(AllTag.toStdString().c_str());

								line = in.readLine();
								myteststream.setString(&line);
								myteststream >> SomeText >> r >> g >> b >> a;

								
								NewMap->SetTableValue(j, r, g, b, a);

							}

							


							cout << "Add this color map to !!" << endl;
							int num = 2;
							if (this->tagMapNameAlreadyExists(TagMapName) == 1)
							{
								int id = this->getTagMapId(TagMapName);

								/*int exists = 1;
								while (exists == 1)
								{
									QString NewColorMapName = TagMapName + "_" + QString::number(num);
									if (this->tagMapNameAlreadyExists(NewColorMapName) == 0)
									{
										exists = 0;
										TagMapName = NewColorMapName;
									}
									else
									{
										num++;
									}
								}*/
								QString mTagMap =  QString("TagMap");
								if (TagMapName == mTagMap)
								{
									this->TagLut = NewMap;
								}
								this->Getmui_ExistingTagMaps()->Stack.at(id).numTags = tagnr;
								this->Getmui_ExistingTagMaps()->Stack.at(id).TagMap= NewMap;
								this->Getmui_ExistingTagMaps()->Stack.at(id).tagNames = tagNames;
								if (id == this->getActiveTagMapId())
								{
									this->Setmui_ActiveTagMap(TagMapName, tagnr, tagNames, NewMap);
								}
								
							}
							else
							{


								this->Getmui_ExistingTagMaps()->Stack.push_back(ExistingTagMaps::Element(TagMapName, NewMap, tagnr, tagNames, 1));
								cout << "and now: " << this->Getmui_ExistingTagMaps()->Stack.size() << " Color maps inside the stack!" << endl;
							}

						}

						cout << "have Just Imported " << ntagmap << " Tag Maps" << endl;
						cout << "now " << this->Getmui_ExistingColorMaps()->Stack.size() << "Tag  maps inside the stack!" << endl;
						this->signal_tagMapsChanged();// emit this->colorMapsChanged();
					}

				}
				/**/

				inputFile.close();


			}
			else
			{
				// mode TAG : todo!!!!
				//filein = fopen(fileName.toLocal8Bit(), "rt");
				int tagnr = 0;
				QFile inputFileTMP(fileName);
				int ok = 0;
				if (inputFileTMP.open(QIODevice::ReadOnly))
				{
					QTextStream in1(&inputFileTMP);
					int cpt = 0;
					while (!in1.atEnd())
					{

						QString line1 = in1.readLine();
						if (line1.length() > 1)
						{
							cpt++;
						}

					}
					tagnr = (int)(cpt / 2);
					cout << "found " << tagnr << " tags inside .TAG file" << endl;

				}
				inputFileTMP.close();

				QFile inputFile(fileName);
				
				if (inputFile.open(QIODevice::ReadOnly))
				{				

					QTextStream in(&inputFile);
					QString SomeText;
					QString TagMapName = QString("TagMap");
						
					vtkSmartPointer<vtkLookupTable> NewMap = vtkSmartPointer<vtkLookupTable>::New();
					std::vector<std::string> tagNames;
					double  r, g, b, a;
					QString Tag;

					NewMap->SetNumberOfTableValues(tagnr);
					NewMap->Build();
					for (int j = 0; j < tagnr; j++)
					{
						QString line = in.readLine();
						QTextStream myteststream(&line);					
						myteststream >> Tag;
						tagNames.push_back(line.toStdString().c_str());

						line = in.readLine();
						myteststream.setString(&line);
						myteststream >> r >> g >> b >> a;


						NewMap->SetTableValue(j, r, g, b, a);

					}




					cout << "Add this color map to TAGMAP default !!" << endl;
					int num = 2;
					
					int id = 0; // default tag map									
					this->TagLut = NewMap;					
					this->Getmui_ExistingTagMaps()->Stack.at(id).numTags = tagnr;
					this->Getmui_ExistingTagMaps()->Stack.at(id).TagMap = NewMap;
					this->Getmui_ExistingTagMaps()->Stack.at(id).tagNames = tagNames;
					if (id == this->getActiveTagMapId())
					{
						this->Setmui_ActiveTagMap(TagMapName, tagnr, tagNames, NewMap);
					}										
					this->signal_tagMapsChanged();// emit this->colorMapsChanged();
					

				}
				/**/

				inputFile.close();




			}
		}

	}
	this->Render();

}
//void mqMorphoDigCore::OpenTAG(QString fileName) {}

int mqMorphoDigCore::SaveTAGMAPFile(QString fileName, int save_only_active)
{
	//save_only_active 
	// 1: save only active color map => then check if extension exists => if not add tgp
	// 0: save ALL colormaps (custom + "hard coded native" ones)
	// 2 : save ALL custom colormaps

	int mode = 0; //tgp

	std::string TAGext = ".tgp";
	std::string TAGext2 = ".TGP";
	std::string TAGext3 = ".tag";
	std::string TAGext4 = ".TAG";
	std::size_t found = fileName.toStdString().find(TAGext);
	std::size_t found2 = fileName.toStdString().find(TAGext2);
	std::size_t found3 = fileName.toStdString().find(TAGext3);
	std::size_t found4 = fileName.toStdString().find(TAGext4);
	if (found == std::string::npos && found2 == std::string::npos && found3 == std::string::npos && found4 == std::string::npos)
	{
		fileName.append(".tgp");
	}
	else if (found3 != std::string::npos || found4 != std::string::npos)
	{
		mode = 1; //TAG! Old meshtools style
	}

	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);
		if (save_only_active == 1)
		{
			if (mode == 0)
			{
				stream << "nr: 1" << endl;
			}
			file.close();
			int i = this->getActiveTagMapId();
			this->SaveTAGMAP(fileName, i, mode);
		}
		else if (save_only_active == 0)//save all
		{
			ExistingTagMaps *tagMaps = this->Getmui_ExistingTagMaps();
			size_t size = tagMaps->Stack.size();
			if (mode == 0)
			{
				stream << "nr: " << size << endl;
			}
			file.close();
			for (int i = 0; i < size; i++)
			{
				if (mode == 0 || i == 0) // just in case the user decided to save all with a .TAG extension, then just save the first one
				{
					this->SaveTAGMAP(fileName, i, mode);
				}
			}
		}
		else // save all customs
		{
			ExistingTagMaps *tagMaps = this->Getmui_ExistingTagMaps();
			size_t size = tagMaps->Stack.size();
			int sizec = 0;
			for (int i = 0; i < size; i++)
			{
				if (tagMaps->Stack.at(i).isCustom == 1)
				{
					sizec++;
				}
			}

			if (mode == 0)
			{
				stream << "nr: " << sizec << endl;
			}
			file.close();
			int ic = 0;
			for (int i = 0; i < size; i++)
			{
				if (tagMaps->Stack.at(i).isCustom == 1)
				{

					if (mode == 0 || ic == 0) // just in case the user decided to save all custom tag maps with a .TAG extension, then just save the first custom one
					{
						this->SaveTAGMAP(fileName, i, mode);
					}
					ic++;
				}
			}
		}
	}
	this->Render();
	return 1;

}

void mqMorphoDigCore::SaveTAGMAP(QString fileName, int mapId, int mode)
{
	//mode 0: tgp
	//mode 1: tag
	ExistingTagMaps *tagMaps = this->Getmui_ExistingTagMaps();
	QString Name = tagMaps->Stack.at(mapId).Name;
	vtkSmartPointer<vtkLookupTable> TagMap = tagMaps->Stack.at(mapId).TagMap;
	std::vector < std::string> tagNames = tagMaps->Stack.at(mapId).tagNames;
	int tagnr = tagMaps->Stack.at(mapId).numTags;

	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		QTextStream stream(&file);
		if (mode == 1)
		{
			for (int i = 0; i < tagnr; i++)
			{
				stream << QString::fromStdString(tagNames.at(i)) << endl;
				double *rgba = TagMap->GetTableValue(i);
				stream << rgba[0] << " " << rgba[1] << " " << rgba[2] << " " << rgba[3] << " " << endl;
			}

		}
		else
		{
			stream <<"name: "<< Name << endl;
			stream << "nt: " << tagnr << endl;
			for (int i = 0; i < tagnr; i++)
			{
				stream << "t" << QString::number(i) << ": "<< QString::fromStdString(tagNames.at(i)) << endl;
				double *rgba = TagMap->GetTableValue(i);
				stream << "c" << QString::number(i) <<": " << rgba[0] << " " << rgba[1] << " " << rgba[2] << " " << rgba[3] << " " << endl;
			}
			
		}		

	}
	file.close();

}


int mqMorphoDigCore::SaveMAPFile(QString fileName, int save_only_active, QString Name, vtkMDVolume *volume)
{
	//save_only_active 
	// 1: save only active color map
	// 0: save ALL colormaps (custom + "hard coded native" ones)
	// 2 : save ALL custom colormaps


	// Name !="" and volume!=NULL only when saving the single CTF of a volume

	std::string MAPext = ".map";
	std::string MAPext2 = ".MAP";
	std::size_t found = fileName.toStdString().find(MAPext);
	std::size_t found2 = fileName.toStdString().find(MAPext2);
	if (found == std::string::npos && found2 == std::string::npos)
	{
		fileName.append(".map");
	}

	
	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
			QTextStream stream(&file);
		if (save_only_active ==1 ||volume !=NULL)
		{
			stream << "nr: 1" << endl;
			file.close();
			if (volume == NULL)
			{
				this->SaveMAP(fileName, this->Getmui_ActiveColorMap()->Name, this->Getmui_ActiveColorMap()->ColorMap);
			}
			else
			{
				this->SaveMAP(fileName, Name, volume->GetCtf());
			}
		}
		else if (save_only_active == 0)//save all
		{
			ExistingColorMaps *colorMaps = this->Getmui_ExistingColorMaps();
			size_t size = colorMaps->Stack.size();

			stream << "nr: " << size<< endl;
			file.close();
			for (int i = 0; i < size; i++)
			{
				this->SaveMAP(fileName, colorMaps->Stack.at(i).Name, colorMaps->Stack.at(i).ColorMap);
			}			
		}
		else // save all customs
		{
			ExistingColorMaps *colorMaps = this->Getmui_ExistingColorMaps();
			size_t size = colorMaps->Stack.size();
			int sizec = 0;
			for (int i = 0; i < size; i++)
			{
				if (colorMaps->Stack.at(i).isCustom == 1)
				{
					sizec++;
				}				
			}

			stream << "nr: " << sizec << endl;
			file.close();
			for (int i = 0; i < size; i++)
			{
				if (colorMaps->Stack.at(i).isCustom == 1)
				{
					this->SaveMAP(fileName, colorMaps->Stack.at(i).Name, colorMaps->Stack.at(i).ColorMap);
				}
			}			
		}						
	}
	
	return 1;

}

void mqMorphoDigCore::SaveOneMAP(vtkSmartPointer<vtkDiscretizableColorTransferFunction> ColorMap)
{
	QInputDialog *giveNameDialog = new QInputDialog();
	bool dialogResult;

	QString Name = giveNameDialog->getText(0, "Color map name", "Name:", QLineEdit::Normal,
		"Custom_color_map", &dialogResult);
	if (dialogResult && Name.length()>0)
	{

		QString fileName = QFileDialog::getSaveFileName(mqMorphoDigCore::instance()->GetMainWindow(),
			tr("Save MAP files"), mqMorphoDigCore::instance()->Getmui_LastUsedDir() + QDir::separator() + Name+".map",
			tr("Map file (*.map)"), NULL
			//, QFileDialog::DontConfirmOverwrite
		);


		cout << fileName.toStdString();
		if (fileName.isEmpty()) return;
		QFile file(fileName);
		if (file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QTextStream stream(&file);
			stream << "nr: 1" << endl;
			stream << "name: " << Name << endl;

			stream << "discretize: " << ColorMap->GetDiscretize() << endl;
			stream << "discretizenr: " << ColorMap->GetNumberOfValues() << endl;
			stream << "enableopacity: " << ColorMap->GetEnableOpacityMapping() << endl;

			//3 set numer of colors

			int nc = ColorMap->GetSize();
			stream << "nc: " << nc << endl;

			//4 set number of opacity nodes
			int no = ColorMap->GetScalarOpacityFunction()->GetSize();
			stream << "no: " << no << endl;

			double *pts = ColorMap->GetDataPointer();
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
				mult = 1 / range;
				c = -cx_min / range;
			}

			for (int j = 0; j < nc; j++)
			{
				double cx = mult * pts[4 * j] + c;
				double r = pts[4 * j + 1];
				double g = pts[4 * j + 2];
				double b = pts[4 * j + 3];
				stream << "c" << j << ": " << cx << " " << r << " " << g << " " << b << endl;
			}
			double *pts2 = ColorMap->GetScalarOpacityFunction()->GetDataPointer();

			double ox_min = DBL_MAX;
			double ox_max = -DBL_MAX;
			for (int j = 0; j < no; j++)
			{
				double curr = pts2[2 * j];
				if (curr < ox_min) { ox_min = curr; }
				if (curr > ox_max) { ox_max = curr; }
			}
			c = 0;
			mult = 1;
			if (ox_max > ox_min)
			{
				double range = ox_max - ox_min;
				mult = 1 / range;
				c = -ox_min / range;
			}

			for (int j = 0; j < no; j++)
			{
				double ox = mult * pts2[2 * j] + c;
				double ov = pts2[2 * j + 1];
				stream << "op" << j << ": " << ox << " " << ov << endl;


			}

		}
		file.close();
	}
}
void mqMorphoDigCore::SaveMAP(QString fileName, QString Name, vtkSmartPointer<vtkDiscretizableColorTransferFunction> ColorMap)
{
	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		QTextStream stream(&file);
		
		stream << "name: " << Name << endl;
				
		stream << "discretize: " << ColorMap->GetDiscretize() << endl;
		stream << "discretizenr: " << ColorMap->GetNumberOfValues() << endl;
		stream << "enableopacity: " << ColorMap->GetEnableOpacityMapping() << endl;
		
		//3 set numer of colors
		
		int nc = ColorMap->GetSize();
		stream << "nc: " << nc<< endl;

		//4 set number of opacity nodes
		int no = ColorMap->GetScalarOpacityFunction()->GetSize();
		stream << "no: " << no << endl;
				
		double *pts = ColorMap->GetDataPointer();
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
			mult = 1 / range;
			c = -cx_min / range;
		}

		for (int j = 0; j < nc; j++)
		{
			double cx = mult*pts[4 * j] + c;
			double r = pts[4 * j + 1];
			double g = pts[4 * j + 2];
			double b = pts[4 * j + 3];
			stream << "c"<< j<< ": "<< cx << " "<< r <<" "<<g<<" "<< b << endl;
		}
		double *pts2 = ColorMap->GetScalarOpacityFunction()->GetDataPointer();

		double ox_min = DBL_MAX;
		double ox_max = -DBL_MAX;
		for (int j = 0; j < no; j++)
		{
			double curr = pts2[2 * j];
			if (curr < ox_min) { ox_min = curr; }
			if (curr > ox_max) { ox_max = curr; }
		}
		c = 0;
		mult = 1;
		if (ox_max > ox_min)
		{
			double range = ox_max - ox_min;
			mult = 1 / range;
			c = -ox_min / range;
		}

		for (int j = 0; j < no; j++)
		{
			double ox = mult*pts2[2 * j] + c;
			double ov = pts2[2 * j + 1];
			stream << "op" << j << ": " <<  ox << " " << ov << endl;
			

		}

	}
	file.close();

}

int mqMorphoDigCore::SaveSTVFile(QString fileName, int save_only_selected)
{


	std::string STVext = ".stv";
	std::string STVext2 = ".STV";
	std::size_t found = fileName.toStdString().find(STVext);
	std::size_t found2 = fileName.toStdString().find(STVext2);
	if (found == std::string::npos && found2 == std::string::npos)
	{
		fileName.append(".stv");
	}




	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);

		vtkSmartPointer<vtkLMActorCollection> myColl = vtkSmartPointer<vtkLMActorCollection>::New();
		
		for (int j = 0; j < 4; j++)
		{
			if (j == 0)
			{
				myColl = this->NormalLandmarkCollection;
			}
			else if (j == 1)
			{
				myColl = this->TargetLandmarkCollection;
			}
			else if (j == 2)
			{
				myColl = this->NodeLandmarkCollection;
			}
			else if (j == 3)
			{
				myColl = this->HandleLandmarkCollection;
			}


			

			if (myColl->GetNumberOfItems() > 0)
			{
				if (save_only_selected == 0)
				{
					stream <<j<<" "<< myColl->GetNumberOfItems() << endl;
				}
				else
				{
					stream << j << " " << myColl->GetNumberOfSelectedActors() << endl;
				}
				myColl->InitTraversal();

				for (vtkIdType i = 0; i < myColl->GetNumberOfItems(); i++)
				{
					vtkLMActor *myActor = vtkLMActor::SafeDownCast(myColl->GetNextActor());
					
					

					if (myActor->GetSelected() == 1 || save_only_selected == 0) 
					{

						double lmpos[3];
						myActor->GetLMOrigin(lmpos);
						
						
						myActor->GetLMOrigin(lmpos);
						double ori[3];
						myActor->GetLMOrientation(ori);
						double lmori[3] = { lmpos[0] + ori[0],lmpos[1] + ori[1] ,lmpos[2] + ori[2] };
						
						if (j < 2)
						{
							stream << "Landmark" << i+1 << ": " << lmpos[0] << " " << lmpos[1] << " " << lmpos[2] << " " << lmori[0] << " " << lmori[1] << " " << lmori[2] << " " << endl;
						}
						else if (j==2)
						{
							int lmnodetype = myActor->GetLMNodeType();
							stream << "CurveNode" << i+1 << ": " << lmpos[0] << " " << lmpos[1] << " " << lmpos[2] << " " << lmori[0] << " " << lmori[1] << " " << lmori[2] << " " << lmnodetype <<endl;
						}
						else
						{
							stream << "CurveHandle" << i+1 << ": " << lmpos[0] << " " << lmpos[1] << " " << lmpos[2] << " " << lmori[0] << " " << lmori[1] << " " << lmori[2] << " " << endl;
						}

						
						

					}

				}
			}
		}




	}
	file.close();
	return 1;

}


//should only be done after main window is initialized.
int mqMorphoDigCore::SaveCURasVERFile(QString fileName, int decimation, int save_format, int save_other_lmks)
{
	std::string VERext = ".ver";
	std::string VERext2 = ".VER";
	std::string LMKext = ".lmk";
	std::string LMKext2 = ".LMK";
	std::string TPSext = ".tps";
	std::string TPSext2 = ".TPS";
	std::string PTSext = ".pts";
	std::string PTSext2 = ".PTS";

	if (save_format == 0)
	{
		std::size_t found = fileName.toStdString().find(VERext);
		std::size_t found2 = fileName.toStdString().find(VERext2);
		if (found == std::string::npos && found2 == std::string::npos)
		{
			fileName.append(".ver");
		}
	}
	else if(save_format == 1)
	{
		std::size_t found = fileName.toStdString().find(LMKext);
		std::size_t found2 = fileName.toStdString().find(LMKext2);
		if (found == std::string::npos && found2 == std::string::npos)
		{
			fileName.append(".lmk");
		}
	}
	else if (save_format == 2)
	{
		std::size_t found = fileName.toStdString().find(PTSext);
		std::size_t found2 = fileName.toStdString().find(PTSext2);
		if (found == std::string::npos && found2 == std::string::npos)
		{
			fileName.append(".pts");
		}
	}
	else 
	{
		std::size_t found = fileName.toStdString().find(TPSext);
		std::size_t found2 = fileName.toStdString().find(TPSext2);
		if (found == std::string::npos && found2 == std::string::npos)
		{
			fileName.append(".tps");
		}
	}
	//Combien de landmarks au total (besoin pour les formats TPS et PTS) en comptant les curves!
	// combien de segments:
	int tot_num_seg = 0;		
	vtkLMActor *Node;
	Node = this->NodeLandmarkCollection->GetLandmarkAfter(0);
	while(Node != NULL)
	{
		if (Node->GetLMNodeType() == STARTING_NODE|| Node->GetLMNodeType() == MILESTONE_NODE) { tot_num_seg++; }
		int ind = Node->GetLMNumber();
		Node = this->NodeLandmarkCollection->GetLandmarkAfter(ind);			
	}
	int tot_lm_number = tot_num_seg * decimation;
	if (save_other_lmks == 1)
	{
		tot_lm_number += this->getNormalLandmarkCollection()->GetNumberOfItems() + this->getTargetLandmarkCollection()->GetNumberOfItems();
	}
	cout << "tot_lm_number=" << tot_lm_number << endl;
	
	vtkIdType ci = 0;
	std::string csi = "00";
	std::string lsi = "00";
	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);
		if (save_format == 2)
		{
			stream << "Version 1.0" << endl;
			stream << tot_lm_number << endl;
		}
		else if (save_format == 3)
		{
			stream << "LM=" << tot_lm_number<< endl;
		}

		if (save_other_lmks == 1)
		{
			vtkLMActor  * normal, *target;
			int num_normal = this->getNormalLandmarkCollection()->GetNumberOfItems();
			
			if (num_normal > 0)
			{
				normal = this->getNormalLandmarkCollection()->GetLandmarkAfter(0);
				

				while (normal != NULL)
				{
					int ind = normal->GetLMNumber();				
					double lmpos[3];
					normal->GetLMOrigin(lmpos);
					double ori[3];
					normal->GetLMOrientation(ori);
					double lmori[3] = { lmpos[0] + ori[0],lmpos[1] + ori[1] ,lmpos[2] + ori[2] };
										
					if (save_format == 0)
					{
						stream << "Landmark" << ci << ": " << lmpos[0] << " " << lmpos[1] << " " << lmpos[2] << " " << lmori[0] << " " << lmori[1] << " " << lmori[2] << " " << endl;
					}
					else if (save_format ==1)
					{
						stream << "Landmark" << ci << ": " << lmpos[0] << " " << lmpos[1] << " " << lmpos[2] << endl;
					}
					else if (save_format == 2)
					{
						stream << "S"<< csi.c_str() << ci << " " << lmpos[0] << " " << lmpos[1] << " " << lmpos[2] << endl;
					}
					else if (save_format == 3)
					{
						stream << lmpos[0] << " " << lmpos[1] << " " << lmpos[2] << endl;
					}
					ci++;
					if (ci < 10) { csi = "00"; }
					else if (ci < 100) { csi = "0"; }
					else { csi = ""; }
					normal = this->getNormalLandmarkCollection()->GetLandmarkAfter(ind);
				}				

			}

			int num_target = this->getTargetLandmarkCollection()->GetNumberOfItems();
			if (num_target > 0)
			{
				target = this->getTargetLandmarkCollection()->GetLandmarkAfter(0);
				while (target != NULL)
				{
					int ind = target->GetLMNumber();
					double lmpos[3];
					target->GetLMOrigin(lmpos);
					double ori[3];
					target->GetLMOrientation(ori);
					double lmori[3] = { lmpos[0] + ori[0],lmpos[1] + ori[1] ,lmpos[2] + ori[2] };

					if (save_format == 0)
					{
						stream << "Landmark" << ci << ": " << lmpos[0] << " " << lmpos[1] << " " << lmpos[2] << " " << lmori[0] << " " << lmori[1] << " " << lmori[2] << " " << endl;
					}
					else if (save_format == 1)
					{
						stream << "Landmark" << ci << ": " << lmpos[0] << " " << lmpos[1] << " " << lmpos[2] << endl;
					}
					else if (save_format == 2)
					{
						stream << "S" << csi.c_str() << ci << " " << lmpos[0] << " " << lmpos[1] << " " << lmpos[2] << endl;
					}
					else if (save_format == 3)
					{
						stream << lmpos[0] << " " << lmpos[1] << " " << lmpos[2] << endl;
					}
					ci++;
					if (ci < 10) { csi = "00"; }
					else if (ci < 100) { csi = "0"; }
					else { csi = ""; }
					target = this->getTargetLandmarkCollection()->GetLandmarkAfter(ind);
				}
			}
		}

		vtkSmartPointer<vtkLMActorCollection> Nodes = vtkSmartPointer<vtkLMActorCollection>::New();
		vtkSmartPointer<vtkLMActorCollection> Handles = vtkSmartPointer<vtkLMActorCollection>::New();
		Nodes = this->NodeLandmarkCollection;
		Handles = this->HandleLandmarkCollection;

	//START
		double length = 0;


		//cout << "Export curves!" << endl;
		// First a rigid line between node landmarks.
		if (Nodes == NULL)
		{
			return 0;
		}



		vtkLMActor  * ob_H1, *ob_H2, *ob_N1, *ob_N2, *ob_Nstart, *ob_Hstart, *ob_HA, *ob_HB, *ob_NA, *ob_NB;
		ob_H1 = NULL;
		ob_N1 = NULL;
		ob_H2 = NULL;
		ob_N2 = NULL;
		ob_Hstart = NULL;
		ob_Nstart = NULL;
		ob_NA = NULL;
		ob_NB = NULL;
		ob_HA = NULL;
		ob_HB = NULL;


		int k, m, num_seg, ind, ind2, indh, indh2;
		int nint = 1002; // will iterate nint , to draw nint+1 lines, that is nint+2 points
		double t, t2;


		int num_landmark_H = Handles->GetNumberOfItems();
		int num_landmark_N = Nodes->GetNumberOfItems();
		int nbp = num_landmark_H;
		double  nn1[3], nn2[3], hh1[3], hh2[3], intvv[3], intvv2[3], slm[3];
		
		int new_segment = 0;
		t2 = 0;
		double preceding_length = 0;
		double current_length = 0;
		int decimation_index = 0;
		double length_to_reach = 0;
		double total_length = this->getBezierCurveSource()->GetCurveSegmentLength(1);


		int print = 0;
		num_seg = 0;
		//if (num_landmark_T == num_landmark_N && num_landmark_T >0)
		csi = "000";
		lsi = "000";
		if (decimation > 1 && decimation < nint)
		{

			int cpt = 0;
			//cout << "here" << endl;
			ob_H1 = Handles->GetLandmarkAfter(0);
			ob_Hstart = ob_H1;
			ob_H2 = Handles->GetLandmarkAfter(ob_H1->GetLMNumber());
			ob_N1 = Nodes->GetLandmarkAfter(0);
			ob_Nstart = ob_N1;
			ob_N2 = Nodes->GetLandmarkAfter(ob_N1->GetLMNumber());
			k = 0;

			while (ob_N1 != NULL && ob_H1 != NULL)
			{

				//stop drawing if the second point is a "start" of a new curve
				if (ob_N1->GetLMNodeType() == STARTING_NODE) {
					num_seg++;
				//	cout << "num_seg:" << num_seg << endl;
					t2 = 0;
					length_to_reach = 0;
					current_length = 0;
					preceding_length = 0;
					decimation_index = 0;
					total_length = this->getBezierCurveSource()->GetCurveSegmentLength(num_seg);
				}
				if (ob_N1->GetLMNodeType() == MILESTONE_NODE) {
					num_seg++;
				//	cout << "num_seg:" << num_seg << endl;
					t2 = 0;
					length_to_reach = 0;
					current_length = 0;
					preceding_length = 0;
					decimation_index = 0;
					total_length = this->getBezierCurveSource()->GetCurveSegmentLength(num_seg);
				}


				if (ob_N1->GetLMNodeType() == CONNECT_NODE) // in that case we should connect that node to the preceding starting point
				{
					ob_NA = ob_N1;
					ob_NB = ob_Nstart;
					ob_HA = ob_H1;
					ob_HB = ob_Hstart;

				}
				else
				{
					ob_NA = ob_N1;
					ob_HA = ob_H1;
					ob_NB = ob_N2;
					ob_HB = ob_H2;
				}

				if (ob_NB != NULL && ob_HB != NULL)
				{
					vtkMatrix4x4 *MatNA = ob_NA->GetMatrix();
					nn1[0] = MatNA->GetElement(0, 3);
					nn1[1] = MatNA->GetElement(1, 3);
					nn1[2] = MatNA->GetElement(2, 3);

					vtkMatrix4x4 *MatNB = ob_NB->GetMatrix();
					nn2[0] = MatNB->GetElement(0, 3);
					nn2[1] = MatNB->GetElement(1, 3);
					nn2[2] = MatNB->GetElement(2, 3);

					vtkMatrix4x4 *MatHA = ob_HA->GetMatrix();
					hh1[0] = MatHA->GetElement(0, 3);
					hh1[1] = MatHA->GetElement(1, 3);
					hh1[2] = MatHA->GetElement(2, 3);

					vtkMatrix4x4 *MatHB = ob_HB->GetMatrix();
					hh2[0] = MatHB->GetElement(0, 3);
					hh2[1] = MatHB->GetElement(1, 3);
					hh2[2] = MatHB->GetElement(2, 3);

					//trick : second handle is mirrored relative to the second point!
					hh2[0] = nn2[0] - (hh2[0] - nn2[0]);
					hh2[1] = nn2[1] - (hh2[1] - nn2[1]);
					hh2[2] = nn2[2] - (hh2[2] - nn2[2]);

					// At this stage : we have all the input we need!
					// Just draw the Bezier curve between nn1 and nn2

					int compute = 1;

					if (ob_NA->GetLMNodeType() == NORMAL_NODE && ob_NB->GetLMNodeType() == STARTING_NODE) { compute = 0; }
					
					if (compute == 1)
					{

						for (m = 0; m <= nint; m++)
						{
							// t is [0.. 1]
							t = (((double)m)) / (((double)nint + 1));
							// glBegin(GL_LINES);
							intvv[0] = (1 - t)*(1 - t)*(1 - t)*nn1[0] + 3 * (1 - t)*(1 - t)*t*hh1[0] + 3 * (1 - t)*t*t*hh2[0] + t*t*t*nn2[0];
							intvv[1] = (1 - t)*(1 - t)*(1 - t)*nn1[1] + 3 * (1 - t)*(1 - t)*t*hh1[1] + 3 * (1 - t)*t*t*hh2[1] + t*t*t*nn2[1];
							intvv[2] = (1 - t)*(1 - t)*(1 - t)*nn1[2] + 3 * (1 - t)*(1 - t)*t*hh1[2] + 3 * (1 - t)*t*t*hh2[2] + t*t*t*nn2[2];
							// glVertex3d(intvv[0], intvv[1], intvv[2]);


							t = (((double)m + 1)) / (((double)nint + 1));
							intvv2[0] = (1 - t)*(1 - t)*(1 - t)*nn1[0] + 3 * (1 - t)*(1 - t)*t*hh1[0] + 3 * (1 - t)*t*t*hh2[0] + t*t*t*nn2[0];
							intvv2[1] = (1 - t)*(1 - t)*(1 - t)*nn1[1] + 3 * (1 - t)*(1 - t)*t*hh1[1] + 3 * (1 - t)*t*t*hh2[1] + t*t*t*nn2[1];
							intvv2[2] = (1 - t)*(1 - t)*(1 - t)*nn1[2] + 3 * (1 - t)*(1 - t)*t*hh1[2] + 3 * (1 - t)*t*t*hh2[2] + t*t*t*nn2[2];

							
							double len = sqrt(vtkMath::Distance2BetweenPoints(intvv, intvv2));
							// cout << "connect:" << ls[0] << "," << ls[1] << endl;
							current_length += len;
							//glVertex3d(intvv2[0], intvv2[1], intvv2[2]);
							// glEnd();

							if ((current_length >= length_to_reach) && (length_to_reach >= preceding_length) && (decimation_index<decimation))
							{

								slm[0] = 0; slm[1] = 0; slm[2] = 0;
								decimation_index++;

								t = 0.5;
								if ((current_length - preceding_length)>0)
								{
									t = (length_to_reach - preceding_length) / (current_length - preceding_length);
								}
								slm[0] = (1 - t)*intvv[0] + t*intvv2[0];
								slm[1] = (1 - t)*intvv[1] + t*intvv2[1];
								slm[2] = (1 - t)*intvv[2] + t*intvv2[2];
								int wnum_seg = num_seg - 1;
								if (wnum_seg < 10) { csi = "00"; }
								else if (wnum_seg < 100) { csi = "0"; }
								else { csi = ""; }

								if (decimation_index == 0) { lsi = "000"; }
								else if (decimation_index < 10) { lsi = "00"; }
								else if (decimation_index < 100) { lsi = "0"; }
								else { lsi = ""; }
								if (save_format ==0)
								{
									stream << "Curve_segment:" <<num_seg<<"-"<<decimation_index<<" " << slm[0] << " " << slm[1] << " " << slm[2] << " 0 0 1"<<	endl;
									
								}
								else if (save_format ==1)
								{
									stream << "Curve_segment:" << num_seg << "-" << decimation_index << " " << slm[0] << " " << slm[1] << " " << slm[2]  << endl;
								}
								else if (save_format == 2)
								{
									stream << "C" << csi.c_str()<<wnum_seg<< "-" << lsi.c_str()<< decimation_index << " " << slm[0] << " " << slm[1] << " " << slm[2] << endl;
								}
								else if (save_format == 3)
								{
									stream << slm[0] << " " << slm[1] << " " << slm[2] << endl;
								}
						
								
								
								

								t2 = (double)(((double)decimation_index) / (((double)decimation - 1.0)));
								length_to_reach = total_length*t2;

							}

						}

					}

				}




				ind = ob_N1->GetLMNumber();
				indh = ob_H1->GetLMNumber();

				ob_H1 = Handles->GetLandmarkAfter(indh);
				ob_N1 = Nodes->GetLandmarkAfter(ind);
				if (ob_N1 != NULL)
				{
					ind2 = ob_N1->GetLMNumber();
					ob_N2 = Nodes->GetLandmarkAfter(ind2);
				}
				if (ob_H1 != NULL)
				{
					indh2 = ob_H1->GetLMNumber();
					ob_H2 = Handles->GetLandmarkAfter(indh2);
				}
				if (ob_N1 != NULL&& ob_N1->GetLMNodeType() == STARTING_NODE) {
					ob_Hstart = ob_H1;
					ob_Nstart = ob_N1;

				}

				k++;
			}
		}
		
//END



	}
	file.close();
	return 1;
}

int mqMorphoDigCore::SaveCURFile(QString fileName, int save_only_selected)
{


	std::string CURext = ".cur";
	std::string CURext2 = ".CUR";
	std::size_t found = fileName.toStdString().find(CURext);
	std::size_t found2 = fileName.toStdString().find(CURext2);
	if (found == std::string::npos && found2 == std::string::npos)
	{
		fileName.append(".cur");
	}




	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);

		vtkSmartPointer<vtkLMActorCollection> myCollN = vtkSmartPointer<vtkLMActorCollection>::New();
		vtkSmartPointer<vtkLMActorCollection> myCollH = vtkSmartPointer<vtkLMActorCollection>::New();
		myCollN = this->NodeLandmarkCollection;
		myCollH = this->HandleLandmarkCollection;

		if (myCollN->GetNumberOfItems() != myCollH->GetNumberOfItems()) { return 0; }
		myCollN->InitTraversal();
		myCollH->InitTraversal();
		for (vtkIdType i = 0; i < myCollH->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActorN = vtkLMActor::SafeDownCast(myCollN->GetNextActor());
			vtkLMActor *myActorH = vtkLMActor::SafeDownCast(myCollH->GetNextActor());
			if (myActorN->GetSelected() == 1 || save_only_selected == 0) // we do not care if H is selected or not. We follow "N".
			{

				double lmposN[3];
				myActorN->GetLMOrigin(lmposN);
				double lmposH[3];
				myActorH->GetLMOrigin(lmposH);
				
				int lmnodetype = myActorN->GetLMNodeType();
				stream << "CurvePart"<<i+1<<": "<< lmposN[0] << " " << lmposN[1] << " " << lmposN[2] << " "
					<< lmposH[0] << " " << lmposH[1] << " " << lmposH[2] << " " << lmnodetype<<
					endl;

			}

		}




	}
	file.close();
	return 1;

}
void mqMorphoDigCore::ExportAvizoLandmarks(QString fileName)
{

	std::string AVext = ".landmarkAscii";
	std::string AVext2 = ".lm";
	std::string AVext3 = ".LM";
	std::size_t found = fileName.toStdString().find(AVext);
	std::size_t found2 = fileName.toStdString().find(AVext2);
	std::size_t found3 = fileName.toStdString().find(AVext3);
	if (found == std::string::npos && found2 == std::string::npos && found3 == std::string::npos)
	{
		fileName.append(".landmarkAscii");
	}
	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);

		vtkSmartPointer<vtkLMActorCollection> myColl = vtkSmartPointer<vtkLMActorCollection>::New();
		 myColl = this->NormalLandmarkCollection; 
		 int num_lmk = this->NormalLandmarkCollection->GetNumberOfItems();
		 if (num_lmk > 0)
		 {
			 int num_sets = 1;
			 //if (num_lmk == this->TargetLandmarkCollection->GetNumberOfItems())
			 if (this->TargetLandmarkCollection->GetNumberOfItems()>0)
			 {
				 num_sets = 2;
			 }
				 stream << "# Avizo 3D ASCII 3.0" << endl;
			 stream << "" << endl;
			 stream << "define Markers " << num_lmk << endl;
			 stream << "" << endl;
			 stream << "Parameters {" << endl;
			 stream << "    NumSets "<<num_sets<<"," << endl;
			 stream << "    ContentType \"LandmarkSet\""<< endl;
			 stream << "}" << endl;
			 stream << "" << endl;
			 stream << "Markers { float[3] Coordinates } @1" << endl; 
			 if (num_sets == 2)
			 {
				 stream << "Markers { float[3] Coordinates2 } @2" << endl;
			 }
			 stream << "" << endl;
			 stream << "# Data section follows" << endl;
			 stream << "@1" << endl;			 
			 myColl->InitTraversal();			 
			 for (vtkIdType i = 0; i < myColl->GetNumberOfItems(); i++)
			 {
				 vtkLMActor *myActor = vtkLMActor::SafeDownCast(myColl->GetNextActor());
				 double lmpos[3];
				 myActor->GetLMOrigin(lmpos);				 				 	
				 stream << lmpos[0] << " " << lmpos[1] << " " << lmpos[2] << endl;
			 }
			 stream << "" << endl;
			 if (num_sets == 2)
			 {
				 myColl = this->TargetLandmarkCollection;
				 stream << "@2" << endl;
				 myColl->InitTraversal();
				 for (vtkIdType i = 0; i < myColl->GetNumberOfItems(); i++)
				 {
					 vtkLMActor *myActor = vtkLMActor::SafeDownCast(myColl->GetNextActor());
					 double lmpos[3];
					 myActor->GetLMOrigin(lmpos);
					 stream << lmpos[0] << " " << lmpos[1] << " " << lmpos[2] << endl;
				 }
			 }
		 }
		/*myColl = this->TargetLandmarkCollection; */

	}
	file.close();

}

int mqMorphoDigCore::SaveLandmarkFile(QString fileName, int lm_type, int file_type, int save_only_selected)
{

	if (file_type == 0)
	{
		std::string VERext = ".ver";
		std::string VERext2 = ".VER";
		std::size_t found = fileName.toStdString().find(VERext);
		std::size_t found2 = fileName.toStdString().find(VERext2);
		if (found == std::string::npos && found2 == std::string::npos)
		{
			fileName.append(".ver");
		}
	}

	if (file_type == 1)
	{
		std::string LMKext = ".lmk";
		std::string LMKext2 = ".LMK";
		std::size_t found = fileName.toStdString().find(LMKext);
		std::size_t found2 = fileName.toStdString().find(LMKext2);
		if (found == std::string::npos && found2 == std::string::npos)
		{
			fileName.append(".lmk");
		}
	}
	if (file_type == 2)
	{
		std::string PTSext = ".pts";
		std::string PTSext2 = ".PTS";
		std::size_t found = fileName.toStdString().find(PTSext);
		std::size_t found2 = fileName.toStdString().find(PTSext2);
		if (found == std::string::npos && found2 == std::string::npos)
		{
			fileName.append(".pts");
		}
	}
	if (file_type == 3)
	{
		std::string TPSext = ".tps";
		std::string TPSext2 = ".TPS";
		std::size_t found = fileName.toStdString().find(TPSext);
		std::size_t found2 = fileName.toStdString().find(TPSext2);
		if (found == std::string::npos && found2 == std::string::npos)
		{
			fileName.append(".tps");
		}
	}
	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);
		
		vtkSmartPointer<vtkLMActorCollection> myColl = vtkSmartPointer<vtkLMActorCollection>::New();
		if (lm_type == 0) { myColl = this->NormalLandmarkCollection; }
		else if (lm_type == 1) { myColl = this->TargetLandmarkCollection; }
		else if (lm_type == 2) { myColl = this->NodeLandmarkCollection; }
		else  { myColl = this->HandleLandmarkCollection; }
		int num_lmk = myColl->GetNumberOfItems();
		if (file_type == 2)
		{
			stream << "Version 1.0" << endl;
			stream << num_lmk << endl;
		}
		if (file_type == 3)
		{
			stream << "LM=" << num_lmk << endl;
		}
		myColl->InitTraversal();
		std::string csi;
		for (vtkIdType i = 0; i < myColl->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(myColl->GetNextActor());
			if (myActor->GetSelected() == 1 || save_only_selected==0)
			{
				
				double lmpos[3];
				myActor->GetLMOrigin(lmpos);
				double ori[3];
				myActor->GetLMOrientation(ori);
				double lmori[3] = { lmpos[0] + ori[0],lmpos[1] + ori[1] ,lmpos[2] + ori[2] };
				if (i < 10) { csi = "00"; }
				else if (i < 100) { csi = "0"; }
				else { csi = ""; }

				if (file_type == 0)
				{
					stream << "Landmark" << i+1 << ": " << lmpos[0]<<" "<<lmpos[1]<<" "<<lmpos[2]<<" "<<lmori[0]<< " "<< lmori[1] << " " <<  lmori[2] << " " << endl;
				}
				else if (file_type ==1)
				{
					stream << "Landmark" << i+1 << ": " << lmpos[0] << " " << lmpos[1] << " " << lmpos[2] << endl;
				}
				else if (file_type == 2)
				{
					stream << "S" << csi.c_str() << i  << " " << lmpos[0] << " " << lmpos[1] << " " << lmpos[2] << endl;
				}
				else if (file_type == 3)
				{
					stream << lmpos[0] << " " << lmpos[1] << " " << lmpos[2] << endl;
				}
			}

		}
		
		


	}
	file.close();
	return 1;

}
void mqMorphoDigCore::SaveSelectedSurfaceScalars(vtkMDActor *myActor, QString fileName)
{
	ExistingArrays *MyList = this->Getmui_ArraysOfSelectedObjects(1); // get scalar list of first selected ofject

	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);
		stream << "Vertex_id	";
		for (size_t i = 0; i < MyList->Stack.size(); i++)
		{
		
			if ((MyList->Stack.at(i).DataType == VTK_FLOAT || MyList->Stack.at(i).DataType == VTK_DOUBLE) && MyList->Stack.at(i).NumComp == 1)
			{

				stream << MyList->Stack.at(i).Name << "	";
			}
			

		}
		stream << endl;

		vtkPolyDataMapper *mapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
		vtkPolyData *mPD = mapper->GetInput();
		for (vtkIdType j = 0; j < mPD->GetNumberOfPoints(); j++)
		{
			stream <<j<< "	";

			for (size_t i = 0; i < MyList->Stack.size(); i++)
			{
				if ((MyList->Stack.at(i).DataType == VTK_FLOAT || MyList->Stack.at(i).DataType == VTK_DOUBLE) && MyList->Stack.at(i).NumComp == 1)
				{
					vtkDataArray *currentScalars;
					currentScalars = (vtkDoubleArray*)mPD->GetPointData()->GetScalars(MyList->Stack.at(i).Name.toStdString().c_str());
					if (currentScalars == NULL)
					{
						currentScalars = (vtkFloatArray*)mPD->GetPointData()->GetScalars(MyList->Stack.at(i).Name.toStdString().c_str());

					}

					if (currentScalars != NULL)
					{
						stream << currentScalars->GetTuple1(j)<<"	";
					}
					else
					{
						stream << "NAN	";
					}


					
				}


			}
			stream << endl;


		}
		//stream << "Name	Mean Median	Variance Min Max Q5 Q10 Q90 Q95" << endl;


		//this->ComputeSelectedNamesLists();


		
	}
	file.close();

}
//SaveSurfaceTagSummary
void mqMorphoDigCore::SaveSurfaceTagSummary(QString fileName, int useTags, QString TagArray, int numVer, vtkIdType TagId)
{
	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		QTextStream stream(&file);

		std::string TagName = "";
		if (TagId < mqMorphoDigCore::instance()->Getmui_ActiveTagMap()->numTags)
		{
			TagName = mqMorphoDigCore::instance()->Getmui_ActiveTagMap()->tagNames.at(TagId);
		}
		else
		{
			TagName = "Tag" + TagId;
		}



		//this->ComputeSelectedNamesLists();

		this->ActorCollection->InitTraversal();



		for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
		{
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
			if (myActor->GetSelected() == 1)
			{
				
				vtkPolyDataMapper *mapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
				vtkPolyData *mPD = mapper->GetInput();
				double Area = 0;
				double Volume = 0;
				vtkIdType NumVert = 0;
				vtkIdType NumCells = 0;
				vtkIdType cpt = 0;
				if (useTags == 0)
				{
					vtkSmartPointer<vtkMassProperties> massProp = vtkSmartPointer<vtkMassProperties>::New();
					cpt = mapper->GetInput()->GetNumberOfPoints();
					massProp->SetInputData(mapper->GetInput());
					massProp->Update();
					Area = massProp->GetSurfaceArea();
					Volume = massProp->GetVolume();
					NumVert = mapper->GetInput()->GetNumberOfPoints();
					NumCells = mapper->GetInput()->GetNumberOfCells();
					stream << myActor->GetName().c_str() << "	" << Area << "	" << Volume << "	" << NumVert << "	" << NumCells << "	" << "whole_surface" << endl;
				}
				else
				{// if useTags=1: construct cell list for TagId
					//most complex stuff: compute by tag.
					vtkIntArray *currentTags = NULL; //
					currentTags = vtkIntArray::SafeDownCast(mPD->GetPointData()->GetScalars(TagArray.toStdString().c_str()));
				
					vtkSmartPointer<vtkIdList> pointIdList =
						vtkSmartPointer<vtkIdList>::New();
					vtkSmartPointer<vtkIdList> cellIdList =
						vtkSmartPointer<vtkIdList>::New();
					for (vtkIdType j = 0; j < mPD->GetNumberOfPoints(); j++)
					{
						if (currentTags != NULL && currentTags->GetTuple1(j) == TagId)
						{
							NumVert++;
						}
					}
					for (vtkIdType j = 0; j < mPD->GetNumberOfCells(); j++)
					{ 
						mPD->GetCellPoints(j, pointIdList);
						int ok = 1;
						if (pointIdList->GetNumberOfIds() == 3)//only triangles are investigated here
						{

							/*if (currentTags != NULL && currentTags->GetTuple1(pointIdList->GetId(0)) == TagId								
								&& currentTags->GetTuple1(pointIdList->GetId(1)) == TagId
								&& currentTags->GetTuple1(pointIdList->GetId(2)) == TagId
								)
							{
								// the 3 points of this cell are tagged the same way!
								cellIdList->InsertNextId(j);
							}*/
							if (currentTags != NULL)
							{
								int cnumVer = 0;
								if (currentTags->GetTuple1(pointIdList->GetId(0)) == TagId) { cnumVer++; }
								if (currentTags->GetTuple1(pointIdList->GetId(1)) == TagId) { cnumVer++; }
								if (currentTags->GetTuple1(pointIdList->GetId(2)) == TagId) { cnumVer++; }
									
								// cnumVer points of this cell are tagged with TagId! Is it enough to consider this cell as belonging to the cell region ? 
								if (cnumVer >= numVer)
								{
									cellIdList->InsertNextId(j);
								}
							}
						}


					}
					NumCells = cellIdList->GetNumberOfIds();
					for (vtkIdType j = 0; j < cellIdList->GetNumberOfIds(); j++)
					{
						mPD->GetCellPoints(cellIdList->GetId(j), pointIdList);
						double A[3], B[3], C[3];
						double *ve = mPD->GetPoint(pointIdList->GetId(0));
						A[0] = ve[0];
						A[1] = ve[1];
						A[2] = ve[2];
						ve = mPD->GetPoint(pointIdList->GetId(1));
						B[0] = ve[0];
						B[1] = ve[1];
						B[2] = ve[2];
						ve = mPD->GetPoint(pointIdList->GetId(2));
						C[0] = ve[0];
						C[1] = ve[1];
						C[2] = ve[2];
						double x1 = B[0] - A[0];
						double x2 = B[1] - A[1];
						double x3 = B[2] - A[2];
						double y1 = C[0] - A[0];
						double y2 = C[1] - A[1];
						double y3 = C[2] - A[2];
						double surf = 0.5*sqrt((x2*y3 - x3*y2)*(x2*y3 - x3*y2)
										+ (x3*y1 - x1*y3)*(x3*y1 - x1*y3) + 
										  (x1*y2 - x2*y1)*(x1*y2 - x2*y1));
						Area += surf;

						double *ven;
						double vn[3];
						ven = myActor->GetCellNormals()->GetTuple(cellIdList->GetId(j));
						vn[0] = ven[0];
						vn[1] = ven[1];
						vn[2] = ven[2];
						double center[3];
						center[0] = (A[0] + B[0] + C[0]) / 3;
						center[1] = (A[1] + B[1] + C[1]) / 3;
						center[2] = (A[2] + B[2] + C[2]) / 3;
						double vol = surf * (center[0] * vn[0] + center[1] * vn[1]+ center[2] * vn[2]) / 3;
						Volume += vol;
					}
					
					if (NumVert > 10)
					{
						stream << myActor->GetName().c_str() << "	" << Area << "	" << Volume << "	" << NumVert << "	" << NumCells << "	" << TagName.c_str() << endl;

					}
				}



			}
		}
	}
	file.close();


}
void mqMorphoDigCore::SaveSurfaceTagSummary(QString fileName, int useTags, int numVer, QString TagArray)
{
	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);
		stream << "Name	Area	Volume	Vertex_nr	Triangle_nr	Structure" << endl;
		cout << "First Summary Done" << endl;
		file.close();
		this->SaveSurfaceTagSummary(fileName, 0, TagArray, numVer, 0);
		//if useTags =1, search Max tagged value in all opened objects
		if (useTags == 1 && TagArray.length() > 0)
		{
			cout << "useTags==1 and TagArray.length()>0" << endl;
			
			vtkIdType maxTagId = this->GetTagRangeMax(TagArray);
			this->ActorCollection->InitTraversal();
			
			cout << "maxTagId=" << maxTagId << endl;
			if (maxTagId > 0)
			{
				for (vtkIdType i = 0; i <= maxTagId; i++)
				{
					cout << "SaveActiveScalarSummary(" << i << ")" << endl;
					this->SaveSurfaceTagSummary(fileName, 1, TagArray, numVer, i);
				}
			}

		}//usetags=1
	}//file open


}
void mqMorphoDigCore::SaveActiveScalarSummary(QString fileName, int useTags, QString TagArray, vtkIdType TagId)
{
	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		QTextStream stream(&file);

		std::string TagName = "";
		if (TagId < mqMorphoDigCore::instance()->Getmui_ActiveTagMap()->numTags)
		{
			TagName = mqMorphoDigCore::instance()->Getmui_ActiveTagMap()->tagNames.at(TagId);
		}
		else
		{
			TagName = "Tag" + TagId;
		}
		
		

		//this->ComputeSelectedNamesLists();

		this->ActorCollection->InitTraversal();



		for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
		{
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
			if (myActor->GetSelected() == 1)
			{

				vtkPolyDataMapper *mapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
				vtkPolyData *mPD = mapper->GetInput();


				vtkDataArray *currentScalars = NULL;
				if ((this->Getmui_ActiveArray()->DataType == VTK_FLOAT || this->Getmui_ActiveArray()->DataType == VTK_DOUBLE) && this->Getmui_ActiveArray()->NumComp == 1)
				{

					currentScalars = (vtkDoubleArray*)mPD->GetPointData()->GetScalars(this->Getmui_ActiveArray()->Name.toStdString().c_str());
					if (currentScalars == NULL)
					{
						currentScalars = (vtkFloatArray*)mPD->GetPointData()->GetScalars(this->Getmui_ActiveArray()->Name.toStdString().c_str());
					}

				}

				vtkIntArray *currentTags = NULL; //
				
				//4 if current tags exist, retrieve what tool is active
				if (useTags == 1)
				{
					currentTags = vtkIntArray::SafeDownCast(mPD->GetPointData()->GetScalars(TagArray.toStdString().c_str()));
				}

				if (currentScalars != NULL)
				{


					double Mean = 0;
					double Median = 0;
					double Variance = 0;
					double StdDev = 0;
					double Min = DBL_MAX;
					double Max = -DBL_MAX;
					double Q5 = 0;
					double Q10 = 0;
					double Q90 = 0;
					double Q95 = 0;
					vtkIdType cpt = 0;
					int ok = 0;
					if (mPD->GetNumberOfPoints() > 10)
					{

						std::vector<float> vals;
						
						for (vtkIdType j = 0; j < mPD->GetNumberOfPoints(); j++)
						{
							if (useTags == 0)
							{
								Mean += currentScalars->GetTuple1(j);
								vals.push_back(currentScalars->GetTuple1(j));
							}
							else
							{
								//1: check if currentTags is null.
								//2: check if vertex j is tagged like TagId
								if (currentTags != NULL && currentTags->GetTuple1(j) == TagId)
								{
									cpt++;
									Mean += currentScalars->GetTuple1(j);
									vals.push_back(currentScalars->GetTuple1(j));

								}
							}
						}
						if (useTags == 0)
						{
							Mean /= mPD->GetNumberOfPoints();
							ok = 1;
						}
						else
						{
							if (cpt > 0)
							{
								Mean /= cpt;
								ok = 1;
							}
						}


						for (vtkIdType j = 0; j < mPD->GetNumberOfPoints(); j++)
						{
							if (useTags == 0)
							{
								Variance += (currentScalars->GetTuple1(j) - Mean)*(currentScalars->GetTuple1(j) - Mean);
							}
							else
							{
								//1: check if currentTags is null.
								//2: check if vertex j is tagged like TagId
								if (currentTags != NULL && currentTags->GetTuple1(j) == TagId)
								{
									
									Variance += (currentScalars->GetTuple1(j) - Mean)*(currentScalars->GetTuple1(j) - Mean);									

								}
							}
						}
						
						if (useTags == 0)
						{
							Variance /= mPD->GetNumberOfPoints();
							
						}
						else
						{
							if (cpt > 0)
							{
								Variance /= cpt;
								
							}
						}
						if (ok==1)
						{
							StdDev = sqrt(Variance);
							std::sort(vals.begin(), vals.end());

							int iMin = 0;
							int iMax = 0;
							int iQ5 = 0;
							int iQ10 = 0;
							int iQ50 = 0;
							int iQ90 = 0;
							int iQ95 = 0;
							if (useTags == 0)
							{
								iMax = mPD->GetNumberOfPoints() - 1;
								iQ5 = (int)(0.05*mPD->GetNumberOfPoints());
								iQ10 = (int)(0.1*mPD->GetNumberOfPoints());
								iQ50 = (int)(0.5*mPD->GetNumberOfPoints());
								iQ90 = (int)(0.9*mPD->GetNumberOfPoints());
								iQ95 = (int)(0.95*mPD->GetNumberOfPoints());
							}
							else
							{
								iMax = cpt - 1; if (iMax < 0) { iMax = 0; }
								iQ5 = (int)(0.05*cpt);
								iQ10 = (int)(0.1*cpt);
								iQ50 = (int)(0.5*cpt);
								iQ90 = (int)(0.9*cpt);
								iQ95 = (int)(0.95*cpt);
							}

							Median = (double)vals.at(iQ50);
							Min = (double)vals.at(iMin);
							Max = (double)vals.at(iMax);
							Q5 = (double)vals.at(iQ5);
							Q10 = (double)vals.at(iQ10);
							Q90 = (double)vals.at(iQ90);
							Q95 = (double)vals.at(iQ95);
						}
						


					}



					//stream << myActor->GetName().c_str() << "	" << massProp->GetNormalizedShapeIndex() << "	" << surface_area << "	" << volume <<  endl;
					if (useTags == 0)
					{
						stream << myActor->GetName().c_str() << "	" << Mean << "	" << Median << "	" << Variance << "	" << StdDev << "	" << Min << "	" << Max << "	" << Q5 << "	" << Q10 << "	" << Q90 << "	" << Q95 << "	" << "whole_surface" << endl;
					}
					else
					{
						if (cpt > 10)
						{
							stream << myActor->GetName().c_str() << "	" << Mean << "	" << Median << "	" << Variance << "	" << StdDev << "	" << Min << "	" << Max << "	" << Q5 << "	" << Q10 << "	" << Q90 << "	" << Q95 << "	" << TagName.c_str() << endl;
						}
					}


				}
				else
				{
					stream << myActor->GetName().c_str() << "	" << "NAN" << "	" << "NAN" << "	" << "NAN" << "	" << "NAN" << "	" << "NAN" << "	" << "NAN" << "	" << "NAN" << "	" << "NAN" << "	" << "NAN" << "	" << "NAN" << "	" << "NAN" << endl;
				}


			}
		}
	}
	file.close();

}
void mqMorphoDigCore::Icosahedron(int numIcosahedrons, double radius, int subdivisions, int mode)
{
	// mode=0: icosahedron
	// mode =1: sphere
	double mRadius = radius;
	double bbz = this->getActorCollection()->GetBoundingBox()[5];
	double bbx = this->getActorCollection()->GetBoundingBox()[3];
	double bby = this->getActorCollection()->GetBoundingBox()[4];
	int msubdivisions = subdivisions;
	if (msubdivisions < 0 || msubdivisions == INT_MAX)
	{

		msubdivisions = 0;
		

	}
	if (mRadius <= 0 || mRadius == DBL_MAX)
	{

		mRadius = 1*this->getActorCollection()->GetBoundingBoxLength() / 20;
		if (mRadius == 0 || mRadius == DBL_MAX) { mRadius = 10; }
		
	}

	

	for (int i = 0; i < numIcosahedrons; i++)
	{
		std::string newname = "Icosahedron";

		//GL_VERSION = 1;
		//@@TODO! 
		newname = this->CheckingName(newname);
		vtkSmartPointer<vtkPlatonicSolidSource> icosahedron= vtkSmartPointer<vtkPlatonicSolidSource>::New();
		icosahedron->SetSolidTypeToIcosahedron();
		icosahedron->Update();

		vtkSmartPointer<vtkReverseSense> rev = vtkSmartPointer<vtkReverseSense>::New();
		rev->SetInputData(icosahedron->GetOutput());
		rev->ReverseCellsOn();
		rev->ReverseNormalsOn();
		rev->Update();
		vtkSmartPointer<vtkSphereSource> sphereSource =
			vtkSmartPointer<vtkSphereSource>::New();
		sphereSource->SetRadius(mRadius);
		sphereSource->Update();

		vtkSmartPointer<vtkLoopSubdivisionFilter> loop =
			vtkSmartPointer<vtkLoopSubdivisionFilter>::New();
		vtkSmartPointer<vtkPolyData>MyObj = vtkSmartPointer<vtkPolyData>::New();
		if (msubdivisions > 0)
		{
			if (mode ==1)
			{
				loop->SetInputData(sphereSource->GetOutput());
			}
			else
			{
				loop->SetInputData(rev->GetOutput());
			}
			
			loop->SetNumberOfSubdivisions(msubdivisions);
			loop->Update();
			MyObj = loop->GetOutput();
		}
		else
		{
			if (mode == 1)
			{
				MyObj  = sphereSource->GetOutput();

			}
			else
			{
				MyObj = rev->GetOutput();

			}
		}
		// now project back  MyObj points to radius mRadius Sphere
		vtkSmartPointer<vtkPoints> points =
			vtkSmartPointer<vtkPoints>::New();

		int nrPoints = MyObj->GetNumberOfPoints();
		points->SetNumberOfPoints(nrPoints);
		for (vtkIdType i = 0; i < nrPoints; i++)
		{
			double pt[3];
			double *mpt;
			mpt = MyObj->GetPoint(i);
			pt[0] = mpt[0]; pt[1] = mpt[1]; pt[2] = mpt[2];
			vtkMath::Normalize(pt);
			vtkMath::MultiplyScalar(pt, radius);
			points->SetPoint(i, pt[0], pt[1], pt[2]);
		}

		//cout << "nr of nodes:" << nrPoints << endl;
		
		MyObj->SetPoints(points);
		VTK_CREATE(vtkMDActor, actor);
		if (this->mui_BackfaceCulling == 0)
		{
			actor->GetProperty()->BackfaceCullingOff();
		}
		else
		{
			actor->GetProperty()->BackfaceCullingOn();
		}
		VTK_CREATE(vtkPolyDataMapper, mapper);

		mapper->SetColorModeToDefault();
		mapper->SetInputData(MyObj);

		/*if (msubdivisions > 0)
		{
			mapper->SetInputData(loop->GetOutput());
		}
		else
		{
			if (mode == 1)
			{
				mapper->SetInputData(sphereSource->GetOutput());
				
			}
			else
			{
				mapper->SetInputData(icosahedron->GetOutput());

			}
			
		}
		*/

		//actor->SetmColor(0.68, 0.47, 0.37, 1);//pink
		actor->SetmColor(0.666667, 0.666667, 1, 0.5);//kind of violet
		actor->SetMapper(mapper);
		actor->SetSelected(0);
		actor->SetName(newname);
		actor->SetColorProperties(this->mui_Ambient, this->mui_Diffuse, this->mui_Specular, this->mui_SpecularPower);
		actor->SetDisplayMode(this->mui_DisplayMode);

		vtkSmartPointer<vtkMatrix4x4> Mat = vtkSmartPointer<vtkMatrix4x4>::New();
		Mat->DeepCopy(actor->GetMatrix());



		Mat->SetElement(0, 3, bbx*0.9);
		Mat->SetElement(2, 3, bbz*1.1);
		Mat->SetElement(1, 3, i*(mRadius)*2.1 + bby*0.9);


		vtkTransform *newTransform = vtkTransform::New();
		newTransform->PostMultiply();

		newTransform->SetMatrix(Mat);
		actor->SetPosition(newTransform->GetPosition());
		actor->SetScale(newTransform->GetScale());
		actor->SetOrientation(newTransform->GetOrientation());
		newTransform->Delete();

		this->getActorCollection()->AddItem(actor);
		emit this->actorsMightHaveChanged();

		std::string action = "Create icosahedron";
		int mCount = BEGIN_UNDO_SET(action);
		this->getActorCollection()->CreateLoadUndoSet(mCount, 1);
		END_UNDO_SET();

		this->getActorCollection()->SetChanged(1);
	}

	this->AdjustCameraAndGrid();
	if (this->Getmui_AdjustLandmarkRenderingSize() == 1)
	{
		this->UpdateLandmarkSettings();
	}

}
void mqMorphoDigCore::Cube(int numCubes, double sizeX, double sizeY, double sizeZ)
{
	double msizeX = sizeX;
	double msizeY = sizeY;
	double msizeZ = sizeZ;
	double bbz = this->getActorCollection()->GetBoundingBox()[5];
	double bbx = this->getActorCollection()->GetBoundingBox()[3];
	double bby = this->getActorCollection()->GetBoundingBox()[4];
	
	if (msizeY <= 0 || msizeX == DBL_MAX)
	{
		
		msizeY = this->getActorCollection()->GetBoundingBoxLength() / 20;
		
		if (msizeY == 0 || msizeX == DBL_MAX) { msizeY = 10; }
	}

	if (msizeY <= 0 || msizeY == DBL_MAX)
	{
		msizeX = msizeY/5;
	}
	if (msizeZ <= 0 || msizeZ == DBL_MAX)
	{
		msizeZ = msizeY / 5;
	}

	for (int i = 0; i < numCubes; i++)
	{
		std::string newname = "Cube";


		//@@TODO! 
		newname = this->CheckingName(newname);

		vtkSmartPointer<vtkMDCubeSource> cube = vtkSmartPointer<vtkMDCubeSource>::New();

		cube->SetXLength(msizeX);
		cube->SetYLength(msizeY);
		cube->SetZLength(msizeZ);
		

		//cylinder->SetCenter(0, i*mcylHeight*1.1, 0);
		cube->Update();



		cout << "More than 10 points!" << endl;
		VTK_CREATE(vtkMDActor, actor);
		if (this->mui_BackfaceCulling == 0)
		{
			actor->GetProperty()->BackfaceCullingOff();
		}
		else
		{
			actor->GetProperty()->BackfaceCullingOn();
		}
		VTK_CREATE(vtkPolyDataMapper, mapper);

		mapper->SetColorModeToDefault();

		mapper->SetInputData(cube->GetOutput());


		//actor->SetmColor(0.68, 0.47, 0.37, 1);//pink
		actor->SetmColor(0.666667, 0.666667, 1, 1);//kind of violet
		actor->SetMapper(mapper);
		actor->SetSelected(0);
		actor->SetName(newname);
		actor->SetColorProperties(this->mui_Ambient, this->mui_Diffuse, this->mui_Specular, this->mui_SpecularPower);
		actor->SetDisplayMode(this->mui_DisplayMode);

		vtkSmartPointer<vtkMatrix4x4> Mat = vtkSmartPointer<vtkMatrix4x4>::New();
		Mat->DeepCopy(actor->GetMatrix());



		Mat->SetElement(0, 3, bbx*0.9);
		Mat->SetElement(2, 3, bbz*1.1);		
		Mat->SetElement(1, 3, i*(msizeY)*1.1 + bby*0.9);
	

		vtkTransform *newTransform = vtkTransform::New();
		newTransform->PostMultiply();

		newTransform->SetMatrix(Mat);
		actor->SetPosition(newTransform->GetPosition());
		actor->SetScale(newTransform->GetScale());
		actor->SetOrientation(newTransform->GetOrientation());
		newTransform->Delete();

		this->getActorCollection()->AddItem(actor);
		emit this->actorsMightHaveChanged();

		std::string action = "Create cube";
		int mCount = BEGIN_UNDO_SET(action);
		this->getActorCollection()->CreateLoadUndoSet(mCount, 1);
		END_UNDO_SET();

		this->getActorCollection()->SetChanged(1);
	}

	this->AdjustCameraAndGrid();
	if (this->Getmui_AdjustLandmarkRenderingSize() == 1)
	{
		this->UpdateLandmarkSettings();
	}

}
void mqMorphoDigCore::Cylinder(int numCyl, double cylHeight, double cylRadius, double cylRadius2, int cylResolution, int coneHeight, int mode, int circular_shaft)
{
	//mode: 0= cylinder with flat top and bottom
	//mode: 1= cylinder with conic extremities

	double mcylHeight = cylHeight;
	double mcylRadius = cylRadius;
	double mcylRadius2 = cylRadius;
	double bbz = this->getActorCollection()->GetBoundingBox()[5];
	double bbx = this->getActorCollection()->GetBoundingBox()[3];
	double bby = this->getActorCollection()->GetBoundingBox()[4];
	if (circular_shaft ==0 && cylRadius2>0)
	{
		mcylRadius2 = cylRadius2;
	}
	
	if (cylHeight <= 0 || cylHeight==DBL_MAX)
	{
		mcylHeight = this->getActorCollection()->GetBoundingBoxLength() / 20;
		if (mcylHeight == 0 || mcylHeight == DBL_MAX) { mcylHeight = 10; }
	
	}
	if (mcylHeight == DBL_MAX) { mcylHeight = 10; }
	if (cylRadius <= 0 || cylRadius == DBL_MAX)
	{
		mcylRadius = mcylHeight / 5;

	}
	for (int i = 0; i < numCyl; i++)
	{
		std::string newname = "Cylinder";
		

		//@@TODO! 
		newname = this->CheckingName(newname);

		vtkSmartPointer<vtkMDCylinderSource> cylinder = vtkSmartPointer<vtkMDCylinderSource>::New();
	
		
		  cylinder->SetResolution(cylResolution);
		  cylinder->SetHeight(mcylHeight);
		  cylinder->SetRadius(mcylRadius);
		  cylinder->SetRadius2(mcylRadius2);
		  if (mode ==1)
		  {
			  cylinder->SetConeHeight(coneHeight);
		  }
		  else
		  {
			  cylinder->SetConeHeight(0);
		  }

		  //cylinder->SetCenter(0, i*mcylHeight*1.1, 0);
		  cylinder->Update();

		  

		  cout << "More than 10 points!" << endl;
		  VTK_CREATE(vtkMDActor, actor);
		  if (this->mui_BackfaceCulling == 0)
		  {
			  actor->GetProperty()->BackfaceCullingOff();
		  }
		  else
		  {
			  actor->GetProperty()->BackfaceCullingOn();
		  }
		  VTK_CREATE(vtkPolyDataMapper, mapper);
		 
		  mapper->SetColorModeToDefault();
		 
		 mapper->SetInputData(cylinder->GetOutput());
		 

		 // actor->SetmColor(0.68,0.47,0.37,1);//pink
		 actor->SetmColor(0.666667, 0.666667, 1, 1);//kind of violet
		  actor->SetMapper(mapper);
		  actor->SetSelected(0);
		  actor->SetName(newname);
		  actor->SetColorProperties(this->mui_Ambient, this->mui_Diffuse, this->mui_Specular, this->mui_SpecularPower);
		  actor->SetDisplayMode(this->mui_DisplayMode);
		 
		  vtkSmartPointer<vtkMatrix4x4> Mat = vtkSmartPointer<vtkMatrix4x4>::New();
		  Mat->DeepCopy(actor->GetMatrix());		 

		  Mat->SetElement(0, 3, bbx*0.9);
		  Mat->SetElement(2, 3, bbz*1.1);

		  if (mode == 0)
		  {

			  Mat->SetElement(1, 3, i*(mcylHeight)*1.1+ bby*0.9);
		  }
		  else
		  { 
			  //Mat->SetElement(1, 3, i*mcylRadius*2.2);
			  Mat->SetElement(1, 3, i*(mcylHeight + 2 * coneHeight*mcylHeight / 100)*1.1 +bby*0.9);
		  }
		
		  vtkTransform *newTransform = vtkTransform::New();
		  newTransform->PostMultiply();

		  newTransform->SetMatrix(Mat);
		  actor->SetPosition(newTransform->GetPosition());
		  actor->SetScale(newTransform->GetScale());
		  actor->SetOrientation(newTransform->GetOrientation());
		  newTransform->Delete();

		  this->getActorCollection()->AddItem(actor);
		  emit this->actorsMightHaveChanged();
		
		  std::string action = "Create cylinder";
		  int mCount = BEGIN_UNDO_SET(action);
		  this->getActorCollection()->CreateLoadUndoSet(mCount, 1);
		  END_UNDO_SET();

		  this->getActorCollection()->SetChanged(1);

		   
	}
	
	this->AdjustCameraAndGrid();
	if (this->Getmui_AdjustLandmarkRenderingSize() == 1)
	{
		this->UpdateLandmarkSettings();
	}
	


}
void mqMorphoDigCore::SaveActiveScalarSummary(QString fileName, int useTags, QString TagArray)
{
	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);

		stream << "Name	Mean	Median	Variance	StdDev	Min	Max	Q5	Q10	Q90	Q95	Tagged_Region" << endl;
		cout << "First Summary Done" << endl;
		file.close();
		this->SaveActiveScalarSummary(fileName, 0, TagArray, 0);
		//if useTags =1, search Max tagged value in all opened objects
		if (useTags == 1 && TagArray.length() > 0)
		{
			cout << "useTags==1 and TagArray.length()>0" << endl;
			vtkIdType maxTagId = this->GetTagRangeMax(TagArray);
			this->ActorCollection->InitTraversal();

			cout << "maxTagId=" << maxTagId << endl;
			if (maxTagId > 0)
			{
				for (vtkIdType i = 0; i <= maxTagId; i++)
				{
					cout << "SaveActiveScalarSummary(" << i<<")"<< endl;
					this->SaveActiveScalarSummary(fileName, 1, TagArray, i);
				}
			}

		}//usetags=1
	}//file open

}

void mqMorphoDigCore::SaveMeshSize(QString fileName)
{
	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);
	
			stream << "Name	Bounding_box_length	Mean_Dist_To_Cendroid	Max_Dist_To_Cendroid	PC1_Length	PC2_Length	PC3_Length	AVG_PC_Length" << endl;
		

		//this->ComputeSelectedNamesLists();

		this->ActorCollection->InitTraversal();



		for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
		{
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
			if (myActor->GetSelected() == 1)
			{

				vtkPolyDataMapper *mapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
				if (mapper != NULL && vtkPolyData::SafeDownCast(mapper->GetInput()) != NULL)
				{

					double bbl = myActor->GetBoundingBoxLength();
					double meanCDist = myActor->GetAvgCentroidDistance();
					double maxCDist = myActor->GetMaxCentroidDistance();
					double pc1 = myActor->GetXYZPCLength(0);
					double pc2 = myActor->GetXYZPCLength(1);
					double pc3 = myActor->GetXYZPCLength(2);
					double avgpc = (pc1 + pc2 + pc3) / 3;
					vtkSmartPointer<vtkMassProperties> massProp = vtkSmartPointer<vtkMassProperties>::New();

					massProp->SetInputData(mapper->GetInput());
					massProp->Update();
					double surface_area = massProp->GetSurfaceArea();
					double volume = massProp->GetVolume();
					

						//stream << myActor->GetName().c_str() << "	" << massProp->GetNormalizedShapeIndex() << "	" << surface_area << "	" << volume <<  endl;
						stream << myActor->GetName().c_str() << "	" << bbl << "	" << meanCDist<<  "	" << maxCDist << "	" << pc1 << "	" << pc2 << "	" << pc3 << "	" << avgpc<< endl;
					

				}


			}
		}
	}
	file.close();

}

int mqMorphoDigCore::SaveShapeMeasures(QString fileName, int mode)
{
	//mode: 1: area and volume 
	//mode: 2: normalized shape index area and volume	
	//mode: 3: convex hull area_ratio and ch_normalized_shape_index, area, volume, ch_area, ch_volume
	
	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);
		
		if (mode == 1)
		{
			stream << "Surface_name	Area	Volume	Triangle_nr	Vertex_nr" << endl; 
		}
		else
		if (mode == 2)
		{
			stream << "Surface_name	Normalized_shape_index Area Volume" << endl;
		}
		else
		{
			stream << "Surface_name	Convex_Hull_Area_Ratio Convex_Hull_Shape_Index Area Volume Convex_hull_area Convex_hull_volume Normalized_shape_index" << endl;
		}
		
		//this->ComputeSelectedNamesLists();
	
		this->ActorCollection->InitTraversal();



		for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
		{
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
			if (myActor->GetSelected() == 1)
			{
			
					vtkPolyDataMapper *mapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
					if (mapper != NULL && vtkPolyData::SafeDownCast(mapper->GetInput()) != NULL)
					{
					
						vtkSmartPointer<vtkMassProperties> massProp = vtkSmartPointer<vtkMassProperties>::New();
						
						massProp->SetInputData(mapper->GetInput());
						massProp->Update();
						double surface_area = massProp->GetSurfaceArea();
						double volume = massProp->GetVolume();
						if (mode == 1)
						{
							
							//stream << myActor->GetName().c_str() << "	" << massProp->GetNormalizedShapeIndex() << "	" << surface_area << "	" << volume <<  endl;
							stream << myActor->GetName().c_str() << "	"  << surface_area << "	" << volume << "	" << mapper->GetInput()->GetNumberOfCells()<< "	"<< mapper->GetInput()->GetNumberOfPoints() << endl;
						}
						else if (mode == 2)
						{
							stream << myActor->GetName().c_str() << "	" << massProp->GetNormalizedShapeIndex() << "	" << surface_area << "	" << volume << endl;
						}
						else if (mode ==3)
						{

							/*vtkSmartPointer<vtkPolyData> myPD = vtkSmartPointer<vtkPolyData>::New();
							vtkSmartPointer<vtkCenterOfMass> centerOfMassFilter = vtkSmartPointer<vtkCenterOfMass>::New();
							centerOfMassFilter->SetInputData(mapper->GetInput());
							centerOfMassFilter->SetUseScalarsAsWeights(false);
							double acenter[3];
							centerOfMassFilter->Update();
							centerOfMassFilter->GetCenter(acenter);
							myPD = vtkPolyData::SafeDownCast(mapper->GetInput());
							double ve[3];

							

							vtkIdType id_min = NULL;
							for (vtkIdType j = 0; j < myPD->GetNumberOfPoints(); j++)
							{
								// for every triangle 
								myPD->GetPoint(j, ve);
								

							}*/



					
							vtkSmartPointer<vtkMassProperties> massPropConvexHull = vtkSmartPointer<vtkMassProperties>::New();
							vtkSmartPointer<vtkQuadricDecimation> decimate =
								vtkSmartPointer<vtkQuadricDecimation>::New();

						
							
							vtkSmartPointer<vtkDelaunay3D> delaunay3D =
								vtkSmartPointer<vtkDelaunay3D>::New();
							decimate->SetInputData(mapper->GetInput());
							decimate->SetVolumePreservation(1);
							double numvert = mapper->GetInput()->GetNumberOfPoints();
							double reduction_factor = 0.1;
							double target_number = 100000;
							double new_factor = target_number/ numvert;
							if (new_factor < 1)
							{
								reduction_factor = 1 - new_factor;
							}
							cout << "try to update quadric decimation by a factor of " << reduction_factor<< endl;


							
							if (new_factor < 1)
							{
								decimate->SetTargetReduction(reduction_factor);
								decimate->Update();
								delaunay3D->SetInputData(decimate->GetOutput());
							}
							else
							{
								delaunay3D->SetInputData(mapper->GetInput());
							}
							cout << "try to update Delaunay 3D" << endl;
							delaunay3D->Update();
							cout << "Delaunay 3D updated" << endl;
							vtkSmartPointer<vtkGeometryFilter> geometryFilter =
								vtkSmartPointer<vtkGeometryFilter>::New();

							geometryFilter->SetInputConnection(delaunay3D->GetOutputPort());
							cout << "Try to update geometry filter" << endl;
							geometryFilter->Update();
							cout << "Geometry filter updated" << endl;
							/*vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter2 =
								vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
							surfaceFilter2->SetInputData(geometryFilter->GetOutput());
							surfaceFilter2->Update();*/


							//delaunay3D->SetInputConnection(mapper->GetInputConnection();

							massPropConvexHull->SetInputData(geometryFilter->GetOutput());
							//massPropConvexHull->SetInputConnection(delaunay3D->GetOutputPort());
							cout << "Try to update massPropConvexHull" << endl;
							massPropConvexHull->Update();
							cout << "massPropConvexHull updated" << endl;
							
							double sqrt_surface_area = sqrt(surface_area);

							double surface_area_convex_hull = massPropConvexHull->GetSurfaceArea();
							double sqrt_surface_area_convex_hull = sqrt(surface_area_convex_hull);


							double volume_convex_hull = massPropConvexHull->GetVolume();
							cout << myActor->GetName().c_str() << " volume=" << massProp->GetVolume() << " volume_convex_hull=" << massPropConvexHull->GetVolume() << endl;
							double cbrt_volume_convex_hull = cbrt(volume_convex_hull);
							double custom_complexity = sqrt_surface_area / (cbrt_volume_convex_hull*2.199085233);
							double surface_ratio = 1;
							if (surface_area_convex_hull > 0) { 
									surface_ratio=surface_area / surface_area_convex_hull; 
							}

							
							
							stream << myActor->GetName().c_str() << "	" << surface_ratio << "	" << custom_complexity << "	" << surface_area << "	" << volume << "	" << surface_area_convex_hull << "	" << volume_convex_hull << "	"<< massProp->GetNormalizedShapeIndex()<<  endl;
							
						}
						
					}
			
			
			}
		}
	}
	file.close();
	return 1;
}

void mqMorphoDigCore::startRubber(int rubber_mode)
{
	// rubber_mode 0: = rubber cut (for selected surfaces), and we will only keep the outside of the selection
	//lasso_mode 1: = rubber cut (for selected surfaces), and we will only keep the inside of the selection
	//lasso_mode 2: = rubber tag and we will only tag the outside of the selection
	//lasso_mode 3: = rubber tag and we will only tag the inside of the selection

	//cout << "Set Lasso style as current interaction style!" << endl;
	//1 change interaction mode
	this->getRenderer()->GetRenderWindow()->GetInteractor()->SetInteractorStyle(this->RubberStyle);
	this->currentRubberMode = rubber_mode;
	if (rubber_mode == 0) { this->setCurrentCursor(9); }
	if (rubber_mode == 1) { this->setCurrentCursor(8); }
	if (rubber_mode == 2) { this->setCurrentCursor(9); }
	if (rubber_mode == 3) { this->setCurrentCursor(8); }
	// 2 inform MorphoDigCore that this is a lasso start cut (and not lasso tag)
}

void mqMorphoDigCore::startLasso(int lasso_mode)
{
	// lasso_mode 0: = lasso cut (for selected surfaces), and we will only keep the outside of the selection
	//lasso_mode 1: = lasso cut (for selected surfaces), and we will only keep the inside of the selection
	//lasso_mode 2: = lasso tag and we will only tag the outside of the selection
	//lasso_mode 3: = lasso tag and we will only tag the inside of the selection
	
	//cout << "Set Lasso style as current interaction style!" << endl;
	//1 change interaction mode
	mqMorphoDigCore::instance()->getRenderer()->GetRenderWindow()->GetInteractor()->SetInteractorStyle(this->LassoStyle);
	this->currentLassoMode = lasso_mode;
	if (lasso_mode == 0) { this->setCurrentCursor(3); }
	if (lasso_mode == 1) { this->setCurrentCursor(2); }
	if (lasso_mode == 2) { this->setCurrentCursor(3); }
	if (lasso_mode == 3) { this->setCurrentCursor(2); }
// 2 inform MorphoDigCore that this is a lasso start cut (and not lasso tag)
}
void mqMorphoDigCore::SwitchMoveMode()
{
	if (this->Getmui_MoveMode() == 0 || this->Getmui_MoveMode() == 1) //0 lmk movemode //1 camera //2 move objects
	{
		this->Setmui_MoveMode(2); // switch to move objects
		this->setCurrentCursor(1); //appropriate cursor

	}
	
	else
	{
		this->Setmui_MoveMode(1);// swtich to camera mve mode
		this->setCurrentCursor(0);//appropriate cursor
	}
	emit this->modeModeChanged();
}
void mqMorphoDigCore::resetCursor()
{
	if (this->Getmui_MoveMode() == 0 || this->Getmui_MoveMode() == 1)
	{
		this->setCurrentCursor(0);
	}
	else
	{
		this->setCurrentCursor(1);
	}
}
void mqMorphoDigCore::stopLasso()
{
	this->resetCursor();
	
	//bacl to normal selection mode
	if (this->currentLassoMode == 0|| this->currentLassoMode == 1)// lasso cut
	{
		int keep_inside = this->currentLassoMode;
		this->lassoCutSelectedActors(keep_inside);
	}
	else
	{
		int tag_inside = this->currentLassoMode;// 2: tag insiden 3 tag outside
		this->lassoTagActors(tag_inside);
		
	}
	
	mqMorphoDigCore::instance()->getRenderer()->GetRenderWindow()->GetInteractor()->SetInteractorStyle(this->Style);

}
void mqMorphoDigCore::stopRubber()
{
	this->resetCursor();

	//bacl to normal selection mode
	if (this->currentRubberMode == 0 || this->currentRubberMode == 1)// rubber cut
	{
		int keep_inside = this->currentRubberMode;
		//@@@ to do
		cout << "Stop rubber cut!" << endl;
		this->rubberCutSelectedActors(keep_inside);
	}
	else
	{
		int tag_inside = this->currentRubberMode;// 2: tag insiden 3 tag outside
		//@@@ to do
		cout << "Stop rubber tag!" << endl;
		this->rubberTagActors(tag_inside);

	}

	mqMorphoDigCore::instance()->getRenderer()->GetRenderWindow()->GetInteractor()->SetInteractorStyle(this->Style);

}
void mqMorphoDigCore::setCurrentCursor(int cursor)

{
//0 = hande cursor (move3.png)
//1 = move pointer (move_mode2.png)
//2 = lasso cut keep inside
//3 = lasso cut keep outside
//4 = select mode
//5 = pencil
//6 = paint bucket
//7 = set landmark
//8 = rubber keep inside
//9 = rubber keep outside
	if (this->getQVTKWidget() != NULL)
	{
		if (cursor == 0)
		{

			QPixmap cursor_pixmap = QPixmap(":/Cursors/move3.png");
			QCursor projectCursor = QCursor(cursor_pixmap, 1, 1);


			this->getQVTKWidget()->setCursor(projectCursor);


		}
		else if (cursor == 1)
		{

			QPixmap cursor_pixmap = QPixmap(":/Cursors/move_mode2.png");
			QCursor projectCursor = QCursor(cursor_pixmap, 1, 1);


			this->getQVTKWidget()->setCursor(projectCursor);


		}
		else if (cursor == 2)
		{

			QPixmap cursor_pixmap = QPixmap(":/Cursors/Lasso_keepinside.png");
			QCursor projectCursor = QCursor(cursor_pixmap, 3, 18);


			this->getQVTKWidget()->setCursor(projectCursor);


		}
		else if (cursor == 3)
		{

			QPixmap cursor_pixmap = QPixmap(":/Cursors/Lasso_keepoutside.png");
			QCursor projectCursor = QCursor(cursor_pixmap, 3, 18);


			this->getQVTKWidget()->setCursor(projectCursor);


		}
		else if (cursor == 4)
		{

			QPixmap cursor_pixmap = QPixmap(":/Cursors/select_mode2.png");
			QCursor projectCursor = QCursor(cursor_pixmap, 18, 18);


			this->getQVTKWidget()->setCursor(projectCursor);


		}
		else if (cursor == 5)
		{

			QPixmap cursor_pixmap = QPixmap(":/Cursors/pencil.png");
			QCursor projectCursor = QCursor(cursor_pixmap, 2, 18);


			this->getQVTKWidget()->setCursor(projectCursor);


		}
		else if (cursor == 6)
		{

			QPixmap cursor_pixmap = QPixmap(":/Cursors/Flood_fill.png");
			QCursor projectCursor = QCursor(cursor_pixmap, 18, 18);


			this->getQVTKWidget()->setCursor(projectCursor);


		}
		else if (cursor == 7)
		{

			QPixmap cursor_pixmap = QPixmap(":/Cursors/magic_wand.png");
			QCursor projectCursor = QCursor(cursor_pixmap, 2, 18);


			this->getQVTKWidget()->setCursor(projectCursor);


		}
		else if (cursor == 8)
		{

			QPixmap cursor_pixmap = QPixmap(":/Cursors/rubber_mode_keepinside.png");
			QCursor projectCursor = QCursor(cursor_pixmap, 18, 18);


			this->getQVTKWidget()->setCursor(projectCursor);


		}
		else if (cursor == 9)
		{

			QPixmap cursor_pixmap = QPixmap(":/Cursors/rubber_mode_keepoutside.png");
			QCursor projectCursor = QCursor(cursor_pixmap, 18, 18);


			this->getQVTKWidget()->setCursor(projectCursor);


		}
		else
		{

			QPixmap cursor_pixmap = QPixmap(":/Cursors/move3.png");
			QCursor projectCursor = QCursor(cursor_pixmap, 1, 1);


			this->getQVTKWidget()->setCursor(projectCursor);



		}
	}

}
//changes mouse cursor
void mqMorphoDigCore::lassoCutSelectedActors(int keep_inside)
{
	POLYGON_LIST poly;
	poly.SetPointList(this->LassoStyle->GetPolygonPoints());
	cout << "Poly valide: " << poly.state << endl;
	if (poly.state == 1)// only for valid lasso selections!
	{


		vtkSmartPointer<vtkMDActorCollection> newcoll = vtkSmartPointer<vtkMDActorCollection>::New();
		this->ActorCollection->InitTraversal();
		vtkIdType num = this->ActorCollection->GetNumberOfItems();
		int modified = 0;
		for (vtkIdType i = 0; i < num; i++)
		{
			cout << "try to get next actor:" << i << endl;
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
			if (myActor->GetSelected() == 1)
			{
				//myActor->SetSelected(0); we don't unselect after a cut


				vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
				if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
				{
					vtkPolyData *myPD = vtkPolyData::SafeDownCast(mymapper->GetInput());
				
					vtkSmartPointer<vtkIntArray> newCuts =
						vtkSmartPointer<vtkIntArray>::New();

					newCuts->SetNumberOfComponents(1); //3d normals (ie x,y,z)
					newCuts->SetNumberOfTuples(myPD->GetNumberOfPoints());
					
					double ve_init_pos[3];;
					double ve_final_pos[3];
					double ve_proj_screen[3];
					vtkSmartPointer<vtkMatrix4x4> Mat = myActor->GetMatrix();
					POLYGON_VERTEX proj_screen;
					int proj_is_inside;
					for (vtkIdType i = 0; i < myPD->GetNumberOfPoints(); i++) {
						// for every triangle 
						myPD->GetPoint(i, ve_init_pos);
						mqMorphoDigCore::TransformPoint(Mat, ve_init_pos, ve_final_pos);
						this->GetWorldToDisplay(ve_final_pos[0], ve_final_pos[1], ve_final_pos[2], ve_proj_screen);
						if (i < 10)
						{
							cout << "ve_proj_screen "<<i<<"=" << ve_proj_screen[0] << "," << ve_proj_screen[1] << "," << ve_proj_screen[2] << endl;
						}
						proj_screen.x = ve_proj_screen[0];
						proj_screen.y = ve_proj_screen[1];
						proj_is_inside = poly.POLYGON_POINT_INSIDE(proj_screen);
						if (i < 10)
						{
						}
						
						if (keep_inside == 0)
						{
							if (proj_is_inside == 0) { proj_is_inside = 1; }
							else
							{
								proj_is_inside = 0;
							}
						}
						if ((ve_proj_screen[2] > -1.0) && ve_proj_screen[2] < 1.0 && (proj_is_inside == 1))
						{
							newCuts->InsertTuple1(i, 1);
						}

					}
					newCuts->SetName("Cuts");
					myPD->GetPointData()->AddArray(newCuts);
					myPD->GetPointData()->SetActiveScalars("Cuts");
					
					vtkSmartPointer<vtkThreshold> selector =
						vtkSmartPointer<vtkThreshold>::New();
					vtkSmartPointer<vtkMaskFields> scalarsOff =
						vtkSmartPointer<vtkMaskFields>::New();
					vtkSmartPointer<vtkGeometryFilter> geometry =
						vtkSmartPointer<vtkGeometryFilter>::New();
					selector->SetInputData((vtkPolyData*)myPD);
					selector->SetInputArrayToProcess(0, 0, 0,
						vtkDataObject::FIELD_ASSOCIATION_CELLS,
						vtkDataSetAttributes::SCALARS);
					selector->SetAllScalars(1);// was g_tag_extraction_criterion_all
					selector->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "Cuts");
					selector->ThresholdBetween(0.9, 1.1);
					selector->Update();
					std::cout << "\nSelector new Number of points:" << selector->GetOutput()->GetNumberOfPoints() << std::endl;
					std::cout << "\nSelector new Number of cells:" << selector->GetOutput()->GetNumberOfCells() << std::endl;

					scalarsOff->SetInputData(selector->GetOutput());
					scalarsOff->CopyAttributeOff(vtkMaskFields::POINT_DATA, vtkDataSetAttributes::SCALARS);
					scalarsOff->CopyAttributeOff(vtkMaskFields::CELL_DATA, vtkDataSetAttributes::SCALARS);
					scalarsOff->Update();
					geometry->SetInputData(scalarsOff->GetOutput());
					geometry->Update();


					
					vtkSmartPointer<vtkPolyData> MyObj = vtkSmartPointer<vtkPolyData>::New();


					MyObj = geometry->GetOutput();
					myPD->GetPointData()->RemoveArray("Cuts");

					std::cout << "\nLasso new Number of points:" << MyObj->GetNumberOfPoints() << std::endl;
					std::cout << "\nLasso  new Number of cells:" << MyObj->GetNumberOfCells() << std::endl;

					if (MyObj->GetNumberOfPoints() > 10)
					{
						VTK_CREATE(vtkMDActor, newactor);
						if (this->mui_BackfaceCulling == 0)
						{
							newactor->GetProperty()->BackfaceCullingOff();
						}
						else
						{
							newactor->GetProperty()->BackfaceCullingOn();
						}


						//VTK_CREATE(vtkDataSetMapper, newmapper);
						VTK_CREATE(vtkPolyDataMapper, newmapper);
						//VTK_CREATE(vtkSmartPointer<vtkDataSetMapper>
						//newmapper->ImmediateModeRenderingOn();
						newmapper->SetColorModeToDefault();

						if (
							(this->mui_ActiveArray->DataType == VTK_INT || this->mui_ActiveArray->DataType == VTK_UNSIGNED_INT)
							&& this->mui_ActiveArray->NumComp == 1
							)
						{
							newmapper->SetScalarRange(0, this->Getmui_ActiveTagMap()->numTags - 1);
							newmapper->SetLookupTable(this->Getmui_ActiveTagMap()->TagMap);
							
						}
						else
						{
							newmapper->SetLookupTable(this->Getmui_ActiveColorMap()->ColorMap);
						}


						newmapper->ScalarVisibilityOn();
						
						cout << "cut object" << MyObj->GetNumberOfPoints() << endl;
						//newmapper->SetInputConnection(delaunay3D->GetOutputPort());
						newmapper->SetInputData(MyObj);

						//VTK_CREATE(vtkActor, actor);

						int num = 2;

						vtkSmartPointer<vtkMatrix4x4> Mat = myActor->GetMatrix();
						vtkTransform *newTransform = vtkTransform::New();
						newTransform->PostMultiply();

						newTransform->SetMatrix(Mat);
						newactor->SetPosition(newTransform->GetPosition());
						newactor->SetScale(newTransform->GetScale());
						newactor->SetOrientation(newTransform->GetOrientation());
						newTransform->Delete();


						double color[4] = { 0.5, 0.5, 0.5, 1 };
						myActor->GetmColor(color);
						/*double trans = 100* color[3];
						if (trans >= 0 && trans <= 100)
						{
							color[3] = (double)((double)trans / 100);


						}*/

						newactor->SetmColor(color);

						newactor->SetMapper(newmapper);
						newactor->SetSelected(0);

						//std::string newname = this->CheckingName(myActor->GetName());
						newactor->SetName(myActor->GetName() + "_lc");
						//newactor->SetName(newname);
						cout << "try to add new actor=" << endl;
						newcoll->AddTmpItem(newactor);
						modified = 1;
					}


					


				}
			}
		}
		if (modified == 1)
		{
			newcoll->InitTraversal();
			vtkIdType num = newcoll->GetNumberOfItems();
			for (vtkIdType i = 0; i < num; i++)
			{
				cout << "try to get next actor from newcoll:" << i << endl;
				vtkMDActor *myActor = vtkMDActor::SafeDownCast(newcoll->GetNextActor());
				myActor->SetColorProperties(this->mui_Ambient, this->mui_Diffuse, this->mui_Specular, this->mui_SpecularPower);
				myActor->SetDisplayMode(this->mui_DisplayMode);
				this->getActorCollection()->AddItem(myActor);
				emit this->actorsMightHaveChanged();
				std::string action = "Convex Hull added: " + myActor->GetName();
				int mCount = BEGIN_UNDO_SET(action);
				this->getActorCollection()->CreateLoadUndoSet(mCount, 1);
				END_UNDO_SET();


			}
			//cout << "camera and grid adjusted" << endl;
			cout << "new actor(s) added" << endl;
			this->Initmui_ExistingArrays();

			cout << "Set actor collection changed" << endl;
			this->getActorCollection()->SetChanged(1);
			cout << "Actor collection changed" << endl;

			this->AdjustCameraAndGrid();
			cout << "Camera and grid adjusted" << endl;

			if (this->Getmui_AdjustLandmarkRenderingSize() == 1)
			{
				this->UpdateLandmarkSettings();
			}
			this->Render();
		}

	}
}

void mqMorphoDigCore::rubberCutSelectedActors(int keep_inside)
{
	POLYGON_LIST poly;
	/*@@@ to do:*/

	int mstart[2];
	int mend[2];

	this->RubberStyle->GetStartPosition(mstart);
	this->RubberStyle->GetEndPosition(mend);
	//std::vector<vtkVector2i>
	vtkVector2i a,b,c,d;
	a[0] = mstart[0];
	a[1] = mstart[1];


	b[0] = mstart[0];
	b[1] = mend[1];

	c[0] = mend[0];
	c[1] = mend[1];

	d[0] = mend[0];
	d[1] = mstart[1];
	cout << a[0] << "," << a[1] << endl;
	cout << b[0] << "," << b[1] << endl;
	cout << c[0] << "," << c[1] << endl;
	cout << d[0] << "," << d[1] << endl;

	std::vector<vtkVector2i> point_list;
	point_list.push_back(a);
	point_list.push_back(b);
	point_list.push_back(c);
	point_list.push_back(d);
	poly.SetPointList(point_list);
	cout << "Poly valide: " << poly.state << endl;
	if (poly.state == 1)// only for valid lasso and rubber band selections!
	{


		vtkSmartPointer<vtkMDActorCollection> newcoll = vtkSmartPointer<vtkMDActorCollection>::New();
		this->ActorCollection->InitTraversal();
		vtkIdType num = this->ActorCollection->GetNumberOfItems();
		int modified = 0;
		for (vtkIdType i = 0; i < num; i++)
		{
			cout << "try to get next actor:" << i << endl;
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
			if (myActor->GetSelected() == 1)
			{
				//myActor->SetSelected(0); we don't unselect after a cut


				vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
				if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
				{
					vtkPolyData *myPD = vtkPolyData::SafeDownCast(mymapper->GetInput());

					vtkSmartPointer<vtkIntArray> newCuts =
						vtkSmartPointer<vtkIntArray>::New();

					newCuts->SetNumberOfComponents(1); //3d normals (ie x,y,z)
					newCuts->SetNumberOfTuples(myPD->GetNumberOfPoints());

					double ve_init_pos[3];;
					double ve_final_pos[3];
					double ve_proj_screen[3];
					vtkSmartPointer<vtkMatrix4x4> Mat = myActor->GetMatrix();
					POLYGON_VERTEX proj_screen;
					int proj_is_inside;
					for (vtkIdType i = 0; i < myPD->GetNumberOfPoints(); i++) {
						// for every triangle 
						myPD->GetPoint(i, ve_init_pos);
						mqMorphoDigCore::TransformPoint(Mat, ve_init_pos, ve_final_pos);
						this->GetWorldToDisplay(ve_final_pos[0], ve_final_pos[1], ve_final_pos[2], ve_proj_screen);
						if (i < 10)
						{
							cout << "ve_proj_screen " << i << "=" << ve_proj_screen[0] << "," << ve_proj_screen[1] << "," << ve_proj_screen[2] << endl;
						}
						proj_screen.x = ve_proj_screen[0];
						proj_screen.y = ve_proj_screen[1];
						proj_is_inside = poly.POLYGON_POINT_INSIDE(proj_screen);
						if (i < 10)
						{
						}

						if (keep_inside == 0)
						{
							if (proj_is_inside == 0) { proj_is_inside = 1; }
							else
							{
								proj_is_inside = 0;
							}
						}
						if ((ve_proj_screen[2] > -1.0) && ve_proj_screen[2] < 1.0 && (proj_is_inside == 1))
						{
							newCuts->InsertTuple1(i, 1);
						}

					}
					newCuts->SetName("Cuts");
					myPD->GetPointData()->AddArray(newCuts);
					myPD->GetPointData()->SetActiveScalars("Cuts");

					vtkSmartPointer<vtkThreshold> selector =
						vtkSmartPointer<vtkThreshold>::New();
					vtkSmartPointer<vtkMaskFields> scalarsOff =
						vtkSmartPointer<vtkMaskFields>::New();
					vtkSmartPointer<vtkGeometryFilter> geometry =
						vtkSmartPointer<vtkGeometryFilter>::New();
					selector->SetInputData((vtkPolyData*)myPD);
					selector->SetInputArrayToProcess(0, 0, 0,
						vtkDataObject::FIELD_ASSOCIATION_CELLS,
						vtkDataSetAttributes::SCALARS);
					selector->SetAllScalars(1);// was g_tag_extraction_criterion_all
					selector->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "Cuts");
					selector->ThresholdBetween(0.9, 1.1);
					selector->Update();
					std::cout << "\nSelector new Number of points:" << selector->GetOutput()->GetNumberOfPoints() << std::endl;
					std::cout << "\nSelector new Number of cells:" << selector->GetOutput()->GetNumberOfCells() << std::endl;

					scalarsOff->SetInputData(selector->GetOutput());
					scalarsOff->CopyAttributeOff(vtkMaskFields::POINT_DATA, vtkDataSetAttributes::SCALARS);
					scalarsOff->CopyAttributeOff(vtkMaskFields::CELL_DATA, vtkDataSetAttributes::SCALARS);
					scalarsOff->Update();
					geometry->SetInputData(scalarsOff->GetOutput());
					geometry->Update();



					vtkSmartPointer<vtkPolyData> MyObj = vtkSmartPointer<vtkPolyData>::New();


					MyObj = geometry->GetOutput();
					myPD->GetPointData()->RemoveArray("Cuts");

					std::cout << "\nLasso new Number of points:" << MyObj->GetNumberOfPoints() << std::endl;
					std::cout << "\nLasso  new Number of cells:" << MyObj->GetNumberOfCells() << std::endl;

					if (MyObj->GetNumberOfPoints() > 10)
					{
						VTK_CREATE(vtkMDActor, newactor);
						if (this->mui_BackfaceCulling == 0)
						{
							newactor->GetProperty()->BackfaceCullingOff();
						}
						else
						{
							newactor->GetProperty()->BackfaceCullingOn();
						}


						//VTK_CREATE(vtkDataSetMapper, newmapper);
						VTK_CREATE(vtkPolyDataMapper, newmapper);
						//VTK_CREATE(vtkSmartPointer<vtkDataSetMapper>
						//newmapper->ImmediateModeRenderingOn();
						newmapper->SetColorModeToDefault();

						if (
							(this->mui_ActiveArray->DataType == VTK_INT || this->mui_ActiveArray->DataType == VTK_UNSIGNED_INT)
							&& this->mui_ActiveArray->NumComp == 1
							)
						{
							newmapper->SetScalarRange(0, this->Getmui_ActiveTagMap()->numTags - 1);
							newmapper->SetLookupTable(this->Getmui_ActiveTagMap()->TagMap);

						}
						else
						{
							newmapper->SetLookupTable(this->Getmui_ActiveColorMap()->ColorMap);
						}


						newmapper->ScalarVisibilityOn();

						cout << "cut object" << MyObj->GetNumberOfPoints() << endl;
						//newmapper->SetInputConnection(delaunay3D->GetOutputPort());
						newmapper->SetInputData(MyObj);

						//VTK_CREATE(vtkActor, actor);

						int num = 2;

						vtkSmartPointer<vtkMatrix4x4> Mat = myActor->GetMatrix();
						vtkTransform *newTransform = vtkTransform::New();
						newTransform->PostMultiply();

						newTransform->SetMatrix(Mat);
						newactor->SetPosition(newTransform->GetPosition());
						newactor->SetScale(newTransform->GetScale());
						newactor->SetOrientation(newTransform->GetOrientation());
						newTransform->Delete();


						double color[4] = { 0.5, 0.5, 0.5, 1 };
						myActor->GetmColor(color);
						/*double trans = 100* color[3];
						if (trans >= 0 && trans <= 100)
						{
						color[3] = (double)((double)trans / 100);


						}*/

						newactor->SetmColor(color);

						newactor->SetMapper(newmapper);
						newactor->SetSelected(0);

						//std::string newname = this->CheckingName(myActor->GetName());
						newactor->SetName(myActor->GetName() + "_rc");
						//newactor->SetName(newname);
						cout << "try to add new actor=" << endl;
						newcoll->AddTmpItem(newactor);
						modified = 1;
					}





				}
			}
		}
		if (modified == 1)
		{
			newcoll->InitTraversal();
			vtkIdType num = newcoll->GetNumberOfItems();
			for (vtkIdType i = 0; i < num; i++)
			{
				cout << "try to get next actor from newcoll:" << i << endl;
				vtkMDActor *myActor = vtkMDActor::SafeDownCast(newcoll->GetNextActor());
				myActor->SetColorProperties(this->mui_Ambient, this->mui_Diffuse, this->mui_Specular, this->mui_SpecularPower);
				myActor->SetDisplayMode(this->mui_DisplayMode);
				this->getActorCollection()->AddItem(myActor);
				emit this->actorsMightHaveChanged();
				std::string action = "Convex Hull added: " + myActor->GetName();
				int mCount = BEGIN_UNDO_SET(action);
				this->getActorCollection()->CreateLoadUndoSet(mCount, 1);
				END_UNDO_SET();


			}
			//cout << "camera and grid adjusted" << endl;
			cout << "new actor(s) added" << endl;
			this->Initmui_ExistingArrays();

			cout << "Set actor collection changed" << endl;
			this->getActorCollection()->SetChanged(1);
			cout << "Actor collection changed" << endl;

			this->AdjustCameraAndGrid();
			cout << "Camera and grid adjusted" << endl;

			if (this->Getmui_AdjustLandmarkRenderingSize() == 1)
			{
				this->UpdateLandmarkSettings();
			}
			this->Render();
		}

	}
}

void mqMorphoDigCore::lassoTagActors(int tag_inside)
{
	POLYGON_LIST poly;
	poly.SetPointList(this->LassoStyle->GetPolygonPoints());
	cout << "Poly valide: " << poly.state << endl;

	if (this->Getmui_TagModeActivated() == 1 && poly.state == 1)// only for valid lasso selections!
	{
		std::string action = "Lasso tag ";
		
		int Count = BEGIN_UNDO_SET(action);
		vtkSmartPointer<vtkMDActorCollection> newcoll = vtkSmartPointer<vtkMDActorCollection>::New();
		this->ActorCollection->InitTraversal();
		vtkIdType num = this->ActorCollection->GetNumberOfItems();
		int modified = 0;
		for (vtkIdType i = 0; i < num; i++)
		{
			cout << "try to get next actor:" << i << endl;
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
			//myActor->SetSelected(0); we don't unselect after a cut
			vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
			if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
			{

				QString sActiveArray = this->Getmui_ActiveArray()->Name;
				vtkIntArray *currentTags = vtkIntArray::SafeDownCast(mymapper->GetInput()->GetPointData()->GetScalars(sActiveArray.toStdString().c_str()));
				//4 if current tags exist, retrieve what tool is active
				if (currentTags != NULL)
				{
				

					std::string mArrayName = sActiveArray.toStdString();

					myActor->SaveState(Count, QString(mArrayName.c_str()), 1);


					vtkPolyData *myPD = vtkPolyData::SafeDownCast(mymapper->GetInput());

					vtkSmartPointer<vtkIntArray> toTag =
						vtkSmartPointer<vtkIntArray>::New();

					toTag->SetNumberOfComponents(1); //3d normals (ie x,y,z)
					toTag->SetNumberOfTuples(myPD->GetNumberOfPoints());

					double ve_init_pos[3];;
					double ve_final_pos[3];
					double ve_proj_screen[3];
					vtkSmartPointer<vtkMatrix4x4> Mat = myActor->GetMatrix();
					POLYGON_VERTEX proj_screen;
					int proj_is_inside;
					for (vtkIdType i = 0; i < myPD->GetNumberOfPoints(); i++) {
						// for every triangle 
						myPD->GetPoint(i, ve_init_pos);
						mqMorphoDigCore::TransformPoint(Mat, ve_init_pos, ve_final_pos);
						this->GetWorldToDisplay(ve_final_pos[0], ve_final_pos[1], ve_final_pos[2], ve_proj_screen);
						if (i < 10)
						{
							cout << "ve_proj_screen " << i << "=" << ve_proj_screen[0] << "," << ve_proj_screen[1] << "," << ve_proj_screen[2] << endl;
						}
						proj_screen.x = ve_proj_screen[0];
						proj_screen.y = ve_proj_screen[1];
						proj_is_inside = poly.POLYGON_POINT_INSIDE(proj_screen);
						if (i < 10)
						{
						}

						if (tag_inside == 2)
						{
							if (proj_is_inside == 0) { proj_is_inside = 1; }
							else
							{
								proj_is_inside = 0;
							}
						}
						if ((ve_proj_screen[2] > -1.0) && ve_proj_screen[2] < 1.0 && (proj_is_inside == 1))
						{
							toTag->InsertTuple1(i, 1);
						}

					}

					int activeTag = this->Getmui_ActiveTag();
					for (vtkIdType j = 0; j < myPD->GetNumberOfPoints(); j++)
					{

						int changeTag = toTag->GetTuple1(j);

						if (changeTag == 1)
						{
							currentTags->SetTuple1(j, activeTag);

						}
					}


					//mymapper->GetLookupTable()->
					currentTags->Modified();
					//mymapper->Update();
					

				}

			}

			this->Render();
		}
		//
		END_UNDO_SET();
	}
}
void mqMorphoDigCore::rubberTagActors(int tag_inside)
{
	POLYGON_LIST poly;

	int mstart[2];
	int mend[2];

	this->RubberStyle->GetStartPosition(mstart);
	this->RubberStyle->GetEndPosition(mend);
	//std::vector<vtkVector2i>
	vtkVector2i a, b, c, d;
	a[0] = mstart[0];
	a[1] = mstart[1];

	b[0] = mstart[0];
	b[1] = mend[1];

	c[0] = mend[0];
	c[1] = mend[1];

	d[0] = mend[0];
	d[1] = mstart[1];

	std::vector<vtkVector2i> point_list;
	point_list.push_back(a);
	point_list.push_back(b);
	point_list.push_back(c);
	point_list.push_back(d);
	poly.SetPointList(point_list);


	cout << "Poly valide: " << poly.state << endl;

	if (this->Getmui_TagModeActivated() == 1 && poly.state == 1)// only for valid lasso selections!
	{
		std::string action = "Lasso tag ";

		int Count = BEGIN_UNDO_SET(action);
		vtkSmartPointer<vtkMDActorCollection> newcoll = vtkSmartPointer<vtkMDActorCollection>::New();
		this->ActorCollection->InitTraversal();
		vtkIdType num = this->ActorCollection->GetNumberOfItems();
		int modified = 0;
		for (vtkIdType i = 0; i < num; i++)
		{
			cout << "try to get next actor:" << i << endl;
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
			//myActor->SetSelected(0); we don't unselect after a cut
			vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
			if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
			{

				QString sActiveArray = this->Getmui_ActiveArray()->Name;
				vtkIntArray *currentTags = vtkIntArray::SafeDownCast(mymapper->GetInput()->GetPointData()->GetScalars(sActiveArray.toStdString().c_str()));
				//4 if current tags exist, retrieve what tool is active
				if (currentTags != NULL)
				{


					std::string mArrayName = sActiveArray.toStdString();

					myActor->SaveState(Count, QString(mArrayName.c_str()), 1);


					vtkPolyData *myPD = vtkPolyData::SafeDownCast(mymapper->GetInput());

					vtkSmartPointer<vtkIntArray> toTag =
						vtkSmartPointer<vtkIntArray>::New();

					toTag->SetNumberOfComponents(1); //3d normals (ie x,y,z)
					toTag->SetNumberOfTuples(myPD->GetNumberOfPoints());

					double ve_init_pos[3];;
					double ve_final_pos[3];
					double ve_proj_screen[3];
					vtkSmartPointer<vtkMatrix4x4> Mat = myActor->GetMatrix();
					POLYGON_VERTEX proj_screen;
					int proj_is_inside;
					for (vtkIdType i = 0; i < myPD->GetNumberOfPoints(); i++) {
						// for every triangle 
						myPD->GetPoint(i, ve_init_pos);
						mqMorphoDigCore::TransformPoint(Mat, ve_init_pos, ve_final_pos);
						this->GetWorldToDisplay(ve_final_pos[0], ve_final_pos[1], ve_final_pos[2], ve_proj_screen);
						if (i < 10)
						{
							cout << "ve_proj_screen " << i << "=" << ve_proj_screen[0] << "," << ve_proj_screen[1] << "," << ve_proj_screen[2] << endl;
						}
						proj_screen.x = ve_proj_screen[0];
						proj_screen.y = ve_proj_screen[1];
						proj_is_inside = poly.POLYGON_POINT_INSIDE(proj_screen);
						if (i < 10)
						{
						}

						if (tag_inside == 2)
						{
							if (proj_is_inside == 0) { proj_is_inside = 1; }
							else
							{
								proj_is_inside = 0;
							}
						}
						if ((ve_proj_screen[2] > -1.0) && ve_proj_screen[2] < 1.0 && (proj_is_inside == 1))
						{
							toTag->InsertTuple1(i, 1);
						}

					}

					int activeTag = this->Getmui_ActiveTag();
					for (vtkIdType j = 0; j < myPD->GetNumberOfPoints(); j++)
					{

						int changeTag = toTag->GetTuple1(j);

						if (changeTag == 1)
						{
							currentTags->SetTuple1(j, activeTag);

						}
					}


					//mymapper->GetLookupTable()->
					currentTags->Modified();
					//mymapper->Update();


				}

			}

			this->Render();
		}
		//
		END_UNDO_SET();
	}
}
void mqMorphoDigCore::addConvexHull()
{
	vtkSmartPointer<vtkMDActorCollection> newcoll = vtkSmartPointer<vtkMDActorCollection>::New();
	this->ActorCollection->InitTraversal();
	vtkIdType num = this->ActorCollection->GetNumberOfItems();
	int modified = 0;
	for (vtkIdType i = 0; i < num; i++)
	{
		cout << "try to get next actor:" << i << endl;
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			myActor->SetSelected(0);

			vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
			if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
			{

				vtkSmartPointer<vtkQuadricDecimation> decimate =
					vtkSmartPointer<vtkQuadricDecimation>::New();
				vtkSmartPointer<vtkDelaunay3D> delaunay3D =
					vtkSmartPointer<vtkDelaunay3D>::New();
				decimate->SetInputData(mymapper->GetInput());
				decimate->SetVolumePreservation(1);
				double numvert = mymapper->GetInput()->GetNumberOfPoints();
				double reduction_factor = 0.1;
				double target_number = 100000;
				double new_factor = target_number / numvert;
				if (new_factor < 1)
				{
					reduction_factor = 1 - new_factor;
				}
				cout << "try to update quadric decimation by a factor of " << reduction_factor << endl;
				new_factor = 0.9;

				if (new_factor < 1)
				{
					decimate->SetTargetReduction(reduction_factor);
					decimate->Update();
					delaunay3D->SetInputData(decimate->GetOutput());
				}
				else
				{
					delaunay3D->SetInputData(mymapper->GetInput());
				}
				cout << "try to update Delaunay 3D" << endl;
				delaunay3D->Update();
				cout << "Delaunay 3D updated" << endl;

				vtkSmartPointer<vtkGeometryFilter> geometryFilter =
				vtkSmartPointer<vtkGeometryFilter>::New();

				geometryFilter->SetInputConnection(delaunay3D->GetOutputPort());
				cout << "Try to update geometry filter" << endl;
				geometryFilter->Update();
				cout << "Geometry filter updated" << endl;
				


			
			

				VTK_CREATE(vtkMDActor, newactor);
				if (this->mui_BackfaceCulling == 0)
				{
					newactor->GetProperty()->BackfaceCullingOff();
				}
				else
				{
					newactor->GetProperty()->BackfaceCullingOn();
				}


				//VTK_CREATE(vtkDataSetMapper, newmapper);
				VTK_CREATE(vtkPolyDataMapper, newmapper);
				//VTK_CREATE(vtkSmartPointer<vtkDataSetMapper>
				//newmapper->ImmediateModeRenderingOn();
				newmapper->SetColorModeToDefault();

				if (
					(this->mui_ActiveArray->DataType == VTK_INT || this->mui_ActiveArray->DataType == VTK_UNSIGNED_INT)
					&& this->mui_ActiveArray->NumComp == 1
					)
				{
					newmapper->SetScalarRange(0, this->Getmui_ActiveTagMap()->numTags - 1);
					newmapper->SetLookupTable(this->Getmui_ActiveTagMap()->TagMap);
					
				}
				else
				{
					newmapper->SetLookupTable(this->Getmui_ActiveColorMap()->ColorMap);
				}


				newmapper->ScalarVisibilityOn();
				VTK_CREATE(vtkPolyData, myData);
				myData = geometryFilter->GetOutput();
				cout << "myConvexHull: nv=" << myData->GetNumberOfPoints() << endl;
				//newmapper->SetInputConnection(delaunay3D->GetOutputPort());
				newmapper->SetInputData(myData);
				
				//VTK_CREATE(vtkActor, actor);

				int num = 2;

				vtkSmartPointer<vtkMatrix4x4> Mat = myActor->GetMatrix();
				vtkTransform *newTransform = vtkTransform::New();
				newTransform->PostMultiply();
				
				newTransform->SetMatrix(Mat);
				newactor->SetPosition(newTransform->GetPosition());
				newactor->SetScale(newTransform->GetScale());
				newactor->SetOrientation(newTransform->GetOrientation());						
				newTransform->Delete();


				double color[4] = { 0.5, 0.5, 0.5, 1 };
				myActor->GetmColor(color);
				double trans = 20;
				if (trans >= 0 && trans <= 100)
				{
					color[3] = (double)((double)trans / 100);


				}
				
				newactor->SetmColor(color);

				newactor->SetMapper(newmapper);
				newactor->SetSelected(0);
				

				newactor->SetName("CH" + myActor->GetName());
				cout << "try to add new actor=" << endl;
				newcoll->AddTmpItem(newactor);
				modified = 1;
				

			}
		}
	}
	if (modified ==1)
	{
		newcoll->InitTraversal();
		vtkIdType num = newcoll->GetNumberOfItems();
		for (vtkIdType i = 0; i < num; i++)
		{
			cout << "try to get next actor from newcoll:" << i << endl;
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(newcoll->GetNextActor());

			myActor->SetDisplayMode(this->mui_DisplayMode);
			this->getActorCollection()->AddItem(myActor);
			emit this->actorsMightHaveChanged();
			std::string action = "Convex Hull added: " + myActor->GetName();
			int mCount = BEGIN_UNDO_SET(action);
			this->getActorCollection()->CreateLoadUndoSet(mCount, 1);
			END_UNDO_SET();

		
		}
		//cout << "camera and grid adjusted" << endl;
		cout << "new actor(s) added" << endl;
		this->Initmui_ExistingArrays();

		cout << "Set actor collection changed" << endl;
		this->getActorCollection()->SetChanged(1);
		cout << "Actor collection changed" << endl;

		this->AdjustCameraAndGrid();
		cout << "Camera and grid adjusted" << endl;

		if (this->Getmui_AdjustLandmarkRenderingSize() == 1)
		{
			this->UpdateLandmarkSettings();
		}
		this->Render();
	}
}
void mqMorphoDigCore::addMirrorXZ()
{
	vtkSmartPointer<vtkMDActorCollection> newcoll = vtkSmartPointer<vtkMDActorCollection>::New();
	this->ActorCollection->InitTraversal();
	vtkIdType num = this->ActorCollection->GetNumberOfItems();
	int modified = 0;
	for (vtkIdType i = 0; i < num; i++)
	{
		cout << "Mirror XZ try to get next actor:" << i << endl;
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			//myActor->SetSelected(0);

			vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
			if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
			{

				
				double numvert = mymapper->GetInput()->GetNumberOfPoints();

				VTK_CREATE(vtkMDActor, newactor);
				if (this->mui_BackfaceCulling == 0)
				{
					newactor->GetProperty()->BackfaceCullingOff();
				}
				else
				{
					newactor->GetProperty()->BackfaceCullingOn();
				}

				VTK_CREATE(vtkPolyDataMapper, newmapper);
				//newmapper->ImmediateModeRenderingOn();
				newmapper->SetColorModeToDefault();

				if (
					(this->mui_ActiveArray->DataType == VTK_INT || this->mui_ActiveArray->DataType == VTK_UNSIGNED_INT)
					&& this->mui_ActiveArray->NumComp == 1
					)
				{
					newmapper->SetScalarRange(0, this->Getmui_ActiveTagMap()->numTags - 1);
					newmapper->SetLookupTable(this->Getmui_ActiveTagMap()->TagMap);
					
				}
				else
				{
					newmapper->SetLookupTable(this->Getmui_ActiveColorMap()->ColorMap);
				}

				newmapper->ScalarVisibilityOn();
				VTK_CREATE(vtkPolyData, myData);
				vtkSmartPointer<vtkReflectionFilter> mfilter = vtkSmartPointer<vtkReflectionFilter>::New();
				mfilter->CopyInputOff();
				mfilter->SetInputData(vtkPolyData::SafeDownCast(mymapper->GetInput()));
				mfilter->SetPlaneToY();
				
				mfilter->Update();
				vtkSmartPointer<vtkUnstructuredGrid> grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
				grid = (vtkUnstructuredGrid*)mfilter->GetOutput();
				vtkSmartPointer<vtkGeometryFilter> fgeo = vtkSmartPointer<vtkGeometryFilter>::New();

				fgeo->SetInputData(grid);
				fgeo->Update();
				//MyObj = fgeo->GetOutput();

				myData = fgeo->GetOutput();
				cout << "myMirror: nv=" << myData->GetNumberOfPoints() << endl;
				//newmapper->SetInputConnection(delaunay3D->GetOutputPort());
				

				//VTK_CREATE(vtkActor, actor);

		
				vtkSmartPointer<vtkCleanPolyData> cleanPolyDataFilter = vtkSmartPointer<vtkCleanPolyData>::New();
				//cleanPolyDataFilter->SetInputData(ObjNormals->GetOutput());
				cleanPolyDataFilter->SetInputData(myData);
				cleanPolyDataFilter->PieceInvariantOff();
				cleanPolyDataFilter->ConvertLinesToPointsOff();
				cleanPolyDataFilter->ConvertPolysToLinesOff();
				cleanPolyDataFilter->ConvertStripsToPolysOff();
				cleanPolyDataFilter->PointMergingOn();
				cleanPolyDataFilter->Update();

				myData = cleanPolyDataFilter->GetOutput();

				std::cout << "\nVtkReflection new Number of points:" << myData->GetNumberOfPoints() << std::endl;
				std::cout << "VtkReflection new Number of cells:" << myData->GetNumberOfCells() << std::endl;

				newmapper->SetInputData(myData);

				

				
				vtkSmartPointer<vtkMatrix4x4> MatOrig = myActor->GetMatrix();
				vtkSmartPointer<vtkMatrix4x4> Mat = vtkSmartPointer<vtkMatrix4x4>::New();
				Mat->DeepCopy (myActor->GetMatrix());

				double n1, n2, n3, n4, n5;
				n1 = -1 * MatOrig->GetElement(1, 3);
				n2 = -1 * MatOrig->GetElement(0, 1);
				n3 = -1 * MatOrig->GetElement(1, 0);
				n4 = -1 * MatOrig->GetElement(1, 2);
				n5 = -1 * MatOrig->GetElement(2, 1);
			



				Mat->SetElement(1, 3, n1);
				Mat->SetElement(0, 1, n2);
				Mat->SetElement(1, 0, n3);
				Mat->SetElement(1, 2, n4);
				Mat->SetElement(2, 1, n5);
				

				vtkTransform *newTransform = vtkTransform::New();
				newTransform->PostMultiply();

				newTransform->SetMatrix(Mat);
				newactor->SetPosition(newTransform->GetPosition());
				newactor->SetScale(newTransform->GetScale());
				newactor->SetOrientation(newTransform->GetOrientation());
				newTransform->Delete();


				double color[4] = { 0.5, 0.5, 0.5, 1 };
				myActor->GetmColor(color);
				newactor->SetmColor(color);

				newactor->SetMapper(newmapper);
				newactor->SetSelected(0);


				newactor->SetName(myActor->GetName()+"_mir");
				cout << "try to add new actor=" << endl;
				newcoll->AddTmpItem(newactor);
				modified = 1;


			}
		}
	}
	if (modified == 1)
	{
		newcoll->InitTraversal();
		vtkIdType num = newcoll->GetNumberOfItems();
		for (vtkIdType i = 0; i < num; i++)
		{
			cout << "try to get next actor from newcoll:" << i << endl;
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(newcoll->GetNextActor());
			myActor->SetColorProperties(this->mui_Ambient, this->mui_Diffuse, this->mui_Specular, this->mui_SpecularPower);
			myActor->SetDisplayMode(this->mui_DisplayMode);
			this->getActorCollection()->AddItem(myActor);
			emit this->actorsMightHaveChanged();
			std::string action = "Mirror object added: " + myActor->GetName();
			int mCount = BEGIN_UNDO_SET(action);
			this->getActorCollection()->CreateLoadUndoSet(mCount, 1);
			END_UNDO_SET();

			
		}
		//cout << "camera and grid adjusted" << endl;
		cout << "new actor(s) added" << endl;
		this->Initmui_ExistingArrays();

		cout << "Set actor collection changed" << endl;
		this->getActorCollection()->SetChanged(1);
		cout << "Actor collection changed" << endl;

		this->AdjustCameraAndGrid();
		cout << "Camera and grid adjusted" << endl;

		if (this->Getmui_AdjustLandmarkRenderingSize() == 1)
		{
			this->UpdateLandmarkSettings();
		}
		this->Render();
	}
}

bool mqMorphoDigCore::RecoverLandmarks(vtkSmartPointer< vtkPoints > landmarks_list_source, vtkSmartPointer< vtkPoints > landmarks_list_target, int all) {

	//all = 0 : only selected landmarks
	//all = 1 : all landmarks
	
	if (this->NormalLandmarkCollection->GetNumberOfItems() ==0)
	{
		QMessageBox msgBox;
		msgBox.setText("ERROR:: RecoverLandmarks : no landmarks.");
		msgBox.exec();
		return false;
	}

	if (all == 0)
	{
		if (this->NormalLandmarkCollection->GetNumberOfSelectedActors() != this->TargetLandmarkCollection->GetNumberOfSelectedActors())
		{
			QMessageBox msgBox;
			msgBox.setText("ERROR:: RecoverLandmarks : not the same number of landmarks.");
			msgBox.exec();
			return false;
		}
		if (this->NormalLandmarkCollection->GetNumberOfSelectedActors() ==0)
		{
			QMessageBox msgBox;
			msgBox.setText("ERROR:: RecoverLandmarks : no selected landmarks.");
			msgBox.exec();
			return false;
			
		}
	}

	if (this->NormalLandmarkCollection->GetNumberOfItems() != this->TargetLandmarkCollection->GetNumberOfItems())
	{
		QMessageBox msgBox;
		msgBox.setText("ERROR:: RecoverLandmarks : not the same number of landmarks.");
		msgBox.exec();		
		return false;
	}
	
	// 
		
	if (all == 1)
	{
		landmarks_list_source->SetNumberOfPoints(this->NormalLandmarkCollection->GetNumberOfItems());
		landmarks_list_target->SetNumberOfPoints(this->NormalLandmarkCollection->GetNumberOfItems());
	}
	else
	{
		landmarks_list_source->SetNumberOfPoints(this->NormalLandmarkCollection->GetNumberOfSelectedActors());
		landmarks_list_target->SetNumberOfPoints(this->NormalLandmarkCollection->GetNumberOfSelectedActors());
	}

		
		vtkIdType cpt = 0;
		this->NormalLandmarkCollection->InitTraversal();
		this->TargetLandmarkCollection->InitTraversal();
		for (vtkIdType i = 0; i < NormalLandmarkCollection->GetNumberOfItems(); i++)
		{
			vtkLMActor *mySourceActor = vtkLMActor::SafeDownCast(NormalLandmarkCollection->GetNextActor());
			vtkLMActor *myTargetActor = vtkLMActor::SafeDownCast(TargetLandmarkCollection->GetNextActor());
			if (mySourceActor->GetSelected() == 1 || all == 1)
			{

				double lmsourcepos[3];
				mySourceActor->GetLMOrigin(lmsourcepos);

				double lmtargetpos[3];
				myTargetActor->GetLMOrigin(lmtargetpos);
				//cout << "source" << i <<":"<< lmsourcepos[0] << "," << lmsourcepos[1] << "," << lmsourcepos[2] << endl;
				landmarks_list_source->SetPoint(cpt, lmsourcepos);
				//cout << "target" << i << ":" << lmtargetpos[0] << ","<< lmtargetpos[1]<<","<< lmtargetpos[2]<< endl;
				landmarks_list_target->SetPoint(cpt, lmtargetpos);
				cpt++;
			}

		}
						
		return true;
	
}


void mqMorphoDigCore::addTPS(int r, double factor, int all)
{

	if (this->NormalLandmarkCollection->GetNumberOfItems() == 0)
	{
		QMessageBox msgBox;
		msgBox.setText("ERROR:: RecoverLandmarks : no landmarks.");
		msgBox.exec();
		return;
	}

	if (all == 0)
	{
		if (this->NormalLandmarkCollection->GetNumberOfSelectedActors() != this->TargetLandmarkCollection->GetNumberOfSelectedActors())
		{
			QMessageBox msgBox;
			msgBox.setText("ERROR:: RecoverLandmarks : not the same number of landmarks.");
			msgBox.exec();
			return;
		}
		if (this->NormalLandmarkCollection->GetNumberOfSelectedActors() == 0)
		{
			QMessageBox msgBox;
			msgBox.setText("ERROR:: RecoverLandmarks : no selected landmarks.");
			msgBox.exec();
			return;

		}
	}

	if (this->NormalLandmarkCollection->GetNumberOfItems() != this->TargetLandmarkCollection->GetNumberOfItems())
	{
		QMessageBox msgBox;
		msgBox.setText("ERROR:: RecoverLandmarks : not the same number of landmarks.");
		msgBox.exec();
		return;
	}

	

	vtkSmartPointer<vtkMDActorCollection> newcoll = vtkSmartPointer<vtkMDActorCollection>::New();
	this->ActorCollection->InitTraversal();
	vtkIdType num = this->ActorCollection->GetNumberOfItems();
	int modified = 0;
	for (vtkIdType i = 0; i < num; i++)
	{
		cout << "Fill holes try to get next actor:" << i << endl;
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
			if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
			{
				double numvert = mymapper->GetInput()->GetNumberOfPoints();

				

				VTK_CREATE(vtkMDActor, newactor);
				if (this->mui_BackfaceCulling == 0)
				{
					newactor->GetProperty()->BackfaceCullingOff();
				}
				else
				{
					newactor->GetProperty()->BackfaceCullingOn();
				}

				VTK_CREATE(vtkPolyDataMapper, newmapper);
				//newmapper->ImmediateModeRenderingOn();
				newmapper->SetColorModeToDefault();

				if (
					(this->mui_ActiveArray->DataType == VTK_INT || this->mui_ActiveArray->DataType == VTK_UNSIGNED_INT)
					&& this->mui_ActiveArray->NumComp == 1
					)
				{
					newmapper->SetScalarRange(0, this->Getmui_ActiveTagMap()->numTags - 1);
					newmapper->SetLookupTable(this->Getmui_ActiveTagMap()->TagMap);

				}
				else
				{
					newmapper->SetLookupTable(this->Getmui_ActiveColorMap()->ColorMap);
				}

				newmapper->ScalarVisibilityOn();

				cout << "TPS basis=" << r<< endl;
			
				/// TPS
				vtkSmartPointer<vtkThinPlateSplineTransform> tps =
				vtkSmartPointer<vtkThinPlateSplineTransform>::New();

				if (r == 1)
				{
					tps->SetBasisToR();
				}
				else
				{
					tps->SetBasisToR2LogR();
				}
				vtkSmartPointer< vtkPoints > p1 = vtkSmartPointer< vtkPoints >::New();
				vtkSmartPointer< vtkPoints > p2 = vtkSmartPointer< vtkPoints >::New();

				// fetch source and target landmarks
				RecoverLandmarks(p1, p2, all);

				tps->SetSourceLandmarks(p1);
				tps->SetTargetLandmarks(p2);
				tps->Update();

				vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
				vtkSmartPointer<vtkPolyData> MyTPSInput = vtkSmartPointer<vtkPolyData>::New();
				vtkSmartPointer<vtkPolyData> MyInput = vtkSmartPointer<vtkPolyData>::New();
				MyInput=mymapper->GetInput();
				MyTPSInput->DeepCopy(mymapper->GetInput());

				// recupère la position de l'object s'il a bougé
				double ve_init_pos[3];;
				double ve_final_pos[3];
				double ve_trans_pos[3];
				vtkSmartPointer<vtkMatrix4x4> Mat = myActor->GetMatrix();


				for (vtkIdType i = 0; i < MyTPSInput->GetNumberOfPoints(); i++) {
					// for every triangle 
					MyTPSInput->GetPoint(i, ve_init_pos);
					mqMorphoDigCore::TransformPoint(Mat, ve_init_pos, ve_final_pos);

					MyTPSInput->GetPoints()->SetPoint((vtkIdType)i, ve_final_pos);
				}
				


				transformFilter->SetInputData(MyTPSInput);

				/// applique le calcul du tps à l'objet
				transformFilter->SetTransform(tps);
				transformFilter->Update();

				// mise à jour du maillage sortant pour le tps
				vtkSmartPointer<vtkPolyData> My_Output = vtkSmartPointer<vtkPolyData>::New();
				My_Output = transformFilter->GetOutput();

			

				// % deformation

				double mfactor = factor / 100;

				for (vtkIdType i = 0; i<My_Output->GetNumberOfPoints(); i++) {
					// for every triangle 
					MyTPSInput->GetPoint(i, ve_init_pos);
					My_Output->GetPoint(i, ve_final_pos);
					ve_trans_pos[0] = mfactor*(float)ve_final_pos[0] + (1 - mfactor)*ve_init_pos[0];
					ve_trans_pos[1] = mfactor*(float)ve_final_pos[1] + (1 - mfactor)*ve_init_pos[1];
					ve_trans_pos[2] = mfactor*(float)ve_final_pos[2] + (1 - mfactor)*ve_init_pos[2];

					My_Output->GetPoints()->SetPoint(i, ve_trans_pos);
				}
				



				vtkSmartPointer<vtkCleanPolyData> cleanPolyDataFilter = vtkSmartPointer<vtkCleanPolyData>::New();
				//cleanPolyDataFilter->SetInputData(ObjNormals->GetOutput());
				cleanPolyDataFilter->SetInputData(My_Output);

				cleanPolyDataFilter->PieceInvariantOff();
				cleanPolyDataFilter->ConvertLinesToPointsOff();
				cleanPolyDataFilter->ConvertPolysToLinesOff();
				cleanPolyDataFilter->ConvertStripsToPolysOff();
				cleanPolyDataFilter->PointMergingOn();
				cleanPolyDataFilter->Update();
				VTK_CREATE(vtkPolyData, myData);

				myData = cleanPolyDataFilter->GetOutput();

				cout << "TPS: nv=" << myData->GetNumberOfPoints() << endl;
				//newmapper->SetInputConnection(delaunay3D->GetOutputPort());



				newmapper->SetInputData(myData);




				/*vtkSmartPointer<vtkMatrix4x4> Mat = vtkSmartPointer<vtkMatrix4x4>::New();
				Mat = myActor->GetMatrix();


				vtkTransform *newTransform = vtkTransform::New();
				newTransform->PostMultiply();

				newTransform->SetMatrix(Mat);
				newactor->SetPosition(newTransform->GetPosition());
				newactor->SetScale(newTransform->GetScale());
				newactor->SetOrientation(newTransform->GetOrientation());
				newTransform->Delete();*/


				double color[4] = { 0.5, 0.5, 0.5, 1 };
				myActor->GetmColor(color);

				color[3] = 0.5;
				newactor->SetmColor(color);

				newactor->SetMapper(newmapper);
				newactor->SetSelected(0);


				newactor->SetName(myActor->GetName() + "_tps");
				cout << "try to add new actor=" << endl;
				newcoll->AddTmpItem(newactor);
				modified = 1;


			}
		}
	}
	if (modified == 1)
	{
		newcoll->InitTraversal();
		vtkIdType num = newcoll->GetNumberOfItems();
		for (vtkIdType i = 0; i < num; i++)
		{
			cout << "try to get next actor from newcoll:" << i << endl;
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(newcoll->GetNextActor());
			myActor->SetColorProperties(this->mui_Ambient, this->mui_Diffuse, this->mui_Specular, this->mui_SpecularPower);
			myActor->SetDisplayMode(this->mui_DisplayMode);
			this->getActorCollection()->AddItem(myActor);
			emit this->actorsMightHaveChanged();
			std::string action = "TPS object added: " + myActor->GetName();
			int mCount = BEGIN_UNDO_SET(action);
			this->getActorCollection()->CreateLoadUndoSet(mCount, 1);
			END_UNDO_SET();


		}
		//cout << "camera and grid adjusted" << endl;
		cout << "new actor(s) added" << endl;
		this->Initmui_ExistingArrays();

		cout << "Set actor collection changed" << endl;
		this->getActorCollection()->SetChanged(1);
		cout << "Actor collection changed" << endl;

		this->AdjustCameraAndGrid();
		cout << "Camera and grid adjusted" << endl;

		if (this->Getmui_AdjustLandmarkRenderingSize() == 1)
		{
			this->UpdateLandmarkSettings();
		}
		this->Render();
	}



}
void mqMorphoDigCore::addFillHoles(int maxsize) 
{
	vtkSmartPointer<vtkMDActorCollection> newcoll = vtkSmartPointer<vtkMDActorCollection>::New();
	this->ActorCollection->InitTraversal();
	vtkIdType num = this->ActorCollection->GetNumberOfItems();
	int modified = 0;
	for (vtkIdType i = 0; i < num; i++)
	{
		cout << "Fill holes try to get next actor:" << i << endl;
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
			if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
			{
				double numvert = mymapper->GetInput()->GetNumberOfPoints();

				

				VTK_CREATE(vtkMDActor, newactor);
				if (this->mui_BackfaceCulling == 0)
				{
					newactor->GetProperty()->BackfaceCullingOff();
				}
				else
				{
					newactor->GetProperty()->BackfaceCullingOn();
				}

				VTK_CREATE(vtkPolyDataMapper, newmapper);
				//newmapper->ImmediateModeRenderingOn();
				newmapper->SetColorModeToDefault();

				if (
					(this->mui_ActiveArray->DataType == VTK_INT || this->mui_ActiveArray->DataType == VTK_UNSIGNED_INT)
					&& this->mui_ActiveArray->NumComp == 1
					)
				{
					newmapper->SetScalarRange(0, this->Getmui_ActiveTagMap()->numTags - 1);
					newmapper->SetLookupTable(this->Getmui_ActiveTagMap()->TagMap);

				}
				else
				{
					newmapper->SetLookupTable(this->Getmui_ActiveColorMap()->ColorMap);
				}

				newmapper->ScalarVisibilityOn();
				
				cout << "holes max size =" << maxsize << endl;
				vtkSmartPointer<vtkFillHolesFilter> fillholes =
					vtkSmartPointer<vtkFillHolesFilter>::New();
				fillholes->SetInputData(vtkPolyData::SafeDownCast(mymapper->GetInput()));
				fillholes->SetHoleSize(maxsize);
				
				fillholes->Update();
				
				vtkSmartPointer<vtkPolyDataNormals> ObjNormals = vtkSmartPointer<vtkPolyDataNormals>::New();
				ObjNormals->SetInputData(fillholes->GetOutput());
				ObjNormals->ComputePointNormalsOn();
				ObjNormals->ComputeCellNormalsOn();
				//ObjNormals->AutoOrientNormalsOff();
				ObjNormals->ConsistencyOff();

				ObjNormals->Update();

				vtkSmartPointer<vtkCleanPolyData> cleanPolyDataFilter = vtkSmartPointer<vtkCleanPolyData>::New();
				cleanPolyDataFilter->SetInputData(ObjNormals->GetOutput());
				//cleanPolyDataFilter->SetInputData(fillholes->GetOutput());
				cleanPolyDataFilter->PieceInvariantOff();
				cleanPolyDataFilter->ConvertLinesToPointsOff();
				cleanPolyDataFilter->ConvertPolysToLinesOff();
				cleanPolyDataFilter->ConvertStripsToPolysOff();
				cleanPolyDataFilter->PointMergingOn();
				cleanPolyDataFilter->Update();
				VTK_CREATE(vtkPolyData, myData);

				myData = cleanPolyDataFilter->GetOutput();
				//myData = fillholes->GetOutput();
				cout << "original surface holes: nv=" << mymapper->GetInput()->GetNumberOfPoints() << endl;
				cout << "fill holes: nv=" << myData->GetNumberOfPoints() << endl;
			




				newmapper->SetInputData(myData);




				vtkSmartPointer<vtkMatrix4x4> Mat = vtkSmartPointer<vtkMatrix4x4>::New();
				Mat = myActor->GetMatrix();



				vtkTransform *newTransform = vtkTransform::New();
				newTransform->PostMultiply();

				newTransform->SetMatrix(Mat);
				newactor->SetPosition(newTransform->GetPosition());
				newactor->SetScale(newTransform->GetScale());
				newactor->SetOrientation(newTransform->GetOrientation());
				newTransform->Delete();


				double color[4] = { 0.5, 0.5, 0.5, 1 };
				myActor->GetmColor(color);
				newactor->SetmColor(color);

				newactor->SetMapper(newmapper);
				newactor->SetSelected(0);


				newactor->SetName(myActor->GetName() + "_holesfilled");
				cout << "try to add new actor=" << endl;
				newcoll->AddTmpItem(newactor);
				modified = 1;


			}
		}
	}
	if (modified == 1)
	{
		newcoll->InitTraversal();
		vtkIdType num = newcoll->GetNumberOfItems();
		for (vtkIdType i = 0; i < num; i++)
		{
			cout << "try to get next actor from newcoll:" << i << endl;
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(newcoll->GetNextActor());
			myActor->SetColorProperties(this->mui_Ambient, this->mui_Diffuse, this->mui_Specular, this->mui_SpecularPower);
			myActor->SetDisplayMode(this->mui_DisplayMode);
			this->getActorCollection()->AddItem(myActor);
			emit this->actorsMightHaveChanged();
			std::string action = "Hole filled object added: " + myActor->GetName();
			int mCount = BEGIN_UNDO_SET(action);
			this->getActorCollection()->CreateLoadUndoSet(mCount, 1);
			END_UNDO_SET();


		}
		//cout << "camera and grid adjusted" << endl;
		cout << "new actor(s) added" << endl;
		this->Initmui_ExistingArrays();

		cout << "Set actor collection changed" << endl;
		this->getActorCollection()->SetChanged(1);
		cout << "Actor collection changed" << endl;

		this->AdjustCameraAndGrid();
		cout << "Camera and grid adjusted" << endl;

		if (this->Getmui_AdjustLandmarkRenderingSize() == 1)
		{
			this->UpdateLandmarkSettings();
		}
		this->Render();
	}

}
void mqMorphoDigCore::addDensify(int subdivisions, int method) 
{
	vtkSmartPointer<vtkMDActorCollection> newcoll = vtkSmartPointer<vtkMDActorCollection>::New();
	this->ActorCollection->InitTraversal();
	vtkIdType num = this->ActorCollection->GetNumberOfItems();
	int modified = 0;
	int msg_nonmanifold = 0; // at the end of the loop, if msg_nonmanifold = 1 => send message that some filters failed
	for (vtkIdType i = 0; i < num; i++)
	{
		int ok_to_add = 1;
		cout << "Densify try to get next actor:" << i << endl;
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
			if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
			{
				double numvert = mymapper->GetInput()->GetNumberOfPoints();
				if (method > 0)
				{
					vtkSmartPointer<vtkFeatureEdges> featureEdges = vtkSmartPointer<vtkFeatureEdges>::New();
					featureEdges->SetInputData(vtkPolyData::SafeDownCast(mymapper->GetInput()));
					featureEdges->BoundaryEdgesOff();
					featureEdges->FeatureEdgesOff();
					featureEdges->ManifoldEdgesOff();
					featureEdges->NonManifoldEdgesOn();
					featureEdges->Update();
					cout << "Feature edges: nr of points in non manyfold edges" << featureEdges->GetOutput()->GetNumberOfPoints() << endl;
					if (featureEdges->GetOutput()->GetNumberOfPoints()>0)
					{
						msg_nonmanifold = 1;
						ok_to_add = 0;
					}
				}
				if (ok_to_add == 1)
				{
					VTK_CREATE(vtkMDActor, newactor);

					if (this->mui_BackfaceCulling == 0)
					{
						newactor->GetProperty()->BackfaceCullingOff();
					}
					else
					{
						newactor->GetProperty()->BackfaceCullingOn();
					}

					VTK_CREATE(vtkPolyDataMapper, newmapper);
					//newmapper->ImmediateModeRenderingOn();
					newmapper->SetColorModeToDefault();

					if (
						(this->mui_ActiveArray->DataType == VTK_INT || this->mui_ActiveArray->DataType == VTK_UNSIGNED_INT)
						&& this->mui_ActiveArray->NumComp == 1
						)
					{
						newmapper->SetScalarRange(0, this->Getmui_ActiveTagMap()->numTags - 1);
						newmapper->SetLookupTable(this->Getmui_ActiveTagMap()->TagMap);
					}
					else
					{
						newmapper->SetLookupTable(this->Getmui_ActiveColorMap()->ColorMap);
					}

					newmapper->ScalarVisibilityOn();

					cout << "densification subdivisions=" << subdivisions << endl;
					vtkSmartPointer<vtkCleanPolyData> cleanPolyDataFilter = vtkSmartPointer<vtkCleanPolyData>::New();
					if (method == 0) // classic densify polydata
					{
						vtkSmartPointer<vtkDensifyPolyData> densify =
							vtkSmartPointer<vtkDensifyPolyData>::New();
						densify->SetInputData(vtkPolyData::SafeDownCast(mymapper->GetInput()));
						densify->SetNumberOfSubdivisions(subdivisions);
						densify->Update();
						cleanPolyDataFilter->SetInputData(densify->GetOutput());


					}
					else if (method == 1)// linear subdivision filter
					{
						vtkSmartPointer<vtkTriangleFilter> triangles =
							vtkSmartPointer<vtkTriangleFilter>::New();
						triangles->SetInputData(vtkPolyData::SafeDownCast(mymapper->GetInput()));
						triangles->Update();
						vtkSmartPointer<vtkFeatureEdges> featureEdges = vtkSmartPointer<vtkFeatureEdges>::New();
						featureEdges->SetInputData(triangles->GetOutput());
						featureEdges->BoundaryEdgesOff();
						featureEdges->FeatureEdgesOff();
						featureEdges->ManifoldEdgesOff();
						featureEdges->NonManifoldEdgesOn();
						featureEdges->Update();
						cout << "Feature edges: nr of points in non manyfold edges" << featureEdges->GetOutput()->GetNumberOfPoints() << endl;

						cout << "Triangles numpoints" << triangles->GetOutput()->GetNumberOfPoints() << endl;
						cout << "Triangles numcells" << triangles->GetOutput()->GetNumberOfCells() << endl;
						vtkSmartPointer<vtkSphereSource> sphereSource =
							vtkSmartPointer<vtkSphereSource>::New();
						sphereSource->Update();

						vtkSmartPointer<vtkLinearSubdivisionFilter> linear =
							vtkSmartPointer<vtkLinearSubdivisionFilter>::New();
						linear->SetInputData(triangles->GetOutput());
						//linear->SetInputData(sphereSource->GetOutput());
						linear->SetNumberOfSubdivisions(subdivisions);
						linear->SetCheckForTriangles(false);
						linear->Update();
						cout << "Linear numpoints" << linear->GetOutput()->GetNumberOfPoints() << endl;
						cout << "Linear numcells" << linear->GetOutput()->GetNumberOfCells() << endl;
						if (linear->GetOutput()->GetNumberOfPoints() > 0)
						{
							cleanPolyDataFilter->SetInputData(linear->GetOutput());
						}
						else
						{
							cleanPolyDataFilter->SetInputData(featureEdges->GetOutput());
						}


					}
					else if (method == 2)
					{
						vtkSmartPointer<vtkSphereSource> sphereSource =
							vtkSmartPointer<vtkSphereSource>::New();
						sphereSource->Update();



						vtkSmartPointer<vtkLoopSubdivisionFilter> loop =
							vtkSmartPointer<vtkLoopSubdivisionFilter>::New();
						loop->SetInputData(vtkPolyData::SafeDownCast(mymapper->GetInput()));
						//loop->SetInputData(sphereSource->GetOutput()); 
						loop->SetNumberOfSubdivisions(subdivisions);
						loop->Update();
						cout << "Loop numpoints" << loop->GetOutput()->GetNumberOfPoints() << endl;
						cout << "Loop numcells" << loop->GetOutput()->GetNumberOfCells() << endl;

						cleanPolyDataFilter->SetInputData(loop->GetOutput());
					}
					else
					{
						vtkSmartPointer<vtkSphereSource> sphereSource =
							vtkSmartPointer<vtkSphereSource>::New();
						sphereSource->Update();


						vtkSmartPointer<vtkButterflySubdivisionFilter> butterfly =
							vtkSmartPointer<vtkButterflySubdivisionFilter>::New();
						butterfly->SetInputData(vtkPolyData::SafeDownCast(mymapper->GetInput()));
						//butterfly->SetInputData(sphereSource->GetOutput());

						butterfly->SetNumberOfSubdivisions(subdivisions);
						butterfly->Update();
						cout << "butterfly numpoints" << butterfly->GetOutput()->GetNumberOfPoints() << endl;
						cout << "butterfly numcells" << butterfly->GetOutput()->GetNumberOfCells() << endl;
						cleanPolyDataFilter->SetInputData(butterfly->GetOutput());
					}



						//cleanPolyDataFilter->SetInputData(ObjNormals->GetOutput());

					cleanPolyDataFilter->PieceInvariantOff();
					cleanPolyDataFilter->ConvertLinesToPointsOff();
					cleanPolyDataFilter->ConvertPolysToLinesOff();
					cleanPolyDataFilter->ConvertStripsToPolysOff();
					cleanPolyDataFilter->PointMergingOn();
					cleanPolyDataFilter->Update();
					VTK_CREATE(vtkPolyData, myData);

					myData = cleanPolyDataFilter->GetOutput();

					cout << "densify: nv=" << myData->GetNumberOfPoints() << endl;
					//newmapper->SetInputConnection(delaunay3D->GetOutputPort());
					newmapper->SetInputData(myData);

					vtkSmartPointer<vtkMatrix4x4> Mat = vtkSmartPointer<vtkMatrix4x4>::New();
					Mat = myActor->GetMatrix();

					vtkTransform *newTransform = vtkTransform::New();
					newTransform->PostMultiply();

					newTransform->SetMatrix(Mat);
					newactor->SetPosition(newTransform->GetPosition());
					newactor->SetScale(newTransform->GetScale());
					newactor->SetOrientation(newTransform->GetOrientation());
					newTransform->Delete();


					double color[4] = { 0.5, 0.5, 0.5, 1 };
					myActor->GetmColor(color);
					newactor->SetmColor(color);

					newactor->SetMapper(newmapper);
					newactor->SetSelected(0);


					newactor->SetName(myActor->GetName() + "_densify");
					cout << "try to add new actor=" << endl;
					newcoll->AddTmpItem(newactor);
					modified = 1;
				}//ok to add

			}
		}
	}
	if (msg_nonmanifold==1)
	{ 
		QMessageBox msgBox;
		msgBox.setText("At least one surface contains non-manifold edges and could not be densified using the chosen method.");
		msgBox.exec();
	}
	if (modified == 1)
	{
		newcoll->InitTraversal();
		vtkIdType num = newcoll->GetNumberOfItems();
		for (vtkIdType i = 0; i < num; i++)
		{
			cout << "try to get next actor from newcoll:" << i << endl;
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(newcoll->GetNextActor());
			myActor->SetColorProperties(this->mui_Ambient, this->mui_Diffuse, this->mui_Specular, this->mui_SpecularPower);
			myActor->SetDisplayMode(this->mui_DisplayMode);
			this->getActorCollection()->AddItem(myActor);
			emit this->actorsMightHaveChanged();
			std::string action = "Densified object added: " + myActor->GetName();
			int mCount = BEGIN_UNDO_SET(action);
			this->getActorCollection()->CreateLoadUndoSet(mCount, 1);
			END_UNDO_SET();


		}
		//cout << "camera and grid adjusted" << endl;
		cout << "new actor(s) added" << endl;
		this->Initmui_ExistingArrays();

		cout << "Set actor collection changed" << endl;
		this->getActorCollection()->SetChanged(1);
		cout << "Actor collection changed" << endl;

		this->AdjustCameraAndGrid();
		cout << "Camera and grid adjusted" << endl;

		if (this->Getmui_AdjustLandmarkRenderingSize() == 1)
		{
			this->UpdateLandmarkSettings();
		}
		this->Render();
	}

}
void  mqMorphoDigCore::addDecimate(int quadric, double factor)
{
	vtkSmartPointer<vtkMDActorCollection> newcoll = vtkSmartPointer<vtkMDActorCollection>::New();
	this->ActorCollection->InitTraversal();
	vtkIdType num = this->ActorCollection->GetNumberOfItems();
	int modified = 0;
	for (vtkIdType i = 0; i < num; i++)
	{
		cout << "Decimate try to get next actor:" << i << endl;
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
			if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
			{
				double numvert = mymapper->GetInput()->GetNumberOfPoints();

			

				VTK_CREATE(vtkMDActor, newactor);
				if (this->mui_BackfaceCulling == 0)
				{
					newactor->GetProperty()->BackfaceCullingOff();
				}
				else
				{
					newactor->GetProperty()->BackfaceCullingOn();
				}

				VTK_CREATE(vtkPolyDataMapper, newmapper);
				//newmapper->ImmediateModeRenderingOn();
				newmapper->SetColorModeToDefault();

				if (
					(this->mui_ActiveArray->DataType == VTK_INT || this->mui_ActiveArray->DataType == VTK_UNSIGNED_INT)
					&& this->mui_ActiveArray->NumComp == 1
					)
				{
					newmapper->SetScalarRange(0, this->Getmui_ActiveTagMap()->numTags - 1);
					newmapper->SetLookupTable(this->Getmui_ActiveTagMap()->TagMap);
				}
				else
				{
					newmapper->SetLookupTable(this->Getmui_ActiveColorMap()->ColorMap);
				}

				newmapper->ScalarVisibilityOn();
				double mfactor = (100 - factor)/100;
				if (mfactor == 0 || mfactor == 1) { mfactor = 0.9; }
				mfactor = 1 - mfactor;
				cout << "decimation factor=" << mfactor << endl;
				vtkSmartPointer<vtkDecimatePro> decimate =
					vtkSmartPointer<vtkDecimatePro>::New();
				decimate->SetInputData(vtkPolyData::SafeDownCast(mymapper->GetInput()));
				decimate->SetTargetReduction(mfactor);

				vtkSmartPointer<vtkQuadricDecimation> decimate2 =
					vtkSmartPointer<vtkQuadricDecimation>::New();
				decimate2->SetInputData(vtkPolyData::SafeDownCast(mymapper->GetInput()));
				decimate2->SetTargetReduction(mfactor);

				if (quadric == 0)
				{
					decimate->Update();
				}
				else
				{
					decimate2->Update();
				}
			
				vtkSmartPointer<vtkCleanPolyData> cleanPolyDataFilter = vtkSmartPointer<vtkCleanPolyData>::New();
				//cleanPolyDataFilter->SetInputData(ObjNormals->GetOutput());
				if (quadric == 0)
				{
					cleanPolyDataFilter->SetInputData(decimate->GetOutput());
					
				}
				else
				{
					cleanPolyDataFilter->SetInputData(decimate2->GetOutput());
					
				}
				cleanPolyDataFilter->PieceInvariantOff();
				cleanPolyDataFilter->ConvertLinesToPointsOff();
				cleanPolyDataFilter->ConvertPolysToLinesOff();
				cleanPolyDataFilter->ConvertStripsToPolysOff();
				cleanPolyDataFilter->PointMergingOn();
				cleanPolyDataFilter->Update();
				VTK_CREATE(vtkPolyData, myData);

				myData = cleanPolyDataFilter->GetOutput();

				cout << "decimate: nv=" << myData->GetNumberOfPoints() << endl;
				//newmapper->SetInputConnection(delaunay3D->GetOutputPort());





				newmapper->SetInputData(myData);
				



				vtkSmartPointer<vtkMatrix4x4> Mat = vtkSmartPointer<vtkMatrix4x4>::New();
				Mat = myActor->GetMatrix();



				vtkTransform *newTransform = vtkTransform::New();
				newTransform->PostMultiply();

				newTransform->SetMatrix(Mat);
				newactor->SetPosition(newTransform->GetPosition());
				newactor->SetScale(newTransform->GetScale());
				newactor->SetOrientation(newTransform->GetOrientation());
				newTransform->Delete();


				double color[4] = { 0.5, 0.5, 0.5, 1 };
				myActor->GetmColor(color);
				newactor->SetmColor(color);

				newactor->SetMapper(newmapper);
				newactor->SetSelected(0);

				
				newactor->SetName(myActor->GetName() + "_decimate");
				cout << "try to add new actor=" << endl;
				newcoll->AddTmpItem(newactor);
				modified = 1;


			}
		}
	}
	if (modified == 1)
	{
		newcoll->InitTraversal();
		vtkIdType num = newcoll->GetNumberOfItems();
		for (vtkIdType i = 0; i < num; i++)
		{
			cout << "try to get next actor from newcoll:" << i << endl;
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(newcoll->GetNextActor());
			myActor->SetColorProperties(this->mui_Ambient, this->mui_Diffuse, this->mui_Specular, this->mui_SpecularPower);
			myActor->SetDisplayMode(this->mui_DisplayMode);
			this->getActorCollection()->AddItem(myActor);
			emit this->actorsMightHaveChanged();
			std::string action = "Decimated object added: " + myActor->GetName();
			int mCount = BEGIN_UNDO_SET(action);
			this->getActorCollection()->CreateLoadUndoSet(mCount, 1);
			END_UNDO_SET();


		}
		//cout << "camera and grid adjusted" << endl;
		cout << "new actor(s) added" << endl;
		this->Initmui_ExistingArrays();

		cout << "Set actor collection changed" << endl;
		this->getActorCollection()->SetChanged(1);
		cout << "Actor collection changed" << endl;

		this->AdjustCameraAndGrid();
		cout << "Camera and grid adjusted" << endl;

		if (this->Getmui_AdjustLandmarkRenderingSize() == 1)
		{
			this->UpdateLandmarkSettings();
		}
		this->Render();
	}
}
void  mqMorphoDigCore::addSmooth(int iteration, double relaxation)
{

	vtkSmartPointer<vtkMDActorCollection> newcoll = vtkSmartPointer<vtkMDActorCollection>::New();
	this->ActorCollection->InitTraversal();
	vtkIdType num = this->ActorCollection->GetNumberOfItems();
	int modified = 0;
	for (vtkIdType i = 0; i < num; i++)
	{
		cout << "Smooth try to get next actor:" << i << endl;
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			//myActor->SetSelected(0);

			vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
			if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
			{


				double numvert = mymapper->GetInput()->GetNumberOfPoints();

			
				VTK_CREATE(vtkMDActor, newactor);
				if (this->mui_BackfaceCulling == 0)
				{
					newactor->GetProperty()->BackfaceCullingOff();
				}
				else
				{
					newactor->GetProperty()->BackfaceCullingOn();
				}

				VTK_CREATE(vtkPolyDataMapper, newmapper);
				//newmapper->ImmediateModeRenderingOn();
				newmapper->SetColorModeToDefault();

				if (
					(this->mui_ActiveArray->DataType == VTK_INT || this->mui_ActiveArray->DataType == VTK_UNSIGNED_INT)
					&& this->mui_ActiveArray->NumComp == 1
					)
				{
					newmapper->SetScalarRange(0, this->Getmui_ActiveTagMap()->numTags - 1);
					newmapper->SetLookupTable(this->Getmui_ActiveTagMap()->TagMap);
				}
				else
				{
					newmapper->SetLookupTable(this->Getmui_ActiveColorMap()->ColorMap);
				}

				newmapper->ScalarVisibilityOn();

				vtkSmartPointer<vtkSmoothPolyDataFilter> Sfilter = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
				Sfilter->SetInputData(vtkPolyData::SafeDownCast(mymapper->GetInput()));
				Sfilter->SetNumberOfIterations(iteration);
				Sfilter->SetRelaxationFactor(relaxation);
				Sfilter->Update();
			

			

				vtkSmartPointer<vtkCleanPolyData> cleanPolyDataFilter = vtkSmartPointer<vtkCleanPolyData>::New();
				//cleanPolyDataFilter->SetInputData(ObjNormals->GetOutput());
				cleanPolyDataFilter->SetInputData(Sfilter->GetOutput());
				cleanPolyDataFilter->PieceInvariantOff();
				cleanPolyDataFilter->ConvertLinesToPointsOff();
				cleanPolyDataFilter->ConvertPolysToLinesOff();
				cleanPolyDataFilter->ConvertStripsToPolysOff();
				cleanPolyDataFilter->PointMergingOn();
				cleanPolyDataFilter->Update();
				VTK_CREATE(vtkPolyData, myData);

				myData = cleanPolyDataFilter->GetOutput();

				cout << "smooth: nv=" << myData->GetNumberOfPoints() << endl;
				//newmapper->SetInputConnection(delaunay3D->GetOutputPort());


			

			
				newmapper->SetInputData(myData);




				vtkSmartPointer<vtkMatrix4x4> Mat = vtkSmartPointer<vtkMatrix4x4>::New();
				Mat=myActor->GetMatrix();

				

				vtkTransform *newTransform = vtkTransform::New();
				newTransform->PostMultiply();

				newTransform->SetMatrix(Mat);
				newactor->SetPosition(newTransform->GetPosition());
				newactor->SetScale(newTransform->GetScale());
				newactor->SetOrientation(newTransform->GetOrientation());
				newTransform->Delete();


				double color[4] = { 0.5, 0.5, 0.5, 1 };
				myActor->GetmColor(color);
				newactor->SetmColor(color);

				newactor->SetMapper(newmapper);
				newactor->SetSelected(0);


				newactor->SetName(myActor->GetName() + "_smooth");
				cout << "try to add new actor=" << endl;
				newcoll->AddTmpItem(newactor);
				modified = 1;


			}
		}
	}
	if (modified == 1)
	{
		newcoll->InitTraversal();
		vtkIdType num = newcoll->GetNumberOfItems();
		for (vtkIdType i = 0; i < num; i++)
		{
			cout << "try to get next actor from newcoll:" << i << endl;
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(newcoll->GetNextActor());
			myActor->SetColorProperties(this->mui_Ambient, this->mui_Diffuse, this->mui_Specular, this->mui_SpecularPower);
			myActor->SetDisplayMode(this->mui_DisplayMode);
			this->getActorCollection()->AddItem(myActor);
			emit this->actorsMightHaveChanged();
			std::string action = "Smoothed object added: " + myActor->GetName();
			int mCount = BEGIN_UNDO_SET(action);
			this->getActorCollection()->CreateLoadUndoSet(mCount, 1);
			END_UNDO_SET();


		}
		//cout << "camera and grid adjusted" << endl;
		cout << "new actor(s) added" << endl;
		this->Initmui_ExistingArrays();

		cout << "Set actor collection changed" << endl;
		this->getActorCollection()->SetChanged(1);
		cout << "Actor collection changed" << endl;

		this->AdjustCameraAndGrid();
		cout << "Camera and grid adjusted" << endl;

		if (this->Getmui_AdjustLandmarkRenderingSize() == 1)
		{
			this->UpdateLandmarkSettings();
		}
		this->Render();
	}

}
void mqMorphoDigCore::addDecompose(int color_mode, int min_region_size)
{
	//color_mode 0 : same color as selected object
	//color_mode 1 : random color (different each time)
	//color_mode 2 : use tag lut (to be implemented!)
	vtkSmartPointer<vtkMDActorCollection> newcoll = vtkSmartPointer<vtkMDActorCollection>::New();
	this->ActorCollection->InitTraversal();
	vtkIdType num = this->ActorCollection->GetNumberOfItems();
	int modified = 0;
	int numTasks = 100000;
	//QString inprogress = QString("none");
	
	for (vtkIdType i = 0; i < num; i++)
	{
		cout << "Largest region of next actor:" << i << endl;
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			//myActor->SetSelected(0);

			vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
			if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
			{


			

				vtkSmartPointer<vtkPolyDataConnectivityFilter> cfilter = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
				cfilter->SetInputData(mymapper->GetInput());
				cfilter->SetExtractionModeToAllRegions();
				cfilter->ColorRegionsOn();
				cfilter->Update();
				int regions = cfilter->GetNumberOfExtractedRegions();
				QProgressDialog progress("Surface decomposition into non connex subregions.", "Abort decomposition", 0, regions);
				progress.setCancelButton(0);
				progress.setWindowModality(Qt::WindowModal);
				std::cout << "\nVtkConnectivity number of regions:" << regions << std::endl;
				vtkSmartPointer<vtkIdTypeArray> region_sizes = vtkSmartPointer<vtkIdTypeArray>::New();
				region_sizes = cfilter->GetRegionSizes();

				vtkSmartPointer<vtkFloatArray> newScalars =
					vtkSmartPointer<vtkFloatArray>::New();

				newScalars->SetNumberOfComponents(1); //3d normals (ie x,y,z)
				newScalars->SetNumberOfTuples(mymapper->GetInput()->GetNumberOfPoints());

				vtkSmartPointer<vtkIdTypeArray> currentRegions = vtkSmartPointer<vtkIdTypeArray>::New();

				//my_data->GetNu
				currentRegions = vtkIdTypeArray::SafeDownCast(cfilter->GetOutput()->GetPointData()->GetArray("RegionId"));

				//std::cout<<"vertex"<<i<<", current region:"<<currentRegions->GetTuple(i)<<std::endl;

				for (vtkIdType i = 0; i<mymapper->GetInput()->GetNumberOfPoints(); i++)	// for each vertex 
				{

					//std::cout<<"vertex"<<i<<", current region:"<<currentRegions->GetTuple(i)[0]<<std::endl;
					newScalars->InsertTuple1(i, (double)currentRegions->GetTuple(i)[0]);


				}
				newScalars->SetName("TMP");
				vtkSmartPointer<vtkPolyData> MyObj = vtkSmartPointer<vtkPolyData>::New();
				MyObj = cfilter->GetOutput();


				MyObj->GetPointData()->RemoveArray("RegionId");
				MyObj->GetPointData()->RemoveArray("TMP");
				MyObj->GetPointData()->AddArray(newScalars);
				//MyObj->GetPointData()->SetActiveScalars("TMP");


				int cpt = 0;
				for (vtkIdType j = 0; j<region_sizes->GetNumberOfTuples(); j++)
				{
										
					progress.setValue(j);
					if (progress.wasCanceled())
						break;

					
					
					if (region_sizes->GetTuple((vtkIdType)j)[0] >= (vtkIdType)min_region_size)
					{
						VTK_CREATE(vtkMDActor, newactor);
						if (this->mui_BackfaceCulling == 0)
						{
							newactor->GetProperty()->BackfaceCullingOff();
						}
						else
						{
							newactor->GetProperty()->BackfaceCullingOn();
						}

						VTK_CREATE(vtkPolyDataMapper, newmapper);
						//newmapper->ImmediateModeRenderingOn();
						newmapper->SetColorModeToDefault();

						if (
							(this->mui_ActiveArray->DataType == VTK_INT || this->mui_ActiveArray->DataType == VTK_UNSIGNED_INT)
							&& this->mui_ActiveArray->NumComp == 1
							)
						{
							newmapper->SetScalarRange(0, this->Getmui_ActiveTagMap()->numTags - 1);
							newmapper->SetLookupTable(this->Getmui_ActiveTagMap()->TagMap);
						}
						else
						{
							newmapper->SetLookupTable(this->Getmui_ActiveColorMap()->ColorMap);
						}

						newmapper->ScalarVisibilityOn();

						vtkSmartPointer<vtkThreshold> selector =
							vtkSmartPointer<vtkThreshold>::New();
						vtkSmartPointer<vtkMaskFields> scalarsOff =
							vtkSmartPointer<vtkMaskFields>::New();
						vtkSmartPointer<vtkGeometryFilter> geometry =
							vtkSmartPointer<vtkGeometryFilter>::New();

						selector->SetInputData(cfilter->GetOutput());

						selector->SetInputArrayToProcess(0, 0, 0,
							vtkDataObject::FIELD_ASSOCIATION_CELLS,
							vtkDataSetAttributes::SCALARS);
						selector->SetAllScalars(1);
						selector->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "TMP");
						selector->ThresholdBetween((double)(j), (double)(j));
						selector->Update();

						scalarsOff->SetInputData(selector->GetOutput());
						scalarsOff->CopyAttributeOff(vtkMaskFields::POINT_DATA, vtkDataSetAttributes::SCALARS);
						scalarsOff->CopyAttributeOff(vtkMaskFields::CELL_DATA, vtkDataSetAttributes::SCALARS);
						scalarsOff->Update();
						geometry->SetInputData(scalarsOff->GetOutput());
						geometry->Update();

						vtkSmartPointer<vtkPolyData> MyObj2 = vtkSmartPointer<vtkPolyData>::New();
						MyObj2 = geometry->GetOutput();

						
						cpt++;


						newmapper->SetInputData(MyObj2);

						vtkSmartPointer<vtkMatrix4x4> Mat = myActor->GetMatrix();


						vtkTransform *newTransform = vtkTransform::New();
						newTransform->PostMultiply();

						newTransform->SetMatrix(Mat);

						newactor->SetPosition(newTransform->GetPosition());
						newactor->SetScale(newTransform->GetScale());
						newactor->SetOrientation(newTransform->GetOrientation());
						newTransform->Delete();


						double color[4] = { 0.5, 0.5, 0.5, 1 };
						if (color_mode == 0)
						{
							myActor->GetmColor(color);
						}
						else
						{
							color[0] = rand() / (RAND_MAX + 1.);
							color[1] = rand() / (RAND_MAX + 1.);
							color[2] = rand() / (RAND_MAX + 1.);

						}
						newactor->SetmColor(color);

						newactor->SetMapper(newmapper);
						newactor->SetSelected(0);


						newactor->SetName(myActor->GetName() + "_" + std::to_string(cpt));
						//cout << "try to add new actor=" << endl;
						newcoll->AddTmpItem(newactor);
						modified = 1;
					

					} // if size big enough

				}// for all regions!

				

				

				



				
				progress.setValue(regions);

			}
			
		}
		
	}
	
	if (modified == 1)
	{
		newcoll->InitTraversal();
		vtkIdType num = newcoll->GetNumberOfItems();
		for (vtkIdType i = 0; i < num; i++)
		{
			cout << "try to get next actor from newcoll:" << i << endl;
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(newcoll->GetNextActor());
			myActor->SetColorProperties(this->mui_Ambient, this->mui_Diffuse, this->mui_Specular, this->mui_SpecularPower);
			myActor->SetDisplayMode(this->mui_DisplayMode);
			this->getActorCollection()->AddItem(myActor);
			emit this->actorsMightHaveChanged();
			std::string action = "Largest region object added: " + myActor->GetName();
			int mCount = BEGIN_UNDO_SET(action);
			this->getActorCollection()->CreateLoadUndoSet(mCount, 1);
			END_UNDO_SET();


		}
		//cout << "camera and grid adjusted" << endl;
		cout << "new actor(s) added" << endl;
		this->Initmui_ExistingArrays();

		cout << "Set actor collection changed" << endl;
		this->getActorCollection()->SetChanged(1);
		cout << "Actor collection changed" << endl;

		this->AdjustCameraAndGrid();
		cout << "Camera and grid adjusted" << endl;

		if (this->Getmui_AdjustLandmarkRenderingSize() == 1)
		{
			this->UpdateLandmarkSettings();
		}
		this->Render();
	}



}
void mqMorphoDigCore::GetVertexColor(vtkMDActor *myActor, vtkIdType ve, int color[4])
{
	
	color[0] = 1;
	color[1] = 0;
	color[2] = 0;
	color[3] = 1;
	if (myActor != NULL)
	{
		vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
		if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
		{

			vtkSmartPointer<vtkPolyData> mPD = vtkSmartPointer<vtkPolyData>::New();
			mPD = mymapper->GetInput();

			double numvert = mPD->GetNumberOfPoints();


			vtkSmartPointer<vtkUnsignedCharArray> newcolors =
				vtkSmartPointer<vtkUnsignedCharArray>::New();
			newcolors->SetNumberOfComponents(4);
			newcolors->SetNumberOfTuples(mymapper->GetInput()->GetNumberOfPoints());

			vtkIdType iRGB = 0;
			int nr, ng, nb, na;

			QString sActiveScalar = this->Getmui_ActiveArray()->Name;
			QString none = QString("Solid color");
			QString RGB = QString("RGB");

			// we define wheter we have to create RGB from scalars/RGB/tags or from "global" color option.
			int RGB_fromglobal = 1;
			// if current active scalar is not the "none" one, and if the actor as current active scalar.
			//if (ActiveScalar != none && 	mergedObjects->GetOutput()->GetPointData()->GetScalars(ActiveScalar.toStdString().c_str()) != NULL)
			if (sActiveScalar != none && 	mymapper->GetInput()->GetPointData()->GetScalars(sActiveScalar.toStdString().c_str()) != NULL)
			{
				RGB_fromglobal = 0;
			}

			vtkUnsignedCharArray *currentRGBcolors = vtkUnsignedCharArray::SafeDownCast(mymapper->GetInput()->GetPointData()->GetScalars(sActiveScalar.toStdString().c_str()));
			vtkFloatArray *currentFScalars = vtkFloatArray::SafeDownCast(mymapper->GetInput()->GetPointData()->GetScalars(sActiveScalar.toStdString().c_str()));
			vtkDoubleArray *currentDScalars = vtkDoubleArray::SafeDownCast(mymapper->GetInput()->GetPointData()->GetScalars(sActiveScalar.toStdString().c_str()));
			vtkIntArray *currentTags = vtkIntArray::SafeDownCast(mymapper->GetInput()->GetPointData()->GetScalars(sActiveScalar.toStdString().c_str()));

		

	
			nr = (unsigned char)(255 * myActor->GetmColor()[0]);
			ng = (unsigned char)(255 * myActor->GetmColor()[1]);
			nb = (unsigned char)(255 * myActor->GetmColor()[2]);
			na = (unsigned char)(255 * myActor->GetmColor()[3]);

			//1st case  : the current actor has not the currently active scalar.
			if (RGB_fromglobal == 1)
			{
				//do nothing here as nr ng nb na have already been correctly instantiated just above

			}
			else
			{
				// 3 cases : 
				//      A: active scalar = RGB => so we retake RGB... as we have started to do so earlier!
				if (this->mui_ActiveArray->DataType == VTK_UNSIGNED_CHAR
					&&  this->mui_ActiveArray->NumComp >= 3)
				{
					if (currentRGBcolors != NULL)
					{
						nr = (unsigned char)(currentRGBcolors->GetTuple(ve)[0]);
						ng = (unsigned char)(currentRGBcolors->GetTuple(ve)[1]);
						nb = (unsigned char)(currentRGBcolors->GetTuple(ve)[2]);
						if (this->mui_ActiveArray->NumComp == 4)
						{
							na = (unsigned char)(currentRGBcolors->GetTuple(ve)[3]);
						}
						else
						{
							na = 1;
						}
					}
					// else we keep global RGB as instantiated above.

				}
				else
				{
					//      B: active scalar = 1 scalar or tag => we translate scalar or tag as RGB
					if ((this->Getmui_ActiveArray()->DataType == VTK_FLOAT ||
						this->Getmui_ActiveArray()->DataType == VTK_DOUBLE ||
						this->Getmui_ActiveArray()->DataType == VTK_INT ||
						this->Getmui_ActiveArray()->DataType == VTK_UNSIGNED_INT


						)
						&& this->Getmui_ActiveArray()->NumComp == 1)
					{

						double cscalar = 0;
						if (this->Getmui_ActiveArray()->DataType == VTK_INT ||
							this->Getmui_ActiveArray()->DataType == VTK_UNSIGNED_INT)
						{
							if (currentTags != NULL)
							{
								cscalar = (double)currentTags->GetTuple(ve)[0];
							}

						}
						else
						{
							if (currentFScalars != NULL)
							{
								cscalar = (double)currentFScalars->GetTuple(ve)[0];
							}
							if (currentDScalars != NULL)
							{
								cscalar = currentDScalars->GetTuple(ve)[0];
							}
						}

						// retrieve scalar value

						double cRGB[3];
						double cOpac = 1;
						mymapper->GetLookupTable()->GetColor(cscalar, cRGB);

						nr = (unsigned char)(255 * cRGB[0]);
						ng = (unsigned char)(255 * cRGB[1]);
						nb = (unsigned char)(255 * cRGB[2]);
						na = (unsigned char)(255 * mymapper->GetLookupTable()->GetOpacity(cscalar));
						if (ve<10)
						{
							cout << "ve=" << ve << ", cRGB[0]=" << cRGB[0] << endl;
						}

						// translate it as RGB


					}
					// else we keep global RGB as instantiated above.

				}



			}


			color[0] = nr;
			color[1] = ng;
			color[2] = nb;
			color[3] = na;
		}
	}
	

}
void mqMorphoDigCore::scalarsRGB(QString newRGB)
{

	std::string mScalarName = "RGB";
	if (newRGB.length() > 0)
	{
		mScalarName = newRGB.toStdString();
	}
	vtkIdType num_selected_meshes = this->getActorCollection()->GetNumberOfSelectedActors();
	if (num_selected_meshes == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}
	vtkIdType num2 = this->ActorCollection->GetNumberOfSelectedActors();
	
	vtkIdType num = this->ActorCollection->GetNumberOfItems();
	
	int modified = 0;
	std::string action = "Compute RGB scalar";
	int Count = BEGIN_UNDO_SET(action);
	cout << "Compute RGB scalar for num=" << num<< ", and num selected = "<<num2<<endl;
	this->ActorCollection->InitTraversal();
	for (vtkIdType i = 0; i < num; i++)
	{
		cout << "RGB distance" << i << endl;
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			cout << "Set selected 0 for actor  " << i << endl;

			myActor->SetSelected(0);
			myActor->SaveState(Count, QString(mScalarName.c_str()));
			
			vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
			if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
			{

				vtkSmartPointer<vtkPolyData> mPD = vtkSmartPointer<vtkPolyData>::New();
				mPD = mymapper->GetInput();

				double numvert =mPD->GetNumberOfPoints();


				vtkSmartPointer<vtkUnsignedCharArray> newcolors =
					vtkSmartPointer<vtkUnsignedCharArray>::New();
				newcolors->SetNumberOfComponents(4);
				newcolors->SetNumberOfTuples(mymapper->GetInput()->GetNumberOfPoints());

					
					for (vtkIdType j = 0; j < mPD->GetNumberOfPoints(); j++)
					{	
						int col[4];
						this->GetVertexColor(myActor, j, col);
						newcolors->InsertTuple4(j, col[0], col[1], col[2], col[3]);						
				}
													


				newcolors->SetName(mScalarName.c_str());
				// test if exists...
				//
				int exists = 0;

				// remove this scalar
				//this->GetPointData()->SetScalars(newScalars);
				mPD->GetPointData()->RemoveArray(mScalarName.c_str());
				mPD->GetPointData()->AddArray(newcolors);
				mPD->GetPointData()->SetActiveScalars(mScalarName.c_str());
				//g_active_scalar = 0;
				// 0 => depth
				// 1 =>	"Maximum_Curvature"
				// 2 => "Minimum_Curvature"
				// 3 => "Gauss_Curvature"
				// 4 => "Mean_Curvature"
				cout << "actor " << i << "is modified" << endl;
				modified = 1;

			}

		}
	}
	if (modified == 1)
	{

		//cout << "camera and grid adjusted" << endl;
		cout << "RGB scalars created" << endl;

		this->Initmui_ExistingArrays();
		this->Setmui_ActiveArrayAndRender(mScalarName.c_str(), VTK_UNSIGNED_CHAR, 4);


	}
	END_UNDO_SET();


}
void mqMorphoDigCore::scalarsCurvature(int curvatureType, QString scalarName)
{
	std::string mScalarName = "Curvature";
	if (scalarName.length() > 0)
	{
		mScalarName = scalarName.toStdString();
	}

	this->ActorCollection->InitTraversal();
	vtkIdType num = this->ActorCollection->GetNumberOfItems();
	int modified = 0;
	std::string action = "Compute curvature scalar";
	int Count = BEGIN_UNDO_SET(action);
	for (vtkIdType i = 0; i < num; i++)
	{
		cout << "Curvature " << i << endl;
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{

			myActor->SetSelected(0);
			myActor->SaveState(Count, QString(mScalarName.c_str()));
			vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
			if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
			{
				vtkSmartPointer<vtkPolyData> mPD = vtkSmartPointer<vtkPolyData>::New();
				mPD = mymapper->GetInput();

				double numvert =mPD->GetNumberOfPoints();

				vtkSmartPointer<vtkCurvatures> curvaturesFilter =
					vtkSmartPointer<vtkCurvatures>::New();
				curvaturesFilter->SetCurvatureType(curvatureType);


				curvaturesFilter->SetInputData(mPD);
				curvaturesFilter->Update();
				vtkSmartPointer<vtkPolyData> MyObj = vtkSmartPointer<vtkPolyData>::New();
				MyObj = curvaturesFilter->GetOutput();

				//cout << "curv   c" << endl;
				// Get active scalars of output
				vtkFloatArray *freshScalars;
				freshScalars = (vtkFloatArray*)MyObj->GetPointData()->GetScalars();


				vtkSmartPointer<vtkFloatArray> newScalars =
					vtkSmartPointer<vtkFloatArray>::New();

				newScalars->DeepCopy(freshScalars);



				newScalars->SetName(mScalarName.c_str());
				// test if exists...
				//
				int exists = 0;

				// remove this scalar
				//this->GetPointData()->SetScalars(newScalars);
				mPD->GetPointData()->RemoveArray(mScalarName.c_str());
				mPD->GetPointData()->AddArray(newScalars);
				mPD->GetPointData()->SetActiveScalars(mScalarName.c_str());
				

				modified = 1;

			}

		}
	}
	if (modified == 1)
	{

		//cout << "camera and grid adjusted" << endl;
		cout << "scalars updated " << endl;

		this->Initmui_ExistingArrays();
		this->Setmui_ActiveArrayAndRender(mScalarName.c_str(), VTK_FLOAT, 1);


	}
	END_UNDO_SET();


}
void mqMorphoDigCore::scalarsNormalization(QString scalarName, double oldMin, double oldMax, double newMin, double newMax, int allowLowerThanNewMin, int allowHigherThanNewMax, int flipOutput)
{
	vtkIdType num_surf = 0;
	num_surf = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
	if (num_surf == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}
	if (oldMax==oldMin || newMax==newMin) {
		QMessageBox msgBox;
		msgBox.setText("min and max values should not be equal.");
		msgBox.exec();
		return;
	}
	if ((this->Getmui_ActiveArray()->DataType == VTK_FLOAT || this->Getmui_ActiveArray()->DataType == VTK_DOUBLE) && this->Getmui_ActiveArray()->NumComp == 1)
	{
		std::string mScalarName = "NormalizedScalar";


		this->ActorCollection->InitTraversal();
		vtkIdType num = this->ActorCollection->GetNumberOfItems();
		int modified = 0;
		std::string action = "Rescale or Normalize scalar";
		int Count = BEGIN_UNDO_SET(action);
		for (vtkIdType i = 0; i < num; i++)
		{
			cout << "Scalar smooth :" << i << endl;
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
			cout << "Got next actor:" << i << endl;
			if (myActor != NULL && myActor->GetSelected() == 1)
			{
				myActor->SetSelected(0);
				vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
				if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
				{
					cout << "found mapper for :" << i << endl;
					vtkSmartPointer<vtkPolyData> mPD = vtkSmartPointer<vtkPolyData>::New();
					mPD = mymapper->GetInput();

					vtkDataArray *currentScalars = NULL;


					currentScalars = vtkDoubleArray::SafeDownCast(mPD->GetPointData()->GetScalars(this->Getmui_ActiveArray()->Name.toStdString().c_str()));
					if (currentScalars == NULL)
					{
						currentScalars = vtkFloatArray::SafeDownCast(mPD->GetPointData()->GetScalars(this->Getmui_ActiveArray()->Name.toStdString().c_str()));
					}
					if (currentScalars != NULL)
					{
						cout << "found scalars for :" << i << endl;

						double numvert = mymapper->GetInput()->GetNumberOfPoints();


						vtkSmartPointer<vtkDoubleArray> newScalars =
							vtkSmartPointer<vtkDoubleArray>::New();

						newScalars->SetNumberOfComponents(1); //3d normals (ie x,y,z)
						newScalars->SetNumberOfTuples(numvert);
				
						std::string scname = scalarName.toStdString();


						if (scname.length() > 0)
						{
							mScalarName = scname;
						}
						myActor->SaveState(Count, QString(mScalarName.c_str()));
						
						/*for (vtkIdType j = 0; j < mPD->GetNumberOfPoints(); j++)
						{
							vals.push_back(currentScalars->GetTuple1(j));
						}*/




						
							vtkIdType ve;


							double vn[3];
							vn[0] = 0;
							vn[1] = 0;
							vn[2] = 1;
							double picked_value = 0;
							for (ve = 0; ve < numvert; ve++)
							{
							
								double newscalar = 0;													
								double curr_scalar = (double)(currentScalars->GetTuple(ve))[0];
								newscalar = newMin+(curr_scalar-oldMin)*(newMax -newMin )/(oldMax - oldMin);

								if (flipOutput == 1)
								{
									newscalar = -1*newscalar + newMin + newMax;
								}
								//std::cout<<curr_scalar<<std::endl;
								if (allowLowerThanNewMin ==0 && newscalar < newMin)
								{
									newscalar = newMin;
								}
								if (allowHigherThanNewMax == 0 && newscalar > newMax)
								{
									newscalar = newMax;
								}

									
																

								newScalars->InsertTuple1(ve, newscalar);


							}
							//std::cout<<"New Scalar computation done "<<std::endl;
							newScalars->SetName(mScalarName.c_str());
							mPD->GetPointData()->RemoveArray(mScalarName.c_str());				
							mPD->GetPointData()->AddArray(newScalars);				
							mPD->GetPointData()->SetActiveScalars(mScalarName.c_str());


						modified = 1;
					}//scalars not null

				}

			}

		}
		if (modified == 1)
		{
			this->Initmui_ExistingArrays();
			this->Setmui_ActiveArrayAndRender(mScalarName.c_str(), VTK_DOUBLE, 1);
			cout << "scalars updated " << endl;
			this->Render();
		}

		END_UNDO_SET();

	}
	else
	{
		QMessageBox msgBox;
		msgBox.setText("Active array must be an existing scalar...");
		msgBox.exec();
		return;
	}
}
void mqMorphoDigCore::scalarsCameraDistance()
{
		
	vtkIdType num_surf = 0;
	num_surf = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
	if (num_surf == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}
	this->ActorCollection->InitTraversal();
	vtkIdType num = this->ActorCollection->GetNumberOfItems();
	int modified = 0;
	std::string action = "Compute depth scalar";
	int Count = BEGIN_UNDO_SET(action);
	for (vtkIdType i = 0; i < num; i++)
	{
		cout << "Camera distance" << i << endl;
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			
			myActor->SetSelected(0);
			myActor->SaveState(Count, QString("Depth"));
			vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
			if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
			{
				

				double numvert = mymapper->GetInput()->GetNumberOfPoints();


				vtkSmartPointer<vtkDoubleArray> newScalars =
					vtkSmartPointer<vtkDoubleArray>::New();

				newScalars->SetNumberOfComponents(1); //3d normals (ie x,y,z)
				newScalars->SetNumberOfTuples(numvert);

				newScalars->SetNumberOfTuples(numvert);

			

				
				vtkSmartPointer<vtkPolyData> mPD = vtkSmartPointer<vtkPolyData>::New();
				mPD = mymapper->GetInput();
				double ve_init_pos[3];;
				double ve_final_pos[3];
				double distance_from_camera;
				vtkSmartPointer<vtkMatrix4x4> Mat = myActor->GetMatrix();
				distance_from_camera = 3;
				double campos[3];
				this->getCamera()->GetPosition(campos);

				for (vtkIdType i = 0; i < numvert; i++)	// for each vertex of this

				{									
						mPD->GetPoint(i, ve_init_pos);
						mqMorphoDigCore::TransformPoint(Mat, ve_init_pos, ve_final_pos);
											
						distance_from_camera = sqrt(vtkMath::Distance2BetweenPoints(campos, ve_final_pos));
				
					newScalars->InsertTuple1(i, distance_from_camera);


				}
				newScalars->SetName("Camera_distance");
				// test if exists...
				//
				int exists = 0;

				// remove this scalar
				//this->GetPointData()->SetScalars(newScalars);
				mPD->GetPointData()->RemoveArray("Depth");
				mPD->GetPointData()->AddArray(newScalars);
				mPD->GetPointData()->SetActiveScalars("Depth");
				//g_active_scalar = 0;
				// 0 => depth
				// 1 =>	"Maximum_Curvature"
				// 2 => "Minimum_Curvature"
				// 3 => "Gauss_Curvature"
				// 4 => "Mean_Curvature"

				modified = 1;

			}
			
		}
	}
	if (modified == 1)
	{
		
		//cout << "camera and grid adjusted" << endl;
		cout << "scalars updated " << endl;

		this->Initmui_ExistingArrays();
		this->Setmui_ActiveArrayAndRender("Depth", VTK_DOUBLE, 1);
		
		
	}
	END_UNDO_SET();



}
vtkSmartPointer<vtkIdList> mqMorphoDigCore::GetConnectedVertices(vtkSmartPointer<vtkPolyData> mesh, double *vn,
	double sc, vtkIdType id, int tool_mode, int compute_avg_norm)
{
	vtkSmartPointer<vtkIdList> connectedVertices =
		vtkSmartPointer<vtkIdList>::New();
	connectedVertices->InsertNextId(id);
	vtkSmartPointer<vtkIdList> cellIdList =
		vtkSmartPointer<vtkIdList>::New();
	mesh->GetPointCells(id, cellIdList);

	for (vtkIdType i = 0; i < cellIdList->GetNumberOfIds(); i++)
	{
		vtkSmartPointer<vtkIdList> pointIdList =
			vtkSmartPointer<vtkIdList>::New();
		mesh->GetCellPoints(cellIdList->GetId(i), pointIdList);

		vtkIdType newid;

		if (pointIdList->GetId(0) != id)
		{
			newid = pointIdList->GetId(0);
		}
		else
		{
			newid = pointIdList->GetId(1);

		}
		if (tool_mode == -1) //  no tool (-1) 
		{
			connectedVertices->InsertNextId(newid);
		}

		/*else if (tool_mode == 2 || (tool_mode == 1 && g_magic_wand_extension == 180) || (tool_mode == 0)) // flood bucket or magic wand with maximal extension or pencil
		{
			// get tag value of current id
			vtkFloatArray *currentTags;
			currentTags = (vtkFloatArray*)this->GetPointData()->GetScalars("Tags");
			if (currentTags == NULL)
			{
				connectedVertices->InsertNextId(newid);
			}
			else
			{
				double currentTag = currentTags->GetTuple(newid)[0];
				//std::cout<<"Current tag ="<<currentTag<<std::endl;
				//std::cout<<"sc ="<<sc<<std::endl;

				if (currentTag == sc || g_magic_wand_over == 1 || currentTag == 0.0) // Tag 0 is special! Overriden...
				{
					connectedVertices->InsertNextId(newid);
				}
			}
		}
		else // magic wand : consider both current tag value and normal angle limit extension
		{

			vtkFloatArray *currentTags;
			currentTags = (vtkFloatArray*)this->GetPointData()->GetScalars("Tags");
			int oktag = 0;
			if (g_magic_wand_over == 1)
			{
				oktag = 1;
			}
			if (currentTags == NULL)
			{
				oktag = 1;
			}
			else
			{
				double currentTag = currentTags->GetTuple(newid)[0];
				if (currentTag == sc || currentTag == 0.0) // Tag 0 is special! Overriden...
				{
					oktag = 1;
				}
			}
			if (oktag == 1)
			{
				// now check normal
				double *vn2;
				float vvn1[3];
				float vvn[3];


				vtkFloatArray* norms = vtkFloatArray::SafeDownCast
				(this->GetPointData()->GetNormals());
				if (norms != NULL)
				{
					vn2 = norms->GetTuple(newid);
					vvn1[0] = (float)vn2[0];
					vvn1[1] = (float)vn2[1];
					vvn1[2] = (float)vn2[2];
					glMatrix wc_mat;
					glMatrix screen_mat;
					glPushMatrix();
					glLoadIdentity();
					glMultMatrixf((GLfloat*) this->Mat2);
					glTranslated(
						this->mean[0],
						this->mean[1],
						this->mean[2]
					);
					glMultMatrixf((GLfloat *)this->Mat1);
					glTranslated(
						-this->mean[0],
						-this->mean[1],
						-this->mean[2]
					);
					this->get_world_coordinates_matrix(wc_mat);
					this->get_screen_projection_matrix(screen_mat);
					glPopMatrix();

					ApplyRotation(vvn1, vvn, wc_mat);	//rotation of the normals..


					double curr_cos = vn[0] * (double)vvn[0] + vn[1] * (double)vvn[1] + vn[2] * (double)vvn[2];
					if (curr_cos>g_magic_wand_extension_min_cos)
					{
						connectedVertices->InsertNextId(newid);
					}

				}
				else
				{
					std::cout << "Norm NULL..." << std::endl;
				}

			}
		}*/

	}
	// now I compute a mean vn.
	if (compute_avg_norm)
	{
		auto norms = vtkFloatArray::SafeDownCast(mesh->GetPointData()->GetNormals());

		//cout << "Have tried to get norms" << endl;
		//cout << "Safe point downcast done ! " << endl;
		if (norms)
		{
			//if (ve<10){std::cout << "Connected vertices: ";}		
			double *norm = norms->GetTuple3(id);
			double newn[3];
			newn[0] = norm [0];
			newn[1] = norm[1];
			newn[2] = norm[2];
			int n_vertices = connectedVertices->GetNumberOfIds();


			for (vtkIdType j = 0; j < n_vertices; j++)
			{	// for all neighbouring vertices							
				norm = norms->GetTuple3(connectedVertices->GetId(j));
				newn[0] += norm[0];
				newn[1] += norm[1];
				newn[2] += norm[2];
												

			}
			
			/*newnx /= (n_vertices+1);
			newny /= (n_vertices + 1);
			newnz /= (n_vertices + 1);*/
			vtkMath::Normalize(newn);
			
			vn[0] = newn[0];
			vn[1] = newn[1];
			vn[2] = newn[2];
		}
	}

	return connectedVertices;
}
void mqMorphoDigCore::RemoveArray(QString arrayName, int onlySelectedObjects)
{
	std::string marrayName = "RGB";
	if (arrayName.length() >0)
	{
		marrayName = arrayName.toStdString();
	}
	else
	{
		return;
	}

	this->ActorCollection->InitTraversal();
	vtkIdType num = this->ActorCollection->GetNumberOfItems();
	int modified = 0;
	for (vtkIdType i = 0; i < num; i++)
	{
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1 || onlySelectedObjects==0)
		{
			myActor->SetSelected(0);
			vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
			if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
			{

				vtkSmartPointer<vtkPolyData> mPD = vtkSmartPointer<vtkPolyData>::New();
					mPD = mymapper->GetInput();
					mPD->GetPointData()->RemoveArray(marrayName.c_str());									
					modified = 1;
			}
				


		}

		
	}
	if (modified == 1)
	{

		//cout << "camera and grid adjusted" << endl;
		cout << "scalars modified " << endl;
		this->Initmui_ExistingArrays();		

	}


}

void mqMorphoDigCore::EditArrayName(vtkSmartPointer<vtkMDActor> actor, QString oldArrayName, QString newArrayName)
{
	//vtkSmartPointer<vtkDoubleArray> newScalars = vtkSmartPointer<vtkDoubleArray>::New();
	vtkDataArray *mScalars = actor->GetMapper()->GetInput()->GetPointData()->GetScalars(oldArrayName.toStdString().c_str());
	mScalars->SetName(newArrayName.toStdString().c_str());
	this->Initmui_ExistingArrays();
}
void mqMorphoDigCore::createTagsConnectivity(QString newTags)
{
	if (newTags.length() == 0) { return; }
	int modified = 0;
	this->ActorCollection->InitTraversal();
	vtkIdType num = this->ActorCollection->GetNumberOfItems();
	for (vtkIdType i = 0; i < num; i++)
	{
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			if (myActor->GetConnectivityFilter() == nullptr)
			{
				cout << "Try to build connectivity filter!" << endl;
				myActor->BuildConnectivityFilter();
				cout << "Connectivity filter built" << endl;
			}

			myActor->SetSelected(0);
			vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
			if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
			{

				vtkSmartPointer<vtkPolyData> mPD = vtkSmartPointer<vtkPolyData>::New();
				mPD = mymapper->GetInput();

				double numvert = mPD->GetNumberOfPoints();


				vtkSmartPointer<vtkIntArray> newTagsArray =
					vtkSmartPointer<vtkIntArray>::New();
				newTagsArray->SetNumberOfComponents(1);
				newTagsArray->SetNumberOfTuples(mymapper->GetInput()->GetNumberOfPoints());
				//initialize array
				for (vtkIdType j = 0; j < mymapper->GetInput()->GetNumberOfPoints(); j++)
				{
					newTagsArray->SetTuple1(j, 0);
				}

				//tag what's found in the cfilter output
				//cout << "start new tags!" << endl;
				vtkSmartPointer<vtkIdList> clist = myActor->GetConnectivityRegionsCorrList();
				if (clist == nullptr) { cout << "clist is nullptr!" << endl; }
				else
				{
					int numIDs = clist->GetNumberOfIds();
					int numVeOrig = mymapper->GetInput()->GetNumberOfPoints();
					//cout << "num VE in orig mesh:" << numVeOrig << endl;
					//cout << "num corrlist ids:" << numIDs << endl;
					int numTup = myActor->GetConnectivityRegions()->GetNumberOfTuples();
					//cout << "num tups in connectivity regions:" << numTup << endl;
					for (vtkIdType j = 0; j < myActor->GetConnectivityRegions()->GetNumberOfTuples(); j++)
					{
						vtkIdType k = myActor->GetConnectivityRegionsCorrList()->GetId(j);
						if (k < mymapper->GetInput()->GetNumberOfPoints())
						{
							newTagsArray->SetTuple1(k, myActor->GetConnectivityRegions()->GetTuple1(j));
						}
						//newTagsArray->SetTuple1(j, myActor->GetConnectivityRegions()->GetTuple1(j));
					}
				}
				//cout << "end new tags!" << endl;

				newTagsArray->SetName(newTags.toStdString().c_str());
				mymapper->GetInput()->GetPointData()->RemoveArray(newTags.toStdString().c_str());
				mymapper->GetInput()->GetPointData()->AddArray(newTagsArray);
				mymapper->GetInput()->GetPointData()->SetActiveScalars(newTags.toStdString().c_str());
				modified = 1;
			}
		}
	}
	if (modified == 1)
	{
		this->Setmui_ActiveArray(newTags, VTK_INT, 1);
		this->Initmui_ExistingArrays();

	}

}

/*void CONTAINER_MESH::Convert_RGB_To_Tags(int exact)
{

		OBJECT_MESH *My_Obj;

		if (this->OBJECTS_ROOT->OBJECTS != NULL)
		{

		My_Obj = this->OBJECTS_ROOT->OBJECTS;

		while(My_Obj!=NULL)
		{

		if (My_Obj->selected ==1)
		{
		//std::cout<<"Found one object!"<<std::endl;
		vtkUnsignedCharArray* test = (vtkUnsignedCharArray*)My_Obj->GetPointData()->GetScalars("RGB");
		if (test !=NULL)
		{
		//std::cout<<"Test not null!"<<std::endl;
		vtkSmartPointer<vtkUnsignedCharArray> colors =
		vtkSmartPointer<vtkUnsignedCharArray>::New();
		colors->SetNumberOfComponents(3);
		colors=(vtkUnsignedCharArray*)My_Obj->GetPointData()->GetScalars("RGB");

		vtkSmartPointer<vtkIntArray> newTags =
		vtkSmartPointer<vtkIntArray>::New();

		newTags->SetNumberOfComponents(1); //3d normals (ie x,y,z)
		newTags->SetNumberOfTuples(My_Obj->numvert);


		if (exact ==0)
		{
		//std::cout<<"Non exact!"<<std::endl;

		// first : build a table of 25 first distinct colours found in RGB scalars.
		GLfloat rgb_distinct_colors[25][4];
		for (int i =0;i<25;i++)
		{
		rgb_distinct_colors[i][0]=0;
		rgb_distinct_colors[i][1]=0;
		rgb_distinct_colors[i][2]=0;
		rgb_distinct_colors[i][3]=1;
		}
		//std::cout<<"RGB list initiated!"<<std::endl;

		int cdistinct=0; //current number of distinct colours found

		for (int i=0;i<My_Obj->numvert;i++)	// for each vertex
		{
		if (cdistinct<25)
		{
		//std::cout<<"Try to get tuple"<< i<<"!"<<std::endl;
		int cur_r =colors->GetTuple(i)[0];
		int cur_g =colors->GetTuple(i)[1];
		int cur_b =colors->GetTuple(i)[2];
		//std::cout<<"RGB "<<i<<"="<<cur_r<<","<<cur_g<<","<<cur_b<<std::endl;
		int already=0;
		for (int j=0; j<cdistinct;j++)
		{
		if ( (cur_r == mround(255*rgb_distinct_colors[j][0]))
		&&(cur_g == mround(255*rgb_distinct_colors[j][1]))
		&&(cur_b == mround(255*rgb_distinct_colors[j][2]))
		)
		{
		already=1;
		}

		}
		if (already==0)
		{
		//std::cout<<"Found "<<i<<"="<<cur_r<<","<<cur_g<<","<<cur_b<<std::endl;

		rgb_distinct_colors[cdistinct][0]= (float)cur_r/255;
		rgb_distinct_colors[cdistinct][1]= (float)cur_g/255;
		rgb_distinct_colors[cdistinct][2]= (float)cur_b/255;
		cdistinct++;
		}


		}
		else
		{break;}


		}
		// then edit g_tag_colors[25][4] accordingly
		for (int i=0;i<cdistinct;i++)
		{
		g_tag_colors[i][0]=rgb_distinct_colors[i][0];
		g_tag_colors[i][1]=rgb_distinct_colors[i][1];
		g_tag_colors[i][2]=rgb_distinct_colors[i][2];
		}

		}

		// now we match RGB and Tags
		for (int i=0;i<My_Obj->numvert;i++)	// for each vertex
		{
		int cur_r =colors->GetTuple(i)[0];
		int cur_g =colors->GetTuple(i)[1];
		int cur_b =colors->GetTuple(i)[2];
		int tag_id=0;
		//std::cout<<"Now match! RGB "<<i<<"="<<cur_r<<","<<cur_g<<","<<cur_b<<std::endl;

		for (int j=0; j<25;j++)
		{
		if ( (cur_r == mround(255*g_tag_colors[j][0]))
		&&(cur_g == mround(255*g_tag_colors[j][1]))
		&&(cur_b == mround(255*g_tag_colors[j][2])))
		{
		tag_id=j;break;
		}

		}
		//std::cout<<"vertex"<<i<<", current region:"<<currentRegions->GetTuple(i)[0]<<std::endl;
		// get Tag id corresponding to actual RGB colour...
		//newTags->InsertTuple1(i, (int)currentRegions->GetTuple(i)[0]);
		newTags->InsertTuple1(i, tag_id);
		}
		newTags->SetName("Tags");
		My_Obj->GetPointData()->RemoveArray("Tags");
		My_Obj->GetPointData()->AddArray(newTags);
		My_Obj->GetPointData()->SetActiveScalars("Tags");
		My_Obj->selected=0;
		My_Obj->view=1;
		}//if RGB colour scalar exists
		}//if selected

		My_Obj = My_Obj->nextobj;
		}//while


}
}*/
void mqMorphoDigCore::createTagsFromRGB(QString newTags, int exact, int N)
{
	if (newTags.length() == 0) { return; }
	int modified = 0;
	this->ActorCollection->InitTraversal();
	vtkIdType num = this->ActorCollection->GetNumberOfItems();
	for (vtkIdType i = 0; i < num; i++)
	{
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{

			myActor->SetSelected(0);
			vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
			if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
			{

				vtkSmartPointer<vtkPolyData> mPD = vtkSmartPointer<vtkPolyData>::New();
				mPD = mymapper->GetInput();

				double numvert = mPD->GetNumberOfPoints();


				vtkSmartPointer<vtkIntArray> newTagsArray =
					vtkSmartPointer<vtkIntArray>::New();
				newTagsArray->SetNumberOfComponents(1);
				newTagsArray->SetNumberOfTuples(mymapper->GetInput()->GetNumberOfPoints());

				if (exact == 0)
				{
					std::cout << "Non exact!" << std::endl;
					// first : build a vector of N first distinct colours found.
					int cdistinct = 0; //current number of distinct colours found
					/*GLfloat rgb_distinct_colors[25][4];
					for (int i = 0; i<25; i++)
					{
						rgb_distinct_colors[i][0] = 0;
						rgb_distinct_colors[i][1] = 0;
						rgb_distinct_colors[i][2] = 0;
						rgb_distinct_colors[i][3] = 1;
					}*/
					std::vector<int> r;
					std::vector<int> g;
					std::vector<int> b;
					std::vector<int> a;
					for (vtkIdType ve = 0; ve < mymapper->GetInput()->GetNumberOfPoints(); ve++)
					{
						if (cdistinct<N)
						{
							int col[4];
							this->GetVertexColor(myActor, ve, col);
							int already = 0;
							for (int i = 0; i < cdistinct; i++)
							{
								if (
									(col[0] == r.at(i))
									&& (col[1] == g.at(i))
									&& (col[2] == b.at(i))
									)
								{
									already = 1;
								}
							}
							if (already == 0)
							{
								//std::cout<<"Found "<<i<<"="<<cur_r<<","<<cur_g<<","<<cur_b<<std::endl;

								r.push_back(col[0]);
								g.push_back(col[1]);
								b.push_back(col[2]);
								a.push_back(col[3]);
								std::cout << "push rgba:" << ve << "=" << ve << ", r:" << col[0] << ", g:" << col[1] << ", b:" << col[2] << ", a:" << col[3] << std::endl;
								cdistinct++;
							}
						}
						else
						{
							break;
						}
						
						// here we have the color.
						//newTagsArray->SetTuple1(j, 0);

					}
					cout << "We found " << cdistinct << " distinct colors" << endl;
					// then edit current Tag Map according to the cdistinct color found 
					ActiveTagMap *tagMap = this->Getmui_ActiveTagMap();
					int currenttagMapId = this->getActiveTagMapId();
					int tagnr = cdistinct+1;
					vtkSmartPointer<vtkLookupTable> mTagLut = vtkSmartPointer<vtkLookupTable>::New();
					mTagLut = this->GetTagLut();
					mTagLut->SetNumberOfTableValues(tagnr);
					mTagLut->Build();
					std::vector<std::string> tagNames;


					double rgba[4];
					
					//0 is special (exterior)
					this->GetDefaultTagColor(0, rgba);
					mTagLut->SetTableValue(0, rgba[0], rgba[1], rgba[2], rgba[3]);
					tagNames.push_back("Exterior");
					
					// the cdistinct other colors
					for (int i = 0; i < cdistinct; i++)
					{
						
						rgba[0] = (double)((double)r.at(i)/255);
						rgba[1] = (double)((double)g.at(i) / 255);
						rgba[2] = (double)((double)b.at(i) / 255);
						rgba[3] = (double)((double)a.at(i) / 255);
						
						mTagLut->SetTableValue(i+1, rgba[0], rgba[1], rgba[2], rgba[3]);
						
						QString TagName = "TagFromRGB" + QString::number(i);
						tagNames.push_back(TagName.toStdString());
						
					}
					cout << "End i loop" << endl;
					QString TagMap = QString("TagMap");
					cout << "Try to set existing color maps!!" << endl;

					this->Getmui_ExistingTagMaps()->Stack.at(currenttagMapId).numTags = tagnr;
					this->Getmui_ExistingTagMaps()->Stack.at(currenttagMapId).tagNames = tagNames;
					this->Getmui_ExistingTagMaps()->Stack.at(currenttagMapId).TagMap = mTagLut;
					this->Setmui_ActiveTagMap(TagMap, tagnr, tagNames, mTagLut);
					
				}// end special case exact==0

				 // now we match RGB and Tags of active tag map
				ActiveTagMap *tagMap = this->Getmui_ActiveTagMap();
				for (vtkIdType ve = 0; ve < mymapper->GetInput()->GetNumberOfPoints(); ve++)
				{
					int col[4];
					this->GetVertexColor(myActor, ve, col);
					int tag_id = 0; //ext by default
					for (int j = 0; j < tagMap->numTags; j++)
					{
						if ((col[0] == (unsigned char)(255 * tagMap->TagMap->GetTableValue(j)[0]))
							&& (col[1] == (unsigned char)(255 * tagMap->TagMap->GetTableValue(j)[1]))
							&& (col[2] == (unsigned char)(255 * tagMap->TagMap->GetTableValue(j)[2]))
							)
						{
							tag_id = j; break;
						}
					}
					// here we have the color.
					newTagsArray->SetTuple1(ve, tag_id);
				}
			
				
				newTagsArray->SetName(newTags.toStdString().c_str());
				mymapper->GetInput()->GetPointData()->RemoveArray(newTags.toStdString().c_str());
				mymapper->GetInput()->GetPointData()->AddArray(newTagsArray);
				mymapper->GetInput()->GetPointData()->SetActiveScalars(newTags.toStdString().c_str());
				modified = 1;
			}
		}
	}
	if (modified == 1)
	{
		this->Setmui_ActiveArray(newTags, VTK_INT, 1);
		this->Initmui_ExistingArrays();

	}
}
void mqMorphoDigCore::MergeTags(int tagSource, int tagTarget)
{
	if (tagSource != tagTarget)
	{
		int modified = 0;
		this->ActorCollection->InitTraversal();
		vtkIdType num = this->ActorCollection->GetNumberOfItems();
		for (vtkIdType i = 0; i < num; i++)
		{
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
			vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
			if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
			{
				vtkSmartPointer<vtkPolyData> mPD = vtkSmartPointer<vtkPolyData>::New();
				mPD = mymapper->GetInput();
				double numvert = mPD->GetNumberOfPoints();
				QString sActiveArray = this->Getmui_ActiveArray()->Name;
				vtkIntArray *currentTags = vtkIntArray::SafeDownCast(mymapper->GetInput()->GetPointData()->GetScalars(sActiveArray.toStdString().c_str()));
				//4 if current tags exist, retrieve what tool is active
				if (currentTags != NULL)
				{
					std::string action = "Merge tags";
					action.append(myActor->GetName().c_str());
					int Count = BEGIN_UNDO_SET(action);
					std::string mArrayName = sActiveArray.toStdString();
					myActor->SaveState(Count, QString(mArrayName.c_str()), 1);
					for (vtkIdType j = 0; j < mPD->GetNumberOfPoints(); j++)
					{
						if (currentTags->GetTuple1(j) == tagSource)
						{
							currentTags->SetTuple1(j, tagTarget);
							modified = 1;
						}
					}
					currentTags->Modified();
					END_UNDO_SET();
				}
			}
		}
		if (modified == 1)
		{
			
			//this->Setmui_ActiveArray(newTags, VTK_INT, 1);
			//this->Initmui_ExistingArrays();
			this->Render();
		}
	}
}
void mqMorphoDigCore::createTags(QString newTags)
{
	if (newTags.length() == 0) { return; }
	int modified = 0;
	this->ActorCollection->InitTraversal();
	vtkIdType num = this->ActorCollection->GetNumberOfItems();
	for (vtkIdType i = 0; i < num; i++)
	{
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{

			myActor->SetSelected(0);			
			vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
			if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
			{

				vtkSmartPointer<vtkPolyData> mPD = vtkSmartPointer<vtkPolyData>::New();
				mPD = mymapper->GetInput();

				double numvert = mPD->GetNumberOfPoints();


				vtkSmartPointer<vtkIntArray> newTagsArray =
					vtkSmartPointer<vtkIntArray>::New();
				newTagsArray->SetNumberOfComponents(1);
				newTagsArray->SetNumberOfTuples(mymapper->GetInput()->GetNumberOfPoints());
				for (vtkIdType j = 0; j < mymapper->GetInput()->GetNumberOfPoints(); j++)
				{
					newTagsArray->SetTuple1(j, 0);
				}
				newTagsArray->SetName(newTags.toStdString().c_str());
				mymapper->GetInput()->GetPointData()->RemoveArray(newTags.toStdString().c_str());
				mymapper->GetInput()->GetPointData()->AddArray(newTagsArray);
				mymapper->GetInput()->GetPointData()->SetActiveScalars(newTags.toStdString().c_str());
				modified = 1;
			}
		}
	}
	if (modified ==1)
	{
		this->Setmui_ActiveArray(newTags, VTK_INT, 1);
		this->Initmui_ExistingArrays();

	}

}
void mqMorphoDigCore::DuplicateArray(vtkSmartPointer<vtkMDActor> actor, QString ArrayName, QString newArrayName)
{
	//no duplication of 
	if (QString::compare(ArrayName, newArrayName, Qt::CaseInsensitive) ==0){ return; }
	if (newArrayName.length() == 0) { return; }

	int mode = 0;
	//0 : scalars
	//1 : RGB
	//2 : tags

	for (int i = 0; i < this->Getmui_ExistingArrays()->Stack.size(); i++)
	{
		QString myExisingArrayName =this->Getmui_ExistingArrays()->Stack.at(i).Name;
		if (ArrayName == myExisingArrayName)
		{

		
			int type = this->Getmui_ExistingArrays()->Stack.at(i).DataType;
			int ncomp = this->Getmui_ExistingArrays()->Stack.at(i).NumComp;
			if (ncomp == 1 && (type == VTK_FLOAT || type == VTK_DOUBLE)) { mode = 0; }
			if (ncomp >= 3 && type == VTK_UNSIGNED_CHAR) { mode = 1; }
			if (ncomp == 1 && (type == VTK_INT || type == VTK_UNSIGNED_INT)) { mode = 2; }

		}
	}


	
	//vtkDataArray* Scalars = actor->GetMapper()->GetInput()->GetPointData()->GetScalars(ArrayName.toStdString().c_str());

	vtkSmartPointer<vtkDataArray> newScalars = nullptr;
	if (mode == 0)
	{
		newScalars = vtkSmartPointer<vtkDoubleArray>::New();
	}
	if (mode == 1)
	{
		newScalars = vtkSmartPointer<vtkUnsignedCharArray>::New();
	}
	if (mode == 2)
	{
		newScalars = vtkSmartPointer<vtkIntArray>::New();
	}
	// now we have to decide what type of array this is!
	 
	 newScalars->DeepCopy(actor->GetMapper()->GetInput()->GetPointData()->GetScalars(ArrayName.toStdString().c_str()));	
	//remove newArrayName array if exists.
	 actor->GetMapper()->GetInput()->GetPointData()->RemoveArray(newArrayName.toStdString().c_str());
	 newScalars->SetName(newArrayName.toStdString().c_str());
	 actor->GetMapper()->GetInput()->GetPointData()->AddArray(newScalars);
	this->Initmui_ExistingArrays();

}

void mqMorphoDigCore::DeleteArray(vtkSmartPointer<vtkMDActor> actor, QString ArrayName)
{
	//this->Getmui_ActiveArray
	//this->Getmui_ActiveArray()->Name
	 actor->GetMapper()->GetInput()->GetPointData()->RemoveArray(ArrayName.toStdString().c_str());
	
	 cout << "Try to init mui scalars" << endl;
	this->Initmui_ExistingArrays();
	cout << "Try to init mui scalars ok" << endl;

	
}

void mqMorphoDigCore::ICP(int transformationMode, int iterationNumber, vtkMDActor *impactedActor, vtkMDActor* observedActor)
{
	if (impactedActor != NULL && observedActor != NULL && (impactedActor != observedActor))
	{
		std::string action = "ICP transform for ";
		action.append(impactedActor->GetName().c_str());
		int Count = BEGIN_UNDO_SET(action);
		impactedActor->SaveState(Count);
		vtkPolyDataMapper *myImpactedMapper = vtkPolyDataMapper::SafeDownCast(impactedActor->GetMapper());
		vtkPolyDataMapper *myObservedMapper = vtkPolyDataMapper::SafeDownCast(observedActor->GetMapper());
		if (myImpactedMapper != NULL && vtkPolyData::SafeDownCast(myImpactedMapper->GetInput()) != NULL)
		{
			

			vtkSmartPointer<vtkPolyData> mImpactedPD = vtkSmartPointer<vtkPolyData>::New();
			mImpactedPD = myImpactedMapper->GetInput();
			vtkSmartPointer<vtkPolyData> mImpactedPDPos = vtkSmartPointer<vtkPolyData>::New();
			mImpactedPDPos->DeepCopy(vtkPolyData::SafeDownCast(mImpactedPD));
			double ve_init_pos[3];;
			double ve_final_pos[3];
			/*vtkSmartPointer<vtkMatrix4x4> Mat = impactedActor->GetMatrix();
			vtkSmartPointer<vtkMatrix4x4> impMat = vtkSmartPointer<vtkMatrix4x4>::New();
			impMat->DeepCopy(Mat);*/
			vtkSmartPointer<vtkMatrix4x4> impMat = impactedActor->GetMatrix();
			
			for (vtkIdType i = 0; i < mImpactedPDPos->GetNumberOfPoints(); i++) {
				// for every triangle 
				mImpactedPDPos->GetPoint(i, ve_init_pos);
				mqMorphoDigCore::TransformPoint(impMat, ve_init_pos, ve_final_pos);

				mImpactedPDPos->GetPoints()->SetPoint((vtkIdType)i, ve_final_pos);
			}
			
			vtkSmartPointer<vtkPolyData> mObservedPD = vtkSmartPointer<vtkPolyData>::New();
			mObservedPD = myObservedMapper->GetInput();
			vtkSmartPointer<vtkPolyData> mObservedPDPos = vtkSmartPointer<vtkPolyData>::New();
			mObservedPDPos->DeepCopy(vtkPolyData::SafeDownCast(mObservedPD));			
			vtkSmartPointer<vtkMatrix4x4> obsMat = observedActor->GetMatrix();
			
			for (vtkIdType i = 0; i < mObservedPDPos->GetNumberOfPoints(); i++) {
				// for every triangle 
				mObservedPDPos->GetPoint(i, ve_init_pos);
				mqMorphoDigCore::TransformPoint(obsMat, ve_init_pos, ve_final_pos);

				mObservedPDPos->GetPoints()->SetPoint((vtkIdType)i, ve_final_pos);
			}


			// now : apply current matrix to 

			vtkSmartPointer<vtkIterativeClosestPointTransform> icp =
				vtkSmartPointer<vtkIterativeClosestPointTransform>::New();

			icp->SetSource(mImpactedPDPos);
			icp->SetTarget(mObservedPDPos);
			icp->GetLandmarkTransform()->SetMode(transformationMode);
			icp->SetMaximumNumberOfIterations(20);
			//icp->StartByMatchingCentroidsOn();
			icp->Modified();
			icp->Update();
			vtkSmartPointer<vtkMatrix4x4> m = icp->GetMatrix();

			vtkSmartPointer<vtkMatrix4x4> finalMat = vtkSmartPointer<vtkMatrix4x4>::New();
			vtkMatrix4x4::Multiply4x4(m, impMat, finalMat);
			impactedActor->ApplyMatrix(finalMat);
			//64, 123, 126
			double r = (double)64/((double)255);
			double g = (double)123/255;
			double b = (double)126/255;
			//impactedActor->SetmColor((double)(64/255), (double)(123 / 255), (double)(126/255), 0.5);
			impactedActor->SetmColor(r, g, b, 0.5);
		}

		END_UNDO_SET();
		this->Render();
	}
}
void mqMorphoDigCore::scalarsDistance(double maxDist, int avg, QString scalarName, vtkMDActor *impactedActor, vtkMDActor* observedActor)
{
	if (impactedActor != NULL && observedActor != NULL &&observedActor!=impactedActor)
	{
		std::string action = "Compute distance for ";
		action.append(impactedActor->GetName().c_str());
		int Count = BEGIN_UNDO_SET(action);

		std::string mScalarName = "Distance";
		if (scalarName.length() > 0)
		{
			mScalarName = scalarName.toStdString();
		}
		impactedActor->SetSelected(0);
		impactedActor->SaveState(Count, QString(mScalarName.c_str()));
		vtkPolyDataMapper *myImpactedMapper = vtkPolyDataMapper::SafeDownCast(impactedActor->GetMapper());
		vtkPolyDataMapper *myObservedMapper = vtkPolyDataMapper::SafeDownCast(observedActor->GetMapper());
		if (myImpactedMapper != NULL && vtkPolyData::SafeDownCast(myImpactedMapper->GetInput()) != NULL)
		{

			vtkSmartPointer<vtkPolyData> mImpactedPD = vtkSmartPointer<vtkPolyData>::New();
			mImpactedPD = myImpactedMapper->GetInput();

			vtkSmartPointer<vtkPolyData> mObservedPD = vtkSmartPointer<vtkPolyData>::New();
			mObservedPD = myObservedMapper->GetInput();


			double numvert = myImpactedMapper->GetInput()->GetNumberOfPoints();


			vtkSmartPointer<vtkDoubleArray> newScalars =
				vtkSmartPointer<vtkDoubleArray>::New();

			newScalars->SetNumberOfComponents(1); //3d normals (ie x,y,z)
			newScalars->SetNumberOfTuples(numvert);
			vtkIdType ve;

			double ve_obs_init_pos[3];;
			double ve_obs_final_pos[3];;
			
			double ve_imp_init_pos[3];;
			double ve_imp_final_pos[3];;


			vtkSmartPointer<vtkMatrix4x4> impMat = impactedActor->GetMatrix();
			vtkSmartPointer<vtkMatrix4x4> obsMat = observedActor->GetMatrix();
	
			
			double AB[3];
			double min_dist = maxDist;
			double tmp_dist = 0;
			
			vtkSmartPointer<vtkKdTreePointLocator> kDTree =
				vtkSmartPointer<vtkKdTreePointLocator>::New();

			vtkSmartPointer<vtkPolyData> observedMoved = vtkSmartPointer<vtkPolyData>::New();
			observedMoved->DeepCopy(vtkPolyData::SafeDownCast(mObservedPD));


			for (vtkIdType i = 0; i < observedMoved->GetNumberOfPoints(); i++) {
				// for every triangle 
				observedMoved->GetPoint(i, ve_obs_init_pos);
				mqMorphoDigCore::TransformPoint(obsMat, ve_obs_init_pos, ve_obs_final_pos);

				observedMoved->GetPoints()->SetPoint((vtkIdType)i, ve_obs_final_pos);
			}


			kDTree->SetDataSet(observedMoved);
			cout << "Start build kdtree"  << endl;
			kDTree->BuildLocator();
			cout << "KdTree built" << endl;
			for (ve = 0; ve < numvert; ve++)
			{
				min_dist = maxDist;
				if ((ve % (int)(numvert / 100)) == 0)
				{
					emit distanceProgression((int)(100 * ve / numvert));
				}
				//progress.setValue(ve);
				/*if (progress.wasCanceled())
				break;*/

				mImpactedPD->GetPoint(ve, ve_imp_init_pos);
				mqMorphoDigCore::TransformPoint(impMat, ve_imp_init_pos, ve_imp_final_pos);
		
				vtkSmartPointer<vtkIdList> connectedVertices = vtkSmartPointer<vtkIdList>::New();
				// get all neighbouring vertices within a maxDist radius
				vtkSmartPointer<vtkIdList> observedNeighbours = vtkSmartPointer<vtkIdList>::New();
				double Radius = 1.1*maxDist;
				kDTree->FindPointsWithinRadius(Radius, ve_imp_final_pos, observedNeighbours);

				double newscalar = 0;
		
				std::vector<double> distances;
				int cpt = 0;


				for (vtkIdType j = 0; j < observedNeighbours->GetNumberOfIds(); j++)
				{

					//if (ve<10){std::cout << connectedVertices->GetId(j) << " ";}
					vtkIdType observedConnectedVertex = observedNeighbours->GetId(j);
					if (ve != observedConnectedVertex || impactedActor != observedActor)// to cases: 1) self observation= we don't want to compare 1 vertex with itself; 2) comparison, then impacted and observed lists do not have links
					{
						observedMoved->GetPoint(observedConnectedVertex, ve_obs_final_pos);
						
						
						AB[0] = ve_obs_final_pos[0] - ve_imp_final_pos[0];
						AB[1] = ve_obs_final_pos[1] - ve_imp_final_pos[1];
						AB[2] = ve_obs_final_pos[2] - ve_imp_final_pos[2];
						double curr_dist = sqrt(AB[0] * AB[0] + AB[1] * AB[1] + AB[2] * AB[2]);
						//ABnorm[0] = 0; ABnorm[1] = 0; ABnorm[2] = 0;
						
						if (curr_dist < maxDist)
						{
							
								tmp_dist = curr_dist;
								distances.push_back(tmp_dist);
								cpt++;

							}
							else
							{
								tmp_dist = maxDist;
							}
						}
						else
						{
							tmp_dist = maxDist;
						}
						if (tmp_dist < min_dist) { min_dist = tmp_dist; }

					

				}

				// if (ve<10){std::cout<<"New Scalar value at "<<ve<<"="<<newscalar<<std::endl;}	
				if (avg <= 1 || cpt == 0)
				{

					newScalars->InsertTuple1(ve, min_dist);
				}
				else
				{
					/*if (ve < 10)
					{
					cout << "try to sort thicknesses"  << endl;
					}*/
					std::sort(distances.begin(), distances.end());
					double dist_avg = 0;
					double mavg = avg;// in some cases there are less thickness candidates thant avg => the avg thickness will be done using less than "avg" nr of candidates.
					if (cpt < avg) { mavg = cpt; }
					for (int i = 0; i < mavg; i++)
					{
						dist_avg += distances.at(i);
					}
					dist_avg /= mavg;
					newScalars->InsertTuple1(ve, dist_avg);					

				}


			}
			std::cout<<"New Distance Scalar computation done "<<std::endl;

			newScalars->SetName(mScalarName.c_str());
			// test if exists...
			//
			int exists = 0;

			// remove this scalar
			//this->GetPointData()->SetScalars(newScalars);
			mImpactedPD->GetPointData()->RemoveArray(mScalarName.c_str());
			mImpactedPD->GetPointData()->AddArray(newScalars);
			mImpactedPD->GetPointData()->SetActiveScalars(mScalarName.c_str());
			

		


		}
		//cout << "camera and grid adjusted" << endl;
		cout << "DISTANCE scalars computed " << endl;
		this->Initmui_ExistingArrays();
		this->Setmui_ActiveArrayAndRender(mScalarName.c_str(), VTK_DOUBLE, 1);


		END_UNDO_SET();
	}

}
void mqMorphoDigCore::scalarsThicknessBetween(double max_thickness, int smooth_normals, int avg, QString scalarName, vtkMDActor *impactedActor, vtkMDActor* observedActor, double angularLimit, int invertObservedNormals)
{
	cout << "Call scalarsThicknessBetween" << endl;
	if (impactedActor != NULL && observedActor != NULL)
	{
		std::string action = "Compute thickness for ";
		action.append(impactedActor->GetName().c_str());
		int Count = BEGIN_UNDO_SET(action);

		std::string mScalarName = "Thickness";
		if (scalarName.length() > 0)
		{
			mScalarName = scalarName.toStdString();
		}
		impactedActor->SaveState(Count, QString(mScalarName.c_str()));
		vtkPolyDataMapper *myImpactedMapper = vtkPolyDataMapper::SafeDownCast(impactedActor->GetMapper());
		vtkPolyDataMapper *myObservedMapper = vtkPolyDataMapper::SafeDownCast(observedActor->GetMapper());
		if (myImpactedMapper != NULL && vtkPolyData::SafeDownCast(myImpactedMapper->GetInput()) != NULL)
		{

			vtkSmartPointer<vtkPolyData> mImpactedPD = vtkSmartPointer<vtkPolyData>::New();
			mImpactedPD = myImpactedMapper->GetInput();

			vtkSmartPointer<vtkPolyData> mObservedPD = vtkSmartPointer<vtkPolyData>::New();
			mObservedPD = myObservedMapper->GetInput();




			double numvert = myImpactedMapper->GetInput()->GetNumberOfPoints();


			vtkSmartPointer<vtkDoubleArray> newScalars =
				vtkSmartPointer<vtkDoubleArray>::New();

			newScalars->SetNumberOfComponents(1); //3d normals (ie x,y,z)
			newScalars->SetNumberOfTuples(numvert);
			vtkIdType ve;

			double ve_obs_init_pos[3];;			
			double ve_obs_final_pos[3];;
			double ven_obs_init_pos[3];;
			
			double ve_imp_init_pos[3];;
			double ven_imp_init_pos[3];;


			vtkSmartPointer<vtkMatrix4x4> impMat = impactedActor->GetMatrix();
			vtkSmartPointer<vtkMatrix4x4> obsMat = observedActor->GetMatrix();

			/*vtkSmartPointer<vtkPolyData> toSave = vtkSmartPointer<vtkPolyData>::New();
			toSave->DeepCopy(vtkPolyData::SafeDownCast(mapper->GetInput()));
			double ve_init_pos[3];;
			double ve_final_pos[3];
			vtkSmartPointer<vtkMatrix4x4> Mat = myActor->GetMatrix();


			for (vtkIdType i = 0; i < toSave->GetNumberOfPoints(); i++) {
				// for every triangle 
				toSave->GetPoint(i, ve_init_pos);
				mqMorphoDigCore::TransformPoint(Mat, ve_init_pos, ve_final_pos);

				toSave->GetPoints()->SetPoint((vtkIdType)i, ve_final_pos);
			}*/

			double pt[3] = { 0,0,1 };
			double pt2[3] = { 0,0,1 };
			double projected_pt2[3] = { 0,0,1 };
			double ptn[3];
			double *ptn2;
			double ptn2obs[3];
			double min_cos = 0.342; // 70 degrees => Don't want to compute thickness using badly oriented vertices.
			if (angularLimit > 0 && angularLimit <= 180)
			{
				min_cos = cos(angularLimit*vtkMath::Pi() / 180);
			}
			cout << "min_cos=" << min_cos << endl;
			double cur_cos = 1.0; // compare ve1's normal and ve2's normal
			double cur_cos2 = 1.0; //compare  ve1's normal and vector between ve1 and ve2
			double AB[3];
			double AC[3];
			double ABnorm[3];

			double picked_value = 0;
			double min_dist = max_thickness;
			double tmp_dist = 0;
			auto impactedNorms = impactedActor->GetPointNormals();
			//  auto impactedNorms =vtkFloatArray::SafeDownCast(mImpactedPD->GetPointData()->GetNormals()); .
			auto observedNorms = observedActor->GetPointNormals(); 
			//auto observedNorms = vtkFloatArray::SafeDownCast(mObservedPD->GetPointData()->GetNormals());
			
			//	cout << "Have tried to get norms" << endl;
			//cout << "Safe point downcast done ! " << endl;
			//if (impactedNorms && observedNorms)
			if (impactedNorms && observedNorms)
			{
				//	cout << "We have found some norms" << endl;

				//QProgressDialog progress("Thickness computation.", "Abort thickness computation", 0, numvert);
				//progress.setWindowModality(Qt::WindowModal);
				vtkSmartPointer<vtkKdTreePointLocator> kDTree =
					vtkSmartPointer<vtkKdTreePointLocator>::New();
				
				vtkSmartPointer<vtkPolyData> observedMoved = vtkSmartPointer<vtkPolyData>::New();
				observedMoved->DeepCopy(vtkPolyData::SafeDownCast(mObservedPD));
				

				for (vtkIdType i = 0; i < observedMoved->GetNumberOfPoints(); i++) {
					// for every triangle 
					observedMoved->GetPoint(i, ve_obs_init_pos);
					mqMorphoDigCore::TransformPoint(obsMat, ve_obs_init_pos, ve_obs_final_pos);

					observedMoved->GetPoints()->SetPoint((vtkIdType)i, ve_obs_final_pos);
				}


				kDTree->SetDataSet(observedMoved);
				
				kDTree->BuildLocator();

				for (ve = 0; ve < numvert; ve++)
				{
					min_dist = max_thickness;
					if ((ve % (int)(numvert / 100)) == 0)
					{
						emit thicknessProgression((int)(100 * ve / numvert));
					}
					//progress.setValue(ve);
					/*if (progress.wasCanceled())
					break;*/

					mImpactedPD->GetPoint(ve, ve_imp_init_pos);
					mqMorphoDigCore::TransformPoint(impMat, ve_imp_init_pos, pt);
					double *mImpactedptn = impactedNorms->GetTuple3(ve);
					ven_imp_init_pos[0] = mImpactedptn[0];
					ven_imp_init_pos[1] = mImpactedptn[1];
					ven_imp_init_pos[2] = mImpactedptn[2];
					
					
					/*ptn[0] = mImpactedptn[0];
					ptn[1] = mImpactedptn[1];
					ptn[2] = mImpactedptn[2];*/

					if (ve < 10)
					{
						//cout << "ptn" << ptn[0] << "," << ptn[1] << "," << ptn[2] << endl;
					}
					vtkSmartPointer<vtkIdList> connectedVertices = vtkSmartPointer<vtkIdList>::New();
					//ptn will be the average norm of all connected vertices.
					if (smooth_normals)
					{
						connectedVertices = GetConnectedVertices(mImpactedPD, ptn, picked_value, ve, -1, 1); //	int tool_mode=-1; //no pencil! no magic wand!
					}
					else
					{
						if (ve < 10)
						{
							//cout << "no ptn avg!" << endl;
						}


					}
					mqMorphoDigCore::RotateNorm(impMat, ven_imp_init_pos, ptn);
					if (ve < 10) {
					//	cout << "ven_imp_init_pos=" << ven_imp_init_pos[0] << "|" << ven_imp_init_pos[1] << "|" << ven_imp_init_pos[2] << "|" << endl;
					//	cout << "ptn=" << ptn[0] << "|" << ptn[1] << "|" << ptn[2] << "|" << endl;

					}

					if (ve < 10)
					{
						//cout << "avg ptn" << ptn[0] << "," << ptn[1] << "," << ptn[2] << endl;
					}

						////self thickness : look for norms in other directions!
						ptn[0] = -ptn[0];
						ptn[1] = -ptn[1];
						ptn[2] = -ptn[2];
					
					// here we will look at the neighbour vertices of pt to compute an average ptn (otherwise this filter is extremely sensitive with non planar/rough surfaces with changing normals)



					// get all neighbouring vertices within a max_thickness radius
					//if (ve<10){std::cout<<"Try to find connected vertices at : "<<ve<<std::endl;}
					vtkSmartPointer<vtkIdList> observedNeighbours = vtkSmartPointer<vtkIdList>::New();
					double Radius = 1.1*max_thickness;
					kDTree->FindPointsWithinRadius(Radius, pt, observedNeighbours);




					//if (ve<10){std::cout << "Connected vertices: ";}
					double newscalar = 0;
					/*if (ve < 10)
					{
					cout << "found" << neighbours->GetNumberOfIds() << " neighbours" << endl;
					}*/

					std::vector<double> thicknesses;
					int cpt = 0;





					for (vtkIdType j = 0; j < observedNeighbours->GetNumberOfIds(); j++)
					{

						//if (ve<10){std::cout << connectedVertices->GetId(j) << " ";}
						vtkIdType observedConnectedVertex = observedNeighbours->GetId(j);
						if (ve != observedConnectedVertex || impactedActor != observedActor)// to cases: 1) self observation= we don't want to compare 1 vertex with itself; 2) comparison, then impacted and observed lists do not have links
						{
							observedMoved->GetPoint(observedConnectedVertex, pt2);
							ptn2 = observedNorms->GetTuple3(observedConnectedVertex);

							ven_obs_init_pos[0] = ptn2[0]; 
							ven_obs_init_pos[1] = ptn2[1];
							ven_obs_init_pos[2] = ptn2[2];
							mqMorphoDigCore::RotateNorm(obsMat, ven_obs_init_pos, ptn2obs);
							if (ve<10 && j < 3) {
							//	cout << "j=" << j << endl;
							//	cout << "ven_obs_init_pos=" << ven_obs_init_pos[0] << "|" << ven_obs_init_pos[1] << "|" << ven_obs_init_pos[2] << "|" << endl;
							//	cout << "ptn2obs=" << ptn2obs[0] << "|" << ptn2obs[1] << "|" << ptn2obs[2] << "|" << endl;

							}

							// do not forget to rotate observed norms!
							/*ptn2obs[0] = ptn2[0];
							ptn2obs[1] = ptn2[1];
							ptn2obs[2] = ptn2[2];*/

							if (observedActor != impactedActor && invertObservedNormals==1)
							{
								ptn2obs[0] = -ptn2obs[0];
								ptn2obs[1] = -ptn2obs[1];
								ptn2obs[2] = -ptn2obs[2];

							}
							AB[0] = pt2[0] - pt[0];
							AB[1] = pt2[1] - pt[1];
							AB[2] = pt2[2] - pt[2];
							double curr_dist = sqrt(AB[0] * AB[0] + AB[1] * AB[1] + AB[2] * AB[2]);
							ABnorm[0] = 0; ABnorm[1] = 0; ABnorm[2] = 0;
							if (curr_dist > 0)
							{
								ABnorm[0] = AB[0] / curr_dist;
								ABnorm[1] = AB[1] / curr_dist;
								ABnorm[2] = AB[2] / curr_dist;
							}
							if (curr_dist < max_thickness)
							{
								// calculate projected point along the normal of point 1 
								// towards point 2
								// seach if vv1n et vv2n sont suffisamment dans la bonne direction.
								cur_cos = ptn[0] * ptn2obs[0] + ptn[1] * ptn2obs[1] + ptn[2] * ptn2obs[2];
								cur_cos2 = ABnorm[0] * ptn[0] + ABnorm[1] * ptn[1] + ABnorm[2] * ptn[2];

								if (ve < 10 && j < 3) {
								//	cout << "cur_cos=" << cur_cos << endl;
								//	cout << "cur_cos2=" << cur_cos2 << endl;
								}

								if (cur_cos > min_cos && cur_cos2 > min_cos )																	
								{
									// we have a candidate!
									// compute projected point 2 along vvn1 !

								
										
									projected_pt2[0] = pt[0] + ptn[0] * AB[0];
									projected_pt2[1] = pt[1] + ptn[1] * AB[1];
									projected_pt2[2] = pt[2] + ptn[2] * AB[2];
									AC[0] = projected_pt2[0] - pt[0];
									AC[1] = projected_pt2[1] - pt[1];
									AC[2] = projected_pt2[2] - pt[2];
									//tmp_dist = (float)sqrt(AC[0]*AC[0]+AC[1]*AC[1]+AC[2]*AC[2]);
									// well I am not sure I want this distance!!!
									tmp_dist = curr_dist;
									thicknesses.push_back(tmp_dist);
									cpt++;

								}
								else
								{
									tmp_dist = max_thickness;
								}
							}
							else
							{
								tmp_dist = max_thickness;
							}
							if (tmp_dist < min_dist) { min_dist = tmp_dist; }

						}

					}

					// if (ve<10){std::cout<<"New Scalar value at "<<ve<<"="<<newscalar<<std::endl;}	
					if (avg <= 1 || cpt == 0 )
					{

						newScalars->InsertTuple1(ve, min_dist);
					}
					else
					{
						/*if (ve < 10)
						{
						cout << "try to sort thicknesses"  << endl;
						}*/
						std::sort(thicknesses.begin(), thicknesses.end());
						double thick_avg = 0;
						double mavg = avg;// in some cases there are less thickness candidates thant avg => the avg thickness will be done using less than "avg" nr of candidates.
						if (cpt < avg) { mavg = cpt; }
						for (int i = 0; i < mavg; i++)
						{
							thick_avg += thicknesses.at(i);
						}
						thick_avg /= mavg;
						newScalars->InsertTuple1(ve, thick_avg);
						if (ve < 10)
						{
							//cout << "Avg thickness=" << thick_avg << ", max min thickness=" << min_dist << endl;
						}

					}


				}
				//std::cout<<"New Scalar computation done "<<std::endl;

				newScalars->SetName(mScalarName.c_str());
				// test if exists...
				//
				int exists = 0;

				// remove this scalar
				//this->GetPointData()->SetScalars(newScalars);
				mImpactedPD->GetPointData()->RemoveArray(mScalarName.c_str());
				mImpactedPD->GetPointData()->AddArray(newScalars);
				mImpactedPD->GetPointData()->SetActiveScalars(mScalarName.c_str());
				//g_active_scalar = 0;
				// 0 => depth
				// 1 =>	"Maximum_Curvature"
				// 2 => "Minimum_Curvature"
				// 3 => "Gauss_Curvature"
				// 4 => "Mean_Curvature"

				//modified = 1;
			}
			else
			{
				cout << "found no norms!" << endl;

			}


		}

		//cout << "camera and grid adjusted" << endl;
		if (impactedActor != observedActor) // very important because in the opposite case, we call this function WITHIN A LOOP on the actor list... and InitMui_ExistingArrays also calls a LOOP on the actor list!
		{
			cout << "thickness scalars between computed: now call initmui_ExistingArrays " << endl;
			this->Initmui_ExistingArrays();
			cout << "set ActiveScalarAndRender" << endl;
			this->Setmui_ActiveArrayAndRender(mScalarName.c_str(), VTK_DOUBLE, 1);
		}
	
		END_UNDO_SET();
}
	

		
	
}
void mqMorphoDigCore::BooleanOperation(vtkMDActor *actorA, vtkMDActor *actorB, int mode)
{
	cout << "Call Boolean operation" << endl;
	vtkSmartPointer<vtkMDActorCollection> newcoll = vtkSmartPointer<vtkMDActorCollection>::New();
	int modified = 0;
	if (actorA != NULL && actorB != NULL && actorA != actorB)
	{
		std::string action = "boolean operation";
		action.append(actorA->GetName().c_str());



		vtkPolyDataMapper *myMapperA = vtkPolyDataMapper::SafeDownCast(actorA->GetMapper());
		vtkPolyDataMapper *myMapperB = vtkPolyDataMapper::SafeDownCast(actorB->GetMapper());
		if (myMapperA != NULL && vtkPolyData::SafeDownCast(myMapperA->GetInput()) != NULL)
		{

			vtkSmartPointer<vtkPolyData> mPD_A = vtkSmartPointer<vtkPolyData>::New();
			mPD_A = myMapperA->GetInput();

			vtkSmartPointer<vtkPolyData> mPD_B = vtkSmartPointer<vtkPolyData>::New();
			mPD_B = myMapperB->GetInput();

			double ve_B_init_pos[3];;
			double ve_B_final_pos[3];

			double ve_A_init_pos[3];;
			double ve_A_final_pos[3];

			vtkSmartPointer<vtkMatrix4x4> Mat_A = actorA->GetMatrix();
			vtkSmartPointer<vtkMatrix4x4> Mat_B = actorB->GetMatrix();

			vtkSmartPointer<vtkPolyData> mPD_A_Moved = vtkSmartPointer<vtkPolyData>::New();
			mPD_A_Moved->DeepCopy(vtkPolyData::SafeDownCast(mPD_A));


			for (vtkIdType i = 0; i < mPD_A_Moved->GetNumberOfPoints(); i++) {
				// for every triangle 
				mPD_A_Moved->GetPoint(i, ve_A_init_pos);
				mqMorphoDigCore::TransformPoint(Mat_A, ve_A_init_pos, ve_A_final_pos);

				mPD_A_Moved->GetPoints()->SetPoint((vtkIdType)i, ve_A_final_pos);
			}

			vtkSmartPointer<vtkPolyData> mPD_B_Moved = vtkSmartPointer<vtkPolyData>::New();
			mPD_B_Moved->DeepCopy(vtkPolyData::SafeDownCast(mPD_B));
			for (vtkIdType i = 0; i < mPD_B_Moved->GetNumberOfPoints(); i++) {
				// for every triangle 
				mPD_B_Moved->GetPoint(i, ve_B_init_pos);
				mqMorphoDigCore::TransformPoint(Mat_B, ve_B_init_pos, ve_B_final_pos);

				mPD_B_Moved->GetPoints()->SetPoint((vtkIdType)i, ve_B_final_pos);
			}
			VTK_CREATE(vtkMDActor, newactor);
			if (this->mui_BackfaceCulling == 0)
			{
				newactor->GetProperty()->BackfaceCullingOff();
			}
			else
			{
				newactor->GetProperty()->BackfaceCullingOn();
			}

			VTK_CREATE(vtkPolyDataMapper, newmapper);
			//newmapper->ImmediateModeRenderingOn();
			newmapper->SetColorModeToDefault();

			if (
				(this->mui_ActiveArray->DataType == VTK_INT || this->mui_ActiveArray->DataType == VTK_UNSIGNED_INT)
				&& this->mui_ActiveArray->NumComp == 1
				)
			{
				newmapper->SetScalarRange(0, this->Getmui_ActiveTagMap()->numTags - 1);
				newmapper->SetLookupTable(this->Getmui_ActiveTagMap()->TagMap);
			}
			else
			{
				newmapper->SetLookupTable(this->Getmui_ActiveColorMap()->ColorMap);
			}

			newmapper->ScalarVisibilityOn();

			vtkSmartPointer<vtkBooleanOperationPolyDataFilter> Bfilter = vtkSmartPointer<vtkBooleanOperationPolyDataFilter>::New();
			//Sfilter->SetInputData(vtkPolyData::SafeDownCast(mymapper->GetInput()));
			Bfilter->SetInputData(0, mPD_A_Moved);
			Bfilter->SetInputData(1, mPD_B_Moved);
			if (mode ==0)
			{
				Bfilter->SetOperationToDifference();
			}else if (mode == 1)
			{
				Bfilter->SetOperationToUnion();
			}
			else 
			{
				Bfilter->SetOperationToIntersection();
			}
			Bfilter->Update();
		
			vtkSmartPointer<vtkCleanPolyData> cleanPolyDataFilter = vtkSmartPointer<vtkCleanPolyData>::New();
			cleanPolyDataFilter->SetInputData(Bfilter->GetOutput());
			cleanPolyDataFilter->PieceInvariantOff();
			cleanPolyDataFilter->ConvertLinesToPointsOff();
			cleanPolyDataFilter->ConvertPolysToLinesOff();
			cleanPolyDataFilter->ConvertStripsToPolysOff();
			cleanPolyDataFilter->PointMergingOn();
			cleanPolyDataFilter->Update();
			VTK_CREATE(vtkPolyData, myData);

			myData = cleanPolyDataFilter->GetOutput();

			cout << "Boolean output: nv=" << myData->GetNumberOfPoints() << endl;
			//newmapper->SetInputConnection(delaunay3D->GetOutputPort());

			newmapper->SetInputData(myData);



			double color[4] = { 0.5, 0.5, 0.5, 1 };
			actorA->GetmColor(color);
			newactor->SetmColor(color);

			newactor->SetMapper(newmapper);
			newactor->SetSelected(0);

			newactor->SetName(actorA->GetName() + "_boolean");
			cout << "try to add new actor=" << endl;
			if (myData->GetNumberOfPoints() > 10)
			{
				newcoll->AddTmpItem(newactor);
				modified = 1;
			}
			else
			{
				QMessageBox msgBox;
				msgBox.setText("No boolean object created. Output contained "+ QString::number(myData->GetNumberOfPoints())+" points");
				msgBox.exec();
				return;
			}
		}


	}

	if (modified == 1)
	{
		newcoll->InitTraversal();
		vtkIdType num = newcoll->GetNumberOfItems();
		for (vtkIdType i = 0; i < num; i++)
		{
			cout << "try to get next actor from newcoll:" << i << endl;
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(newcoll->GetNextActor());
			myActor->SetColorProperties(this->mui_Ambient, this->mui_Diffuse, this->mui_Specular, this->mui_SpecularPower);
			myActor->SetDisplayMode(this->mui_DisplayMode);
			this->getActorCollection()->AddItem(myActor);
			emit this->actorsMightHaveChanged();
			std::string action = "Boolean resulting object added: " + myActor->GetName();
			int mCount = BEGIN_UNDO_SET(action);
			this->getActorCollection()->CreateLoadUndoSet(mCount, 1);
			END_UNDO_SET();
		}
		//cout << "camera and grid adjusted" << endl;
		cout << "new actor(s) added" << endl;
		this->Initmui_ExistingArrays();

		cout << "Set actor collection changed" << endl;
		this->getActorCollection()->SetChanged(1);
		cout << "Actor collection changed" << endl;

		this->AdjustCameraAndGrid();
		cout << "Camera and grid adjusted" << endl;

		if (this->Getmui_AdjustLandmarkRenderingSize() == 1)
		{
			this->UpdateLandmarkSettings();
		}
		this->Render();
	}


}
void mqMorphoDigCore::RecomputePointNormals(vtkSmartPointer<vtkPolyData> mesh)
{
	cout << "Start recompute point normals" << endl;

	auto pointNorms = vtkFloatArray::SafeDownCast(mesh->GetPointData()->GetNormals());
	vtkSmartPointer<vtkFloatArray> newPointNorms = vtkSmartPointer<vtkFloatArray>::New();
	if (pointNorms == NULL)
	{
		newPointNorms->SetNumberOfComponents(3);
		newPointNorms->SetNumberOfTuples(mesh->GetNumberOfPoints());
	}
	else
	{
		newPointNorms->DeepCopy(pointNorms);
	}
	

	auto cellNorms = vtkFloatArray::SafeDownCast(mesh->GetCellData()->GetNormals());
	if (cellNorms != NULL)
	{
		for (vtkIdType ve = 0; ve < mesh->GetNumberOfPoints(); ve++)
		{

			vtkSmartPointer<vtkIdList> cellPts = vtkSmartPointer<vtkIdList>::New();
			vtkSmartPointer<vtkIdList> ptCells = vtkSmartPointer<vtkIdList>::New();
			mesh->GetPointCells(ve, ptCells);
			double norm[3] = { 0,0,0 };
			for (vtkIdType j = 0; j<ptCells->GetNumberOfIds(); j++)
			{
				double *cn = cellNorms->GetTuple3(ptCells->GetId(j));
				norm[0] += cn[0];
				norm[1] += cn[1];
				norm[2] += cn[2];

			}
			if (norm[0] != 0 || norm[1] != 0 || norm[2] != 0)
			{
				if (ve < 10)
				{
					//cout << "Normalize norm" << endl;
				}
				vtkMath::Normalize(norm);

			}
			else
			{
				norm[2] = 1;
			}
			newPointNorms->SetTuple3(ve, norm[0], norm[1], norm[2]);			
			
		}
		mesh->GetPointData()->SetNormals(newPointNorms);
	}
}
void mqMorphoDigCore::RecomputeCellNormals(vtkSmartPointer<vtkPolyData> mesh)
{
	cout << "Start recompute cell normals" << endl;
	vtkSmartPointer<vtkFloatArray>cellNorms = vtkFloatArray::SafeDownCast(mesh->GetCellData()->GetNormals());
	vtkSmartPointer<vtkFloatArray> newCellNorms = vtkSmartPointer<vtkFloatArray>::New();
	
	if (cellNorms == NULL)
	{
		newCellNorms->SetNumberOfComponents(3);
		newCellNorms->SetNumberOfTuples(mesh->GetNumberOfCells());
	}
	else
	{
		newCellNorms->DeepCopy(cellNorms);
	}

	
	
		for (vtkIdType ce = 0; ce < mesh->GetNumberOfCells(); ce++)
		{

			vtkSmartPointer<vtkIdList> cellPts = vtkSmartPointer<vtkIdList>::New();
			
			mesh->GetCellPoints(ce, cellPts);
			double A[3] = { 0,0,0 };
			double B[3] = { 0,0,0 };
			double C[3] = { 0,0,0 };
			double norm[3] = { 0,0,0 };
			for (vtkIdType j = 0; j<cellPts->GetNumberOfIds(); j++)
			{
				double *pt = mesh->GetPoint(cellPts->GetId(j));
				if (j == 0) { A[0] = pt[0]; A[1] = pt[1]; A[2] = pt[2]; }
				if (j == 1) { B[0] = pt[0]; B[1] = pt[1]; B[2] = pt[2]; }
				if (j == 2) { C[0] = pt[0]; C[1] = pt[1]; C[2] = pt[2]; }				

			}
			if (cellPts->GetNumberOfIds() >= 3)
			{
				if (ce < 10)
				{
					//cout << "Triangle compute normal" << endl;
				}
				vtkTriangle::ComputeNormal(A, B, C, norm);
			}			
			else
			{
				norm[2] = 1;
			}
			
			newCellNorms->SetTuple3(ce, norm[0], norm[1], norm[2]);

		}

		mesh->GetCellData()->SetNormals(newCellNorms);
	
}
void mqMorphoDigCore::ShrinkWrapIterative(QString scalarName, int mode, int iteration, double stopCriterion, double radius, double angularLimit, double maxStepAmplitude, vtkMDActor *impactedActor, vtkMDActor* observedActor, int smoothWrapping)
{
	cout << "Call ShrinkWrap" << endl;
	if (radius <= 0) { radius = 1; }
	if (maxStepAmplitude <= 0) { maxStepAmplitude = radius; }
	if (maxStepAmplitude > radius) { maxStepAmplitude = radius; }

	vtkSmartPointer<vtkMDActorCollection> newcoll = vtkSmartPointer<vtkMDActorCollection>::New();
	int modified = 0;
	if (impactedActor != NULL && observedActor != NULL && impactedActor != observedActor)
	{
		
		
		//int Count = BEGIN_UNDO_SET(action);

		std::string mScalarName = "ShrinkWrap_Last_Displacement";
		if (scalarName.length() > 0)
		{
			mScalarName = scalarName.toStdString();
		}
		//impactedActor->SaveState(Count, QString(mScalarName.c_str()));

		vtkPolyDataMapper *myImpactedMapper = vtkPolyDataMapper::SafeDownCast(impactedActor->GetMapper());
		vtkPolyDataMapper *myObservedMapper = vtkPolyDataMapper::SafeDownCast(observedActor->GetMapper());
		if (myImpactedMapper != NULL && vtkPolyData::SafeDownCast(myImpactedMapper->GetInput()) != NULL)
		{

			vtkSmartPointer<vtkPolyData> mImpactedPD = vtkSmartPointer<vtkPolyData>::New();
			mImpactedPD = myImpactedMapper->GetInput();

			vtkSmartPointer<vtkPolyData> mObservedPD = vtkSmartPointer<vtkPolyData>::New();
			mObservedPD = myObservedMapper->GetInput();

			double init_pos[3];
			double final_pos[3];
			
			/*double ve_imp_init_pos[3];;
			double ve_imp_final_pos[3];
			double ven_imp_final_pos[3];;*/


			vtkSmartPointer<vtkMatrix4x4> impMat = impactedActor->GetMatrix();
			vtkSmartPointer<vtkMatrix4x4> obsMat = observedActor->GetMatrix();

			vtkSmartPointer<vtkPolyData> impactedMoved = vtkSmartPointer<vtkPolyData>::New();
			impactedMoved->DeepCopy(vtkPolyData::SafeDownCast(mImpactedPD));

			vtkSmartPointer<vtkPolyData> observedMoved = vtkSmartPointer<vtkPolyData>::New();
			observedMoved->DeepCopy(vtkPolyData::SafeDownCast(mObservedPD));
			for (vtkIdType i = 0; i < observedMoved->GetNumberOfPoints(); i++) {
				// for every triangle 
				observedMoved->GetPoint(i, init_pos);
				mqMorphoDigCore::TransformPoint(obsMat, 
					init_pos, final_pos);

				observedMoved->GetPoints()->SetPoint(i, final_pos);
			}
			// now transform the norms
			this->RecomputeCellNormals(observedMoved);
			this->RecomputePointNormals(observedMoved);
			
			



			vtkIdType impactedNumvert = myImpactedMapper->GetInput()->GetNumberOfPoints();



			for (vtkIdType i = 0; i < impactedMoved->GetNumberOfPoints(); i++) {
				// for every triangle 
				impactedMoved->GetPoint(i, init_pos);
				mqMorphoDigCore::TransformPoint(impMat, init_pos, final_pos);

				impactedMoved->GetPoints()->SetPoint(i, final_pos);
			}
			// now compute the norms!
			
			int ok = 1;
			int lmk_max = 10;
			int lmk_cpt = 0;
		for (vtkIdType it=0; it<iteration;it++)
		{
			cout << "ITERATIVE SHRINK WRAP ITERATION " << it << endl;
			// now transform the norms
			this->RecomputeCellNormals(impactedMoved);
			this->RecomputePointNormals(impactedMoved);
			
		
				vtkSmartPointer<vtkDoubleArray> newScalars =
					vtkSmartPointer<vtkDoubleArray>::New();
				newScalars->SetNumberOfComponents(1); //3d normals (ie x,y,z)
				newScalars->SetNumberOfTuples(impactedNumvert);
			
				vtkSmartPointer<vtkDoubleArray> smoothScalars =
					vtkSmartPointer<vtkDoubleArray>::New();
				smoothScalars->SetNumberOfComponents(1); //3d normals (ie x,y,z)
				smoothScalars->SetNumberOfTuples(impactedNumvert);
				double obs_pt[3] = { 0,0,1 };
				double imp_pt[3] = { 0,0,1 };
				double obs_norm[3];
				double imp_norm[3];

				double proj_pt[3] = { 0,0,1 };

				double *ptn;

				double min_cos = 0.342; // 70 degrees => Don't want to compute thickness using badly oriented vertices.
				if (angularLimit > 0 && angularLimit <= 180)
				{
					min_cos = cos(angularLimit*vtkMath::Pi() / 180);
				}
				cout << "min_cos=" << min_cos << endl;

				double cur_cos = 1.0; // compare ve1's normal and ve2's normal
				double cur_cos2 = 1.0; //compare  ve1's normal and vector between ve1 and ve2
				double AB[3];

				double ABnorm[3];

				double picked_value = 0;
				double min_dist = radius;
				double tmp_dist = 0;
				auto impactedNorms = vtkFloatArray::SafeDownCast(impactedMoved->GetPointData()->GetNormals());
				auto observedNorms = vtkFloatArray::SafeDownCast(observedMoved->GetPointData()->GetNormals());
				if (impactedNorms && observedNorms)
				{
					cout << "ITERATION "<< it<< ", We have found some norms" << endl;
					vtkSmartPointer<vtkKdTreePointLocator> kDTree =
						vtkSmartPointer<vtkKdTreePointLocator>::New();
					kDTree->SetDataSet(observedMoved);
					kDTree->BuildLocator();
					//first do only one loop => iterative shrink wrap should start here!
					vtkSmartPointer<vtkDoubleArray> displacementField = vtkSmartPointer<vtkDoubleArray>::New();
					
					displacementField->SetNumberOfComponents(3);
					displacementField->SetNumberOfTuples(impactedNumvert);

					for (vtkIdType ve2 = 0; ve2 < impactedNumvert; ve2++)
					{
						if ((ve2 % (int)(impactedNumvert / 10)) == 0)
						{
							//cout << "ve=" << ve2 << endl;
							emit iterativeShrinkWrapProgression((int)(100 * ve2 / impactedNumvert));
						}
						impactedMoved->GetPoint(ve2, imp_pt);
						ptn = impactedNorms->GetTuple(ve2);
						imp_norm[0] = ptn[0];
						imp_norm[1] = ptn[1];
						imp_norm[2] = ptn[2];
						// 2 choix poissibles 
						
						// Choix A: dans tous N plus proches. 
						// Garder que ceux qui sont dans la sphère
						// se diriger dans la direction de la normale d'un facteur correspondant à la moyenne des dist pondérés par cos2 

						// Bon faudrait quand même se diriger un peu vers le barycentre des 20 plus proches, ou faire une pondération qui prenne en compte la proximité (force de gravité)  ????? Sinon on n'y arrrive pas du tout.
						// il faut aussi à mon avis virer le filtre qui calcule les normales dans cette fonction, j'ai l'impression qu'il modifie la topologie sans prévenir... 

						//
						// Choix B: dans tous ces ponts vers lesquels on se dirige (utilisation du critère cos2)
						// repérer le plus proche (ou les N plus proches). 
						// se diriger vers lui d'un facteur cos2 (ou vers leur moyennen dans l'espace d'un facteur cos2)
						// le problème de cette approche et ques les trous se dépeuplent et que la sphère se déforme beaucoup.

						vtkSmartPointer<vtkIdList> observedNeighbours = vtkSmartPointer<vtkIdList>::New();



						
						double newscalar = maxStepAmplitude; // if we do not find neighbours, we will move as far as permitted.
						double currAmpl = 0;
						//double currDist = 0;
						
						// Stratégie A
						kDTree->FindClosestNPoints(20, imp_pt, observedNeighbours);
						vtkIdType toto = 0;
						double maxDist = 0;
						double sumDist = 0;
						double minDist = radius;
						double baryCentre[3] = { 0,0,0 };
						for (vtkIdType j = 0; j < observedNeighbours->GetNumberOfIds(); j++)
						{
							vtkIdType obsVerId = observedNeighbours->GetId(j);
							if (((int)ve2 % (int)(impactedNumvert / 10)) == 0 && j == 0)
							{
								//	cout << "first observed neighbour init_pos=" << init_pos[0] << "," << init_pos[1] << "," << init_pos[2] << endl;
							}
							observedMoved->GetPoint(obsVerId, obs_pt);
							baryCentre[0] += obs_pt[0];
							baryCentre[1] += obs_pt[1];
							baryCentre[2] += obs_pt[2];
							ptn = observedNorms->GetTuple3(obsVerId);
							obs_norm[0] = ptn[0];
							obs_norm[1] = ptn[1];
							obs_norm[2] = ptn[2];
							AB[0] = obs_pt[0] - imp_pt[0];
							AB[1] = obs_pt[1] - imp_pt[1];
							AB[2] = obs_pt[2] - imp_pt[2];
							double curr_dist = sqrt(AB[0] * AB[0] + AB[1] * AB[1] + AB[2] * AB[2]);
							sumDist += curr_dist;
							if (maxDist < curr_dist) { maxDist = curr_dist; }
							if (minDist > curr_dist) { minDist = curr_dist; }
						}
						// we have sumDist, minDist et maxDist

						for (vtkIdType j = 0; j < observedNeighbours->GetNumberOfIds(); j++)
						{
							vtkIdType obsVerId = observedNeighbours->GetId(j);
							if (((int)ve2 % (int)(impactedNumvert / 10)) == 0 && j == 0)
							{
								//	cout << "first observed neighbour init_pos=" << init_pos[0] << "," << init_pos[1] << "," << init_pos[2] << endl;
							}
							observedMoved->GetPoint(obsVerId, obs_pt);
							ptn = observedNorms->GetTuple3(obsVerId);
							obs_norm[0] = ptn[0];
							obs_norm[1] = ptn[1];
							obs_norm[2] = ptn[2];
							AB[0] = obs_pt[0] - imp_pt[0];
							AB[1] = obs_pt[1] - imp_pt[1];
							AB[2] = obs_pt[2] - imp_pt[2];
							double curr_dist = sqrt(AB[0] * AB[0] + AB[1] * AB[1] + AB[2] * AB[2]);							
							ABnorm[0] = 0; ABnorm[1] = 0; ABnorm[2] = 0;
							if (curr_dist > 0)
							{
								ABnorm[0] = AB[0] / curr_dist;
								ABnorm[1] = AB[1] / curr_dist;
								ABnorm[2] = AB[2] / curr_dist;
							}
							cur_cos = obs_norm[0] * imp_norm[0] + obs_norm[1] * imp_norm[1] + obs_norm[2] * imp_norm[2]; // we do not care about this one, or do we?
							cur_cos2 = -1 * (ABnorm[0] * imp_norm[0] + ABnorm[1] * imp_norm[1] + ABnorm[2] * imp_norm[2]);// the direction "forward" view angle < 							
						
							if ( curr_dist < radius)
							{
								if (lmk_cpt < lmk_max)
								{
								/*	this->CreateLandmark(imp_pt, imp_norm, 0);
									this->CreateLandmark(obs_pt, obs_norm, 1);*/
									lmk_cpt++;
								}
								toto = toto + 1;
								
								//cpt_candidates++;
								double factor = cur_cos2*curr_dist;
								
								if (((int)ve2 % (int)(impactedNumvert / 10)) == 0 && j == 0)
								{
								/*	cout << "cur_cos=" << cur_cos << endl;
									cout << "cur_cos2=" << cur_cos2 << endl;
									cout << "min_cos=" << min_cos << endl;
									cout << "curr_dist=" << curr_dist << endl;
									cout << "factor=" << factor << endl;
									cout << "ABnorm[0]=" << ABnorm[0] << "ABnorm[1]=" << ABnorm[1] << "ABnorm[2]=" << ABnorm[2] << endl;*/
								}		

								currAmpl += factor;


							}


						}
						if (toto > 0)
						{
							currAmpl /= toto;
							if (currAmpl > maxStepAmplitude) { currAmpl = maxStepAmplitude; }
							//cout << "REAL MOVE currampl=" << currAmpl << ", ve=" << ve2 << ", cpt=" << toto << endl;
						}						
						else
						{
							currAmpl = maxStepAmplitude;
							
						}




						if ((ve2 % (int)(impactedNumvert / 10)) == 0)
						{
							//cout << "currampl=" << currAmpl << ", ve=" << ve2 << endl;
						}
						//Stratégie B (non finie)
						/*kDTree->FindPointsWithinRadius(radius, imp_pt, observedNeighbours);
						vtkIdType cpt_realneighbors = 0;
						vtkIdType cpt_notrealneighbors = 0;
						vtkIdType cpt_candidates = 0;
						if (((int)ve2 % (int)(impactedNumvert / 10)) == 0)
						{
							//cout << "ve=" << ve2 << ", neighbours" << observedNeighbours->GetNumberOfIds() << ", imp_pt:" << imp_pt[0] << "," << imp_pt[1] << "," << imp_pt[2] << endl;
						}
						if (observedNeighbours->GetNumberOfIds() > 0)
						{
							cout << "ve=" << ve2 << ", KD tree neighbours:" << observedNeighbours->GetNumberOfIds() << ", imp_pt:" << imp_pt[0] << "," << imp_pt[1] << "," << imp_pt[2] << endl;
						}
						for (vtkIdType j = 0; j < observedNeighbours->GetNumberOfIds(); j++)
						{
							vtkIdType obsVerId = observedNeighbours->GetId(j);
							if (((int)ve2 % (int)(impactedNumvert / 10)) == 0 && j == 0)
							{
							//	cout << "first observed neighbour init_pos=" << init_pos[0] << "," << init_pos[1] << "," << init_pos[2] << endl;
							}
							observedMoved->GetPoint(obsVerId, obs_pt);
							ptn = observedNorms->GetTuple3(obsVerId);
							obs_norm[0] = ptn[0];
							obs_norm[1] = ptn[1];
							obs_norm[2] = ptn[2];
							AB[0] = obs_pt[0] - imp_pt[0];
							AB[1] = obs_pt[1] - imp_pt[1];
							AB[2] = obs_pt[2] - imp_pt[2];
							double curr_dist = sqrt(AB[0] * AB[0] + AB[1] * AB[1] + AB[2] * AB[2]);
							ABnorm[0] = 0; ABnorm[1] = 0; ABnorm[2] = 0;
							if (curr_dist > 0)
							{
								ABnorm[0] = AB[0] / curr_dist;
								ABnorm[1] = AB[1] / curr_dist;
								ABnorm[2] = AB[2] / curr_dist;
							}
							cur_cos = obs_norm[0] * imp_norm[0] + obs_norm[1] * imp_norm[1] + obs_norm[2] * imp_norm[2]; // we do not care about this one, or do we?
							cur_cos2 = -1*(ABnorm[0] * imp_norm[0] + ABnorm[1] * imp_norm[1] + ABnorm[2] * imp_norm[2]);// the direction "forward" view angle < 
							if (lmk_cpt < lmk_max)
							{
								this->CreateLandmark(imp_pt, imp_norm, 0);
								this->CreateLandmark(obs_pt, obs_norm, 1);
								lmk_cpt++;
							}
							
							if (curr_dist > radius)
							{
								cpt_notrealneighbors++;
							}
							else
							{
								cpt_realneighbors++;
							}
							if (cur_cos2 > min_cos && curr_dist < radius)
							{
								 

								double factor = cur_cos2*curr_dist;
								cout << "cur_cos2=" << cur_cos2 << "," << "curr_dist=" << curr_dist <<  ", factor=" << factor << endl;

								if (((int)ve2 % (int)(impactedNumvert / 10)) == 0 && j == 0)
								{
									cout << "cur_cos=" << cur_cos << endl;
									cout << "cur_cos2=" << cur_cos2 << endl;
									cout << "min_cos=" << min_cos << endl;
									cout << "curr_dist=" << curr_dist << endl;
									cout << "factor=" << factor << endl;
									cout << "ABnorm[0]=" << ABnorm[0] << "ABnorm[1]=" << ABnorm[1] << "ABnorm[2]=" << ABnorm[2] << endl;

								}
								// we have a candidate!
								// compute projected point 2 along vvn1 !



								cpt_candidates++;

								currAmpl += factor;


							}


						}
						if (cpt_candidates > 0)
						{
							currAmpl /= cpt_candidates;
							if (currAmpl > maxStepAmplitude) { currAmpl = maxStepAmplitude; }
							cout << "REAL MOVE currampl=" << currAmpl << ", ve=" << ve2 << ", cpt="<< cpt_candidates << endl;
						}
						else if (cpt_realneighbors == 0)
						{
							if ((ve2 % (int)(impactedNumvert / 10)) == 0)
							{
								//cout << "currampl=" << currAmpl << ", ve=" << ve2 << "NO REAL NEIGHBOUR FOUND" << endl;
							}
							// this case should not happen but should be totally trust the KD Tree ? 
							currAmpl = maxStepAmplitude; 

						}
						else
						{
							currAmpl = 0; // case where there are real neighbors around but no one is satisfying the desirfed angular condition(s).
							if ((ve2 % (int)(impactedNumvert / 10)) == 0)
							{
								//cout << "currampl=" << currAmpl << ", ve=" << ve2 << "Neigh exist but are not satisfactory" << endl;
							}
							cout << "NO CANDIDATE currampl=" << currAmpl << ", ve=" << ve2 << ", cpt=" << cpt_candidates << endl;
						}




						if ((ve2 % (int)(impactedNumvert / 10)) == 0)
						{
							//cout << "currampl=" << currAmpl << ", ve=" << ve2 << endl;
						}

						*/

						newScalars->InsertTuple1(ve2, currAmpl);




					}
					if (smoothWrapping == 1)
					{

						smoothScalars = this->scalarSmooth(newScalars, impactedMoved, radius, 0, 0, 0, 2);
						newScalars->DeepCopy(smoothScalars);

					}



					//std::cout << "New Scalar computation done " << std::endl;
					for (vtkIdType i = 0; i < impactedMoved->GetNumberOfPoints(); i++) {
						// for every triangle 
						impactedMoved->GetPoint(i, imp_pt);
						double ampli = newScalars->GetTuple1(i);
						
						ptn = impactedNorms->GetTuple(i);
						imp_norm[0] = ptn[0];
						imp_norm[1] = ptn[1];
						imp_norm[2] = ptn[2];
						proj_pt[0] = imp_pt[0] - imp_norm[0] * ampli;
						proj_pt[1] = imp_pt[1] - imp_norm[1] * ampli;
						proj_pt[2] = imp_pt[2] - imp_norm[2] * ampli;
						//if (ampli>0){ cout << "i=" << i << ", ampli=" << ampli << endl; }
						if ((i % (int)(impactedNumvert / 10)) == 0)
						{
							//	cout << "i=" << i << endl;
							//	cout << "i="<<i<<", ampli="<<ampli << endl;
							//	cout << "ven_imp_final_pos[0]=" << ven_imp_final_pos[0] << endl;
							//	cout << "ve_imp_init_pos[0]=" << ve_imp_init_pos[0] << ", ve_imp_init_pos[1]=" << ve_imp_init_pos[1] << "ve_imp_init_pos[2]=" << ve_imp_init_pos[2] << endl;

							//	cout << "ve_imp_final_pos[0]=" << ve_imp_final_pos[0] << ", ve_imp_final_pos[1]=" << ve_imp_final_pos[1] << "ve_imp_final_pos[2]=" << ve_imp_final_pos[2] << endl;

						}
						impactedMoved->GetPoints()->SetPoint((vtkIdType)i, proj_pt);
					}
					// now compute the norms!
					this->RecomputeCellNormals(impactedMoved);
					this->RecomputePointNormals(impactedMoved);
					


					newScalars->SetName(mScalarName.c_str());
					// test if exists...
					//
					int exists = 0;

					// remove this scalar
					//this->GetPointData()->SetScalars(newScalars);
					impactedMoved->GetPointData()->RemoveArray(mScalarName.c_str());
					impactedMoved->GetPointData()->AddArray(newScalars);
					impactedMoved->GetPointData()->SetActiveScalars(mScalarName.c_str());

				}// end iteration loop
				else
				{
					ok = 0;
				}
				

				
			}// end iteration loop
			if (ok == 1)
			{
				// Not sure I create a new actor. Yes actually... 
				VTK_CREATE(vtkMDActor, newactor);
				if (this->mui_BackfaceCulling == 0)
				{
					newactor->GetProperty()->BackfaceCullingOff();
				}
				else
				{
					newactor->GetProperty()->BackfaceCullingOn();
				}

				VTK_CREATE(vtkPolyDataMapper, newmapper);
				//newmapper->ImmediateModeRenderingOn();
				newmapper->SetColorModeToDefault();

				if (
					(this->mui_ActiveArray->DataType == VTK_INT || this->mui_ActiveArray->DataType == VTK_UNSIGNED_INT)
					&& this->mui_ActiveArray->NumComp == 1
					)
				{
					newmapper->SetScalarRange(0, this->Getmui_ActiveTagMap()->numTags - 1);
					newmapper->SetLookupTable(this->Getmui_ActiveTagMap()->TagMap);
				}
				else
				{
					newmapper->SetLookupTable(this->Getmui_ActiveColorMap()->ColorMap);
				}

				newmapper->ScalarVisibilityOn();




				vtkSmartPointer<vtkCleanPolyData> cleanPolyDataFilter = vtkSmartPointer<vtkCleanPolyData>::New();
				//@@ TO CHANGE
				cleanPolyDataFilter->SetInputData(impactedMoved);
				//@@ TO CHANGE

				cleanPolyDataFilter->PieceInvariantOff();
				cleanPolyDataFilter->ConvertLinesToPointsOff();
				cleanPolyDataFilter->ConvertPolysToLinesOff();
				cleanPolyDataFilter->ConvertStripsToPolysOff();
				cleanPolyDataFilter->PointMergingOn();
				cleanPolyDataFilter->Update();



				VTK_CREATE(vtkPolyData, myData);

				myData = cleanPolyDataFilter->GetOutput();

				cout << "shrink wrap output: nv=" << myData->GetNumberOfPoints() << endl;
				//newmapper->SetInputConnection(delaunay3D->GetOutputPort());

				newmapper->SetInputData(myData);



				double color[4] = { 0.5, 0.5, 0.5, 1 };
				impactedActor->GetmColor(color);
				newactor->SetmColor(color);

				newactor->SetMapper(newmapper);
				newactor->SetSelected(0);

				newactor->SetName(impactedActor->GetName() + "_shrinkwrap");
				cout << "try to add new actor=" << endl;
				newcoll->AddTmpItem(newactor);
				modified = 1;
			}
		}

	}

	if (modified == 1)
	{
		newcoll->InitTraversal();
		vtkIdType num = newcoll->GetNumberOfItems();
		for (vtkIdType i = 0; i < num; i++)
		{
			cout << "try to get next actor from newcoll:" << i << endl;
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(newcoll->GetNextActor());
			myActor->SetColorProperties(this->mui_Ambient, this->mui_Diffuse, this->mui_Specular, this->mui_SpecularPower);
			myActor->SetDisplayMode(this->mui_DisplayMode);
			this->getActorCollection()->AddItem(myActor);
			emit this->actorsMightHaveChanged();
			std::string action = "Iterative shrinked wrapped object added: " + myActor->GetName();
			int mCount = BEGIN_UNDO_SET(action);
			this->getActorCollection()->CreateLoadUndoSet(mCount, 1);
			END_UNDO_SET();
		}
		//cout << "camera and grid adjusted" << endl;
		cout << "new actor(s) added" << endl;
		this->Initmui_ExistingArrays();

		cout << "Set actor collection changed" << endl;
		this->getActorCollection()->SetChanged(1);
		cout << "Actor collection changed" << endl;

		this->AdjustCameraAndGrid();
		cout << "Camera and grid adjusted" << endl;

		if (this->Getmui_AdjustLandmarkRenderingSize() == 1)
		{
			this->UpdateLandmarkSettings();
		}
		this->Render();
	}



}
void mqMorphoDigCore::ShrinkWrap(int iteration, double relaxation, vtkMDActor *impactedActor, vtkMDActor* observedActor)
{
	cout << "Call ShrinkWrap" << endl;
	vtkSmartPointer<vtkMDActorCollection> newcoll = vtkSmartPointer<vtkMDActorCollection>::New();
	int modified = 0;
	if (impactedActor != NULL && observedActor != NULL && impactedActor != observedActor)
	{
		std::string action = "shrink wrap";
		action.append(impactedActor->GetName().c_str());
	

		
		vtkPolyDataMapper *myImpactedMapper = vtkPolyDataMapper::SafeDownCast(impactedActor->GetMapper());
		vtkPolyDataMapper *myObservedMapper = vtkPolyDataMapper::SafeDownCast(observedActor->GetMapper());
		if (myImpactedMapper != NULL && vtkPolyData::SafeDownCast(myImpactedMapper->GetInput()) != NULL)
		{

			vtkSmartPointer<vtkPolyData> mImpactedPD = vtkSmartPointer<vtkPolyData>::New();
			mImpactedPD = myImpactedMapper->GetInput();

			vtkSmartPointer<vtkPolyData> mObservedPD = vtkSmartPointer<vtkPolyData>::New();
			mObservedPD = myObservedMapper->GetInput();
	
			double ve_obs_init_pos[3];;
			double ve_obs_final_pos[3];
			
			double ve_imp_init_pos[3];;
			double ve_imp_final_pos[3];
			
			vtkSmartPointer<vtkMatrix4x4> impMat = impactedActor->GetMatrix();
			vtkSmartPointer<vtkMatrix4x4> obsMat = observedActor->GetMatrix();

			vtkSmartPointer<vtkPolyData> impactedMoved = vtkSmartPointer<vtkPolyData>::New();
			impactedMoved->DeepCopy(vtkPolyData::SafeDownCast(mImpactedPD));


			for (vtkIdType i = 0; i < impactedMoved->GetNumberOfPoints(); i++) {
				// for every triangle 
				impactedMoved->GetPoint(i, ve_imp_init_pos);
				mqMorphoDigCore::TransformPoint(impMat, ve_imp_init_pos, ve_imp_final_pos);

				impactedMoved->GetPoints()->SetPoint((vtkIdType)i, ve_imp_final_pos);
			}

			vtkSmartPointer<vtkPolyData> observedMoved = vtkSmartPointer<vtkPolyData>::New();
			observedMoved->DeepCopy(vtkPolyData::SafeDownCast(mObservedPD));
			for (vtkIdType i = 0; i < observedMoved->GetNumberOfPoints(); i++) {
				// for every triangle 
				observedMoved->GetPoint(i, ve_obs_init_pos);
				mqMorphoDigCore::TransformPoint(obsMat, ve_obs_init_pos, ve_obs_final_pos);

				observedMoved->GetPoints()->SetPoint((vtkIdType)i, ve_obs_final_pos);
			}
				VTK_CREATE(vtkMDActor, newactor);
				if (this->mui_BackfaceCulling == 0)
				{
					newactor->GetProperty()->BackfaceCullingOff();
				}
				else
				{
					newactor->GetProperty()->BackfaceCullingOn();
				}

				VTK_CREATE(vtkPolyDataMapper, newmapper);
				//newmapper->ImmediateModeRenderingOn();
				newmapper->SetColorModeToDefault();

				if (
					(this->mui_ActiveArray->DataType == VTK_INT || this->mui_ActiveArray->DataType == VTK_UNSIGNED_INT)
					&& this->mui_ActiveArray->NumComp == 1
					)
				{
					newmapper->SetScalarRange(0, this->Getmui_ActiveTagMap()->numTags - 1);
					newmapper->SetLookupTable(this->Getmui_ActiveTagMap()->TagMap);
				}
				else
				{
					newmapper->SetLookupTable(this->Getmui_ActiveColorMap()->ColorMap);
				}

				newmapper->ScalarVisibilityOn();

				vtkSmartPointer<vtkSmoothPolyDataFilter> Sfilter = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
				//Sfilter->SetInputData(vtkPolyData::SafeDownCast(mymapper->GetInput()));
				Sfilter->SetInputData(0, impactedMoved);
				Sfilter->SetInputData(1, observedMoved);
				Sfilter->SetNumberOfIterations(iteration);
				Sfilter->SetRelaxationFactor(relaxation);
				Sfilter->Update();

				vtkSmartPointer<vtkCleanPolyData> cleanPolyDataFilter = vtkSmartPointer<vtkCleanPolyData>::New();
				cleanPolyDataFilter->SetInputData(Sfilter->GetOutput());
				cleanPolyDataFilter->PieceInvariantOff();
				cleanPolyDataFilter->ConvertLinesToPointsOff();
				cleanPolyDataFilter->ConvertPolysToLinesOff();
				cleanPolyDataFilter->ConvertStripsToPolysOff();
				cleanPolyDataFilter->PointMergingOn();
				cleanPolyDataFilter->Update();
				VTK_CREATE(vtkPolyData, myData);

				myData = cleanPolyDataFilter->GetOutput();

				cout << "shrink wrap output: nv=" << myData->GetNumberOfPoints() << endl;
				//newmapper->SetInputConnection(delaunay3D->GetOutputPort());

				newmapper->SetInputData(myData);

				

				double color[4] = { 0.5, 0.5, 0.5, 1 };
				impactedActor->GetmColor(color);
				newactor->SetmColor(color);

				newactor->SetMapper(newmapper);
				newactor->SetSelected(0);

				newactor->SetName(impactedActor->GetName() + "_shrinkwrap");
				cout << "try to add new actor=" << endl;
				newcoll->AddTmpItem(newactor);
				modified = 1; 
		}

		
	}

	if (modified == 1)
	{
		newcoll->InitTraversal();
		vtkIdType num = newcoll->GetNumberOfItems();
		for (vtkIdType i = 0; i < num; i++)
		{
			cout << "try to get next actor from newcoll:" << i << endl;
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(newcoll->GetNextActor());
			myActor->SetColorProperties(this->mui_Ambient, this->mui_Diffuse, this->mui_Specular, this->mui_SpecularPower);
			myActor->SetDisplayMode(this->mui_DisplayMode);
			this->getActorCollection()->AddItem(myActor);
			emit this->actorsMightHaveChanged();
			std::string action = "Shrinked wrapped object added: " + myActor->GetName();
			int mCount = BEGIN_UNDO_SET(action);
			this->getActorCollection()->CreateLoadUndoSet(mCount, 1);
			END_UNDO_SET();
		}
		//cout << "camera and grid adjusted" << endl;
		cout << "new actor(s) added" << endl;
		this->Initmui_ExistingArrays();

		cout << "Set actor collection changed" << endl;
		this->getActorCollection()->SetChanged(1);
		cout << "Actor collection changed" << endl;

		this->AdjustCameraAndGrid();
		cout << "Camera and grid adjusted" << endl;

		if (this->Getmui_AdjustLandmarkRenderingSize() == 1)
		{
			this->UpdateLandmarkSettings();
		}
		this->Render();
	}


}
std::vector<std::string> mqMorphoDigCore::getActorNames()
{
	std::vector<std::string> myList;
	this->ActorCollection->InitTraversal();
	vtkIdType num = this->ActorCollection->GetNumberOfItems();
	int modified = 0;
	for (vtkIdType i = 0; i < num; i++)
	{
		cout << "Get actor name :" << i << endl;
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		myList.push_back(myActor->GetName());
	}
	return myList;
}

vtkMDActor* mqMorphoDigCore::getFirstActorFromName(QString actorName)
{
	this->ActorCollection->InitTraversal();
	vtkIdType num = this->ActorCollection->GetNumberOfItems();
	int modified = 0;
	for (vtkIdType i = 0; i < num; i++)
	{
		cout << "Get first actor from name:" << i << endl;
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetName().compare(actorName.toStdString())==0)
		{
			return myActor;
		}
	}


	return NULL;
}
void mqMorphoDigCore::scalarsThickness(double max_thickness, int smooth_normals, int avg, QString scalarName, double angularLimit )
{
	cout << "thickness scalars start " << endl;
	std::string mScalarName = "Thickness";
	if (scalarName.length() >0)
	{
		mScalarName = scalarName.toStdString();
	}
	
	this->ActorCollection->InitTraversal();
	vtkIdType num = this->ActorCollection->GetNumberOfItems();
	int modified = 0;
	for (vtkIdType i = 0; i < num; i++)
	{
		cout << "scalarsThickness:" << i << endl;
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor!=NULL && myActor->GetSelected() == 1)
		{
			myActor->SetSelected(0);
			// here we can call : 
			cout << "thickness scalars between called " << endl;
			this->scalarsThicknessBetween( max_thickness,  smooth_normals,  avg,  scalarName,  myActor, myActor, angularLimit);
			modified = 1;

		}
	}
	if (modified == 1)
	{

		//cout << "camera and grid adjusted" << endl;
		cout << "thickness within scalars computed, call initExistingArrays " << endl;
		this->Initmui_ExistingArrays();
		cout << "Active scalars and render" << endl;
		this->Setmui_ActiveArrayAndRender(mScalarName.c_str(), VTK_DOUBLE, 1);
		
	}
}
void mqMorphoDigCore::scalarsComplexity(double localAreaLimit, int customLocalAreaLimit, QString scalarName, int mode)
{
	//mode = 0: convex hull area ratio ( surface_area / surface_area_convex_hull ) 
	//mode = 1: convex hull shape index (sqrt_surface_area / (cbrt_volume_convex_hull*2.199085233)
	//mode = 2: local area / sphere area (surface_area / surface_area_sphere;
	//mode = 3: local sphere shape index ( sqrt_surface_area / (cbrt_volume_sphere*2.199085233)
	double defaultSearchSize = this->ActorCollection->GetBoundingBoxLength()/40;
	cout << "COmplexity scalars start " << endl;
	std::string mScalarName = "Complexity";
	if (scalarName.length() >0)
	{
		mScalarName = scalarName.toStdString();
	}

	this->ActorCollection->InitTraversal();
	vtkIdType num = this->ActorCollection->GetNumberOfItems();
	int modified = 0;
	for (vtkIdType i = 0; i < num; i++)
	{
		cout << "scalarsComplexity:" << i << endl;
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			double searchSize = defaultSearchSize;
			
			if (customLocalAreaLimit == 1 && localAreaLimit > 0)
			{
				searchSize = localAreaLimit;
				cout << "Custom search size :" << searchSize << endl;
			}
			else
			{
				
				searchSize = myActor->GetXYZAvgPCLength()/18; // looks like a reasonable neighbourhood sphere radius size.
				cout << "Search size based on myActor GetXYZAvgPCLength / 18:" << searchSize << endl;
			}

			cout << "searchSize=" << searchSize;
			myActor->SetSelected(0);
			// here we can call : 
			cout << "scalars complexity start" << endl;
			std::string action = "Compute complexity for ";
			action.append(myActor->GetName().c_str());
			int Count = BEGIN_UNDO_SET(action);
			std::string mScalarName = "Complexity";
			if (scalarName.length() > 0)
			{
				mScalarName = scalarName.toStdString();
			}
			myActor->SaveState(Count, QString(mScalarName.c_str()));
			vtkPolyDataMapper *myMapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
			if (myMapper != NULL && vtkPolyData::SafeDownCast(myMapper->GetInput()) != NULL)
			{

				vtkSmartPointer<vtkPolyData> mPD = vtkSmartPointer<vtkPolyData>::New();
				mPD = myMapper->GetInput();
				double numvert = mPD->GetNumberOfPoints();


				vtkSmartPointer<vtkDoubleArray> newScalars =
					vtkSmartPointer<vtkDoubleArray>::New();

				newScalars->SetNumberOfComponents(1); //3d normals (ie x,y,z)
				newScalars->SetNumberOfTuples(numvert);
				
				double ve_pos[3];
				vtkSmartPointer<vtkKdTreePointLocator> kDTree =
					vtkSmartPointer<vtkKdTreePointLocator>::New();
				kDTree->SetDataSet(mPD);

				kDTree->BuildLocator();
			
				for (vtkIdType i = 0; i < numvert; i++) {
					clock_t t0, t1, tt0, tt1;
					tt0 = clock();
					if (i % 1000 == 0)
					{
						cout << "complexity, vertex:" << i << endl;
					}
					// for every triangle 
					mPD->GetPoint(i, ve_pos);
					double currentComplexity = 0;
					if ((i % (int)(numvert / 100)) == 0)
					{
						emit complexityProgression((int)(100 * i / numvert));
					}
					vtkSmartPointer<vtkIdList> observedNeighbours = vtkSmartPointer<vtkIdList>::New();
					double Radius = searchSize;

					
					//double sec0;
					double ssec0;
					//mode = 0: convex hull area ratio
					//mode = 1: convex hull shape index
					t0 = clock();
					kDTree->FindPointsWithinRadius(searchSize, ve_pos, observedNeighbours);
					t1 = clock();
					if (i % 1000 == 0)
					{
						cout << "number of neighbours:" << observedNeighbours->GetNumberOfIds() << endl;
					//	sec0 = (double)(t1 - t0) / CLOCKS_PER_SEC;
					//	cout << "sec0=" << sec0 << endl;
					}
					//cout << "mode = " << mode << endl;
					// now extract surface
					//compute surface
					int printmode = 0;
					if (i%1000 ==0)
					{
						//printmode = 1;
					}
					if (Radius <=0)
					{
						Radius = 1;
					}
					currentComplexity = this->ComputeComplexity(mPD, observedNeighbours,Radius,  mode, printmode);
					if (i % 1000 == 0)
					{
						cout << "currentComplexity:" << currentComplexity << endl;
					}

					newScalars->InsertTuple1(i, currentComplexity);
					tt1 = clock();
				
					if (i % 1000 == 0)
					{
						ssec0 = (double)(tt1 - tt0) / CLOCKS_PER_SEC;
						cout << "Total time for 1 vertex:" << ssec0 << endl;
						
					}
				}

				newScalars->SetName(mScalarName.c_str());
				// test if exists...
			
				// remove this scalar
				//this->GetPointData()->SetScalars(newScalars);
				mPD->GetPointData()->RemoveArray(mScalarName.c_str());
				mPD->GetPointData()->AddArray(newScalars);
				mPD->GetPointData()->SetActiveScalars(mScalarName.c_str());

				
			}

			

		END_UNDO_SET(); 
			//@@ ok!
		
		// test if exists...
		//
	
		// remove this scalar
		
			modified = 1;

		}
	}
	if (modified == 1)
	{

		//cout << "camera and grid adjusted" << endl;
		cout << "thickness within scalars computed, call initExistingArrays " << endl;
		this->Initmui_ExistingArrays();
		cout << "Active scalars and render" << endl;
		this->Setmui_ActiveArrayAndRender(mScalarName.c_str(), VTK_DOUBLE, 1);

	}
}

double mqMorphoDigCore::ComputeComplexity(vtkSmartPointer<vtkPolyData> mPD, vtkSmartPointer<vtkIdList> list, double sphere_radius, int mode, int printmode)
{
	clock_t t1, t2,t3,t4,t5;
	double sec1, sec2, sec3, sec4;
	
	//mode = 0: convex hull area ratio ( surface_area / surface_area_convex_hull ) 
	//mode = 1: convex hull shape index (sqrt_surface_area / (cbrt_volume_convex_hull*2.199085233)
	//mode = 2: local area / sphere area (surface_area / surface_area_sphere;
	//mode = 3: local sphere shape index ( sqrt_surface_area / (cbrt_volume_sphere*2.199085233)
	t1 = clock();

	/*vtkSmartPointer<vtkPolyDataConnectivityFilter> cfilter = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
	cfilter->SetInputData(mPD);
	cfilter->SetExtractionModeToPointSeededRegions();

	for (vtkIdType j = 0; j < list->GetNumberOfIds(); j++)
	{
		//newCuts->SetTuple1(list->GetId(j), 1);
		cfilter->AddSeed(list->GetId(j));
	}

	cfilter->Update();*/


	vtkSmartPointer<vtkIntArray> newCuts =
		vtkSmartPointer<vtkIntArray>::New();

	newCuts->SetNumberOfComponents(1); //3d normals (ie x,y,z)
	newCuts->SetNumberOfTuples(mPD->GetNumberOfPoints());


	
	
	
	for (vtkIdType i = 0; i < mPD->GetNumberOfPoints(); i++) {
		newCuts->InsertTuple1(i, 0);
	}
	for (vtkIdType j = 0; j < list->GetNumberOfIds(); j++)
	{
		newCuts->SetTuple1(list->GetId(j), 1);
		
	}
	

	
	newCuts->SetName("Cuts");
	mPD->GetPointData()->AddArray(newCuts);
	mPD->GetPointData()->SetActiveScalars("Cuts");

	vtkSmartPointer<vtkThreshold> selector =
		vtkSmartPointer<vtkThreshold>::New();
	vtkSmartPointer<vtkMaskFields> scalarsOff =
		vtkSmartPointer<vtkMaskFields>::New();
	vtkSmartPointer<vtkGeometryFilter> geometry =
		vtkSmartPointer<vtkGeometryFilter>::New();
	selector->SetInputData((vtkPolyData*)mPD);
	selector->SetInputArrayToProcess(0, 0, 0,
		vtkDataObject::FIELD_ASSOCIATION_CELLS,
		vtkDataSetAttributes::SCALARS);
	selector->SetAllScalars(1);// was g_tag_extraction_criterion_all
	selector->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "Cuts");
	selector->ThresholdBetween(0.9, 1.1);
	selector->Update();


	t2 = clock();
	sec1 = (double)(t2 - t1) / CLOCKS_PER_SEC;
	//std::cout << "\nSelector new Number of points:" << selector->GetOutput()->GetNumberOfPoints() << std::endl;
	//std::cout << "\nSelector new Number of cells:" << selector->GetOutput()->GetNumberOfCells() << std::endl;
	
	scalarsOff->SetInputData(selector->GetOutput());
	scalarsOff->CopyAttributeOff(vtkMaskFields::POINT_DATA, vtkDataSetAttributes::SCALARS);
	scalarsOff->CopyAttributeOff(vtkMaskFields::CELL_DATA, vtkDataSetAttributes::SCALARS);
	scalarsOff->Update();
	geometry->SetInputData(scalarsOff->GetOutput());
	geometry->Update();
	

	vtkSmartPointer<vtkPolyData> MyObj = vtkSmartPointer<vtkPolyData>::New();


	MyObj = geometry->GetOutput();
	//MyObj = cfilter->GetOutput();
	t3 = clock();
	sec2 = (double)(t3 - t2) / CLOCKS_PER_SEC;
	mPD->GetPointData()->RemoveArray("Cuts");
	vtkSmartPointer<vtkMassProperties> massProp = vtkSmartPointer<vtkMassProperties>::New();
	massProp->SetInputData(MyObj);
	massProp->Update();
	double surface_area = massProp->GetSurfaceArea();
	double sqrt_surface_area = sqrt(surface_area);

	
	if (mode < 2) // use of CPU TIME CONSUMING delaunay3D = convex hulls
	{
		vtkSmartPointer<vtkMassProperties> massPropConvexHull = vtkSmartPointer<vtkMassProperties>::New();
		vtkSmartPointer<vtkQuadricDecimation> decimate =
		vtkSmartPointer<vtkQuadricDecimation>::New();
		vtkSmartPointer<vtkDelaunay3D> delaunay3D =
			vtkSmartPointer<vtkDelaunay3D>::New();
		decimate->SetInputData(MyObj);
		decimate->SetVolumePreservation(1);
		double numvert = MyObj->GetNumberOfPoints();
		double reduction_factor = 0.1;
		double target_number = 10000;//max 10 000 points for local convex hull
		double new_factor = target_number / numvert;
		if (new_factor < 1)
		{
			reduction_factor = 1 - new_factor;
		}

		if (new_factor < 1)
		{
			decimate->SetTargetReduction(reduction_factor);
			decimate->Update();
			delaunay3D->SetInputData(decimate->GetOutput());
		}
		else
		{
			delaunay3D->SetInputData(MyObj);
		}
		delaunay3D->Update();
		t4 = clock();
		sec3 = (double)(t4 - t3) / CLOCKS_PER_SEC;
		vtkSmartPointer<vtkGeometryFilter> geometryFilter =
			vtkSmartPointer<vtkGeometryFilter>::New();

		geometryFilter->SetInputConnection(delaunay3D->GetOutputPort());
		geometryFilter->Update();
		
		massPropConvexHull->SetInputData(geometryFilter->GetOutput());
		massPropConvexHull->Update();
		
		double surface_area_convex_hull = massPropConvexHull->GetSurfaceArea();
		double sqrt_surface_area_convex_hull = sqrt(surface_area_convex_hull);


		double volume_convex_hull = massPropConvexHull->GetVolume();
		//cout << myActor->GetName().c_str() << " volume=" << massProp->GetVolume() << " volume_convex_hull=" << massPropConvexHull->GetVolume() << endl;
		double cbrt_volume_convex_hull = cbrt(volume_convex_hull);
		double custom_complexity = sqrt_surface_area / (cbrt_volume_convex_hull*2.199085233);
		double surface_ratio = 1;
		if (surface_area_convex_hull > 0) {
			surface_ratio = surface_area / surface_area_convex_hull;
		}
		t5 = clock();
		sec4 = (double)(t5 - t4) / CLOCKS_PER_SEC;
		if (printmode == 1)
		{
			cout << "convex hulls were computed... mode=" << mode <<  endl;
			cout << "sec1:" << sec1 << endl;
			cout << "sec2:" << sec2 << endl;
			cout << "sec3:" << sec3 << endl;
			cout << "sec4:" << sec4 << endl;
		}
		if (mode == 1) { return custom_complexity; }
		else { return surface_ratio; }
	}
	else
	{

		t4 = clock();
		sec3 = (double)(t4 - t3) / CLOCKS_PER_SEC;

		double surface_area_sphere = 4 * vtkMath::Pi() * sphere_radius*sphere_radius;
		double sqrt_surface_area_sphere = sqrt(surface_area_sphere);


		double volume_sphere = 4* vtkMath::Pi() * sphere_radius*sphere_radius*sphere_radius/3;
		//cout << myActor->GetName().c_str() << " volume=" << massProp->GetVolume() << " volume_convex_hull=" << massPropConvexHull->GetVolume() << endl;
		double cbrt_volume_sphere = 1;
		double custom_complexity = 1;
		if (volume_sphere > 0)
		{
			cbrt_volume_sphere = cbrt(volume_sphere);
			custom_complexity = sqrt_surface_area / (cbrt_volume_sphere*2.199085233);
		}
		
		double surface_ratio = 1;
		if (surface_area_sphere > 0) {
			surface_ratio = surface_area / surface_area_sphere;
		}
		t5 = clock();
		sec4 = (double)(t5 - t4) / CLOCKS_PER_SEC;
		if (printmode == 1)
		{
			cout << "sphere ratios... mode=" <<mode<< endl;
			cout << "sec1:" << sec1 << endl;
			cout << "sec2:" << sec2 << endl;
			cout << "sec3:" << sec3 << endl;
			cout << "sec4:" << sec4 << endl;
		}
		if (mode == 3) { return custom_complexity; }
		else { return surface_ratio; }

	}
	
	//Complexity process is computed on MyObj
	
}
double mqMorphoDigCore::ComputeActiveScalarsMedian(vtkSmartPointer<vtkPolyData> mPD, vtkSmartPointer<vtkIdList> list, int cutMinMax, double cutMin, double cutMax)
{
	vtkDataArray *currentScalars = NULL;
	if ((this->Getmui_ActiveArray()->DataType == VTK_FLOAT || this->Getmui_ActiveArray()->DataType == VTK_DOUBLE) && this->Getmui_ActiveArray()->NumComp == 1)
	{

		currentScalars = vtkDoubleArray::SafeDownCast(mPD->GetPointData()->GetScalars(this->Getmui_ActiveArray()->Name.toStdString().c_str()));
		if (currentScalars == NULL)
		{
			currentScalars = vtkFloatArray::SafeDownCast(mPD->GetPointData()->GetScalars(this->Getmui_ActiveArray()->Name.toStdString().c_str()));
		}

	}



	if (currentScalars != NULL&& list->GetNumberOfIds()>0)
	{
		double currScalar = 0;

	
		std::vector<double> vals;
		for (vtkIdType j = 0; j < list->GetNumberOfIds(); j++)
		{
			
			currScalar = (double)currentScalars->GetTuple1(list->GetId(j));
			if (cutMinMax == 1)
			{
				if (currScalar < cutMin) { currScalar = cutMin; }
				if (currScalar > cutMax) { currScalar = cutMax; }
			}
			vals.push_back(currScalar);
		}
		std::sort(vals.begin(), vals.end());
		int iMedian = (int)(list->GetNumberOfIds() / 2);
		currScalar = vals.at(iMedian);
		return currScalar;
	}
	else
	{
		return 0;
	}

}
double mqMorphoDigCore::ComputeActiveScalarsMean(vtkSmartPointer<vtkPolyData> mPD, vtkSmartPointer<vtkIdList> list, int cutMinMax, double cutMin, double cutMax)
{
	vtkDataArray *currentScalars = NULL;
	if ((this->Getmui_ActiveArray()->DataType == VTK_FLOAT || this->Getmui_ActiveArray()->DataType == VTK_DOUBLE) && this->Getmui_ActiveArray()->NumComp == 1)
	{

		currentScalars = vtkDoubleArray::SafeDownCast(mPD->GetPointData()->GetScalars(this->Getmui_ActiveArray()->Name.toStdString().c_str()));
		if (currentScalars == NULL)
		{
			currentScalars = vtkFloatArray::SafeDownCast(mPD->GetPointData()->GetScalars(this->Getmui_ActiveArray()->Name.toStdString().c_str()));
		}

	}

	

	if (currentScalars != NULL&& list->GetNumberOfIds()>0)
	{
		double currMean = 0;


		for (vtkIdType j = 0; j < list->GetNumberOfIds(); j++)
		{
			double currScalar = (double)currentScalars->GetTuple1(list->GetId(j));
			if (cutMinMax == 1)
			{
				if (currScalar< cutMin) { currScalar = cutMin; }
				if (currScalar > cutMax) { currScalar = cutMax; }
			}
			currMean += currScalar;

		}
		currMean /= list->GetNumberOfIds();
		return currMean;
	}
	else
	{
		return 0;
	}


	//Complexity process is computed on MyObj

}
vtkSmartPointer<vtkDoubleArray> mqMorphoDigCore::scalarSmooth(vtkDataArray *inputArray, vtkSmartPointer<vtkPolyData> mPD,  double localAreaLimit, int cutMinMax, double cutPercent, int mode, int smoothing_method)
{
	//mode = 0: raw smoothing (average of direct neighbours)
	//mode = 1: smooth within local sphere of radius ~ surface avg size / 40
	//mode = 2: smooth within local sphere of radius defined by the user

	//smoothing_method = 0 => average
	//smoothing_method = 1 => median
	vtkSmartPointer<vtkDoubleArray> newScalars =
		vtkSmartPointer<vtkDoubleArray>::New();
	newScalars->SetNumberOfComponents(1); //3d normals (ie x,y,z)
	newScalars->SetNumberOfTuples(inputArray->GetNumberOfTuples());

	vtkIdType numTuples = inputArray->GetNumberOfTuples();
	vtkIdType numVert = mPD->GetNumberOfPoints();
	cout << "scalarSmooth : numTuples=" << numTuples << endl;;
	cout << "scalarSmooth : numVert=" << numTuples << endl;;

	double cutMin = DBL_MAX;
	double cutMax = -DBL_MAX;
	if (cutMinMax == 1)
	{
		std::vector<float> vals;
		for (vtkIdType j = 0; j < inputArray->GetNumberOfTuples(); j++)
		{
			vals.push_back(inputArray->GetTuple1(j));
		}
		std::sort(vals.begin(), vals.end());
		int iMin = (int)(cutPercent*inputArray->GetNumberOfTuples() / 100);
		int iMax = (int)((100 - cutPercent)*inputArray->GetNumberOfTuples() / 100);

		if (iMax == inputArray->GetNumberOfTuples()) {
			iMax = inputArray->GetNumberOfTuples() - 1;
		}
		cutMin = (double)vals.at(iMin);
		cutMax = (double)vals.at(iMax);
		cout << "Will cut between " << cutMin << ", and " << cutMax << endl;

	}

	if (mode == 0)
	{


	
		vtkIdType ve;


		double vn[3];
		vn[0] = 0;
		vn[1] = 0;
		vn[2] = 1;
		double picked_value = 0;
		for (ve = 0; ve < inputArray->GetNumberOfTuples(); ve++)
		{
			// get all vertices connected to this point (neighbouring vertices).
			//if (ve<10){std::cout<<"Try to find connected vertices at : "<<ve<<std::endl;}
			vtkSmartPointer<vtkIdList> connectedVertices = vtkSmartPointer<vtkIdList>::New();
			connectedVertices = GetConnectedVertices(mPD, vn, picked_value, ve, -1, 0); //	int tool_mode=-1; //no pencil! no magic wand!

			vtkSmartPointer<vtkIdTypeArray> ids =
				vtkSmartPointer<vtkIdTypeArray>::New();
			ids->SetNumberOfComponents(1);
			ids->InsertNextValue(ve);
			//if (ve<10){std::cout << "Connected vertices: ";}
			for (vtkIdType j = 0; j < connectedVertices->GetNumberOfIds(); j++)
			{
				//if (ve<10){std::cout << connectedVertices->GetId(j) << " ";}
				if (connectedVertices->GetId(j) != ve)
				{
					ids->InsertNextValue(connectedVertices->GetId(j));
				}
			}
			float newscalar = 0;
			int n_vertices = connectedVertices->GetNumberOfIds();
			float oldscalar = (float)(inputArray->GetTuple(ve))[0];
			newscalar = 0;			
			//on ajoute une fois la valeur actuelle.

			// get all scalars 				
			//if (ve<10){std::cout<<std::endl;}

			if (smoothing_method == 0)
			{
				for (vtkIdType j = 0; j < ids->GetNumberOfTuples(); j++)
				{	// for all neighbouring vertices							
					vtkIdType at = ids->GetTuple(j)[0];
					//if (ve<10){std::cout<<"old scalar value at "<<at<<"=";}
					double curr_scalar = (double)(inputArray->GetTuple(at))[0];
					//std::cout<<curr_scalar<<std::endl;
					if (cutMinMax == 1)
					{
						if (newscalar < cutMin) { newscalar = cutMin; }
						if (newscalar > cutMax) { newscalar = cutMax; }
					}
					newscalar += curr_scalar;

				}
				if (n_vertices > 0)
				{
					newscalar /= n_vertices;
				}				
			}
			else
			{
				// find median
				std::vector<double> vals;
				for (vtkIdType j = 0; j < ids->GetNumberOfTuples(); j++)
				{	// for all neighbouring vertices							
					vtkIdType at = ids->GetTuple(j)[0];
					//if (ve<10){std::cout<<"old scalar value at "<<at<<"=";}
					double curr_scalar = (double)(inputArray->GetTuple(at))[0];
					if (cutMinMax == 1)
					{
						if (curr_scalar < cutMin) { curr_scalar = cutMin; }
						if (curr_scalar > cutMax) { curr_scalar = cutMax; }
					}
					vals.push_back(curr_scalar);
				}
				std::sort(vals.begin(), vals.end());
				int iMedian = (int)(ids->GetNumberOfTuples() / 2);
				newscalar = vals.at(iMedian);

			}
			// if (ve<10){std::cout<<"New Scalar value at "<<ve<<"="<<newscalar<<std::endl;}	
			newScalars->InsertTuple1(ve, newscalar);


		}

	}
	else
	{
		/*
		if (this->Ui->localAuto->isChecked()) { mode = 1; }
		if (this->Ui->localCustom->isChecked()) { mode = 2; }*/
		double searchSize = localAreaLimit;
		
		cout << "mode=" << mode << ", searchSize=" << searchSize << endl;
		double numvert = inputArray->GetNumberOfTuples();


		

		double ve_pos[3];
		vtkSmartPointer<vtkKdTreePointLocator> kDTree =
			vtkSmartPointer<vtkKdTreePointLocator>::New();
		kDTree->SetDataSet(mPD);

		kDTree->BuildLocator();

		for (vtkIdType i = 0; i < numvert; i++) {
			//clock_t t0, t1, tt0, tt1;
			//tt0 = clock();
			/*if (i % 1000 == 0)
			{
			cout << "complexity, vertex:" << i << endl;
			}*/
			// for every triangle 
			mPD->GetPoint(i, ve_pos);
			double currentMean = 0;
			if ((i % (int)(numvert / 100)) == 0)
			{
				emit smoothingProgression((int)(100 * i / numvert));
			}
			vtkSmartPointer<vtkIdList> observedNeighbours = vtkSmartPointer<vtkIdList>::New();
			double Radius = searchSize;


			//double sec0;
			//double ssec0;
			//mode = 0: convex hull area ratio
			//mode = 1: convex hull shape index
			//t0 = clock();
			kDTree->FindPointsWithinRadius(searchSize, ve_pos, observedNeighbours);
			//t1 = clock();
			if (i % 10000 == 0)
			{
				cout << "number of neighbours(" << i << "): " << observedNeighbours->GetNumberOfIds() << endl;
				//	sec0 = (double)(t1 - t0) / CLOCKS_PER_SEC;
				//	cout << "sec0=" << sec0 << endl;
			}
			//cout << "mode = " << mode << endl;
			// now extract surface
			//compute surface
			int printmode = 0;
			if (i % 1000 == 0)
			{
				//printmode = 1;
			}
			if (Radius <= 0)
			{
				Radius = 1;
			}
			if (smoothing_method == 0)
			{
				currentMean = this->ComputeActiveScalarsMean(mPD, observedNeighbours, cutMinMax, cutMin, cutMax);
			}
			else
			{
				currentMean = this->ComputeActiveScalarsMedian(mPD, observedNeighbours, cutMinMax, cutMin, cutMax);
			}
			if (i % 10000 == 0)
			{
				cout << "currentMean:" << currentMean << endl;
			}
			if (cutMinMax == 1)
			{
				if (currentMean < cutMin) { currentMean = cutMin; }
				if (currentMean > cutMax) { currentMean = cutMax; }
			}

			newScalars->InsertTuple1(i, currentMean);
			//tt1 = clock();
			/*if (i % 1000 == 0)
			{
			ssec0 = (double)(tt1 - tt0) / CLOCKS_PER_SEC;
			cout << "Total time for 1 vertex:" << ssec0 << endl;

			}*/
		}

		
		



	}
		return newScalars;
}

void mqMorphoDigCore::scalarsSmooth(QString scalarName, double localAreaLimit, int cutMinMax, double cutPercent, int mode, int smoothing_method)
{
	//mode = 0: raw smoothing (average of direct neighbours)
	//mode = 1: smooth within local sphere of radius ~ surface avg size / 40
	//mode = 2: smooth within local sphere of radius defined by the user

	//smoothing_method = 0 => average
	//smoothing_method = 1 => median

	// cut = 0 : cut ctuPercert% of Min and Max "abherent" values (for instance : vtkCurvatures yields extreme Min and Max values which make smoothing difficult)
	vtkIdType num_selected_meshes = this->getActorCollection()->GetNumberOfSelectedActors();
	if (num_selected_meshes == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}

	if ((this->Getmui_ActiveArray()->DataType == VTK_FLOAT || this->Getmui_ActiveArray()->DataType == VTK_DOUBLE) && this->Getmui_ActiveArray()->NumComp == 1)
	{
		std::string mScalarName = "SmoothedScalar";
		this->ActorCollection->InitTraversal();
		vtkIdType num = this->ActorCollection->GetNumberOfItems();
		int modified = 0;

		std::string action = "Smooth active scalars";		
		int Count = BEGIN_UNDO_SET(action);

		for (vtkIdType i = 0; i < num; i++)
		{
			cout << "Scalar smooth :" << i << endl;
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
			cout << "Got next actor:" << i << endl;
			if (myActor!=NULL && myActor->GetSelected() == 1)
			{


				myActor->SetSelected(0);
				vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
				if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
				{
					cout << "found mapper for :" << i << endl;
					vtkSmartPointer<vtkPolyData> mPD = vtkSmartPointer<vtkPolyData>::New();
					mPD = mymapper->GetInput();

					vtkDataArray *currentScalars = NULL;


					currentScalars = vtkDoubleArray::SafeDownCast(mPD->GetPointData()->GetScalars(this->Getmui_ActiveArray()->Name.toStdString().c_str()));
					if (currentScalars == NULL)
					{
						currentScalars = vtkFloatArray::SafeDownCast(mPD->GetPointData()->GetScalars(this->Getmui_ActiveArray()->Name.toStdString().c_str()));
					}
					if (currentScalars!=NULL)
					{
					
						cout << "found scalars for :" << i << endl;

						double numvert = mymapper->GetInput()->GetNumberOfPoints();




						std::string scname = scalarName.toStdString();


						if (scname.length() > 0)
						{
							mScalarName = scname;
						}
						myActor->SaveState(Count, QString(mScalarName.c_str()));

						double searchSize = localAreaLimit;
						if (mode == 1 || localAreaLimit <= 0)
						{
							searchSize = myActor->GetXYZAvgPCLength() / 40; // looks like a reasonable neighbourhood sphere radius size.
						}

						vtkSmartPointer<vtkDoubleArray> newScalars = this->scalarSmooth(currentScalars, mPD,searchSize, cutMinMax, cutPercent, mode, smoothing_method);
						
					

							newScalars->SetName(mScalarName.c_str());
							
							// test if exists...

							// remove this scalar
							//this->GetPointData()->SetScalars(newScalars);
							mPD->GetPointData()->RemoveArray(mScalarName.c_str());
							mPD->GetPointData()->AddArray(newScalars);
							mPD->GetPointData()->SetActiveScalars(mScalarName.c_str());



						



					
						

						modified = 1;
					}//scalars not null

				}

			}
			
		}
		if (modified == 1)
		{		


			this->Initmui_ExistingArrays();
			this->Setmui_ActiveArrayAndRender(mScalarName.c_str(), VTK_DOUBLE, 1);
			cout << "scalars updated " << endl;
			this->Render();
		}
		END_UNDO_SET();

	}

}

void mqMorphoDigCore::addKeepLargest()
{
	vtkSmartPointer<vtkMDActorCollection> newcoll = vtkSmartPointer<vtkMDActorCollection>::New();
	this->ActorCollection->InitTraversal();
	vtkIdType num = this->ActorCollection->GetNumberOfItems();
	int modified = 0;
	for (vtkIdType i = 0; i < num; i++)
	{
		cout << "Largest region of next actor:" << i << endl;
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			//myActor->SetSelected(0);

			vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
			if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
			{


				double numvert = mymapper->GetInput()->GetNumberOfPoints();

			
				VTK_CREATE(vtkMDActor, newactor);
				if (this->mui_BackfaceCulling == 0)
				{
					newactor->GetProperty()->BackfaceCullingOff();
				}
				else
				{
					newactor->GetProperty()->BackfaceCullingOn();
				}

				VTK_CREATE(vtkPolyDataMapper, newmapper);
				//newmapper->ImmediateModeRenderingOn();
				newmapper->SetColorModeToDefault();

				if (
					(this->mui_ActiveArray->DataType == VTK_INT || this->mui_ActiveArray->DataType == VTK_UNSIGNED_INT)
					&& this->mui_ActiveArray->NumComp == 1
					)
				{
					newmapper->SetScalarRange(0, this->Getmui_ActiveTagMap()->numTags - 1);
					newmapper->SetLookupTable(this->Getmui_ActiveTagMap()->TagMap);
				}
				else
				{
					newmapper->SetLookupTable(this->Getmui_ActiveColorMap()->ColorMap);
				}

				newmapper->ScalarVisibilityOn();
				VTK_CREATE(vtkPolyData, myData);

				vtkSmartPointer<vtkPolyDataConnectivityFilter> cfilter = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
				cfilter->SetInputData(mymapper->GetInput());
				cfilter->SetExtractionModeToLargestRegion();
				cfilter->Update();

				

				vtkSmartPointer<vtkCleanPolyData> cleanPolyDataFilter = vtkSmartPointer<vtkCleanPolyData>::New();
				//cleanPolyDataFilter->SetInputData(ObjNormals->GetOutput());
				cleanPolyDataFilter->SetInputData(cfilter->GetOutput());
				cleanPolyDataFilter->PieceInvariantOff();
				cleanPolyDataFilter->ConvertLinesToPointsOff();
				cleanPolyDataFilter->ConvertPolysToLinesOff();
				cleanPolyDataFilter->ConvertStripsToPolysOff();
				cleanPolyDataFilter->PointMergingOn();
				cleanPolyDataFilter->Update();

				myData = cleanPolyDataFilter->GetOutput();



				

				newmapper->SetInputData(myData);

				vtkSmartPointer<vtkMatrix4x4> Mat = myActor->GetMatrix();


				vtkTransform *newTransform = vtkTransform::New();
				newTransform->PostMultiply();

				newTransform->SetMatrix(Mat);
				newactor->SetPosition(newTransform->GetPosition());
				newactor->SetScale(newTransform->GetScale());
				newactor->SetOrientation(newTransform->GetOrientation());
				newTransform->Delete();


				double color[4] = { 0.5, 0.5, 0.5, 1 };
				myActor->GetmColor(color);
				newactor->SetmColor(color);

				newactor->SetMapper(newmapper);
				newactor->SetSelected(0);


				newactor->SetName(myActor->GetName() + "_largest");
				cout << "try to add new actor=" << endl;
				newcoll->AddTmpItem(newactor);
				modified = 1;


			}
		}
	}
	if (modified == 1)
	{
		newcoll->InitTraversal();
		vtkIdType num = newcoll->GetNumberOfItems();
		for (vtkIdType i = 0; i < num; i++)
		{
			cout << "try to get next actor from newcoll:" << i << endl;
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(newcoll->GetNextActor());
			myActor->SetColorProperties(this->mui_Ambient, this->mui_Diffuse, this->mui_Specular, this->mui_SpecularPower);
			myActor->SetDisplayMode(this->mui_DisplayMode);
			this->getActorCollection()->AddItem(myActor);
			emit this->actorsMightHaveChanged();
			std::string action = "Largest region object added: " + myActor->GetName();
			int mCount = BEGIN_UNDO_SET(action);
			this->getActorCollection()->CreateLoadUndoSet(mCount, 1);
			END_UNDO_SET();


		}
		//cout << "camera and grid adjusted" << endl;
		cout << "new actor(s) added" << endl;
		this->Initmui_ExistingArrays();

		cout << "Set actor collection changed" << endl;
		this->getActorCollection()->SetChanged(1);
		cout << "Actor collection changed" << endl;

		this->AdjustCameraAndGrid();
		cout << "Camera and grid adjusted" << endl;

		if (this->Getmui_AdjustLandmarkRenderingSize() == 1)
		{
			this->UpdateLandmarkSettings();
		}
		this->Render();
	}

}
void mqMorphoDigCore::DeleteSelectedActors()
{
	this->ActorCollection->DeleteSelectedActors();
	this->VolumeCollection->DeleteSelectedVolumes();
	this->NormalLandmarkCollection->DeleteSelectedActors();
	this->TargetLandmarkCollection->DeleteSelectedActors();
	this->NodeLandmarkCollection->DeleteSelectedActors();
	this->HandleLandmarkCollection->DeleteSelectedActors();
	this->FlagLandmarkCollection->DeleteSelectedActors();
	emit this->actorsMightHaveChanged();
	
}
void mqMorphoDigCore::addInvert()
{
	vtkSmartPointer<vtkMDActorCollection> newcoll = vtkSmartPointer<vtkMDActorCollection>::New();
	this->ActorCollection->InitTraversal();
	vtkIdType num = this->ActorCollection->GetNumberOfItems();
	int modified = 0;
	for (vtkIdType i = 0; i < num; i++)
	{
		cout << "Invert next actor:" << i << endl;
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			//myActor->SetSelected(0);

			vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
			if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
			{


				double numvert = mymapper->GetInput()->GetNumberOfPoints();

		
				VTK_CREATE(vtkMDActor, newactor);
				if (this->mui_BackfaceCulling == 0)
				{
					newactor->GetProperty()->BackfaceCullingOff();
				}
				else
				{
					newactor->GetProperty()->BackfaceCullingOn();
				}

				VTK_CREATE(vtkPolyDataMapper, newmapper);
				//newmapper->ImmediateModeRenderingOn();
				newmapper->SetColorModeToDefault();

				if (
					(this->mui_ActiveArray->DataType == VTK_INT || this->mui_ActiveArray->DataType == VTK_UNSIGNED_INT)
					&& this->mui_ActiveArray->NumComp == 1
					)
				{
					newmapper->SetScalarRange(0, this->Getmui_ActiveTagMap()->numTags - 1);
					newmapper->SetLookupTable(this->Getmui_ActiveTagMap()->TagMap);

				}
				else
				{
					newmapper->SetLookupTable(this->Getmui_ActiveColorMap()->ColorMap);
				}

				newmapper->ScalarVisibilityOn();
				VTK_CREATE(vtkPolyData, myData);
				vtkSmartPointer<vtkReverseSense> mfilter = vtkSmartPointer<vtkReverseSense>::New();
				//
				mfilter->SetInputData(vtkPolyData::SafeDownCast(mymapper->GetInput()));			
				mfilter->ReverseCellsOn();
				//mfilter->SetReverseNormals(1); => causes MorphoDig to crash... extremely odd!
				mfilter->Update();
				
				myData = mfilter->GetOutput();
				
				// dirty hack because vtkReverseSense crashes when trying to reverse normals....
				vtkSmartPointer<vtkFloatArray> normalsArray =
					vtkSmartPointer<vtkFloatArray>::New();

				normalsArray->SetNumberOfComponents(3); //3d normals (ie x,y,z)
				normalsArray->SetNumberOfTuples(myData->GetNumberOfPoints());
				double *vn;
				float vn2[3];
				vtkFloatArray* norms = vtkFloatArray::SafeDownCast
				(myData->GetPointData()->GetNormals());
				//std::cout << "\n old norms :"<<norms->GetNumberOfTuples()
				//  <<"\n Current object  number of points:"<<this->GetNumberOfPoints();

				for (int i = 0; i<myData->GetNumberOfPoints(); i++) {
					vn = norms->GetTuple((vtkIdType)i);
					vn2[0] = (float)(-vn[0]); vn2[1] = (float)(-vn[1]); vn2[2] = (float)(-vn[2]);
					normalsArray->SetTuple(i, vn2);
					//std::cout << "\n i:"<<i<<"|vn2[0]"<<vn2[0]<<"|vn2[1]"<<vn2[1]<<"|vn2[2]"<<vn2[2];
				}
				normalsArray->SetName("Normals"); // MorphoDig needs a name... 
				myData->GetPointData()->SetNormals(normalsArray);


				std::cout << "\nVtkInvert new Number of points:" << myData->GetNumberOfPoints() << std::endl;
				std::cout << "VtkInvert new Number of cells:" << myData->GetNumberOfCells() << std::endl;

				newmapper->SetInputData(myData);

				vtkSmartPointer<vtkMatrix4x4> Mat = myActor->GetMatrix();
				

				vtkTransform *newTransform = vtkTransform::New();
				newTransform->PostMultiply();

				newTransform->SetMatrix(Mat);
				newactor->SetPosition(newTransform->GetPosition());
				newactor->SetScale(newTransform->GetScale());
				newactor->SetOrientation(newTransform->GetOrientation());
				newTransform->Delete();


				double color[4] = { 0.5, 0.5, 0.5, 1 };
				myActor->GetmColor(color);
				newactor->SetmColor(color);

				newactor->SetMapper(newmapper);
				newactor->SetSelected(0);


				//newactor->SetName(myActor->GetName() + "_inv");
				//std::string newname = this->CheckingName(myActor->GetName());
				newactor->SetName(myActor->GetName() + "_inv");
				cout << "try to add new actor=" << endl;
				newcoll->AddTmpItem(newactor);
				modified = 1;


			}
		}
	}
	if (modified == 1)
	{
		newcoll->InitTraversal();
		vtkIdType num = newcoll->GetNumberOfItems();
		for (vtkIdType i = 0; i < num; i++)
		{
			cout << "try to get next actor from newcoll:" << i << endl;
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(newcoll->GetNextActor());
			myActor->SetColorProperties(this->mui_Ambient, this->mui_Diffuse, this->mui_Specular, this->mui_SpecularPower);
			myActor->SetDisplayMode(this->mui_DisplayMode);
			this->getActorCollection()->AddItem(myActor);
			emit this->actorsMightHaveChanged();
			std::string action = "Inverted object added: " + myActor->GetName();
			int mCount = BEGIN_UNDO_SET(action);
			this->getActorCollection()->CreateLoadUndoSet(mCount, 1);
			END_UNDO_SET();


		}
		//cout << "camera and grid adjusted" << endl;
		cout << "new actor(s) added" << endl;
		this->Initmui_ExistingArrays();

		cout << "Set actor collection changed" << endl;
		this->getActorCollection()->SetChanged(1);
		cout << "Actor collection changed" << endl;

		this->AdjustCameraAndGrid();
		cout << "Camera and grid adjusted" << endl;

		if (this->Getmui_AdjustLandmarkRenderingSize() == 1)
		{
			this->UpdateLandmarkSettings();
		}
		this->Render();
	}


}

/*
void mqMorphoDigCore::Invert()
{
For some reason this function works... but the rendering of the modified surface is wrong. Needs to press "W" (wireframe rendering) and then "S" (surface rendering) 
to achieve desired the correct rendering. Do not understand why yet.


	this->ActorCollection->InitTraversal();
	vtkIdType num = this->ActorCollection->GetNumberOfItems();
	int modified = 0;
	for (vtkIdType i = 0; i < num; i++)
	{
		cout << "Mirror XZ try to get next actor:" << i << endl;
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			myActor->SetSelected(0);
			
			vtkPolyDataMapper *mymapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
			if (mymapper != NULL && vtkPolyData::SafeDownCast(mymapper->GetInput()) != NULL)
			{


				double numvert = mymapper->GetInput()->GetNumberOfPoints();
				
				VTK_CREATE(vtkPolyData, myData);
				myData = mymapper->GetInput();

				//1) Modify normals
				vtkSmartPointer<vtkFloatArray> normalsArray =
					vtkSmartPointer<vtkFloatArray>::New();

				normalsArray->SetNumberOfComponents(3); //3d normals (ie x,y,z)
				normalsArray->SetNumberOfTuples(myData->GetNumberOfPoints());
				double *vn;
				float vn2[3];
				vtkFloatArray* norms = vtkFloatArray::SafeDownCast
				(myData->GetPointData()->GetNormals());
				//std::cout << "\n old norms :"<<norms->GetNumberOfTuples()
				//  <<"\n Current object  number of points:"<<this->GetNumberOfPoints();

				for (int i = 0; i<myData->GetNumberOfPoints(); i++) {
					vn = norms->GetTuple((vtkIdType)i);
					vn2[0] = (float)(-vn[0]); vn2[1] = (float)(-vn[1]); vn2[2] = (float)(-vn[2]);
					normalsArray->SetTuple(i, vn2);
					//std::cout << "\n i:"<<i<<"|vn2[0]"<<vn2[0]<<"|vn2[1]"<<vn2[1]<<"|vn2[2]"<<vn2[2];
				}

				//add the normals to the cells in the polydata
				//std::cout << "\n Set Normals? Reference count:"<<normalsArray->GetReferenceCount();
				//normalsArray->GetReferenceCount();
				myData->GetPointData()->SetNormals(normalsArray);		

				//2) invert triangle order
				vtkIdType ve1, ve2, ve3;
				vtkSmartPointer<vtkIdList> oldpoints = vtkSmartPointer<vtkIdList>::New();
				vtkSmartPointer<vtkIdList> newpoints = vtkSmartPointer<vtkIdList>::New();

				//std::cout << "\n Surface invert";
				//for every triangle
				for (vtkIdType i = 0; i < myData->GetNumberOfCells(); i++)
				{
					myData->GetCellPoints(i, oldpoints);
					ve1 = oldpoints->GetId(0);
					ve2 = oldpoints->GetId(1);
					ve3 = oldpoints->GetId(2);
					if (i==10)
					{  std::cout << "i:"<<i<<"BEFORE: ve1="<<ve1<<"|ve2="<<ve2<<"|ve3="<<ve3<<endl;

					}

					myData->ReverseCell(i);

					myData->GetCellPoints(i, newpoints);
					ve1 = newpoints->GetId(0);
					ve2 = newpoints->GetId(1);
					ve3 = newpoints->GetId(2);
					if (i==10)
					{
						std::cout << "i:" << i << "AFTER: ve1=" << ve1 << "|ve2=" << ve2 << "|ve3=" << ve3 << endl;

					}
				}
				myData->Modified();
				
				//myData->
				for (vtkIdType i = 0; i < myData->GetNumberOfCells(); i++)
				{
					myData->GetCellPoints(i, newpoints);
					ve1 = newpoints->GetId(0);
					ve2 = newpoints->GetId(1);
					ve3 = newpoints->GetId(2);
					if (i == 10)
					{
						std::cout << "i:" << i << "AFTER: ve1=" << ve1 << "|ve2=" << ve2 << "|ve3=" << ve3 << endl;

					}
				}

				modified = 1;


			}
		}
		
		//myActor->GetProperty()->SetRepresentationToWireframe();
		//myActor->GetProperty()->SetRepresentationToSurface();
	}
	if (modified == 1)
	{
		
		this->Render();
	}
}
*/

void mqMorphoDigCore::groupSelectedActors()
{
	cout << "Group 1" << endl;
	int numsel = this->ActorCollection->GetNumberOfSelectedActors();
	if (numsel < 2)
	{
		QMessageBox msgBox;
		msgBox.setText("Select at least two surfaces to use this function!");
		msgBox.exec();

		return;
	}
	else
	{
		QInputDialog *nameDialog = new QInputDialog();
		bool dialogResult;
		QString newActorName = nameDialog->getText(0, "Name", "New surface name:", QLineEdit::Normal,
			"Group", &dialogResult);
		if (!dialogResult)
		{
			return;
			
		
		}
		cout << "Group 2" << endl;

		vtkSmartPointer<vtkAppendPolyData> mergedObjects = vtkSmartPointer<vtkAppendPolyData>::New();
		int Ok = 1;
		int modified = 0;

		//cout << "myActor is null" << endl;

		this->ActorCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
		{
			vtkMDActor *myActor2 = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
			if (myActor2->GetSelected() == 1)
			{
				cout << "Group 3" << i<< endl;
				vtkPolyDataMapper *mapper = vtkPolyDataMapper::SafeDownCast(myActor2->GetMapper());
				if (mapper != NULL && vtkPolyData::SafeDownCast(mapper->GetInput()) != NULL)
				{
					vtkSmartPointer<vtkPolyData> toSave = vtkSmartPointer<vtkPolyData>::New();
					toSave->DeepCopy(vtkPolyData::SafeDownCast(mapper->GetInput()));
					double ve_init_pos[3];;
					double ve_final_pos[3];
					vtkSmartPointer<vtkMatrix4x4> Mat = myActor2->GetMatrix();


					for (vtkIdType i = 0; i < toSave->GetNumberOfPoints(); i++) {
						// for every triangle 
						toSave->GetPoint(i, ve_init_pos);
						mqMorphoDigCore::TransformPoint(Mat, ve_init_pos, ve_final_pos);

						toSave->GetPoints()->SetPoint((vtkIdType)i, ve_final_pos);
					}
					// if we do not do that for each "toSave" before including them inside the "mergedObjects", vtkPolyDataNormals will crash... 
					vtkSmartPointer<vtkPolyDataNormals> ObjNormals = vtkSmartPointer<vtkPolyDataNormals>::New();
					cout << "Group 4bis2" << endl;
					ObjNormals->SetInputData(toSave);
					ObjNormals->ComputePointNormalsOn();
					ObjNormals->ComputeCellNormalsOn();
					ObjNormals->AutoOrientNormalsOn();
					ObjNormals->ConsistencyOff();
					cout << "Group 4bis5" << endl;
					ObjNormals->Update();

					cout << " toSave->GetNumberOfPoints()=" << toSave->GetNumberOfPoints() << endl;
					mergedObjects->AddInputData(ObjNormals->GetOutput());
					//mergedObjects->AddInputData(toSave);
					modified = 1;
				}


			}
		}
		if (modified == 1)
		{
			cout << "Group 4" << endl;
			
			mergedObjects->Update();
			
			cout << "Group 4bis" << endl;
			// in group actor, we still need vtkPolyDataNormals


			cout << "Group 4ter" << endl;
			VTK_CREATE(vtkMDActor, newactor);
			if (this->mui_BackfaceCulling == 0)
			{
				newactor->GetProperty()->BackfaceCullingOff();
			}
			else
			{
				newactor->GetProperty()->BackfaceCullingOn();
			}
			cout << "Group 5" << endl;
			VTK_CREATE(vtkPolyDataMapper, newmapper);
			newmapper->SetColorModeToDefault();

			if (
				(this->mui_ActiveArray->DataType == VTK_INT || this->mui_ActiveArray->DataType == VTK_UNSIGNED_INT)
				&& this->mui_ActiveArray->NumComp == 1
				)
			{
				newmapper->SetScalarRange(0, this->Getmui_ActiveTagMap()->numTags - 1);
				newmapper->SetLookupTable(this->Getmui_ActiveTagMap()->TagMap);
			}
			else
			{
				newmapper->SetLookupTable(this->Getmui_ActiveColorMap()->ColorMap);
			}

			newmapper->ScalarVisibilityOn();

			cout << "Group 6" << endl;
			newmapper->SetInputData(mergedObjects->GetOutput());
			//newmapper->SetInputData(ObjNormals->GetOutput());

			int num = 2;




			double color[4] = { 0.3, 0.4, 0.5, 1 };


			newactor->SetmColor(color);

			newactor->SetMapper(newmapper);
			newactor->SetSelected(0);

			std::string actorName = this->CheckingName(newActorName.toStdString());
			newactor->SetName(actorName);
			cout << "Group 7" << endl;
			cout << "try to add new actor=" << endl;
			newactor->SetColorProperties(this->mui_Ambient, this->mui_Diffuse, this->mui_Specular, this->mui_SpecularPower);
			newactor->SetDisplayMode(this->mui_DisplayMode);
			cout << "Group 7 display passed" << endl;
			this->getActorCollection()->AddItem(newactor);
			std::string action = "Grouped actor added: " + newactor->GetName();
			int mCount = BEGIN_UNDO_SET(action);
			this->getActorCollection()->CreateLoadUndoSet(mCount, 1);
			END_UNDO_SET();
			cout << "new actor(s) added" << endl;
			this->Initmui_ExistingArrays();

			cout << "Set actor collection changed" << endl;
			this->getActorCollection()->SetChanged(1);
			cout << "Actor collection changed" << endl;

			this->AdjustCameraAndGrid();
			cout << "Camera and grid adjusted" << endl;

			cout << "Group 8" << endl;
			if (this->Getmui_AdjustLandmarkRenderingSize() == 1)
			{
				this->UpdateLandmarkSettings();
			}
			this->Render();

		}

	}
}
void mqMorphoDigCore::SaveVolume(QString fileName, int file_type, int compression, vtkMDVolume *myVolume, int compression_type, int  bigendian)
{
	// File_type 0 : mhd
	// File_type 1 : mha
	// File_type 2 : vti
	std::string MHAext(".mha");
	std::string MHAext2(".MHA");
	std::string MHDext(".mhd");
	std::string MHDext2(".MHD");
	std::string VTIext(".vti");
	std::string VTIext2(".VTI");

	//compression_type = 1 : Zlib
	//compression_type = 2 : LZ4
	//compression_type = 2 : ZMA




	int Ok = 1;
	cout << "here am I" << endl;
	vtkMDVolume *myVolume2 = myVolume;

	if (myVolume == NULL) // from menu => then save first selected volume!
	{
		cout << "myVolume is null => we save first selected volume!" << endl;
		myVolume2 = this->GetFirstSelectedVolume();


	}

	if (myVolume2 != NULL)
	{
		vtkSmartPointer<vtkImageData> output = vtkSmartPointer<vtkImageData>::New();
		output = myVolume2->GetImageData();
		if (file_type == 0 || file_type == 1)
		{
			std::size_t found = fileName.toStdString().find(MHDext);
			std::size_t found2 = fileName.toStdString().find(MHDext2);			
			if (file_type == 0 && found == std::string::npos && found2 == std::string::npos)
			{
				fileName.append(".mhd");

			}

			found = fileName.toStdString().find(MHAext);
			found2 = fileName.toStdString().find(MHAext2);
			if (file_type == 1 && found == std::string::npos && found2 == std::string::npos)
			{
				fileName.append(".mha");
			}

			vtkSmartPointer<vtkMetaImageWriter> mhWriter = vtkSmartPointer<vtkMetaImageWriter>::New();
			mhWriter->SetFileName(fileName.toLocal8Bit());
			if (compression == 0) { mhWriter->SetCompression(false); }
			else{ mhWriter->SetCompression(true); } 

			

			mhWriter->SetInputData(output);
			mhWriter->Write();
			
		}
		else if (file_type == 2)
		{

			std::size_t found = fileName.toStdString().find(VTIext);
			std::size_t found2 = fileName.toStdString().find(VTIext2);
			if (found == std::string::npos && found2 == std::string::npos)
			{
				fileName.append(".vti");

			}			
			vtkSmartPointer<vtkXMLImageDataWriter> xmlWriter = vtkSmartPointer<vtkXMLImageDataWriter>::New();
			xmlWriter->SetFileName(fileName.toLocal8Bit());
			xmlWriter->SetInputData(output);
			if (compression == 0) { xmlWriter->SetCompressorTypeToNone(); }
			else
			{
				if (compression_type == 1)
				{
					xmlWriter->SetCompressorTypeToZLib();
				}
				else if (compression_type == 2)
				{
					xmlWriter->SetCompressorTypeToLZ4();
				}
				else if (compression_type == 3)
				{
					xmlWriter->SetCompressorTypeToLZMA();
				}
				else
				{
					xmlWriter->SetCompressorTypeToZLib();
				}
				
			}
			if (bigendian == 1) { xmlWriter->SetByteOrderToBigEndian(); }
			else { xmlWriter->SetByteOrderToLittleEndian(); }
			
			xmlWriter->Write();
			
		}

		

	}


}
int mqMorphoDigCore::SaveSurfaceFile(QString fileName, int write_type, int position_mode, int file_type, std::vector<std::string> scalarsToBeRemoved, int RGBopt, int save_norms, vtkMDActor *myActor)
{
	// Write_Type 0 : Binary LE or "Default Binary"
	// Write_Type 1 : Binary BE 
	// Write_Type 2 : ASCII



	// if length = 1 and contains "all" or length = 0 => save all scalars.
	// else : remove the selected scalars

	// Position_mode 0 : orignal position
	// Position_mode 1 : moved position

	// File_type 0 : stl
	// File_type 1 : vtk
	// File_type 2 : ply
	// File_type 3 : obj
	// File_type 4 : vtp

	// If myActor is NULL : => what will be saved is an aggregation of all selected surface objects.
	// If myActor is not NULL: => what will be saved is the underlying surface object.


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


	vtkSmartPointer<vtkAppendPolyData> mergedObjects = vtkSmartPointer<vtkAppendPolyData>::New();
	int Ok = 1;
	cout << "here am I" << endl;

	if (myActor == NULL)
	{
		cout << "myActor is null" << endl;

		this->ActorCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
		{
			vtkMDActor *myActor2 = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
			if (myActor2->GetSelected() == 1)
			{
					vtkPolyDataMapper *mapper = vtkPolyDataMapper::SafeDownCast(myActor2->GetMapper());
					if (mapper != NULL && vtkPolyData::SafeDownCast(mapper->GetInput()) != NULL)
					{
						vtkSmartPointer<vtkPolyData> toSave = vtkSmartPointer<vtkPolyData>::New();
						toSave->DeepCopy(vtkPolyData::SafeDownCast(mapper->GetInput()));
						cout << "here am I 2" << endl;
						if (position_mode == 1)
						{
							double ve_init_pos[3];;
							double ve_final_pos[3];
							vtkSmartPointer<vtkMatrix4x4> Mat = myActor2->GetMatrix();


							for (vtkIdType j = 0; j < toSave->GetNumberOfPoints(); j++) {
								// for every triangle 
								toSave->GetPoint(j, ve_init_pos);
								mqMorphoDigCore::TransformPoint(Mat, ve_init_pos, ve_final_pos);

								toSave->GetPoints()->SetPoint((vtkIdType)j, ve_final_pos);
							}
						}
						cout << "here am I 3" << endl;
						// herea rare case where we need to compute normals (save merged surface)!
						vtkSmartPointer<vtkPolyDataNormals> ObjNormals = vtkSmartPointer<vtkPolyDataNormals>::New();
						ObjNormals->SetInputData(toSave);
						ObjNormals->ComputePointNormalsOn();
						ObjNormals->ComputeCellNormalsOn();
						ObjNormals->AutoOrientNormalsOff();
						ObjNormals->ConsistencyOff();
						cout << "update normals " << endl;
						ObjNormals->Update();
						mergedObjects->AddInputData(ObjNormals->GetOutput());
				}
			}
		}
	}
	else
	{
		
			cout << "I am where I should be in the save single actor function!" << endl;
			vtkPolyDataMapper *mapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
			if (mapper != NULL && vtkPolyData::SafeDownCast(mapper->GetInput()) != NULL)
			{
				vtkSmartPointer<vtkPolyData> toSave = vtkSmartPointer<vtkPolyData>::New();
				toSave->DeepCopy(vtkPolyData::SafeDownCast(mapper->GetInput()));
				if (position_mode == 1)
				{
					double ve_init_pos[3];;
					double ve_final_pos[3];
					vtkSmartPointer<vtkMatrix4x4> Mat = myActor->GetMatrix();


					for (vtkIdType i = 0; i < toSave->GetNumberOfPoints(); i++) {
						// for every triangle 
						toSave->GetPoint(i, ve_init_pos);
						mqMorphoDigCore::TransformPoint(Mat, ve_init_pos, ve_final_pos);

						toSave->GetPoints()->SetPoint((vtkIdType)i, ve_final_pos);
					}
				
				}
				
				// here rare case where we need to compute normals (save merged surface)! otherwise weird weird bug... 
				vtkSmartPointer<vtkPolyDataNormals> ObjNormals = vtkSmartPointer<vtkPolyDataNormals>::New();
				ObjNormals->SetInputData(toSave);
				ObjNormals->ComputePointNormalsOn();
				ObjNormals->ComputeCellNormalsOn();
				ObjNormals->AutoOrientNormalsOff();
				ObjNormals->ConsistencyOff();
				ObjNormals->Update();
				mergedObjects->AddInputData(ObjNormals->GetOutput());
		}

	}
	cout << "will call mergedobjects update" << endl;
	Ok = 1;
	mergedObjects->Update();


	vtkSmartPointer<vtkCleanPolyData> cleanPolyDataFilter = vtkSmartPointer<vtkCleanPolyData>::New();
	//
	cleanPolyDataFilter->SetInputData(mergedObjects->GetOutput());
	cleanPolyDataFilter->PieceInvariantOff();
	cleanPolyDataFilter->ConvertLinesToPointsOff();
	cleanPolyDataFilter->ConvertPolysToLinesOff();
	cleanPolyDataFilter->ConvertStripsToPolysOff();
	cleanPolyDataFilter->PointMergingOn();
	cleanPolyDataFilter->Update();


	if (save_norms == 0)
	{
		cleanPolyDataFilter->GetOutput()->GetPointData()->SetNormals(NULL);
		cleanPolyDataFilter->GetOutput()->GetCellData()->SetNormals(NULL);
	}
	if (scalarsToBeRemoved.size() > 0)
	{
		for (int i = 0; i < scalarsToBeRemoved.size(); i++)
		{
			cout << "Removed scalar:" << scalarsToBeRemoved.at(i) << endl;
			cleanPolyDataFilter->GetOutput()->GetPointData()->RemoveArray(scalarsToBeRemoved.at(i).c_str());
		}
	}

	// "RGB"
	if (RGBopt == 0)
	{
		// then do nothing, as we will keep RGB scalars (if those exist), and if they don't we do not care
	}
	else if (RGBopt ==1)
	{ 
		//remove RGB scalars if they exist
		cleanPolyDataFilter->GetOutput()->GetPointData()->RemoveArray("RGB");
	}
	else if (RGBopt == 2)
	{
		vtkSmartPointer<vtkUnsignedCharArray> newcolors =
			vtkSmartPointer<vtkUnsignedCharArray>::New();
		newcolors->SetNumberOfComponents(4);
		newcolors->SetNumberOfTuples(cleanPolyDataFilter->GetOutput()->GetNumberOfPoints());
		
		vtkIdType iRGB = 0;
		int nr, ng, nb, na;
		this->ActorCollection->InitTraversal();
		QString sActiveArray = this->Getmui_ActiveArray()->Name;
		QString none = QString("Solid color");
		QString RGB = QString("RGB");
		for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
		{
			vtkMDActor *myActor2 = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
			if (myActor2->GetSelected() == 1)
			{
				
				


				vtkPolyDataMapper *mapper = vtkPolyDataMapper::SafeDownCast(myActor2->GetMapper());
				if (mapper != NULL && vtkPolyData::SafeDownCast(mapper->GetInput()) != NULL)
				{
					// we define whether we have to create RGB from scalars/RGB/tags or from "global" color option.
					int RGB_fromglobal = 1;
					// if current active scalar is not the "none" one, and if the actor as current active scalar.
					//if (ActiveScalar != none && 	cleanPolyDataFilter->GetOutput()->GetPointData()->GetScalars(ActiveArray.toStdString().c_str()) != NULL)
					if (sActiveArray != none && 	mapper->GetInput()->GetPointData()->GetScalars(sActiveArray.toStdString().c_str()) != NULL)
					{
						RGB_fromglobal = 0;
						cout << "RGB from global =0" << endl;
					}

					//auto colors =	vtkSmartPointer<vtkUnsignedCharArray>::New();
					//colors->SetNumberOfComponents(4);
					//vtkUnsignedCharArray *colors = (vtkUnsignedCharArray*)cleanPolyDataFilter->GetOutput()->GetPointData()->GetScalars("RGB");
					//vtkFloatArray *currentFScalars = (vtkFloatArray*)cleanPolyDataFilter->GetOutput()->GetPointData()->GetScalars(ActiveArray.toStdString().c_str());
					//vtkDoubleArray *currentDScalars = (vtkDoubleArray*)cleanPolyDataFilter->GetOutput()->GetPointData()->GetScalars(ActiveArray.toStdString().c_str());
					vtkUnsignedCharArray *currentRGBcolors = vtkUnsignedCharArray::SafeDownCast(mapper->GetInput()->GetPointData()->GetScalars(sActiveArray.toStdString().c_str()));
					vtkFloatArray *currentFScalars = vtkFloatArray::SafeDownCast(mapper->GetInput()->GetPointData()->GetScalars(sActiveArray.toStdString().c_str()));
					vtkDoubleArray *currentDScalars = vtkDoubleArray::SafeDownCast(mapper->GetInput()->GetPointData()->GetScalars(sActiveArray.toStdString().c_str()));
					vtkIntArray *currentTags = vtkIntArray::SafeDownCast(mapper->GetInput()->GetPointData()->GetScalars(sActiveArray.toStdString().c_str()));
					if (currentRGBcolors == NULL) { cout << "ccurrentRGBcolors is null" << endl; }
					if (currentFScalars == NULL) { cout << "ccurrentFcolors is null" << endl; }
					if (currentDScalars == NULL) { cout << "ccurrentDcolors is null" << endl; }
					if (currentTags == NULL) { cout << "currentTags is null" << endl; }

						
					
					// on cherche la scalar active pour ce maillage
					vtkSmartPointer<vtkPolyData> toSave = mapper->GetInput();
					for (vtkIdType j = 0; j < toSave->GetNumberOfPoints(); j++)
					{
						//Fill nr ng nb with at solid Color by default... 
						nr = (unsigned char)(255 * myActor2->GetmColor()[0]);
						ng = (unsigned char)(255 * myActor2->GetmColor()[1]);
						nb = (unsigned char)(255 * myActor2->GetmColor()[2]);
						na = (unsigned char)(255 * myActor2->GetmColor()[3]);

						//1st case  : the current actor has not the currently active scalar.
						if (RGB_fromglobal == 1)
						{
							//do nothing here as nr ng nb na have already been correctly instantiated just above
						
						}
						else
						{
							
							// 2 cases : 
							//      A: active scalar = RGBlike => so we retake RGB... as we have started to do so earlier!
							if (this->mui_ActiveArray->DataType == VTK_UNSIGNED_CHAR
								&&  this->mui_ActiveArray->NumComp >= 3)
							{
								if (currentRGBcolors != NULL)
								{
									nr = (unsigned char)(currentRGBcolors->GetTuple(j)[0]);
									ng = (unsigned char)(currentRGBcolors->GetTuple(j)[1]);
									nb = (unsigned char)(currentRGBcolors->GetTuple(j)[2]);
									if (this->mui_ActiveArray->NumComp == 4)
									{
										na = (unsigned char)(currentRGBcolors->GetTuple(j)[3]);
									}
									else
									{
										na = 1;
									}
								}
								// else we keep global RGB as instantiated above.

							}							
							else
							{
								//@@@ wrong for tags????
								//    
					
								//      B: active scalar = 1 scalar or tag => we translate scalar or tag as RGB
								if ((this->Getmui_ActiveArray()->DataType == VTK_FLOAT ||
									this->Getmui_ActiveArray()->DataType == VTK_DOUBLE ||
									this->Getmui_ActiveArray()->DataType == VTK_INT ||
									this->Getmui_ActiveArray()->DataType == VTK_UNSIGNED_INT


									)
									&& this->Getmui_ActiveArray()->NumComp == 1)
								{
									

									double cscalar = 0;
									if (this->Getmui_ActiveArray()->DataType == VTK_INT ||
										this->Getmui_ActiveArray()->DataType == VTK_UNSIGNED_INT)
									{
										if (currentTags != NULL)
										{
											cscalar = (double)currentTags->GetTuple(j)[0];
										}
										if (j<10)
										{
											cout << "j=" << j << ", cscalar=" << cscalar << endl;
										}

									}
									else
									{
										if (currentFScalars != NULL)
										{
											cscalar = (double)currentFScalars->GetTuple(j)[0];
										}
										if (currentDScalars != NULL)
										{
											cscalar = currentDScalars->GetTuple(j)[0];
										}
									}
									
									
									// retrieve scalar value
									
									double cRGB[3];
									double cOpac=1;
									mapper->GetLookupTable()->GetColor(cscalar, cRGB);
									nr = (unsigned char)(255 * cRGB[0]);
									ng = (unsigned char)(255 * cRGB[1]);
									nb = (unsigned char)(255 * cRGB[2]);
									na= (unsigned char)(255 * mapper->GetLookupTable()->GetOpacity(cscalar));
									if (j<10)
									{
										cout << "j=" << j << ", cRGB[0]=" << cRGB[0] << endl;
									}
									// translate it as RGB


								}
								// else we keep global RGB as instantiated above.
								
							}
							
							

						}



						newcolors->InsertTuple4(iRGB, nr, ng, nb, na);
						iRGB++;

					}
				}

			}

		}
		newcolors->SetName("RGB");
		cleanPolyDataFilter->GetOutput()->GetPointData()->RemoveArray("RGB");
		cleanPolyDataFilter->GetOutput()->GetPointData()->AddArray(newcolors);

		// And YES...  NR NG NB should be a 0.. 255 value !!!! For the moment, color[] is a 0.0 ... 1.0 range
		//std::cout<<"currentScalars null "<<std::endl;
		
		

		//create a new RGB scalar!
		/*
		From MeshTools 1.3
		//std::cout<<"Update RGB of "<<this->name<<std::endl;
		vtkSmartPointer<vtkUnsignedCharArray> newcolors =
		vtkSmartPointer<vtkUnsignedCharArray>::New();
		newcolors->SetNumberOfComponents(4);
		newcolors->SetNumberOfTuples(this->GetNumberOfPoints());
		vtkFloatArray *currentScalars;
		currentScalars =(vtkFloatArray*)this->GetPointData()->GetScalars();  // couleur des courbure
		GLfloat cv[4];
		// now we create and populate RGB and Tags
		//std::cout<<"g_scalar_display_mode= "<<g_scalar_display_mode<<std::endl;
		//std::cout<<"g_tag_display_mode= "<<g_tag_display_mode<<std::endl;
		if (currentScalars!=NULL && (g_scalar_display_mode ==1 || g_tag_display_mode ==1))
		{
		//	std::cout<<"currentScalars not null "<<std::endl;
		}
		else
		{
		//	std::cout<<"currentScalars null "<<std::endl;
		}
		for (int i=0;i<this->GetNumberOfPoints();i++)	// for each vertex
		{
		int nr,ng,nb,na;

		//Colour scale!
		if (currentScalars!=NULL && (g_scalar_display_mode ==1 || g_tag_display_mode ==1))
		{
		//std::cout<<"currentScalars not null "<<std::endl;
		double cur_t = currentScalars->GetTuple(i)[0];

		ConvertScalarToColor((float)cur_t, cv, 1);
		nr= (unsigned char)(mround(255*cv[0]));
		ng= (unsigned char)(mround(255*cv[1]));
		nb= (unsigned char)(mround(255*cv[2]));
		na= (unsigned char)(mround(255*cv[3]));


		}
		else
		{
		// And YES...  NR NG NB should be a 0.. 255 value !!!! For the moment, color[] is a 0.0 ... 1.0 range
		//std::cout<<"currentScalars null "<<std::endl;
		nr= (unsigned char)(mround(255*this->color[0]));
		ng= (unsigned char)(mround(255*this->color[1]));
		nb= (unsigned char)(mround(255*this->color[2]));
		na= (unsigned char)(mround(255*this->blend));

		}
		if (i<500)
		{
		//std::cout<<"nr="<<nr<<", ng="<<ng<<", nb="<<nb<<std::endl;
		}
		////newcolors->InsertTuple3(i, (unsigned char)mround(255*cv[0]),(unsigned char)mround(255*cv[1]),(unsigned char)mround(255*cv[2]));
		//newcolors->InsertTuple3(i, 0,0,255);
		if(i<10)
		{
		//std::cout<<"nr="<<nr<<", ng="<<ng<<", nb="<<nb<<", na="<<na<<std::endl;
		}
		newcolors->InsertTuple4(i, nr,ng,nb,na);
		newcolors->InsertTuple4(i, nr,ng,nb,na);
		}
		newcolors->SetName("RGB");

		*/
	}
	if (file_type == 3)
	{
		vtkSmartPointer<vtkOBJWriter> Writer =
			vtkSmartPointer<vtkOBJWriter>::New();
		
		// test if "extension exists!"
		//
		std::size_t found = fileName.toStdString().find(OBJext);
		std::size_t found2 = fileName.toStdString().find(OBJext2);
		if (found == std::string::npos && found2 == std::string::npos)
		{
			fileName.append(".obj");

		}

		Writer->SetFileName(fileName.toLocal8Bit());
		Writer->SetInputData(cleanPolyDataFilter->GetOutput());
		//  stlWrite->Update();
		Writer->Write();

	}

	if (file_type == 0)
	{
		vtkSmartPointer<vtkSTLWriter> Writer =
			vtkSmartPointer<vtkSTLWriter>::New();
		if (write_type == 0)
		{
			Writer->SetFileTypeToBinary();
			
		}

		else
		{
			Writer->SetFileTypeToASCII();
		}
		// test if "extension exists!"
		//
		std::size_t found = fileName.toStdString().find(STLext);
		std::size_t found2 = fileName.toStdString().find(STLext2);
		if (found == std::string::npos && found2 == std::string::npos)
		{
			fileName.append(".stl");

		}
		
			Writer->SetFileName(fileName.toLocal8Bit());
			Writer->SetInputData(cleanPolyDataFilter->GetOutput());
			//  stlWrite->Update();
			Writer->Write();
		
	}

	if (file_type == 1 || file_type ==4 )
	{
		if (file_type == 1)
		{
			vtkSmartPointer<vtkPolyDataWriter> Writer =
				vtkSmartPointer<vtkPolyDataWriter>::New();
			if (write_type == 0)
			{
				Writer->SetFileTypeToBinary();

			}
			else
			{
				Writer->SetFileTypeToASCII();
			}
			std::size_t found = fileName.toStdString().find(VTKext);
			std::size_t found2 = fileName.toStdString().find(VTKext2);

			if (found == std::string::npos && found2 == std::string::npos)
			{
				fileName.append(".vtk");
			}
			

			Writer->SetFileName(fileName.toLocal8Bit());
			Writer->SetInputData(cleanPolyDataFilter->GetOutput());
			//  stlWrite->Update();
			Writer->Write();
		}
		else if (file_type == 4)
		{
			vtkSmartPointer<vtkXMLPolyDataWriter> Writer =
				vtkSmartPointer<vtkXMLPolyDataWriter>::New();
			
			
			std::size_t found3 = fileName.toStdString().find(VTKext3);
			std::size_t found4 = fileName.toStdString().find(VTKext4);
			if (found3 == std::string::npos && found4 == std::string::npos)
			{
				fileName.append(".vtp");
			}
			

			Writer->SetFileName(fileName.toLocal8Bit());
			Writer->SetInputData(cleanPolyDataFilter->GetOutput());
			//  stlWrite->Update();
			Writer->Write();
		}
		
	}

	if (file_type == 2)
	{
		vtkSmartPointer<vtkPLYWriter> Writer =
			vtkSmartPointer<vtkPLYWriter>::New();
		if (write_type == 0)
		{
			Writer->SetFileTypeToBinary();
			Writer->SetDataByteOrderToLittleEndian();
			//std::cout << "\nBinary Little endian";
		}
		else if (write_type == 1)
		{
			Writer->SetFileTypeToBinary();
			Writer->SetDataByteOrderToBigEndian();
			// std::cout << "\nBinary Big endian";

		}
		else
		{
			Writer->SetFileTypeToASCII();
			//std::cout << "\nASCII";
		}
		
		vtkPolyData *MyMergedObject = cleanPolyDataFilter->GetOutput();

		// Test if RGB scalar exists.
		vtkUnsignedCharArray* test = (vtkUnsignedCharArray*)MyMergedObject->GetPointData()->GetScalars("RGB");
		if (test != NULL)
		{
			// for the moment: useless piece of code... 
			 std::cout<<"Save Surface file: colors found inside merged object!"<<std::endl;

			vtkSmartPointer<vtkUnsignedCharArray> colors =
				vtkSmartPointer<vtkUnsignedCharArray>::New();
			colors->SetNumberOfComponents(4);
			colors = (vtkUnsignedCharArray*)MyMergedObject->GetPointData()->GetScalars("RGB");

			vtkSmartPointer<vtkUnsignedCharArray> colorsRGB =
				vtkSmartPointer<vtkUnsignedCharArray>::New();
			colorsRGB->SetNumberOfComponents(3);
			colorsRGB->SetNumberOfTuples(MyMergedObject->GetNumberOfPoints());
			for (int i = 0; i<MyMergedObject->GetNumberOfPoints(); i++)	// for each vertex 
			{			

				int nr, ng, nb;

				nr = colors->GetTuple(i)[0];
				ng = colors->GetTuple(i)[1];
				nb = colors->GetTuple(i)[2];


				colorsRGB->InsertTuple3(i, nr, ng, nb);
			}
			colorsRGB->SetName("Colors");
			MyMergedObject->GetPointData()->AddArray(colorsRGB);
			//colors->SetName("Colors");	  
			std::cout << "Colors num of tuples :" << colors->GetNumberOfTuples() << std::endl;
			for (int i = 0; i<10; i++)
			{
				//std::cout<<"RGB stuff i:"<<colors->GetTuple(i)[0]<<","<<colors->GetTuple(i)[1]<<","<<colors->GetTuple(i)[2]<<std::endl;
				//std::cout<<"RGB "<<i<<"="<<cur_r<<","<<cur_g<<","<<cur_b<<std::endl;
			}
			Writer->SetArrayName("Colors");
		}
		else
		{

			cout << "Save surface file: No 4 colors found inside merged object" << endl;

		}


	


		std::size_t found = fileName.toStdString().find(PLYext);
		std::size_t found2 = fileName.toStdString().find(PLYext2);
		if (found == std::string::npos && found2 == std::string::npos)
		{
			fileName.append(".ply");
		}
		ifstream file(fileName.toLocal8Bit());
		
		
			Writer->SetFileName(fileName.toLocal8Bit());
			//
			Writer->SetInputData(MyMergedObject);
			//Writer->SetInputData((vtkPolyData*)My_Obj);	
			Writer->Write();
		
		//std::cout << "\nWriter should have written : "<<filename.c_str();
	}
	return 1;
}

void mqMorphoDigCore::Setmui_LastUsedDir(QString dir) { this->mui_LastUsedDir = dir; }

QString mqMorphoDigCore::Getmui_LastUsedDir() { return this->mui_LastUsedDir; }
void mqMorphoDigCore::Setmui_X1Label(QString label) { this->mui_X1Label = label;
//cout << "this->mui_X1Label " << this->mui_X1Label.toStdString() << endl;
}
QString mqMorphoDigCore::Getmui_DefaultX1Label() { return this->mui_DefaultX1Label; }
QString mqMorphoDigCore::Getmui_X1Label() { return this->mui_X1Label; }


void mqMorphoDigCore::Setmui_X2Label(QString label) { this->mui_X2Label = label; 
//cout << "this->mui_X2Label " << this->mui_X2Label.toStdString() << endl;
}
QString mqMorphoDigCore::Getmui_DefaultX2Label() { return this->mui_DefaultX2Label; }
QString mqMorphoDigCore::Getmui_X2Label() { return this->mui_X2Label; }

void mqMorphoDigCore::Setmui_Y1Label(QString label) { this->mui_Y1Label = label; 
//cout << "this->mui_Y1Label " << this->mui_Y1Label.toStdString() << endl;
}
QString mqMorphoDigCore::Getmui_DefaultY1Label() { return this->mui_DefaultY1Label; }
QString mqMorphoDigCore::Getmui_Y1Label() { return this->mui_Y1Label; }


void mqMorphoDigCore::Setmui_Y2Label(QString label) { this->mui_Y2Label = label; 

//cout << "this->mui_Y2Label " << this->mui_Y2Label.toStdString() << endl;
}
QString mqMorphoDigCore::Getmui_DefaultY2Label() { return this->mui_DefaultY2Label; }
QString mqMorphoDigCore::Getmui_Y2Label() { return this->mui_Y2Label; }

void mqMorphoDigCore::Setmui_Z1Label(QString label) { this->mui_Z1Label = label; }
QString mqMorphoDigCore::Getmui_DefaultZ1Label() { return this->mui_DefaultZ1Label; }
QString mqMorphoDigCore::Getmui_Z1Label() { return this->mui_Z1Label; }


void mqMorphoDigCore::Setmui_Z2Label(QString label) { this->mui_Z2Label = label; }
QString mqMorphoDigCore::Getmui_DefaultZ2Label() { return this->mui_DefaultZ2Label; }
QString mqMorphoDigCore::Getmui_Z2Label() { return this->mui_Z2Label; }


void mqMorphoDigCore::LandmarksPushBackOrReorient(int mode)
{
	//mode0: pushback
	//mode 1: reorient

	//strategy
	//1) create a merged object of all opened objects... I guess normals should be reoriented!
	//2) create a kdtree on the merged object
	//3) call "loop" function foar each landmark list, passing the Kdtree as an argument: find closest vertex 

	vtkSmartPointer<vtkAppendPolyData> mergedObjects = vtkSmartPointer<vtkAppendPolyData>::New();
	int Ok = 1;
	
		//cout << "myActor is null" << endl;
	cout << "Try to create merged object landmarks" << endl;
	if (this->ActorCollection->GetNumberOfItems() == 0) { return; }

		this->ActorCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
		{
		vtkMDActor *myActor2 = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
						
			vtkPolyDataMapper *mapper = vtkPolyDataMapper::SafeDownCast(myActor2->GetMapper());
			if (mapper != NULL && vtkPolyData::SafeDownCast(mapper->GetInput()) != NULL)
			{
				vtkSmartPointer<vtkPolyData> toSave = vtkSmartPointer<vtkPolyData>::New();
				toSave->DeepCopy(vtkPolyData::SafeDownCast(mapper->GetInput()));
				double ve_init_pos[3];;
				double ve_final_pos[3];

				double ven_init_pos[3];;
				double ven_final_pos[3];
				vtkSmartPointer<vtkMatrix4x4> Mat = myActor2->GetMatrix();
				auto impactedNorms = vtkFloatArray::SafeDownCast(toSave->GetPointData()->GetNormals());


				for (vtkIdType i = 0; i < toSave->GetNumberOfPoints(); i++) {
					// for every triangle 
					toSave->GetPoint(i, ve_init_pos);
					double *mImpactedptn = impactedNorms->GetTuple3(i);
					ven_init_pos[0] = mImpactedptn[0];
					ven_init_pos[1] = mImpactedptn[1];
					ven_init_pos[2] = mImpactedptn[2];

					mqMorphoDigCore::TransformPoint(Mat, ve_init_pos, ve_final_pos);
					
					mqMorphoDigCore::RotateNorm(Mat, ven_init_pos, ven_final_pos);
					if (i < 50 || i== 14044)
					{
						cout << "Pt init:" << endl;
						cout << ve_init_pos[0] << ", " << ve_init_pos[1] << "," << ve_init_pos[2] << endl;
						cout << "Pt final:" << endl;
						cout << ve_final_pos[0] << ", " << ve_final_pos[1] << "," << ve_final_pos[2] << endl;
						cout << "Ori init:" << endl;
						cout << ven_init_pos[0] << ", " << ven_init_pos[1] << "," << ven_init_pos[2] << endl;
						cout << "Ori final:" << endl;
						cout << ven_final_pos[0] << ", " << ven_final_pos[1] << "," << ven_final_pos[2] << endl;

					}
					
					toSave->GetPoints()->SetPoint((vtkIdType)i, ve_final_pos);
					impactedNorms->SetTuple3(i, ve_final_pos[0], ve_final_pos[1], ve_final_pos[2]);
				}
				mergedObjects->AddInputData(toSave);
			}

				
			
		}		
	
	cout << "Update merged object" << endl;
	mergedObjects->Update();
	//Other case where we need to recompute normals outside "vtkMDActor"
	vtkSmartPointer<vtkPolyDataNormals> ObjNormals = vtkSmartPointer<vtkPolyDataNormals>::New();
	ObjNormals->SetInputData(mergedObjects->GetOutput());
	ObjNormals->ComputePointNormalsOn();
	ObjNormals->ComputeCellNormalsOn();
	//ObjNormals->AutoOrientNormalsOff();
	ObjNormals->ConsistencyOff();

	ObjNormals->Update();

	vtkSmartPointer<vtkCleanPolyData> cleanPolyDataFilter = vtkSmartPointer<vtkCleanPolyData>::New();
	cleanPolyDataFilter->SetInputData(ObjNormals->GetOutput());
	cleanPolyDataFilter->PieceInvariantOff();
	cleanPolyDataFilter->ConvertLinesToPointsOff();
	cleanPolyDataFilter->ConvertPolysToLinesOff();
	cleanPolyDataFilter->ConvertStripsToPolysOff();
	cleanPolyDataFilter->PointMergingOn();
	cleanPolyDataFilter->Update();

	


	//Just some tests
	double ven_init_pos[3];;
	auto impactedNorms = vtkFloatArray::SafeDownCast(cleanPolyDataFilter->GetOutput()->GetPointData()->GetNormals());
	double *mImpactedptn = impactedNorms->GetTuple3(14044);
	ven_init_pos[0] = mImpactedptn[0];
	ven_init_pos[1] = mImpactedptn[1];
	ven_init_pos[2] = mImpactedptn[2];
	cout << "Normal 14044 of merged object:" << endl;
	cout << ven_init_pos[0] << ", " << ven_init_pos[1] << "," << ven_init_pos[2] << endl;

	// 2 : build kdtree on merged object
	vtkSmartPointer<vtkKdTreePointLocator> kDTree =
		vtkSmartPointer<vtkKdTreePointLocator>::New();
	cout << "Try to set KDTree" << endl;
	kDTree->SetDataSet(cleanPolyDataFilter->GetOutput());
	cout << "Try to build locator" << endl;
	kDTree->BuildLocator();
	
	std::string action;
	if (mode == 0)
	{
		action = "Push back landmarks on surface";
	}
	else
	{
		action = "Reorient landmarks according to closest point's normal";
	}

	cout << "Begin undo set" << endl;
	int mCount = BEGIN_UNDO_SET(action);

	//double Radius = this->ActorCollection->GetBoundingBoxLength()/20;
	cout << "Normal landmarks" << endl;
	this->LandmarkPushBackOrReorient(mode, this->NormalLandmarkCollection, kDTree, cleanPolyDataFilter->GetOutput(), mCount);
	cout << "Target landmarks" << endl;
	this->LandmarkPushBackOrReorient(mode, this->TargetLandmarkCollection, kDTree, cleanPolyDataFilter->GetOutput(), mCount);
	cout << "Node landmarks" << endl;
	this->LandmarkPushBackOrReorient(mode, this->NodeLandmarkCollection, kDTree, cleanPolyDataFilter->GetOutput(), mCount);
	cout << "Handle landmarks" << endl;
	this->LandmarkPushBackOrReorient(mode, this->HandleLandmarkCollection, kDTree, cleanPolyDataFilter->GetOutput(), mCount);
	cout << "Flag landmarks" << endl;
	this->LandmarkPushBackOrReorient(mode, this->FlagLandmarkCollection, kDTree, cleanPolyDataFilter->GetOutput(), mCount);

	END_UNDO_SET();
	cout << "Done!" << endl;
	//kDTree->FindPointsWithinRadius(Radius, ve_imp_final_pos, observedNeighbours);
}
void mqMorphoDigCore::LandmarkPushBackOrReorient(int mode, vtkSmartPointer<vtkLMActorCollection> LmkCollection, vtkSmartPointer<vtkKdTreePointLocator> kDTree, vtkSmartPointer<vtkPolyData> PD, int mcount)
{

	//mode0: pushback
	//mode 1: reorient
	LmkCollection->InitTraversal();
	cout << "Get impacted norms" << endl;
	auto impactedNorms = vtkFloatArray::SafeDownCast(PD->GetPointData()->GetNormals());

	for (vtkIdType i = 0; i < LmkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myLMK = vtkLMActor::SafeDownCast(LmkCollection->GetNextActor());
		
		if (myLMK->GetSelected() == 1 )
		{

			double lmpos[3];
			myLMK->GetLMOrigin(lmpos);
			double lmori[3];
			myLMK->GetLMOrientation(lmori);
			cout << "Try to find closest point!" << endl;
			vtkIdType k = kDTree->FindClosestPoint(lmpos);
			cout << k << " is the closest"<< endl;
			
			myLMK->SaveState(mcount);
			
			if (mode == 0)
			{
				double ve_final_pos[3];
				cout << "Get point " <<k << endl;
				PD->GetPoint(k, ve_final_pos);
				cout << ve_final_pos[0] << ", " << ve_final_pos[1] << "," << ve_final_pos[2] << endl;
				myLMK->SetLMOrigin(ve_final_pos);
			}
			else
			{
				cout << "Get norm " << k << endl;
				double *mImpactedptn = impactedNorms->GetTuple3(k);
				double ven_final_pos[3];
				ven_final_pos[0] = mImpactedptn[0];
				ven_final_pos[1] = mImpactedptn[1];
				ven_final_pos[2] = mImpactedptn[2];
				cout << "Ori init:" << endl;
				cout << lmori[0] << ", " << lmori[1] << "," << lmori[2] << endl;
				cout << "Ori final:" << endl;
				cout << ven_final_pos[0] << ", " << ven_final_pos[1]<<","<< ven_final_pos[2] << endl;
				myLMK->SetLMOrientation(ven_final_pos);
			}
			myLMK->Modified();
			LmkCollection->Modified();
		
			
			
			//cpt++;
		}

	}

	
}
void mqMorphoDigCore::LandmarksReorient()
{
//mqMorphoDigCore::instance()->UpdateFirstSelectedLandmark(pos, norm);
	vtkIdType num_lmk = 0;
	num_lmk = mqMorphoDigCore::instance()->getNormalLandmarkCollection()->GetNumberOfSelectedActors();
	num_lmk += mqMorphoDigCore::instance()->getTargetLandmarkCollection()->GetNumberOfSelectedActors();
	num_lmk += mqMorphoDigCore::instance()->getNodeLandmarkCollection()->GetNumberOfSelectedActors();
	num_lmk += mqMorphoDigCore::instance()->getHandleLandmarkCollection()->GetNumberOfSelectedActors();
	if (num_lmk == 0) {
		QMessageBox msgBox;
		msgBox.setText("No landmark selected. Please select at least one landmark to use this option.");
		msgBox.exec();
		return;
	}
	this->LandmarksPushBackOrReorient(1);
	this->Render();
}

void mqMorphoDigCore::LandmarksPushBack()
{
	vtkIdType num_lmk = 0;
	num_lmk = mqMorphoDigCore::instance()->getNormalLandmarkCollection()->GetNumberOfSelectedActors();
	num_lmk += mqMorphoDigCore::instance()->getTargetLandmarkCollection()->GetNumberOfSelectedActors();
	num_lmk += mqMorphoDigCore::instance()->getNodeLandmarkCollection()->GetNumberOfSelectedActors();
	num_lmk += mqMorphoDigCore::instance()->getHandleLandmarkCollection()->GetNumberOfSelectedActors();
	if (num_lmk == 0) {
		QMessageBox msgBox;
		msgBox.setText("No landmark selected. Please select at least one landmark to use this option.");
		msgBox.exec();
		return;
	}
	this->LandmarksPushBackOrReorient(0);
	this->Render();
}
void mqMorphoDigCore::LandmarksMoveUp()
{
	vtkIdType num_lmk = 0;
	num_lmk = mqMorphoDigCore::instance()->getNormalLandmarkCollection()->GetNumberOfSelectedActors();
	num_lmk += mqMorphoDigCore::instance()->getTargetLandmarkCollection()->GetNumberOfSelectedActors();
	num_lmk += mqMorphoDigCore::instance()->getNodeLandmarkCollection()->GetNumberOfSelectedActors();
	num_lmk += mqMorphoDigCore::instance()->getHandleLandmarkCollection()->GetNumberOfSelectedActors();
	if (num_lmk == 0) {
		QMessageBox msgBox;
		msgBox.setText("No landmark selected. Please select at least one landmark to use this option.");
		msgBox.exec();
		return;
	}
	this->NormalLandmarkCollection->LandmarksMoveUp();
	this->TargetLandmarkCollection->LandmarksMoveUp();
	this->NodeLandmarkCollection->LandmarksMoveUp();
	this->HandleLandmarkCollection->LandmarksMoveUp();
	this->Render();
	
}
void mqMorphoDigCore::LandmarksMoveDown()
{
	vtkIdType num_lmk = 0;
	num_lmk = mqMorphoDigCore::instance()->getNormalLandmarkCollection()->GetNumberOfSelectedActors();
	num_lmk += mqMorphoDigCore::instance()->getTargetLandmarkCollection()->GetNumberOfSelectedActors();
	num_lmk += mqMorphoDigCore::instance()->getNodeLandmarkCollection()->GetNumberOfSelectedActors();
	num_lmk += mqMorphoDigCore::instance()->getHandleLandmarkCollection()->GetNumberOfSelectedActors();
	if (num_lmk == 0) {
		QMessageBox msgBox;
		msgBox.setText("No landmark selected. Please select at least one landmark to use this option.");
		msgBox.exec();
		return;
	}
	this->NormalLandmarkCollection->LandmarksMoveDown();
	this->TargetLandmarkCollection->LandmarksMoveDown();
	this->NodeLandmarkCollection->LandmarksMoveDown();
	this->HandleLandmarkCollection->LandmarksMoveDown();
	this->Render();
	
}

void mqMorphoDigCore::SelectLandmarkRange(int start, int end, int lm_type)
{
	//cout << "Select landmark range: " << start << ", " << end << ", " << lm_type << endl;
	vtkSmartPointer<vtkLMActorCollection> myColl = vtkSmartPointer<vtkLMActorCollection>::New();
	if (lm_type==0)
	{ 
		myColl = this->NormalLandmarkCollection;
	}
	else if (lm_type == 1)
	{
		myColl = this->TargetLandmarkCollection;
	}
	else if (lm_type == 2)
	{
		//cout << "Node landmark!" << endl;

		
		myColl = this->NodeLandmarkCollection;
		//cout << "myColl->GetNumberOfItems()=" << myColl->GetNumberOfItems() << endl;
	}
	else //if (lm_type == 3)
	{
		myColl = this->HandleLandmarkCollection;
	}
	int coll_modified = 0;

	myColl->InitTraversal();
	for (vtkIdType i = 0; i < myColl->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(myColl->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			myActor->SetSelected(0);
			myActor->SetChanged(1);
			coll_modified = 1;
			//cout << "Unselect one landmark:" << myActor->GetLMNumber() << endl;

		}
	}
	myColl->InitTraversal();
	for (vtkIdType i = 0; i < myColl->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(myColl->GetNextActor());
		int mynum = myActor->GetLMNumber();
		//cout << "Actor number: " << mynum << endl;
		if (myActor->GetLMNumber()<=end && myActor->GetLMNumber() >=start)
		{

			myActor->SetSelected(1);
			myActor->SetChanged(1);
			coll_modified = 1;
			//cout << "Select one landmark:" << myActor->GetLMNumber() << endl;
		}
	}
	if (coll_modified == 1) {
		myColl->Modified();
	}
	this->Render();
}

vtkSmartPointer<vtkLookupTable> mqMorphoDigCore::GetTagLut()
{
	return this->TagLut;
}
void mqMorphoDigCore::CreateSurfaceUndoSet(int count_tot)
{
	emit this->actorsMightHaveChanged();
	this->Initmui_ExistingArrays();
	std::string action = "Load surface file(s)";
	int mCount = BEGIN_UNDO_SET(action);
	this->getActorCollection()->CreateLoadUndoSet(mCount, count_tot);
	END_UNDO_SET();

	this->AdjustCameraAndGrid();
	//cout << "camera and grid adjusted" << endl;

	if (this->Getmui_AdjustLandmarkRenderingSize() == 1)
	{
		this->UpdateLandmarkSettings();
	}
	this->Render();
	this->matchTagMapToActorCollection();
}
void mqMorphoDigCore::CreateLandmarkUndoSet(int lmk_type, int count_tot, int count_norm, int count_targ, int count_node, int count_handle)
{

	vtkLMActorCollection *Collection =NULL;
	std::string action;
	if (lmk_type == NORMAL_LMK)
	{
		 action = "Create Normal landmark(s)";
		 Collection = this->NormalLandmarkCollection;
		
	}
	else if (lmk_type == TARGET_LMK)
	{
		action = "Create Target landmark(s)";
		Collection = this->TargetLandmarkCollection;
	}
	else if (lmk_type == NODE_LMK)
	{
		action = "Create Node landmark(s)";
		Collection = this->NodeLandmarkCollection;
	}
	else if (lmk_type == HANDLE_LMK)
	{
		action = "Create Handle landmark(s)";
		Collection = this->HandleLandmarkCollection;
		
	}
	else if (lmk_type == FLAG_LMK)
	{
		action = "Create Flag(s)";
		Collection = this->FlagLandmarkCollection;
		
	}

	if (Collection != NULL)
	{
		int mCount = BEGIN_UNDO_SET(action);
		Collection->CreateLoadUndoSet(mCount, count_tot);
		END_UNDO_SET();
	}
	else
	{
		// Create a landmark undo set for all!!!!
		action = "Create Landmarks";
		int mCount = BEGIN_UNDO_SET(action);
		this->NormalLandmarkCollection->CreateLoadUndoSet(mCount, count_norm);
		this->TargetLandmarkCollection->CreateLoadUndoSet(mCount, count_targ);
		this->NodeLandmarkCollection->CreateLoadUndoSet(mCount, count_node);
		this->HandleLandmarkCollection->CreateLoadUndoSet(mCount, count_handle);
		//this->FlagLandmarkCollection->CreateLoadUndoSet(mCount, 1);
		END_UNDO_SET();
	}
}
void mqMorphoDigCore::CreateLandmark(double coord[3], double ori[3], int lmk_type, int node_type)
{
	
	//cout << "CreateLandmark:" << lmk_type <<", "<< node_type << endl;
	// lmk_type : 0 for normal landmarks
	// lmk_type : 1 for target landmarks
	// lmk_type : 2 for curve nodes
	// lmk_type : 3 for curve handles
	// lmk_type : 4 for flag landmark


	//node_type: only used if mode ==2, curve node
	//node_type: 1 curve starting point 
	//node_type: 0 normal node
	//node_type: 2 curve milestone
	//node_type: 3 connect to preceding starting point

	VTK_CREATE(vtkLMActor, myLM);
	int num = 0;
	myLM->SetLMType(lmk_type);

	if (lmk_type == NORMAL_LMK)
	{
		num = this->NormalLandmarkCollection->GetNextLandmarkNumber();
	}
	else if (lmk_type == TARGET_LMK)
	{
		num = this->TargetLandmarkCollection->GetNextLandmarkNumber();
	}
	else if (lmk_type == NODE_LMK)
	{
		num = this->NodeLandmarkCollection->GetNextLandmarkNumber();
	}
	else if (lmk_type == HANDLE_LMK)
	{
		num = this->HandleLandmarkCollection->GetNextLandmarkNumber();
	}
	else if (lmk_type== FLAG_LMK)
	{
		num = this->FlagLandmarkCollection->GetNextLandmarkNumber();
	}

	myLM->SetLMOriginAndOrientation(coord, ori);
	//myLM->SetLMOrigin(pos[0], pos[1], pos[2]);
	//myLM->SetLMOrientation(norm[0], norm[1], norm[2]);
	if (lmk_type != FLAG_LMK)
	{
		if (this->Getmui_AdjustLandmarkRenderingSize() == 1)
		{
			myLM->SetLMSize(this->AdjustedLandmarkSize());
		}
		else
		{
			myLM->SetLMSize(this->Getmui_LandmarkRenderingSize());
		}
	}
	else
	{
		myLM->SetLMSize(this->Getmui_FlagRenderingSize());
	}
	
	if (lmk_type == NORMAL_LMK)
	{
		myLM->SetLMType(NORMAL_LMK);
	}
	else if (lmk_type== TARGET_LMK)
	{
		myLM->SetLMType(TARGET_LMK);
	}
	else if (lmk_type == NODE_LMK)
	{
		// to do : 
		myLM->SetLMType(NODE_LMK);
		if (node_type>-1)
		{ 
			//lmtype: 1 curve starting point
			//lmtype: 0 normal node
			//lmtype: 2 curve milestone
			//lmtype: 3 connect to preceding starting point
			if (node_type == NORMAL_NODE) { myLM->SetLMType(NODE_LMK);  myLM->SetLMNodeType(NORMAL_NODE);	}
			if (node_type == STARTING_NODE) { myLM->SetLMType(NODE_LMK);  myLM->SetLMNodeType(STARTING_NODE); }
			if (node_type == MILESTONE_NODE) { myLM->SetLMType(NODE_LMK);  myLM->SetLMNodeType(MILESTONE_NODE); }
			if (node_type == CONNECT_NODE) { myLM->SetLMType(NODE_LMK);  myLM->SetLMNodeType(CONNECT_NODE); }
			


		}
		else
		{			
			if (num > 1)
			{
				vtkLMActor *myPrecedingLM = NULL;
				//@implement GETLMBefore(num)
				//vtkLMActor *myPrecedingLM = this->NodeLandmarkCollection->GetLMBefore(num);
				if (myPrecedingLM != NULL)
				{
					if (myPrecedingLM->GetLMNodeType() == CONNECT_NODE)// if curve ending point
					{
						myLM->SetLMNodeType(STARTING_NODE); // curve starting point
					}
					else
					{
						myLM->SetLMNodeType(NORMAL_NODE); // curve conventional node
					}
				}
				else
				{
					myLM->SetLMNodeType(NORMAL_NODE); // curve conventional node
				}
			}
			else // num ==1
			{
				myLM->SetLMNodeType(STARTING_NODE); //curve starting point
			}
		}
	}
	else if(lmk_type == HANDLE_LMK)
	{
		myLM->SetLMType(HANDLE_LMK); //curve handle
	}
	else 
	{
		//cout << "Set LM TYPE TO FLAG!" << endl;
		myLM->SetLMType(FLAG_LMK);
		std::string flag = "Flag ";
		std::string flag_num = flag + std::to_string(num);
		myLM->SetLMText(flag_num);
	}
	
	myLM->SetLMNumber(num);
	
	myLM->SetLMBodyType(this->Getmui_LandmarkBodyType());
	
	myLM->SetSelected(0);

	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(myLM->getLMBody());
	mapper->Update();
	myLM->SetMapper(mapper);


	//myLM->PrintSelf(cout, vtkIndent(1));
	

	if (lmk_type == NORMAL_LMK)
	{
		
		
		this->NormalLandmarkCollection->AddItem(myLM);
		//emit this->actorsMightHaveChanged();
		
		this->NodeLandmarkCollection->ReorderLandmarks();
		this->NormalLandmarkCollection->SetChanged(1);
		//std::string action = "Create Normal landmark";
		//int mCount = BEGIN_UNDO_SET(action);
		//this->getNormalLandmarkCollection()->CreateLoadUndoSet(mCount, 1);
		//END_UNDO_SET();
		
	}
	else if (lmk_type == TARGET_LMK)
	{

		this->TargetLandmarkCollection->AddItem(myLM);
		//emit this->actorsMightHaveChanged();
		this->NodeLandmarkCollection->ReorderLandmarks();
		this->TargetLandmarkCollection->SetChanged(1);
		//std::string action = "Create Target landmark";
		//int mCount = BEGIN_UNDO_SET(action);
		//this->getTargetLandmarkCollection()->CreateLoadUndoSet(mCount, 1);
		//END_UNDO_SET();
		
	}
	else if (lmk_type == NODE_LMK)
	{
		this->NodeLandmarkCollection->AddItem(myLM);
		//emit this->actorsMightHaveChanged();
		this->NodeLandmarkCollection->ReorderLandmarks();
		this->NodeLandmarkCollection->SetChanged(1);
		//std::string action = "Create Curve Node";
		//int mCount = BEGIN_UNDO_SET(action);
		//this->getNodeLandmarkCollection()->CreateLoadUndoSet(mCount, 1);
		//END_UNDO_SET();
		
	}
	else if (lmk_type == HANDLE_LMK)
	{
		this->HandleLandmarkCollection->AddItem(myLM);
		//emit this->actorsMightHaveChanged();
		this->HandleLandmarkCollection->ReorderLandmarks();
		this->HandleLandmarkCollection->SetChanged(1);
		//std::string action = "Create Curve Handle";
		//int mCount = BEGIN_UNDO_SET(action);
		//this->getHandleLandmarkCollection()->CreateLoadUndoSet(mCount, 1);
		//END_UNDO_SET();
	}
	else if (lmk_type == FLAG_LMK)
	{
		this->FlagLandmarkCollection->AddItem(myLM);	
		//emit this->actorsMightHaveChanged();
		this->FlagLandmarkCollection->SetChanged(1);
		//std::string action = "Create Flag Landmark";
		//int mCount = BEGIN_UNDO_SET(action);
		//this->getFlagLandmarkCollection()->CreateLoadUndoSet(mCount, 1);
		//END_UNDO_SET();
	}
	
	
}
void mqMorphoDigCore::UpdateFirstSelectedLandmark(double coord[3], double ori[3])
{

	int lmk_type = this->Getmui_LandmarkMode();
	vtkSmartPointer<vtkLMActor> MyFirst = NULL;
	vtkSmartPointer<vtkLMActorCollection> myColl = NULL;
	if (lmk_type == NORMAL_LMK)
	{
		myColl = this->NormalLandmarkCollection;				

	}
	else if (lmk_type == TARGET_LMK)
	{
		myColl = this->TargetLandmarkCollection;		
	}
	else if (lmk_type == NODE_LMK)
	{
		myColl = this->NodeLandmarkCollection;
		
	}
	else if (lmk_type == HANDLE_LMK)
	{
		myColl = this->HandleLandmarkCollection;
	}
	else //if (lmk_type == FLAG_LMK)
	{
		myColl = this->FlagLandmarkCollection;
	}

	MyFirst = myColl->GetFirstSelectedActor();

	if (MyFirst == NULL)
	{
		myColl = this->NormalLandmarkCollection;
		MyFirst = myColl->GetFirstSelectedActor();
		if (MyFirst == NULL)
		{
			myColl = this->TargetLandmarkCollection;
			MyFirst = myColl->GetFirstSelectedActor();
			if (MyFirst == NULL)
			{
				myColl = this->NodeLandmarkCollection;
				MyFirst = myColl->GetFirstSelectedActor();
				if (MyFirst == NULL)
				{
					myColl = this->HandleLandmarkCollection;
					MyFirst = myColl->GetFirstSelectedActor();
					if (MyFirst == NULL)
					{
						myColl = this->FlagLandmarkCollection;
						MyFirst = myColl->GetFirstSelectedActor();
						

					}
				}
			}
		}
	}
		
	if (MyFirst != NULL)
	{
		std::string action = "Update xyz and nx ny nz coordinates ";
		int mCount = BEGIN_UNDO_SET(action);
			MyFirst->SaveState(mCount);
			MyFirst->SetLMOrigin(coord);
			MyFirst->SetLMOrientation(ori);
			MyFirst->Modified();
			myColl->Modified();
		END_UNDO_SET();
	}


}
void mqMorphoDigCore::ResetOrientationHelperLabels()
{
	this->SetOrientationHelperLabels(this->mui_X1Label.toStdString(), this->mui_X2Label.toStdString(), this->mui_Y1Label.toStdString(), this->mui_Y2Label.toStdString(), this->mui_Z1Label.toStdString(), this->mui_Z2Label.toStdString());

}
void mqMorphoDigCore::SetOrientationHelperLabels(std::string X1, std::string X2, std::string Y1, std::string Y2, std::string Z1, std::string Z2 )
{
	vtkSmartPointer<vtkOrientationHelperActor> axes = vtkOrientationHelperActor::SafeDownCast(this->OrientationHelperWidget->GetOrientationMarker());
	axes->SetXAxisLabelText(X1.c_str());
	axes->SetX2AxisLabelText(X2.c_str());
	axes->SetYAxisLabelText(Y1.c_str());
	axes->SetY2AxisLabelText(Y2.c_str());
	axes->SetZAxisLabelText(Z1.c_str());
	axes->SetZ2AxisLabelText(Z2.c_str());
}
void mqMorphoDigCore::InitializeOrientationHelper()
{
	vtkSmartPointer<vtkOrientationHelperActor> axes =
		vtkSmartPointer<vtkOrientationHelperActor>::New();

	//

	//vtkOrientationMarkerWidget* widget = vtkOrientationMarkerWidget::New();
	// Does not work with a smart pointer, can't figure out why


	this->OrientationHelperWidget->SetOutlineColor(0.9300, 0.5700, 0.1300);
	this->OrientationHelperWidget->SetOrientationMarker(axes);
	this->OrientationHelperWidget->SetDefaultRenderer(this->getRenderer());
	this->OrientationHelperWidget->SetInteractor(this->RenderWindow->GetInteractor());
	this->OrientationHelperWidget->SetViewport(0.0, 0.0, 0.2, 0.2);
	this->OrientationHelperWidget->SetEnabled(1);
	this->OrientationHelperWidget->InteractiveOff();
	this->OrientationHelperWidget->PickingManagedOn();
	this->ResetOrientationHelperLabels();


}
void mqMorphoDigCore::SetMainWindow(QMainWindow *_mainWindow)
{
	this->MainWindow = _mainWindow;
}
void mqMorphoDigCore::InitStatusBar()
{
	this->myRAMThread = new QRAMThread();
	
	this->myRAMProgressBar = new QProgressBar();
	QStatusBar *statusBar = this->GetMainWindow()->statusBar();
	QLabel *mode = new QLabel("My ram");
	

	mode->setMinimumSize(mode->sizeHint());
	mode->setAlignment(Qt::AlignCenter);
	mode->setText("Used RAM:");
	mode->setToolTip("Currently used ram.");
	statusBar->addPermanentWidget(mode);
	this->myRAMProgressBar->setMaximumWidth(50);
	statusBar->addPermanentWidget(this->myRAMProgressBar);
	connect(this->myRAMThread, SIGNAL(usedRAM(int)), this, SLOT(slotRAMProgressBar(int)));
	this->myRAMThread->start();
}

QMainWindow* mqMorphoDigCore::GetMainWindow() {
	return this->MainWindow	;
}
void mqMorphoDigCore::SetProjectWindow(QMainWindow *_projectWindow)
{
	this->ProjectWindow = _projectWindow;
}
QMainWindow* mqMorphoDigCore::GetProjectWindow() {
	return this->ProjectWindow;
}
//Called to repplace camera and grid positions when switching from "orange grid mode" to "blue grid mode"
//= when camera focalpoint and grid center are changed between 0,0,0 and COM of all opened meshes.

void mqMorphoDigCore::GetCenterOfMass(double center[3])
{
	double newcamerafocalpoint[3] = { 0,0,0 };
	double actorsCOM[3] = { 0,0,0 };
	double volumesCOM[3] = { 0,0,0 };
	this->getActorCollection()->GetCenterOfMass(actorsCOM);
	cout << "actorsCOM:" << actorsCOM[0] << "," << actorsCOM[1] << "," << actorsCOM[2] << "," << endl;
	
	this->getActorCollection()->GetCenterOfMass(newcamerafocalpoint);
	this->getVolumeCollection()->GetCenterOfMass(volumesCOM);
	cout << "volumesCOM:" << volumesCOM[0] << "," << volumesCOM[1] << "," << volumesCOM[2] << "," << endl;
	if (this->getVolumeCollection()->GetNumberOfItems() > 0)
	{
		newcamerafocalpoint[0] += volumesCOM[0];
		newcamerafocalpoint[1] += volumesCOM[1];
		newcamerafocalpoint[2] += volumesCOM[2];
		if (this->getActorCollection()->GetNumberOfItems() > 0)
		{
			newcamerafocalpoint[0] /= 2;
			newcamerafocalpoint[1] /= 2;
			newcamerafocalpoint[2] /= 2;
		}
	}
	center[0] = newcamerafocalpoint[0];
	center[1] = newcamerafocalpoint[1];
	center[2] = newcamerafocalpoint[2];
	cout << "centerofmass=" << center[0] << "," << center[1] << "," << center[2] << "," << endl;
}

void mqMorphoDigCore::ReplaceCameraAndGrid()
{
	double newcamerafocalpoint[3] = { 0,0,0 };
	if (this->Getmui_CameraCentreOfMassAtOrigin() == 0)
	{
		this->GetCenterOfMass(newcamerafocalpoint);
		cout << "Replace camera and grid : new Cam Foc Point" << newcamerafocalpoint[0] << "," << newcamerafocalpoint[1] << "," << newcamerafocalpoint[1] << "," << endl;
	}

	double oldcampos[3];
	double newcampos[3];
	this->getCamera()->GetPosition(oldcampos);
	double oldcamerafocalpoint[3];
	this->getCamera()->GetFocalPoint(oldcamerafocalpoint);

	double dispvector[3];
	vtkMath::Subtract(newcamerafocalpoint, oldcamerafocalpoint, dispvector);
	vtkMath::Add(oldcampos, dispvector, newcampos);
	this->getCamera()->SetPosition(newcampos);
	this->getCamera()->SetFocalPoint(newcamerafocalpoint);

	this->getGridActor()->SetGridOrigin(newcamerafocalpoint);
	this->getGridActor()->SetOutlineMode(this->Getmui_CameraCentreOfMassAtOrigin());
	//this->getGridActor()->SetGridType(gridtype);	
	this->Render();


}
//Called to place camera and grid positions at some desired coordinate
void mqMorphoDigCore::ReplaceCameraAndGridAt(double x, double y, double z)
{
	double newcamerafocalpoint[3] = { x,y, z };
	

	double oldcampos[3];
	double newcampos[3];
	this->getCamera()->GetPosition(oldcampos);
	double oldcamerafocalpoint[3];
	this->getCamera()->GetFocalPoint(oldcamerafocalpoint);

	double dispvector[3];
	vtkMath::Subtract(newcamerafocalpoint, oldcamerafocalpoint, dispvector);
	vtkMath::Add(oldcampos, dispvector, newcampos);
	this->getCamera()->SetPosition(newcampos);
	this->getCamera()->SetFocalPoint(newcamerafocalpoint);

	this->getGridActor()->SetGridOrigin(newcamerafocalpoint);
	this->getGridActor()->SetOutlineMode(2);
	//this->getGridActor()->SetGridType(gridtype);	
	this->Render();


}
double mqMorphoDigCore::GetBoundingBoxLengthOfSelectedActors()
{
	double boundsa[6];
	this->ActorCollection->GetBoundingBoxSelected(boundsa);
	double Normalboundslm[6];
	this->NormalLandmarkCollection->GetBoundingBoxSelected(Normalboundslm);
	double Targetboundslm[6];
	this->TargetLandmarkCollection->GetBoundingBoxSelected(Targetboundslm);
	double Nodeboundslm[6];
	this->NodeLandmarkCollection->GetBoundingBoxSelected(Nodeboundslm);
	double Handleboundslm[6];
	this->HandleLandmarkCollection->GetBoundingBoxSelected(Handleboundslm);
	double Flagboundslm[6];
	this->FlagLandmarkCollection->GetBoundingBoxSelected(Flagboundslm);



	double largestboundsselected[6];
	largestboundsselected[0] = DBL_MAX;
	largestboundsselected[1] = -DBL_MAX;
	largestboundsselected[2] = DBL_MAX;
	largestboundsselected[3] = -DBL_MAX;
	largestboundsselected[4] = DBL_MAX;
	largestboundsselected[5] = -DBL_MAX;

	if (boundsa[0] < largestboundsselected[0]) { largestboundsselected[0] = boundsa[0]; }
	if (boundsa[1] > largestboundsselected[1]) { largestboundsselected[1] = boundsa[1]; }
	if (boundsa[2] < largestboundsselected[2]) { largestboundsselected[2] = boundsa[2]; }
	if (boundsa[3] > largestboundsselected[3]) { largestboundsselected[3] = boundsa[3]; }
	if (boundsa[4] < largestboundsselected[4]) { largestboundsselected[4] = boundsa[4]; }
	if (boundsa[5] > largestboundsselected[5]) { largestboundsselected[5] = boundsa[5]; }

	if (Normalboundslm[0] < largestboundsselected[0]) { largestboundsselected[0] = Normalboundslm[0]; }
	if (Normalboundslm[1] > largestboundsselected[1]) { largestboundsselected[1] = Normalboundslm[1]; }
	if (Normalboundslm[2] < largestboundsselected[2]) { largestboundsselected[2] = Normalboundslm[2]; }
	if (Normalboundslm[3] > largestboundsselected[3]) { largestboundsselected[3] = Normalboundslm[3]; }
	if (Normalboundslm[4] < largestboundsselected[4]) { largestboundsselected[4] = Normalboundslm[4]; }
	if (Normalboundslm[5] > largestboundsselected[5]) { largestboundsselected[5] = Normalboundslm[5]; }

	if (Targetboundslm[0] < largestboundsselected[0]) { largestboundsselected[0] = Targetboundslm[0]; }
	if (Targetboundslm[1] > largestboundsselected[1]) { largestboundsselected[1] = Targetboundslm[1]; }
	if (Targetboundslm[2] < largestboundsselected[2]) { largestboundsselected[2] = Targetboundslm[2]; }
	if (Targetboundslm[3] > largestboundsselected[3]) { largestboundsselected[3] = Targetboundslm[3]; }
	if (Targetboundslm[4] < largestboundsselected[4]) { largestboundsselected[4] = Targetboundslm[4]; }
	if (Targetboundslm[5] > largestboundsselected[5]) { largestboundsselected[5] = Targetboundslm[5]; }

	if (Nodeboundslm[0] < largestboundsselected[0]) { largestboundsselected[0] = Nodeboundslm[0]; }
	if (Nodeboundslm[1] > largestboundsselected[1]) { largestboundsselected[1] = Nodeboundslm[1]; }
	if (Nodeboundslm[2] < largestboundsselected[2]) { largestboundsselected[2] = Nodeboundslm[2]; }
	if (Nodeboundslm[3] > largestboundsselected[3]) { largestboundsselected[3] = Nodeboundslm[3]; }
	if (Nodeboundslm[4] < largestboundsselected[4]) { largestboundsselected[4] = Nodeboundslm[4]; }
	if (Nodeboundslm[5] > largestboundsselected[5]) { largestboundsselected[5] = Nodeboundslm[5]; }

	if (Handleboundslm[0] < largestboundsselected[0]) { largestboundsselected[0] = Handleboundslm[0]; }
	if (Handleboundslm[1] > largestboundsselected[1]) { largestboundsselected[1] = Handleboundslm[1]; }
	if (Handleboundslm[2] < largestboundsselected[2]) { largestboundsselected[2] = Handleboundslm[2]; }
	if (Handleboundslm[3] > largestboundsselected[3]) { largestboundsselected[3] = Handleboundslm[3]; }
	if (Handleboundslm[4] < largestboundsselected[4]) { largestboundsselected[4] = Handleboundslm[4]; }
	if (Handleboundslm[5] > largestboundsselected[5]) { largestboundsselected[5] = Handleboundslm[5]; }

	if (Flagboundslm[0] < largestboundsselected[0]) { largestboundsselected[0] = Flagboundslm[0]; }
	if (Flagboundslm[1] > largestboundsselected[1]) { largestboundsselected[1] = Flagboundslm[1]; }
	if (Flagboundslm[2] < largestboundsselected[2]) { largestboundsselected[2] = Flagboundslm[2]; }
	if (Flagboundslm[3] > largestboundsselected[3]) { largestboundsselected[3] = Flagboundslm[3]; }
	if (Flagboundslm[4] < largestboundsselected[4]) { largestboundsselected[4] = Flagboundslm[4]; }
	if (Flagboundslm[5] > largestboundsselected[5]) { largestboundsselected[5] = Flagboundslm[5]; }

	double A[3];
	double B[3];
	double diag[3];
	A[0] = largestboundsselected[0];
	A[1] = largestboundsselected[2];
	A[2] = largestboundsselected[4];
	B[0] = largestboundsselected[1];
	B[1] = largestboundsselected[3];
	B[2] = largestboundsselected[5];
	diag[0] = B[0] - A[0];
	diag[1] = B[1] - A[1];
	diag[2] = B[2] - A[2];
	double lengthxyz = sqrt((diag[0])*(diag[0]) + (diag[1])*(diag[1]) + (diag[2])*(diag[2]));
	return lengthxyz;

}
void mqMorphoDigCore::GetCenterOfMassOfSelectedActors(double com[3])
{
	com[0] = 0;
	com[1] = 0;
	com[2] = 0;
	//cout << "start a com:" << endl;
	//double com[3] = { 0, 0,0 };	
	//Conception weakness : call COM before VN otherwise computation may not have been triggered!!



	int nv = 0;
	double *coma = this->ActorCollection->GetCenterOfMassOfSelectedActors();
	int nva = this->ActorCollection->GetGlobalSelectedVN();
	if (nva>0) {
		com[0] += coma[0] * nva;
		com[1] += coma[1] * nva;
		com[2] += coma[2] * nva;
	}
	//cout << " com:" << com[0] << "," << com[1] << "," << com[2] << endl;
	//cout << "selected nva:" << nva << endl;
	//cout << "start  lm com:" << endl;
	//Conception weakness : call COM before VN otherwise computation may not have been triggered!!
	nv = nva;
	double *Normalcomlm = this->NormalLandmarkCollection->GetCenterOfMassOfSelectedActors();
	int Normalnvlm = this->NormalLandmarkCollection->GetGlobalSelectedVN();
	if (Normalnvlm > 0) {
		//cout << "nvlm>0" << endl;

		com[0] += Normalcomlm[0] * Normalnvlm;
		com[1] += Normalcomlm[1] * Normalnvlm;
		com[2] += Normalcomlm[2] * Normalnvlm;
	}
	nv += Normalnvlm;

	double *Targetcomlm = this->TargetLandmarkCollection->GetCenterOfMassOfSelectedActors();
	int Targetnvlm = this->TargetLandmarkCollection->GetGlobalSelectedVN();
	if (Targetnvlm > 0) {
		//cout << "nvlm>0" << endl;

		com[0] += Targetcomlm[0] * Targetnvlm;
		com[1] += Targetcomlm[1] * Targetnvlm;
		com[2] += Targetcomlm[2] * Targetnvlm;
	}
	nv += Targetnvlm;

	double *Nodecomlm = this->NodeLandmarkCollection->GetCenterOfMassOfSelectedActors();
	int Nodenvlm = this->NodeLandmarkCollection->GetGlobalSelectedVN();
	if (Nodenvlm > 0) {
		//cout << "nvlm>0" << endl;

		com[0] += Nodecomlm[0] * Nodenvlm;
		com[1] += Nodecomlm[1] * Nodenvlm;
		com[2] += Nodecomlm[2] * Nodenvlm;
	}
	nv += Nodenvlm;

	double *Handlecomlm = this->HandleLandmarkCollection->GetCenterOfMassOfSelectedActors();
	int Handlenvlm = this->HandleLandmarkCollection->GetGlobalSelectedVN();
	if (Handlenvlm > 0) {
		//cout << "nvlm>0" << endl;

		com[0] += Handlecomlm[0] * Handlenvlm;
		com[1] += Handlecomlm[1] * Handlenvlm;
		com[2] += Handlecomlm[2] * Handlenvlm;
	}
	nv += Handlenvlm;

	double *Flagcomlm = this->FlagLandmarkCollection->GetCenterOfMassOfSelectedActors();
	int Flagnvlm = this->FlagLandmarkCollection->GetGlobalSelectedVN();
	if (Flagnvlm > 0) {
		//cout << "nvlm>0" << endl;

		com[0] += Flagcomlm[0] * Flagnvlm;
		com[1] += Flagcomlm[1] * Flagnvlm;
		com[2] += Flagcomlm[2] * Flagnvlm;
	}
	nv += Flagnvlm;

	if (nv > 0) {
		com[0] /= nv; com[1] /= nv; com[2] /= nv;
	}
	if (this->VolumeCollection->GetNumberOfSelectedVolumes() > 0)
	{
		double *Volcom = this->VolumeCollection->GetCenterOfMassOfSelectedVolumes();
		com[0] += Volcom[0];
		com[1] += Volcom[1];
		com[2] += Volcom[2];
		if (nv > 0) {
			com[0] /= 2;
			com[1] /= 2;
			com[2] /= 2;
		}
	}
	//cout << "global selected com:" << com[0] << ","<<com[1] << "," << com[2] << endl;
	//cout << "global selected nv:" << nv << endl;
	//return com;
}
void mqMorphoDigCore::AdjustCameraAndGrid()
{
	double newcamerafocalpoint[3] = { 0,0,0 };
	double actorCOM[3] = { 0,0,0 };
	double volumeCOM[3] = { 0,0,0 };
	if (this->Getmui_CameraCentreOfMassAtOrigin() == 0)
	{
		this->GetCenterOfMass(newcamerafocalpoint);
		cout << "Adjust new Cam Foc Point" << newcamerafocalpoint[0] << "," << newcamerafocalpoint[1] << "," << newcamerafocalpoint[2] << "," << endl;
		this->getGridActor()->SetGridOrigin(newcamerafocalpoint);


	}

	double multfactor = 1 / tan(this->getCamera()->GetViewAngle() *  vtkMath::Pi() / 360.0);
	double ActorBoxLength = this->getActorCollection()->GetBoundingBoxLength();
	double VolumeBoxLength = this->getVolumeCollection()->GetBoundingBoxLength();
	double GlobalBoundingBoxLength = (VolumeBoxLength + ActorBoxLength);
	if (GlobalBoundingBoxLength == std::numeric_limits<double>::infinity() || GlobalBoundingBoxLength == 0)
	{
		GlobalBoundingBoxLength = 120;
	}

	double oldcampos[3];
	double newcampos[3];
	this->getCamera()->GetPosition(oldcampos);
	double oldcamerafocalpoint[3];
	this->getCamera()->GetFocalPoint(oldcamerafocalpoint);

	double dispvector[3];

	vtkMath::Subtract(oldcampos, oldcamerafocalpoint, dispvector);
	vtkMath::Normalize(dispvector);
	double newdist = multfactor*GlobalBoundingBoxLength;
	vtkMath::MultiplyScalar(dispvector, newdist);

	vtkMath::Add(newcamerafocalpoint, dispvector, newcampos);

	this->getCamera()->SetPosition(newcampos);
	this->getCamera()->SetFocalPoint(newcamerafocalpoint);

	// now adjust if necessary..
	if (this->Getmui_CameraOrtho() == 1)
	{
		this->getCamera()->SetParallelScale(GlobalBoundingBoxLength);
		
	}
	this->getRenderer()->ResetCameraClippingRange();
	this->ActivateClippingPlane();
	//this->ui->qvtkWidget->update();
	mqMorphoDigCore::instance()->signal_zoomChanged();
	this->Render();
	


}

/*void mqMorphoDigCore::RedrawBezierCurves()
{
	
	//this->BezierCurveSource->Update();
	cout << "Render" << endl;
	this->Render();
}*/
void mqMorphoDigCore::ResetCameraOrthoPerspective()
{
	if (this->Getmui_CameraOrtho() == 1)
	{
		this->getCamera()->SetParallelProjection(true);
		this->DollyCameraForParallelScale();
	}
	else
	{

		this->getCamera()->SetParallelProjection(false);
		this->DollyCameraForPerspectiveMode();


	}
	//cout << "Parallel scale"<<this->MorphoDigCore->getCamera()->GetParallelScale()<<endl;
	double dist = 0;


	double campos[3] = { 0,0,0 };
	double foc[3] = { 0,0,0 };
	this->getCamera()->GetPosition(campos);
	//cout << "Camera Position:" << campos[0] <<","<<campos[1]<<","<<campos[2]<< endl;
	this->getCamera()->GetFocalPoint(foc);
	//cout << "Camera Position:" << foc[0] << "," << foc[1] << "," << foc[2] << endl;
	
	dist = sqrt(vtkMath::Distance2BetweenPoints(campos, foc));
	//cout << "Distance between camera and focal point:" << dist << endl;

	//cout << "Camera viewing angle:" << this->MorphoDigCore->getCamera()->GetViewAngle() << endl;

	this->Render(); // update main window!
}
/*
In perspective mode, "zoom" (dolly) in/out changes the position of the camera
("dolly" functions of vtkInteractorStyleTrackballCamera.cxx and of vtkInteractorStyleJoystickCamera )
Beware : no real "Zoom" function is applied in these styles!!!!
=> before I create  MorphoDig' own interactor styles, camera's parallel scale (=ortho "zoom") should
be updated when switching from "perspective" to "ortho" to keep track of that change...
=> Once these styles are created, this function should be removed!

*/
void mqMorphoDigCore::DollyCameraForParallelScale()
{
	double campos[3] = { 0,0,0 };
	double foc[3] = { 0,0,0 };

	this->getCamera()->GetPosition(campos);
	this->getCamera()->GetFocalPoint(foc);
	double dist = sqrt(vtkMath::Distance2BetweenPoints(campos, foc));
	
	double multfactor = 1 / tan(this->getCamera()->GetViewAngle() *  vtkMath::Pi() / 360.0);

	double newparallelscale = dist / multfactor;
	this->getCamera()->SetParallelScale(newparallelscale);
	cout << "Dolly4Parallel" << endl;
}

/*
In parallel mode, "zoom" (dolly) in/out does not change the position of the camera
("dolly" functions of vtkInteractorStyleTrackballCamera.cxx and of vtkInteractorStyleJoystickCamera )
Beware : no real "Zoom" function is applied in these styles!!!!
=> before I create  MorphoDig' own interactor styles, camera's position in perspective mode should
be updated when switching from "ortho" to "perspective" to keep track of that change...
=> Once these styles are created, this function should be removed!

*/
void mqMorphoDigCore::DollyCameraForPerspectiveMode()
{
	double campos[3] = { 0,0,0 };
	double foc[3] = { 0,0,0 };
	double dispvector[3];
	this->getCamera()->GetPosition(campos);
	this->getCamera()->GetFocalPoint(foc);
	double multfactor = 3.73; // at 30° vtk : angle = 2*atan((h/2)/d). 
							  // then 2*d  =12/tan(viewangle/2) 
	multfactor = 1 / tan(this->getCamera()->GetViewAngle() *  vtkMath::Pi() / 360.0);
	//cout << "DollyCameraForPerspectiveMode" << endl;
	//cout << "multfactor" << multfactor << endl;
	//cout << "Old posisition:" << campos[0] << "," << campos[1] << "," << campos[2] << endl;

	vtkMath::Subtract(campos, foc, dispvector);
	//cout<<"Disp Vector:" << dispvector[0] << ","<<dispvector[1] << "," << dispvector[2] << endl;
	vtkMath::Normalize(dispvector);
	//cout << "Normalized Disp Vector:" << dispvector[0] << "," << dispvector[1] << "," << dispvector[2] << endl;

	double newdist = multfactor*this->getCamera()->GetParallelScale();
	//cout << "New dist:" << newdist << endl;
	vtkMath::MultiplyScalar(dispvector, newdist);
	//cout << "Multiplied Disp Vector:" << dispvector[0] << "," << dispvector[1] << "," << dispvector[2] << endl;
	double newpos[3] = { 0,0,0 };
	vtkMath::Add(foc, dispvector, newpos);
	//cout << "New pos:" << newpos[0] << "," << newpos[1] << "," << newpos[2] << endl;

	this->getCamera()->SetPosition(newpos);


	cout << "Dolly4Perspective" << endl;
}

//On ajoute un indice au nom si le nom existe déjà.
//fonction recurente pour savoir quel indice lui donner.
std::string  mqMorphoDigCore::CheckingName(std::string name_obj) {
	//cout << "check: " << name_obj << endl;

	//1st: check if does not exist AT ALL (before searching for "(")
	this->ActorCollection->InitTraversal();
	int exists = 0;
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMDActor * myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		std::string a_name = myActor->GetName();

		if (a_name.compare(name_obj) == 0) {// if such an object exists... we  increase cpt_name

			exists = 1;

		}

	}
	if (exists==0)
	{
		return name_obj;
	}

	//2nd: suffix = base(newnum) is needed

	int cpt_name = 1;
	std::string s_cpt_name = std::to_string(cpt_name);
	std::string name_base = name_obj;
	std::string postfix = "";
	size_t nPos = name_obj.find_first_of("(");
	if (nPos >0 &&nPos<= name_obj.length())
	{
		//cout << "nPos=" << nPos << endl;		
		name_base = name_base.substr(0, nPos);
	}
	//cout << "name_base: " << name_base << endl;
	
	this->ActorCollection->InitTraversal();
	
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMDActor * myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		std::string a_name_base = myActor->GetName();
		
		//cout << "actor name: " << a_name_base << endl;
		
		size_t anPos = a_name_base.find_first_of("(");
		size_t anPos2 = a_name_base.find_first_of(")");
		//cout << "AnPos:" << anPos << endl;
		if (anPos > 0 && anPos <= a_name_base.length())
		{
			a_name_base = a_name_base.substr(0, anPos);
			
		}
		//cout << "a_name_base: " << a_name_base<<endl;
		if (a_name_base.compare(name_base) ==0) {// if such an object exists... we  increase cpt_name
			
			cpt_name++;			
			
		}

	}

	// 3rd: now find a correct "num" in base(num) name
	if (cpt_name > 1)
	{
		// we now that in all, there are "cpt_name" objects having the same name basis
		// we search for the first "basis(cpt)" that does not exist and we give that name...
		
		for (int j = 2; j <= cpt_name; j++)
		{
			exists = 0;
			this->ActorCollection->InitTraversal();
			std::string j_name = name_base + "(" + std::to_string(j) + ")";
			for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
			{
				vtkMDActor * myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
				
				std::string a_name = myActor->GetName();
				if (a_name.compare(j_name) == 0) 
				{
					exists = 1;
				}
				
			}
			if (exists==0)
			{
				//does not exist => we give this name.
				name_obj = j_name;
				return name_obj;
			}
			
		}
		// here: all items were found in the series : basis basis(2) basis(3) etc. => we name it basis(4)
		s_cpt_name = std::to_string(cpt_name+1);
		name_obj = name_base + "(" + s_cpt_name + ")";
	}
	
	
	return name_obj;
}

void mqMorphoDigCore::Setmui_DisplayMode(int mode)
{
	this->mui_DisplayMode = mode;
	this->ActorCollection->SetDisplayMode(mode);
}
int mqMorphoDigCore::Getmui_DisplayMode()
{
	return this->mui_DisplayMode;
	
}
int mqMorphoDigCore::Getmui_DefaultDisplayMode()
{
	return this->mui_DefaultDisplayMode;

}

void mqMorphoDigCore::SetGridVisibility()
{
	vtkPropCollection* props = this->getRenderer()->GetViewProps(); //iterate through and set each visibility to 0
	props->InitTraversal();
	std::string str1("vtkGridActor");
	for (int i = 0; i < props->GetNumberOfItems(); i++)
	{
		vtkProp *myprop = props->GetNextProp();
		if (str1.compare(myprop->GetClassName()) == 0)
		{
			if (this->Getmui_ShowGrid() == 1)
			{
				myprop->VisibilityOn();
			}
			else
			{
				myprop->VisibilityOff();
			}
		}

	}
	this->Render();
}

double mqMorphoDigCore::GetHundredPxSU()
{
	//100 px in mm 
	double pan_center[3] = { 0,0,0 };
	if (mqMorphoDigCore::instance()->Getmui_CameraCentreOfMassAtOrigin() == 0)
	{
		this->GetCenterOfMassOfSelectedActors(pan_center);
	}
	double dPanCenter[3] = { 0,0,0 };
	double origin[4] = { 0, 0, 1,1 };
	double away[4] = { 0, 0, 2,1 };
	this->GetWorldToDisplay(pan_center[0], pan_center[1], pan_center[2], dPanCenter);
	this->GetDisplayToWorld(dPanCenter[0], dPanCenter[1], dPanCenter[2], origin);
	this->GetDisplayToWorld(dPanCenter[0], dPanCenter[1] + 100, dPanCenter[2], away);
	double p1[3] = { away[0], away[1], away[2] };
	double p2[3] = { origin[0], origin[1], origin[2] };

	double dist = sqrt(vtkMath::Distance2BetweenPoints(p1, p2));
	return dist;
}
void mqMorphoDigCore::SetGridInfos()
{
	QString myAnnotation;
	//QString myBeginning("Size unit: ");
	//myAnnotation = myBeginning + this->Getmui_SizeUnit();
	//QString follows("\nGrid: 1 square=");
	QString follows("Grid: 1 square=");
	QString onehundredpx("");
	double pan_center[3] = { 0,0,0 };
	if (mqMorphoDigCore::instance()->Getmui_CameraOrtho() == 1)
	{
		
		double HundredPxSU = mqMorphoDigCore::instance()->GetHundredPxSU();
		QString hundredpx = QString::number(HundredPxSU, 'f', 2);
		onehundredpx = ", 100px=" + hundredpx + this->Getmui_SizeUnit();
		
	}
	
	
	QString valueAsString = QString::number(this->Getmui_GridSpacing());
	
	
	//myAnnotation = myAnnotation + follows;
	myAnnotation = follows + valueAsString;
	//myAnnotation = myAnnotation + this->Getmui_SizeUnit() + ", 100px="+hundredpx+ this->Getmui_SizeUnit();
	myAnnotation = myAnnotation + this->Getmui_SizeUnit()+ onehundredpx;
	this->cornerAnnotation->SetText(vtkCornerAnnotation::LowerRight, myAnnotation.toStdString().c_str());
	//QString myTest("Loulou fait du ski\nEt voila\nToutou");
	if (this->Getmui_ShowGrid() == 1)
	{
		this->cornerAnnotation->VisibilityOn();
	}
	else
	{
		this->cornerAnnotation->VisibilityOff();
	}
	this->cornerAnnotation->SetLinearFontScaleFactor(2);
	this->cornerAnnotation->SetNonlinearFontScaleFactor(1);
	this->cornerAnnotation->SetMaximumFontSize(12);
	double *bg = this->Getmui_BackGroundColor();
	double r, g, b;
	r = bg[0]; g = bg[1]; b = bg[2];
	double sum = r + g + b;
	if (
		sum > 1.5 
		)
	{
		r = 0; g = 0; b = 0;
	}
	
	else 
	{
		r = 1; g = 1; b = 1;
		
	}
	
	this->cornerAnnotation->GetTextProperty()->SetColor(r, g,b);
	this->Render();
}

void mqMorphoDigCore::SetOrientationHelperVisibility()
{

	//std::string str1("vtkOrientationHelperActor");
	if (this->Getmui_ShowOrientationHelper() == 1)
	{
		this->OrientationHelperWidget->GetOrientationMarker()->VisibilityOn();
	}
	else
	{
		this->OrientationHelperWidget->GetOrientationMarker()->VisibilityOff();
	}
	this->Render();
}

vtkMDActor * mqMorphoDigCore::GetFirstSelectedActor()
{
	this->ActorCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			return myActor;
		}
	}

	return NULL;
}

vtkMDVolume * mqMorphoDigCore::GetFirstSelectedVolume()
{
	this->VolumeCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->VolumeCollection->GetNumberOfItems(); i++)
	{
		vtkMDVolume *myVolume = vtkMDVolume::SafeDownCast(this->VolumeCollection->GetNextVolume());
		if (myVolume->GetSelected() == 1)
		{
			return myVolume;
		}
	}

	return NULL;
}
vtkMDActor* mqMorphoDigCore::GetLastActor()
{
	return vtkMDActor::SafeDownCast(this->getActorCollection()->GetLastActor());
}
vtkMDVolume* mqMorphoDigCore::GetLastVolume()
{
	return this->getVolumeCollection()->GetLastVolume();
}

vtkLMActor* mqMorphoDigCore::GetLastLandmark(int mode)
{
	if (mode == 0) {return vtkLMActor::SafeDownCast(this->getNormalLandmarkCollection()->GetLastActor());
	}
	else if (mode == 1) {
		return vtkLMActor::SafeDownCast(this->getTargetLandmarkCollection()->GetLastActor());
	}
	else if (mode == 2) {
		return vtkLMActor::SafeDownCast(this->getNodeLandmarkCollection()->GetLastActor());
	}
	else if (mode == 3) {
		return vtkLMActor::SafeDownCast(this->getHandleLandmarkCollection()->GetLastActor());
	}
	//else if (mode == 4) {
	else  {
		return vtkLMActor::SafeDownCast(this->getFlagLandmarkCollection()->GetLastActor());
	}
	
}

void mqMorphoDigCore::ApplyMatrix(vtkSmartPointer<vtkMatrix4x4> Mat, int mode)
{
	
	// mode : 0 for last inserted surface
	// mode : 1 for all selected surfaces AND volumes
	// mode : 2 for all selected landmarks/flags
	// mode : 3 for last inserted volume	

	if (mode == 0)
	{
		vtkMDActor *actor = this->GetLastActor();
		if (actor != NULL)
		{
			actor->ApplyMatrix(Mat);
		}
	}
	if (mode == 3)
	{
		vtkMDVolume *volume = this->GetLastVolume();
		if (volume != NULL)
		{
			volume->ApplyMatrix(Mat);
			
		}
	}	
	else if (mode == 1 )
	{
		this->ActorCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
		{
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
			if (myActor->GetSelected() == 1)
			{
				myActor->ApplyMatrix(Mat);
				myActor->SetSelected(0);
			}
		}
		this->VolumeCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->VolumeCollection->GetNumberOfItems(); i++)
		{
			vtkMDVolume *myVolume = vtkMDVolume::SafeDownCast(this->VolumeCollection->GetNextVolume());
			if (myVolume->GetSelected() == 1)
			{
				myVolume->ApplyMatrix(Mat);
				myVolume->SetSelected(0);
			}
		}

	}
	else if (mode ==2) 
	{

		this->NormalLandmarkCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->NormalLandmarkCollection->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NormalLandmarkCollection->GetNextActor());
			if (myActor->GetSelected() == 1 )
			{
				myActor->ApplyMatrix(Mat);;
				myActor->SetSelected(0);
			}
		}
		this->TargetLandmarkCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->TargetLandmarkCollection->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->TargetLandmarkCollection->GetNextActor());
			if (myActor->GetSelected() == 1)
			{
				myActor->ApplyMatrix(Mat);;
				myActor->SetSelected(0);
			}
		}
		this->NodeLandmarkCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->NodeLandmarkCollection->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NodeLandmarkCollection->GetNextActor());
			if (myActor->GetSelected() == 1)
			{
				myActor->ApplyMatrix(Mat);;
				myActor->SetSelected(0);
			}
		}
		this->HandleLandmarkCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->HandleLandmarkCollection->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->HandleLandmarkCollection->GetNextActor());
			if (myActor->GetSelected() == 1)
			{
				myActor->ApplyMatrix(Mat);;
				myActor->SetSelected(0);
			}
		}
		this->FlagLandmarkCollection->InitTraversal();
		for (vtkIdType i = 0; i < this->FlagLandmarkCollection->GetNumberOfItems(); i++)
		{
			//cout << "modify flag " << i << endl;
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->FlagLandmarkCollection->GetNextActor());
			if (myActor->GetSelected() == 1)
			{
				myActor->ApplyMatrix(Mat);;
				myActor->SetSelected(0);
				//cout << "modify flag " << i << "done"<< endl;
			}
		}
		
	}
	cout <<" Actor collection changed" << endl;
	this->ActorCollection->SetChanged(1);
}

void mqMorphoDigCore::SelectAll(int Count)
{}
void mqMorphoDigCore::UnselectAll(int Count)
{
	int node_handle_collections_changed = 0;
	this->VolumeCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->VolumeCollection->GetNumberOfItems(); i++)
	{
		vtkMDVolume *myVolume = vtkMDVolume::SafeDownCast(this->VolumeCollection->GetNextVolume());
		if (myVolume->GetSelected() == 1 && Count > 0)
		{
			myVolume->SaveState(Count);
		}
	}
	this->ActorCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1 &&Count>0)
		{
			myActor->SaveState(Count);
		}
	}
	this->NormalLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NormalLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NormalLandmarkCollection->GetNextActor());
		if (myActor->GetSelected() == 1&&Count>0)
		{
			myActor->SaveState(Count);
		}
	}
	this->TargetLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->TargetLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->TargetLandmarkCollection->GetNextActor());
		if (myActor->GetSelected() == 1 && Count>0)
		{
			myActor->SaveState(Count);

		}
	}
	this->NodeLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NodeLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NodeLandmarkCollection->GetNextActor());
		if (myActor->GetSelected() == 1 && Count>0)
		{
			myActor->SaveState(Count);
			node_handle_collections_changed = 1;
		}
	}
	this->HandleLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->HandleLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->HandleLandmarkCollection->GetNextActor());
		if (myActor->GetSelected() == 1 && Count>0)
		{
			myActor->SaveState(Count);
			node_handle_collections_changed = 1;
		}
	}
	this->FlagLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->FlagLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->FlagLandmarkCollection->GetNextActor());
		if (myActor->GetSelected() == 1 && Count>0)
		{
			myActor->SaveState(Count);
		}
	}

	this->ActorCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			myActor->SetSelected(0);
			myActor->SetisVisible(1);
			myActor->SetChanged(1);

		}


	}
	this->VolumeCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->VolumeCollection->GetNumberOfItems(); i++)
	{
		vtkMDVolume *myVolume = vtkMDVolume::SafeDownCast(this->VolumeCollection->GetNextVolume());
		if (myVolume->GetSelected() == 1)
		{
			myVolume->SetSelected(0);
			myVolume->SetisVisible(1);
			myVolume->SetChanged(1);

		}


	}
	this->NormalLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NormalLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NormalLandmarkCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			myActor->SetSelected(0);
			myActor->SetChanged(1);

		}


	}
	this->TargetLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->TargetLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->TargetLandmarkCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			myActor->SetSelected(0);
			myActor->SetChanged(1);

		}


	}
	this->NodeLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NodeLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NodeLandmarkCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			myActor->SetSelected(0);
			myActor->SetChanged(1);

		}


	}
	this->HandleLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->HandleLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->HandleLandmarkCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			myActor->SetSelected(0);
			myActor->SetChanged(1);

		}


	}
	this->FlagLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->FlagLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->FlagLandmarkCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			myActor->SetSelected(0);
			myActor->SetChanged(1);

		}


	}
	if (node_handle_collections_changed == 1)
	{
		this->NodeLandmarkCollection->Modified();
		this->HandleLandmarkCollection->Modified();
	}
}

void mqMorphoDigCore::Render()
{
	
	this->RenderWindow->Render();
}

void mqMorphoDigCore::Setmui_ShowOrientationHelper(int orientationHelper) { this->mui_ShowOrientationHelper = orientationHelper; }
int mqMorphoDigCore::Getmui_DefaultShowOrientationHelper() { return this->mui_DefaultShowOrientationHelper; }
int mqMorphoDigCore::Getmui_ShowOrientationHelper() { return this->mui_ShowOrientationHelper; }

void mqMorphoDigCore::Setmui_CameraCentreOfMassAtOrigin(int comao) { this->mui_CameraCentreOfMassAtOrigin = comao; }
int mqMorphoDigCore::Getmui_DefaultCameraCentreOfMassAtOrigin() { return this->mui_DefaultCameraCentreOfMassAtOrigin; }
int mqMorphoDigCore::Getmui_CameraCentreOfMassAtOrigin() { return this->mui_CameraCentreOfMassAtOrigin; }


void mqMorphoDigCore::Setmui_CameraOrtho(int ortho) { this->mui_CameraOrtho = ortho; }
int mqMorphoDigCore::Getmui_DefaultCameraOrtho() { return this->mui_DefaultCameraOrtho; }
int mqMorphoDigCore::Getmui_CameraOrtho() { return this->mui_CameraOrtho; }

void mqMorphoDigCore::Setmui_ShowGrid(int showgrid) { this->mui_ShowGrid = showgrid; }
int mqMorphoDigCore::Getmui_ShowGrid() { return this->mui_ShowGrid; }
int mqMorphoDigCore::Getmui_DefaultShowGrid() { return this->mui_DefaultShowGrid; };

void mqMorphoDigCore::Setmui_GridSpacing(double gridspacing) { this->mui_GridSpacing = gridspacing; this->GridActor->SetGridSpacing(gridspacing);

}
double mqMorphoDigCore::Getmui_GridSpacing() { return this->mui_GridSpacing; }
double mqMorphoDigCore::Getmui_DefaultGridSpacing() { return this->mui_DefaultGridSpacing; }

void mqMorphoDigCore::Setmui_SizeUnit(QString unit) { this->mui_SizeUnit = unit; }
QString mqMorphoDigCore::Getmui_SizeUnit() { return this->mui_SizeUnit; }
QString mqMorphoDigCore::Getmui_DefaultSizeUnit() { return this->mui_DefaultSizeUnit; }

void mqMorphoDigCore::Setmui_MoveMode(int movemode) { this->mui_MoveMode= movemode;
if (movemode == 0) { this->UnselectAll(-1);  }
}
int mqMorphoDigCore::Getmui_MoveMode() { return this->mui_MoveMode; }
int mqMorphoDigCore::Getmui_DefaultMoveMode() { return this->mui_DefaultMoveMode; };

void mqMorphoDigCore::Setmui_ArrayVisibility(int scalarvisibility)
{
	if (this->mui_ArrayVisibility != scalarvisibility)
	{
		//cout << "Scalar visibility has changed" << endl;
		//1 refresh scalar bar actor !
		int sba_refresh_needed = 0; //conventional scalar bar actor
		int tsba_refresh_needed = 0; //tag scalar bar actor (tag legent)
		if ((this->mui_ActiveArray->DataType==VTK_FLOAT|| this->mui_ActiveArray->DataType == VTK_DOUBLE)&& this->mui_ActiveArray->NumComp == 1)
		{
			//cout << "SBA refresh needed" << endl;
			sba_refresh_needed = 1;
		}
		if (this->mui_ActiveArray->DataType == VTK_INT  && this->mui_ActiveArray->NumComp == 1)
		{
			//cout << "SBA refresh needed" << endl;
			tsba_refresh_needed = 1;
		}
		if (scalarvisibility == 1)
		{
			// turn on if needed
			if (sba_refresh_needed == 1)
			{
				this->ScalarBarActor->SetLookupTable(this->Getmui_ActiveColorMap()->ColorMap);
				this->ScalarBarActor->SetTitle(this->Getmui_ActiveArray()->Name.toStdString().c_str());
				this->Renderer->AddActor(ScalarBarActor);
				//cout << "Add SBA" << endl;
			}
			if (tsba_refresh_needed == 1)
			{
				this->TagScalarBarActor->SetLookupTable(this->Getmui_ActiveTagMap()->TagMap);
				this->TagScalarBarActor->SetNumberOfLabels(this->Getmui_ActiveTagMap()->numTags);
				this->TagScalarBarActor->SetTitle(this->Getmui_ActiveArray()->Name.toStdString().c_str());
				//this->Renderer->AddActor(TagScalarBarActor);
				//cout << "Add SBA" << endl;
			}

		}
		else
		{
			//turn off if needed
			if (sba_refresh_needed == 1)
			{
			//	cout << "Remove SBA" << endl;
				this->Renderer->RemoveActor(ScalarBarActor);
			}
			
				//	cout << "Remove SBA" << endl;
				//this->Renderer->RemoveActor(TagScalarBarActor);
			
		}
	

		// 2 refresh all actors!
		this->mui_ArrayVisibility = scalarvisibility;
		this->ActorCollection->InitTraversal();

		for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
		{
			vtkMDActor * myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		
			if (myActor->GetMapper() != NULL)
			{
				if (scalarvisibility == 1 && myActor->GetSelected()==0)
				{
				
					QString none = QString("Solid color");
					if (this->mui_ActiveArray->Name != none)
					{
						vtkPolyData *myPD = vtkPolyData::SafeDownCast(myActor->GetMapper()->GetInput());
						if (myPD->GetPointData()->GetScalars(this->mui_ActiveArray->Name.toStdString().c_str()) != NULL)
						{

							vtkPolyDataMapper::SafeDownCast(myActor->GetMapper())->ScalarVisibilityOn();
						}
						
					}
					
				}
				else
				{
					vtkPolyDataMapper::SafeDownCast(myActor->GetMapper())->ScalarVisibilityOff();
				}
			}
		}
		

	}
}
int mqMorphoDigCore::Getmui_DefaultArrayVisibility() { return this->mui_DefaultArrayVisibility; }

int mqMorphoDigCore::Getmui_ArrayVisibility() { return this->mui_ArrayVisibility; }

ExistingArrays * mqMorphoDigCore::Getmui_ExistingArrays()
{
	return this->mui_ExistingArrays;
}


ExistingArrays * mqMorphoDigCore::Getmui_ArraysOfActor(vtkSmartPointer<vtkMDActor> actor)
{
	// browse through all actors and check for existing scalar
	// add
	cout << "Init mui scalars of selected objects" << endl;
	QStringList existing;
	this->mui_ArrayList->Stack.clear();
	QString none = QString("Solid color");
	this->mui_ArrayList->Stack.push_back(ExistingArrays::Element(none, -1, 0));
	
	vtkPolyDataMapper *mapper = vtkPolyDataMapper::SafeDownCast(actor->GetMapper());

	if (mapper != NULL && vtkPolyData::SafeDownCast(mapper->GetInput()) != NULL)
	{
		vtkPolyData *myPD = vtkPolyData::SafeDownCast(mapper->GetInput());
		int nbarrays = myPD->GetPointData()->GetNumberOfArrays();
		for (int i = 0; i < nbarrays; i++)
		{
			//cout << "Array " << i << "=" << myPD->GetPointData()->GetArrayName(i) << endl;
			int num_comp = myPD->GetPointData()->GetArray(i)->GetNumberOfComponents();
			//cout << "Array" << i << " has "<<myPD->GetPointData()->GetArray(i)->GetNumberOfComponents()<< " components"<<endl;
			//std::cout << VTK_UNSIGNED_CHAR << " unsigned char" << std::endl;
			//std::cout << VTK_UNSIGNED_INT << " unsigned int" << std::endl;
			//std::cout << VTK_FLOAT << " float" << std::endl;
			//std::cout << VTK_DOUBLE << " double" << std::endl;
			int dataType = myPD->GetPointData()->GetArray(i)->GetDataType();

			if (dataType == VTK_UNSIGNED_CHAR && (num_comp == 3 || num_comp == 4))
			{
				// ok to add RGB like scalars
				cout << "Found RGB like" << endl;
				QString RGBlike = QString(myPD->GetPointData()->GetArrayName(i));
				this->Addmui_ExistingArrays(RGBlike, dataType, num_comp, 0);
			}
			if ((dataType == VTK_UNSIGNED_INT || dataType == VTK_INT) && (num_comp == 1))
			{
				// ok to add TAG like scalars
				QString Taglike = QString(myPD->GetPointData()->GetArrayName(i));
				this->Addmui_ExistingArrays(Taglike, dataType, num_comp, 0);
			}
			if ((dataType == VTK_FLOAT || (dataType == VTK_DOUBLE)) && (num_comp == 1))
			{
				// ok to add conventional scalars (like curvature, thickness, height etc... )
				QString ConvScalar = QString(myPD->GetPointData()->GetArrayName(i));
				this->Addmui_ExistingArrays(ConvScalar, dataType, num_comp, 0);
			}

		}

	}			
	int exists = 0;
	return this->mui_ArrayList;
}


int mqMorphoDigCore::GetNumerOfNonRGBArraysOfSelectedObjects(int exclude_rgb)
{
	ExistingArrays *MyList = this->Getmui_ArraysOfSelectedObjects();
	QString rgb = QString("RGB");
	QString sc = QString("Solid color");
	int cpt = 0;
	if (exclude_rgb==0)
	{
		if (MyList->Stack.size() > 1)
		{
			cout << "Scalar name" << MyList->Stack.at(0).Name.toStdString() << endl;
		}
		return (MyList->Stack.size()-1);
	}
	else
	{
		for (int i = 0; i < MyList->Stack.size(); i++)
		{
			//if ((MyList->Stack.at(i).DataType == VTK_FLOAT || MyList->Stack.at(i).DataType == VTK_DOUBLE || MyList->Stack.at(i).DataType == VTK_INT) && MyList->Stack.at(i).NumComp == 1)
			// {

			cout <<"Scalar name"<< MyList->Stack.at(i).Name.toStdString() << endl;
			if (MyList->Stack.at(i).Name != rgb && MyList->Stack.at(i).Name != sc)
			{
				cpt++;
			}
			else if (MyList->Stack.at(i).Name == rgb)
			{
				if ((MyList->Stack.at(i).DataType == VTK_FLOAT || MyList->Stack.at(i).DataType == VTK_DOUBLE || MyList->Stack.at(i).DataType == VTK_INT) && MyList->Stack.at(i).NumComp == 1)
				{
					//rare case: a Scalar or Tag array named RGB
					cpt++;
				}
			}

			// }


		}
	}
	return cpt;

}

ExistingArrays * mqMorphoDigCore::Getmui_ArraysOfSelectedObjects(int onlyfirst)
{
	
		// browse through all actors and check for existing scalar
	// add
	cout << "Init mui scalars of selected objects" << endl;
	QStringList existing;
	this->ActorCollection->InitTraversal();
	this->mui_ArrayList->Stack.clear();
	QString none = QString("Solid color");
	this->mui_ArrayList->Stack.push_back(ExistingArrays::Element(none,-1,0));
	int stop = 0;
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMDActor * myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if(myActor->GetSelected()==1 &&stop ==0)
		{
			if (onlyfirst == 1) { stop = 1; }
			vtkPolyDataMapper *mapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
		
			if (mapper != NULL && vtkPolyData::SafeDownCast(mapper->GetInput()) != NULL)
			{
				vtkPolyData *myPD = vtkPolyData::SafeDownCast(mapper->GetInput());
				int nbarrays = myPD->GetPointData()->GetNumberOfArrays();
				for (int i = 0; i < nbarrays; i++)
				{
					//cout << "Array " << i << "=" << myPD->GetPointData()->GetArrayName(i) << endl;
					int num_comp = myPD->GetPointData()->GetArray(i)->GetNumberOfComponents();
					//cout << "Array" << i << " has "<<myPD->GetPointData()->GetArray(i)->GetNumberOfComponents()<< " components"<<endl;
					//std::cout << VTK_UNSIGNED_CHAR << " unsigned char" << std::endl;
					//std::cout << VTK_UNSIGNED_INT << " unsigned int" << std::endl;
					//std::cout << VTK_FLOAT << " float" << std::endl;
					//std::cout << VTK_DOUBLE << " double" << std::endl;
					int dataType = myPD->GetPointData()->GetArray(i)->GetDataType();


					if (dataType == VTK_UNSIGNED_CHAR && (num_comp == 3 || num_comp == 4))
					{
						// ok to add RGB like scalars
						QString RGBlike = QString(myPD->GetPointData()->GetArrayName(i));
						this->Addmui_ExistingArrays(RGBlike, dataType, num_comp, 0);
					}
					if ((dataType == VTK_UNSIGNED_INT || dataType == VTK_INT) && (num_comp == 1))
					{
						// ok to add TAG like scalars
						QString Taglike = QString(myPD->GetPointData()->GetArrayName(i));
						this->Addmui_ExistingArrays(Taglike, dataType, num_comp, 0);
					}
					if ((dataType == VTK_FLOAT || (dataType == VTK_DOUBLE)) && (num_comp == 1))
					{
						// ok to add conventional scalars (like curvature, thickness, height etc... )
						QString ConvScalar = QString(myPD->GetPointData()->GetArrayName(i));
						this->Addmui_ExistingArrays(ConvScalar, dataType, num_comp, 0);
					}

				}

			}
		}


	}
	int exists = 0;


	
	return this->mui_ArrayList;

}

ExistingTagMaps* mqMorphoDigCore::Getmui_ExistingTagMaps()
{
	return this->mui_ExistingTagMaps;

}



ExistingColorMaps * mqMorphoDigCore::Getmui_ExistingColorMaps()
{
	return this->mui_ExistingColorMaps;

}

void mqMorphoDigCore::Addmui_ExistingArrays(QString Scalar, int dataType, int numComp, int toglobalList)
{
	// if only_selected == 1 : populate the list of scalars of selected actors
	int exists = 0;
	QString none = QString("Solid color");
	/*if (this->mui_ExistingArrays.size() == 1 && this->mui_ExistingArrays.at(0) == none)
	{
		cout << "1 scalar, and this is none! Confirmation:" << this->mui_ExistingArrays.at(0).toStdString() << endl;
		this->mui_ExistingArrays.clear();
		this->mui_ExistingArrays.push_back(Scalar);
		
	}
	else
	{*/
		
		//check first if Scalar already exists!
	ExistingArrays * myCurrentArrayList;
	if (toglobalList ==1)
	{
		myCurrentArrayList = this->mui_ExistingArrays;
	}
	else
	{
		myCurrentArrayList = this->mui_ArrayList;
	}

		for (int i = 0; i < myCurrentArrayList->Stack.size(); i++)
		{
			QString myScalar = myCurrentArrayList->Stack.at(i).Name;
			if (myScalar == Scalar)
			{
				exists = 1;
				//cout << Scalar.toStdString() << " already exists" << endl;
			}

		}
		if (exists == 0)
		{
			myCurrentArrayList->Stack.push_back(ExistingArrays::Element(Scalar, dataType, numComp));
			
		}
	/*}*/
}


void mqMorphoDigCore::Initmui_ExistingArrays()
{
	// browse through all actors and check for existing scalar
	// add
	cout << "Init mui existing scalars" << endl;
	QStringList existing;
	this->ActorCollection->InitTraversal();
	this->mui_ExistingArrays->Stack.clear();
	QString none = QString("Solid color");
	this->mui_ExistingArrays->Stack.push_back(ExistingArrays::Element(none,-1,0));
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMDActor * myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		vtkPolyDataMapper *mapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());
		
		if (mapper != NULL && vtkPolyData::SafeDownCast(mapper->GetInput()) != NULL)
		{
			vtkPolyData *myPD = vtkPolyData::SafeDownCast(mapper->GetInput());
			int nbarrays = myPD->GetPointData()->GetNumberOfArrays();
			for (int i = 0; i < nbarrays; i++)
			{
				//cout << "Array " << i << "=" << myPD->GetPointData()->GetArrayName(i) << endl;
				int num_comp = myPD->GetPointData()->GetArray(i)->GetNumberOfComponents();
				//cout << "Array" << i << " has "<<myPD->GetPointData()->GetArray(i)->GetNumberOfComponents()<< " components"<<endl;
				//std::cout << VTK_UNSIGNED_CHAR << " unsigned char" << std::endl;
				//std::cout << VTK_UNSIGNED_INT << " unsigned int" << std::endl;
				//std::cout << VTK_FLOAT << " float" << std::endl;
				//std::cout << VTK_DOUBLE << " double" << std::endl;
				int dataType = myPD->GetPointData()->GetArray(i)->GetDataType();
				if (dataType == VTK_UNSIGNED_CHAR) { 
					//cout << "Array" << i << " contains UNSIGNED CHARs" << endl; 
				}
				if (dataType == VTK_UNSIGNED_INT) { 
					//cout << "Array" << i << " contains UNSIGNED INTs" << endl; 
				}
				if (dataType == VTK_INT) { 
					//cout << "Array" << i << " contains INTs" << endl; 
				}
				if (dataType == VTK_FLOAT) { 
					//cout << "Array" << i << " contains FLOATs" << endl; 
				}
				if (dataType == VTK_DOUBLE) { 
				//	cout << "Array" << i << " contains DOUBLEs" << endl; 
				}

				if (dataType == VTK_UNSIGNED_CHAR && (num_comp == 3 || num_comp == 4))
				{
					// ok to add RGB like scalars
					QString RGBlike = QString(myPD->GetPointData()->GetArrayName(i));
					this->Addmui_ExistingArrays(RGBlike, dataType, num_comp);
				}
				if ((dataType == VTK_UNSIGNED_INT || dataType == VTK_INT)  && (num_comp == 1))
				{
					// ok to add TAG like scalars
					QString Taglike = QString(myPD->GetPointData()->GetArrayName(i));
					this->Addmui_ExistingArrays(Taglike, dataType, num_comp);
				}
				if ((dataType == VTK_FLOAT || (dataType == VTK_DOUBLE)) && (num_comp == 1))
				{
					// ok to add conventional scalars (like curvature, thickness, height etc... )
					QString ConvScalar = QString(myPD->GetPointData()->GetArrayName(i));
					this->Addmui_ExistingArrays(ConvScalar, dataType, num_comp);
				}
				
			}
			/*if ((vtkUnsignedCharArray*)myPD->GetPointData()->GetScalars("RGB") != NULL)
			{
				QString RGB = QString("RGB");
				this->Addmui_ExistingArrays(RGB);
			}
			if ((vtkUnsignedCharArray*)myPD->GetPointData()->GetScalars("Init_RGB") != NULL)
			{
				QString RGB = QString("Init_RGB");
				this->Addmui_ExistingArrays(RGB);
			}
			if ((vtkIntArray*)myPD->GetPointData()->GetScalars("Tags") != NULL)
			{
				
				
				QString Tags = QString("Tags");
				this->Addmui_ExistingArrays(Tags);
			}*/

		}

		
	}
	int exists = 0;
	

	for (int i = 0; i < this->mui_ExistingArrays->Stack.size(); i++)
	{
		QString myScalar = this->mui_ExistingArrays->Stack.at(i).Name;
		if (myScalar == this->mui_ActiveArray->Name)
		{
			exists = 1;
			
		}

	}

	// if RGB exists, and this->mui_ActiveArray ==none, set active scalar to RGB
	/*QString RGB = QString("RGB");
	for (int i = 0; i < this->mui_ExistingArrays->Stack.size(); i++)
	{
		QString myScalar = this->mui_ExistingArrays->Stack.at(i).Name;
		if (this->mui_ActiveArray->Name == none && myScalar ==RGB)
		{
			exists = 1;
			this->Setmui_ActiveArrayAndRender(this->mui_ExistingArrays->Stack.at(i).Name, this->mui_ExistingArrays->Stack.at(i).DataType, this->mui_ExistingArrays->Stack.at(i).NumComp);

		}

	}*/
	
	// last case : no RGB and none is not set as the active scalar before.
	if (exists == 0)
	{
		this->Setmui_ActiveArrayAndRender(none, -1, 0);
	}

	/*
	
	(((vtkUnsignedCharArray*)myPD->GetPointData()->GetScalars("RGB") == NULL) &&
							  ((vtkIntArray*)myPD->GetPointData()->GetScalars("Tags") == NULL))
								)
	*/
	this->signal_existingArraysChanged();
	
}

ActiveTagMap*   mqMorphoDigCore::Getmui_ActiveTagMap()
{
	return this->mui_ActiveTagMap;
}

ActiveColorMap * mqMorphoDigCore::Getmui_ActiveColorMap()
{
	return this->mui_ActiveColorMap;
}

ActiveArray* mqMorphoDigCore::Getmui_ActiveArray()
{
	return this->mui_ActiveArray;
}

void mqMorphoDigCore::RefreshColorMapsAndArrayVisibility()
{

	//1 refresh scalar bar actor if needed.
	int sba_refresh_needed = 0;
	if ((this->mui_ActiveArray->DataType == VTK_FLOAT || this->mui_ActiveArray->DataType == VTK_DOUBLE) && this->mui_ActiveArray->NumComp == 1)
	{
		sba_refresh_needed = 1;
	}
	
	if (sba_refresh_needed == 1)
	{
			this->ScalarBarActor->SetLookupTable(this->Getmui_ActiveColorMap()->ColorMap);
			this->ScalarBarActor->SetTitle(this->Getmui_ActiveArray()->Name.toStdString().c_str());
			
			
	}
	int tsba_refresh_needed = 0;
	if ((this->mui_ActiveArray->DataType == VTK_INT ) && this->mui_ActiveArray->NumComp == 1)
	{
		tsba_refresh_needed = 1;
	}

	if (tsba_refresh_needed == 1)
	{
		this->TagScalarBarActor->SetLookupTable(this->Getmui_ActiveTagMap()->TagMap);
		this->TagScalarBarActor->SetNumberOfLabels(this->Getmui_ActiveTagMap()->numTags);
		this->TagScalarBarActor->SetTitle(this->Getmui_ActiveArray()->Name.toStdString().c_str());


	}
	

	//2 refresh all actors
	this->ActorCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		cout << "Something here or..." << endl;
		vtkMDActor * myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		vtkPolyDataMapper *mapper = vtkPolyDataMapper::SafeDownCast(myActor->GetMapper());

		if (mapper != NULL && vtkPolyData::SafeDownCast(mapper->GetInput()) != NULL)
		{
			vtkPolyData *myPD = vtkPolyData::SafeDownCast(mapper->GetInput());
			//vtkPolyDataMapper::SafeDownCast(myActor->GetMapper())->ScalarVisibilityOff();
			QString none = QString("Solid color");

			if (
				(this->mui_ActiveArray->DataType == VTK_INT || this->mui_ActiveArray->DataType == VTK_UNSIGNED_INT)
				&& this->mui_ActiveArray->NumComp == 1
				)
			{
				cout << "Set Tag Lut!!!" << endl;
				mapper->SetScalarRange(0, this->Getmui_ActiveTagMap()->numTags - 1);
				mapper->SetLookupTable(this->Getmui_ActiveTagMap()->TagMap);


			}
			else
			{
				//mapper->SetScalarRange(0, 200);
				mapper->SetLookupTable(this->Getmui_ActiveColorMap()->ColorMap);
			
				
				//mapper->SetLookupTable(this->GetScalarRainbowLut());
			}

			if (this->mui_ActiveArray->Name == none)
			{
				vtkPolyDataMapper::SafeDownCast(myActor->GetMapper())->ScalarVisibilityOff();
				cout << "Scalar visibility off from Set mui active scalars" << endl;
			}
			else
			{


				if (myPD->GetPointData()->GetScalars(this->mui_ActiveArray->Name.toStdString().c_str()) != NULL)
				{
					if (this->mui_ArrayVisibility == 1 && myActor->GetSelected() == 0)
					{

						vtkPolyDataMapper::SafeDownCast(myActor->GetMapper())->ScalarVisibilityOn();
					}
					myPD->GetPointData()->SetActiveScalars(this->mui_ActiveArray->Name.toStdString().c_str());
				}
				else
				{
					vtkPolyDataMapper::SafeDownCast(myActor->GetMapper())->ScalarVisibilityOff();
				}
			}


		}
		

	}
}

void mqMorphoDigCore::Setmui_PencilSize(int pencilSize, int emit_signal)
{
	this->mui_PencilSize = pencilSize;
	if (emit_signal == 1)
	{
		emit this->pencilSizeChanged(pencilSize);
	}
}
int mqMorphoDigCore::Getmui_PencilSize()
{
	return this->mui_PencilSize;
}

void mqMorphoDigCore::Setmui_PencilContiguous(int pencilContiguous)
{
	this->mui_PencilContiguous = pencilContiguous;
}
int mqMorphoDigCore::Getmui_PencilContiguous()
{
	return this->mui_PencilContiguous;
}
void mqMorphoDigCore::Setmui_PencilLimitAngle(int pencilLimitAngle)
{
	this->mui_PencilLimitAngle = pencilLimitAngle;
}
int mqMorphoDigCore::Getmui_PencilLimitAngle()
{
	return this->mui_PencilLimitAngle;
}
void mqMorphoDigCore::Setmui_ActiveTag(int activeTag)
{
	this->mui_ActiveTag = activeTag;
}
int mqMorphoDigCore::Getmui_ActiveTag()
{
	return this->mui_ActiveTag;
}
void  mqMorphoDigCore::Setmui_ActiveTagMap(QString name, int numtags, std::vector<std::string> tagnames, vtkSmartPointer<vtkLookupTable> tagMap)
{
	this->mui_ActiveTagMap->Name = name;
	this->mui_ActiveTagMap->TagMap = tagMap;
	this->mui_ActiveTagMap->numTags = numtags;
	this->mui_ActiveTagMap->tagNames = tagnames;

	this->RefreshColorMapsAndArrayVisibility();
	cout << "Now active color map is " << name.toStdString() << endl;
}

void mqMorphoDigCore::Setmui_ActiveTagMapAndRender(QString name, int numtags, std::vector<std::string> tagnames, vtkSmartPointer<vtkLookupTable> tagMap)
{
	this->Setmui_ActiveTagMap(name, numtags, tagnames, tagMap);
	this->Render();
}
;



void mqMorphoDigCore::Setmui_ActiveColorMap(QString name, vtkSmartPointer<vtkDiscretizableColorTransferFunction> colorMap)
{
	this->mui_ActiveColorMap->Name = name;
	this->mui_ActiveColorMap->ColorMap = colorMap;

	this->RefreshColorMapsAndArrayVisibility();
	cout << "Now active color map is " << name.toStdString() << endl;
}
void mqMorphoDigCore::Setmui_ActiveColorMapAndRender(QString name, vtkSmartPointer<vtkDiscretizableColorTransferFunction> colorMap)
{
	this->Setmui_ActiveColorMap( name, colorMap);
	this->Render();
}

void mqMorphoDigCore::Setmui_ActiveArrayAndRender(QString Scalar, int dataType, int numComp)
{
	this->Setmui_ActiveArray(Scalar, dataType, numComp);
	emit this->activeArrayChanged();
	this->Render();
}
void mqMorphoDigCore::Setmui_ActiveArray(QString Scalar, int dataType, int numComp)
{
	int sba_refresh_needed = 0;
	int tsba_refresh_needed = 0;
	int old_sba_on = 0;
	int old_tsba_on = 0;
	int new_sba_on = 0;
	int new_tsba_on = 0;
	if ((dataType == VTK_FLOAT || dataType == VTK_DOUBLE) && numComp == 1)
	{
		new_sba_on = 1;
		
	}
	if ((dataType == VTK_INT ) && numComp == 1)
	{
		new_tsba_on = 1;

	}
	if ((this->mui_ActiveArray->DataType == VTK_FLOAT || this->mui_ActiveArray->DataType == VTK_DOUBLE) && this->mui_ActiveArray->NumComp == 1)
	{
		old_sba_on = 1;
	}
	if ((this->mui_ActiveArray->DataType == VTK_INT ) && this->mui_ActiveArray->NumComp == 1)
	{
		old_tsba_on = 1;
	}
	if (old_sba_on != new_sba_on)
	{
		sba_refresh_needed = 1;
		if (new_sba_on == 1)
		{
			this->ScalarBarActor->SetLookupTable(this->Getmui_ActiveColorMap()->ColorMap);
			this->ScalarBarActor->SetTitle(this->Getmui_ActiveArray()->Name.toStdString().c_str());
			
			this->Renderer->AddActor(ScalarBarActor);
		}
		else
		{
			this->Renderer->RemoveActor(ScalarBarActor);
		}
	}
	if (old_tsba_on != new_tsba_on)
	{
		tsba_refresh_needed = 1;
		if (new_tsba_on == 1)
		{
			this->TagScalarBarActor->SetLookupTable(this->Getmui_ActiveTagMap()->TagMap);
			this->TagScalarBarActor->SetNumberOfLabels(this->Getmui_ActiveTagMap()->numTags);
			
			this->TagScalarBarActor->SetTitle(Scalar.toStdString().c_str());
			
			//this->Renderer->AddActor(TagScalarBarActor);
		}
		else
		{
			//this->Renderer->RemoveActor(TagScalarBarActor);
		}
	}
	this->mui_ActiveArray->Name = Scalar;
	this->mui_ActiveArray->DataType = dataType;
	this->mui_ActiveArray->NumComp = numComp;
	cout << "Now active scalar is " << Scalar.toStdString() << endl;
	
	if ((this->mui_ActiveArray->DataType == VTK_INT) && this->mui_ActiveArray->NumComp == 1)
	{
		this->matchTagMapToActorCollection();
	}


	this->RefreshColorMapsAndArrayVisibility();
	
	

	cout << "Hello!!!!" << endl;

	// now brows through all actors and set active scalar 
	/*
			}*/

}




int mqMorphoDigCore::Getmui_DefaultAnaglyph() { return this->mui_DefaultAnaglyph; }
int mqMorphoDigCore::Getmui_Anaglyph() { return this->mui_Anaglyph; }
void mqMorphoDigCore::Setmui_Anaglyph(int anaglyph)
{
	this->mui_Anaglyph = anaglyph;
	if (this->RenderWindow != NULL)
	{
		if (anaglyph == 1)
		{
			this->RenderWindow->StereoRenderOn();
			this->RenderWindow->StereoUpdate();
			this->RenderWindow->Render();
		}
		else
		{
			this->RenderWindow->StereoRenderOff();
			this->RenderWindow->Render();
		}
	}
}


void mqMorphoDigCore::Setmui_LandmarkBodyType(int type) {
	this->mui_LandmarkBodyType = type; 
	this->UpdateLandmarkSettings();
}
int mqMorphoDigCore::Getmui_DefaultLandmarkBodyType() { return this->mui_DefaultLandmarkBodyType; }
int mqMorphoDigCore::Getmui_LandmarkBodyType() { return this->mui_LandmarkBodyType; }


void mqMorphoDigCore::Setmui_LandmarkMode(int mode) {
	this->mui_LandmarkMode = mode;
	//this->UpdateLandmarkSettings();
}
int mqMorphoDigCore::Getmui_DefaultLandmarkMode() { return this->mui_DefaultLandmarkMode; }
int mqMorphoDigCore::Getmui_LandmarkMode() { return this->mui_LandmarkMode; }


void mqMorphoDigCore::Setmui_LandmarkRenderingSize(double size)
{ this->mui_LandmarkRenderingSize = size;
this->UpdateLandmarkSettings();
}
double mqMorphoDigCore::Getmui_DefaultLandmarkRenderingSize() { return this->mui_DefaultLandmarkRenderingSize; }
double mqMorphoDigCore::Getmui_LandmarkRenderingSize() { return this->mui_LandmarkRenderingSize; }

void mqMorphoDigCore::Setmui_AdjustLandmarkRenderingSize(int adjust)
{
	this->mui_AdjustLandmarkRenderingSize = adjust;
	this->UpdateLandmarkSettings();
}
int mqMorphoDigCore::Getmui_DefaultAdjustLandmarkRenderingSize() { return this->mui_DefaultAdjustLandmarkRenderingSize; }
int mqMorphoDigCore::Getmui_AdjustLandmarkRenderingSize() { return this->mui_AdjustLandmarkRenderingSize; }

void mqMorphoDigCore::Setmui_AdjustScaleFactor(double factor) {
	this->mui_AdjustScaleFactor = factor; 
	this->UpdateLandmarkSettings();
}
double mqMorphoDigCore::Getmui_DefaultAdjustScaleFactor() { return this->mui_DefaultAdjustScaleFactor; }
double mqMorphoDigCore::Getmui_AdjustScaleFactor() { return this->mui_AdjustScaleFactor; }


void mqMorphoDigCore::Setmui_FlagRenderingSize(double size) { 
	this->mui_FlagRenderingSize = size; 
	cout << mui_FlagRenderingSize << endl;
}
double mqMorphoDigCore::Getmui_DefaultFlagRenderingSize() { return this->mui_DefaultFlagRenderingSize; }

double mqMorphoDigCore::Getmui_FlagRenderingSize() { return this->mui_FlagRenderingSize; 
//cout << "Default f r z" << this->mui_FlagRenderingSize<<endl;
}



double* mqMorphoDigCore::Getmui_MeshColor() { return this->mui_MeshColor; }
void mqMorphoDigCore::Getmui_MeshColor(double c[4])
{
	double *co = this->Getmui_MeshColor();

	c[0] = co[0];
	c[1] = co[1];
	c[2] = co[2];
	c[3] = co[3];
}
double* mqMorphoDigCore::Getmui_DefaultMeshColor() { return this->mui_DefaultMeshColor; }
void mqMorphoDigCore::Getmui_DefaultMeshColor(double c[4])
{
	double *co = this->Getmui_DefaultMeshColor();

	c[0] = co[0];
	c[1] = co[1];
	c[2] = co[2];
	c[3] = co[3];
}
void mqMorphoDigCore::Setmui_MeshColor(double c1, double c2, double c3, double c4)
{
	double c[4];
	c[0] = c1;
	c[1] = c2;
	c[2] = c3;
	c[3] = c4;
	

	this->Setmui_MeshColor(c);
}
void mqMorphoDigCore::Setmui_MeshColor(double c[4])
{
	this->mui_MeshColor[0] = c[0];
	this->mui_MeshColor[1] = c[1];
	this->mui_MeshColor[2] = c[2];
	this->mui_MeshColor[3] = c[3];
	//cout << "Core: this->mui_MeshColor[3]="<<this->mui_MeshColor[3] << endl;
	this->ActorCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected()==1)
		{
			
			myActor->SetmColor(this->mui_MeshColor);
			myActor->SetSelected(0);
		}
	}
}

double* mqMorphoDigCore::Getmui_FlagColor() { return this->mui_FlagColor; }
void mqMorphoDigCore::Getmui_FlagColor(double c[3])
{
	double *co = this->Getmui_FlagColor();

	c[0] = co[0];
	c[1] = co[1];
	c[2] = co[2];	
}

double* mqMorphoDigCore::Getmui_DefaultFlagColor() { return this->mui_DefaultFlagColor; }

void mqMorphoDigCore::Getmui_DefaultFlagColor(double c[3])
{
	double *co = this->Getmui_DefaultFlagColor();

	c[0] = co[0];
	c[1] = co[1];
	c[2] = co[2];
	
}
void mqMorphoDigCore::Setmui_FlagColor(double c1, double c2, double c3)
{
	double c[3];
	c[0] = c1;
	c[1] = c2;
	c[2] = c3;
	


	this->Setmui_FlagColor(c);
}
void mqMorphoDigCore::Setmui_FlagColor(double c[3])
{
	this->mui_FlagColor[0] = c[0];
	this->mui_FlagColor[1] = c[1];
	this->mui_FlagColor[2] = c[2];
	
	//cout << "Core: this->mui_MeshColor[3]="<<this->mui_MeshColor[3] << endl;
	/*this->FlagCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->FlagCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->ActorCollection->GetNextActor());
		if (myActor->GetSelected() == 1)
		{

			//myActor->SetmColor(this->mui_FlagColor);
			//myActor->SetSelected(0);
		}
	}*/
}



double* mqMorphoDigCore::Getmui_BackGroundColor() { return this->mui_BackGroundColor; }
void mqMorphoDigCore::Getmui_BackGroundColor(double bg[3])
{
	double *bgr = this->Getmui_BackGroundColor();

	bg[0] = bgr[0];
	bg[1] = bgr[1];
	bg[2] = bgr[2];
	
}


double* mqMorphoDigCore::Getmui_DefaultBackGroundColor() { return this->mui_DefaultBackGroundColor; }
void mqMorphoDigCore::Getmui_DefaultBackGroundColor(double bg[3])
{
	double *bgr = this->Getmui_DefaultBackGroundColor();

	bg[0] = bgr[0];
	bg[1] = bgr[1];
	bg[2] = bgr[2];

}


double* Getmui_DefaultBackGroundColor();
void Getmui_DefaultBackGroundColor(double bg[3]);
void mqMorphoDigCore::Setmui_BackGroundColor(double bg1, double bg2, double bg3)
{
	double background[3];
	background[0] = bg1;
	background[1] = bg2;
	background[2] = bg3;

	this->Setmui_BackGroundColor(background);
}
void mqMorphoDigCore::Setmui_BackGroundColor(double background[3])
{
	this->mui_BackGroundColor[0] = background[0];
	this->mui_BackGroundColor[1] = background[1];
	this->mui_BackGroundColor[2] = background[2];
	this->Renderer->SetBackground(background);
	//this->RenderWindow->Render();
}

double* mqMorphoDigCore::Getmui_BackGroundColor2() { return this->mui_BackGroundColor2; }
void mqMorphoDigCore::Getmui_BackGroundColor2(double bg[3])
{
	double *bgr = this->Getmui_BackGroundColor2();

	bg[0] = bgr[0];
	bg[1] = bgr[1];
	bg[2] = bgr[2];

}


double* mqMorphoDigCore::Getmui_DefaultBackGroundColor2() { return this->mui_DefaultBackGroundColor2; }
void mqMorphoDigCore::Getmui_DefaultBackGroundColor2(double bg[3])
{
	double *bgr = this->Getmui_DefaultBackGroundColor2();

	bg[0] = bgr[0];
	bg[1] = bgr[1];
	bg[2] = bgr[2];

}

void mqMorphoDigCore::Setmui_BackGroundColor2(double bg1, double bg2, double bg3)
{
	double background[3];
	background[0] = bg1;
	background[1] = bg2;
	background[2] = bg3;

	this->Setmui_BackGroundColor2(background);
}
void mqMorphoDigCore::Setmui_BackGroundColor2(double background[3])
{
	this->mui_BackGroundColor2[0] = background[0];
	this->mui_BackGroundColor2[1] = background[1];
	this->mui_BackGroundColor2[2] = background[2];
	this->Renderer->SetBackground2(background);
	//this->RenderWindow->Render();
}
double mqMorphoDigCore::AdjustedLandmarkSize()
{

	double bbl = this->ActorCollection->GetBoundingBoxLength();
	
	double adjusted_size = this->Getmui_AdjustScaleFactor()*bbl / 50;
	if (adjusted_size > 0 && bbl < DBL_MAX)
	{
		return adjusted_size;
	}
	else
	{
		if (this->Getmui_LandmarkRenderingSize() > 0)
		{
			return this->Getmui_LandmarkRenderingSize();

		}
		else
		{
			return this->Getmui_DefaultLandmarkRenderingSize();
		}
	}

}
void mqMorphoDigCore::UpdateLandmarkSettings(vtkLMActor *myActor)
{
	
	myActor->SetLMBodyType(this->Getmui_LandmarkBodyType());
	if (myActor->GetLMType() != FLAG_LMK)
	{
		
		if (this->Getmui_AdjustLandmarkRenderingSize() == 1)
		{
			//myActor->SetLMSize(this->Getmui_LandmarkRenderingSize());
			myActor->SetLMSize(this->AdjustedLandmarkSize());
			
		}
		else
		{
			myActor->SetLMSize(this->Getmui_LandmarkRenderingSize());
			
			//Change landmark size for all landmarks but flags.

		}
	}
	else
	{
		//Do not change size!!!
		myActor->SetLMSize(myActor->GetLMSize());
	}


	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(myActor->getLMBody());
	mapper->Update();
	myActor->SetMapper(mapper);
	//myActor->SetDisplayText(this->mui_DisplayLandmarkText);

}
void mqMorphoDigCore::UpdateLandmarkSettings()
{
	this->NormalLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NormalLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NormalLandmarkCollection->GetNextActor());
		UpdateLandmarkSettings(myActor);
		
	}
	this->TargetLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->TargetLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->TargetLandmarkCollection->GetNextActor());
		UpdateLandmarkSettings(myActor);

	}
	this->NodeLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NodeLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NodeLandmarkCollection->GetNextActor());
		UpdateLandmarkSettings(myActor);

	}
	this->HandleLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->HandleLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->HandleLandmarkCollection->GetNextActor());
		UpdateLandmarkSettings(myActor);

	}
	this->FlagLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->FlagLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->FlagLandmarkCollection->GetNextActor());
		UpdateLandmarkSettings(myActor);

	}
}

void mqMorphoDigCore::Undo()
{
	// a Set is only a label (action) and an id
	//vtkUndoSet *MyUndoSet = this->UndoStack->GetNextUndoSet();
	//this->ActorCollection->Undo(MySet);
	//cout << "Root Undo!" << endl;
	this->UndoStack->undo(); // removes the next undo set.. 
	emit this->actorsMightHaveChanged();
	emit this->volumesMightHaveChanged();

}
void mqMorphoDigCore::Undo(int Count)
{
	//cout << "Undo(" <<Count<<")"<< endl;
	//Calls for the Undo method in vtkActorCollection for this particular Count etc.. 
	this->ActorCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		cout << "MyActor" << myActor->GetName()<< "undo " <<Count<< endl;
		myActor->Undo(Count);		
	}
	this->ActorCollection->Undo(Count);

	this->VolumeCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->VolumeCollection->GetNumberOfItems(); i++)
	{
		vtkMDVolume *myVolume = vtkMDVolume::SafeDownCast(this->VolumeCollection->GetNextVolume());
		cout << "MyVolume " << myVolume->GetName() << " undo " << Count << endl;
		myVolume->Undo(Count);
	}
	this->VolumeCollection->Undo(Count);
	
	this->NormalLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NormalLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NormalLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor undo from core!" << endl;
		myActor->Undo(Count);
	}
	// To update to take into account reorder!
	this->NormalLandmarkCollection->Undo(Count);

	this->TargetLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->TargetLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->TargetLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor undo from core!" << endl;
		myActor->Undo(Count);
	}
	// To update to take into account reorder!
	this->TargetLandmarkCollection->Undo(Count);

	this->NodeLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NodeLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NodeLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor undo from core!" << endl;
		myActor->Undo(Count);
	}
	//@@ dirty! Recompute bezier curve whatever we do!
	// To update to take into account reorder!
	this->NodeLandmarkCollection->Undo(Count);
	this->NodeLandmarkCollection->ReorderLandmarks();
	this->NodeLandmarkCollection->Modified();

	this->HandleLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->HandleLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->HandleLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor undo from core!" << endl;
		myActor->Undo(Count);
	}
	
	// To update to take into account reorder!
	this->HandleLandmarkCollection->Undo(Count);

	//@@ dirty! Recompute bezier curve whatever we do!
	this->HandleLandmarkCollection->ReorderLandmarks();
	this->HandleLandmarkCollection->Modified();


	this->FlagLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->FlagLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->FlagLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor undo from core!" << endl;
		myActor->Undo(Count);
	}
	// To update to take into account reorder!
	this->FlagLandmarkCollection->Undo(Count);
	this->Initmui_ExistingArrays();
}
void mqMorphoDigCore::Redo()
{
	//cout << "Root Redo!" << endl;
	this->UndoStack->redo(); // removes the next undo set.. 
	emit this->actorsMightHaveChanged();
	emit this->volumesMightHaveChanged();
}

void mqMorphoDigCore::Redo(int Count)
{
	//cout << "Redo(" << Count << ")" << endl;
	//Calls for the Undo method in vtkActorCollection for this particular Count etc.. 
	this->ActorCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		//cout << "MyActor Redo!" << endl;
		myActor->Redo(Count);
	}
	this->ActorCollection->Redo(Count);

	this->VolumeCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->VolumeCollection->GetNumberOfItems(); i++)
	{
		vtkMDVolume *myVolume = vtkMDVolume::SafeDownCast(this->VolumeCollection->GetNextVolume());
		cout << "MyVolume" << myVolume->GetName() << "redo " << Count << endl;
		myVolume->Redo(Count);
	}
	this->VolumeCollection->Redo(Count);
	
	this->NormalLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NormalLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NormalLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor redo from core!" << endl;
		myActor->Redo(Count);
	}
	// To update to take into account reorder!
	this->NormalLandmarkCollection->Redo(Count);

	this->TargetLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->TargetLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->TargetLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor undo from core!" << endl;
		myActor->Redo(Count);
	}
	// To update to take into account reorder!
	this->TargetLandmarkCollection->Redo(Count);

	this->NodeLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NodeLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NodeLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor undo from core!" << endl;
		myActor->Redo(Count);
	}
	// To update to take into account reorder!
	this->NodeLandmarkCollection->Redo(Count);
	//@@ dirty! Recompute bezier curve whatever we do!
	this->NodeLandmarkCollection->ReorderLandmarks();
	this->NodeLandmarkCollection->Modified();

	this->HandleLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->HandleLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->HandleLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor undo from core!" << endl;
		myActor->Redo(Count);
	}

	// To update to take into account reorder!
	this->HandleLandmarkCollection->Redo(Count);
	//@@ dirty! Recompute bezier curve whatever we do!
	this->HandleLandmarkCollection->ReorderLandmarks();
	this->HandleLandmarkCollection->Modified();

	this->FlagLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->FlagLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->FlagLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor undo from core!" << endl;
		myActor->Redo(Count);
	}
	// To update to take into account reorder!
	this->FlagLandmarkCollection->Redo(Count);
	this->Initmui_ExistingArrays();
}

void mqMorphoDigCore::Erase(int Count)
{
	//cout << "Erase(" << Count << ")" << endl;
	//Calls for the Undo method in vtkActorCollection for this particular Count etc.. 
	this->ActorCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
	{
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
		//cout << "MyActor Erase!" << endl;
		myActor->Erase(Count);
	}
	this->ActorCollection->Erase(Count);

	this->NormalLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NormalLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NormalLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor redo from core!" << endl;
		myActor->Erase(Count);
	}
	// To update to take into account reorder!
	this->NormalLandmarkCollection->Erase(Count);

	this->TargetLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->TargetLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->TargetLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor undo from core!" << endl;
		myActor->Erase(Count);
	}
	// To update to take into account reorder!
	this->TargetLandmarkCollection->Erase(Count);

	this->NodeLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->NodeLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->NodeLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor undo from core!" << endl;
		myActor->Erase(Count);
	}
	// To update to take into account reorder!
	this->NodeLandmarkCollection->Erase(Count);

	this->HandleLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->HandleLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->HandleLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor undo from core!" << endl;
		myActor->Erase(Count);
	}
	// To update to take into account reorder!
	this->HandleLandmarkCollection->Erase(Count);

	this->FlagLandmarkCollection->InitTraversal();
	for (vtkIdType i = 0; i < this->FlagLandmarkCollection->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->FlagLandmarkCollection->GetNextActor());
		//cout << "Call MyLMActor undo from core!" << endl;
		myActor->Erase(Count);
	}
	// To update to take into account reorder!
	this->FlagLandmarkCollection->Erase(Count);


}
void mqMorphoDigCore::setUndoStack(mqUndoStack* stack)
{
	if (stack != this->UndoStack)
	{
		this->UndoStack = stack;
		/*if (stack)
		{
			stack->setParent(this);
		}*/
		//emit this->undoStackChanged(stack);
	}
}

//-----------------------------------------------------------------------------

/*vtkSmartPointer<vtkUndoStack> mqMorphoDigCore::getUndoStack()
{
	return this->UndoStack;
}*/

void mqMorphoDigCore::signal_existingArraysChanged()
{
	//cout << "Emit existing scalars changed" << endl;
	emit this->existingArraysChanged();
}

void mqMorphoDigCore::signal_projectionModeChanged()
{
	//cout << "Emit projection Mode Changed" << endl;
	emit this->projectionModeChanged();
}

void mqMorphoDigCore::signal_tagMapsChanged()
{
	emit this->tagMapsChanged();
}
void mqMorphoDigCore::signal_colorMapsChanged()
{
	//cout << "Emit projection Mode Changed" << endl;
	cout << "colorMapsChanged signal_colorMapsChanged" << endl;
	emit this->colorMapsChanged();
}
void mqMorphoDigCore::signal_zoomChanged()
{
	//cout << "Emit projection Mode Changed" << endl;
	emit this->zoomChanged();
}

void mqMorphoDigCore::signal_actorSelectionChanged()
{
	//cout << "Emit actor Selection changed" << endl;
	emit this->actorSelectionChanged();
}
void mqMorphoDigCore::signal_volumeSelectionChanged()
{
	//cout << "Emit volume Selection changed" << endl;
	emit this->volumeSelectionChanged();
}
void mqMorphoDigCore::signal_lmSelectionChanged()
{
	emit this->lmSelectionChanged();
}

mqUndoStack* mqMorphoDigCore::getUndoStack()
{
return this->UndoStack;
}

vtkSmartPointer<vtkActor> mqMorphoDigCore::getBezierActor()
{
	return this->BezierActor;
}
vtkSmartPointer<vtkActor> mqMorphoDigCore::getBezierSelectedActor()
{
	return this->BezierSelectedActor;
}
vtkSmartPointer<vtkActor> mqMorphoDigCore::getBezierNHActor()
{
	return this->BezierNHActor;
}
vtkSmartPointer<vtkBezierCurveSource> mqMorphoDigCore::getBezierCurveSource()
{
	return this->BezierCurveSource;
}
vtkSmartPointer<vtkMDActorCollection> mqMorphoDigCore::getActorCollection()
{
	return this->ActorCollection;
}
vtkSmartPointer<vtkMDVolumeCollection> mqMorphoDigCore::getVolumeCollection()
{
	return this->VolumeCollection;
}
vtkSmartPointer<vtkLMActorCollection> mqMorphoDigCore::getNormalLandmarkCollection()
{
	return this->NormalLandmarkCollection;
}
vtkSmartPointer<vtkLMActorCollection> mqMorphoDigCore::getTargetLandmarkCollection()
{
	return this->TargetLandmarkCollection;
}
vtkSmartPointer<vtkLMActorCollection> mqMorphoDigCore::getNodeLandmarkCollection()
{
	return this->NodeLandmarkCollection;
}
vtkSmartPointer<vtkLMActorCollection> mqMorphoDigCore::getHandleLandmarkCollection()
{
	return this->HandleLandmarkCollection;
}
vtkSmartPointer<vtkLMActorCollection> mqMorphoDigCore::getFlagLandmarkCollection()
{
	return this->FlagLandmarkCollection;
}
/*
vtkMDActorCollection* mqMorphoDigCore::getActorCollection()
{
	return this->ActorCollection;
}*/

vtkSmartPointer<vtkRenderer> mqMorphoDigCore::getRenderer()
{
	return this->Renderer;
}
vtkSmartPointer<vtkCamera> mqMorphoDigCore::getCamera()
{
	return this->Camera;
}
vtkSmartPointer<vtkGridActor> mqMorphoDigCore::getGridActor()
{
	return this->GridActor;
}

void mqMorphoDigCore::TransformPoint(vtkMatrix4x4* matrix, double pointin[3], double pointout[3]) {
	double pointPred[4]; double pointNew[4] = { 0, 0, 0, 0 };
	pointPred[0] = pointin[0];
	pointPred[1] = pointin[1];
	pointPred[2] = pointin[2];
	pointPred[3] = 1;

	matrix->MultiplyPoint(pointPred, pointNew);
	pointout[0] = pointNew[0];
	pointout[1] = pointNew[1];
	pointout[2] = pointNew[2];
}

void mqMorphoDigCore::RotateNorm(vtkMatrix4x4* matrix, double normin[3], double normout[3]) {
	/*result[0] = input[0]*mat[0][0] + input[1]*mat[1][0] + input[2]*mat[2][0];	
	result[1] = input[0]*mat[0][1] + input[1]*mat[1][1] + input[2]*mat[2][1];	
	result[2] = input[0]*mat[0][2] + input[1]*mat[1][2] + input[2]*mat[2][2];	*/
	
	normout[0] = normin[0] * matrix->GetElement(0, 0) + normin[1] * matrix->GetElement(0, 1) + normin[2] * matrix->GetElement(0, 2);
	normout[1] = normin[0] * matrix->GetElement(1, 0) + normin[1] * matrix->GetElement(1, 1) + normin[2] * matrix->GetElement(1, 2);
	normout[2] = normin[0] * matrix->GetElement(2, 0) + normin[1] * matrix->GetElement(2, 1) + normin[2] * matrix->GetElement(2, 2);
	/*normout[0] = normin[0] * matrix->GetElement(0, 0) + normin[1] * matrix->GetElement(1, 0) + normin[2] * matrix->GetElement(2, 0);
	normout[1] = normin[0] * matrix->GetElement(0, 1) + normin[1] * matrix->GetElement(1, 1) + normin[2] * matrix->GetElement(2, 1);
	normout[2] = normin[0] * matrix->GetElement(0, 2) + normin[1] * matrix->GetElement(1, 2) + normin[2] * matrix->GetElement(2, 2);*/

}
void mqMorphoDigCore::SetSelectedActorsColor(int r, int g, int b) 
{
	int num_selected = this->ActorCollection->GetNumberOfSelectedActors();
	if (num_selected > 0)
	{
		this->ActorCollection->InitTraversal();

		std::string action = "Modify color of selected actors";
		int mCount = BEGIN_UNDO_SET(action);
		
	
		for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
		{
			vtkMDActor * myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
			if (myActor->GetSelected() == 1)
			{
				myActor->SaveState(mCount);
				double color[4] = { 0.5, 0.5, 0.5, 1 };
				myActor->GetmColor(color);
				if (r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255)
				{
					color[0] = (double)((double)r / 255);
					color[1] = (double)((double)g / 255);
					color[2] = (double)((double)b / 255);

				}
				myActor->SetmColor(color);
				myActor->SetSelected(0);
				myActor->Modified();
			}
		}
		END_UNDO_SET();
		this->Render();
	}
}
void mqMorphoDigCore::GetWorldToDisplay(double x, double y, double z, double displayPt[3])
{
	this->getRenderer()->SetWorldPoint(x, y, z, 1.0);
	this->getRenderer()->WorldToDisplay();
	this->getRenderer()->GetDisplayPoint(displayPt);
}

void mqMorphoDigCore::GetDisplayToWorld(double x, double y, double z, double worldPt[4])
{

	this->getRenderer()->SetDisplayPoint(x, y, z);
	this->getRenderer()->DisplayToWorld();
	this->getRenderer()->GetWorldPoint(worldPt);
	if (worldPt[3])
	{
		worldPt[0] /= worldPt[3];
		worldPt[1] /= worldPt[3];
		worldPt[2] /= worldPt[3];
		worldPt[3] = 1.0;
	}
}
void mqMorphoDigCore::SetSelectedActorsTransparency(int trans) {

	int num_selected = this->ActorCollection->GetNumberOfSelectedActors();
	if (num_selected > 0)
	{
		this->ActorCollection->InitTraversal();

		std::string action = "Modify color of selected actors";
		int mCount = BEGIN_UNDO_SET(action);


		for (vtkIdType i = 0; i < this->ActorCollection->GetNumberOfItems(); i++)
		{
			vtkMDActor * myActor = vtkMDActor::SafeDownCast(this->ActorCollection->GetNextActor());
			if (myActor->GetSelected() == 1)
			{
				myActor->SaveState(mCount);
				double color[4] = { 0.5, 0.5, 0.5, 1 };
				myActor->GetmColor(color);
				if (trans >= 0 && trans <= 100 )
				{
					color[3] = (double)((double)trans / 100);
					

				}
				myActor->SetmColor(color);
				myActor->SetSelected(0);
				myActor->Modified();
			}
		}
		END_UNDO_SET();
		this->Render();
	}
}
void mqMorphoDigCore::slotRAMProgressBar(int percent)
{
	this->myRAMProgressBar->setValue(percent);

}
void mqMorphoDigCore::slotEditGridInfos()
{
	this->SetGridInfos();

}
void mqMorphoDigCore::slotExtractActiveTag()
{
	vtkIdType num_selected_meshes = this->getActorCollection()->GetNumberOfSelectedActors();
	if (num_selected_meshes == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}
	this->Extract_Tag_Range(this->Getmui_ActiveTag(), this->Getmui_ActiveTag());
}
void mqMorphoDigCore::slotDecomposeTag()
{
	// checks wether active array is tag array... 	
	ActiveArray *myActiveArray = mqMorphoDigCore::instance()->Getmui_ActiveArray();
	int ok = 0;
	if ((myActiveArray->DataType == VTK_INT || myActiveArray->DataType == VTK_UNSIGNED_INT) && myActiveArray->NumComp == 1)
	{
		ok = 1;
	}
	if (ok == 0)
	{
		QMessageBox msgBox;
		msgBox.setText("A Tag array must be active to use this option.");
		msgBox.exec();
		return;
	}
	vtkIdType num_selected_meshes =this->getActorCollection()->GetNumberOfSelectedActors();
	if (num_selected_meshes == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}

	//
	//this->Decompose_Tag();
	vtkSmartPointer<vtkIdTypeArray> region_sizes = vtkSmartPointer<vtkIdTypeArray>::New();
	// get min and max ranges.
	cout << "Try to Get min and max" << endl;
	int Tag_Min = this->GetTagRangeMin();
	cout << "Tag_min" << Tag_Min<< endl;
	int Tag_Max = this->GetTagRangeMax();
	cout << "Tag_max" << Tag_Max << endl;

	this->Decompose_Tag(Tag_Min, Tag_Max);
	

}

void mqMorphoDigCore::slotLandmarkMoveUp()
{

	this->LandmarksMoveUp();
}
void mqMorphoDigCore::slotLandmarkPushBack()
{
	this->LandmarksPushBack();
}

void mqMorphoDigCore::slotLandmarkReorient()
{
	this->LandmarksReorient();
}

void mqMorphoDigCore::slotUpdateAllSelectedFlagsColors()
{
	this->UpdateAllSelectedFlagsColors();
}
void mqMorphoDigCore::slotLandmarkMoveDown()
{

	this->LandmarksMoveDown();
}
void mqMorphoDigCore::slotGroup() { 
	
	vtkIdType num_selected_meshes = this->getActorCollection()->GetNumberOfSelectedActors();
	if (num_selected_meshes < 2) {
		QMessageBox msgBox;
		msgBox.setText("At least 2 surfaces must be selected to use this option.");
		msgBox.exec();
		return;
	}
	this->groupSelectedActors(); }

void mqMorphoDigCore::slotLassoCutKeepInside() { 
	
	vtkIdType num_selected_meshes = this->getActorCollection()->GetNumberOfSelectedActors();
	if (num_selected_meshes == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}
	this->startLasso(1); }
void mqMorphoDigCore::slotLassoCutKeepOutside() { 
	vtkIdType num_selected_meshes = this->getActorCollection()->GetNumberOfSelectedActors();
	if (num_selected_meshes == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}
	this->startLasso(0); }
void mqMorphoDigCore::slotLassoTagInside() { 
	
	
	this->startLasso(3); }
void mqMorphoDigCore::slotLassoTagOutside() { this->startLasso(2); }

void mqMorphoDigCore::slotRubberCutKeepInside() {
	
	vtkIdType num_selected_meshes = this->getActorCollection()->GetNumberOfSelectedActors();
	if (num_selected_meshes == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}
	this->startRubber(1); }
void mqMorphoDigCore::slotRubberCutKeepOutside() { 
	vtkIdType num_selected_meshes = this->getActorCollection()->GetNumberOfSelectedActors();
	if (num_selected_meshes == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}
	this->startRubber(0); }
void mqMorphoDigCore::slotRubberTagInside() { 
	
	
	this->startRubber(3); }
void mqMorphoDigCore::slotRubberTagOutside() { this->startRubber(2); }

void mqMorphoDigCore::slotConvexHULL() {
	vtkIdType num_selected_meshes = this->getActorCollection()->GetNumberOfSelectedActors();
	if (num_selected_meshes == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}
	this->addConvexHull(); }
void mqMorphoDigCore::slotMirror() { 
	vtkIdType num_selected_meshes = this->getActorCollection()->GetNumberOfSelectedActors();
	if (num_selected_meshes == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}
	this->addMirrorXZ(); }
void mqMorphoDigCore::slotInvert() { 
	vtkIdType num_selected_meshes = this->getActorCollection()->GetNumberOfSelectedActors();
	if (num_selected_meshes == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}
		this->addInvert(); 

}

void mqMorphoDigCore::slotCreateTagArray()
{
	vtkIdType num_selected_meshes = this->getActorCollection()->GetNumberOfSelectedActors();
	if (num_selected_meshes == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}
	QString TagArrayName = QString("Tags");
	QInputDialog *newTagName = new QInputDialog();
	bool dialogResult;
	QString newTags = newTagName->getText(0, "Tag array name:", "name:", QLineEdit::Normal,
		TagArrayName, &dialogResult);
	if (dialogResult)
	{
		cout << "Tag array chosen name:" << newTags.toStdString() << endl;
		this->createTags(newTags);
		this->Render();
	}
	else
	{
		cout << "cancel " << endl;
	}
}
void mqMorphoDigCore::slotCreateTagArrayConnectivity()
{
	QString TagArrayName = QString("ConnectivityTags");
	QInputDialog *newTagName = new QInputDialog();
	bool dialogResult;
	QString newTags = newTagName->getText(0, "Tag array name:", "name:", QLineEdit::Normal,
		TagArrayName, &dialogResult);
	if (dialogResult)
	{
		cout << "Tag array chosen name:" << newTags.toStdString() << endl;
		this->createTagsConnectivity(newTags);
	}
	else
	{
		cout << "cancel " << endl;
	}
}
void mqMorphoDigCore::slotScalarsRGB()
{
	vtkIdType num_selected_meshes = this->getActorCollection()->GetNumberOfSelectedActors();
	if (num_selected_meshes == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}
	QString RGB = QString("RGB");
	QInputDialog *newRGBName= new QInputDialog();
	bool dialogResult;
	QString newRGB = newRGBName->getText(0, "RGB array name", "name:", QLineEdit::Normal,
		RGB, &dialogResult);
	if (dialogResult)
	{
		cout << "RGB chosen name:" << newRGB.toStdString() << endl;
		this->scalarsRGB(newRGB);		
	}
	else
	{
		cout << "cancel " << endl;
	}


	
}

void mqMorphoDigCore::slotScalarsCameraDistance()
{
	this->scalarsCameraDistance();
}


void mqMorphoDigCore::slotKeepLargest() {
	vtkIdType num_selected_meshes = this->getActorCollection()->GetNumberOfSelectedActors();
	if (num_selected_meshes == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}
	this->addKeepLargest();
}


void mqMorphoDigCore::slotGrey() { this->SetSelectedActorsColor(150, 150, 150); }
void mqMorphoDigCore::slotYellow(){ this->SetSelectedActorsColor(165, 142, 22); }
void mqMorphoDigCore::slotRed(){ this->SetSelectedActorsColor(186, 37, 37); }
void mqMorphoDigCore::slotPink(){ this->SetSelectedActorsColor(173, 120, 95); }
void mqMorphoDigCore::slotBlue(){ this->SetSelectedActorsColor(64, 123, 126); }
void mqMorphoDigCore::slotViolet(){ this->SetSelectedActorsColor(120, 51, 145); }
void mqMorphoDigCore::slotBone(){ this->SetSelectedActorsColor(161, 146, 95); }
void mqMorphoDigCore::slotGreen(){ this->SetSelectedActorsColor(39, 136, 42); }
void mqMorphoDigCore::slotDarkred(){ this->SetSelectedActorsColor(115, 8, 15); }
void mqMorphoDigCore::slotDarkblue(){ this->SetSelectedActorsColor(52, 52, 160); }
void mqMorphoDigCore::slotDarkgreen(){ this->SetSelectedActorsColor(42, 110, 47); }
void mqMorphoDigCore::slotOrange(){ this->SetSelectedActorsColor(195, 91, 0); }
void mqMorphoDigCore::slotBrown(){ this->SetSelectedActorsColor(130, 78, 47); }
