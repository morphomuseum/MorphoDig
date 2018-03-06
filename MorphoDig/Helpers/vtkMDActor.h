/*=========================================================================

Program:   MorphoDig
Module:    vtkMDActor.h



=========================================================================*/


#ifndef vtkMDActor_h
#define vtkMDActor_h


#include <vtkOpenGLActor.h>
#include <vtkMatrix4x4.h>
#include <vtkPlanes.h>
#include <vtkSmartPointer.h>
#include <vector>

class vtkMDActorUndoRedo
{
public:
	struct Element
	{
		vtkSmartPointer<vtkMatrix4x4> Matrix;
		double Color[4];
		int Selected;
		int UndoCount;
		std::string Name;
		Element(vtkSmartPointer<vtkMatrix4x4> m, double c[4], int selected, int Count, std::string name)
		{
			this->Matrix =m;
			this->UndoCount = Count;
			this->Color[0] = c[0];
			this->Color[1] = c[1];
			this->Color[2] = c[2];
			this->Color[3] = c[3];
			this->Selected = selected;
			this->Name = name;
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
	
	vtkTypeMacro(vtkMDActor, vtkOpenGLActor);
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

	//vtkSetVector4Macro(mColor, double);
	vtkGetVector4Macro(mColor, double);
	void SetmColor(double r, double g, double b, double a);
	void SetmColor(double c[4]);
	int IsInsideFrustum(vtkSmartPointer<vtkPlanes>myPlanes);
	// Actual actor render method.
	vtkIdType GetNumberOfPoints();
	void Render(vtkRenderer *ren, vtkMapper *mapper);
	virtual void SetSelected(int selected);
	virtual void SaveState(int mCount);
	virtual void Redo(int mCount); // Try to redo (if exists) "mCount" event
	virtual void Erase(int mCount); // Try to erase (if exists) "mCount" event
	virtual void Undo(int mCount); // Try to undo (if exists) "mCount" event
	virtual void PopUndoStack();
	virtual void PopRedoStack();
	virtual void ApplyMatrix(vtkSmartPointer<vtkMatrix4x4> Mat); // can be overriden in LMActor!
	

protected:
	vtkMDActor();
	~vtkMDActor();

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

