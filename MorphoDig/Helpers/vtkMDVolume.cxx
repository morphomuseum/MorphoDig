/*=========================================================================

Program:   MorphoDig
Module:    vtkMDVolume.cxx
=========================================================================*/
#include "mqMorphoDigCore.h"
#include "vtkMDVolume.h"

#include <vtkObjectFactory.h>
#include <vtkBoundingBox.h>
#include <vtkMath.h>
#include <vtkDataArray.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkVolumeProperty.h>
#include <vtkPolyDataNormals.h>
#include <vtkTable.h>
#include <vtkPCAStatistics.h>
#include <vtkLine.h>
#include <vtkIdList.h>
#include <vtkTexture.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkCenterOfMass.h>
#include <vtkTransform.h>
#include <vtkPolyData.h>
#include <vtkDoubleArray.h>
#include <vtkUnsignedIntArray.h>
#include <vtkFloatArray.h>
#include <vtkIntArray.h>
#include <vtkUnsignedCharArray.h>
#include <vtkPlane.h>
#include <QString>
vtkStandardNewMacro(vtkMDVolume);



//----------------------------------------------------------------------------
vtkMDVolume::vtkMDVolume()
{
	this->UndoRedo = new vtkMDVolumeUndoRedo;
	this->Selected = 1;
	this->Changed = 0;
	this->Name = "New Volume";
}

//----------------------------------------------------------------------------
vtkMDVolume::~vtkMDVolume()
{
	this->UndoRedo->RedoStack.clear();
	this->UndoRedo->UndoStack.clear();
	delete this->UndoRedo;
	



}

void vtkMDVolume::SetColorAmbient(double ambient)
{
	this->GetProperty()->SetAmbient(ambient);
}

void vtkMDVolume::SetColorSpecular(double specular)
{
	this->GetProperty()->SetSpecular(specular);
}
void vtkMDVolume::SetColorDiffuse(double diffuse)
{
	this->GetProperty()->SetDiffuse(diffuse);
}
void  vtkMDVolume::SetColorSpecularPower(double specularPower)
{
	this->GetProperty()->SetSpecularPower(specularPower);
}
void vtkMDVolume::SetColorProperties(int ambient, int diffuse, int specular, double specularPower)
{
	double mAmbient = (double)ambient / 100;
	double mDiffuse = (double)diffuse / 100;
	double mSpecular = (double)specular / 100;	
	this->SetColorProperties(mAmbient, mDiffuse, mSpecular, specularPower);
}
void vtkMDVolume::SetColorProperties(double ambient, double diffuse, double specular, double specularPower)
{
	this->SetColorDiffuse(diffuse);
	this->SetColorSpecular(specular);
	this->SetColorSpecularPower(specularPower);
	this->SetColorAmbient(ambient);
}
int vtkMDVolume::IsInsideFrustum(vtkSmartPointer<vtkPlanes> myPlanes)
{
	cout << "Is inside frustum!" << endl;
	int is_inside = 0;
	int is_insideALL[6] = { 0,0,0,0,0,0 };
	vtkSmartPointer<vtkMatrix4x4> Mat = vtkSmartPointer<vtkMatrix4x4>::New();
	this->GetMatrix(Mat);
	vtkSmartVolumeMapper *mapper = vtkSmartVolumeMapper::SafeDownCast(this->GetMapper());
	vtkBoundingBox bbox (this->GetBounds());
	//this->GetB
	if (mapper != NULL)
	{
		

		//we have six planes
		for (vtkIdType j = 0; j < 8; j++)
		{
			is_insideALL[0] = 0;
			is_insideALL[1] = 0;
			is_insideALL[2] = 0;
			is_insideALL[3] = 0;
			is_insideALL[4] = 0;
			is_insideALL[5] = 0;
			double pt[3];
			double ptwc[3];
			double dist;
			for (vtkIdType i = 0; i < myPlanes->GetNumberOfPlanes(); i++)
			{
				
				vtkPlane *plane = myPlanes->GetPlane(i);
			
					bbox.GetCorner(j, pt);
					mqMorphoDigCore::TransformPoint(Mat, pt, ptwc);
					 dist = plane->EvaluateFunction(pt);
					 if (dist < 0) {						
						 is_insideALL[i] = 1;}
			}
			if (is_insideALL[0] == 1 && is_insideALL[1] == 1 && is_insideALL[2] == 1 && is_insideALL[3] == 1 && is_insideALL[4] == 1 && is_insideALL[5] == 1)
			{
				is_inside = 1; 
				cout << "found one inside point: " << j << ", x=" << ptwc[0] << ", y=" << ptwc[1] << ", z=" << ptwc[2] << endl;
				break;
				
			}
			
		}
	}
	
		return is_inside;
	
}

double vtkMDVolume::GetBoundingBoxLength()
{

	
		double bounds[6];

		this->GetBounds(bounds);
		double A[3];//min
		double B[3];//max

	A[0] = bounds[0];
	A[1] = bounds[2];
	A[2] = bounds[4];
	B[0] = bounds[1];
	B[1] = bounds[3];
	B[2] = bounds[5];
	
	double diag[3];
	diag[0] = B[0] - A[0];
	diag[1] = B[1] - A[1];
	diag[2] = B[2] - A[2];
	double lengthxyz = sqrt((diag[0])*(diag[0]) + (diag[1])*(diag[1]) + (diag[2])*(diag[2]));

	return lengthxyz;
}
void vtkMDVolume::SetSelected(int selected)
{
	this->Selected = selected;
	if (selected == 1)
	{
		//ici, ajouter une box visible
		if (this->GetMapper() != NULL)
		{
		
		}
		
	}
	else
	{
		//enlever la box

	}
}
void vtkMDVolume::Undo(int mCount)
{

	cout << "Inside MD Volume "<< this->GetName()<<" Undo " << mCount<<  endl;
	if (this->UndoRedo->UndoStack.empty())
	{
		return;
	}
	if (mCount == this->UndoRedo->UndoStack.back().UndoCount)
	{
		//cout << "Undo Volume event " << this->UndoRedo->UndoStack.back().UndoCount << endl;
		// ici : faire l'appel global à undo de ce count là!!  
		cout << "Call PopUndoStack from  MD Volume" << endl;
		this->PopUndoStack();
	}
	
	
	

}
void vtkMDVolume::Redo(int mCount)
{
	//cout << "Inside Volume Undo, try to undo " <<mCount<< endl;
	if (this->UndoRedo->RedoStack.empty())
	{
		cout << "Redo Stack empty!" << endl;
		return;
	}
	//cout << "Youngest redo count= " << this->UndoRedo->RedoStack.back().UndoCount<< endl;

	if (mCount == this->UndoRedo->RedoStack.back().UndoCount)
	{
		//cout << "Redo Volume event " << this->UndoRedo->RedoStack.back().UndoCount << endl;
		// ici : faire l'appel global à undo de ce count là!!  
		this->PopRedoStack();
	}
}
void vtkMDVolume::Erase(int mCount)
{
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
}

void vtkMDVolume::ApplyMatrix(vtkSmartPointer<vtkMatrix4x4> Mat)

{
	vtkProp3D *prop3D = vtkProp3D::SafeDownCast(this);
	vtkTransform *newTransform = vtkTransform::New();
	newTransform->PostMultiply();
	newTransform->SetMatrix(Mat);
	prop3D->SetPosition(newTransform->GetPosition());
	prop3D->SetScale(newTransform->GetScale());
	prop3D->SetOrientation(newTransform->GetOrientation());
	newTransform->Delete();

}
void vtkMDVolume::GetMCenter(double center[3])
{
	double *mCenter = this->GetCenter();
	center[0] = mCenter[0];
	center[1] = mCenter[1];
	center[2] = mCenter[2];

}

void vtkMDVolume::PopUndoStack()
{
	cout << "Inside MD Volume "<< this->GetName()<<" PopUndoStack" << endl;
	if (this->UndoRedo->UndoStack.empty())
	{
		return;
	}
		
	vtkSmartPointer<vtkMatrix4x4> Mat = vtkSmartPointer<vtkMatrix4x4>::New();
	this->GetMatrix(Mat);	
	vtkSmartPointer<vtkMatrix4x4> SavedMat = vtkSmartPointer<vtkMatrix4x4>::New();
	SavedMat->DeepCopy(Mat);
	// Now put undo Matrix inside object : 
	vtkSmartPointer<vtkDataArray> toRestoreArray = nullptr;
	//auto toRestoreArray = vtkSmartPointer<vtkDataArray>::New();	
	vtkDataArray *toSaveArray;
	toSaveArray = NULL;
	vtkSmartPointer<vtkDataArray> savedArray = nullptr;
	//savedArray = NULL;
	
	




	Mat->DeepCopy(this->UndoRedo->UndoStack.back().Matrix);
	
	this->ApplyMatrix(Mat);


	this->GetMatrix(Mat);
	//std::cout << "Real Matrix: " << endl << *Mat << std::endl;

	
	
	int mCurrentSelected = this->Selected;
	this->SetSelected(this->UndoRedo->UndoStack.back().Selected);
	this->Name = this->UndoRedo->UndoStack.back().Name;
	//cout << "Undo name: " << this->UndoRedo->UndoStack.back().Name;
	cout << "PopUndoStack Set Selected: " << mCurrentSelected << endl;
	this->UndoRedo->RedoStack.push_back(vtkMDVolumeUndoRedo::Element(SavedMat, mCurrentSelected, this->UndoRedo->UndoStack.back().UndoCount, this->UndoRedo->UndoStack.back().Name));
	this->UndoRedo->UndoStack.pop_back();
	this->Modified();
}
void vtkMDVolume::PopRedoStack()
{
	if (this->UndoRedo->RedoStack.empty())
	{
		return;
	}
	vtkSmartPointer<vtkMatrix4x4> Mat = vtkSmartPointer<vtkMatrix4x4>::New();
	this->GetMatrix(Mat);
	vtkSmartPointer<vtkMatrix4x4> SavedMat = vtkSmartPointer<vtkMatrix4x4>::New();
	SavedMat->DeepCopy(Mat);

	
	

	

	// Now put redp Matrix inside object : 
	Mat->DeepCopy(this->UndoRedo->RedoStack.back().Matrix);
	/*vtkProp3D *prop3D = vtkProp3D::SafeDownCast(this);
	vtkTransform *newTransform = vtkTransform::New();
	newTransform->PostMultiply();
	newTransform->SetMatrix(Mat);
	prop3D->SetPosition(newTransform->GetPosition());
	prop3D->SetScale(newTransform->GetScale());
	prop3D->SetOrientation(newTransform->GetOrientation());
	newTransform->Delete();*/
	this->ApplyMatrix(Mat);
	
	int mCurrentSelected = this->Selected;
	this->SetSelected(this->UndoRedo->RedoStack.back().Selected);
	cout << "PopRedoStack Set Selected: " << mCurrentSelected << endl;
	this->Name = this->UndoRedo->RedoStack.back().Name;	
	this->UndoRedo->UndoStack.push_back(vtkMDVolumeUndoRedo::Element(SavedMat, mCurrentSelected, this->UndoRedo->RedoStack.back().UndoCount, this->UndoRedo->RedoStack.back().Name));
	this->UndoRedo->RedoStack.pop_back();
	this->Modified();
}

void vtkMDVolume::SaveState(int mCount)
{
	//arrayType: 0 "scalar"
	// 1 : "tag"
	// 2 : "rgb"
	//cout << "myVolume Save Position: redostack clear." << endl;
	this->UndoRedo->RedoStack.clear();

	//int Count = 2;//MorphoDig::instance()->GetUndoCount()+1;
	int Count = mCount;
	
	// détruit ce qui est trop vieux dans le vector!
	//to do!
	vtkSmartPointer<vtkMatrix4x4> Mat = vtkSmartPointer<vtkMatrix4x4>::New();
	this->GetMatrix(Mat);
	int mSelected = this->Selected;
	//std::cout << "Saved Matrix: " << endl << *Mat << std::endl;
	std::string name = this->Name;
	//cout << "Save name=" << name << endl;
	vtkSmartPointer<vtkMatrix4x4> SavedMat = vtkSmartPointer<vtkMatrix4x4>::New();
	SavedMat->DeepCopy(Mat);

	
	


	
	this->UndoRedo->UndoStack.push_back(vtkMDVolumeUndoRedo::Element(SavedMat, mSelected, mCount, name));

}
//----------------------------------------------------------------------------
// Shallow copy of an Volume.
void vtkMDVolume::ShallowCopy(vtkProp *prop)
{
	vtkMDVolume *f = vtkMDVolume::SafeDownCast(prop);
	

	// Now do superclass
	this->vtkVolume::ShallowCopy(prop);
}


//----------------------------------------------------------------------------
void vtkMDVolume::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);

	os << indent << "Selected: "<< this->Selected;
	

}
// Actual Volume render method.
