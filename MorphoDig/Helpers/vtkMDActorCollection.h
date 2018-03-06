/*=========================================================================

Program:   MorphoDig
Module:    vtkMDActorCollection.h



=========================================================================*/


#ifndef vtkMDActorCollection_h
#define vtkMDActorCollection_h


#include <vtkActorCollection.h>
#include <vtkMDActor.h>
#include <vtkSmartPointer.h>
#include <vector>

class vtkMTCollectionUndoRedo
{
public:
	struct Element
	{
		vtkSmartPointer<vtkActorCollection> Collection;
		int UndoCount;
		int EventType; // CREATE_EVENT(1) or DELETE_EVENT
		Element(vtkSmartPointer<vtkActorCollection> col,  int event_type, int Count)
		{
			this->Collection = col;
			this->EventType = event_type;
			this->UndoCount = Count;
			
		}
	};
	typedef std::vector<Element> VectorOfElements;
	VectorOfElements UndoStack;
	VectorOfElements RedoStack;
};

class  vtkMDActorCollection : public vtkActorCollection
{
public:
	static vtkMDActorCollection *New();
	vtkTypeMacro(vtkMDActorCollection, vtkActorCollection);


	void AddItem(vtkActor *a);
	void AddTmpItem(vtkActor *a);
	vtkMDActor* GetFirstSelectedActor();
	vtkMDActor *GetActorBefore(vtkMDActor *Actor);
	vtkMDActor *GetActorAfter(vtkMDActor *Actor);
	vtkSetMacro(BoundingBoxLength, double);
	vtkSetMacro(BoundingBoxLengthOfSelectedActors, double);
	double GetBoundingBoxLengthOfSelectedActors();
	double GetBoundingBoxLength();

	vtkSetVector6Macro(BoundingBox, double);
	vtkGetVector6Macro(BoundingBox, double);

	vtkSetVector6Macro(BoundingBoxSelected, double);
	vtkGetVector6Macro(BoundingBoxSelected, double);
	
	vtkSetMacro(GlobalVN, int);
	vtkGetMacro(GlobalVN, int);
	vtkSetMacro(GlobalSelectedVN, int);
	vtkGetMacro(GlobalSelectedVN, int);
	vtkSetMacro(Changed, int);
	vtkGetMacro(Changed, int);
	vtkBooleanMacro(Changed, int);
	void PrintSelf(ostream& os, vtkIndent indent);
	
	void ApplyProperties(vtkProperty *p);
	virtual vtkIdType GetNumberOfSelectedActors();
	void GetCenterOfMass(double center[3]);
	void GetCenterOfMassOfSelectedActors(double center[3]);
	double * GetCenterOfMass();
	double * GetCenterOfMassOfSelectedActors();
	void DeleteSelectedActors(); //delete all selected actors
	void Redo(int mCount); // Try to redo (if exists) "mCount" event
	void Erase(int mCount); // Try to erase (if exists) "mCount" event
	void Undo(int mCount); // Try to undo (if exists) "mCount" event
	void PopUndoStack();
	void PopRedoStack();
	void CreateLoadUndoSet(int count, int creationcount);
	vtkSmartPointer<vtkRenderer> getRenderer() { return this->Renderer; }
	void SetRenderer(vtkSmartPointer<vtkRenderer> rend)
	{
		this->Renderer = rend;
	}

	

protected:
	
	vtkMDActorCollection();
	~vtkMDActorCollection();
	
	virtual void ApplyChanges(); // when actors have been modified (actor themselves selection/unselection or list add, remove etc...)
	virtual void ComputeBoundingBoxLength();
	virtual void ComputeCenterOfMass(); // will be overridden in derived vtkLMActorCollection Class
	virtual int ActorChanged(); // tells whether an actor was changed.
	double centerOfMass[3];
	double centerOfMassOfSelectedActors[3];
	int Changed;
	int GlobalVN;
	int GlobalSelectedVN;
	double BoundingBox[6];
	double BoundingBoxSelected[6];
	double BoundingBoxLength;
	double BoundingBoxLengthOfSelectedActors;
	vtkSmartPointer<vtkRenderer> Renderer;
	vtkMTCollectionUndoRedo* UndoRedo;


private:
	vtkMDActorCollection(const vtkMDActorCollection&);  // Not implemented.
	void operator=(const vtkMDActorCollection&);  // Not implemented.
	
};

#endif

