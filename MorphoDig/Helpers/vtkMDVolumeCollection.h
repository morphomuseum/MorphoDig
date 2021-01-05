/*=========================================================================

Program:   MorphoDig
Module:    vtkMDVolumeCollection.h



=========================================================================*/


#ifndef vtkMDVolumeCollection_h
#define vtkMDVolumeCollection_h


#include <vtkVolumeCollection.h>
#include <vtkMDVolume.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vector>

class vtkMDVolumeCollectionUndoRedo
{
public:
	struct Element
	{
		vtkSmartPointer<vtkVolumeCollection> Collection;
		int UndoCount;
		int EventType; // CREATE_EVENT(1) or DELETE_EVENT
		Element(vtkSmartPointer<vtkVolumeCollection> col,  int event_type, int Count)
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

class  vtkMDVolumeCollection : public vtkVolumeCollection
{
public:
	static vtkMDVolumeCollection *New();
	vtkTypeMacro(vtkMDVolumeCollection, vtkVolumeCollection);

	
	void AddItem(vtkVolume *a);
	void AddTmpItem(vtkVolume *a);
	vtkMDVolume* GetFirstSelectedVolume();
	vtkMDVolume* GetLastVolume();
	vtkMDVolume *GetVolumeBefore(vtkMDVolume *Volume);
	vtkMDVolume *GetVolumeAfter(vtkMDVolume *Volume);
	vtkSetMacro(BoundingBoxLength, double);
	vtkSetMacro(BoundingBoxLengthOfSelectedVolumes, double);
	double GetBoundingBoxLengthOfSelectedVolumes();
	double GetBoundingBoxLength();

	vtkSetVector6Macro(BoundingBox, double);
	vtkGetVector6Macro(BoundingBox, double);

	vtkSetVector6Macro(BoundingBoxSelected, double);
	vtkGetVector6Macro(BoundingBoxSelected, double);
	
	vtkSetMacro(Changed, int);
	vtkGetMacro(Changed, int);
	vtkBooleanMacro(Changed, int);
	void PrintSelf(ostream& os, vtkIndent indent);
	void UpdateColorProperties(int ambient, int diffuse, int specular, double specularPower);

	
	virtual vtkIdType GetNumberOfSelectedVolumes();
	void GetCenterOfMass(double center[3]);
	void GetCenterOfMassOfSelectedVolumes(double center[3]);
	double * GetCenterOfMass();
	double * GetCenterOfMassOfSelectedVolumes();
	void DeleteSelectedVolumes(); //delete all selected Volumes
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
	
	vtkMDVolumeCollection();
	~vtkMDVolumeCollection();
	
	virtual void ApplyChanges(); // when Volumes have been modified (Volume themselves selection/unselection or list add, remove etc...)
	virtual void ComputeBoundingBoxLength();
	virtual void ComputeCenterOfMass(); // will be overridden in derived vtkLMVolumeCollection Class
	virtual int VolumeChanged(); // tells whether an Volume was changed.
	double centerOfMass[3];
	double centerOfMassOfSelectedVolumes[3];
	int Changed;
	double BoundingBox[6];
	double BoundingBoxSelected[6];
	double BoundingBoxLength;
	double BoundingBoxLengthOfSelectedVolumes;
	vtkSmartPointer<vtkRenderer> Renderer;
	vtkMDVolumeCollectionUndoRedo* UndoRedo;


private:
	vtkMDVolumeCollection(const vtkMDVolumeCollection&);  // Not implemented.
	void operator=(const vtkMDVolumeCollection&);  // Not implemented.
	
};

#endif

