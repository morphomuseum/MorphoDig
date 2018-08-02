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
#include "QRAMThread.h"
#include "vtkOrientationHelperWidget.h"
#include "vtkMDActorCollection.h"
#include "vtkBezierCurveSource.h"
#include "vtkLMActorCollection.h"
#include "vtkMDInteractorStyle.h"
#include <vtkInteractorStyleDrawPolygon.h>

#include "vtkGridActor.h"
#include "vtkLMActor.h"
//#include "vtkUndoStack.h" => for some reason the ompilation fails if this header is included
//#include "vtkUndoStackInternal.h"

#include <QProgressBar>
#include <QVTKOpenGLWidget.h>
#include <vtkScalarBarActor.h>
#include <vtkKdTreePointLocator.h>
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
		int isCustom;

		Element(QString name, vtkSmartPointer<vtkDiscretizableColorTransferFunction> colormap, int custom=0)
		{
			this->Name = name;
			this->ColorMap = colormap;
			this->isCustom = custom;

		}
	};
	typedef std::vector<Element> VectorOfElements;
	VectorOfElements Stack;
	
};

class ExistingTagMaps
{
public:
	struct Element
	{
		QString Name;
		vtkSmartPointer<vtkLookupTable> TagMap;
		int numTags;
		std::vector<std::string> tagNames;
		int isCustom;

		Element(QString name, vtkSmartPointer<vtkLookupTable> tagmap, int numtags, std::vector<std::string> tagnames, int custom = 0)
		{
			this->Name = name;
			this->TagMap = tagmap;
			this->isCustom = custom;
			this->tagNames = tagnames;
			this->numTags = numtags;

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


class ActiveTagMap
{
public:
	QString Name;
	vtkSmartPointer<vtkLookupTable> TagMap;
	int numTags;
	std::vector<std::string> tagNames;

};


class ActiveColorMap
{
public:
	QString Name;
	vtkSmartPointer<vtkDiscretizableColorTransferFunction> ColorMap;

};

// 
class mqUndoStack;

typedef struct {
public:
	double x, y, z;
} POLYGON_VERTEX;
class POLYGON_LIST
{
public:
	
	std::vector<vtkVector2i> point_list;
	int point_number;
	int state; //state 0 = not finished or not valid; state 1 =closed and valide


	POLYGON_LIST()
	{
		this->point_list.clear();
		this->point_number = 0;
		this->state = 0;


	}
	int POLYGON_POINT_INSIDE(POLYGON_VERTEX P)
	{
		int    cn = 0;    // the crossing number counter

						  // loop through all edges of the polygon
		for (int i = 0; i<this->point_list.size() - 1; i++) {    // edge from V[i] to V[i+1]
			const vtkVector2i &V1 = point_list[i];
			const vtkVector2i &V2 = point_list[i+1];
			if (((V1[1] <= P.y) && (V2[1] > P.y))    // an upward crossing
				|| ((V1[1] > P.y) && (V2[1] <= P.y))) { // a downward crossing
																				// compute the actual edge-ray intersect x-coordinate
				double vt = (double)(P.y - V1[1]) / (V2[1] - V1[1]);
				if (P.x < V1[0] + vt * (V2[0] -V1[0])) // P.x < intersect
					++cn;   // a valid crossing of y=P.y right of P.x
			}
		}
		//last edge
		const vtkVector2i &F = point_list[0]; //first point
		const vtkVector2i &L = point_list[point_number - 1];//last point

		if (((F[1] <= P.y) && (L[1] > P.y))    // an upward crossing
			|| ((F[1] > P.y) && (L[1] <= P.y))) { // a downward crossing
																					   // compute the actual edge-ray intersect x-coordinate
			double vt = (double)(P.y - F[1]) / (L[1] - F[1]);
			if (P.x < F[0] + vt * (L[0] - F[0])) // P.x < intersect
				++cn;   // a valid crossing of y=P.y right of P.x
		}
		return (cn & 1);    // 0 if even (out), and 1 if odd (in)

	}
	void SetPointList(std::vector<vtkVector2i> points)
	{
		this->point_list = points;
		this->point_number = (int)points.size();
		this->state =0;
		this->state = this->Polygon_valid();
	}
	
	void Polygon_init()
	{
		
		this->point_number = 0;
		this->state = 0;


	}
	int Poly_cross(double x1, double y1, double x2, double y2, double u1, double u2, double v1, double v2)
	{

		double b1;
		double b2;
		double xi, yi;
		double a1, a2;
		if ((x2 - x1 == 0) || (u2 - u1 == 0))
		{
			return 0;
		}// Cases were one edge is vertical are avoided...(A and B exceptions)

		b1 = (y2 - y1) / (x2 - x1); //(A) 
		b2 = (v2 - v1) / (u2 - u1); // (B) 

		if (b2 - b1 == 0)
		{
			return 0;
		}// Cases were two edges are parallel are avoided. (C exception)

		a1 = y1 - b1*x1;
		a2 = v1 - b2*u1;
		xi = -(a1 - a2) / (b1 - b2); //(C) 
		yi = a1 + b1*xi;
		if (
			((x1 - xi)*(xi - x2) >= 0)
			&& ((u1 - xi)*(xi - u2) >= 0)
			&& ((y1 - yi)*(yi - y2) >= 0)
			&& ((v1 - yi)*(yi - v2) >= 0)
			)
		{
			return 1;
		}
		else { return 0; }


	}
	int Polygon_valid()						// Allocate Memory For Each Object
	{
		int x1, x2, y1, y2, u1, u2, v1, v2;
		// And Defines points

		int valid = 1;

		if (this->point_number < 3)
		{
			return 0;
		}
		else
		{//return 1;
			if (this->point_number == 3)
			{
				const vtkVector2i &A = point_list[0];
				const vtkVector2i &B = point_list[1];
				const vtkVector2i &C = point_list[2];
				
				x1 = (int)A[0];
				x2 = (int)B[0];
				y1 = (int)A[1];
				y2 = (int)B[1];
				u1 = (int)B[0];
				u2 = (int)C[0];
				v1 = (int)B[1];
				v2 = (int)C[1];
				if (Poly_cross((double)x1, (double)y1, (double)x2, (double)y2, (double)u1, (double)u2, (double)v1, (double)v2))
				{
					valid = 0;
				}
				//test if paralleles
			}
			else
			{
				int i;
				for (i = 0; i < point_number - 1; i++)
				{
					const vtkVector2i &V1 = point_list[i];
					const vtkVector2i &V2 = point_list[i + 1];
					x1 = (int)V1[0];
					x2 = (int)V2[0];
					y1 = (int)V1[1];
					y2 = (int)V2[1];
					for (int j = i + 2; j < point_number - 1; j++)
					{
						const vtkVector2i &U1 = point_list[j];
						const vtkVector2i &U2 = point_list[j + 1];
						//fprintf(stderr, "Segment %d - %d\n", i + 1, j + 1);
						u1 = (int)U1[0];
						u2 = (int)U2[0];
						v1 = (int)U1[1];
						v2 = (int)U2[1];


						if (Poly_cross((double)x1, (double)y1, (double)x2, (double)y2, (double)u1, (double)u2, (double)v1, (double)v2))
						{
							cout << "Segment " << j + 1 << "-" << j + 2 << ": U1[0]" << U1[0] << ", U1[1]" << U1[1] << ", U2[0]" << U2[0] << "U2[1]" << U1[1] << endl;
							cout << "Vs Segment " << i + 1 << "-" << i + 2 << ": V1[0]" << V1[0] << ", V1[1]" << V1[1] << ", V2[0]" << V2[0] << "V2[1]" << V1[1] << endl;

							valid = 0;
						}
					}
				
			}
			//Last edge
			const vtkVector2i &F = point_list[0]; //first point
			const vtkVector2i &L = point_list[point_number - 1];//last point
			x1 = (int)L[0];
			x2 = (int)F[0];
			y1 = (int)L[1];
			y2 = (int)F[1];
		
			for (i = 1; i<point_number - 2; i++)
			{
				const vtkVector2i &V1 = point_list[i];
				const vtkVector2i &V2 = point_list[i + 1];


					u1 = (int)V1[0];
					u2 = (int)V2[0];
					v1 = (int)V1[1];
					v2 = (int)V2[1];
					//fprintf(stderr, "Segment %d - %d\n", point_number, i + 1);
					
					if (Poly_cross((double)x1, (double)y1, (double)x2, (double)y2, (double)u1, (double)u2, (double)v1, (double)v2))
					{
						cout << "Last " << point_number << "-" << 1 << ": F[0]" << F[0] << ", F[1]" << F[1] << ", L[0]" << L[0] << "L[1]" << L[1] << endl;
						cout << "Vs Segment" << i + 1 << "-" << i + 2 << ": V1[0]" << V1[0] << ", V1[1]" << V1[1] << ", V2[0]" << V2[0] << "V2[1]" << V1[1] << endl;

						valid = 0;
					}
				}

			}
		}
		return valid;
	}		

};


class  mqMorphoDigCore : public QObject
{
	Q_OBJECT
		typedef QObject Superclass;
public:
	QRAMThread *myRAMThread;
	QProgressBar *myRAMProgressBar;
	std::vector<std::string> g_selected_names;
	std::vector<std::string> g_distinct_selected_names;
	void createTags(QString newTags);
	void createTagsFromRGB(QString newTags, int exact, int N);
	void createTagsConnectivity(QString newTags);
	void RemoveScalar(QString scalarName, int onlySelectedObjects);
	void GetDisplayToWorld(double x, double y, double z, double worldPt[4]);
	void GetWorldToDisplay(double x, double y, double z, double displayPt[3]);
	double GetHundredPxSU();
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
	int Getmui_TagModeActivated();
	void Setmui_TagModeActivated(int activated);
	int Getmui_TagTool();
	void Setmui_TagTool(int tool);
	void Setmui_MoveMode(int movemode);
	int Getmui_MoveMode();
	int Getmui_DefaultMoveMode();
	void SwitchMoveMode();
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

	void TagAt(vtkIdType pickid, vtkMDActor *myActor, int toverride);
	void SavePOS(vtkSmartPointer<vtkMatrix4x4> Mat, QString fileName);
	void SaveORI(QString fileName);
	int SaveNTWFile(QString fileName, int save_ori, int save_tag, int save_surfaces_as_ply, int apply_position_to_surfaces = 0);
	int SaveSTVFile(QString fileName, int save_only_selected);
	int SaveMAPFile(QString fileName, int save_only_active);
	void SaveMAP(QString fileName, QString Name, vtkSmartPointer<vtkDiscretizableColorTransferFunction> ColorMap);
	int SaveTAGMAPFile(QString fileName, int save_only_active);
	void SaveTAGMAP(QString fileName, int mapId, int mode);
	void OpenMAP(QString fileName);
	int SaveCURFile(QString fileName, int save_only_selected);
	int SaveCURasVERFile(QString fileName, int decimation, int save_format, int save_other_lmks);

	int SaveShapeMeasures(QString fileName, int mode);
	void SaveMeshSize(QString fileName);
	void SaveSelectedSurfaceScalars(vtkMDActor *myActor, QString fileName);
	void SaveActiveScalarSummary(QString fileName);
	int SaveSurfaceFile(QString fileName, int write_type, int position_mode, int file_type, std::vector<std::string> scalarsToBeRemoved, int RGBopt = 0, int save_norms = 0, vtkMDActor *myActor = NULL);
	int SaveLandmarkFile(QString fileName, int lm_type, int file_type, int save_only_selected);
	int SaveFlagFile(QString fileName, int save_only_selected);
	void DeleteSelectedActors();
	void OpenFLG(QString fileName);
	void OpenCUR(QString fileName);
	void OpenSTV(QString fileName);
	void MergeTags(int tagSource, int tagTarget);
	void OpenTAGMAP(QString fileName);
	void OpenORI(QString fileName);
	void OpenNTW(QString fileName);
	void OpenVER(QString fileName, int mode);
	void OpenLMK(QString fileName, int mode);
	void OpenMesh(QString fileName);
	void OpenPOS(QString fileName, int mode);
	void OpenPOSTrans(QString fileName, int mode);
	void GetVertexColor(vtkMDActor *myActor, vtkIdType ve, int color[4]);
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
	void EditScalarName(vtkSmartPointer<vtkMDActor> actor, QString oldScalarName, QString newScalarName);
	void DuplicateScalar(vtkSmartPointer<vtkMDActor> actor, QString ScalarName, QString newScalarName);
	void DeleteScalar(vtkSmartPointer<vtkMDActor> actor, QString ScalarName);
	ExistingScalars *Getmui_ScalarsOfActor(vtkSmartPointer<vtkMDActor> actor);
	ExistingScalars *Getmui_ScalarsOfSelectedObjects(int onlyfirst=0);
	ExistingScalars* Getmui_ExistingScalars();
	void Addmui_ExistingScalars(QString Scalar, int dataType, int numComp, int toglobalList =1);
	
	void Initmui_ExistingScalars();
	void Setmui_ActiveScalars(QString Scalar, int dataType, int numComp);

	void Setmui_ActiveScalarsAndRender(QString Scalar, int dataType, int numComp);
	void RefreshColorMapsAndScalarVisibility();
	ActiveScalars* Getmui_ActiveScalars();


	ExistingColorMaps* Getmui_ExistingColorMaps();
	ActiveColorMap* Getmui_ActiveColorMap();
	void Setmui_ActiveColorMap(QString name, vtkSmartPointer<vtkDiscretizableColorTransferFunction> colorMap);
	void Setmui_ActiveColorMapAndRender(QString name, vtkSmartPointer<vtkDiscretizableColorTransferFunction> colorMap);



	ExistingTagMaps* Getmui_ExistingTagMaps();
	ActiveTagMap* Getmui_ActiveTagMap();

	void Setmui_ActiveTagMap(QString name, int numtags, std::vector<std::string> tagnames, vtkSmartPointer<vtkLookupTable> tagMap);
	void Setmui_ActiveTagMapAndRender(QString name, int numtags, std::vector<std::string> tagnames, vtkSmartPointer<vtkLookupTable> tagMap);
	void Setmui_ActiveTag(int activeTag);
	int Getmui_ActiveTag();
	int Getmui_PencilSize();
	void Setmui_PencilSize(int pencilSize);

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
	void InitStatusBar();
	void SetProjectWindow(QMainWindow *_projectWindow);
	QMainWindow* GetMainWindow();
	QMainWindow* GetProjectWindow();
	void LandmarksMoveUp();
	void LandmarksPushBack();
	void LandmarksReorient();
	void LandmarksPushBackOrReorient(int mode);
	void LandmarkPushBackOrReorient(int mode, vtkSmartPointer<vtkLMActorCollection> LmkCollection, vtkSmartPointer<vtkKdTreePointLocator> kDTree,  vtkSmartPointer<vtkPolyData> PD, int mcount);
	
	
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
  vtkSmartPointer<vtkIdList> GetConnectedVertices(vtkSmartPointer<vtkPolyData> mesh, double *vn,
	  double sc, vtkIdType id, int tool_mode, int compute_avg_norm=0);
  void scalarsThickness(double max_thickness, int smooth_normals, int avg, QString scalarName, double angularLimit);
  void scalarsComplexity(double localAreaLimit, int customLocalAreaLimit, QString scalarName, int mode);
  void scalarsCurvature(int curvatureType, QString scalarName);
  void scalarsThicknessBetween(double max_thickness, int smooth_normals, int avg, QString scalarName, vtkMDActor *impactedActor, vtkMDActor* observedActor, double angularLimit, int invertObservedNormales =0);
  void scalarsDistance(double maxDist, int avg, QString scalarName, vtkMDActor *impactedActor, vtkMDActor* observedActor);
  void ICP(int transformationMode, int iterationNumber, vtkMDActor *impactedActor, vtkMDActor* observedActor);
  vtkMDActor * getFirstActorFromName(QString actorName);
  std::vector<std::string> getActorNames();
  
  void scalarsCameraDistance(); //compute camera distance for each selected scalar.
  void scalarsSmooth(double localAreaLimit, int cutMinMax, double cutPercent, int mode); //compute gaussian blur of current scalars
  void scalarsRGB(QString newRGB);
  void addDecompose(int color_mode, int min_region_size);// create for each selected surface as many object as extisting independent subregions in terms of connectivity.
  void addConvexHull();// create a convex hull for each selected surface
  void lassoCutSelectedActors(int keep_inside);
  void lassoTagActors(int tag_inside);
  void groupSelectedActors();
  void startLasso(int lasso_mode);//change interaction style
  void setCurrentCursor(int cursor); //changes mouse cursor
  void resetCursor(); // reset cursor when lasso stops, tag stop, landmark stop
  void stopLasso();//change interaction style back to normal
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
  void ReplaceCameraAndGridAt(double x, double y, double z);
  void DollyCameraForPerspectiveMode();
  void DollyCameraForParallelScale();
  void ResetCameraOrthoPerspective();
  void SetDisplayMode(int mode);
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
  static void RotateNorm(vtkMatrix4x4* matrix, double normin[3], double normout[3]);
  
  void signal_tagMapsChanged();
  void signal_colorMapsChanged();
  void signal_lmSelectionChanged();
  void signal_actorSelectionChanged();
  void signal_projectionModeChanged();
  void signal_zoomChanged();
  void signal_existingScalarsChanged();
  void signal_activeScalarChanged();
  //void signal_ActorsMightHaveChanged();
  vtkDiscretizableColorTransferFunction* GetOneColorMap();
  void UpdateLookupTablesRanges(double min, double max);

  void UpdateLookupTablesToData();
  void createCustomColorMap(QString name, vtkDiscretizableColorTransferFunction *STC);
  void createCustomTagMap(QString name);
  
  void invertRGB(vtkDiscretizableColorTransferFunction *STC);
  void invertOpacity(vtkDiscretizableColorTransferFunction *STC);
	double GetSuggestedScalarRangeMin();
	double GetSuggestedScalarRangeMax();
	double GetScalarRangeMin();
	double GetScalarRangeMax();
  void SetSelectedActorsTransparency(int trans);
  vtkSmartPointer<vtkLookupTable> GetTagLut();
  void GetDefaultTagColor(int tagnr, double rgba[4]);
  void setQVTKWidget(QVTKOpenGLWidget *mqvtkWidget);
  
  QVTKOpenGLWidget* getQVTKWidget();
  vtkSmartPointer<vtkDiscretizableColorTransferFunction> GetScalarRainbowLut();
  vtkSmartPointer<vtkDiscretizableColorTransferFunction> GetScalarRedLut();
  void SetNormalInteractorStyle(vtkSmartPointer<vtkMDInteractorStyle> mStyle);
  void SetLassoInteractorStyle(vtkSmartPointer<vtkInteractorStyleDrawPolygon> mLassoStyle);
  //void SetCurrentInteractorStyle(vtkSmartPointer<vtkMDInteractorStyle> mStyle);
  void InitLuts();
  void ComputeSelectedNamesLists();
  int colorMapNameAlreadyExists(QString proposed_name);
  int tagMapNameAlreadyExists(QString proposed_name);
  int tagAlreadyExists(int tagnr);
  void clearTag(int tagnr);
  int highestTagInActorCollection();
  void increaseTagNumberTo(int newtagnr);
  int getActiveTagMapId();
  int getTagMapId(QString tagMapName);
  void deleteColorMap(int i);
  void matchTagMapToActorCollection();
  
  void deleteTagMap(int i);
  void addTagToTagMap(int i);
  void removeTagFromTagMap(int i);
  void reinitializeColorMap(int i);
  void reinitializeTagMap(int i);
  double ComputeComplexity(vtkSmartPointer<vtkPolyData> mPD, vtkSmartPointer<vtkIdList> list, double sphere_radius, int mode, int printmode);
  double ComputeActiveScalarsMean(vtkSmartPointer<vtkPolyData> mPD, vtkSmartPointer<vtkIdList> list);
signals:
  
  void projectionModeChanged();
  void tagMapsChanged();
  void colorMapsChanged();
  void zoomChanged();
  void lmSelectionChanged();
  void actorSelectionChanged();
  void existingScalarsChanged();
  void activeScalarChanged();
  void actorsMightHaveChanged();
  void modeModeChanged();
  void thicknessProgression(int percent);
  void complexityProgression(int percent);
  void smoothingProgression(int percent);
  void distanceProgression(int percent);
protected:
	
	~mqMorphoDigCore();
	//vtkUndoStack* mUndoStack;
	vtkSmartPointer<vtkLookupTable> TagLut;
	//int TagTableSize;

	vtkSmartPointer<vtkMDInteractorStyle> Style;
	vtkSmartPointer<vtkInteractorStyleDrawPolygon> LassoStyle;
	vtkSmartPointer<vtkScalarBarActor> ScalarBarActor;
	vtkSmartPointer<vtkScalarBarActor> TagScalarBarActor;
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
	QMainWindow* ProjectWindow;
	//vtkUndoStack* UndoStack;
	mqUndoStack* UndoStack;
	//vtkSmartPointer<vtkUndoStack> UndoStack;

	//QString mui_ActiveScalars;
	ActiveScalars *mui_ActiveScalars;
	ExistingScalars *mui_ExistingScalars;
	ExistingScalars *mui_ScalarsList; //can be of a given actor, or of selected objects

	ActiveColorMap *mui_ActiveColorMap;
	ExistingColorMaps *mui_ExistingColorMaps;


	ActiveTagMap *mui_ActiveTagMap;
	ExistingTagMaps *mui_ExistingTagMaps;
	int mui_ActiveTag;
	int mui_PencilSize;

	QString mui_LastUsedDir;
	int mui_MoveMode;
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
	int mui_TagModeActivated;
	int mui_TagTool;
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
	int mui_DefaultMoveMode;
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
	int mui_DisplayMode; //0 cell 1 point 2 wireframe 3 points
	vtkOrientationHelperWidget* OrientationHelperWidget;
public slots:
	virtual void slotLandmarkMoveUp();
	virtual void slotLandmarkMoveDown();
	virtual void slotLandmarkPushBack();
	virtual void slotLandmarkReorient();
	virtual void slotUpdateAllSelectedFlagsColors();
	virtual void slotConvexHULL();
	virtual void slotGroup();
	virtual void slotLassoCutKeepInside();
	virtual void slotLassoCutKeepOutside();
	virtual void slotLassoTagInside();
	virtual void slotLassoTagOutside();
	virtual void slotMirror();
	virtual void slotInvert();
	virtual void slotKeepLargest();
	virtual void slotScalarsCameraDistance();
	virtual void slotScalarsRGB();
	virtual void slotCreateTagArray();
	virtual void slotCreateTagArrayConnectivity();
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
	virtual void slotEditGridInfos();
	virtual void slotRAMProgressBar(int percent);
private:
	static mqMorphoDigCore* Instance;
	QVTKOpenGLWidget *qvtkWidget;
	int currentLassoMode;
	int selected_file_exists(QString path, QString ext, QString postfix);
	int context_file_exists(QString path, QString ext, QString postfix);

};

#endif
