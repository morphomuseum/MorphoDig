/*=========================================================================

Program:   MorphoDig
Module:    vtkLMActorCollection.cxx
=========================================================================*/
#include "vtkLMActorCollection.h"
#include "vtkLMActor.h"
#include "vtkMDActor.h"
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

vtkStandardNewMacro(vtkLMActorCollection);


//----------------------------------------------------------------------------
vtkLMActorCollection::vtkLMActorCollection()
{
	
}

//----------------------------------------------------------------------------
vtkLMActorCollection::~vtkLMActorCollection()
{
	

}


void vtkLMActorCollection::LandmarksMoveUp()
{
// mode = 0 : user landmark move up (more than one can be selected
	// mode = 0 is followed by a renumbering of landmark indices

	// mode = 1 : called by "Reorder_Landmarks" (only one landmark is selected): 
	// mode = 1 is not followed by a change of landmark indices...


	
	vtkLMActor *NewFirst =NULL;
	vtkLMActor *B =NULL; // the one (the one we switch with C)
	vtkLMActor *C =NULL; // the object we move up 
	
	if (this->GetNumberOfSelectedActors() > 0)
	{
		this->InitTraversal();
		NewFirst = vtkLMActor::SafeDownCast(this->GetNextActor());
		if (NewFirst != NULL)
		{
			C = this->GetLandmarkAfter(NewFirst->GetLMNumber());
			B = NewFirst;			
			// Maintenant on descend dans la liste
			while (C != NULL)
			{
				B = NULL;
				B = this->GetLandmarkBefore(C->GetLMNumber());								
				if (C->GetSelected() == 1)
				{
					if (B != NULL && B->GetSelected() == 0)
					{
						
							//A->nextobj = C;
						int new_c = B->GetLMNumber();
						int new_b = C->GetLMNumber();
						C->SetLMNumber(new_c);
						B->SetLMNumber(new_b);
						this->ReorderLandmarks();

						
					}
				}
				C = this->GetLandmarkAfter(C->GetLMNumber());
				

			}

		}

	}
	
	this->Modified();
}

void vtkLMActorCollection::LandmarksMoveDown()
{

	// mode = 0 : user landmark move up (more than one can be selected
	// mode = 0 is followed by a renumbering of landmark indices

	// mode = 1 : called by "Reorder_Landmarks" (only one landmark is selected): 
	// mode = 1 is not followed by a change of landmark indices...


	vtkLMActor *LastOBJ = NULL;
	vtkLMActor *NewFirst = NULL;
	vtkLMActor *B = NULL; // the one we move down 
	vtkLMActor *C = NULL; // the object we move up 

	if (this->GetNumberOfSelectedActors() > 0)
	{
		this->InitTraversal();
		LastOBJ = vtkLMActor::SafeDownCast(this->GetLastActor());
		
		if (LastOBJ != NULL)
		{
			B = this->GetLandmarkBefore(LastOBJ->GetLMNumber());
			C = LastOBJ;			
			// Maintenant on descend dans la liste
			while (B != NULL)
			{				
				C = this->GetLandmarkAfter(B->GetLMNumber());
				if (B->GetSelected() == 1)
				{
					if (C != NULL && C->GetSelected() == 0)
					{

						//A->nextobj = C;
						int new_c = B->GetLMNumber();
						int new_b = C->GetLMNumber();
						C->SetLMNumber(new_c);
						B->SetLMNumber(new_b);
						this->ReorderLandmarks();


					}
				}
				B = this->GetLandmarkBefore(B->GetLMNumber());


			}

		}

	}
}

void vtkLMActorCollection::AddItem(vtkActor *a)
{
	//We only want vtkLMActors in this collection!
	std::string str1("vtkLMActor");
	if (str1.compare(a->GetClassName()) == 0)
	{
		this->Superclass::AddItem(a);
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(a);
		//this->Renderer->AddActor(a);
		//this->CurrentRenderer->AddActor(myLM);
		if (mqMorphoDigCore::instance()->Getmui_DisplayLandmarkText() == 1)
		{
			this->Renderer->AddActor(myActor->GetLMLabelActor3D());
		}

	}

	//because for Bezier Curves, we are watching for a modification event!
	//this->Modified();	
}

vtkIdType vtkLMActorCollection::GetNumberOfSelectedActors()
{
	
	vtkIdType cpt = 0;
	this->InitTraversal();
	
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{
		
		vtkActor *act = this->GetNextActor();

		std::string str1("vtkLMActor");
		if (str1.compare(act->GetClassName()) == 0)
		{
		
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(act);
			if (myActor->GetSelected() == 1) {
				cpt++;
			}
		}
	}
	return cpt;


}

int vtkLMActorCollection::AreLandmarksWellOrdered()
{
	this->InitTraversal();
	if (this->GetNumberOfItems() == 0) { return 1; }
	int well_ordered = 1;
	int next = 1;
	int previous = 0;

	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->GetNextActor());
		next = myActor->GetLMNumber();
		if (next < previous) { well_ordered = 0; return well_ordered; }
		else { previous = next; }
	}
	return well_ordered;
}
void vtkLMActorCollection::ReorderLandmarks()
{
	if (this->AreLandmarksWellOrdered() == 0)
	{
		int well_ordered = 0;				
		while (well_ordered == 0)
		{
			well_ordered = 1;
			this->InitTraversal();
			int next = 1;
			int previous = 0;
			vtkLMActor *precedingLM = NULL;
			for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
			{
				vtkLMActor *nextLM = vtkLMActor::SafeDownCast(this->GetNextActor());
				next = nextLM->GetLMNumber();
				if (next < previous) 
				{ 
					if (precedingLM != NULL)
					{
						this->RemoveItem(precedingLM);
						this->AddItem(precedingLM);
					}
					well_ordered = 0; 
					break; 
				}
				else 
				{
					previous = next; precedingLM = nextLM;
				}
			}			
		}
	}

}
vtkLMActor* vtkLMActorCollection::GetLandmarkAfter(int num, int only_selected)
{
	//only_selected : 0 by default.
	//only_selected : 1 => returns next selected landmark
	if (this->AreLandmarksWellOrdered() == 0)
	{
		ReorderLandmarks();

	}
	int next;
	this->InitTraversal();
	if (this->GetNumberOfItems() == 0) { return NULL; }
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->GetNextActor());
		next = myActor->GetLMNumber();
		if (next>num  && (only_selected == 0|| myActor->GetSelected()==1))
		{
			 return myActor; 
		}				

	}
	
	return NULL;
}
vtkLMActor* vtkLMActorCollection::GetLandmarkBefore(int num, int only_selected)
{
	//cout << "Get landmark before" << num << endl;
	if (this->AreLandmarksWellOrdered() == 0)
	{
		ReorderLandmarks();

	}
	int curr = 1;	
	this->InitTraversal();
	if (this->GetNumberOfItems() == 0) { return NULL; }

	vtkLMActor *myActor = NULL;
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{
		vtkLMActor *myNextActor = vtkLMActor::SafeDownCast(this->GetNextActor());
		curr = myNextActor->GetLMNumber();

		if (curr<num && (only_selected == 0 || myActor->GetSelected() == 1))
		{
			myActor = myNextActor;
		}
		

	}
	// that way, we should get the greatest LMNumber which is still < num
	return myActor;
}
int vtkLMActorCollection::GetNextLandmarkNumber()
{
	//we assume here that landmarks are well ordered in the collection... 
	//if we have 1 2 3 4 6 7 8, we should return 5
	// if we have nothing, we should return 1
	// if we have 1 2 3 4, we should return 5
	if (this->AreLandmarksWellOrdered() == 0)
	{
		ReorderLandmarks();

	}
	int next = 1;
	int previous = 0;
	this->InitTraversal();
	if (this->GetNumberOfItems() == 0) { return 1; }
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->GetNextActor());
		next = myActor->GetLMNumber();
		if (next - previous > 1) { next = previous + 1; return next; }
		else
		{
			previous = next;			
		}
		
	}
	next++;
	return next;
}




/*void vtkLMActorCollection::ComputeBoundingBoxLength()
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
		vtkLMActor* actor = vtkLMActor::SafeDownCast(this->GetNextActor());
		if (i == 0) {
			actor->GetBounds(largestbounds);
			if (actor->GetSelected() == 1)
			{
				actor->GetBounds(largestboundsselected);
			}
		}
		else
		{
			double bounds[6];

			actor->GetBounds(bounds);
			if (bounds[0] < largestbounds[0]) { largestbounds[0] = bounds[0]; }
			if (bounds[1] > largestbounds[1]) { largestbounds[1] = bounds[1]; }
			if (bounds[2] < largestbounds[2]) { largestbounds[2] = bounds[2]; }
			if (bounds[3] > largestbounds[3]) { largestbounds[3] = bounds[3]; }
			if (bounds[4] < largestbounds[4]) { largestbounds[4] = bounds[4]; }
			if (bounds[5] > largestbounds[5]) { largestbounds[5] = bounds[5]; }
			if (actor->GetSelected() == 1)
			{

				if (bounds[0] < largestboundsselected[0]) { largestboundsselected[0] = bounds[0]; }
				if (bounds[1] > largestboundsselected[1]) { largestboundsselected[1] = bounds[1]; }
				if (bounds[2] < largestboundsselected[2]) { largestboundsselected[2] = bounds[2]; }
				if (bounds[3] > largestboundsselected[3]) { largestboundsselected[3] = bounds[3]; }
				if (bounds[4] < largestboundsselected[4]) { largestboundsselected[4] = bounds[4]; }
				if (bounds[5] > largestboundsselected[5]) { largestboundsselected[5] = bounds[5]; }
			}

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
}*/
void vtkLMActorCollection::ComputeCenterOfMass()
{
	//cout << "LM Compute center of mass" << endl;
	this->centerOfMass[0] = 0;
	this->centerOfMass[1] = 0;
	this->centerOfMass[2] = 0;
	this->centerOfMassOfSelectedActors[0] = 0;
	this->centerOfMassOfSelectedActors[1] = 0;
	this->centerOfMassOfSelectedActors[2] = 0;


	vtkIdType nLM = this->GetNumberOfItems();
	vtkIdType nSelectedLM = 0;
	vtkSmartPointer<vtkCenterOfMass> centerOfMassFilter =
		vtkSmartPointer<vtkCenterOfMass>::New();
	this->InitTraversal();
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->GetNextActor());


		double acenter[3];
		myActor->GetLMOrigin(acenter);
		
		
		centerOfMass[0] += acenter[0] ;
		centerOfMass[1] += acenter[1] ;
		centerOfMass[2] += acenter[2] ;
		if (myActor->GetSelected() == 1)
		{
		
			centerOfMassOfSelectedActors[0] += acenter[0] ;
			centerOfMassOfSelectedActors[1] += acenter[1] ;
			centerOfMassOfSelectedActors[2] += acenter[2] ;
			nSelectedLM++;
		}			
	}	
	if (nLM > 0)
	{
		centerOfMass[0] /= nLM;
		centerOfMass[1] /= nLM;
		centerOfMass[2] /= nLM;

	}
	if (nSelectedLM > 0)
	{
		centerOfMassOfSelectedActors[0] /= nSelectedLM;
		centerOfMassOfSelectedActors[1] /= nSelectedLM;
		centerOfMassOfSelectedActors[2] /= nSelectedLM;
	}
	this->SetGlobalSelectedVN(nSelectedLM);
	this->SetGlobalVN(nLM);
	//cout << "New LM Coll sCOM:" << centerOfMassOfSelectedActors[0] << "," << centerOfMassOfSelectedActors[1] << "," << centerOfMassOfSelectedActors[2] << endl;
	//cout << "New LM Coll sVn:" << nSelectedLM << endl;

}


//----------------------------------------------------------------------------
void vtkLMActorCollection::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);

	//os << indent << "Selected: "<< this->Selected;
	

}

vtkLMActor* vtkLMActorCollection::GetFirstSelectedActor()
{
	this->InitTraversal();
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{
		
			vtkActor *act = this->GetNextActor();
			if (act == NULL) { return NULL; }
			std::string str1("vtkLMActor");
			if (str1.compare(act->GetClassName()) == 0)
			{

				vtkLMActor *myActor = vtkLMActor::SafeDownCast(act);
				if (myActor->GetSelected() == 1) { return myActor; }
			}
		
	}
	

	return NULL;
}



/*void vtkLMActorCollection::ApplyChanges()
{
	
	this->ComputeCenterOfMass();	
	this->ComputeBoundingBoxLength();
	

	// Reset state "changed" to 0 for this and all actors.
	this->Changed = 0;
	this->InitTraversal();
	for (vtkIdType i = 0; i < this->GetNumberOfItems(); i++)
	{		
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(this->GetNextActor());
		myActor->SetChanged(0);
	}
}*/