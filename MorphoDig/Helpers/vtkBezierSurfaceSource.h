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

#ifndef VTK_BEZIER_SURFACE_SOURCE_H
#define VTK_BEZIER_SURFACE_SOURCE_H

#include "vtkPolyDataAlgorithm.h"

class vtkImageData;
class vtkTransform;

class vtkBezierSurfaceSource : public vtkPolyDataAlgorithm
{
public:
  static vtkBezierSurfaceSource *New();
  vtkTypeMacro(vtkBezierSurfaceSource, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  vtkPolyData* GetControlPointsOutput();
  vtkPolyData* GetBezierSurfaceOutput(); // same as GetOutput()

  void SetNumberOfControlPoints(int m, int n);
  int* GetNumberOfControlPoints() { return this->NumberOfControlPoints; }

  void SetControlPoint(int m, int n, double pt[3]);
  void GetControlPoint(int m, int n, double pt[3]);
  //double* GetControlPoint(int m, int n);
  void ResetControlPoints();

  void SetDimensions(int x, int y);
  int* GetDimensions() { return this->Dimensions; }

  void SetTransform(vtkTransform *transform);
  vtkTransform* GetTransform() { return this->Transform; }

protected:
  vtkBezierSurfaceSource();
  ~vtkBezierSurfaceSource();
  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
  vtkBezierSurfaceSource(const vtkBezierSurfaceSource&);  // Not implemented.
  void operator=(const vtkBezierSurfaceSource&);  // Not implemented.

  void UpdateControlPointsPolyData(vtkPolyData* pd);
  void UpdateBezierSurfacePolyData(vtkPolyData* pd);

private:
  int NumberOfControlPoints[2];
  int Dimensions[2];
  double* ControlPoints;
  vtkTransform *Transform;
  vtkTransform *InverseTransform;
};

#endif
