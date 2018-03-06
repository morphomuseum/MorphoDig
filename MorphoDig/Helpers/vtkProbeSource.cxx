/*=========================================================================

  Program:   MorphoDig
  Module:    Modified from Visualization Toolkit vtkArrowSource.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkProbeSource.h"

#include <vtkAppendPolyData.h>
#include <vtkConeSource.h>
#include <vtkCylinderSource.h>
#include <vtkReverseSense.h>
#include <vtkSphereSource.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkPolyData.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>

vtkStandardNewMacro(vtkProbeSource);

vtkProbeSource::vtkProbeSource()
{
	this->TipType = 1; //cone 1=> sphere
  this->ArrowLength = 1;
  this->TipResolution = 12;
  this->TipRadius = 0.05;
  this->TipLength = 0.2;
  this->ShaftResolution = 12;
  this->ShaftRadius = 0.01;
  this->Invert = false;

  this->SetNumberOfInputPorts(0);
}

int vtkProbeSource::RequestData(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **vtkNotUsed(inputVector),
  vtkInformationVector *outputVector)
{
  // get the info object
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // get the ouptut
  vtkPolyData *output = vtkPolyData::SafeDownCast(
    outInfo->Get(vtkDataObject::DATA_OBJECT()));

  int piece, numPieces;
  vtkCylinderSource *cyl = vtkCylinderSource::New();
  vtkTransform *trans0 = vtkTransform::New();
  vtkTransformFilter *tf0 = vtkTransformFilter::New();
  vtkConeSource *cone = vtkConeSource::New();
  vtkSphereSource *sphereSource = vtkSphereSource::New();
  vtkTransform *trans1 = vtkTransform::New();
  vtkTransform *trans2 = vtkTransform::New();
  vtkTransformFilter *tf1 = vtkTransformFilter::New();
  vtkTransformFilter *tf2 = vtkTransformFilter::New();
  vtkAppendPolyData *append = vtkAppendPolyData::New();
  vtkReverseSense *reverse = vtkReverseSense::New();
 
  piece = outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_PIECE_NUMBER());
  numPieces = outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_PIECES());
  outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_GHOST_LEVELS());

  if (this->TipType == 0)
  {
	  cyl->SetResolution(this->ShaftResolution);
	  cyl->SetRadius(this->ArrowLength*this->ShaftRadius);
	  cyl->SetHeight((1.0 - this->TipLength)*this->ArrowLength);
	  cyl->SetCenter(0, (1.0 - this->TipLength)*0.5*this->ArrowLength, 0.0);
	  cyl->CappingOn();

	  trans0->RotateZ(-90.0);
	  tf0->SetTransform(trans0);
	  tf0->SetInputConnection(cyl->GetOutputPort());

	  cone->SetResolution(this->TipResolution);
	  cone->SetHeight(this->TipLength*this->ArrowLength);
	  cone->SetRadius(this->TipRadius*this->ArrowLength);

	  trans1->Translate(this->ArrowLength*(1.0 - this->TipLength*0.5), 0.0, 0.0);
	  tf1->SetTransform(trans1);
	  tf1->SetInputConnection(cone->GetOutputPort());

	 

	
  }
  else
  {
	  cyl->SetResolution(this->ShaftResolution);
	  cyl->SetRadius(this->ArrowLength*this->ShaftRadius);
	  cyl->SetHeight((1.0 - (0.5*this->TipLength))*this->ArrowLength);
	  cyl->SetCenter(0, (1.0 - (0.5*this->TipLength))*0.5*this->ArrowLength, 0.0);
	  cyl->CappingOn();

	  trans0->RotateZ(-90.0);
	  tf0->SetTransform(trans0);
	  tf0->SetInputConnection(cyl->GetOutputPort());
	 
	  cone->SetResolution(this->TipResolution);
	  cone->SetHeight(this->TipLength*this->ArrowLength);
	  cone->SetRadius(this->TipRadius*this->ArrowLength);

	  sphereSource->SetCenter(0, 0, 0);
	  sphereSource->SetRadius(0.5*this->TipLength*this->ArrowLength);

	  trans1->Translate(this->ArrowLength*(1.0 - this->TipLength*0.5*0.5), 0.0, 0.0);
	  tf1->SetTransform(trans1);
	  tf1->SetInputConnection(sphereSource->GetOutputPort());
	  //tf1->SetInputConnection(cone->GetOutputPort());
	  
	  

  }
  append->AddInputConnection(tf0->GetOutputPort());
  append->AddInputConnection(tf1->GetOutputPort());
  
  trans2->Translate(1 * this->ArrowLength, 0, 0);
  trans2->RotateY(180.0);
  //trans2->Scale(-1, 1, 1);

  tf2->SetTransform(trans2);
  tf2->SetInputConnection(append->GetOutputPort());
  reverse->SetInputConnection(tf2->GetOutputPort());
  reverse->ReverseCellsOn();
  reverse->ReverseNormalsOn();


  if (piece == 0 && numPieces > 0)
    {
    if (this->Invert)
      {
		  tf2->Update();
		  output->ShallowCopy(tf2->GetOutput());
		  /*if (this->TipType == 0)
		  {
			  tf2->Update();
			  output->ShallowCopy(tf2->GetOutput());
		  }
		  else
		  {*/
			  //reverse->Update();
			  //output->ShallowCopy(reverse->GetOutput());
		  /*}*/
      }
    else
      {
      append->Update();
      output->ShallowCopy(append->GetOutput());
      }
    }
  sphereSource->Delete();
  cone->Delete();
  trans0->Delete();
  tf0->Delete();
  cyl->Delete();
  trans1->Delete();
  tf1->Delete();
  append->Delete();
  tf2->Delete();
  trans2->Delete();
  reverse->Delete();

  return 1;
}

void vtkProbeSource::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << "ArrowLength: " << this->ArrowLength << "\n";
  os << indent << "TipResolution: " << this->TipResolution << "\n";
  os << indent << "TipRadius: " << this->TipRadius << "\n";
  os << indent << "TipLength: " << this->TipLength << "\n";
  
  os << indent << "ShaftResolution: " << this->ShaftResolution << "\n";
  os << indent << "ShaftRadius: " << this->ShaftRadius << "\n";

  os << indent << "Invert: " << this->Invert << "\n";
}
