/*=========================================================================

  Program:   MorphoDig
  Module:    MorphoDigCore.h


=========================================================================*/
/**
 * @class   MorphoDigCore
 * @brief   Contains the most important objects such as the undo stack and the 
 * 
*/

#ifndef mqMorphoDigCore_h
#define mqMorphoDigCore_h
#include <QObject>
#include <QPointer>
#include "vtkOrientationHelperWidget.h"
#include "vtkMDActorCollection.h"
#include "vtkBezierCurveSource.h"
#include "vtkLMActorCollection.h"
#include "vtkGridActor.h"
#include "vtkLMActor.h"
//#include "vtkUndoStack.h" => for some reason the ompilation fails if this header is included
//#include "vtkUndoStackInternal.h"

#include <vtkScalarBarActor.h>
#include <vtkDiscretizableColorTransferFunction.h>
#include <vtkSmartPointer.h>    
#include <vtkCornerAnnotation.h>
#include <vtkCamera.h>
#include <vtkRenderWindow.h>
#include <QMainWindow>


class ExistingScalars
{
public:
	struct Element
	{
		QString Name;
		int DataType;
		int NumComp;

		Element(QString name, int datatype, int numcomp)
		{
			this->Name = name;
			this->DataType = datatype;
			this->NumComp = numcomp;

		}
	};
	typedef std::vector<Element> VectorOfElements;
	VectorOfElements Stack;
};

class ExistingColorMaps
{
public:
	struct Element
	{
		QString Name;
		vtkSmartPointer<vtkDiscretizableColorTransferFunction> ColorMap;


		Element(QString name, vtkSmartPointer<vtkDiscretizableColorTransferFunction> colormap)
		{
			this->Name = name;
			this->ColorMap = colormap;


		}
	};
	typedef std::vector<Element> VectorOfElements;
	VectorOfElements Stack;
	
};

class ActiveScalars
{
public:
	QString Name;
	int DataType;
	int NumComp;
	
};


class ActiveColorMap
{
public:
	QString Name;
	vtkSmartPointer<vtkDiscretizableColorTransferFunction> ColorMap;

};

// 
class mqUndoStack;

class  mqMorphoDigCore : public QObject
{
	Q_OBJECT
		typedef QObject Superclass;
public:
	std::vector<std::string> g_selected_names;
	std::vector<std::string> g_distinct_selected_names;

	void UnselectAll(int Count);
	void SelectAll(int Count);
	void Setmui_Anaglyph(int anaglyph);
	int Getmui_DefaultAnaglyph();
	int Getmui_Anaglyph();
	void GetCenterOfMassOfSelectedActors(double com[3]);
	double GetBoundingBoxLengthOfSelectedActors();
	void Setmui_ScalarVisibility(int scalarvisibility);
	int Getmui_DefaultScalarVisibility();
	int Getmui_ScalarVisibility();

	void Setmui_MoveAll(int moveall);
	int Getmui_MoveAll();
	int Getmui_DefaultMoveAll();

	void Setmui_ShowGrid(int showgrid);
	int Getmui_ShowGrid();
	int Getmui_DefaultShowGrid();

	void Setmui_GridSpacing(double gridspacing);
	double Getmui_GridSpacing();
	double Getmui_DefaultGridSpacing();

	void Setmui_SizeUnit(QString unit);
	QString Getmui_SizeUnit();
	QString Getmui_DefaultSizeUnit();

	

	void Setmui_ShowOrientationHelper(int orientationHelper);
	int Getmui_DefaultShowOrientationHelper();
	int Getmui_ShowOrientationHelper();

	void Setmui_CameraCentreOfMassAtOrigin(int comao);
	int Getmui_DefaultCameraCentreOfMassAtOrigin();
	int Getmui_CameraCentreOfMassAtOrigin();


	void Setmui_CameraOrtho(int ortho);
	int Getmui_DefaultCameraOrtho();
	int Getmui_CameraOrtho();

	void Setmui_LastUsedDir(QString dir);
	QString Getmui_LastUsedDir();

	void Setmui_X1Label(QString label);
	QString Getmui_DefaultX1Label();
	QString Getmui_X1Label();


	void Setmui_X2Label(QString label);
	QString Getmui_DefaultX2Label();
	QString Getmui_X2Label();

	void Setmui_Y1Label(QString label);
	QString Getmui_DefaultY1Label();
	QString Getmui_Y1Label();


	void Setmui_Y2Label(QString label);
	QString Getmui_DefaultY2Label();
	QString Getmui_Y2Label();

	void Setmui_Z1Label(QString label);
	QString Getmui_DefaultZ1Label();
	QString Getmui_Z1Label();


	void Setmui_Z2Label(QString label);
	QString Getmui_DefaultZ2Label();
	QString Getmui_Z2Label();
	void SavePOS(vtkSmartPointer<vtkMatrix4x4> Mat, QString fileName);
	void SaveORI(QString fileName);
	int SaveNTWFile(QString fileName, int save_ori, int save_tag, int save_surfaces_as_ply, int apply_position_to_surfaces =0);
	int SaveSTVFile(QString fileName, int save_only_selected);
	int SaveCURFile(QString fileName, int save_only_selected);
	int SaveCURasVERFile(QString fileName, int decimation, int save_format, int save_other_lmks);
	
	int SaveShapeMeasures(QString fileName, int mode);
	
	int SaveSurfaceFile(QString fileName, int write_type, int position_mode, int file_type, int save_norms = 0, vtkMDActor *myActor = NULL);
	int SaveLandmarkFile(QString fileName, int lm_type, int file_type, int save_only_selected);
	int SaveFlagFile(QString fileName, int save_only_selected);
	void DeleteSelectedActors();
	void OpenFLG(QString fileName);
	void OpenCUR(QString fileName);
	void OpenSTV(QString fileName);
	void OpenTAG(QString fileName);
	void OpenORI(QString fileName);
	void OpenNTW(QString fileName);
	void OpenVER(QString fileName, int mode);
	void OpenLMK(QString fileName, int mode);
	void OpenMesh(QString fileName);
	void OpenPOS(QString fileName, int mode);
	void OpenPOSTrans(QString fileName, int mode);

	double* Getmui_MeshColor();
	void Getmui_MeshColor(double c[4]);
	double* Getmui_DefaultMeshColor();
	void Getmui_DefaultMeshColor(double c[4]);
	void Setmui_MeshColor(double c1, double c2, double c3, double c4);
	void Setmui_MeshColor(double c[4]);


	double* Getmui_FlagColor();
	void Getmui_FlagColor(double c[3]);
	double* Getmui_DefaultFlagColor();
	void Getmui_DefaultFlagColor(double c[3]);
	void Setmui_FlagColor(double c1, double c2, double c3);
	void Setmui_FlagColor(double c[3]);

	void Setmui_LandmarkBodyType(int type);
	int Getmui_DefaultLandmarkBodyType();
	int Getmui_LandmarkBodyType();

	void Setmui_LandmarkMode(int mode);
	int Getmui_DefaultLandmarkMode();
	int Getmui_LandmarkMode();

	void Setmui_LandmarkRenderingSize(double size);
	double Getmui_DefaultLandmarkRenderingSize();
	double Getmui_LandmarkRenderingSize();


	void Setmui_AdjustLandmarkRenderingSize(int adjust);
	int Getmui_DefaultAdjustLandmarkRenderingSize();
	int Getmui_AdjustLandmarkRenderingSize();

	void Setmui_AdjustScaleFactor(double factor);
	double Getmui_DefaultAdjustScaleFactor();
	double Getmui_AdjustScaleFactor();

	void Setmui_FlagRenderingSize(double size);
	double Getmui_DefaultFlagRenderingSize();
	double Getmui_FlagRenderingSize();




	double* Getmui_BackGroundColor();
	void Getmui_BackGroundColor(double bg[3]);
	double* Getmui_DefaultBackGroundColor();
	void Getmui_DefaultBackGroundColor(double bg[3]);
	void Setmui_BackGroundColor(double bg1, double bg2, double bg3);
	void Setmui_BackGroundColor(double background[3]);


	ExistingScalars* Getmui_ExistingScalars();
	void Addmui_ExistingScalars(QString Scalar, int dataType, int numComp);
	void Initmui_ExistingScalars();
	void Setmui_ActiveScalars(QString Scalar, int dataType, int numComp);

	void Setmui_ActiveScalarsAndRender(QString Scalar, int dataType, int numComp);
	void RefreshColorMapsAndScalarVisibility();
	ActiveScalars* Getmui_ActiveScalars();


	ExistingColorMaps* Getmui_ExistingColorMaps();
	ActiveColorMap* Getmui_ActiveColorMap();
	void Setmui_ActiveColorMap(QString name, vtkSmartPointer<vtkDiscretizableColorTransferFunction> colorMap);
	void Setmui_ActiveColorMapAndRender(QString name, vtkSmartPointer<vtkDiscretizableColorTransferFunction> colorMap);

	

	double* Getmui_BackGroundColor2();
	void Getmui_BackGroundColor2(double bg[3]);
	double* Getmui_DefaultBackGroundColor2();
	void Getmui_DefaultBackGroundColor2(double bg[3]);

	void Setmui_BackGroundColor2(double bg1, double bg2, double bg3);
	void Setmui_BackGroundColor2(double background[3]);
	//static mqMorphoDigCore* New();
	static mqMorphoDigCore* instance();
	void UpdateLandmarkSettings();
	void UpdateLandmarkSettings(vtkLMActor *myActor);
	void SetMainWindow(QMainWindow *_mainWindow);
	QMainWindow* GetMainWindow();
	void LandmarksMoveUp();
	void ChangeClippingPlane();
	int Getmui_ClippinPlane();
	void Setmui_ClippinPlane(int on_off);

	void ActivateBackfaceCulling();
	void ChangeBackfaceCulling();
	int Getmui_BackfaceCulling();
	void Setmui_BackfaceCulling(int on_off);

	void ActivateClippingPlane();

	void LandmarksMoveDown();
	void SelectLandmarkRange(int start, int end, int lm_type);
	void UpdateAllSelectedFlagsColors();
	void UpdateAllSelectedFlagsColors(double flagcolor[4]);
	void UpdateAllSelectedFlagsLengths(double flag_rendering_size);
  //void SetRenderWindow(vtkSmartPointer<vtkRenderWindow> renwin)
  void SetRenderWindow(vtkRenderWindow *renwin)
  {
	  this->RenderWindow = renwin;
  }
  
  vtkSmartPointer<vtkActor> getBezierActor();
  vtkSmartPointer<vtkActor> getBezierNHActor();
  vtkSmartPointer<vtkActor> getBezierSelectedActor();
  vtkSmartPointer<vtkBezierCurveSource> getBezierCurveSource();
  vtkSmartPointer<vtkLMActorCollection> getNormalLandmarkCollection();
  vtkSmartPointer<vtkLMActorCollection> getTargetLandmarkCollection();
  vtkSmartPointer<vtkLMActorCollection> getNodeLandmarkCollection();
  vtkSmartPointer<vtkLMActorCollection> getHandleLandmarkCollection();
  vtkSmartPointer<vtkLMActorCollection> getFlagLandmarkCollection();

  vtkSmartPointer<vtkMDActorCollection> getActorCollection();
  //vtkMDActorCollection* getActorCollection();
  vtkSmartPointer<vtkRenderer> getRenderer();
  vtkSmartPointer<vtkCamera> getCamera();
  vtkSmartPointer<vtkGridActor> getGridActor();
  //void Invert();// Invert each selected surface
  bool RecoverLandmarks(vtkSmartPointer< vtkPoints > landmarks_list_source, vtkSmartPointer< vtkPoints > landmarks_list_target, int all);
  void addFillHoles(int maxsize);
  void addDensify(int subdivisions);
  void addTPS(int r, double factor, int all);
  void addSmooth(int iteration, double relaxation);
  void addDecimate(int quadric, double factor);
  void addInvert();// create an inverted surface for each selected surface
  void addKeepLargest();// create for each selected surface an object which keeps only the largest "independent" region of the corresponding object.
  void addDecompose(int color_mode, int min_region_size);// create for each selected surface as many object as extisting independent subregions in terms of connectivity.
  void addConvexHull();// create a convex hull for each selected surface
  void addMirrorXZ(); //create a mirror surface through XZ plane for each selected surface
  void Redo(); // calls the undoStack Redo function
  void Undo(); // callse the undoStack Undo function
  void Render(); // called when something has changed and the app needs to redraw
  void Redo(int Count); // send redo message to the concerned elements (actors mostly!)
  void Erase(int Count); // erase action number Count (in actors!)
  void Undo(int Count); // send undo message to the concerned elements (actors!)
  double AdjustedLandmarkSize();
  //vtkSmartPointer<vtkUndoStack> getUndoStack();
  mqUndoStack* getUndoStack();
  void setUndoStack(mqUndoStack* stack);
  mqUndoStack* getUndoStack() const { return this->UndoStack; }
  mqMorphoDigCore();
  void AdjustCameraAndGrid();
  void ReplaceCameraAndGrid();
  void DollyCameraForPerspectiveMode();
  void DollyCameraForParallelScale();
  void ResetCameraOrthoPerspective();
  void SetGridVisibility();
  void SetGridInfos();
  void SetOrientationHelperVisibility();
  std::string CheckingName(std::string name_obj);
  vtkMDActor* GetLastActor();
  vtkLMActor* GetLastLandmark(int mode);
  vtkMDActor * GetFirstSelectedActor();
  void ApplyMatrix(vtkSmartPointer<vtkMatrix4x4> Mat, int mode);
  void InitializeOrientationHelper();
  void SetOrientationHelperLabels(std::string X1, std::string X2, std::string Y1, std::string Y2, std::string Z1, std::string Z2);
  void ResetOrientationHelperLabels();
  void CreateLandmark(double coord[3], double ori[3], int lmk_type, int node_type = -1);
  void UpdateFirstSelectedLandmark(double coord[3], double ori[3]);
  static void TransformPoint(vtkMatrix4x4* matrix, double pointin[3], double pointout[3]);
  void signal_lmSelectionChanged();
  void signal_actorSelectionChanged();
  void signal_existingScalarsChanged();
  void signal_activeScalarChanged();
  double GetScalarRangeMin();
  void UpddateLookupTablesRanges(double min, double max);
	double GetScalarRangeMax();
	double GetSuggestedScalarRangeMin();
	double GetSuggestedScalarRangeMax();
  void SetSelectedActorsTransparency(int trans);
  vtkSmartPointer<vtkLookupTable> GetTagLut();
  vtkSmartPointer<vtkDiscretizableColorTransferFunction> GetScalarRainbowLut();
  vtkSmartPointer<vtkDiscretizableColorTransferFunction> GetScalarRedLut();
  void InitLuts();
  void ComputeSelectedNamesLists();
signals:

  void lmSelectionChanged();
  void actorSelectionChanged();
  void existingScalarsChanged();
  void activeScalarChanged();


protected:
	
	~mqMorphoDigCore();
	//vtkUndoStack* mUndoStack;
	vtkSmartPointer<vtkLookupTable> TagLut;
	int TagTableSize;

	vtkSmartPointer<vtkScalarBarActor> ScalarBarActor;
	vtkSmartPointer<vtkDiscretizableColorTransferFunction> ScalarRainbowLut;
	vtkSmartPointer<vtkDiscretizableColorTransferFunction> ScalarRedLut;

	vtkSmartPointer<vtkBezierCurveSource> BezierCurveSource;
	vtkSmartPointer<vtkCornerAnnotation> cornerAnnotation;
	vtkSmartPointer<vtkPolyDataMapper> BezierMapper;

	vtkSmartPointer<vtkActor> BezierActor;
	vtkSmartPointer<vtkPolyDataMapper> BezierNHMapper;

	vtkSmartPointer<vtkActor> BezierNHActor;
	vtkSmartPointer<vtkPolyDataMapper> BezierSelectedMapper;

	vtkSmartPointer<vtkActor> BezierSelectedActor;
	

	vtkSmartPointer<vtkMDActorCollection> ActorCollection;
	vtkSmartPointer<vtkLMActorCollection> NormalLandmarkCollection;
	vtkSmartPointer<vtkLMActorCollection> TargetLandmarkCollection;
	vtkSmartPointer<vtkLMActorCollection> NodeLandmarkCollection;
	vtkSmartPointer<vtkLMActorCollection> HandleLandmarkCollection;
	vtkSmartPointer<vtkLMActorCollection> FlagLandmarkCollection;
	//vtkMDActorCollection *ActorCollection;
	vtkSmartPointer<vtkRenderer> Renderer;
	//vtkSmartPointer<vtkRenderWindow> RenderWindow;
	vtkRenderWindow* RenderWindow;
	vtkSmartPointer<vtkCamera> Camera;
	vtkSmartPointer<vtkGridActor> GridActor;
	vtkSmartPointer<vtkLMActor> LMActor;
	vtkSmartPointer<vtkLMActor> LMActor2;
	QMainWindow* MainWindow;
	//vtkUndoStack* UndoStack;
	mqUndoStack* UndoStack;
	//vtkSmartPointer<vtkUndoStack> UndoStack;

	//QString mui_ActiveScalars;
	ActiveScalars *mui_ActiveScalars;
	ExistingScalars *mui_ExistingScalars;

	ActiveColorMap *mui_ActiveColorMap;
	ExistingColorMaps *mui_ExistingColorMaps;

	QString mui_LastUsedDir;
	int mui_MoveAll;
	int mui_ShowGrid;
	double mui_GridSpacing;
	QString mui_SizeUnit;
	double mui_DefaultGridSpacing;
	QString mui_DefaultSizeUnit;

	int mui_ClippingPlane;
	int mui_BackfaceCulling;

	int mui_ScalarVisibility;
	int mui_DefaultScalarVisibility;

	int mui_Anaglyph;
	int mui_ShowOrientationHelper;
	int mui_CameraCentreOfMassAtOrigin;
	int mui_CameraOrtho;

	QString mui_X1Label;
	QString mui_X2Label;
	QString mui_Y1Label;
	QString mui_Y2Label;
	QString mui_Z1Label;
	QString mui_Z2Label;

	QString mui_DefaultX1Label;
	QString mui_DefaultX2Label;
	QString mui_DefaultY1Label;
	QString mui_DefaultY2Label;
	QString mui_DefaultZ1Label;
	QString mui_DefaultZ2Label;

	int mui_DefaultShowGrid;
	int mui_DefaultMoveAll;
	int mui_DefaultAnaglyph;
	int mui_DefaultShowOrientationHelper;
	int mui_DefaultCameraCentreOfMassAtOrigin;
	int mui_DefaultCameraOrtho;

	double mui_MeshColor[4];
	double mui_FlagColor[3];
	double mui_BackGroundColor[3];
	double mui_BackGroundColor2[3];

	double mui_DefaultMeshColor[4];
	double mui_DefaultFlagColor[3];
	double mui_DefaultBackGroundColor[3];
	double mui_DefaultBackGroundColor2[3];
	
	int mui_DefaultLandmarkBodyType;
	int mui_LandmarkBodyType;

	double mui_DefaultLandmarkRenderingSize;
	double mui_LandmarkRenderingSize;

	double mui_DefaultAdjustLandmarkRenderingSize;
	double mui_AdjustLandmarkRenderingSize;

	double mui_DefaultAdjustScaleFactor;
	double mui_AdjustScaleFactor;

	int mui_DefaultFlagRenderingSize;
	int mui_FlagRenderingSize;

	int mui_DefaultLandmarkMode; // 0 normal 1target 2node 3handle 4flag
	int mui_LandmarkMode;
	double ScalarRangeMin;
	double ScalarRangeMax;
	void SetSelectedActorsColor(int r, int g, int b);
	
	vtkOrientationHelperWidget* OrientationHelperWidget;
public slots:
	virtual void slotLandmarkMoveUp();
	virtual void slotLandmarkMoveDown();
	virtual void slotUpdateAllSelectedFlagsColors();
	virtual void slotConvexHULL();
	virtual void slotMirror();
	virtual void slotInvert();
	virtual void slotKeepLargest();
	virtual void  slotGrey();
	virtual void slotYellow();
	virtual void slotRed();
	virtual void slotPink();
	virtual void slotBlue();
	virtual void slotViolet();
	virtual void slotBone();
	virtual void slotGreen();
	virtual void slotDarkred();
	virtual void slotDarkblue();
	virtual void slotDarkgreen();
	virtual void slotOrange();
	virtual void slotBrown();

private:
	static mqMorphoDigCore* Instance;

	
	int selected_file_exists(std::string path, std::string ext, std::string postfix);
	int context_file_exists(std::string path, std::string ext, std::string postfix);

};

#endif
