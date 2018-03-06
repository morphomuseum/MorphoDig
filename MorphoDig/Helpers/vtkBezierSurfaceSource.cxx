/****************************************************************************
 **
 ** Copyright (C) VCreate Logic Private Limited, Bangalore
 **
 ** Use of this file is limited according to the terms specified by
 ** VCreate Logic Private Limited, Bangalore. Details of those terms
 ** are listed in licence.txt included as part of the distribution package
 ** of this file. This file may not be distributed without including the
 ** licence.txt file.
 **
 ** Contact info@vcreatelogic.com if any conditions of this licensing are
 ** not clear to you.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ****************************************************************************/

/**
   Original Authors:
   Prashanth N Udupa (prashanth@vcreatelogic.com)
   Paul Bourke (paul.bourke@gmail.com)

   Contributors:
   Rafael Palomar (rafaelpalomaravalos@gmail.com)

   Credits:
   The bezier surface algorithm is based on the code and concepts presented in
   this page: "B�zier Surface (in 3D)"
   http://local.wasp.uwa.edu.au/~pbourke/geometry/bezier/index.html

   Credits to Paul Bourke for explaining Bezier surfaces so well.
*/

#include "vtkBezierSurfaceSource.h"

#include <vtkObjectFactory.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>
#include <vtkImageData.h>
#include <vtkExecutive.h>
#include <vtkOutputWindow.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

#include <cmath>


vtkStandardNewMacro(vtkBezierSurfaceSource);

vtkBezierSurfaceSource::vtkBezierSurfaceSource()
{
  this->Dimensions[0] = 10;
  this->Dimensions[1] = 10;

  this->NumberOfControlPoints[0] = 0;
  this->NumberOfControlPoints[1] = 0;
  this->ControlPoints = 0;
  this->SetNumberOfControlPoints(4, 4);

  this->SetNumberOfInputPorts(0);
  this->SetNumberOfOutputPorts(2);

  this->Transform = vtkTransform::New();
  this->Transform->Identity();

  this->InverseTransform = vtkTransform::New();
  this->InverseTransform->Identity();

  vtkPolyData *output2 = vtkPolyData::New();
  this->GetExecutive()->SetOutputData(1, output2);
  output2->Delete();
}

vtkBezierSurfaceSource::~vtkBezierSurfaceSource()
{
  if(this->ControlPoints)
    {
    delete [] this->ControlPoints;
    }

  if(this->Transform)
    {
    this->Transform->Delete();
    }

  if(this->InverseTransform)
    {
    this->InverseTransform->Delete();
    }
}

void vtkBezierSurfaceSource::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkPolyDataAlgorithm::PrintSelf(os, indent);

  os << "Dimensions: " << this->Dimensions[0] << ", " << this->Dimensions[1] << "\n";
  os << "Number of Control Points : " << this->NumberOfControlPoints[0] << ", " << this->NumberOfControlPoints[1] << "\n";

  int index = 0;
  for(int i=0; i<this->NumberOfControlPoints[0]; i++)
    {
    for(int j=0; j<this->NumberOfControlPoints[1]; j++)
      {
        double cpt[4];
        cpt[0] = this->ControlPoints[index*3];
        cpt[1] = this->ControlPoints[index*3+1];
        cpt[2] = this->ControlPoints[index*3+2];
        cpt[3] = 1.0;

        this->Transform->MultiplyPoint(cpt,cpt);
        os << "Control Point[" << i << ", " << j << "] = " << cpt[0] << ", " << cpt[1] << ", " << cpt[2] << "\n";
        ++index;
      }
    os << "\n";
    }
}

vtkPolyData* vtkBezierSurfaceSource::GetControlPointsOutput()
{
  if (this->GetNumberOfOutputPorts() < 2)
    {
    return NULL;
    }
  return vtkPolyData::SafeDownCast( this->GetExecutive()->GetOutputData(1) );
}

vtkPolyData* vtkBezierSurfaceSource::GetBezierSurfaceOutput()
{
  return vtkPolyData::SafeDownCast( this->GetExecutive()->GetOutputData(0) );
}

void vtkBezierSurfaceSource::SetNumberOfControlPoints(int m, int n)
{
  if( this->NumberOfControlPoints[0] == m && this->NumberOfControlPoints[1] == n )
    {
    return;
    }

  this->NumberOfControlPoints[0] = m;
  this->NumberOfControlPoints[1] = n;

  if(this->NumberOfControlPoints[0] < 2)
    {
    this->NumberOfControlPoints[0] = 2;
    }
  if(this->NumberOfControlPoints[1] < 2)
    {
    this->NumberOfControlPoints[1] = 2;
    }

  if(this->ControlPoints)
    {
    delete [] this->ControlPoints;
    }

  this->ControlPoints = new double[m*n*3];
  this->ResetControlPoints();
}

void vtkBezierSurfaceSource::SetControlPoint(int m, int n, double pt[3])
{
  if(m < 0 || m >= this->NumberOfControlPoints[0])
    {
    return;
    }

  if(n < 0 || n >= this->NumberOfControlPoints[1])
    {
    return;
    }

  int index = n + m*this->NumberOfControlPoints[0];
  double cpt[4];
  cpt[0] = pt[0];
  cpt[1] = pt[1];
  cpt[2] = pt[2];
  cpt[3] = 1.0;

  this->InverseTransform->MultiplyPoint(cpt,cpt);

  this->ControlPoints[index*3]=cpt[0];
  this->ControlPoints[index*3+1]=cpt[1];
  this->ControlPoints[index*3+2]=cpt[2];

  this->Modified();
}

void vtkBezierSurfaceSource::GetControlPoint(int m, int n, double pt[3])
{
  if(m < 0 || m >= this->NumberOfControlPoints[0])
    {
    return;
    }

  if(n < 0 || n >= this->NumberOfControlPoints[1])
    {
    return;
    }

  int index = n + m*this->NumberOfControlPoints[0];
  double cpt[4];
  cpt[0] = this->ControlPoints[index*3];
  cpt[1] = this->ControlPoints[index*3+1];
  cpt[2] = this->ControlPoints[index*3+2];
  cpt[3] = 1.0;

  this->Transform->MultiplyPoint(cpt,cpt);
  pt[0] = cpt[0];
  pt[1] = cpt[1];
  pt[2] = cpt[2];
}

void vtkBezierSurfaceSource::ResetControlPoints()
{
  // Lets create control points at a default distance of 0.1 units from each other.
  // 0.1 units along both m and n.
  int m = this->NumberOfControlPoints[0];
  int n = this->NumberOfControlPoints[1];
  double distx = 1.0 / (m-1);
  double disty = 1.0 / (n-1);
  double minx = -0.5;
  double miny = -0.5;
  int index = 0;
  for(int i=0; i<m; i++)
    {
    for(int j=0; j<n; j++)
      {
      double* pt = this->ControlPoints + (index*3);
      pt[0] = minx + i*distx;
      pt[1] = miny + j*disty;
      pt[2] = 0;
      ++index;
      }
    }

  this->Modified();
}

void vtkBezierSurfaceSource::SetDimensions(int x, int y)
{
  if(x == this->Dimensions[0] && y == this->Dimensions[1])
    {
    return;
    }

  this->Dimensions[0] = x;
  this->Dimensions[1] = y;

  this->Modified();
}

void vtkBezierSurfaceSource::SetTransform(vtkTransform *transform)
{
  if(transform)
    {
    this->Transform->DeepCopy(transform);
    this->InverseTransform->DeepCopy(transform);
    this->InverseTransform->Inverse();
    }

  this->Modified();
}

int vtkBezierSurfaceSource::RequestData(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **vtkNotUsed(inputVector),
  vtkInformationVector *outputVector)
{
  vtkInformation *cpOutInfo = outputVector->GetInformationObject(1);
  if(cpOutInfo)
    {
    vtkPolyData *cpOutput = vtkPolyData::SafeDownCast(cpOutInfo->Get(vtkDataObject::DATA_OBJECT()));
    this->UpdateControlPointsPolyData(cpOutput);
    }

  vtkInformation *bsOutInfo = outputVector->GetInformationObject(0);
  if(bsOutInfo)
    {
    vtkPolyData *bsOutput = vtkPolyData::SafeDownCast(bsOutInfo->Get(vtkDataObject::DATA_OBJECT()));
    this->UpdateBezierSurfacePolyData(bsOutput);
    }

  return 1;
}

void vtkBezierSurfaceSource::UpdateControlPointsPolyData(vtkPolyData* pd)
{
  if(!pd)
    {
    return;
    }

  // Create points array (geometry)
  vtkSmartPointer<vtkPoints> points =
    vtkSmartPointer<vtkPoints>::New();

  int nrPoints = this->NumberOfControlPoints[0]*this->NumberOfControlPoints[1];
  points->SetNumberOfPoints(nrPoints);

  for(int i=0; i<nrPoints; i++)
    {
    double pt[4];
    pt[0] = this->ControlPoints[i*3];
    pt[1] = this->ControlPoints[i*3+1];
    pt[2] = this->ControlPoints[i*3+2];
    pt[3] = 1.0;

    this->Transform->MultiplyPoint(pt,pt);
    points->SetPoint(i, pt[0], pt[1], pt[2]);
    }

  pd->SetPoints(points);

  // Create quads-segment array (topology)
  int grid_x = this->NumberOfControlPoints[0];
  int grid_y = this->NumberOfControlPoints[1];

  vtkSmartPointer<vtkCellArray> cells =
    vtkSmartPointer<vtkCellArray>::New();

  for(int i=0; i<grid_x-1; i++)
    {
    for(int j=0; j<grid_y-1; j++)
      {
      int base = j*grid_x + i;
      int a = base;
      int b = base+1;
      int c = base+grid_x+1;
      int d = base+grid_x;

      vtkIdType ls[2];

      ls[0] = a; ls[1] = b;
      cells->InsertNextCell(2, ls);

      ls[0] = b; ls[1] = c;
      cells->InsertNextCell(2, ls);

      ls[0] = c; ls[1] = d;
      cells->InsertNextCell(2, ls);

      ls[0] = d; ls[1] = a;
      cells->InsertNextCell(2, ls);
      }
    }
  pd->SetLines(cells);

}

/*
  The bezier surface algorithm is based on the code and concepts presented in
  this page: "B�zier Surface (in 3D)"
  http://local.wasp.uwa.edu.au/~pbourke/geometry/bezier/index.html

  Credits to Paul Bourke for explaining Bezier surfaces so well.
*/
// Methods used while computing the bezier surface
double BezierBlend(int k, double mu, int n);
void EvalBezierSurface(const double* controlPoints, int m, int n, int dimx, int dimy, vtkPoints* surfacePoints);

void vtkBezierSurfaceSource::UpdateBezierSurfacePolyData(vtkPolyData* pd)
{
  if(!pd)
    {
    return;
    }

  // First construct a grid.
  // Construct the basic grid
  int grid_x = Dimensions[0];
  int grid_y = Dimensions[1];

  vtkSmartPointer<vtkPoints> points =
    vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkDoubleArray> tcoords =
    vtkSmartPointer<vtkDoubleArray>::New();

  points->SetNumberOfPoints(grid_x*grid_y);
  tcoords->SetNumberOfComponents(2);
  tcoords->SetNumberOfTuples(grid_x*grid_y);

  for(int i=0; i<grid_x; i++)
    {
    for(int j=0; j<grid_y; j++)
      {
      // Points need not be computed, because the EvalBezierSurface()
      // method does it for us.

      double s = double(i)/double(grid_x);
      double t = double(j)/double(grid_y);
      tcoords->SetTuple2(j*grid_x+i, s, t);
      }
    }

  // Now evaluate the bezier curve on the grid.
  int m = this->NumberOfControlPoints[0];
  int n = this->NumberOfControlPoints[1];
  int dimx = this->Dimensions[0];
  int dimy = this->Dimensions[1];
  EvalBezierSurface(this->ControlPoints, m, n, dimx, dimy, points);

  for(int i=0; i<points->GetNumberOfPoints(); i++)
    {
    double *point = points->GetPoint(i);
    double pt[4];
    pt[0] = point[0];
    pt[1] = point[1];
    pt[2] = point[2];
    pt[3] = 1.0;

    this->Transform->MultiplyPoint(pt,pt);
    points->SetPoint(i, pt[0],pt[1],pt[2]);
    }

  // Set the points into the output polydata.
  pd->SetPoints(points);

  pd->GetPointData()->SetTCoords(tcoords);

  vtkSmartPointer<vtkCellArray> cells =
    vtkSmartPointer<vtkCellArray>::New();

  for(int i=0; i<grid_x-1; i++)
    {
    for(int j=0; j<grid_y-1; j++)
      {
      int base = j*grid_x + i;
      int a = base;
      int b = base+1;
      int c = base+grid_x+1;
      int d = base+grid_x;

      vtkIdType tgl[3];
      tgl[2] = a; tgl[1] = b; tgl[0] = c;
      cells->InsertNextCell(3, tgl);
      tgl[2] = a; tgl[1] = c; tgl[0] = d;
      cells->InsertNextCell(3, tgl);
      }
    }
  pd->SetStrips(cells);

}

// This function computes the blending function as used in the B�zier
// surface code above. It is written for clarity, not efficiency.
// Normally, if the number of control points is constant, the blending
// function would be calculated once for each desired value of mu.
double BezierBlend(int k, double mu, int n)
{
  int nn, kn, nkn;
  double blend=1;

  nn = n;
  kn = k;
  nkn = n - k;

  while (nn >= 1)
    {
    blend *= nn;
    nn--;
    if (kn > 1)
      {
      blend /= (double)kn;
      kn--;
      }
    if (nkn > 1)
      {
      blend /= (double)nkn;
      nkn--;
      }
    }

  if (k > 0)
    {
    blend *= pow(mu,(double)k);
    }

  if (n-k > 0)
    {
    blend *= pow(1-mu,(double)(n-k));
    }

  return blend;
}

// Evaluates a bezier surface.
void EvalBezierSurface(const double* controlPoints, int m, int n, int dimx, int dimy, vtkPoints* surfacePoints)
{
  int i, j, ki, kj;
  double mui, muj, bi, bj;

  int ptIndex = 0;
  int ctrlPtIndex = 0;
  double pt[3];

  for(i=0; i<dimx; i++)
    {
    mui = i / (double)(dimx-1);
    for(j=0; j<dimy; j++)
      {
      muj = j / (double)(dimy-1);

      // Get the surface point and initialize it.
      surfacePoints->GetPoint(ptIndex, pt);
      pt[0] = pt[1] = pt[2] = 0;

      // Loop through all control points and compute its effect on the
      // surface point
      ctrlPtIndex = 0;
      for (ki=0; ki<m; ki++)
        {
        bi = BezierBlend(ki,mui,m-1);

        for (kj=0; kj<n; kj++)
          {
          bj = BezierBlend(kj,muj,n-1);

          const double* ctrlPt = controlPoints + (ctrlPtIndex*3);
          ++ctrlPtIndex;

          pt[0] += (ctrlPt[0] * bi * bj);
          pt[1] += (ctrlPt[1] * bi * bj);
          pt[2] += (ctrlPt[2] * bi * bj);
          }
        }

      surfacePoints->SetPoint(ptIndex, pt);
      ++ptIndex;
      }
    }
}
