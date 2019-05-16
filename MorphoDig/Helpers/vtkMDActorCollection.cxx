/*=========================================================================

Program:   MorphoDig
Module:    vtkMDActorCollection.cxx
=========================================================================*/
#include "vtkMDActorCollection.h"
#include "vtkMDActor.h"
#include "vtkLMActor.h"

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

vtkStandardNewMacro(vtkMDActorCollection);

#define CREATE_EVENT 1
#define DELETE_EVENT 0

//----------------------------------------------------------------------------
vtkMDActorCollection::vtkMDActorCollection()
{
	//this->Selected = 1;
	this->Renderer = NULL;
	this->UndoRedo = new vtkMTCollectionUndoRedo;
	this->centerOfMass[0] = 0;
	this->centerOfMass[1] = 0;
	this->centerOfMass[2] = 0;
	this->centerOfMassOfSelectedActors[0] = 0;
	this->centerOfMassOfSelectedActors[1] = 0;
	this->centerOfMassOfSelectedActors[2] = 0;
	this->SetGlobalSelectedVN(0);
	this->SetGlobalVN(0);
	this->BoundingBoxLength = 0;
	this->BoundingBoxLengthOfSelectedActors = 0;
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

vtkIdType vtkMDActorCollection::GetNumberOfSelectedActors()
{
	vtkIdType cpt = 0;
	this->InitTraversal();	
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{
		vtkActor *act = this->GetNextActor();
		std::string str1("vtkMDActor");
		if (str1.compare(act->GetClassName()) == 0)
		{
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(act);
			if ( myActor->GetSelected() == 1) {
				cpt++;
			}
		}
	}
	return cpt;


}

vtkMDActor* vtkMDActorCollection::GetActorBefore(vtkMDActor *Actor)
{
	vtkMDActor *Before = NULL;
	this->InitTraversal();
	if (this->GetNumberOfItems() == 0) { return NULL; }

	
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{
		vtkMDActor *myNextActor = vtkMDActor::SafeDownCast(this->GetNextActor());
		

		if (myNextActor ==Actor)
		{
			return Before;
		}
		Before = myNextActor;

	}
	

	return Before;
}



vtkMDActor *vtkMDActorCollection::GetActorAfter(vtkMDActor *Actor)
{
	vtkMDActor *After = NULL;
	int actor_found = 0;
	this->InitTraversal();
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{
		After= vtkMDActor::SafeDownCast(this->GetNextActor());
		if (actor_found == 1) { return After; }
		if (After ==Actor)
		{
			actor_found = 1;
		}

	}
	

	return NULL;
}

vtkMDActor* vtkMDActorCollection::GetFirstSelectedActor()
{
	this->InitTraversal();
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{

		vtkActor *act = this->GetNextActor();
		if (act == NULL) { return NULL; }
		std::string str1("vtkMDActor");
		if (str1.compare(act->GetClassName()) == 0)
		{

			vtkMDActor *myActor = vtkMDActor::SafeDownCast(act);
			if (myActor->GetSelected() == 1) { return myActor; }
		}

	}


	return NULL;
}
void vtkMDActorCollection::UpdateColorProperties(int ambient, int diffuse, int specular, double specularPower)
{

	double mAmbient = (double)ambient / 100;
	double mDiffuse = (double)diffuse / 100;
	double mSpecular = (double)specular / 100;
	this->InitTraversal();
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{

		vtkMDActor *act = vtkMDActor::SafeDownCast(this->GetNextActor());
		if (act != NULL) {
			act->SetColorProperties(mAmbient, mDiffuse, mSpecular, specularPower);			

		}

	}


}
//----------------------------------------------------------------------------
void vtkMDActorCollection::SetDisplayMode(int mode)
{
	//mode 0 : cell normals
	//mode 1 : point normals
	//mode 2 : wireframe
	//mode 3 : points
	this->InitTraversal();
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{

		vtkMDActor *act = vtkMDActor::SafeDownCast(this->GetNextActor());
		if (act != NULL) {
			act->SetDisplayMode(mode);
			
		}

	}

}

vtkMDActorCollection::~vtkMDActorCollection()
{
	
	this->UndoRedo->RedoStack.clear();
	this->UndoRedo->UndoStack.clear();
	delete this->UndoRedo;
}
void vtkMDActorCollection::AddItem(vtkActor *a)
{
	//cout << "Add item" << endl;
	this->Superclass::AddItem(a);
	//cout << "Item added" << endl;
	this->Renderer->AddActor(a);
	//cout << "Actor added to renderer" << endl;
	std::string str1("vtkMDActor");
	if (str1.compare(a->GetClassName()) == 0)
	{
		mqMorphoDigCore::instance()->Initmui_ExistingArrays();
	}

}
void vtkMDActorCollection::AddTmpItem(vtkActor *a)
{
	//cout << "Add item" << endl;
	this->Superclass::AddItem(a);
	
	

}
void vtkMDActorCollection::CreateLoadUndoSet(int count, int creationcount)
{
	cout << "CreateLoadUndoSet(" << count << "," << creationcount << ")" << endl;
	vtkSmartPointer<vtkActorCollection> NewlyCreatedActors = vtkSmartPointer<vtkActorCollection>::New();
	// we retrieve the last "creationcount" created actors.
	this->InitTraversal();
	int num_actors = this->GetNumberOfItems();
	int num_toremain = num_actors - creationcount;
	vtkActor *myActor;
	if (num_toremain >= 0)
	{
		for (vtkIdType i = 0; i < num_toremain; i++)
		{
			//these actors will remain in the renderer in case of undo
			
			myActor = this->GetNextActor();
		}	
		for (vtkIdType i = 0; i < creationcount; i++)
		{
			//these actors are put in the undo collection
			NewlyCreatedActors->AddItem(this->GetNextActor());
		}
	}
	
	
	this->UndoRedo->UndoStack.push_back(vtkMTCollectionUndoRedo::Element(NewlyCreatedActors, CREATE_EVENT, count));
	

}
void vtkMDActorCollection::ApplyProperties(vtkProperty *p)
{
	this->Superclass::ApplyProperties(p);
	
}


		



void vtkMDActorCollection::DeleteSelectedActors()
{
	int anychange = 0;
	this->InitTraversal();
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{
		vtkActor *act = this->GetNextActor();
		std::string str1("vtkMDActor");
		if (str1.compare(act->GetClassName()) == 0)
		{
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(act);
			if (myActor->GetSelected() == 1) { anychange = 1; }
		}
		std::string str2("vtkLMActor");
		if (str2.compare(act->GetClassName()) == 0)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(act);
			if (myActor->GetSelected() == 1) { anychange = 1; }
		}
		
	}
	if (anychange == 1)
	{
		vtkSmartPointer<vtkActorCollection> undocoll = vtkSmartPointer<vtkActorCollection>::New();
		std::string action = "Delete selected actors";
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
					vtkActor *act = this->GetNextActor();
					std::string str1("vtkMDActor");
					if (str1.compare(act->GetClassName()) == 0)
					{
						vtkMDActor *myActor = vtkMDActor::SafeDownCast(act);
						if (!found && myActor->GetSelected() == 1) {
							undocoll->AddItem(myActor);
							this->RemoveItem(myActor);
							this->Renderer->RemoveActor(myActor);
							if (myActor->GetBox() != NULL)
							{
								myActor->GetBox()->SetEnabled(false);
							}
							found = 1;
						}
					}
					std::string str2("vtkLMActor");
					if (str2.compare(act->GetClassName()) == 0)
					{
						vtkLMActor *myActor = vtkLMActor::SafeDownCast(act);
						if (!found && myActor->GetSelected() == 1) {
							undocoll->AddItem(myActor);
							this->RemoveItem(myActor);
							this->Renderer->RemoveActor(myActor);
							this->Renderer->RemoveActor(myActor->GetLMLabelActor3D());
							found = 1;
						}
					}

					

				}
				if (found == 0) { done = 1; }
			}
		}
		this->InitTraversal();
		// the is not a load call (-1)
		this->UndoRedo->UndoStack.push_back(vtkMTCollectionUndoRedo::Element(undocoll,DELETE_EVENT, mCount));
		END_UNDO_SET();
		this->Changed = 1;
	}
	
	mqMorphoDigCore::instance()->Initmui_ExistingArrays();

} //delete all selected actors
void vtkMDActorCollection::Redo(int mCount) {
	//cout << "Inside actor Undo, try to undo " <<mCount<< endl;
	if (this->UndoRedo->RedoStack.empty())
	{
		//cout << "Redo Stack empty!" << endl;
		return;
	}
	//cout << "Youngest redo count= " << this->UndoRedo->RedoStack.back().UndoCount<< endl;

	if (mCount == this->UndoRedo->RedoStack.back().UndoCount)
	{
		//cout << "Redo actor event " << this->UndoRedo->RedoStack.back().UndoCount << endl;
		// ici : faire l'appel global à undo de ce count là!!  
		this->PopRedoStack();
		mqMorphoDigCore::instance()->Initmui_ExistingArrays();
	}

} // Try to redo (if exists) "mCount" event
void vtkMDActorCollection::Erase(int mCount) {

	if (this->UndoRedo->UndoStack.empty())
	{
		return;
	}
	int oldestCount = this->UndoRedo->UndoStack.front().UndoCount;
	if (oldestCount <= mCount)
	{
		//cout << "ERASE actor event " << oldestCount << endl;
		this->UndoRedo->UndoStack.erase(this->UndoRedo->UndoStack.begin());
	}
} // Try to erase (if exists) "mCount" event
void vtkMDActorCollection::Undo(int mCount) 
{
	cout << "Inside MDActorCollection Undo " <<mCount<< endl;
	if (this->UndoRedo->UndoStack.empty())
	{
		return;
	}
	if (mCount == this->UndoRedo->UndoStack.back().UndoCount)
	{
		cout << "Undo actor event " << this->UndoRedo->UndoStack.back().UndoCount << endl;
		this->PopUndoStack();
		mqMorphoDigCore::instance()->Initmui_ExistingArrays();
	}

} // Try to undo (if exists) "mCount" event
void vtkMDActorCollection::PopUndoStack() {
	if (this->UndoRedo->UndoStack.empty())
	{
		return;
	}
	cout << "PopUndoStack(" << this->UndoRedo->UndoStack.back().UndoCount << "," << this->UndoRedo->UndoStack.back().EventType << ")" << endl;
	vtkSmartPointer<vtkActorCollection> ActColl = this->UndoRedo->UndoStack.back().Collection;
	
	// If stored event was a DELETE_EVENT, we need to put back deleted object in renderer + this
	if (this->UndoRedo->UndoStack.back().EventType==DELETE_EVENT)
	{
		ActColl->InitTraversal();
		for (vtkIdType i = 0; i < ActColl->GetNumberOfItems(); i++)
		{
			//vtkMDActor *myActor = vtkMDActor::SafeDownCast(ActColl->GetNextActor());
			vtkActor *myActor = ActColl->GetNextActor();
			this->AddItem(myActor);
			this->Renderer->AddActor(myActor);
			std::string str0("vtkMDActor");
			if (str0.compare(myActor->GetClassName()) == 0)
			{
				vtkMDActor *myMDActor = vtkMDActor::SafeDownCast(myActor);
				if (myMDActor->GetdisplayROI() == 1) {
					if (myMDActor->GetBox() != NULL)
					{
						myMDActor->GetBox()->SetEnabled(true);
					}
				}
			}


			
			// if myActor is a landmark => Add label to the renderer


			std::string str1("vtkLMActor");
			if (str1.compare(myActor->GetClassName()) == 0)
			{
				vtkLMActor *myLMActor;
				myLMActor = vtkLMActor::SafeDownCast(myActor);
				if (mqMorphoDigCore::instance()->Getmui_DisplayLandmarkText() == 1)
				{
					this->Renderer->AddActor(myLMActor->GetLMLabelActor3D());
				}
			}
			this->Changed = 1;
		}
		this->UndoRedo->RedoStack.push_back(vtkMTCollectionUndoRedo::Element(ActColl, DELETE_EVENT, this->UndoRedo->UndoStack.back().UndoCount));
	}
	// If stored event was a CREATE_EVENT, we need to remove last inserted objects + from this
	else
	{	
		//cout << "Try to remove last created objects" << endl;
		ActColl->InitTraversal();
		// actor(s) creation (file loading / create 1 landmark etc...)
		// this means that this action corresponds to "create a new actor (or load a bunch of new actors)".
		for (vtkIdType i = 0; i < ActColl->GetNumberOfItems(); i++)
		{
						
			
			vtkActor *myActor = ActColl->GetNextActor();			
			this->RemoveItem(myActor);
			this->Renderer->RemoveActor(myActor);
			std::string str1("vtkLMActor");
			if (str1.compare(myActor->GetClassName()) == 0)
			{
				vtkLMActor *myLMActor;
				myLMActor = vtkLMActor::SafeDownCast(myActor);
				this->Renderer->RemoveActor(myLMActor->GetLMLabelActor3D());
			}

		}
		this->UndoRedo->RedoStack.push_back(vtkMTCollectionUndoRedo::Element(ActColl, CREATE_EVENT, this->UndoRedo->UndoStack.back().UndoCount));
	}		
	this->UndoRedo->UndoStack.pop_back();
	this->Modified();
}
void vtkMDActorCollection::PopRedoStack() {
	if (this->UndoRedo->RedoStack.empty())
	{
		return;
	}
	cout << "PopRedoStack(" << this->UndoRedo->RedoStack.back().UndoCount << "," << this->UndoRedo->RedoStack.back().EventType << ")" << endl;
	vtkSmartPointer<vtkActorCollection> ActColl = this->UndoRedo->RedoStack.back().Collection;
	
	// If stored event was a DELETE_EVENT, we need to remove again deleted object in renderer + from this
	if (this->UndoRedo->RedoStack.back().EventType ==DELETE_EVENT)
	{
		ActColl->InitTraversal();
		for (vtkIdType i = 0; i < ActColl->GetNumberOfItems(); i++)
		{

			//vtkMDActor *myActor = vtkMDActor::SafeDownCast(ActColl->GetNextActor());
			vtkActor *myActor = ActColl->GetNextActor();
			this->RemoveItem(myActor);
			this->Renderer->RemoveActor(myActor);
			std::string str0("vtkMDActor");
			if (str0.compare(myActor->GetClassName()) == 0)
			{
				vtkMDActor *myMDActor = vtkMDActor::SafeDownCast(myActor);
				if (myMDActor->GetBox() != NULL)
				{
					myMDActor->GetBox()->SetEnabled(false);
				}
			}
			
			std::string str1("vtkLMActor");
			if (str1.compare(myActor->GetClassName()) == 0)
			{
				vtkLMActor *myLMActor;
				myLMActor = vtkLMActor::SafeDownCast(myActor);
				this->Renderer->RemoveActor(myLMActor->GetLMLabelActor3D());

			}
			this->Changed = 1;
		}
		this->UndoRedo->UndoStack.push_back(vtkMTCollectionUndoRedo::Element(ActColl, DELETE_EVENT, this->UndoRedo->RedoStack.back().UndoCount));
	}
	else
	{
		// If stored event was a CREATE_EVENT, we need to put again corresponding objects in renderer + inside this
		ActColl->InitTraversal();
		cout << "Try to re-add last created actor(s)" << endl;

		// actor(s) creation (file loading / create 1 landmark etc...)
		// this means that this action corresponds to "create a new actor (or load a bunch of new actors)".
		ActColl->InitTraversal();
		for (vtkIdType i = 0; i < ActColl->GetNumberOfItems(); i++)
		{

			vtkActor *myActor = ActColl->GetNextActor();
			//cout << "Try to get last actor, creation count = " << this->UndoRedo->RedoStack.back().CreationCount << "this->Number of itemps=" << this->GetNumberOfItems() << endl;

			this->AddItem(myActor);
			this->Renderer->AddActor(myActor);
			// if myActor is a landmark => Add label to the renderer
			std::string str1("vtkLMActor");
			if (str1.compare(myActor->GetClassName()) == 0)
			{
				vtkLMActor *myLMActor;
				myLMActor = vtkLMActor::SafeDownCast(myActor);
				if (mqMorphoDigCore::instance()->Getmui_DisplayLandmarkText() == 1)
				{
					this->Renderer->AddActor(myLMActor->GetLMLabelActor3D());
				}
			}
			this->Changed = 1;

		}		
		this->UndoRedo->UndoStack.push_back(vtkMTCollectionUndoRedo::Element(ActColl, CREATE_EVENT, this->UndoRedo->RedoStack.back().UndoCount));

	}

	
		
	this->UndoRedo->RedoStack.pop_back();
	this->Modified();

}
int vtkMDActorCollection::ActorChanged()
{
	int anychange = 0;
	this->InitTraversal();
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{		
		vtkActor *myActor = this->GetNextActor();
		
		std::string str1("vtkMDActor");
		if (str1.compare(myActor->GetClassName()) == 0)
		{
			vtkMDActor *myMTActor = vtkMDActor::SafeDownCast(myActor);
			if (myMTActor->GetChanged() == 1) { anychange = 1; }
		}
		std::string str2("vtkLMActor");
		if (str2.compare(myActor->GetClassName()) == 0)
		{
			vtkLMActor *myLMActor = vtkLMActor::SafeDownCast(myActor);
			if (myLMActor->GetChanged() == 1) { anychange = 1; }
		}
	}
	return anychange;
}
void vtkMDActorCollection::ComputeCenterOfMass()
{
	//cout << "MT Compute center of mass" << endl;
	this->centerOfMass[0] = 0;
	this->centerOfMass[1] = 0;
	this->centerOfMass[2] = 0;
	this->GlobalSelectedVN = 0;
	this->GlobalVN = 0;
	this->centerOfMassOfSelectedActors[0] = 0;
	this->centerOfMassOfSelectedActors[1] = 0;
	this->centerOfMassOfSelectedActors[2] = 0;
	


	vtkIdType globalvn = 0;
	vtkIdType globalSelectedvn = 0;
	vtkSmartPointer<vtkCenterOfMass> centerOfMassFilter =
		vtkSmartPointer<vtkCenterOfMass>::New();
	this->InitTraversal();
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{
		/*std::string str1("vtkMDActor");
		std::string str2("vtkMDActor");
		if (str1.compare(a->GetClassName()) == 0)
		{
		if (this->GetNextActor()->GetClassName())*/
		//
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(this->GetNextActor());

		//vtkActor *myActor = this->GetNextActor();


		centerOfMassFilter->SetInputData(myActor->GetMapper()->GetInput());
		centerOfMassFilter->SetUseScalarsAsWeights(false);
		double acenter[3];
		vtkTransform *Transform = vtkTransform::New();
		centerOfMassFilter->Update();
		centerOfMassFilter->GetCenter(acenter);
		if (myActor->GetMatrix() != NULL)
		{
			Transform->SetMatrix(myActor->GetMatrix());
			
	
			vtkSmartPointer<vtkPoints>  data = vtkSmartPointer<vtkPoints> ::New();
		   data->SetDataTypeToDouble();
		   data->SetNumberOfPoints(1);
			data->SetPoint(0, acenter);			
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
			acenter[0] = tcenter[0];
			acenter[1] = tcenter[1];
			acenter[2] = tcenter[2];

		}
		else
		{
			cout << "My Actor Matrix is null!" << endl;
		}
		vtkIdType vertnum = myActor->GetMapper()->GetInput()->GetNumberOfPoints();
		centerOfMass[0] += acenter[0] * vertnum;
		centerOfMass[1] += acenter[1] * vertnum;
		centerOfMass[2] += acenter[2] * vertnum;
		if (myActor->GetSelected() == 1)
		{
		
			centerOfMassOfSelectedActors[0] += acenter[0] * vertnum;
			centerOfMassOfSelectedActors[1] += acenter[1] * vertnum;
			centerOfMassOfSelectedActors[2] += acenter[2] * vertnum;
			globalSelectedvn += vertnum;
		}

		globalvn += vertnum;
		//cout << "MTACOLL: Total VN = " << globalvn << endl;
		//cout << "MTACOLL: acenter[] = " << acenter[0] <<"," << acenter[1] << "," << acenter[2] << endl;
	}
	//cout << "MTACOLL: Result: Total VN = " << globalvn << endl;
	if (globalvn > 0)
	{
		centerOfMass[0] /= globalvn;
		centerOfMass[1] /= globalvn;
		centerOfMass[2] /= globalvn;

	}
	if (globalSelectedvn > 0)
	{
		centerOfMassOfSelectedActors[0] /= globalSelectedvn;
		centerOfMassOfSelectedActors[1] /= globalSelectedvn;
		centerOfMassOfSelectedActors[2] /= globalSelectedvn;

	}
	this->SetGlobalSelectedVN(globalSelectedvn);
	this->SetGlobalVN(globalvn);
	//cout << "New MT Coll sCOM:" << centerOfMassOfSelectedActors[0] << "," << centerOfMassOfSelectedActors[1] << "," << centerOfMassOfSelectedActors[2] << endl;	
	//cout << "New MT Coll sVn:" << globalSelectedvn << endl;
}

void vtkMDActorCollection::ComputeBoundingBoxLength()
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
		vtkActor* actor = this->GetNextActor();
		
			double bounds[6];

			actor->GetBounds(bounds);
			if (bounds[0] < largestbounds[0]) { largestbounds[0] = bounds[0]; }
			if (bounds[1] > largestbounds[1]) { largestbounds[1] = bounds[1]; }
			if (bounds[2] < largestbounds[2]) { largestbounds[2] = bounds[2]; }
			if (bounds[3] > largestbounds[3]) { largestbounds[3] = bounds[3]; }
			if (bounds[4] < largestbounds[4]) { largestbounds[4] = bounds[4]; }
			if (bounds[5] > largestbounds[5]) { largestbounds[5] = bounds[5]; }
			int selected = 0;
			std::string str1("vtkMDActor");
			std::string str2("vtkLMActor");
			if (str1.compare(actor->GetClassName()) == 0)
			{
				vtkMDActor* MTactor = vtkMDActor::SafeDownCast(actor);
				if (MTactor->GetSelected() == 1)
				{
					selected = 1;
				}
			}
			if (str2.compare(actor->GetClassName()) == 0)
			{
				vtkLMActor* LMactor = vtkLMActor::SafeDownCast(actor);
				if (LMactor->GetSelected() == 1)
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
	this->SetBoundingBoxLengthOfSelectedActors(lengthxyz);
}

void vtkMDActorCollection::ApplyChanges()
{
	//cout << "Compute center of mass" << endl;
	this->ComputeCenterOfMass();
	//cout << "that?" << endl;
	this->ComputeBoundingBoxLength();
	//cout << "or?" << endl;

	// Reset state "changed" to 0 for this and all actors.
	this->Changed = 0;
	this->InitTraversal();
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{
		vtkActor* actor = this->GetNextActor();

		//cout << "thut?" << endl;
		std::string str1("vtkMDActor");
		std::string str2("vtkLMActor");
		if (str1.compare(actor->GetClassName()) == 0)
		{
			vtkMDActor *myMTActor = vtkMDActor::SafeDownCast(actor);
			myMTActor->SetChanged(0);
		}
		if (str2.compare(actor->GetClassName()) == 0)
		{
			vtkLMActor *myLMActor = vtkLMActor::SafeDownCast(actor);
			myLMActor->SetChanged(0);
		}

	}
}


void vtkMDActorCollection::GetCenterOfMass(double center[3])
{
	if (this->Changed == 1 || this->ActorChanged() == 1) { 
		
		ApplyChanges(); 
}
	double *com = this->GetCenterOfMass();

	center[0] = com[0];
	center[1] = com[1];
	center[2] = com[2];
	

}
double vtkMDActorCollection::GetBoundingBoxLength()
{
	if (this->Changed == 1 || this->ActorChanged() == 1) { ApplyChanges(); }
	return this->BoundingBoxLength;


}

double vtkMDActorCollection::GetBoundingBoxLengthOfSelectedActors()
{
	if (this->Changed == 1 || this->ActorChanged() == 1) { ApplyChanges(); }
	return this->BoundingBoxLengthOfSelectedActors;


}

double *vtkMDActorCollection::GetCenterOfMass()
{
	if (this->Changed == 1 || this->ActorChanged() == 1) { ApplyChanges(); }
	return this->centerOfMass;
}

void vtkMDActorCollection::GetCenterOfMassOfSelectedActors(double center[3])
{
	if (this->Changed == 1 || this->ActorChanged() == 1) { ApplyChanges(); }
	double *com = this->GetCenterOfMassOfSelectedActors();

	center[0] = com[0];
	center[1] = com[1];
	center[2] = com[2];


}



double *vtkMDActorCollection::GetCenterOfMassOfSelectedActors()
{
	if (this->Changed == 1 || this->ActorChanged()==1) { ApplyChanges(); }
	return this->centerOfMassOfSelectedActors;
}

//----------------------------------------------------------------------------
void vtkMDActorCollection::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);

	//os << indent << "Selected: "<< this->Selected;
	

}
