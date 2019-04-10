/*=========================================================================

Program:   MorphoDig
Module:    vtkMDVolumeCollection.cxx
=========================================================================*/
#include "vtkMDVolumeCollection.h"
#include "vtkMDVolume.h"

#include <vtkObjectFactory.h>
#include <vtkMath.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>
#include <vtkCenterOfMass.h>
#include <vtkSmartPointer.h>


#include <vtkDataSet.h>
#include <vtkMapper.h>

#include "mqUndoStack.h"
#include "mqMorphoDigCore.h"

vtkStandardNewMacro(vtkMDVolumeCollection);

#define CREATE_EVENT 1
#define DELETE_EVENT 0

//----------------------------------------------------------------------------
vtkMDVolumeCollection::vtkMDVolumeCollection()
{
	//this->Selected = 1;
	this->Renderer = NULL;
	this->UndoRedo = new vtkMDVolumeCollectionUndoRedo;
	this->centerOfMass[0] = 0;
	this->centerOfMass[1] = 0;
	this->centerOfMass[2] = 0;
	this->centerOfMassOfSelectedVolumes[0] = 0;
	this->centerOfMassOfSelectedVolumes[1] = 0;
	this->centerOfMassOfSelectedVolumes[2] = 0;
	this->BoundingBoxLength = 0;
	this->BoundingBoxLengthOfSelectedVolumes = 0;
	this->BoundingBox[0] = 0; 
	this->BoundingBox[1] = 0;
	this->BoundingBox[2] = 0;
	this->BoundingBox[3] = 0;
	this->BoundingBox[4] = 0;
	this->BoundingBox[5] = 0;
	this->BoundingBoxSelected[0] = 0;
	this->BoundingBoxSelected[1] = 0;
	this->BoundingBoxSelected[2] = 0;
	this->BoundingBoxSelected[3] = 0;
	this->BoundingBoxSelected[4] = 0;
	this->BoundingBoxSelected[5] = 0;
	

	this->Changed = 0;
}

vtkIdType vtkMDVolumeCollection::GetNumberOfSelectedVolumes()
{
	vtkIdType cpt = 0;
	this->InitTraversal();	
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{
		vtkVolume *act = this->GetNextVolume();
		std::string str1("vtkMDVolume");
		if (str1.compare(act->GetClassName()) == 0)
		{
			vtkMDVolume *myVolume = vtkMDVolume::SafeDownCast(act);
			if ( myVolume->GetSelected() == 1) {
				cpt++;
			}
		}
	}
	return cpt;


}

vtkMDVolume* vtkMDVolumeCollection::GetVolumeBefore(vtkMDVolume *Volume)
{
	vtkMDVolume *Before = NULL;
	this->InitTraversal();
	if (this->GetNumberOfItems() == 0) { return NULL; }

	
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{
		vtkMDVolume *myNextVolume = vtkMDVolume::SafeDownCast(this->GetNextVolume());
		

		if (myNextVolume ==Volume)
		{
			return Before;
		}
		Before = myNextVolume;

	}
	

	return Before;
}



vtkMDVolume *vtkMDVolumeCollection::GetVolumeAfter(vtkMDVolume *Volume)
{
	vtkMDVolume *After = NULL;
	int Volume_found = 0;
	this->InitTraversal();
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{
		After= vtkMDVolume::SafeDownCast(this->GetNextVolume());
		if (Volume_found == 1) { return After; }
		if (After ==Volume)
		{
			Volume_found = 1;
		}

	}
	

	return NULL;
}

vtkMDVolume* vtkMDVolumeCollection::GetFirstSelectedVolume()
{
	this->InitTraversal();
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{

		vtkVolume *vol = this->GetNextVolume();
		if (vol == NULL) { return NULL; }
		std::string str1("vtkMDVolume");
		if (str1.compare(vol->GetClassName()) == 0)
		{

			vtkMDVolume *myVolume = vtkMDVolume::SafeDownCast(vol);
			if (myVolume->GetSelected() == 1) { return myVolume; }
		}

	}


	return NULL;
}
vtkMDVolume* vtkMDVolumeCollection::GetLastVolume()
{
	this->InitTraversal();
	vtkVolume *act=NULL; 
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{
		act = this->GetNextVolume();				

	}
	if (act == NULL) { return NULL; }
	std::string str1("vtkMDVolume");
	if (str1.compare(act->GetClassName()) == 0)
	{

		vtkMDVolume *myVolume = vtkMDVolume::SafeDownCast(act);
		return myVolume; 
	}


	return NULL;
}
void vtkMDVolumeCollection::UpdateColorProperties(int ambient, int diffuse, int specular, double specularPower)
{

	double mAmbient = (double)ambient / 100;
	double mDiffuse = (double)diffuse / 100;
	double mSpecular = (double)specular / 100;
	this->InitTraversal();
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{

		vtkMDVolume *act = vtkMDVolume::SafeDownCast(this->GetNextVolume());
		if (act != NULL) {
			act->SetColorProperties(mAmbient, mDiffuse, mSpecular, specularPower);			

		}

	}


}
//----------------------------------------------------------------------------

vtkMDVolumeCollection::~vtkMDVolumeCollection()
{
	
	this->UndoRedo->RedoStack.clear();
	this->UndoRedo->UndoStack.clear();
	delete this->UndoRedo;
}
void vtkMDVolumeCollection::AddItem(vtkVolume *a)
{
	//cout << "Add item" << endl;
	this->Superclass::AddItem(a);
	//cout << "Item added" << endl;
	this->Renderer->AddVolume(a);
	//cout << "Volume added to renderer" << endl;

	mqMorphoDigCore::instance()->Initmui_ExistingArrays();

}
void vtkMDVolumeCollection::AddTmpItem(vtkVolume *a)
{
	//cout << "Add item" << endl;
	this->Superclass::AddItem(a);
	
	

}
void vtkMDVolumeCollection::CreateLoadUndoSet(int count, int creationcount)
{
	cout << "CreateLoadUndoSet(" << count << "," << creationcount << ")" << endl;
	vtkSmartPointer<vtkVolumeCollection> NewlyCreatedVolumes = vtkSmartPointer<vtkVolumeCollection>::New();
	// we retrieve the last "creationcount" created Volumes.
	this->InitTraversal();
	int num_Volumes = this->GetNumberOfItems();
	int num_toremain = num_Volumes - creationcount;
	vtkVolume *myVolume;
	if (num_toremain >= 0)
	{
		for (vtkIdType i = 0; i < num_toremain; i++)
		{
			//these Volumes will remain in the renderer in case of undo
			cout << "this one remains" << endl;
			myVolume = this->GetNextVolume();
		}	
		for (vtkIdType i = 0; i < creationcount; i++)
		{
			//these Volumes are put in the undo collection
			NewlyCreatedVolumes->AddItem(this->GetNextVolume());
		}
	}
	
	
	this->UndoRedo->UndoStack.push_back(vtkMDVolumeCollectionUndoRedo::Element(NewlyCreatedVolumes, CREATE_EVENT, count));
	

}


void vtkMDVolumeCollection::DeleteSelectedVolumes()
{
	int anychange = 0;
	this->InitTraversal();
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{
		vtkVolume *act = this->GetNextVolume();
		std::string str1("vtkMDVolume");
		if (str1.compare(act->GetClassName()) == 0)
		{
			vtkMDVolume *myVolume = vtkMDVolume::SafeDownCast(act);
			if (myVolume->GetSelected() == 1) { anychange = 1; }
		}
		
		
	}
	if (anychange == 1)
	{
		vtkSmartPointer<vtkVolumeCollection> undocoll = vtkSmartPointer<vtkVolumeCollection>::New();
		std::string action = "Delete selected Volumes";
		int mCount = BEGIN_UNDO_SET(action);
		
		int done = 0;
		while (!done)
		{
			
			if (this->GetNumberOfItems() == 0) { done = 1; }
			else
			{
				this->InitTraversal();
				int found = 0;
				for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
				{
					vtkVolume *act = this->GetNextVolume();
					std::string str1("vtkMDVolume");
					if (str1.compare(act->GetClassName()) == 0)
					{
						vtkMDVolume *myVolume = vtkMDVolume::SafeDownCast(act);
						if (!found && myVolume->GetSelected() == 1) {
							undocoll->AddItem(myVolume);
							this->RemoveItem(myVolume);
							this->Renderer->RemoveVolume(myVolume);
							found = 1;
						}
					}
					

					

				}
				if (found == 0) { done = 1; }
			}
		}
		this->InitTraversal();
		// the is not a load call (-1)
		this->UndoRedo->UndoStack.push_back(vtkMDVolumeCollectionUndoRedo::Element(undocoll,DELETE_EVENT, mCount));
		END_UNDO_SET();
		this->Changed = 1;
	}
	
	mqMorphoDigCore::instance()->Initmui_ExistingArrays();

} //delete all selected Volumes
void vtkMDVolumeCollection::Redo(int mCount) {
	//cout << "Inside Volume Undo, try to undo " <<mCount<< endl;
	if (this->UndoRedo->RedoStack.empty())
	{
		//cout << "Redo Stack empty!" << endl;
		return;
	}
	//cout << "Youngest redo count= " << this->UndoRedo->RedoStack.back().UndoCount<< endl;

	if (mCount == this->UndoRedo->RedoStack.back().UndoCount)
	{
		//cout << "Redo Volume event " << this->UndoRedo->RedoStack.back().UndoCount << endl;
		// ici : faire l'appel global à undo de ce count là!!  
		this->PopRedoStack();
		mqMorphoDigCore::instance()->Initmui_ExistingArrays();
	}

} // Try to redo (if exists) "mCount" event
void vtkMDVolumeCollection::Erase(int mCount) {

	if (this->UndoRedo->UndoStack.empty())
	{
		return;
	}
	int oldestCount = this->UndoRedo->UndoStack.front().UndoCount;
	if (oldestCount <= mCount)
	{
		//cout << "ERASE Volume event " << oldestCount << endl;
		this->UndoRedo->UndoStack.erase(this->UndoRedo->UndoStack.begin());
	}
} // Try to erase (if exists) "mCount" event
void vtkMDVolumeCollection::Undo(int mCount) 
{
	cout << "Inside MDVolumeCollection Undo " <<mCount<< endl;
	if (this->UndoRedo->UndoStack.empty())
	{
		cout << "Stack is empty" << endl;
		return;
	}
	if (mCount == this->UndoRedo->UndoStack.back().UndoCount)
	{
		cout << "Undo Volume event " << this->UndoRedo->UndoStack.back().UndoCount << endl;
		this->PopUndoStack();
		mqMorphoDigCore::instance()->Initmui_ExistingArrays();
	}
	else
	{

	}

} // Try to undo (if exists) "mCount" event
void vtkMDVolumeCollection::PopUndoStack() {
	if (this->UndoRedo->UndoStack.empty())
	{
		return;
	}
	cout << "PopUndoStack(" << this->UndoRedo->UndoStack.back().UndoCount << "," << this->UndoRedo->UndoStack.back().EventType << ")" << endl;
	vtkSmartPointer<vtkVolumeCollection> ActColl = this->UndoRedo->UndoStack.back().Collection;
	
	// If stored event was a DELETE_EVENT, we need to put back deleted object in renderer + this
	if (this->UndoRedo->UndoStack.back().EventType==DELETE_EVENT)
	{
		ActColl->InitTraversal();
		for (vtkIdType i = 0; i < ActColl->GetNumberOfItems(); i++)
		{
			//vtkMDVolume *myVolume = vtkMDVolume::SafeDownCast(ActColl->GetNextVolume());
			vtkVolume *myVolume = ActColl->GetNextVolume();
			this->AddItem(myVolume);
			this->Renderer->AddVolume(myVolume);
			// if myVolume is a landmark => Add label to the renderer

			
			
			this->Changed = 1;
		}
		this->UndoRedo->RedoStack.push_back(vtkMDVolumeCollectionUndoRedo::Element(ActColl, DELETE_EVENT, this->UndoRedo->UndoStack.back().UndoCount));
	}
	// If stored event was a CREATE_EVENT, we need to remove last inserted objects + from this
	else
	{	
		cout << "Try to remove last created objects" << endl;
		ActColl->InitTraversal();
		// Volume(s) creation (file loadingetc...)
		// this means that this action corresponds to "create a new Volume (or load a bunch of new Volumes)".
		for (vtkIdType i = 0; i < ActColl->GetNumberOfItems(); i++)
		{


			vtkVolume *myVolume= ActColl->GetNextVolume();
			this->RemoveItem(myVolume);
			this->Renderer->RemoveActor(myVolume);
			

		}

		this->UndoRedo->RedoStack.push_back(vtkMDVolumeCollectionUndoRedo::Element(ActColl, CREATE_EVENT, this->UndoRedo->UndoStack.back().UndoCount));
	}		
	this->UndoRedo->UndoStack.pop_back();
	this->Modified();
}
void vtkMDVolumeCollection::PopRedoStack() {
	if (this->UndoRedo->RedoStack.empty())
	{
		return;
	}
	cout << "PopRedoStack(" << this->UndoRedo->RedoStack.back().UndoCount << "," << this->UndoRedo->RedoStack.back().EventType << ")" << endl;
	vtkSmartPointer<vtkVolumeCollection> ActColl = this->UndoRedo->RedoStack.back().Collection;
	
	// If stored event was a DELETE_EVENT, we need to remove again deleted object in renderer + from this
	if (this->UndoRedo->RedoStack.back().EventType ==DELETE_EVENT)
	{
		ActColl->InitTraversal();
		for (vtkIdType i = 0; i < ActColl->GetNumberOfItems(); i++)
		{

			//vtkMDVolume *myVolume = vtkMDVolume::SafeDownCast(ActColl->GetNextVolume());
			vtkVolume *myVolume = ActColl->GetNextVolume();
			this->RemoveItem(myVolume);
			this->Renderer->RemoveVolume(myVolume);
			
			this->Changed = 1;
		}
		this->UndoRedo->UndoStack.push_back(vtkMDVolumeCollectionUndoRedo::Element(ActColl, DELETE_EVENT, this->UndoRedo->RedoStack.back().UndoCount));
	}
	else
	{
		// If stored event was a CREATE_EVENT, we need to put again corresponding objects in renderer + inside this
		ActColl->InitTraversal();
		cout << "Try to re-add last created Volume(s)" << endl;

		// Volume(s) creation (file loading / create 1 landmark etc...)
		// this means that this action corresponds to "create a new Volume (or load a bunch of new Volumes)".
		ActColl->InitTraversal();
		for (vtkIdType i = 0; i < ActColl->GetNumberOfItems(); i++)
		{

			vtkVolume *myVolume = ActColl->GetNextVolume();
			//cout << "Try to get last Volume, creation count = " << this->UndoRedo->RedoStack.back().CreationCount << "this->Number of itemps=" << this->GetNumberOfItems() << endl;

			this->AddItem(myVolume);
			this->Renderer->AddVolume(myVolume);
			// if myVolume is a landmark => Add label to the renderer
			
			this->Changed = 1;

		}		
		this->UndoRedo->UndoStack.push_back(vtkMDVolumeCollectionUndoRedo::Element(ActColl, CREATE_EVENT, this->UndoRedo->RedoStack.back().UndoCount));

	}

	
		
	this->UndoRedo->RedoStack.pop_back();
	this->Modified();

}
int vtkMDVolumeCollection::VolumeChanged()
{
	int anychange = 0;
	this->InitTraversal();
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{		
		vtkVolume *myVolume = this->GetNextVolume();
		
		std::string str1("vtkMDVolume");
		if (str1.compare(myVolume->GetClassName()) == 0)
		{
			vtkMDVolume *myMDVolume = vtkMDVolume::SafeDownCast(myVolume);
			if (myMDVolume->GetChanged() == 1) { anychange = 1; }
		}
		
	}
	return anychange;
}
void vtkMDVolumeCollection::ComputeCenterOfMass()
{
	//cout << "MD Compute center of mass" << endl;
	this->centerOfMass[0] = 0;
	this->centerOfMass[1] = 0;
	this->centerOfMass[2] = 0;
	this->centerOfMassOfSelectedVolumes[0] = 0;
	this->centerOfMassOfSelectedVolumes[1] = 0;
	this->centerOfMassOfSelectedVolumes[2] = 0;
	int numVol = this->GetNumberOfItems();
	int numSelVol = this->GetNumberOfSelectedVolumes();
	this->InitTraversal();
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{
		vtkMDVolume *myVolume = vtkMDVolume::SafeDownCast(this->GetNextVolume());
		double Vcenter[3];
		myVolume->GetMCenter(Vcenter);
		cout << "Vcenter:" << Vcenter[0] << "," << Vcenter[1] << "," << Vcenter[2] << endl;
		vtkTransform *Transform = vtkTransform::New();
		if (myVolume->GetMatrix() != NULL)
		{
			// Actually: Vcenter already takes into account the Matrix. No need to retransform this Vcenter...
			Transform->SetMatrix(myVolume->GetMatrix());
			
	
			vtkSmartPointer<vtkPoints>  data = vtkSmartPointer<vtkPoints> ::New();
		   data->SetDataTypeToDouble();
		   data->SetNumberOfPoints(1);
			data->SetPoint(0, Vcenter);			
			vtkSmartPointer<vtkPolyData> pdata = vtkSmartPointer<vtkPolyData>::New();
			pdata->SetPoints(data);		

			vtkSmartPointer<vtkTransformFilter> transformFilter =
				vtkSmartPointer<vtkTransformFilter>::New();
			transformFilter->SetInputData(pdata);
			transformFilter->SetTransform(Transform);
			transformFilter->Update();
			vtkSmartPointer<vtkPoints> Tdata = vtkSmartPointer<vtkPoints>::New();
			Tdata = transformFilter->GetOutput()->GetPoints();
	
			//get transformed center!
			double *tcenter = Tdata->GetPoint(0);
			//cout << "acenter:" << acenter[0] << "," << acenter[1] << "," << acenter[2] << endl;

			//MultiplyPoint(const double in[4], double out[4])

			//cout << "tcenter:" << tcenter[0] << "," << tcenter[1] << "," << tcenter[2] << endl;
			//We do not do the following 3 lines any longer.
			/*Vcenter[0] = tcenter[0];
			Vcenter[1] = tcenter[1];
			Vcenter[2] = tcenter[2];*/

		}
		else
		{
			cout << "My Volume Matrix is null!" << endl;
		}
		centerOfMass[0] += Vcenter[0] ;
		centerOfMass[1] += Vcenter[0];
		centerOfMass[2] += Vcenter[2];
		if (myVolume->GetSelected() == 1)
		{
		
			centerOfMassOfSelectedVolumes[0] += Vcenter[0];
			centerOfMassOfSelectedVolumes[1] += Vcenter[1];
			centerOfMassOfSelectedVolumes[2] += Vcenter[2];
			
		}

		
	}
	if (numVol > 0)
	{
		centerOfMass[0] /= numVol;
		centerOfMass[1] /= numVol;
		centerOfMass[2] /= numVol;

	}
	if (numSelVol > 0)
	{
		centerOfMassOfSelectedVolumes[0] /= numSelVol;
		centerOfMassOfSelectedVolumes[1] /= numSelVol;
		centerOfMassOfSelectedVolumes[2] /= numSelVol;

	}
	
}

void vtkMDVolumeCollection::ComputeBoundingBoxLength()
{
	double largestbounds[6];
	largestbounds[0] = DBL_MAX;
	largestbounds[1] = -DBL_MAX;
	largestbounds[2] = DBL_MAX;
	largestbounds[3] = -DBL_MAX;
	largestbounds[4] = DBL_MAX;
	largestbounds[5] = -DBL_MAX;
	double largestboundsselected[6];
	largestboundsselected[0] = DBL_MAX;
	largestboundsselected[1] = -DBL_MAX;
	largestboundsselected[2] = DBL_MAX;
	largestboundsselected[3] = -DBL_MAX;
	largestboundsselected[4] = DBL_MAX;
	largestboundsselected[5] = -DBL_MAX;

	this->InitTraversal();
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{
		vtkVolume* Volume = this->GetNextVolume();
		
			double bounds[6];

			Volume->GetBounds(bounds);
			if (bounds[0] < largestbounds[0]) { largestbounds[0] = bounds[0]; }
			if (bounds[1] > largestbounds[1]) { largestbounds[1] = bounds[1]; }
			if (bounds[2] < largestbounds[2]) { largestbounds[2] = bounds[2]; }
			if (bounds[3] > largestbounds[3]) { largestbounds[3] = bounds[3]; }
			if (bounds[4] < largestbounds[4]) { largestbounds[4] = bounds[4]; }
			if (bounds[5] > largestbounds[5]) { largestbounds[5] = bounds[5]; }
			int selected = 0;
			std::string str1("vtkMDVolume");
			std::string str2("vtkLMVolume");
			if (str1.compare(Volume->GetClassName()) == 0)
			{
				vtkMDVolume* MDVolume = vtkMDVolume::SafeDownCast(Volume);
				if (MDVolume->GetSelected() == 1)
				{
					selected = 1;
				}
			}
			
			if (selected == 1)
			{

				if (bounds[0] < largestboundsselected[0]) { largestboundsselected[0] = bounds[0]; }
				if (bounds[1] > largestboundsselected[1]) { largestboundsselected[1] = bounds[1]; }
				if (bounds[2] < largestboundsselected[2]) { largestboundsselected[2] = bounds[2]; }
				if (bounds[3] > largestboundsselected[3]) { largestboundsselected[3] = bounds[3]; }
				if (bounds[4] < largestboundsselected[4]) { largestboundsselected[4] = bounds[4]; }
				if (bounds[5] > largestboundsselected[5]) { largestboundsselected[5] = bounds[5]; }
			}

		

	}
	double A[3];//min
	double B[3];//max

	A[0] = largestbounds[0];
	A[1] = largestbounds[2];
	A[2] = largestbounds[4];
	B[0] = largestbounds[1];
	B[1] = largestbounds[3];
	B[2] = largestbounds[5];
	this->SetBoundingBox(largestbounds);
	//cout << "A:" << A[0] << "," << A[1] << "," << A[2] << endl;
	//cout << "B:" << B[0] << "," << B[1] << "," << B[2] << endl;
	double diag[3];
	diag[0] = B[0] - A[0];
	diag[1] = B[1] - A[1];
	diag[2] = B[2] - A[2];
	double lengthxyz = sqrt((diag[0])*(diag[0]) + (diag[1])*(diag[1]) + (diag[2])*(diag[2]));

	this->SetBoundingBoxLength(lengthxyz);
	//cout << "Bounding box lenght:" << lengthxyz << endl;
	A[0] = largestboundsselected[0];
	A[1] = largestboundsselected[2];
	A[2] = largestboundsselected[4];
	B[0] = largestboundsselected[1];
	B[1] = largestboundsselected[3];
	B[2] = largestboundsselected[5];
	this->SetBoundingBoxSelected(largestboundsselected);
	//cout << "A:" << A[0] << "," << A[1] << "," << A[2] << endl;
	//cout << "B:" << B[0] << "," << B[1] << "," << B[2] << endl;	
	diag[0] = B[0] - A[0];
	diag[1] = B[1] - A[1];
	diag[2] = B[2] - A[2];
	lengthxyz = sqrt((diag[0])*(diag[0]) + (diag[1])*(diag[1]) + (diag[2])*(diag[2]));
	//cout << "Bounding box lenght selected:" << lengthxyz << endl;
	this->SetBoundingBoxLengthOfSelectedVolumes(lengthxyz);
}

void vtkMDVolumeCollection::ApplyChanges()
{
	cout << "Compute center of mass" << endl;
	this->ComputeCenterOfMass();
	cout << "that?" << endl;
	this->ComputeBoundingBoxLength();
	cout << "or?" << endl;

	// Reset state "changed" to 0 for this and all Volumes.
	this->Changed = 0;
	this->InitTraversal();
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{
		vtkVolume* Volume = this->GetNextVolume();

		//cout << "thut?" << endl;
		std::string str1("vtkMDVolume");
		if (str1.compare(Volume->GetClassName()) == 0)
		{
			vtkMDVolume *myMDVolume= vtkMDVolume::SafeDownCast(Volume);
			myMDVolume->SetChanged(0);
		}
		

	}
}


void vtkMDVolumeCollection::GetCenterOfMass(double center[3])
{
	if (this->Changed == 1 || this->VolumeChanged() == 1) { 
		
		ApplyChanges(); 
}
	double *com = this->GetCenterOfMass();

	center[0] = com[0];
	center[1] = com[1];
	center[2] = com[2];
	

}
double vtkMDVolumeCollection::GetBoundingBoxLength()
{
	if (this->Changed == 1 || this->VolumeChanged() == 1) { ApplyChanges(); }
	return this->BoundingBoxLength;


}

double vtkMDVolumeCollection::GetBoundingBoxLengthOfSelectedVolumes()
{
	if (this->Changed == 1 || this->VolumeChanged() == 1) { ApplyChanges(); }
	return this->BoundingBoxLengthOfSelectedVolumes;


}

double *vtkMDVolumeCollection::GetCenterOfMass()
{
	if (this->Changed == 1 || this->VolumeChanged() == 1) { ApplyChanges(); }
	return this->centerOfMass;
}

void vtkMDVolumeCollection::GetCenterOfMassOfSelectedVolumes(double center[3])
{
	if (this->Changed == 1 || this->VolumeChanged() == 1) { ApplyChanges(); }
	double *com = this->GetCenterOfMassOfSelectedVolumes();

	center[0] = com[0];
	center[1] = com[1];
	center[2] = com[2];


}



double *vtkMDVolumeCollection::GetCenterOfMassOfSelectedVolumes()
{
	if (this->Changed == 1 || this->VolumeChanged()==1) { ApplyChanges(); }
	return this->centerOfMassOfSelectedVolumes;
}

//----------------------------------------------------------------------------
void vtkMDVolumeCollection::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);

	//os << indent << "Selected: "<< this->Selected;
	

}
