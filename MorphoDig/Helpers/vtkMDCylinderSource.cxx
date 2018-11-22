/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkMDCylinderSource.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkMDCylinderSource.h"

#include <vtkMath.h>
#include <vtkCellArray.h>
#include <vtkFloatArray.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>

#include <cmath>

vtkStandardNewMacro(vtkMDCylinderSource);

vtkMDCylinderSource::vtkMDCylinderSource (int res)
{
  this->Resolution = res;
  this->Height = 1.0;
  this->Radius = 0.5;
  this->Radius2 = 0.5;
  this->ConeHeight = 0;
  this->Center[0] = this->Center[1] = this->Center[2] = 0.0;
  this->OutputPointsPrecision = SINGLE_PRECISION;

  this->SetNumberOfInputPorts(0);
}

int vtkMDCylinderSource::RequestData(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **vtkNotUsed(inputVector),
  vtkInformationVector *outputVector)
{
  // get the info object
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // get the ouptut
  vtkPolyData *output = vtkPolyData::SafeDownCast(
    outInfo->Get(vtkDataObject::DATA_OBJECT()));

  double angle= 2.0 * vtkMath::Pi()/this->Resolution;
  int numPolys, numPts;
  double xbot[3], nbot[3];
  double xtop[3], ntop[3];
  double *center = this->Center;
  int i, idx;
  vtkIdType pts[VTK_CELL_SIZE];
  vtkPoints *newPoints;
  //vtkFloatArray *newNormals;
  //vtkFloatArray *newTCoords;
  vtkCellArray *newPolys;

//
// Set things up; allocate memory
//

  
    numPts = 2*this->Resolution +2; // Top and bottom points are not duplicated any longer + 2 at the center of the top and the bottom ... normal issues? 
    //numPolys = 2*this->Resolution+2;
	numPolys = 4 * this->Resolution;
  

  newPoints = vtkPoints::New();

  // Set the desired precision for the points in the output.
  if(this->OutputPointsPrecision == vtkAlgorithm::DOUBLE_PRECISION)
  {
    newPoints->SetDataType(VTK_DOUBLE);
  }
  else
  {
    newPoints->SetDataType(VTK_FLOAT);
  }

  newPoints->Allocate(numPts);
  //newNormals = vtkFloatArray::New();
  //newNormals->SetNumberOfComponents(3);
  //newNormals->Allocate(numPts);
  //newNormals->SetName("Normals");
 
  newPolys = vtkCellArray::New();
  newPolys->Allocate(newPolys->EstimateSize(numPolys,this->Resolution));
//
// Generate points and point data for sides
//
  for (i=0; i<this->Resolution; i++)
  {
    // x coordinate
    nbot[0] = ntop[0] = cos(i*angle);
    xbot[0] = (nbot[0] * this->Radius) + center[0];
    xtop[0] = (ntop[0] * this->Radius) + center[0];

    // y coordinate
    xbot[1] = 0.5 * this->Height + center[1];
    xtop[1] = -0.5 * this->Height + center[1];
    nbot[1] = ntop[1] = 0.0;
 
    // z coordinate
    nbot[2] = ntop[2] = -sin(i*angle);
    xbot[2] = (nbot[2] * this->Radius2) + center[2];
    xtop[2] = (ntop[2] * this->Radius2) + center[2];

    idx = 2*i;
    newPoints->InsertPoint(idx,xbot);
    newPoints->InsertPoint(idx+1,xtop);
    //newNormals->InsertTuple(idx,nbot);
   // newNormals->InsertTuple(idx+1,ntop);
  }
//
// Generate polygons for sides
//
  for (i=0; i<this->Resolution; i++)
  {
    pts[0] = 2*i; //0
    pts[1] = pts[0] + 1; //1
    pts[2] = (pts[1] + 2) % (2*this->Resolution);//3
  
    newPolys->InsertNextCell(3,pts);
	pts[0] = 2 * i;	//0
	pts[1] = (pts[0]+1 + 2) % (2 * this->Resolution);//3
	pts[2] = pts[1] - 1;//2
	newPolys->InsertNextCell(3, pts);
  }
//
// Generate points and point data for top/bottom polygons
//
  
   
	xbot[0] = xtop[0] = 0;
	xbot[1] = 0.5 * this->Height + this->ConeHeight*this->Height/100;
	xtop[1] = -0.5 * this->Height- this->ConeHeight*this->Height/100;
	xbot[2] = xtop[2] = 0;

	// bottom center
	newPoints->InsertPoint(2* this->Resolution, xbot);
	// top center 
	newPoints->InsertPoint(2 * this->Resolution+1, xtop);
//
// Generate polygons for top/bottom polygons
//
 /*   for (i=0; i<this->Resolution; i++)
    {
      pts[i] = 2*this->Resolution + i;
    }
    newPolys->InsertNextCell(this->Resolution,pts);
    for (i=0; i<this->Resolution; i++)
    {
      pts[i] = 3*this->Resolution + i;
    }
    newPolys->InsertNextCell(this->Resolution,pts);*/

	// Y minus
	for (i = 0; i<this->Resolution; i++)
	{
		//bottom
		pts[0] =2 * this->Resolution; //bottom center
		pts[1] = 2* i; // 0
		pts[2] = (pts[1]+2) % (2 * this->Resolution); //2		
		newPolys->InsertNextCell(3, pts);
		
		//top
		pts[0] = 2 * this->Resolution +1; //top center		
		pts[1] = (2 * i + 1 + 2) % (2 * this->Resolution); //3
		pts[2] = 2 * i + 1; // 1
		newPolys->InsertNextCell(3, pts);
		
	}
 
//
// Update ourselves and release memory
//
  output->SetPoints(newPoints);
  newPoints->Delete();

 // output->GetPointData()->SetNormals(newNormals);
  //newNormals->Delete();

 

  newPolys->Squeeze(); // since we've estimated size; reclaim some space
  output->SetPolys(newPolys);
  newPolys->Delete();

  return 1;
}

void vtkMDCylinderSource::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "Resolution: " << this->Resolution << "\n";
  os << indent << "Height: " << this->Height << "\n";
  os << indent << "Radius: " << this->Radius << "\n";
  os << indent << "Radius2: " << this->Radius2 << "\n";
  os << indent << "Center: (" << this->Center[0] << ", "
     << this->Center[1] << ", " << this->Center[2] << " )\n";  
  os << indent << "Output Points Precision: " << this->OutputPointsPrecision << "\n";
}
