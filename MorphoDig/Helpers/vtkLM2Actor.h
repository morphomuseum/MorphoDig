/*=========================================================================

Program:   MeshTools
Module:    vtkLM2Actor.h



=========================================================================*/


#ifndef vtkLM2Actor_h
#define vtkLM2Actor_h


#include "vtkMTActor.h"
#include <vtkMatrix4x4.h>
#include <vtkSmartPointer.h>
#include <vector>

class vtkActor;
class vtkCaptionActor2D;
class vtkConeSource;
class vtkCylinderSource;
class vtkLineSource;
class vtkPolyData;
class vtkPropCollection;
class vtkProperty;
class vtkRenderer;
class vtkSphereSource;

class vtkLM2ActorUndoRedo
{
public:
	struct Element
	{
		vtkSmartPointer<vtkMatrix4x4> Matrix;
		//double Color[4];
		int Selected;
		int Number;
		int Type;
		int UndoCount;
		std::string Label;
		Element(vtkSmartPointer<vtkMatrix4x4> m, 
//			double c[4], 
			int selected, int number, int type, std::string label, int Count)
		{
			this->Matrix = m;
			this->Number = number;
			this->Type = type;
			this->Label = label;
			this->UndoCount = Count;
			/*this->Color[0] = c[0];
			this->Color[1] = c[1];
			this->Color[2] = c[2];
			this->Color[3] = c[3];*/
			this->Selected = selected;
		}
	};
	typedef std::vector<Element> VectorOfElements;
	VectorOfElements UndoStack;
	VectorOfElements RedoStack;
};

class  vtkLM2Actor : public  vtkMTActor
{
public:
	static vtkLM2Actor *New();
	vtkTypeMacro(vtkLM2Actor, vtkProp3D);
	void PrintSelf(ostream& os, vtkIndent indent);

	

	// Description:
	void ShallowCopy(vtkProp *prop);

	// Description:
	// Get the bounds for this Actor as (Xmin,Xmax,Ymin,Ymax,Zmin,Zmax). (The
	// method GetBounds(double bounds[6]) is available from the superclass.)
	
	void SetLMOrigin(double x, double y, double z);
	void SetLMOrigin(double origin[3]);
	void GetLMOrigin(double origin[3]);
	double * GetLMOrigin();
	void SetLMColor();
	void SetLMOrientation(double x, double y, double z);
	void SetLMOrientation(double orientation[3]);
	void GetLMOrientation(double orientation[3]);
	double * GetLMOrientation();

	virtual void SetSelected(int selected);
	virtual void SaveState(int mCount);
	virtual void Redo(int mCount); // Try to redo (if exists) "mCount" event
	virtual void Erase(int mCount); // Try to erase (if exists) "mCount" event
	virtual void Undo(int mCount); // Try to undo (if exists) "mCount" event
	virtual void PopUndoStack();
	virtual void PopRedoStack();
	// Description:
	// Get the actors mtime plus consider its properties and texture if set.







	// Description:
	// Retrieve handles to the X, Y and Z axis (so that you can set their text
	// properties for example)
	vtkSmartPointer<vtkCaptionActor2D> GetLMLabelActor2D()
	{
		return this->LMLabel;
	}


	// Description:
	// Set/get the label text.
	vtkSetStringMacro(LMLabelText);
	vtkGetStringMacro(LMLabelText);


	vtkSmartPointer<vtkPolyData> getLMBody() { return this->LMBody; }
	// Description:


	// Enable/disable drawing the axis labels.
	vtkSetMacro(LMDrawLabel, int);
	vtkGetMacro(LMDrawLabel, int);

	//void SetSelected(int selected);
	vtkGetMacro(Selected, int);

	vtkBooleanMacro(LMDrawLabel, int);
	vtkGetMacro(LMType, int);
	void SetLMType(int type);
	vtkGetMacro(LMBodyType, int);
	void SetLMBodyType(int type);
	vtkGetMacro(LMSize, double);
	void SetLMSize(double size);

	vtkGetMacro(LMNumber, int);
	void SetLMNumber(int num);

protected:
	vtkLM2Actor();
	~vtkLM2Actor();

	vtkSmartPointer<vtkPolyData> LMBody;	
	void               UpdateProps();
	char              *LMLabelText;
	vtkSmartPointer<vtkCaptionActor2D> LMLabel;
	int LMNumber; // landmark number in sequence
	int LMBodyType; //0; sphere //1 needle needle 
	int LMType; // 0; normal landmark (red) // 1 target landmark (yellow)
				// 2; curve node (dark red) // 3; curve handle (orange) // 4 curve starting point (green)
				// 5 curve milestone (blue) // 6 curve ending point (cyann)
	int                LMDrawLabel;
	double		     LMSize;
	double			LMOrigin[3];
	double			LMOrientation[3];
	int Selected;
	vtkLM2ActorUndoRedo* UndoRedo;
private:
	vtkLM2Actor(const vtkLM2Actor&);  // Not implemented.
	void operator=(const vtkLM2Actor&);  // Not implemented.
	void CreateLMBody(); //instantiates LM Body (sphere or needle)
	void CreateLMLabelText();//instantiates the  label actor



};

#endif

