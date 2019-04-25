/*=========================================================================

Program:   MorphoDig
Module:    vtkMDVolume.cxx
=========================================================================*/
#include "mqMorphoDigCore.h"
#include "vtkMDVolume.h"

#include <vtkProperty.h>
#include <vtkObjectFactory.h>
#include <vtkPiecewiseFunction.h>
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
	this->Outline = vtkSmartPointer<vtkOutlineFilter>::New();
	this->OutlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	this->OutlineActor = vtkSmartPointer<vtkActor>::New();
	this->OutlineMapper->SetInputConnection(this->Outline->GetOutputPort());
	vtkSmartPointer<vtkActor> outlineActor =
		vtkSmartPointer<vtkActor>::New();
	this->OutlineActor->SetMapper(this->OutlineMapper);
	this->OutlineActor->GetProperty()->SetColor(0.5, 0.5, 0.5);
	this->Ctf = vtkSmartPointer<vtkDiscretizableColorTransferFunction>::New();
	this->ImageData = vtkSmartPointer<vtkImageData>::New();
	this->Box = vtkSmartPointer<vtkBoxWidget>::New();
	this->Changed = 0;
	this->Name = "New Volume";
	this->ScalarDisplayMax = (double)VTK_UNSIGNED_INT_MAX;
	this->ScalarDisplayMin = (double) VTK_UNSIGNED_INT_MIN;
	this->ScalarOpacityUnitDistance = 1;


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
	//cout << "Volume ambient property:" << ambient << endl;
	this->GetProperty()->SetAmbient(ambient);
	//this->GetProperty()->ShadeOn();
	//this->GetProperty()->SetInterpolationTypeToLinear();
}

void vtkMDVolume::SetScalarDisplayMax(double max)
{
	this->ScalarDisplayMax = max;
	//this->UpdateLookupTableRange();
}
void vtkMDVolume::SetScalarDisplayMin(double min)
{
	this->ScalarDisplayMin = min;
	//this->UpdateLookupTableRange();
}
void vtkMDVolume::SetScalarOpacityUnitDistance(double SOUD)
{
	this->ScalarOpacityUnitDistance = SOUD;
	this->GetProperty()->SetScalarOpacityUnitDistance(SOUD);
}
void vtkMDVolume::SetInterpolationToLinear(bool linear)
{

	if (linear)
	{
		this->GetProperty()->SetInterpolationTypeToLinear();
	}
	else
	{
		this->GetProperty()->SetInterpolationTypeToNearest();
	}

}
double vtkMDVolume::GetLookupTableMax(){
	vtkSmartPointer<vtkDiscretizableColorTransferFunction> CM = this->GetCtf();

	double *pts = CM->GetDataPointer();

	int numnodes = CM->GetSize();
	double old_max = -DBL_MAX;
	for (int j = 0; j < numnodes; j++)
	{
		double curr = pts[4 * j];
		if (curr > old_max) { old_max = curr; }

	}

	vtkPiecewiseFunction* OF = CM->GetScalarOpacityFunction();
	int numnodes2 = OF->GetSize();
	double *pts2 = OF->GetDataPointer();
	double old_max2 = -DBL_MAX;
	for (int j = 0; j < numnodes2; j++)
	{
		double curr = pts2[2 * j];
		if (curr > old_max2) { old_max2 = curr; }

	}
	if (old_max2 > old_max) { return old_max2; }
	else { return old_max; }
}
double vtkMDVolume::GetLookupTableMin()
{

	vtkSmartPointer<vtkDiscretizableColorTransferFunction> CM = this->GetCtf();

	double *pts = CM->GetDataPointer();

	int numnodes = CM->GetSize();
	double old_min = DBL_MAX;

	for (int j = 0; j < numnodes; j++)
	{
		double curr = pts[4 * j];
		//cout << "x" << j << "=" << curr << endl;
		if (curr < old_min) { old_min = curr; }

	}

	vtkPiecewiseFunction* OF = CM->GetScalarOpacityFunction();
	int numnodes2 = OF->GetSize();
	double *pts2 = OF->GetDataPointer();
	//cout << this->mui_ExistingColorMaps->Stack.at(i).Name.toStdString() << ": OF num nodes = " << numnodes2 << endl;
	double old_min2 = DBL_MAX;
	for (int j = 0; j < numnodes2; j++)
	{
		double curr = pts2[2 * j];
		//cout << "x" << j << "=" << curr << endl;
		if (curr < old_min2) { old_min2 = curr; }
	}
	if (old_min < old_min2) { return old_min2; }
	else { return old_min; }

}
void vtkMDVolume::UpdateLookupTableRange()
{
	cout << "UpdateLookupTableRange inside vtkMDVolume" << endl;
	vtkSmartPointer<vtkDiscretizableColorTransferFunction> CM = this->GetCtf();
	cout << "1" << endl;
	double *pts = CM->GetDataPointer();
	cout << "2" << endl;
	int numnodes = CM->GetSize();
	double old_min = DBL_MAX;
	double old_max = -DBL_MAX;
	for (int j = 0; j < numnodes; j++)
	{
		double curr = pts[4 * j];
		//cout << "x" << j << "=" << curr << endl;
		if (curr < old_min) { old_min = curr; }
		if (curr > old_max) { old_max = curr; }


	}
	cout << "3" << endl;
	//cout << "old max:" << old_max << ", old min:" << old_min << endl;
	if (old_max > old_min)
	{
		double old_range = old_max - old_min;
		double new_range = this->ScalarDisplayMax - this->ScalarDisplayMin;
		double mult = new_range / old_range;
		double c = this->ScalarDisplayMin - old_min * mult;
		for (int k = 0; k < numnodes; k++)
		{
			pts[4 * k] = pts[4 * k] * mult + c;
			//cout << "nx" << k << "=" << pts[4 * k] << endl;
		}
		CM->FillFromDataPointer(numnodes, pts);

	}
	cout << "4" << endl;
	vtkPiecewiseFunction* OF = CM->GetScalarOpacityFunction();
	int numnodes2 = OF->GetSize();
	cout << "5" << endl;
	double *pts2 = OF->GetDataPointer();
	cout << "6" << endl;
	//cout << this->mui_ExistingColorMaps->Stack.at(i).Name.toStdString() << ": OF num nodes = " << numnodes2 << endl;
	double old_min2 = DBL_MAX;
	double old_max2 = -DBL_MAX;
	for (int j = 0; j < numnodes2; j++)
	{
		double curr = pts2[2 * j];
		//cout << "x" << j << "=" << curr << endl;
		if (curr < old_min2) { old_min2 = curr; }
		if (curr > old_max2) { old_max2 = curr; }

	}
	cout << "7" << endl;
	if (old_max2 > old_min2)
	{
		double old_range = old_max2 - old_min2;
		double new_range = this->ScalarDisplayMax - this->ScalarDisplayMin;
		double mult = new_range / old_range;
		double c = this->ScalarDisplayMin - old_min2 * mult;
		for (int k = 0; k < numnodes2; k++)
		{
			pts2[2 * k] = pts2[2 * k] * mult + c;
			//cout << "nx" << k << "=" << pts2[2*k] << endl;
		}
		OF->FillFromDataPointer(numnodes2, pts2);

	}
	cout << "8" << endl;
}
void vtkMDVolume::UpdateLookupTableRange(double min, double max)
{
	this->ScalarDisplayMax = max;
	this->ScalarDisplayMin = min;
	this->UpdateLookupTableRange();
	

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
void vtkMDVolume::SetCtf(vtkSmartPointer<vtkDiscretizableColorTransferFunction> ctf)
{
	this->Ctf = ctf;
	this->GetProperty()->SetColor(ctf);
	this->GetProperty()->SetScalarOpacity(ctf->GetScalarOpacityFunction());

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
			mqMorphoDigCore::instance()->getRenderer()->AddActor(this->OutlineActor);
		}
		
	}
	else
	{
		//enlever la box
		mqMorphoDigCore::instance()->getRenderer()->RemoveActor(this->OutlineActor);

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
	//double *mCenter = this->GetCenter();
	//cout << "Center:" << mCenter[0] << "," << mCenter[1] << "," << mCenter[2] << "," << endl;
	vtkProp3D *prop3D = vtkProp3D::SafeDownCast(this);
	vtkProp3D *prop3D2 = vtkProp3D::SafeDownCast(this->GetOutlineActor());
	vtkTransform *newTransform = vtkTransform::New();
	newTransform->PostMultiply();
	newTransform->SetMatrix(Mat);
	prop3D->SetPosition(newTransform->GetPosition());
	prop3D->SetScale(newTransform->GetScale());
	prop3D->SetOrientation(newTransform->GetOrientation());
	prop3D2->SetPosition(newTransform->GetPosition());
	prop3D2->SetScale(newTransform->GetScale());
	prop3D2->SetOrientation(newTransform->GetOrientation());
	newTransform->Delete();
	

	//cout << "Center:" << mCenter[0] << "," << mCenter[1] << "," << mCenter[2] << "," << endl;
	this->SetChanged(1);

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
	Mat->DeepCopy(this->UndoRedo->UndoStack.back().Matrix);	
	this->ApplyMatrix(Mat);
	this->GetMatrix(Mat);
	//std::cout << "Real Matrix: " << endl << *Mat << std::endl;

	
	vtkSmartPointer<vtkDiscretizableColorTransferFunction>ctf = vtkSmartPointer<vtkDiscretizableColorTransferFunction>::New();
	ctf = this->GetCtf();
	vtkSmartPointer<vtkDiscretizableColorTransferFunction>Savedctf = vtkSmartPointer<vtkDiscretizableColorTransferFunction>::New();
	Savedctf->DeepCopy(ctf);

	ctf->DeepCopy(this->UndoRedo->UndoStack.back().Ctf);
	this->SetCtf(ctf);

	//maybe we need to Deep copy the opacity function as well????

	
	int mCurrentSelected = this->Selected;
	this->SetSelected(this->UndoRedo->UndoStack.back().Selected);
	std::string savedName = this->Name;	
	this->Name = this->UndoRedo->UndoStack.back().Name;
	//cout << "Undo name: " << this->UndoRedo->UndoStack.back().Name;
	cout << "PopUndoStack Set Selected: " << mCurrentSelected << endl;

	
	cout << "Old Min Max:" << this->ScalarDisplayMin << "," << this->ScalarDisplayMax << endl;
	double mCurrentScalarDisplayMax = this->ScalarDisplayMax;
	this->SetScalarDisplayMax(this->UndoRedo->UndoStack.back().ScalarDisplayMax);
	
	double mCurrentScalarDisplayMin = this->ScalarDisplayMin;
	this->SetScalarDisplayMin(this->UndoRedo->UndoStack.back().ScalarDisplayMin);

	cout << "New Min Max:" << this->ScalarDisplayMin << "," << this->ScalarDisplayMax << endl;

	this->UpdateLookupTableRange();
	double mCurrentScalarOpacityUnitDistance = this->ScalarOpacityUnitDistance;
	this->SetScalarOpacityUnitDistance(this->UndoRedo->UndoStack.back().ScalarOpacityUnitDistance);

	// savedName => avant, c'était this->UndoRedo->UndoStack.back().Name ???? Ok ????

	this->UndoRedo->RedoStack.push_back(vtkMDVolumeUndoRedo::Element(SavedMat, Savedctf, mCurrentSelected, this->UndoRedo->UndoStack.back().UndoCount, this->UndoRedo->UndoStack.back().Name,
		mCurrentScalarOpacityUnitDistance,
		mCurrentScalarDisplayMin,
		mCurrentScalarDisplayMax
		
		));
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
	this->ApplyMatrix(Mat);


	vtkSmartPointer<vtkDiscretizableColorTransferFunction>ctf = vtkSmartPointer<vtkDiscretizableColorTransferFunction>::New();
	ctf = this->GetCtf();
	vtkSmartPointer<vtkDiscretizableColorTransferFunction>Savedctf = vtkSmartPointer<vtkDiscretizableColorTransferFunction>::New();
	Savedctf->DeepCopy(ctf);
	ctf->DeepCopy(this->UndoRedo->RedoStack.back().Ctf);
	this->SetCtf(ctf);
	//maybe we need to Deep copy the opacity function as well????

	int mCurrentSelected = this->Selected;
	this->SetSelected(this->UndoRedo->RedoStack.back().Selected);
	cout << "PopRedoStack Set Selected: " << mCurrentSelected << endl;
	std::string savedName = this->Name;
	this->Name = this->UndoRedo->RedoStack.back().Name;	
	

	double mCurrentScalarDisplayMax = this->ScalarDisplayMax;
	cout << "Old Min Max:" << this->ScalarDisplayMin << "," << this->ScalarDisplayMax << endl;
	this->SetScalarDisplayMax(this->UndoRedo->RedoStack.back().ScalarDisplayMax);
	double mCurrentScalarDisplayMin = this->ScalarDisplayMin;
	
	this->SetScalarDisplayMin(this->UndoRedo->RedoStack.back().ScalarDisplayMin);
	this->UpdateLookupTableRange();
	cout << "New Min Max:" << this->ScalarDisplayMin << "," << this->ScalarDisplayMax << endl;
	double mCurrentScalarOpacityUnitDistance = this->ScalarOpacityUnitDistance;
	this->SetScalarOpacityUnitDistance(this->UndoRedo->RedoStack.back().ScalarOpacityUnitDistance);


	// savedName => avant, c'était this->UndoRedo->RedoStack.back().Name ???? Ok ????
	this->UndoRedo->UndoStack.push_back(vtkMDVolumeUndoRedo::Element(SavedMat, Savedctf, mCurrentSelected, this->UndoRedo->RedoStack.back().UndoCount, this->UndoRedo->RedoStack.back().Name,
		mCurrentScalarOpacityUnitDistance,
		mCurrentScalarDisplayMin,
		mCurrentScalarDisplayMax
	
	));
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
	vtkSmartPointer<vtkMatrix4x4> SavedMat = vtkSmartPointer<vtkMatrix4x4>::New();
	SavedMat->DeepCopy(Mat);

	vtkSmartPointer<vtkDiscretizableColorTransferFunction>ctf = vtkSmartPointer<vtkDiscretizableColorTransferFunction>::New();
	ctf = this->GetCtf();
	vtkSmartPointer<vtkDiscretizableColorTransferFunction>Savedctf = vtkSmartPointer<vtkDiscretizableColorTransferFunction>::New();
	Savedctf->DeepCopy(ctf);
	
	//maybe we need to Deep copy the opacity function as well????


	int mSelected = this->Selected;
	//std::cout << "Saved Matrix: " << endl << *Mat << std::endl;
	std::string name = this->Name;
	//cout << "Save name=" << name << endl;
	
	
	
	double mCurrentScalarDisplayMax = this->ScalarDisplayMax;
	

	double mCurrentScalarDisplayMin = this->ScalarDisplayMin;
	

	double mCurrentScalarOpacityUnitDistance = this->ScalarOpacityUnitDistance;
	

	
	this->UndoRedo->UndoStack.push_back(vtkMDVolumeUndoRedo::Element(SavedMat, Savedctf, mSelected, mCount, name,
		mCurrentScalarOpacityUnitDistance,
		mCurrentScalarDisplayMin,
		mCurrentScalarDisplayMax
		));

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
