/*=========================================================================

Program:   MorphoDig
Module:    vtkMDVolume.cxx
=========================================================================*/
#include "mqMorphoDigCore.h"
#include "vtkMDVolume.h"

#include <vtkProperty.h>
#include <vtkObjectFactory.h>
#include <vtkImageFlip.h>
#include <vtkImagePermute.h>
#include <vtkPiecewiseFunction.h>
#include <vtkBoundingBox.h>
#include <vtkMath.h>
#include <vtkDataArray.h>
#include <vtkSmartPointer.h>
#include <vtkImageResample.h>
#include <vtkRenderer.h>
#include <vtkVolumeProperty.h>
#include <vtkImageProperty.h>
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
#include <QMessageBox>
vtkStandardNewMacro(vtkMDVolume);



//----------------------------------------------------------------------------
vtkMDVolume::vtkMDVolume()
{
	this->UndoRedo = new vtkMDVolumeUndoRedo;
	this->Selected = 1;
	
	this->displayROI = 0;
	this->enableROI = 0;
	this->isVisible = 1;
	this->isVisibleXY = 0;
	this->isVisibleXZ = 0;
	this->isVisibleYZ = 0;
	this->myDim[0] = 0;
	this->myDim[1] = 0;
	this->myDim[2] = 0;
	/*this->SliceXY->GetProperty()->SetColorWindow(range[1] - range[0]);
    this->SliceXY->GetProperty()->SetColorLevel(0.5*(range[0] + range[1]));
    this->SliceXY->GetProperty()->SetInterpolationTypeToNearest();

    vtkSmartPointer<vtkRenderer> renderer =
      vtkSmartPointer<vtkRenderer>::New();
renderer->AddViewProp(image);*/
	this->Outline = vtkSmartPointer<vtkOutlineFilter>::New();
	this->OutlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	this->OutlineActor = vtkSmartPointer<vtkActor>::New();
	this->OutlineMapper->SetInputConnection(this->Outline->GetOutputPort());
	vtkSmartPointer<vtkActor> outlineActor =
		vtkSmartPointer<vtkActor>::New();
	this->OutlineActor->SetMapper(this->OutlineMapper);
	this->OutlineActor->GetProperty()->SetColor(0.5, 0.5, 0.5);
	this->Ctf = vtkSmartPointer<vtkDiscretizableColorTransferFunction>::New();
	this->Hist = vtkSmartPointer<vtkImageAccumulate>::New();
	this->ImageDataBinComputed = 0;
	this->useImageDataBinForVR = 0;
	this->ImageData = vtkSmartPointer<vtkImageData>::New();
	this->ImageDataBin = vtkSmartPointer<vtkImageData>::New();
	this->SliceXY = vtkSmartPointer<vtkImageSlice>::New();
	this->SliceXZ = vtkSmartPointer<vtkImageSlice>::New();
	this->SliceYZ = vtkSmartPointer<vtkImageSlice>::New();
	this->SliceXY2 = vtkSmartPointer<vtkImageSlice>::New();
	this->SliceXZ2 = vtkSmartPointer<vtkImageSlice>::New();
	this->SliceYZ2 = vtkSmartPointer<vtkImageSlice>::New();
	this->SliceXY->GetProperty()->SetInterpolationTypeToNearest();
	this->SliceXZ->GetProperty()->SetInterpolationTypeToNearest();
	this->SliceYZ->GetProperty()->SetInterpolationTypeToNearest();
	this->SliceXYMapper = vtkSmartPointer<vtkImageResliceMapper>::New();
	this->SliceXYMapper2 = vtkSmartPointer<vtkImageSliceMapper>::New();
	this->SliceYZMapper = vtkSmartPointer<vtkImageResliceMapper>::New();
	this->SliceYZMapper2 = vtkSmartPointer<vtkImageSliceMapper>::New();
	this->SliceXZMapper = vtkSmartPointer<vtkImageResliceMapper>::New();
	this->SliceXZMapper2 = vtkSmartPointer<vtkImageSliceMapper>::New();
	this->SliceXYMapper->SliceFacesCameraOff();
	this->SliceXZMapper->SliceFacesCameraOff();
	this->SliceYZMapper->SliceFacesCameraOff();

	this->SliceXYMapper2->SliceFacesCameraOff();
	this->SliceXYMapper2->SetOrientationToZ();
	this->SliceXZMapper2->SliceFacesCameraOff();
	this->SliceXZMapper2->SetOrientationToY();
	this->SliceYZMapper2->SliceFacesCameraOff();
	this->SliceYZMapper2->SetOrientationToX();
	this->SliceXYMapper->ResampleToScreenPixelsOff();
	/*this->SliceXYMapper2->SetSliceNumber(10);
	this->SliceXZMapper2->SetSliceNumber(10);
	this->SliceYZMapper2->SetSliceNumber(10);*/

	SliceXY->SetMapper(this->SliceXYMapper);
	SliceYZ->SetMapper(this->SliceYZMapper);
	SliceXZ->SetMapper(this->SliceXZMapper);
	SliceXY2->SetMapper(this->SliceXYMapper2);
	SliceYZ2->SetMapper(this->SliceYZMapper2);
	SliceXZ2->SetMapper(this->SliceXZMapper2);
	
	//this->Box = NULL;
	this->Box = vtkSmartPointer<vtkBoxWidget>::New();
	this->Changed = 0;
	this->Name = "New Volume";
	this->ScalarDisplayMax = (double)VTK_UNSIGNED_INT_MAX;
	this->ScalarDisplayMin = (double) VTK_UNSIGNED_INT_MIN;
	this->ScalarOpacityUnitDistance = 1;
	this->init_center[0] = 0;
	this->init_center[1] = 0;
	this->init_center[2] = 0;

}

//----------------------------------------------------------------------------
vtkMDVolume::~vtkMDVolume()
{
	this->UndoRedo->RedoStack.clear();
	this->UndoRedo->UndoStack.clear();
	delete this->UndoRedo;
	



}


void vtkMDVolume::SetisVisible(int visible)
{
	
	cout << "is visible:" << visible << endl;
	int hasChanged = 0;
	if (visible != this->isVisible) { hasChanged = 1; }
	if (hasChanged = 0) { return; }
	this->isVisible = visible;
	if (visible == 1)
	{
		//ici, ajouter une box visible
		if (this->GetMapper() != NULL)
		{						

			if (this->displayROI == 1 && this->Box != NULL) { this->Box->SetEnabled(true); }
			if (this->isVisibleVR== 1 ) { mqMorphoDigCore::instance()->getRenderer()->AddVolume(this);  }
			if (this->isVisibleXY == 1) { mqMorphoDigCore::instance()->getRenderer()->AddViewProp(this->SliceXY2); }
			if (this->isVisibleXZ == 1) { mqMorphoDigCore::instance()->getRenderer()->AddViewProp(this->SliceXZ2); }
			if (this->isVisibleYZ == 1) { mqMorphoDigCore::instance()->getRenderer()->AddViewProp(this->SliceYZ2); }
		}
	}
	else
	{		
		if (this->Box != NULL) {
			this->Box->SetEnabled(false);
		}
		mqMorphoDigCore::instance()->getRenderer()->RemoveVolume(this);
		mqMorphoDigCore::instance()->getRenderer()->RemoveViewProp(this->SliceXY2);
		mqMorphoDigCore::instance()->getRenderer()->RemoveViewProp(this->SliceXZ2);
		mqMorphoDigCore::instance()->getRenderer()->RemoveViewProp(this->SliceYZ2);
	}

}

void vtkMDVolume::SetisVisibleVR(int visible)
{
	cout << "is visible VR:" << visible << endl;
	int hasChanged = 0;
	if (visible != this->isVisibleVR) { hasChanged = 1; }
	if (hasChanged = 0) { return; }
	this->isVisibleVR = visible;
	if (visible == 1)
	{		
		if (this->GetMapper() != NULL)
		{
			mqMorphoDigCore::instance()->getRenderer()->AddVolume(this);			
			if (this->displayROI == 1 && this->Box != NULL) { this->Box->SetEnabled(true); }
		}
	}
	else
	{		
		if (this->Box != NULL) {
			this->Box->SetEnabled(false);
		}
		mqMorphoDigCore::instance()->getRenderer()->RemoveVolume(this);

	}

}

void vtkMDVolume::SetisVisibleXY(int visible)
{
	int hasChanged = 0;
	if (visible != this->isVisible) { hasChanged = 1; }
	if (hasChanged = 0) { return; }
	this->isVisibleXY = visible;
	if (visible == 1)
	{
		
		if (this->GetMapper() != NULL)
		{
			mqMorphoDigCore::instance()->getRenderer()->AddViewProp(this->SliceXY2);			
		}
	}
	else
	{
		
		mqMorphoDigCore::instance()->getRenderer()->RemoveViewProp(this->SliceXY2);				

	}
}
void vtkMDVolume::SetisVisibleXZ(int visible)
{
	int hasChanged = 0;
	if (visible != this->isVisible) { hasChanged = 1; }
	if (hasChanged = 0) { return; }
	this->isVisibleXZ = visible;
	if (visible == 1)
	{

		if (this->GetMapper() != NULL)
		{
			mqMorphoDigCore::instance()->getRenderer()->AddViewProp(this->SliceXZ2);
		}
	}
	else
	{

		mqMorphoDigCore::instance()->getRenderer()->RemoveViewProp(this->SliceXZ2);

	}
}
void vtkMDVolume::SetisVisibleYZ(int visible)
{
	int hasChanged = 0;
	if (visible != this->isVisible) { hasChanged = 1; }
	if (hasChanged = 0) { return; }
	this->isVisibleYZ = visible;
	if (visible == 1)
	{

		if (this->GetMapper() != NULL)
		{
			mqMorphoDigCore::instance()->getRenderer()->AddViewProp(this->SliceYZ2);
		}
	}
	else
	{

		mqMorphoDigCore::instance()->getRenderer()->RemoveViewProp(this->SliceYZ2);

	}
}
double vtkMDVolume::GetRangeMin()
{
	return this->GetImageData()->GetScalarRange()[0];
}

double vtkMDVolume::GetRangeMax()
{
	return this->GetImageData()->GetScalarRange()[1];
}
void vtkMDVolume::SetenableROI(int enable)
{
	if (this->Box != NULL)
	{
		if (enable == 0) {

			this->enableROI = 0;
			this->RemoveBox();
		}
		else
		{
			this->enableROI = 1;
			this->CreateBox();
		}
	}
}
void vtkMDVolume::SetdisplayROI(int disp)
{
	if (this->Box != NULL)
	{
		if (disp == 0) {

			this->displayROI = 0;
			this->Box->SetEnabled(false);
		}
		else
		{
			this->displayROI = 1;
			this->Box->SetEnabled(true);
		}
	}
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
	this->UpdateSlices();
	
	//this->UpdateLookupTableRange();
}
void vtkMDVolume::UpdateSlices()
{
	this->SliceXY->GetProperty()->SetColorWindow(this->ScalarDisplayMax - this->ScalarDisplayMin);
	this->SliceXY->GetProperty()->SetColorLevel(0.5*(this->ScalarDisplayMin + this->ScalarDisplayMax));
	this->SliceXZ->GetProperty()->SetColorWindow(this->ScalarDisplayMax - this->ScalarDisplayMin);
	this->SliceXZ->GetProperty()->SetColorLevel(0.5*(this->ScalarDisplayMin + this->ScalarDisplayMax));
	this->SliceYZ->GetProperty()->SetColorWindow(this->ScalarDisplayMax - this->ScalarDisplayMin);
	this->SliceYZ->GetProperty()->SetColorLevel(0.5*(this->ScalarDisplayMin + this->ScalarDisplayMax));


	this->SliceXY2->GetProperty()->SetColorWindow(this->ScalarDisplayMax - this->ScalarDisplayMin);
	this->SliceXY2->GetProperty()->SetColorLevel(0.5*(this->ScalarDisplayMin + this->ScalarDisplayMax));
	this->SliceXZ2->GetProperty()->SetColorWindow(this->ScalarDisplayMax - this->ScalarDisplayMin);
	this->SliceXZ2->GetProperty()->SetColorLevel(0.5*(this->ScalarDisplayMin + this->ScalarDisplayMax));
	this->SliceYZ2->GetProperty()->SetColorWindow(this->ScalarDisplayMax - this->ScalarDisplayMin);
	this->SliceYZ2->GetProperty()->SetColorLevel(0.5*(this->ScalarDisplayMin + this->ScalarDisplayMax));

	
}
void vtkMDVolume::SetScalarDisplayMin(double min)
{
	this->ScalarDisplayMin = min;
	this->UpdateSlices();
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
	//cout << "1" << endl;
	double *pts = CM->GetDataPointer();
	//cout << "2" << endl;
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
	//cout << "3" << endl;
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
	//cout << "4" << endl;
	vtkPiecewiseFunction* OF = CM->GetScalarOpacityFunction();
	int numnodes2 = OF->GetSize();
	//cout << "5" << endl;
	double *pts2 = OF->GetDataPointer();
	//cout << "6" << endl;
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
	//cout << "7" << endl;
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
	//cout << "8" << endl;
	cout << "UpdateLookupTableRange inside vtkMDVolume done" << endl;
}
void vtkMDVolume::UpdateLookupTableRange(double min, double max)
{
	this->ScalarDisplayMax = max;
	this->ScalarDisplayMin = min;
	this->UpdateSlices();
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
	//NOT SURE!
	/*this->SliceXY->GetProperty()->SetLookupTable(ctf);
	this->SliceXZ->GetProperty()->SetLookupTable(ctf);
	this->SliceYZ->GetProperty()->SetLookupTable(ctf);*/

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
	//on ne selectionne ici que les points!!!!
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
	cout << "Volume selected:" << selected << endl;
	this->Selected = selected;
	if (selected == 1)
	{
		//ici, ajouter une box visible
		if (this->GetMapper() != NULL)
		{
			mqMorphoDigCore::instance()->getRenderer()->AddActor(this->OutlineActor);
			if (this->displayROI == 1 && this->Box!= NULL) { this->Box->SetEnabled(true); }
		}
	}
	else
	{
		//enlever la box
		mqMorphoDigCore::instance()->getRenderer()->RemoveActor(this->OutlineActor);
		if (this->Box != NULL) {
			this->Box->SetEnabled(false);
		}
	}
}
void vtkMDVolume::GetBoxCenter(double boxCenter[3])
{
	vtkTransform *t = vtkTransform::New();

	this->Box->GetTransform(t);
	vtkSmartPointer<vtkMatrix4x4> Mat = t->GetMatrix();
	double cx = Mat->GetElement(0, 3);
	double cy = Mat->GetElement(1, 3);
	double cz = Mat->GetElement(2, 3);
	boxCenter[0] = cx;
	boxCenter[1] = cy;
	boxCenter[2] = cz;
}
void vtkMDVolume::GetBoxBounds(double boxBounds[6])
{
	double xmin, xmax, ymin, ymax, zmin, zmax;
	xmin = DBL_MAX;
	ymin = DBL_MAX;
	zmin = DBL_MAX;
	xmax = -DBL_MAX;
	ymax = -DBL_MAX;
	zmax = -DBL_MAX;
	double mBoxCenter[3] = { 0,0,0 };
	double mBoxDiff[3] = { 0,0,0 };
	this->GetBoxCenter(mBoxCenter);
	vtkPlanes *p = vtkPlanes::New();
	this->Box->GetPlanes(p);
	for (vtkIdType i = 0; i < p->GetNumberOfPlanes(); i++)
	{

		vtkPlane *plane = p->GetPlane(i);
		double origin[3] = { 0,0,0 };
		plane->GetOrigin(origin);
		mBoxDiff[0] = origin[0] - mBoxCenter[0];
		mBoxDiff[1] = origin[1] - mBoxCenter[1];
		mBoxDiff[2] = origin[2] - mBoxCenter[2];
		if (origin[0] < xmin) { xmin = origin[0]; }
		if (origin[0] > xmax) { xmax = origin[0]; }
		if (origin[1] < ymin) { ymin = origin[1]; }
		if (origin[1] > ymax) { ymax = origin[1]; }
		if (origin[2] < zmin) { zmin = origin[2]; }
		if (origin[2] > zmax) { zmax = origin[2]; }
		cout << "Plane " << i << ":" << origin[0] << "," << origin[1] << "," << origin[2] << endl;
		//cout << "Plane " << i << ":" << mBoxDiff[0] << "," << mBoxDiff[1] << "," << mBoxDiff[2] << endl;
	}
	boxBounds[0] = xmin;
	boxBounds[1] = xmax;
	boxBounds[2] = ymin;
	boxBounds[3] = ymax;
	boxBounds[4] = zmin;
	boxBounds[5] = zmax;
}
void vtkMDVolume::ComputeImageDataBin()
{
	vtkSmartPointer<vtkImageResample> resample = vtkSmartPointer<vtkImageResample>::New();
	resample->SetInputData(this->ImageData);
	double magnification = 0.5;
	long long int numVox = (long long int ) (this->myDim[0] * this->myDim[1] * this->myDim[2]/8);
	if (numVox > mqMorphoDigCore::instance()->Getmui_VolumeOutOfCoreThreshold())
	{
		int num_bin_needed = 2;// at least 2!
		int ratio = (int) (numVox / mqMorphoDigCore::instance()->Getmui_VolumeOutOfCoreThreshold());
		if (ratio > 4096)
		{
			num_bin_needed = 6;// could it happen????
		}
		else if (ratio > 512)
		{
			num_bin_needed = 5;
		}
		else if (ratio > 64)
		{
			num_bin_needed = 4;
		}
		else if (ratio > 8)
		{
			num_bin_needed = 3;
		}
		else
		{
			num_bin_needed = 2;
		}
		magnification = 1;
		for (int i = 0; i < num_bin_needed; i++)
		{
			magnification /= 2;
		}
		
	}
	cout << "binning magnification:" << endl;
	resample->SetAutoCropOutput(true);
	resample->SetAxisMagnificationFactor(0, magnification);
	resample->SetAxisMagnificationFactor(1, magnification);
	resample->SetAxisMagnificationFactor(2, magnification);
	resample->Update();
	this->ImageDataBin = resample->GetOutput();

	this->ImageDataBinComputed = 1;
}

// used to modify the input!!!

void vtkMDVolume::SwapXY()
{
	vtkSmartPointer<vtkImagePermute> permute = vtkSmartPointer<vtkImagePermute>::New();
	permute->SetInputData(this->ImageData);
	permute->SetFilteredAxes(1, 0, 2);
	permute->Update();
	vtkSmartPointer<vtkImageData> permutedData = permute->GetOutput();
	this->SetImageDataAndMap(permutedData);
}
void vtkMDVolume::SwapXZ()
{
	vtkSmartPointer<vtkImagePermute> permute = vtkSmartPointer<vtkImagePermute>::New();
	permute->SetInputData(this->ImageData);
	permute->SetFilteredAxes(2, 1, 0);
	permute->Update();
	vtkSmartPointer<vtkImageData> permutedData = permute->GetOutput();
	this->SetImageDataAndMap(permutedData);

}
void vtkMDVolume::SwapYZ()
{
	vtkSmartPointer<vtkImagePermute> permute = vtkSmartPointer<vtkImagePermute>::New();
	permute->SetInputData(this->ImageData);
	permute->SetFilteredAxes(0, 2, 1);
	permute->Update();
	vtkSmartPointer<vtkImageData> permutedData = permute->GetOutput();
	this->SetImageDataAndMap(permutedData);
}
void vtkMDVolume::SwapXYZ()
{
	vtkSmartPointer<vtkImagePermute> permute = vtkSmartPointer<vtkImagePermute>::New();
	permute->SetInputData(this->ImageData);
	permute->SetFilteredAxes(1, 2, 0);
	permute->Update();
	vtkSmartPointer<vtkImageData> permutedData = permute->GetOutput();
	this->SetImageDataAndMap(permutedData);
}
void vtkMDVolume::FlipZ()
{
	vtkSmartPointer<vtkImageFlip> flip = vtkSmartPointer<vtkImageFlip>::New();
	flip->SetInputData(this->ImageData);
	flip->SetFilteredAxis(2);
	flip->Update();
	vtkSmartPointer<vtkImageData> flippedData = flip->GetOutput();
	this->SetImageDataAndMap(flippedData);
}
void vtkMDVolume::FlipX()
{
	cout << "flip X!" << endl;
	vtkSmartPointer<vtkImageFlip> flip = vtkSmartPointer<vtkImageFlip>::New();
	flip->SetInputData(this->ImageData);
	flip->SetFilteredAxis(0);
	flip->Update();
	vtkSmartPointer<vtkImageData> flippedData = flip->GetOutput();
	this->SetImageDataAndMap(flippedData);
}
void vtkMDVolume::FlipY()
{
	vtkSmartPointer<vtkImageFlip> flip = vtkSmartPointer<vtkImageFlip>::New();
	flip->SetInputData(this->ImageData);
	flip->SetFilteredAxis(1);
	flip->Update();
	vtkSmartPointer<vtkImageData> flippedData = flip->GetOutput();
	this->SetImageDataAndMap(flippedData);
}

void vtkMDVolume::ChangeSpacing(double newSpacingX, double newSpacingY, double newSpacingZ)
{
	this->ImageData->SetSpacing(newSpacingX, newSpacingY, newSpacingZ);
	this->SetImageDataAndMap(this->ImageData);
}

void vtkMDVolume::Resample(double newSpacingX, double newSpacingY, double newSpacingZ)
{
	//to implement
}
void vtkMDVolume::SetImageDataAndMap(vtkSmartPointer<vtkImageData> imgData)
{
	this->SetImageData(imgData);
	vtkSmartPointer<vtkSmartVolumeMapper> mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
	this->SetMapper(mapper);
	if (this->useImageDataBinForVR == 1)
	{
		
		mapper->SetInputData(this->GetImageDataBin());
	}
	else
	{
		mapper->SetInputData(this->GetImageData());
	}
}
void vtkMDVolume::SetImageData(vtkSmartPointer<vtkImageData> imgData)
{
	this->ImageData = imgData;
	this->ImageDataBinComputed = 0;
	// now add a few things related to SliceXYMapper etc... 
	
	this->SliceXYMapper->SetInputData(imgData);
	this->SliceXZMapper->SetInputData(imgData);
	this->SliceYZMapper->SetInputData(imgData);
	this->SliceXYMapper2->SetInputData(imgData);
	this->SliceXZMapper2->SetInputData(imgData);
	this->SliceYZMapper2->SetInputData(imgData);

	
	
	imgData->GetDimensions(this->myDim);

	this->SliceXYMapper2->SetSliceNumber((int)(this->myDim[2]/2));
	this->SliceXZMapper2->SetSliceNumber((int)(this->myDim[1] / 2));
	this->SliceYZMapper2->SetSliceNumber((int)(this->myDim[0] / 2));

}
void vtkMDVolume::SetDesiredMappedImageData()
{
	long long int numVox = this->myDim[0] * this->myDim[1]*this->myDim[2];
	if (numVox > mqMorphoDigCore::instance()->Getmui_VolumeOutOfCoreThreshold())
	{
		//
		QMessageBox msgBox;
		msgBox.setText("Number of voxels > Out of core threshold value (adjustable in Edit->Volume options). Volume rendering will be achieved on a subsampled version.");
		msgBox.exec();
		//vtkSmartVolumeMapper::SafeDownCast(this->GetMapper())->SetInputData(this->GetImageDataBin());
		this->SetuseImageDataBinForVR(1);
	}
	else
	{

		//vtkSmartVolumeMapper::SafeDownCast(this->GetMapper())->SetInputData(this->ImageData);
		this->SetuseImageDataBinForVR(0);
	}
	//
	//
}
int vtkMDVolume::SetuseImageDataBinForVR(int use)
{
	int changed = 1;
	/*if (use != this->useImageDataBinForVR)
	{
		changed = 1;
	}*/
	if (changed == 1)
	{
		long long int numVox = this->myDim[0] * this->myDim[1] * this->myDim[2];
		if (use == 1)
		{
			cout << "change input data binned image" << endl;
			this->useImageDataBinForVR = 1;
			vtkSmartPointer<vtkSmartVolumeMapper> mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
			this->SetMapper(mapper);
			mapper->SetInputData(this->GetImageDataBin());
			
		}
		else
		{
			if (numVox > mqMorphoDigCore::instance()->Getmui_VolumeOutOfCoreThreshold())
			{
				QMessageBox msgBox;
				msgBox.setText("Number of voxels > Out of core threshold value (adjustable in Edit->Volume options). MorphoDig may crash if your graphic ressources are too low. Continue ?");
				msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
				msgBox.setDefaultButton(QMessageBox::No);
				int ret = msgBox.exec();
				if (ret == QMessageBox::No) { 
					cout << "no!!!" << endl;
					return 0; }
			}
			cout << "change input data normal image!" << endl;
			vtkSmartPointer<vtkSmartVolumeMapper> mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
			this->SetMapper(mapper);
			mapper->SetInputData(this->GetImageData());
			
			this->useImageDataBinForVR = 0;
		}
	}
	return 1;

}
vtkSmartPointer<vtkImageData> vtkMDVolume::GetImageDataBin()
{
	if (this->ImageDataBinComputed ==0)
	{
		cout << "ComputeImageDataBin" << endl;
		this->ComputeImageDataBin();
	}
	return this->ImageDataBin;
}
void vtkMDVolume::SetImageDataBin(vtkSmartPointer<vtkImageData> imgDataBin)
{
	this->ImageDataBin = imgDataBin;
	

}

void vtkMDVolume::SetSliceNumberXY(int slice)
{
	if (slice >= 0 && slice <= this->myDim[2])
	{
		this->SliceXYMapper2->SetSliceNumber(slice);
	}
}
void vtkMDVolume::SetSliceNumberXZ(int slice)
{
	if (slice >= 0 && slice <= this->myDim[1])
	{
		this->SliceXZMapper2->SetSliceNumber(slice);
	}
}
void vtkMDVolume::SetSliceNumberYZ(int slice)
{
	if (slice >= 0 && slice <= this->myDim[0])
	{
		this->SliceYZMapper2->SetSliceNumber(slice);
	}
}
void vtkMDVolume::CreateBox()
{
	//just in case a Clipping Box already exists.
	cout << "Call remove box from Create Box" << endl;
	this->RemoveBox();
	cout << "Create box 1" << endl;
	vtkSmartPointer<vtkBoxWidget>box = vtkSmartPointer<vtkBoxWidget>::New();
	box->SetInteractor(mqMorphoDigCore::instance()->getRenderer()->GetRenderWindow()->GetInteractor());
	box->SetPlaceFactor(1.01);
	cout << "Create box 2" << endl;
	box->SetInputData(this->GetImageData());
	box->SetDefaultRenderer(mqMorphoDigCore::instance()->getRenderer());
	box->InsideOutOn();
	box->PlaceWidget();
	box->SetInteractor(mqMorphoDigCore::instance()->getRenderer()->GetRenderWindow()->GetInteractor());
	cout << "Create box 3" << endl;
	vtkSmartPointer<vtkBoxWidgetCallback> callback = vtkSmartPointer<vtkBoxWidgetCallback>::New();
	callback->SetMapper(vtkSmartVolumeMapper::SafeDownCast(this->GetMapper()));
	
	box->AddObserver(vtkCommand::InteractionEvent, callback);

	box->EnabledOff();
	box->RotationEnabledOff();
	box->GetSelectedFaceProperty()->SetOpacity(0.0);
	this->SetBox(box);
	
	cout << "Create box 4... Now try to place it!" << endl;
	vtkTransform *t = vtkTransform::New();
	vtkSmartPointer<vtkMatrix4x4> Mat = this->GetMatrix();
	vtkSmartPointer<vtkMatrix4x4> translationMat = vtkSmartPointer<vtkMatrix4x4>::New();
	double tx, ty, tz;

	tx = Mat->GetElement(0, 3);
	ty = Mat->GetElement(1, 3);
	tz = Mat->GetElement(2, 3);
	// Ok, but not the good stuff!!!
	double center_init[3], center_final[3];
	this->GetInitCenter(center_init);
	mqMorphoDigCore::TransformPoint(Mat, center_init, center_final);
	
	/*double ve_init_pos[3];;
							double ve_final_pos[3];
							vtkSmartPointer<vtkMatrix4x4> Mat = myActor2->GetMatrix();


							for (vtkIdType j = 0; j < toSave->GetNumberOfPoints(); j++) {
								// for every triangle 
								toSave->GetPoint(j, ve_init_pos);
								mqMorphoDigCore::TransformPoint(Mat, ve_init_pos, ve_final_pos);*/

	cout << "center init" << center_init[0]<< "," << center_init[1] << "," << center_init[2] << endl;
	cout << "center final" << center_final[0] << "," << center_final[1] << "," << center_final[2] << endl;
	cout << "tx" << tx << "ty" << ty << "tz" << tz << endl;
	tx = center_init[0] - center_final[0];
	ty = center_init[1] - center_final[1];
	tz = center_init[2] - center_final[2];
	cout << "center init - final: tx" << tx << "ty" << ty << "tz" << tz << endl;

	translationMat->SetElement(0, 3, -tx);
	translationMat->SetElement(1, 3, -ty);
	translationMat->SetElement(2, 3, -tz);
	//New strategy: 


	box->GetTransform(t);
	/*vtkSmartPointer<vtkMatrix4x4> InitMat = t->GetMatrix();
	tx = InitMat->GetElement(0, 3); 0
	ty = InitMat->GetElement(1, 3); 0
	tz = InitMat->GetElement(2, 3); 0*/
	//cout << "Init tx" << tx << "ty" << ty << "tz" << tz << endl;

	t->SetMatrix(translationMat);
	box->SetTransform(t);
	
	t->Delete();
	cout << "Box placed.... correctly?" << endl;
}
void vtkMDVolume::PlaceBox(double BoxBounds[6])
{
	this->Box->PlaceWidget(BoxBounds);
	this->Box->Modified();
	vtkPlanes *planes = vtkPlanes::New();
	this->Box->GetPlanes(planes);
	this->Mapper->SetClippingPlanes(planes);
	planes->Delete();
}
void vtkMDVolume::RemoveBox()
{
	if (this->Box != NULL)
	{
		cout << "Remove Box 1, disable" << endl;
		this->Box->EnabledOff();
		this->Box->RemoveAllObservers();
		this->Box->SetInteractor(NULL);
		this->Box->SetDefaultRenderer(NULL);
		this->Box->Off();
		this->Box->Modified();
		this->GetMapper()->RemoveAllClippingPlanes();
		
		cout << "Remove Box 2, fast delete" << endl;
		//this->Box->FastDelete();
		cout << "Now this->Box is NULL pointer" << endl;
		
		//this->Box = NULL;
		this->Box = vtkSmartPointer<vtkBoxWidget>::New();
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
	vtkProp3D *myPropr3 = vtkProp3D::SafeDownCast(this->GetSliceXY2());
	vtkProp3D *myPropr4 = vtkProp3D::SafeDownCast(this->GetSliceXZ2());
	vtkProp3D *myPropr5 = vtkProp3D::SafeDownCast(this->GetSliceYZ2());
	vtkTransform *newTransform = vtkTransform::New();
	newTransform->PostMultiply();
	newTransform->SetMatrix(Mat);
	prop3D->SetPosition(newTransform->GetPosition());
	prop3D->SetScale(newTransform->GetScale());
	prop3D->SetOrientation(newTransform->GetOrientation());
	prop3D2->SetPosition(newTransform->GetPosition());
	prop3D2->SetScale(newTransform->GetScale());
	prop3D2->SetOrientation(newTransform->GetOrientation());

	myPropr3->SetPosition(newTransform->GetPosition());
	myPropr3->SetScale(newTransform->GetScale());
	myPropr3->SetOrientation(newTransform->GetOrientation());

	myPropr4->SetPosition(newTransform->GetPosition());
	myPropr4->SetScale(newTransform->GetScale());
	myPropr4->SetOrientation(newTransform->GetOrientation());

	myPropr5->SetPosition(newTransform->GetPosition());
	myPropr5->SetScale(newTransform->GetScale());
	myPropr5->SetOrientation(newTransform->GetOrientation());

	newTransform->Delete();
	

	//cout << "Center:" << mCenter[0] << "," << mCenter[1] << "," << mCenter[2] << "," << endl;
	this->SetChanged(1);

}
void vtkMDVolume::GetInitCenter(double center[3])
{
	center[0] = this->init_center[0];
	center[1] = this->init_center[1];
	center[2] = this->init_center[2];

}
void vtkMDVolume::InitCenter()
{
	double *mCenter = this->GetCenter();
	this->init_center[0] = mCenter[0];
	this->init_center[1] = mCenter[1];
	this->init_center[2] = mCenter[2];
	cout << "Volume center initialized at:" << this->init_center[0] << "," << this->init_center[1] << "," << this->init_center[2] << endl;
	
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
	vtkSmartPointer<vtkPiecewiseFunction> OF = ctf->GetScalarOpacityFunction();
	vtkSmartPointer<vtkPiecewiseFunction> SavedOF = vtkSmartPointer<vtkPiecewiseFunction>::New();
	Savedctf->DeepCopy(ctf);
	SavedOF->DeepCopy(OF);
	Savedctf->SetScalarOpacityFunction(SavedOF);

	ctf->DeepCopy(this->UndoRedo->UndoStack.back().Ctf);
	OF->DeepCopy(this->UndoRedo->UndoStack.back().Ctf->GetScalarOpacityFunction());
	ctf->SetScalarOpacityFunction(OF);
	
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
	vtkSmartPointer<vtkPiecewiseFunction> OF = ctf->GetScalarOpacityFunction();
	vtkSmartPointer<vtkPiecewiseFunction> SavedOF = vtkSmartPointer<vtkPiecewiseFunction>::New();
	SavedOF->DeepCopy(OF);
	Savedctf->DeepCopy(ctf);
	
	Savedctf->SetScalarOpacityFunction(SavedOF);

	ctf->DeepCopy(this->UndoRedo->RedoStack.back().Ctf);
	OF->DeepCopy(this->UndoRedo->RedoStack.back().Ctf->GetScalarOpacityFunction());
	ctf->SetScalarOpacityFunction(OF);
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

	vtkSmartPointer<vtkPiecewiseFunction> OF = ctf->GetScalarOpacityFunction();
	vtkSmartPointer<vtkPiecewiseFunction> SavedOF = vtkSmartPointer<vtkPiecewiseFunction>::New();
	SavedOF->DeepCopy(OF);
	Savedctf->SetScalarOpacityFunction(SavedOF);
	


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
