/*=========================================================================

Program:   MorphoDig
Module:    vtkMDVolume.h



=========================================================================*/


#ifndef vtkMDVolume_h
#define vtkMDVolume_h
#include <vtkVolume.h>
#include <vtkMatrix4x4.h>
#include <vtkDataArray.h>
#include <vtkPlanes.h>
#include <vtkSmartPointer.h>
#include <vtkImageAccumulate.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkOutlineFilter.h>
#include <vtkImageData.h>
#include <vtkKdTreePointLocator.h>
#include <vtkSmartPointer.h>
#include <vtkDiscretizableColorTransferFunction.h>
#include <vtkFloatArray.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkBoxWidget.h>
#include <vector>
#include <QString>
class vtkMDVolumeUndoRedo
{
public:
	struct Element
	{
		vtkSmartPointer<vtkMatrix4x4> Matrix;
		vtkSmartPointer<vtkDiscretizableColorTransferFunction> Ctf;
		int Selected;
		int UndoCount;
		double ScalarOpacityUnitDistance;
		double ScalarDisplayMin;
		double ScalarDisplayMax;
		
		std::string Name;
		Element(vtkSmartPointer<vtkMatrix4x4> m, vtkSmartPointer<vtkDiscretizableColorTransferFunction> ctf, int selected, int Count, std::string name, double SOUD, double DisplayMin, double DisplayMax)
		{
			this->Ctf = ctf;
			this->Matrix =m;
			this->UndoCount = Count;
			this->Selected = selected;
			this->Name = name;
			this->ScalarDisplayMax = DisplayMax;
			this->ScalarDisplayMin = DisplayMin;
			this->ScalarOpacityUnitDistance = SOUD;
		}
	};
	typedef std::vector<Element> VectorOfElements;
	VectorOfElements UndoStack;
	VectorOfElements RedoStack;
};

class  vtkMDVolume : public vtkVolume
{
public:
	static vtkMDVolume *New();
	
	vtkTypeMacro(vtkMDVolume, vtkVolume);
	void PrintSelf(ostream& os, vtkIndent indent);
	
	// Description:
	void GetInitCenter(double center[3]);
	void InitCenter();
	void ShallowCopy(vtkProp *prop);
	void CreateBox();
	void RemoveBox();
	//vtkSetMacro(Selected, int);
	vtkGetMacro(Selected, int);
	vtkBooleanMacro(Selected, int);


	vtkGetMacro(ScalarDisplayMax, double);
	//vtkSetMacro(ScalarDisplayMax, double);

	vtkGetMacro(ScalarDisplayMin, double);
	//vtkSetMacro(ScalarDisplayMin, double);

	vtkGetMacro(ScalarOpacityUnitDistance, double);
	//vtkSetMacro(ScalarOpacityUnitDistance, double);

	//isVisible
	void SetisVisible(int visible);
	//	vtkSetMacro(displayROI, int);
	vtkGetMacro(isVisible, int);

	void SetdisplayROI(int disp);
	//	vtkSetMacro(displayROI, int);
	vtkGetMacro(displayROI, int);
	
	void SetenableROI(int enable);
	//	vtkSetMacro(displayROI, int);
	vtkGetMacro(enableROI, int);

	vtkSetMacro(Changed, int);
	vtkGetMacro(Changed, int);
	vtkBooleanMacro(Changed, int);

	vtkGetMacro(Name, std::string);
	vtkSetMacro(Name, std::string);
	//vtkSetMacro(Ctf, vtkSmartPointer<vtkDiscretizableColorTransferFunction>);
	void SetCtf(vtkSmartPointer<vtkDiscretizableColorTransferFunction> ctf);
	vtkGetMacro(Ctf, vtkSmartPointer<vtkDiscretizableColorTransferFunction>);
	
	vtkGetMacro(Hist, vtkSmartPointer<vtkImageAccumulate>);
	vtkSetMacro(Hist, vtkSmartPointer<vtkImageAccumulate>);
	vtkSetMacro(ImageData, vtkSmartPointer<vtkImageData>);
	vtkGetMacro(ImageData, vtkSmartPointer<vtkImageData>);

	vtkSetMacro(Box, vtkSmartPointer<vtkBoxWidget>);
	vtkGetMacro(Box, vtkSmartPointer<vtkBoxWidget>);
	
	vtkSetMacro(Outline, vtkSmartPointer<vtkOutlineFilter>);
	vtkGetMacro(Outline, vtkSmartPointer<vtkOutlineFilter>);
	vtkSetMacro(OutlineMapper, vtkSmartPointer<vtkPolyDataMapper>);
	vtkGetMacro(OutlineMapper, vtkSmartPointer<vtkPolyDataMapper>);
	vtkSetMacro(OutlineActor, vtkSmartPointer<vtkActor>);
	vtkGetMacro(OutlineActor, vtkSmartPointer<vtkActor>);
	
	double GetBoundingBoxLength();
	double GetLookupTableMin();
	double GetLookupTableMax();
	void SetInterpolationToLinear(bool linear);
	void SetScalarOpacityUnitDistance(double SOUD);
	void SetScalarDisplayMax(double max);
	void SetScalarDisplayMin(double min);
	void UpdateLookupTableRange(double min, double max);
	void UpdateLookupTableRange();
	int IsInsideFrustum(vtkSmartPointer<vtkPlanes>myPlanes);
	// Actual Volume render method.
	virtual void SetSelected(int selected);
	virtual void SaveState(int mCount);
	virtual void Redo(int mCount); // Try to redo (if exists) "mCount" event
	virtual void Erase(int mCount); // Try to erase (if exists) "mCount" event
	virtual void Undo(int mCount); // Try to undo (if exists) "mCount" event
	virtual void PopUndoStack();
	virtual void PopRedoStack();
	virtual void ApplyMatrix(vtkSmartPointer<vtkMatrix4x4> Mat); 
	void SetColorAmbient(double ambient);
	void GetMCenter(double center[3]);
	double GetRangeMin();
	double GetRangeMax();
	void GetBoxCenter(double boxCenter[3]);
	void GetBoxBounds(double boxBounds[6]);
	void PlaceBox(double boxBounds[6]);

	void SetColorSpecular(double specular);
	void SetColorSpecularPower(double specularPower);
	void SetColorDiffuse(double diffuse);
	void SetColorProperties(double ambient, double diffuse, double specular, double specularPower);
	void SetColorProperties(int ambient, int diffuse, int specular, double specularPower);
	
protected:
	vtkMDVolume();
	~vtkMDVolume();
	
	int Selected;
	int displayROI;
	int enableROI;
	int isVisible;
	int Changed; // used by vtkMDVolumeCollectionUndoRedo class to recompute global center of mass and center of mass
	vtkSmartPointer<vtkDiscretizableColorTransferFunction> Ctf;
	vtkSmartPointer<vtkImageAccumulate> Hist;
	vtkSmartPointer<vtkImageData> ImageData;
	vtkSmartPointer<vtkBoxWidget> Box;
	vtkSmartPointer<vtkOutlineFilter> Outline;
	vtkSmartPointer<vtkPolyDataMapper> OutlineMapper;
	vtkSmartPointer<vtkActor> OutlineActor;
	//of selected objects etc... 
	vtkMDVolumeUndoRedo* UndoRedo;
	std::string Name;
	double ScalarDisplayMax;
	double ScalarDisplayMin;
	double ScalarOpacityUnitDistance;

private:
	double init_center[3];
	vtkMDVolume(const vtkMDVolume&);  // Not implemented.
	void operator=(const vtkMDVolume&);  // Not implemented.


	
};

#endif

