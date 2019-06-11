/*=========================================================================

Program:   MorphoDig
Module:    vtkMDActor.h



=========================================================================*/


#ifndef vtkMDActor_h
#define vtkMDActor_h


#include <vtkOpenGLActor.h>
#include <vtkMatrix4x4.h>
#include <vtkDataArray.h>
#include <vtkPlanes.h>
#include <vtkSmartPointer.h>
#include <vtkKdTreePointLocator.h>
#include <vtkSmartPointer.h>
#include <vtkFloatArray.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkBoxWidget.h>
#include <vector>
#include <QString>
class vtkMDActorUndoRedo
{
public:
	struct Element
	{
		vtkSmartPointer<vtkMatrix4x4> Matrix;
		vtkSmartPointer<vtkDataArray> sauvArray;
		vtkSmartPointer<vtkBoxWidget> Box;
		QString arrayName;
		int arrayType;// 0 double 1 int(tag) 2 char (RGB/RGBA)
		double Color[4];
		int Selected;
		int isVisible;
		int UndoCount;
		std::string Name;
		Element(vtkSmartPointer<vtkMatrix4x4> m, double c[4], int selected, int Count, std::string name, QString marrayName, vtkSmartPointer<vtkDataArray> marray, int mArrayType=0)
		{
			this->Matrix =m;
			this->UndoCount = Count;
			this->Color[0] = c[0];
			this->Color[1] = c[1];
			this->Color[2] = c[2];
			this->Color[3] = c[3];
			this->Selected = selected;
			this->Name = name;
			this->sauvArray = marray;
			this->arrayName = marrayName;
			this->arrayType = mArrayType;
		}
	};
	typedef std::vector<Element> VectorOfElements;
	VectorOfElements UndoStack;
	VectorOfElements RedoStack;
};

class  vtkMDActor : public vtkOpenGLActor
{
public:
	static vtkMDActor *New();
	void CreateBox();
	void RemoveBox();
	vtkTypeMacro(vtkMDActor, vtkOpenGLActor);
	void PrintSelf(ostream& os, vtkIndent indent);
	void BuildKdTree();
	void FreeKdTree();
	vtkSmartPointer<vtkKdTreePointLocator> GetKdTree();
	void SetDisplayMode(int mode);
	void BuildConnectivityFilter();
	void FreeConnectivityFilter();
	vtkSmartPointer<vtkPolyDataConnectivityFilter> GetConnectivityFilter();

	vtkSmartPointer<vtkIdTypeArray> GetConnectivityRegions();
	//vtkSmartPointer<vtkKdTreePointLocator> GetKdTree();
	vtkSmartPointer<vtkIdList> GetConnectivityRegionsCorrList();
	vtkSmartPointer<vtkIdList> GetConnectivityRegionsCorrList2();
	vtkIdType GetCorrPickedId(vtkIdType picked);
	// Description:
	void ShallowCopy(vtkProp *prop);

	//isVisible
	void SetisVisible(int visible);
	//	vtkSetMacro(displayROI, int);
	vtkGetMacro(isVisible, int);

	//vtkSetMacro(Selected, int);
	vtkGetMacro(Selected, int);
	vtkBooleanMacro(Selected, int);

	vtkSetMacro(Changed, int);
	vtkGetMacro(Changed, int);
	vtkBooleanMacro(Changed, int);

	vtkGetMacro(Name, std::string);
	vtkSetMacro(Name, std::string);

	vtkSetMacro(Box, vtkSmartPointer<vtkBoxWidget>);
	vtkGetMacro(Box, vtkSmartPointer<vtkBoxWidget>);
	void SetdisplayROI(int disp);
	//	vtkSetMacro(displayROI, int);
	vtkGetMacro(displayROI, int);

	//vtkSetVector4Macro(mColor, double);
	vtkGetVector4Macro(mColor, double);
	void SetmColor(double r, double g, double b, double a);
	void SetmColor(double c[4]);

	double GetBoundingBoxLength();
	double GetAvgCentroidDistance();
	double GetMaxCentroidDistance();
	double GetXYZAvgPCLength();
	double GetXYZPCLength(int pc);// gives a measure "size" of the mesh. Length of the vtkPolyData projected on PC1, PC2 or PC3 of a PCA of x,y,z coordinates of the VTK PolyData
	int IsInsideFrustum(vtkSmartPointer<vtkPlanes>myPlanes);
	// Actual actor render method.
	vtkIdType GetNumberOfPoints();
	vtkIdType GetNumberOfCells();
	void Render(vtkRenderer *ren, vtkMapper *mapper);
	virtual void SetSelected(int selected);
	virtual void SaveState(int mCount, QString arrayToSave = QString(), int arrayType=0);
	virtual void Redo(int mCount); // Try to redo (if exists) "mCount" event
	virtual void Erase(int mCount); // Try to erase (if exists) "mCount" event
	virtual void Undo(int mCount); // Try to undo (if exists) "mCount" event
	virtual void PopUndoStack();
	virtual void PopRedoStack();
	
	virtual void ApplyMatrix(vtkSmartPointer<vtkMatrix4x4> Mat); // can be overriden in LMActor!
	vtkSmartPointer<vtkFloatArray> GetPointNormals();
	vtkSmartPointer<vtkFloatArray> GetCellNormals();
	vtkSmartPointer<vtkFloatArray> pointNormals;
	vtkSmartPointer<vtkFloatArray> cellNormals;
	void SetColorAmbient(double ambient);
	void SetColorSpecular(double specular);
	void SetColorSpecularPower(double specularPower);
	void SetColorDiffuse(double diffuse);
	void SetColorProperties(double ambient, double diffuse, double specular, double specularPower);
	void SetColorProperties(int ambient, int diffuse, int specular, double specularPower);
	void GetBoxCenter(double boxCenter[3]);
	void GetBoxBounds(double boxBounds[6]);
protected:
	vtkMDActor();
	~vtkMDActor();
	vtkSmartPointer<vtkBoxWidget> Box;
	int displayROI;
	int isVisible;
	vtkSmartPointer<vtkKdTreePointLocator> KdTree;
	vtkSmartPointer<vtkPolyDataConnectivityFilter> cFilter;
	vtkSmartPointer<vtkIdList> cFilterCorrList;// for some reason vertice ids are not the same in cFilter input / output => so a corresponding list has to be built . Here: 1,2,3,4 = cFilter ids. GetId(i) returns original list id
	vtkSmartPointer<vtkIdList> cFilterCorrList2;// for some reason vertice ids are not the same in cFilter input / output => so a corresponding list has to be built. Here: 1,2,3,4 = original list ids. GetId(i) returns cfilter list id

	int Selected;
	int Changed; // used by MTActorCollection class to recompute global center of mass and center of mass
	//of selected objects etc... 
	double mColor[4]; // mesh "uniform" color (no vertex coloring) and transparency.
	vtkMDActorUndoRedo* UndoRedo;
	std::string Name;

private:
	vtkMDActor(const vtkMDActor&);  // Not implemented.
	void operator=(const vtkMDActor&);  // Not implemented.


	
};

#endif

