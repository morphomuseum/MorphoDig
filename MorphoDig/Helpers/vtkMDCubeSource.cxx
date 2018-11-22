/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkMDCubeSource.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkMDCubeSource.h"

#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"

#include <cmath>

vtkStandardNewMacro(vtkMDCubeSource);

vtkMDCubeSource::vtkMDCubeSource(double xL, double yL, double zL)
{
  this->XLength = fabs(xL);
  this->YLength = fabs(yL);
  this->ZLength = fabs(zL);

  this->Center[0] = 0.0;
  this->Center[1] = 0.0;
  this->Center[2] = 0.0;

  this->OutputPointsPrecision = SINGLE_PRECISION;

  this->SetNumberOfInputPorts(0);
}

int vtkMDCubeSource::RequestData(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **vtkNotUsed(inputVector),
  vtkInformationVector *outputVector)
{
  // get the info object
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // get the ouptut
  vtkPolyData *output = vtkPolyData::SafeDownCast(
    outInfo->Get(vtkDataObject::DATA_OBJECT()));

  double x[3], n[3], tc[3];
  int numPolys=24, numPts=30;
  int i, j, k;
  vtkIdType pts[4];
  vtkPoints *newPoints;
  vtkCellArray *newPolys;

//
// Set things up; allocate memory
//
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
 
 

  newPolys = vtkCellArray::New();
  newPolys->Allocate(newPolys->EstimateSize(numPolys,24));
//
// Generate points and normals
//

  for (x[0]=this->Center[0]-this->XLength/2.0, i=0;  i<2;
	  i++, x[0]+=this->XLength)
  {
    for (x[1]=this->Center[1]-this->YLength/2.0, j=0; j<2;
    j++, x[1]+=this->YLength)
    {      
      for (x[2]=this->Center[2]-this->ZLength/2.0, k=0; k<2;
      k++, x[2]+=this->ZLength)
      {       
        newPoints->InsertNextPoint(x);
       
      }
    }
  }
  x[0] = this->Center[0] - this->XLength / 2.0;
  x[1] = this->Center[1];
  x[2] = this->Center[2];
  newPoints->InsertNextPoint(x);
  x[0] = this->Center[0] + this->XLength / 2.0;
  newPoints->InsertNextPoint(x);

 /* pts[0] = 0; pts[1] = 1; pts[2] = 3; pts[3] = 2;
  newPolys->InsertNextCell(4,pts);
  pts[0] = 4; pts[1] = 6; pts[2] = 7; pts[3] = 5;
  newPolys->InsertNextCell(4,pts);*/

  // 0 1 3 2 => 0 1 8; 1 3 8; 3 2 8 ; 2 0 8  
  pts[0] = 0; pts[1] = 1; pts[2] = 8;
  newPolys->InsertNextCell(3, pts);
  pts[0] = 1; pts[1] = 3; pts[2] = 8;
  newPolys->InsertNextCell(3, pts);
  pts[0] = 3; pts[1] = 2; pts[2] = 8;
  newPolys->InsertNextCell(3, pts);
  pts[0] = 2; pts[1] = 0; pts[2] = 8;
  newPolys->InsertNextCell(3, pts);

  // 4 6 7 5 => 4 6 9; 6 7 9; 7 5 9 ; 5 4 9
  pts[0] = 4; pts[1] = 6; pts[2] = 9;
  newPolys->InsertNextCell(3, pts);
  pts[0] = 6; pts[1] = 7; pts[2] = 9;
  newPolys->InsertNextCell(3, pts);
  pts[0] = 7; pts[1] = 5; pts[2] = 9;
  newPolys->InsertNextCell(3, pts);
  pts[0] = 5; pts[1] = 4; pts[2] = 9;
  newPolys->InsertNextCell(3, pts);

  for (x[1]=this->Center[1]-this->YLength/2.0, i=0;
  i<2; i++, x[1]+=this->YLength)
  {
    for (x[0]=this->Center[0]-this->XLength/2.0, j=0; j<2;
    j++, x[0]+=this->XLength)
    {      
      for (x[2]=this->Center[2]-this->ZLength/2.0, k=0; k<2;
      k++, x[2]+=this->ZLength)
      {        
        newPoints->InsertNextPoint(x);     
      }
    }
  }
  x[0] = this->Center[0];
  x[1] = this->Center[1] - this->YLength / 2.0;
  x[2] = this->Center[2];
  newPoints->InsertNextPoint(x);
  x[1] = this->Center[1] + this->YLength / 2.0;
  newPoints->InsertNextPoint(x);

 
  // 10 12 13 11 => 10 12 18; 12 13 18; 13 11 18 ; 11 10 18  
  pts[0] =10; pts[1] = 12; pts[2] = 18;
  newPolys->InsertNextCell(3, pts);
  pts[0] = 12; pts[1] = 13; pts[2] = 18;
  newPolys->InsertNextCell(3, pts);
  pts[0] = 13; pts[1] = 11; pts[2] = 18;
  newPolys->InsertNextCell(3, pts);
  pts[0] = 11; pts[1] = 10; pts[2] = 18;
  newPolys->InsertNextCell(3, pts);

  // 14 15 17 16 => 14 15 19; 15 17 19; 17 16 19 ; 16 14 19
  pts[0] = 14; pts[1] = 15; pts[2] = 19;
  newPolys->InsertNextCell(3, pts);
  pts[0] = 15; pts[1] = 17; pts[2] = 19;
  newPolys->InsertNextCell(3, pts);
  pts[0] = 17; pts[1] = 16; pts[2] = 19;
  newPolys->InsertNextCell(3, pts);
  pts[0] = 16; pts[1] = 14; pts[2] = 19;
  newPolys->InsertNextCell(3, pts);

 /* pts[0] = 8; pts[1] = 10; pts[2] = 11; pts[3] = 9;
  newPolys->InsertNextCell(4,pts);
  pts[0] = 12; pts[1] = 13; pts[2] = 15; pts[3] = 14;
  newPolys->InsertNextCell(4,pts);*/

  for (x[2]=this->Center[2]-this->ZLength/2.0, n[2]=(-1.0), n[0]=n[1]=0.0, i=0;
  i<2; i++, x[2]+=this->ZLength, n[2]+=2.0)
  {
    for (x[1]=this->Center[1]-this->YLength/2.0, j=0; j<2;
    j++, x[1]+=this->YLength)
    {
      tc[1] = x[1] + 0.5;
      for (x[0]=this->Center[0]-this->XLength/2.0, k=0; k<2;
      k++, x[0]+=this->XLength)
      {
        tc[0] = ( x[0] + 0.5 ) * ( 2*i - 1 );
        newPoints->InsertNextPoint(x);     
      }
    }
  }
  x[0] = this->Center[0];
  x[1] = this->Center[1] ;
  x[2] = this->Center[2] - this->ZLength / 2.0;
  newPoints->InsertNextPoint(x);
  x[2] = this->Center[2] + this->ZLength / 2.0;
  newPoints->InsertNextPoint(x);


  // 20 22 23 21 => 20 22 28; 22 23 28; 23 21 28 ; 21 20 28  
  pts[0] = 20; pts[1] = 22; pts[2] = 28;
  newPolys->InsertNextCell(3, pts);
  pts[0] = 22; pts[1] = 23; pts[2] = 28;
  newPolys->InsertNextCell(3, pts);
  pts[0] = 23; pts[1] = 21; pts[2] = 28;
  newPolys->InsertNextCell(3, pts);
  pts[0] = 21; pts[1] = 20; pts[2] = 28;
  newPolys->InsertNextCell(3, pts);

  // 24 25 27 26 => 24 25 29; 25 27 29; 27 26 29 ; 26 24 29
  pts[0] = 24; pts[1] = 25; pts[2] = 29;
  newPolys->InsertNextCell(3, pts);
  pts[0] = 25; pts[1] = 27; pts[2] = 29;
  newPolys->InsertNextCell(3, pts);
  pts[0] = 27; pts[1] = 26; pts[2] = 29;
  newPolys->InsertNextCell(3, pts);
  pts[0] = 26; pts[1] = 24; pts[2] = 29;
  newPolys->InsertNextCell(3, pts);

  /*pts[0] = 16; pts[1] = 18; pts[2] = 19; pts[3] = 17;
  newPolys->InsertNextCell(4,pts);
  pts[0] = 20; pts[1] = 21; pts[2] = 23; pts[3] = 22;
  newPolys->InsertNextCell(4,pts);*/


//
// Update ourselves and release memory
//
  output->SetPoints(newPoints);
  newPoints->Delete();

 
 

 
 

  newPolys->Squeeze(); // since we've estimated size; reclaim some space
  output->SetPolys(newPolys);
  newPolys->Delete();

  return 1;
}

// Convenience method allows creation of cube by specifying bounding box.
void vtkMDCubeSource::SetBounds(double xMin, double xMax,
                              double yMin, double yMax,
                              double zMin, double zMax)
{
  double bounds[6];
  bounds[0] = xMin;
  bounds[1] = xMax;
  bounds[2] = yMin;
  bounds[3] = yMax;
  bounds[4] = zMin;
  bounds[5] = zMax;
  this->SetBounds (bounds);
}

void vtkMDCubeSource::SetBounds(const double bounds[6])
{
  this->SetXLength(bounds[1]-bounds[0]);
  this->SetYLength(bounds[3]-bounds[2]);
  this->SetZLength(bounds[5]-bounds[4]);

  this->SetCenter((bounds[1]+bounds[0])/2.0, (bounds[3]+bounds[2])/2.0,
                  (bounds[5]+bounds[4])/2.0);
}

void vtkMDCubeSource::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "X Length: " << this->XLength << "\n";
  os << indent << "Y Length: " << this->YLength << "\n";
  os << indent << "Z Length: " << this->ZLength << "\n";
  os << indent << "Center: (" << this->Center[0] << ", "
               << this->Center[1] << ", " << this->Center[2] << ")\n";
  os << indent << "Output Points Precision: " << this->OutputPointsPrecision << "\n";
}
