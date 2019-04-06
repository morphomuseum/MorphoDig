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
#include <vtkKdTreePointLocator.h>
#include <vtkSmartPointer.h>
#include <vtkFloatArray.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vector>
#include <QString>
class vtkMDVolumeUndoRedo
{
public:
	struct Element
	{
		vtkSmartPointer<vtkMatrix4x4> Matrix;
		int Selected;
		int UndoCount;
		std::string Name;
		Element(vtkSmartPointer<vtkMatrix4x4> m,  int selected, int Count, std::string name)
		{
			this->Matrix =m;
			this->UndoCount = Count;
			this->Selected = selected;
			this->Name = name;
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
	void ShallowCopy(vtkProp *prop);

	//vtkSetMacro(Selected, int);
	vtkGetMacro(Selected, int);
	vtkBooleanMacro(Selected, int);

	vtkSetMacro(Changed, int);
	vtkGetMacro(Changed, int);
	vtkBooleanMacro(Changed, int);

	vtkGetMacro(Name, std::string);
	vtkSetMacro(Name, std::string);

	
	double GetBoundingBoxLength();
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
	void SetColorSpecular(double specular);
	void SetColorSpecularPower(double specularPower);
	void SetColorDiffuse(double diffuse);
	void SetColorProperties(double ambient, double diffuse, double specular, double specularPower);
	void SetColorProperties(int ambient, int diffuse, int specular, double specularPower);
	
protected:
	vtkMDVolume();
	~vtkMDVolume();
	
	int Selected;
	int Changed; // used by vtkMDVolumeCollectionUndoRedo class to recompute global center of mass and center of mass
	//of selected objects etc... 
	vtkMDVolumeUndoRedo* UndoRedo;
	std::string Name;

private:
	vtkMDVolume(const vtkMDVolume&);  // Not implemented.
	void operator=(const vtkMDVolume&);  // Not implemented.


	
};

#endif

