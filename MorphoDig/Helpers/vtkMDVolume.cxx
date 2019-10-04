/*=========================================================================

Program:   MorphoDig
Module:    vtkMDVolume.cxx
=========================================================================*/
#include "mqMorphoDigCore.h"
#include "vtkMDVolume.h"
#include <vtkExtractVOI.h>
#include <vtkImageCast.h>
#include <vtkProperty.h>
#include <vtkObjectFactory.h>
#include <vtkImageFlip.h>
#include <vtkTransformFilter.h>
#include <vtkImagePermute.h>
#include <vtkImageChangeInformation.h>
#include <vtkImageMedian3D.h>
#include <vtkImageGaussianSmooth.h>
#include <vtkImageShiftScale.h>
#include <vtkPiecewiseFunction.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkBoundingBox.h>
#include <vtkMath.h>
#include <vtkDataArray.h>
#include <vtkSmartPointer.h>
#include <vtkImageResample.h>
#include <vtkImageReslice.h>
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
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkOpenGLGPUVolumeRayCastMapper.h>

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
#include <QInputDialog>
#include <vtkImageResample.h>
vtkStandardNewMacro(vtkMDVolume);



//----------------------------------------------------------------------------
vtkMDVolume::vtkMDVolume()
{
	this->UndoRedo = new vtkMDVolumeUndoRedo;
	this->Selected = 1;
	this->mapper_type = 2;
	this->displayROI = 0;
	this->enableROI = 0;
	this->isVisible = 1;
	this->isVisibleXY = 0;
	this->isVisibleXZ = 0;
	this->isVisibleYZ = 0;
	this->myDim[0] = 0;
	this->myDim[1] = 0;
	this->myDim[2] = 0;
	//this->KdTree = nullptr;
	//this->Octree = nullptr;
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
	this->MaskBinComputed = 0;
	this->useImageDataBinForVR = 0;
	this->ImageData = vtkSmartPointer<vtkImageData>::New();
	this->ImageDataBin = vtkSmartPointer<vtkImageData>::New();
	this->Mask = vtkSmartPointer<vtkImageData>::New();
	this->MaskBin = vtkSmartPointer<vtkImageData>::New();
	this->MaskBinComputed = 0;
	this->MaskEnabled = 0;
	this->MaskInitialized = 0;
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
	this->CropBox = vtkSmartPointer<vtkBoxWidget>::New();
	this->Changed = 0;
	this->Name = "New Volume";
	this->ScalarDisplayMax = (double)VTK_UNSIGNED_INT_MAX;
	this->ScalarDisplayMin = (double) VTK_UNSIGNED_INT_MIN;
	this->ScalarOpacityUnitDistance = 1;
	this->init_center[0] = 0;
	this->init_center[1] = 0;
	this->init_center[2] = 0;
	this->InitializeMapper();

}
void vtkMDVolume::SetMapperType(int mtype)
{
	this->mapper_type = mtype;
	this->SetImageDataAndMap(this->GetImageData());//will call Initialize mapper
	
}
void vtkMDVolume::InitializeMapper()
{
	if (mapper_type == 0)
	{
		vtkSmartPointer<vtkSmartVolumeMapper> mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
		//mapper->SetAutoAdjustSampleDistances(true);
		this->SetMapper(mapper);

	}
	else if (mapper_type == 1)
	{

		vtkSmartPointer <vtkGPUVolumeRayCastMapper> mapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
		//mapper->SetUseJittering(true);
		//mapper->SetAutoAdjustSampleDistances(true);
		//mapper->SetClampDepthToBackface(true);
		//mapper->SetUseDepthPass(true);
		this->SetMapper(mapper);
	}
	else if (mapper_type == 2)
	{
		//mapper->SetRequestedRenderModeToGPU();
		vtkSmartPointer <vtkOpenGLGPUVolumeRayCastMapper> mapper = vtkSmartPointer<vtkOpenGLGPUVolumeRayCastMapper>::New();
		//mapper->SetUseJittering(true);
		//mapper->SetAutoAdjustSampleDistances(true);
		//mapper->SetClampDepthToBackface(true);
		//mapper->SetUseDepthPass(true);
		this->SetMapper(mapper);

	}
}
//----------------------------------------------------------------------------
vtkMDVolume::~vtkMDVolume()
{
	//this->FreeKdTree();
	//this->FreeOctree();
	this->UndoRedo->RedoStack.clear();
	this->UndoRedo->UndoStack.clear();
	delete this->UndoRedo;
	



}
/*
void vtkMDVolume::BuildKdTree()
{
	

		this->KdTree = vtkSmartPointer<vtkKdTreePointLocator>::New();
	
		this->KdTree->SetDataSet(this->GetMask());
		cout << "Build KDtree with mask of " << this->GetMask()->GetNumberOfPoints() << " points" << endl;

		this->KdTree->BuildLocator();
	
}
*/
/*void vtkMDVolume::FreeKdTree()
{
	this->KdTree = nullptr;
}*/
/*
vtkSmartPointer<vtkKdTreePointLocator> vtkMDVolume::GetKdTree()
{
	return this->KdTree;
}
*/
/*
void vtkMDVolume::BuildOctree()
{


	this->Octree = vtkSmartPointer<vtkOctreePointLocator>::New();

	this->Octree->SetDataSet(this->GetMask());
	cout << "Build Octree with mask of " << this->GetMask()->GetNumberOfPoints()<<" points"<< endl;
	this->Octree->BuildLocator();

}
*/
/*
void vtkMDVolume::FreeOctree()
{
	this->Octree = nullptr;
}
*/


/*
vtkSmartPointer<vtkOctreePointLocator> vtkMDVolume::GetOctree()
{
	return this->Octree;
}
*/
void vtkMDVolume::ConnectMapperToMask()
{
	if (this->MaskEnabled == 1 && this->MaskInitialized==1)
	{
		if (this->mapper_type == 1)
		{
			vtkGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetMaskTypeToBinary();
			if (this->useImageDataBinForVR == 0)
			{
				vtkGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetMaskInput(this->GetMask());
			}
			else
			{
				vtkGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetMaskInput(this->GetMaskBin());
			}
		}
		else if (this->mapper_type == 2)
		{
			vtkOpenGLGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetMaskTypeToBinary();
			if (this->useImageDataBinForVR == 0)
			{
				vtkOpenGLGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetMaskInput(this->GetMask());
			}
			else
			{
				vtkOpenGLGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetMaskInput(this->GetMaskBin());
			}
		}
	}
	else
	{
		//stop mask inside mapper.
		if (this->mapper_type == 1)
		{

			vtkGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetMaskInput(NULL);

		}
		else if (this->mapper_type == 2)
		{

			vtkOpenGLGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetMaskInput(NULL);

		}

	}

}
void vtkMDVolume::SetMaskEnabled(int maskEnabled)
{
	this->MaskEnabled = maskEnabled;
	if (this->MaskEnabled == 1)
	{
		if (this->MaskInitialized == 0)
		{
			cout << "Initialize mask..." << endl;
			this->InitializeMask();
		}
					
	}
				
	this->ConnectMapperToMask();
	

	// now associate
}
void vtkMDVolume::HardenMask()
{
	if (this->MaskEnabled == 1 && this->GetMaskInitialized() == 1)
	{

		/*
		QMessageBox msgBox;
		msgBox.setText("This option will modify your volume data. There is no undo! Do you want to proceed anyway? ");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setDefaultButton(QMessageBox::No);
		int ret = msgBox.exec();
		if (ret == QMessageBox::No) {
			cout << "no!!!" << endl;
			return;
		}
		*/
		double default_value;
		double max_value;
		double min_value;
		int dataType = this->GetImageData()->GetScalarType();
		default_value = 0;
		min_value = 0;
		max_value = 65535;
		QString dataTypeAsString;
		if (dataType == VTK_UNSIGNED_SHORT)
		{
			default_value = 0;
			min_value = 0;
			max_value = 65535;


		}
		else if (dataType == VTK_SHORT)
		{
			default_value = -1000;
			min_value = -32767;
			max_value = 32767;

		}
		else if (dataType == VTK_CHAR)
		{
			default_value = -127;
			min_value = 0;
			max_value = 127;
		}
		else if (dataType == VTK_UNSIGNED_CHAR)
		{
			default_value = 0;
			min_value = 0;
			max_value = 255;
		}
		else if (dataType == VTK_SIGNED_CHAR)
		{
			default_value = 0;
			min_value = -127;
			max_value = 127;

		}
		else if (dataType == VTK_FLOAT)
		{
			default_value = 0;
			min_value = -FLT_MAX;
			max_value = FLT_MAX;

		}
		else if (dataType == VTK_DOUBLE)
		{
			default_value = 0;
			min_value = -DBL_MAX;
			max_value = DBL_MAX;
		}

		bool ok;
		double fillValue = QInputDialog::getDouble(0, "Harden mask",
			"Fill masked voxels with value (warning no undo):", default_value, min_value, max_value, 0, &ok);

		if (!ok) { return; }

		cout << "Harden mask!" << endl;
		// creates an empty mask
		this->MaskBinComputed = 0;
		int dims[3];


		this->Mask->GetDimensions(dims);

		for (int z = 0; z < dims[2]; z++)
		{
			for (int y = 0; y < dims[1]; y++)
			{
				for (int x = 0; x < dims[0]; x++)
				{

					unsigned char* pixel = static_cast<unsigned char*>(this->Mask->GetScalarPointer(x, y, z));
					if (pixel[0] == 0)
					{
						pixel[0] = 255;
						// et là appliquer la desired value!
						int dataType = this->GetImageData()->GetScalarType();

						QString dataTypeAsString;
						if (dataType == VTK_UNSIGNED_SHORT)
						{
							unsigned short* pixel2 = static_cast<unsigned short*>(this->GetImageData()->GetScalarPointer(x, y, z));
							pixel2[0] = fillValue;

						}
						else if (dataType == VTK_SHORT)
						{
							signed short* pixel2 = static_cast<signed short*>(this->GetImageData()->GetScalarPointer(x, y, z));
							pixel2[0] = fillValue;

						}
						else if (dataType == VTK_CHAR)
						{
							char* pixel2 = static_cast<char*>(this->GetImageData()->GetScalarPointer(x, y, z));
							pixel2[0] = fillValue;
						}
						else if (dataType == VTK_UNSIGNED_CHAR)
						{
							unsigned char* pixel2 = static_cast<unsigned char*>(this->GetImageData()->GetScalarPointer(x, y, z));
							pixel2[0] = fillValue;
						}
						else if (dataType == VTK_SIGNED_CHAR)
						{
							signed char* pixel2 = static_cast<signed char*>(this->GetImageData()->GetScalarPointer(x, y, z));
							pixel2[0] = fillValue;

						}
						else if (dataType == VTK_FLOAT)
						{
							float* pixel2 = static_cast<float*>(this->GetImageData()->GetScalarPointer(x, y, z));
							pixel2[0] = fillValue;

						}
						else if (dataType == VTK_DOUBLE)
						{
							double* pixel2 = static_cast<double*>(this->GetImageData()->GetScalarPointer(x, y, z));
							pixel2[0] = fillValue;

						}
						else if (dataType == VTK_BIT)
						{
							//dataTypeAsString = "Bit (1 bit)";
						}
					}


				}
			}
		}
		Mask->Modified();
		//update also volume image data!
		this->UpdateMaskData(Mask);
		QString myName = QString(this->GetName().c_str());
		myName = myName + "_hrd";
		this->SetName(myName.toStdString());
		this->Modified();
		this->SetImageDataAndMap(this->GetImageData());
		this->SetImageDataBinComputed(0);
		if (this->GetuseImageDataBinForVR() == 1) { this->ComputeImageDataBin(); }
	}
}
int vtkMDVolume::MaskWorthSaving()
{
	int worth = 0;
	int foundwhite= 0;
	int foundblack = 0;
	if (this->MaskEnabled == 1 && this->GetMaskInitialized() == 1)
	{
		int dims[3];


		this->Mask->GetDimensions(dims);

		for (int z = 0; z < dims[2]; z++)
		{
			for (int y = 0; y < dims[1]; y++)
			{
				for (int x = 0; x < dims[0]; x++)
				{

					unsigned char* pixel = static_cast<unsigned char*>(this->Mask->GetScalarPointer(x, y, z));
					if (pixel[0] == 0) { foundblack=1; }
					else if (pixel[0] == 255) { foundwhite =1; }

				}
			}
		}
		if (foundblack == 1 && foundwhite == 1) { worth = 1; }
	}
	return worth;
}
void vtkMDVolume::InvertMask()
{
	if (this->MaskEnabled == 1 && this->GetMaskInitialized() == 1)
	{
		cout << "Invert mask!" << endl;
		// creates an empty mask
		this->MaskBinComputed = 0;
		int dims[3];


		this->Mask->GetDimensions(dims);

		for (int z = 0; z < dims[2]; z++)
		{
			for (int y = 0; y < dims[1]; y++)
			{
				for (int x = 0; x < dims[0]; x++)
				{

					unsigned char* pixel = static_cast<unsigned char*>(this->Mask->GetScalarPointer(x, y, z));
					if (pixel[0] == 0) { pixel[0] = 255; }
					else if (pixel[0] == 255) { pixel[0] = 0; }

				}
			}
		}
		Mask->Modified();
		this->UpdateMaskData(Mask);
		this->SetImageDataBinComputed(0);
		if (this->GetuseImageDataBinForVR() == 1) { this->ComputeImageDataBin(); }
	}
}
void vtkMDVolume::InitializeMask()
{
	cout << "Initialize mask!" << endl;
	// creates an empty mask
	this->MaskBinComputed = 0;
	this->Mask = vtkSmartPointer<vtkImageData>::New();
	int dims[3];
	double res[3];
	double origin[3];
	
	
	cout << "Get Matrix!" << endl;
	this->GetImageData()->GetDimensions(dims);
	this->GetImageData()->GetSpacing(res);
	cout << "Get Origin!" << endl;
	this->GetImageData()->GetOrigin(origin);
	


	cout << "Set dims!" << endl;
	this->Mask->SetDimensions(dims);
	this->Mask->SetSpacing(res);
	cout << "Set Origin" << endl;

	this->Mask->SetOrigin(origin);
	cout << "Set Allocate scalars" << endl;
	this->Mask->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

	std::cout << "Initialize Mask Dims: " << " x: " << dims[0] << " y: " << dims[1] << " z: " << dims[2] << std::endl;

	std::cout << "Initialize Mask Res: " << " x: " << res[0] << " y: " << res[1] << " z: " << res[2] << std::endl;
	std::cout << "Initialize Mask Number of points: " << this->Mask->GetNumberOfPoints() << std::endl;
	std::cout << "Initialize Number of cells: " << this->Mask->GetNumberOfCells() << std::endl;
	for (int z = 0; z < dims[2]; z++)
	{
		for (int y = 0; y < dims[1]; y++)
		{
			for (int x = 0; x < dims[0]; x++)
			{
				
				unsigned char* pixel = static_cast<unsigned char*>(this->Mask->GetScalarPointer(x, y, z));
				
					pixel[0] = 255;
					
			}
		}
	}
	Mask->Modified();
	
	int dataType = this->Mask->GetScalarType();
	
	QString dataTypeAsString;
	if (dataType == VTK_UNSIGNED_SHORT)
	{
		dataTypeAsString = "Unsigned shorts (16 bits)";
	}
	else if (dataType == VTK_SHORT)
	{
		dataTypeAsString = "Signed shorts (16 bits)";
	}
	else if (dataType == VTK_CHAR)
	{
		dataTypeAsString = "Char (8 bits)";
	}
	else if (dataType == VTK_UNSIGNED_CHAR)
	{
		dataTypeAsString = "Unsigned char (8 bits)";
	}
	else if (dataType == VTK_SIGNED_CHAR)
	{
		dataTypeAsString = "Signed char (8 bits)";
	}
	else if (dataType == VTK_FLOAT)
	{
		dataTypeAsString = "Float (32 bits)";
	}
	else if (dataType == VTK_DOUBLE)
	{
		dataTypeAsString = "Double (64 bits)";
	}
	else if (dataType == VTK_BIT)
	{
		dataTypeAsString = "Bit (1 bit)";
	}
	else
	{
		dataTypeAsString = "Unknown";
	}
	cout << "Initialize Mask data type: " << dataTypeAsString.toStdString() << endl;
	//this->Mask->SetScalarType()
	this->MaskInitialized = 1;
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
			this->ConnectMapperToMask();
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
	// ne doit pas fonctionner à la rotation!
	vtkBoundingBox bbox(this->GetBounds()); // Expressed in WC, so after a rotation it gets larger than the actual bounding box... but this does not really matter... 
	//vtkSmartPointer<vtkBoundingBox> bbox = vtkSmartPointer<vtkBoundingBox>::New();
	//bbox.SetBounds(this->GetBounds());
	//appelé depuis une sélection
	//on ne selectionne ici que les 8 points de la box!!!!
	// changer en faisant une liste de 1000 points
	vtkSmartPointer<vtkPoints> pts =vtkSmartPointer<vtkPoints>::New();
	pts->SetNumberOfPoints(1000);
	vtkIdType ip = 0;
	double x, y, z;
	double ptA[3], ptB[3], ptD[3];

	double ptA2[3];

	bbox.GetCorner(0, ptA);
	bbox.GetCorner(1, ptB);
	//bbox.GetCorner(4, ptC);
	bbox.GetCorner(2, ptD);

	bbox.GetCorner(4, ptA2);
	
	//bbox.GetCorner(3, ptB2);
	//bbox.GetCorner(5, ptC2);
	//bbox.GetCorner(7, ptD2);

	for (vtkIdType i = 0; i < 10; i++) // i is 
	{

		// A-> D
		for (vtkIdType j = 0; j < 10; j++)
		{
		
			for (vtkIdType k = 0; k < 10; k++)
			{// A -> B

				x = ptA[0] + (ptB[0] - ptA[0])*k / 9;
				y = ptA[1] + (ptB[1] - ptA[1])*k / 9;
				z = ptA[2] + (ptB[2] - ptA[2])*k / 9;
				x += (ptD[0] - ptA[0])*j / 9;
				y += (ptD[1] - ptA[1])*j / 9;
				z += (ptD[2] - ptA[2])*j / 9;
				x+= (ptA2[0] - ptA[0])*i/ 9;
				y += (ptA2[1] - ptA[1])*i / 9;
				z += (ptA2[2] - ptA[2])*i / 9;
				if (ip < 1000)
				{
					pts->InsertPoint(ip, x, y, z);
					ip++;
				}
			}
		}
	}
	// on prend les 4 points d'un premier plan, les 4 points d'un deuxième plan
	// 1 3 5 7 => calculer les 100 points
	// 0 2 4 6 => calculer les 100 points
	// puis calculer les 800 points restants 
	// Une fois les 1000 calculés, les tester tous!
	cout << "Is inside frustum!" << endl;
	int is_inside = 0;
	int is_insideALL[6] = { 0,0,0,0,0,0 };
	vtkSmartPointer<vtkMatrix4x4> Mat = vtkSmartPointer<vtkMatrix4x4>::New();
	this->GetMatrix(Mat);
	
	int ok = 0;
	if (this->mapper_type == 0)
	{
		vtkSmartVolumeMapper *mapper = vtkSmartVolumeMapper::SafeDownCast(this->GetMapper());
		if (mapper != NULL) { ok = 1; }
	}
	else if (this->mapper_type == 1)
	{
		vtkGPUVolumeRayCastMapper *mapper = vtkGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper());
		if (mapper != NULL) { ok = 1; }
	}
	else if (this->mapper_type == 2)
	{
		vtkOpenGLGPUVolumeRayCastMapper *mapper = vtkOpenGLGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper());
		if (mapper != NULL) { ok = 1; }
	}
	//this->GetB
	if (ok)
	{
		

		//we have six planes
		for (vtkIdType j = 0; j < pts->GetNumberOfPoints(); j++)
		//for (vtkIdType j = 0; j < 8; j++)
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
			
				//bbox->GetCorner(j, pt);
				pts->GetPoint(j, pt);
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
void vtkMDVolume::ComputeMaskBin()
{
	vtkSmartPointer<vtkImageResample> resample = vtkSmartPointer<vtkImageResample>::New();
	resample->SetInputData(this->Mask);
	double magnification = 0.5;
	long long int numVox = (long long int) (this->myDim[0] * this->myDim[1] * this->myDim[2] / 8);
	if (numVox > mqMorphoDigCore::instance()->Getmui_VolumeOutOfCoreThreshold())
	{
		int num_bin_needed = 2;// at least 2!
		int ratio = (int)(numVox / mqMorphoDigCore::instance()->Getmui_VolumeOutOfCoreThreshold());
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
	this->MaskBin = resample->GetOutput();

	this->MaskBinComputed = 1;
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
	this->Outline->SetInputData(permutedData);
}
void vtkMDVolume::SwapXZ()
{
	vtkSmartPointer<vtkImagePermute> permute = vtkSmartPointer<vtkImagePermute>::New();
	permute->SetInputData(this->ImageData);
	permute->SetFilteredAxes(2, 1, 0);
	permute->Update();
	vtkSmartPointer<vtkImageData> permutedData = permute->GetOutput();
	this->SetImageDataAndMap(permutedData);
	this->Outline->SetInputData(permutedData);

}
void vtkMDVolume::SwapYZ()
{
	vtkSmartPointer<vtkImagePermute> permute = vtkSmartPointer<vtkImagePermute>::New();
	permute->SetInputData(this->ImageData);
	permute->SetFilteredAxes(0, 2, 1);
	permute->Update();
	vtkSmartPointer<vtkImageData> permutedData = permute->GetOutput();
	this->SetImageDataAndMap(permutedData);
	this->Outline->SetInputData(permutedData);
}
void vtkMDVolume::SwapXYZ()
{
	vtkSmartPointer<vtkImagePermute> permute = vtkSmartPointer<vtkImagePermute>::New();
	permute->SetInputData(this->ImageData);
	permute->SetFilteredAxes(1, 2, 0);
	permute->Update();
	vtkSmartPointer<vtkImageData> permutedData = permute->GetOutput();
	this->SetImageDataAndMap(permutedData);
	this->Outline->SetInputData(permutedData);
}
void vtkMDVolume::MedianFilter(int x, int y, int z)
{
	vtkSmartPointer<vtkImageMedian3D> median = vtkSmartPointer<vtkImageMedian3D>::New();
	median->SetInputData(this->ImageData);
	median->SetKernelSize(x, y, z);
	median->Update();
	vtkSmartPointer<vtkImageData> medianData = median->GetOutput();
	QString myName = QString(this->GetName().c_str());
	myName = myName + "_med";
	this->SetName(myName.toStdString());
	this->Modified();
	this->SetImageDataAndMap(medianData);
	this->Outline->SetInputData(medianData);
	this->UpdateHistogram();
	mqMorphoDigCore::instance()->sendSignalVolumeUpdateHistogram();
	// send message to recompute histogram and update XY slices!
	
}
void vtkMDVolume::Invert()
{
	//vtkSmartPointer<vtkImageShiftScale> shiftScale = vtkSmartPointer<vtkImageShiftScale>::New();
	// Bug with shiftscale : pixels with "0" value remain "0"... wrong!
	//shiftScale->SetInputData(this->ImageData);

	int dataType = this->GetImageData()->GetScalarType();	
	double shiftV = 0;
	if (dataType == VTK_UNSIGNED_SHORT)
	{
		
		shiftV = 65535;


	}
	else if (dataType == VTK_SHORT)
	{
		
		
		shiftV = 0;

	}
	else if (dataType == VTK_CHAR)
	{
		
		shiftV = 0;
	}
	else if (dataType == VTK_UNSIGNED_CHAR)
	{
		
		shiftV = 255;
	}
	else if (dataType == VTK_SIGNED_CHAR)
	{
		shiftV = 0;
		
	}
	else if (dataType == VTK_FLOAT)
	{
		shiftV = 0;
		

	}
	else if (dataType == VTK_DOUBLE)
	{
		shiftV= 0;
		
	}
	
		int dims[3];


	this->GetImageData()->GetDimensions(dims);
	double fillValue = 0;
	for (int z = 0; z < dims[2]; z++)
	{
		for (int y = 0; y < dims[1]; y++)
		{
			for (int x = 0; x < dims[0]; x++)
			{

				/*unsigned char* pixel = static_cast<unsigned char*>(this->Mask->GetScalarPointer(x, y, z));
				if (pixel[0] == 0) { pixel[0] = 255; }
				else if (pixel[0] == 255) { pixel[0] = 0; }*/
				if (dataType == VTK_UNSIGNED_SHORT)
				{
					unsigned short* pixel2 = static_cast<unsigned short*>(this->GetImageData()->GetScalarPointer(x, y, z));
					fillValue = (double)(-1 * pixel2[0]) + shiftV;
					pixel2[0] = fillValue;

				}
				else if (dataType == VTK_SHORT)
				{
					signed short* pixel2 = static_cast<signed short*>(this->GetImageData()->GetScalarPointer(x, y, z));
					fillValue = (double)(-1 * pixel2[0]) + shiftV;
					pixel2[0] = fillValue;

				}
				else if (dataType == VTK_CHAR)
				{
					char* pixel2 = static_cast<char*>(this->GetImageData()->GetScalarPointer(x, y, z));
					fillValue = (double)(-1 * pixel2[0]) + shiftV;
					pixel2[0] = fillValue;
				}
				else if (dataType == VTK_UNSIGNED_CHAR)
				{
					unsigned char* pixel2 = static_cast<unsigned char*>(this->GetImageData()->GetScalarPointer(x, y, z));
					fillValue = (double)(-1 * pixel2[0]) + shiftV;
					pixel2[0] = fillValue;
				}
				else if (dataType == VTK_SIGNED_CHAR)
				{
					signed char* pixel2 = static_cast<signed char*>(this->GetImageData()->GetScalarPointer(x, y, z));
					fillValue = (double)(-1 * pixel2[0]) + shiftV;
					pixel2[0] = fillValue;

				}
				else if (dataType == VTK_FLOAT)
				{
					float* pixel2 = static_cast<float*>(this->GetImageData()->GetScalarPointer(x, y, z));
					fillValue = (double)(-1 * pixel2[0]) + shiftV;
					pixel2[0] = fillValue;

				}
				else if (dataType == VTK_DOUBLE)
				{
					double* pixel2 = static_cast<double*>(this->GetImageData()->GetScalarPointer(x, y, z));
					fillValue = (double)(-1 * pixel2[0]) + shiftV;
					pixel2[0] = fillValue;

				}
				else if (dataType == VTK_BIT)
				{
					//dataTypeAsString = "Bit (1 bit)";
				}

			}
		}
	}
	QString myName = QString(this->GetName().c_str());
	myName = myName + "_inv";
	this->SetName(myName.toStdString());
	this->GetImageData()->Modified();
	this->GetImageData()->GetPointData()->GetScalars()->Modified(); // otherwise scalar range is not computed!
	
	//this->GetImageData()->GetScalarRan
	cout << "Invert, new scalar range:"<<this->GetImageData()->GetScalarRange()[0] << "," << this->GetImageData()->GetScalarRange()[1] << endl;
	/*this->SetScalarDisplayMin(this->GetImageData()->GetScalarRange()[0]);
	this->SetScalarDisplayMax(this->GetImageData()->GetScalarRange()[1]);*/
	this->Modified();
	this->SetImageDataAndMap(this->GetImageData());
	this->SetImageDataBinComputed(0);
	if (this->GetuseImageDataBinForVR() == 1) { this->ComputeImageDataBin(); }
	this->Outline->SetInputData(this->GetImageData());
	cout << "Call UpdateHistogram" << endl;
	this->UpdateHistogram();
	cout << "Call sendSignalVolumeUpdateHistogram" << endl;
	mqMorphoDigCore::instance()->sendSignalVolumeUpdateHistogram();
	mqMorphoDigCore::instance()->sendSignalVolumesMightHaveChanged();
	mqMorphoDigCore::instance()->Render();

	//shiftScale->SetClampOverflow(true);
	//shiftScale->Set
	//shiftScale->SetShift(shiftV); 
	//shiftScale->SetScale(-1);
	//shiftScale->Update();
	//vtkSmartPointer<vtkImageData> shiftScaleData = shiftScale->GetOutput();
	//this->SetImageDataAndMap(shiftScaleData);
	//this->Outline->SetInputData(shiftScaleData);
	
	

}

void vtkMDVolume::GaussianFilter(double std_x, double std_y, double std_z, double rad_x, double rad_y, double rad_z)
{
	vtkSmartPointer<vtkImageGaussianSmooth> gaussian = vtkSmartPointer<vtkImageGaussianSmooth>::New();
	gaussian->SetInputData(this->ImageData);
	gaussian->SetDimensionality(3);
	gaussian->SetStandardDeviations(std_x, std_y, std_z);
	gaussian->SetRadiusFactors(rad_x, rad_y, rad_z);
	gaussian->Update();
	vtkSmartPointer<vtkImageData> gaussianData = gaussian->GetOutput();
	QString myName = QString(this->GetName().c_str());
	myName = myName + "_gauss";
	this->SetName(myName.toStdString());
	this->Modified();
	this->SetImageDataAndMap(gaussianData);
	this->Outline->SetInputData(gaussianData);
	this->UpdateHistogram();
	mqMorphoDigCore::instance()->sendSignalVolumeUpdateHistogram();

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
	if (this->MaskEnabled== 1 && this->MaskInitialized==1)
	{
		this->Mask->SetSpacing(newSpacingX, newSpacingY, newSpacingZ);
	}
	this->SetImageDataAndMap(this->ImageData);
}
void vtkMDVolume::Reslice(int extended, int interpolationMethod)
{
	vtkSmartPointer<vtkImageReslice> reslice = vtkSmartPointer<vtkImageReslice>::New();
	reslice->SetInputData(this->ImageData);

	vtkSmartPointer<vtkMatrix4x4> Mat = this->GetMatrix();
	Mat->PrintSelf(std::cout, vtkIndent(1));
	vtkSmartPointer<vtkMatrix4x4> MatCpy = vtkSmartPointer<vtkMatrix4x4>::New();
	MatCpy->DeepCopy(Mat);
	MatCpy->SetElement(0,0, Mat->GetElement(0, 0));
	MatCpy->SetElement(0, 1, Mat->GetElement(1, 0));
	MatCpy->SetElement(0, 2, Mat->GetElement(2, 0));	
	MatCpy->SetElement(1, 0, Mat->GetElement(0, 1));
	MatCpy->SetElement(1, 1, Mat->GetElement(1, 1));
	MatCpy->SetElement(1, 2, Mat->GetElement(2, 1));
	MatCpy->SetElement(2, 0, Mat->GetElement(0, 2));
	MatCpy->SetElement(2, 1, Mat->GetElement(1, 2));
	MatCpy->SetElement(2, 2, Mat->GetElement(2, 2));


	MatCpy->SetElement(0, 3, 0);
	MatCpy->SetElement(1, 3, 0);
	MatCpy->SetElement(2, 3, 0);

	MatCpy->PrintSelf(std::cout, vtkIndent(1));
	

	//MatCpy->Transpose();
	vtkSmartPointer<vtkMatrix4x4> MatTrans = vtkSmartPointer<vtkMatrix4x4>::New();
	//vtkMatrix4x4::Transpose(Mat, MatTrans);
	


	//reslice->SetResliceAxes(this->GetMatrix());
	//reslice->SetRSetResliceAxes(this->GetMatrix());
	reslice->SetResliceAxes(MatCpy);
	//reslice->SetResliceAxes(MatTrans);
	if (interpolationMethod == 0)
	{
		reslice->SetInterpolationModeToLinear();
	}
	else if (interpolationMethod == 1)
	{
		reslice->SetInterpolationModeToNearestNeighbor();

	}
	else
	{
		reslice->SetInterpolationModeToCubic();
	}
	if (extended == 1)
	{
		reslice->SetAutoCropOutput(true);
	}
	else
	{
		reslice->SetAutoCropOutput(false);
	}
	reslice->Update();
	vtkSmartPointer<vtkImageData> reslicedData = reslice->GetOutput();
	vtkSmartPointer<vtkMatrix4x4> MatRetranslated = vtkSmartPointer<vtkMatrix4x4>::New();
	MatRetranslated->SetElement(0, 3, Mat->GetElement(0, 3));
	MatRetranslated->SetElement(1, 3, Mat->GetElement(1, 3));
	MatRetranslated->SetElement(2, 3, Mat->GetElement(2, 3));
	this->ApplyMatrix(MatRetranslated);
	QString myName = QString(this->GetName().c_str());
	myName = myName + "_rsp";
	this->SetName(myName.toStdString());

	//mask is not set nor initialized... 
	this->MaskEnabled = 0;
	this->MaskInitialized = 0;

	this->Modified();
	this->SetImageDataAndMap(reslicedData);
	this->Outline->SetInputData(reslicedData);
}

void vtkMDVolume::Resample(double newSpacingX, double newSpacingY, double newSpacingZ, int interpolationMethod)
{
	cout << "Resample image!!" << endl;
	vtkSmartPointer<vtkImageResample> resample = vtkSmartPointer<vtkImageResample>::New();
	resample->SetInputData(this->ImageData);
	
	resample->SetAxisOutputSpacing(0, newSpacingX);
	resample->SetAxisOutputSpacing(1, newSpacingY);
	resample->SetAxisOutputSpacing(2, newSpacingZ);
	//resample->Set
	if (interpolationMethod == 0)
	{
		resample->SetInterpolationModeToLinear();
	}
	else if (interpolationMethod == 1)
	{
		resample->SetInterpolationModeToNearestNeighbor();

	}
	else
	{
		resample->SetInterpolationModeToCubic();
	}
	resample->Update();
	vtkSmartPointer<vtkImageData> resampledData = resample->GetOutput();

	QString myName = QString(this->GetName().c_str());
	myName = myName + "_rsp";
	this->SetName(myName.toStdString());
	//mask is not set nor initialized... 
	this->MaskEnabled = 0;
	this->MaskInitialized = 0;
	this->Modified();
	//reinit mask!!!!
	this->SetImageDataAndMap(resampledData);

	
	//to implement
}
void vtkMDVolume::SetImageDataAndMap(vtkSmartPointer<vtkImageData> imgData)
{
	this->SetImageData(imgData);
	
	this->InitializeMapper();
	if (this->useImageDataBinForVR == 1)
	{
		if (this->mapper_type == 0)
		{
			vtkSmartVolumeMapper::SafeDownCast(this->GetMapper())->SetInputData(this->GetImageDataBin());
		}
		else if (this->mapper_type == 1)
		{
			vtkGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetInputData(this->GetImageDataBin());
		}
		else if (this->mapper_type == 2)
		{
			vtkOpenGLGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetInputData(this->GetImageDataBin());
		}
		
	}
	else
	{
		if (this->mapper_type == 0)
		{
			vtkSmartVolumeMapper::SafeDownCast(this->GetMapper())->SetInputData(this->GetImageData());
		}
		else if (this->mapper_type == 1)
		{
			vtkGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetInputData(this->GetImageData());
		}
		else if (this->mapper_type == 2)
		{
			vtkOpenGLGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetInputData(this->GetImageData());
		}
		
	}
	this->ConnectMapperToMask();
	
}
void vtkMDVolume::SetMaskData(vtkSmartPointer<vtkImageData> mskData)
{
	// to be called after SetImageDataAndMap, never before!
	this->Mask = mskData;
	this->MaskBinComputed = 0;
	


}
void vtkMDVolume::UpdateMaskData(vtkSmartPointer<vtkImageData> mskData)
{
	// to be called after SetImageDataAndMap, never before!
	this->Mask = mskData;
	this->MaskBinComputed = 0;

	// Reset to NULL
	if (this->mapper_type == 1)
	{

		vtkGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetMaskInput(NULL);

	}
	else if (this->mapper_type == 2)
	{

		vtkOpenGLGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetMaskInput(NULL);

	}


	if (this->mapper_type == 1)
	{
		vtkGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetMaskTypeToBinary();
		if (this->useImageDataBinForVR == 1)
		{
			vtkGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetMaskInput(this->GetMaskBin());
		}
		else
		{
			vtkGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetMaskInput(this->GetMask());
		}

	}
	else if (this->mapper_type == 2)
	{
		vtkOpenGLGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetMaskTypeToBinary();

		if (this->useImageDataBinForVR == 1)
		{
			vtkOpenGLGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetMaskInput(this->GetMaskBin());
		}
		else
		{
			vtkOpenGLGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetMaskInput(this->GetMask());
		}
	}

}
void vtkMDVolume::SetImageData(vtkSmartPointer<vtkImageData> imgData)
{
	this->ImageData = imgData;
	this->ImageDataBinComputed = 0;
	//this->MaskInitialized = 0; => in some cases we want to keep the mask!!!
	
	// now add a few things related to SliceXYMapper etc... 
	
	this->SliceXYMapper->SetInputData(imgData);
	this->SliceXZMapper->SetInputData(imgData);
	this->SliceYZMapper->SetInputData(imgData);
	this->SliceXYMapper2->SetInputData(imgData);
	this->SliceXZMapper2->SetInputData(imgData);
	this->SliceYZMapper2->SetInputData(imgData);

	
	
	imgData->GetDimensions(this->myDim);
	cout << "New dims:" << this->myDim[0] << "," << this->myDim[1] << "," << this->myDim[2] << endl;
	this->SliceXYMapper2->SetSliceNumber((int)(this->myDim[2]/2));
	this->SliceXZMapper2->SetSliceNumber((int)(this->myDim[1] / 2));
	this->SliceYZMapper2->SetSliceNumber((int)(this->myDim[0] / 2));

}
void vtkMDVolume::SetDesiredMappedImageData()
{
	vtkIdType numVox = (vtkIdType) ((vtkIdType)this->myDim[0] * (vtkIdType)this->myDim[1]* (vtkIdType)this->myDim[2]);
	cout << "................................................................................................" << endl;
	cout << "Found numVox="<<numVox << " voxels"<<endl;
	cout << "................................................................................................" << endl;
	if (numVox > mqMorphoDigCore::instance()->Getmui_VolumeOutOfCoreThreshold())
	{
		//
		QMessageBox msgBox;
		msgBox.setText("Number of voxels > Out of core threshold value (adjustable in Edit->Volume options). Volume rendering will be achieved on a subsampled version.");
		msgBox.exec();
		this->SetuseImageDataBinForVR(1);
	}
	else
	{

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
			this->InitializeMapper();

			cout << "change input data binned image" << endl;
			this->useImageDataBinForVR = 1;
			if (this->mapper_type == 0)
			{
				vtkSmartVolumeMapper::SafeDownCast(this->GetMapper())->SetInputData(this->GetImageDataBin());
			}
			else if (this->mapper_type == 1)
			{
				vtkGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetInputData(this->GetImageDataBin());
				
			}
			else if (this->mapper_type == 2)
			{
				vtkOpenGLGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetInputData(this->GetImageDataBin());
				
			}
			
			
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
			this->InitializeMapper();

			cout << "change input data normal image!" << endl;
			
			//mapper->SetInputData(this->GetImageData());
			if (this->mapper_type == 0)
			{
				vtkSmartVolumeMapper::SafeDownCast(this->GetMapper())->SetInputData(this->GetImageData());
			}
			else if (this->mapper_type == 1)
			{
				vtkGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetInputData(this->GetImageData());
				
					
				
			}
			else if (this->mapper_type == 2)
			{
				vtkOpenGLGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetInputData(this->GetImageData());
				
			}
			this->useImageDataBinForVR = 0;
		}
		if (this->enableROI == 1
			)
		{
			cout << "Try to use clipping planes!" << endl;
			/*volume->PlaceBox(BoxBounds);*/
			vtkPlanes *planes = vtkPlanes::New();
			this->Box->GetPlanes(planes);
			if (this->mapper_type == 0)
			{
				vtkSmartVolumeMapper::SafeDownCast(this->GetMapper())->SetClippingPlanes(planes);
			}
			else if (this->mapper_type == 1)
			{
				vtkGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetClippingPlanes(planes);
			}
			else if (this->mapper_type == 2)
			{
				vtkOpenGLGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetClippingPlanes(planes);
			}
			
			planes->Delete();
		}
		if (this->GetMaskEnabled())
		{
			if (this->MaskInitialized == 0)
			{
				this->InitializeMask();
			}
			if (this->mapper_type == 1)
			{
				vtkGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetMaskTypeToBinary();
				if (use == 1)
				{
					vtkGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetMaskInput(this->GetMaskBin());
				}
				else
				{
					vtkGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetMaskInput(this->GetMask());
				}
				
			}
			else if (this->mapper_type == 2)
			{
				vtkOpenGLGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetMaskTypeToBinary();

				if (use == 1)
				{
					vtkOpenGLGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetMaskInput(this->GetMaskBin());
				}
				else
				{
					vtkOpenGLGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper())->SetMaskInput(this->GetMask());
				}
			}
		}

	}
	return 1;

}
vtkSmartPointer<vtkImageData> vtkMDVolume::GetMaskBin()
{
	if (this->MaskBinComputed == 0)
	{
		cout << "ComputeImageDataBin" << endl;
		this->ComputeMaskBin();
	}
	return this->MaskBin;
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
void vtkMDVolume::SetMaskBin(vtkSmartPointer<vtkImageData> mskBin)
{
	this->MaskBin = mskBin;
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
void vtkMDVolume::GetCropCenter(double cropCenter[3])
{
	vtkTransform *t = vtkTransform::New();

	this->Box->GetTransform(t);
	vtkSmartPointer<vtkMatrix4x4> Mat = t->GetMatrix();
	double cx = Mat->GetElement(0, 3);
	double cy = Mat->GetElement(1, 3);
	double cz = Mat->GetElement(2, 3);
	cropCenter[0] = cx;
	cropCenter[1] = cy;
	cropCenter[2] = cz;
}
void vtkMDVolume::GetCropBounds(double cropBounds[6])
{
	double xmin, xmax, ymin, ymax, zmin, zmax;
	xmin = DBL_MAX;
	ymin = DBL_MAX;
	zmin = DBL_MAX;
	xmax = -DBL_MAX;
	ymax = -DBL_MAX;
	zmax = -DBL_MAX;
	double mCropCenter[3] = { 0,0,0 };
	double mCropDiff[3] = { 0,0,0 };
	this->GetCropCenter(mCropCenter);
	vtkPlanes *p = vtkPlanes::New();
	this->CropBox->GetPlanes(p);
	for (vtkIdType i = 0; i < p->GetNumberOfPlanes(); i++)
	{

		vtkPlane *plane = p->GetPlane(i);
		double origin[3] = { 0,0,0 };
		plane->GetOrigin(origin);
		mCropDiff[0] = origin[0] - mCropCenter[0];
		mCropDiff[1] = origin[1] - mCropCenter[1];
		mCropDiff[2] = origin[2] - mCropCenter[2];
		if (origin[0] < xmin) { xmin = origin[0]; }
		if (origin[0] > xmax) { xmax = origin[0]; }
		if (origin[1] < ymin) { ymin = origin[1]; }
		if (origin[1] > ymax) { ymax = origin[1]; }
		if (origin[2] < zmin) { zmin = origin[2]; }
		if (origin[2] > zmax) { zmax = origin[2]; }
		cout << "Plane " << i << ":" << origin[0] << "," << origin[1] << "," << origin[2] << endl;
		//cout << "Plane " << i << ":" << mBoxDiff[0] << "," << mBoxDiff[1] << "," << mBoxDiff[2] << endl;
	}
	cropBounds[0] = xmin;
	cropBounds[1] = xmax;
	cropBounds[2] = ymin;
	cropBounds[3] = ymax;
	cropBounds[4] = zmin;
	cropBounds[5] = zmax;


}
void vtkMDVolume::GetCropDimensions(int cropDimensions[6])
{
	int xMin, xMax, yMin, yMax, zMin, zMax;
	double cropXMin, cropXMax, cropYMin, cropYMax, cropZMin, cropZMax;
	double CropBounds[6];
	double CorrectedCropBounds[6];
	
	this->GetCropBounds(CropBounds); // are expressed in world coordinates
	cropXMin = CropBounds[0];
	cropXMax = CropBounds[1];
	cropYMin = CropBounds[2];
	cropYMax = CropBounds[3];
	cropZMin = CropBounds[4];
	cropZMax = CropBounds[5];
	int dim[3];
	this->GetImageData()->GetDimensions(dim);
	double res[3];
	this->GetImageData()->GetSpacing(res);

	xMin = dim[0];
	xMax = 0;
	yMin = dim[1];
	yMax = 0;
	zMin = dim[2];
	zMax = 0;


	cout << "Num cells = " << this->GetImageData()->GetNumberOfCells() << endl;

	cout << "Num points = " << this->GetImageData()->GetNumberOfPoints() << endl;

	/*for (vtkIdType i = 0; i < this->GetImageData()->GetNumberOfPoints() ; i++)
	{
		double pt[3];
		this->GetImageData()->GetPoint(i, pt);
		if (i < 10) { cout << "Pt" << i << ":" << pt[0] << "," << pt[1] << "," << pt[2] <<  endl; }
	}*/
	int ijk[3] = { 0, 0, 0 };
	vtkSmartPointer<vtkMatrix4x4> Mat = vtkSmartPointer<vtkMatrix4x4>::New();
	this->GetMatrix(Mat);
	/* ... we probably can achieve the same thing with the 8 bounding box points in much less time!
	for (vtkIdType i = 0; i < dim[0]; i++)
	{
		ijk[0] = i;
		for (vtkIdType j = 0; j < dim[1]; j++)
		{
			ijk[1] = j;
			for (vtkIdType k = 0; k < dim[2]; k++)
			{
				ijk[2] = k;
				vtkIdType ptId = this->GetImageData()->ComputePointId(ijk);

				double pt[3];
				this->GetImageData()->GetPoint(ptId, pt);
				double ptwc[3];
				mqMorphoDigCore::TransformPoint(Mat, pt, ptwc);
				//if (i < 5 && j<5 && k<5) { cout <<"i"<<i<<"j"<<j<<"k"<<k<< ", Pt" << ptId << ":" << pt[0] << "," << pt[1] << "," << pt[2] << endl; }
				if (i < xMin && cropXMin < ptwc[0]) { xMin = i; }
				if (i > xMax && cropXMax > ptwc[0]) { xMax = i; }
				if (j < yMin && cropYMin < ptwc[1]) { yMin = j; }
				if (j > yMax && cropYMax > ptwc[1]) { yMax = j; }
				if (k < zMin && cropZMin < ptwc[2]) { zMin = k; }
				if (k > zMax && cropZMax > ptwc[2]) { zMax = k; }

			}
		}
	}
	*/

	//vtkSmartPointer<vtkBoundingBox>bbox = vtkSmartPointer<vtkBoundingBox>::New();
	vtkBoundingBox bbox(this->GetBounds()); // already in WC...
	double Bounds[6];
	this->GetBounds(Bounds);
	cout << "Bounds:" << Bounds[0] << "," << Bounds[1] << "," << Bounds[2] << "," << Bounds[3] << "," << Bounds[4] << "," << Bounds[5] << endl;
	//bbox->SetBounds(this->GetBounds());
	
	cout << "CropBounds:" << CropBounds[0] << "," << CropBounds[1] << "," << CropBounds[2] << "," << CropBounds[3] << "," << CropBounds[4] << "," << CropBounds[5] << endl;
	//translate to cropbounds in doubles
	CorrectedCropBounds[0] = Bounds[0];
	CorrectedCropBounds[1] = Bounds[1];
	CorrectedCropBounds[2] = Bounds[2];
	CorrectedCropBounds[3] = Bounds[3];
	CorrectedCropBounds[4] = Bounds[4];
	CorrectedCropBounds[5] = Bounds[5];
	if (CropBounds[0] > Bounds[0] && CropBounds[0] < Bounds[1]) { CorrectedCropBounds[0] = CropBounds[0]; }
	if (CropBounds[1] < Bounds[1] && CropBounds[1] > Bounds[0]) { CorrectedCropBounds[1] = CropBounds[1]; }
	if (CropBounds[2] > Bounds[2] && CropBounds[2] < Bounds[3]) { CorrectedCropBounds[2] = CropBounds[2]; }
	if (CropBounds[3] < Bounds[3] && CropBounds[3] > Bounds[2]) { CorrectedCropBounds[3] = CropBounds[3]; }
	if (CropBounds[4] > Bounds[4] && CropBounds[4] < Bounds[5]) { CorrectedCropBounds[4] = CropBounds[4]; }
	if (CropBounds[5] < Bounds[5] && CropBounds[5] > Bounds[4]) { CorrectedCropBounds[5] = CropBounds[5]; }
	cout << "CorrectedCropBounds:" << CorrectedCropBounds[0] << "," << CorrectedCropBounds[1] << "," << CorrectedCropBounds[2] << "," << CorrectedCropBounds[3] << "," << CorrectedCropBounds[4] << "," << CorrectedCropBounds[5] << endl;
	double lengthX = Bounds[1] - Bounds[0];
	double lengthY = Bounds[3] - Bounds[2];
	double lengthZ = Bounds[5] - Bounds[4];
	cout << "Length:" << lengthX << "," << lengthY << "," << lengthZ << endl;

	double xMinPercent, xMaxPercent, yMinPercent, yMaxPercent, zMinPercent, zMaxPercent ;
	xMinPercent = (CorrectedCropBounds[0] - Bounds[0]) / lengthX;
	xMaxPercent = (CorrectedCropBounds[1] - Bounds[0]) / lengthX;
	yMinPercent = (CorrectedCropBounds[2] - Bounds[2]) / lengthY;
	yMaxPercent = (CorrectedCropBounds[3] - Bounds[2]) / lengthY;
	zMinPercent = (CorrectedCropBounds[4] - Bounds[4]) / lengthZ;
	zMaxPercent = (CorrectedCropBounds[5] - Bounds[4]) / lengthZ;
	cout << "Percentages:" << xMinPercent << "," << xMaxPercent << "," << yMinPercent << "," << xMaxPercent << "," << zMinPercent << "," << zMaxPercent << endl;

	xMin = (int)(xMinPercent *dim[0]);
	xMax = (int)(xMaxPercent *dim[0]);
	yMin = (int)(yMinPercent *dim[1]);
	yMax = (int)(yMaxPercent *dim[1]);
	zMin = (int)(zMinPercent *dim[2]); 
	zMax = (int)(zMaxPercent *dim[2]);
	/*xMin = (int)(CropBounds[0] / res[0]);
	xMax = (int)(CropBounds[1] / res[0]);
	yMin = (int)(CropBounds[2] / res[1]);
	yMax = (int)(CropBounds[3] / res[1]);
	zMin = (int)(CropBounds[4] / res[2]);
	zMax = (int)(CropBounds[5] / res[2]);*/


	
	

	cout << "Found crop xm xM ym yM zm zM" << xMin << "," << xMax << "," << yMin << "," << yMax << "," << zMin << "," << zMax << endl;


	
	/*xMin = (int)(CropBounds[0] / res[0]);
	xMax = (int)(CropBounds[1] / res[0]);
	yMin = (int)(CropBounds[2] / res[1]);
	yMax = (int)(CropBounds[3] / res[1]);
	zMin = (int)(CropBounds[4] / res[2]);
	zMax = (int)(CropBounds[5] / res[2]);*/
	
	// just in case the crop region would extend the actal volume dimensions... 
	if (xMin < 0) { xMin = 0; }
	if (xMax > dim[0]) { xMax = dim[0]; }
	if (yMin < 0) { yMin = 0; }
	if (yMax > dim[1]) { yMax = dim[1]; }
	if (zMin < 0) { zMin = 0; }
	if (zMax > dim[2]) { zMax = dim[2]; }

	// in case crop volume would be empty, just to not crop... 
	if (xMin > xMax || yMin > yMax || zMin > zMax)
	{
		xMin = 0;
		xMax = dim[0];
		yMin = 0;
		yMax = dim[1];
		zMin = 0;
		zMax = dim[2];
	}
	cout << "Adjusted crop dimensions : xm xM ym yM zm zM" << xMin << "," << xMax << "," << yMin << "," << yMax << "," << zMin << "," << zMax << endl;
	int extent[6];
	this->GetImageData()->GetExtent(extent);
	cout << "Image extent:" << extent[0] << "," << extent[1] << "," << extent[2] << "," << extent[3] << "," << extent[4] << "," << extent[5] << endl;
	cropDimensions[0] = xMin+extent[0];
	cropDimensions[1] = xMax + extent[0];
	cropDimensions[2] = yMin+extent[2];
	cropDimensions[3] = yMax + extent[2];
	cropDimensions[4] = zMin+extent[4];
	cropDimensions[5] = zMax + extent[4];

}
int vtkMDVolume::isRotated()
{
	vtkSmartPointer<vtkMatrix4x4> Mat = vtkSmartPointer<vtkMatrix4x4>::New();
	this->GetMatrix(Mat);
	if (
		Mat->GetElement(0, 0) != 1 ||
		Mat->GetElement(0, 1) != 0 ||
		Mat->GetElement(0, 2) != 0 ||
		Mat->GetElement(1, 0) != 0 ||
		Mat->GetElement(1, 1) != 1 ||
		Mat->GetElement(1, 2) != 0 ||
		Mat->GetElement(2, 0) != 0 ||
		Mat->GetElement(2, 1) != 0 ||
		Mat->GetElement(2, 2) != 1
		)	{return 1; }
	else { return 0; }

}
void vtkMDVolume::CropVolume()
{
	cout <<"crop volume" <<endl; 
	int CropDimensions[6];
	int Extent[6];
	
	int ExtentCropped[6];
	
	double oldOrigin[3];
	double oldOriginWC[3];
	double newOrigin[3];
	double newOriginWC[3];

	//int isRotated = this->IsRotated();
	if (this->isRotated())
	{
		QMessageBox msgBox;
		msgBox.setText("This volume was rotated. In order to be cropped, it must be resliced, which may (slightly) decrease data quality. Do you want to proceed anyway? ");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setDefaultButton(QMessageBox::No);
		int ret = msgBox.exec();
		if (ret == QMessageBox::No) {
			cout << "no!!!" << endl;
			return;
		}
		this->Reslice(1, 2);
	}
	this->GetImageData()->GetExtent(Extent);

	/*MatRetranslated->SetElement(0, 3, Mat->GetElement(0, 3));
	MatRetranslated->SetElement(1, 3, Mat->GetElement(1, 3));
	MatRetranslated->SetElement(2, 3, Mat->GetElement(2, 3));*/

	//{
	// warning message!
	//this->reslice();}

	this->GetCropDimensions(CropDimensions);
	vtkSmartPointer<vtkExtractVOI> voi = vtkSmartPointer<vtkExtractVOI>::New();
	int dim[3];
	double res[3];
	int dim2[3];
	double res2[3];
	this->GetImageData()->GetDimensions(dim);
	this->GetImageData()->GetSpacing(res);
	int xMin, xMax, yMin, yMax, zMin, zMax;
	xMin = CropDimensions[0];
	xMax = CropDimensions[1];
	yMin = CropDimensions[2];
	yMax = CropDimensions[3];
	zMin = CropDimensions[4];
	zMax = CropDimensions[5];

	voi->SetInputData(this->GetImageData());
	//voi->SetIncludeBoundary(true);
	//voi->set
	
	cout << "Crop dims:" << xMin << "," << xMax << "," << yMin << "," << yMax << "," << zMin << "," << zMax  << endl;
	
	
	
	/*for (vtkIdType i = 0; i < this->GetImageData()->GetNumberOfPoints(); i++)
	{
		
		this->GetImageData()->SetAxi
	}*/
	voi->SetVOI(xMin, xMax, yMin, yMax, zMin, zMax);
	voi->Update();
	vtkSmartPointer<vtkImageData> croppedData = voi->GetOutput();
	
	//extractedCastFilter->SetOutputScalarTypeToUnsignedChar();
	

	croppedData->GetDimensions(dim2);
	croppedData->GetSpacing(res2);
	

	this->GetImageData()->GetOrigin(oldOrigin);
	
	
	croppedData->GetOrigin(oldOrigin);
	
	cout << "oldOrigin:" << oldOrigin[0] << "," << oldOrigin[1] << "," << oldOrigin[2] << endl;
	vtkSmartPointer<vtkMatrix4x4> Mat = this->GetMatrix();
	double tx, ty, tz;


	tx = Mat->GetElement(0, 3);
	ty = Mat->GetElement(1, 3);
	tz = Mat->GetElement(2, 3);
	oldOriginWC[0] = oldOrigin[0] + tx;
	oldOriginWC[1] = oldOrigin[1] + ty;
	oldOriginWC[2] = oldOrigin[2] + tz;
	cout << "oldOriginWC:" << oldOriginWC[0] << "," << oldOriginWC[1] << "," << oldOriginWC[2] << endl;
	//cout << "Translation:" << tx << "," << ty <<","<< tz << endl;
	
	newOriginWC[0] = xMin*res[0] + oldOriginWC[0];
	newOriginWC[1] = yMin * res[1] + oldOriginWC[1];
	newOriginWC[2] = zMin*res[2] + oldOriginWC[2];
	cout << "newOriginWC:" << xMin * res[0] << "," << yMin * res[1] << "," << zMin * res[2] << endl;
	newOrigin[0] = newOriginWC[0]-tx;
	newOrigin[1] = newOriginWC[1] - ty;
	newOrigin[2] = newOriginWC[2] - tz; 
	double pt[3];
	croppedData->GetPoint(0, pt);
	
	cout << "Crop point 0 coordinates:" << pt[0] << "," << pt[1] << "," << pt[2] << endl;
	
	cout << "New origin:" << newOrigin[0]<< "," << newOrigin[1] << "," << newOrigin[2] << endl;
	//croppedData->SetOrigin(pt[0], pt[1], pt[2]); // si on fait ça il faut faire un hard transform pour remettre à 0 tous les points.
	
	
//	croppedData->GetExtent(extent);
	croppedData->GetExtent(ExtentCropped);
	cout << "Init data extent:" << Extent[0] << "," << Extent[1] << "," << Extent[2] << "," << Extent[3] << "," << Extent[4] << "," << Extent[5] << endl;


	cout << "Cropped data extent:" << ExtentCropped[0] << "," << ExtentCropped[1] << "," << ExtentCropped[2] << "," << ExtentCropped[3] << "," << ExtentCropped[4] <<  "," << ExtentCropped[5]<<endl;

	
	vtkSmartPointer<vtkImageChangeInformation> change = vtkSmartPointer<vtkImageChangeInformation>::New();
	change->SetInputData(croppedData);
	change->SetOutputSpacing(res[0], res[1], res[2]);
	//change->SetOriginTranslation(pt[0], pt[1], pt[2]);
	change->SetOutputOrigin(pt[0], pt[1], pt[2]);
	change->SetExtentTranslation(-ExtentCropped[0], -ExtentCropped[2], -ExtentCropped[4]);



	//change->CenterImageOn();
	change->Update();
	//double pt2[3];

	//cout << "PT2" << pt2[0] << "," << pt2[1] << "," << pt2[2] << endl;
	
	
	
	//si on veut faire ça, il faut aussi mettre à jour la matrice de position pour maintenir la même position!
	//croppedData->SetExtent(0, dim2[0] - 1, 0, dim2[1] - 1, 0, dim2[2] - 1);
	
	QString myName = QString(this->GetName().c_str());
	myName = myName + "_crp";
	this->SetName(myName.toStdString());
	this->MaskEnabled = 0;
	this->MaskInitialized = 0;
	this->Modified();
	//this->SetImageDataAndMap(croppedData);
	//this->Outline->SetInputData(croppedData);
	change->GetOutput()->GetExtent(Extent);
	cout << "Extent modified:" << Extent[0] << "," << Extent[1] << "," << Extent[2] << "," << Extent[3] << "," << Extent[4] << "," << Extent[5] << endl;

	this->SetImageDataAndMap(change->GetOutput());
	this->Outline->SetInputData(change->GetOutput());
	this->UpdateHistogram();
	mqMorphoDigCore::instance()->sendSignalVolumeUpdateHistogram();
}
void vtkMDVolume::CreateCropBox()
{
	//just in case a Clipping Box already exists.
	cout << "Call remove cropbox from CreateCropBox" << endl;
	this->RemoveCropBox();
	cout << "Create cropbox 1" << endl;
	vtkSmartPointer<vtkBoxWidget>cropbox = vtkSmartPointer<vtkBoxWidget>::New();
	cropbox->SetInteractor(mqMorphoDigCore::instance()->getRenderer()->GetRenderWindow()->GetInteractor());
	cropbox->SetPlaceFactor(1.01);
	cout << "Create box 2" << endl;
	cropbox->SetInputData(this->GetImageData());
	cropbox->SetDefaultRenderer(mqMorphoDigCore::instance()->getRenderer());
	cropbox->InsideOutOn();
	cropbox->PlaceWidget();
	cropbox->SetInteractor(mqMorphoDigCore::instance()->getRenderer()->GetRenderWindow()->GetInteractor());
	cout << "Create box 3" << endl;
	//vtkSmartPointer<vtkBoxWidgetCallback> callback = vtkSmartPointer<vtkBoxWidgetCallback>::New();
	
	//cropbox->AddObserver(vtkCommand::InteractionEvent, callback);

	cropbox->EnabledOn();
	cropbox->RotationEnabledOff();
	cropbox->GetSelectedFaceProperty()->SetOpacity(0.0);
	this->SetCropBox(cropbox);

	cout << "Create cropbox 4... Now try to place it!" << endl;


	

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
	

	cout << "center init" << center_init[0] << "," << center_init[1] << "," << center_init[2] << endl;
	cout << "center final" << center_final[0] << "," << center_final[1] << "," << center_final[2] << endl;
	cout << "tx" << tx << "ty" << ty << "tz" << tz << endl;
	tx = center_init[0] - center_final[0];
	ty = center_init[1] - center_final[1];
	tz = center_init[2] - center_final[2];
	cout << "center init - final: tx" << tx << "ty" << ty << "tz" << tz << endl;

	translationMat->SetElement(0, 3, -tx);
	translationMat->SetElement(1, 3, -ty);
	translationMat->SetElement(2, 3, -tz);	


	cropbox->GetTransform(t);
	

	t->SetMatrix(translationMat);
	cropbox->SetTransform(t);

	t->Delete();
	cout << "CropBox placed.... correctly?" << endl;
	
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
	if (this->mapper_type==0)
	{
		callback->SetMapper(vtkSmartVolumeMapper::SafeDownCast(this->GetMapper()));
	}
	else if (this->mapper_type == 1)
	{
		callback->SetMapper1(vtkGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper()));
	}
	else if (this->mapper_type == 2)
	{
		callback->SetMapper2(vtkOpenGLGPUVolumeRayCastMapper::SafeDownCast(this->GetMapper()));
	}
	
	
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
void vtkMDVolume::RemoveCropBox()
{
	if (this->CropBox != NULL)
	{
		cout << "Remove CropBox 1, disable" << endl;
		this->CropBox->EnabledOff();
		this->CropBox->RemoveAllObservers();
		this->CropBox->SetInteractor(NULL);
		this->CropBox->SetDefaultRenderer(NULL);
		this->CropBox->Off();
		this->CropBox->Modified();
		//this->GetMapper()->RemoveAllClippingPlanes();		
		
		this->CropBox = vtkSmartPointer<vtkBoxWidget>::New();
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
void vtkMDVolume::UpdateHistogram()
{
	this->Hist->SetInputData(this->GetImageData());		
	this->Hist->Modified();
	this->Hist->Update();
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
