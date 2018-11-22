/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkMDCubeSource.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkMDCubeSource
 * @brief   create a polygonal representation of a cube
 *
 * vtkMDCubeSource creates a cube centered at origin. The cube is represented
 * with four-sided polygons. It is possible to specify the length, width,
 * and height of the cube independently.
*/

#ifndef vtkMDCubeSource_h
#define vtkMDCubeSource_h

#include "vtkFiltersSourcesModule.h" // For export macro
#include "vtkPolyDataAlgorithm.h"

class  vtkMDCubeSource : public vtkPolyDataAlgorithm
{
public:
  static vtkMDCubeSource *New();
  vtkTypeMacro(vtkMDCubeSource,vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //@{
  /**
   * Set the length of the cube in the x-direction.
   */
  vtkSetClampMacro(XLength,double,0.0,VTK_DOUBLE_MAX);
  vtkGetMacro(XLength,double);
  //@}

  //@{
  /**
   * Set the length of the cube in the y-direction.
   */
  vtkSetClampMacro(YLength,double,0.0,VTK_DOUBLE_MAX);
  vtkGetMacro(YLength,double);
  //@}

  //@{
  /**
   * Set the length of the cube in the z-direction.
   */
  vtkSetClampMacro(ZLength,double,0.0,VTK_DOUBLE_MAX);
  vtkGetMacro(ZLength,double);
  //@}

  //@{
  /**
   * Set the center of the cube.
   */
  vtkSetVector3Macro(Center,double);
  vtkGetVectorMacro(Center,double,3);
  //@}

  //@{
  /**
   * Convenience method allows creation of cube by specifying bounding box.
   */
  void SetBounds(double xMin, double xMax,
                 double yMin, double yMax,
                 double zMin, double zMax);
  void SetBounds(const double bounds[6]);
  //@}

  //@{
  /**
   * Set/get the desired precision for the output points.
   * vtkAlgorithm::SINGLE_PRECISION - Output single-precision floating point.
   * vtkAlgorithm::DOUBLE_PRECISION - Output double-precision floating point.
   */
  vtkSetMacro(OutputPointsPrecision,int);
  vtkGetMacro(OutputPointsPrecision,int);
  //@}

protected:
  vtkMDCubeSource(double xL=1.0, double yL=1.0, double zL=1.0);
  ~vtkMDCubeSource() override {}

  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
  double XLength;
  double YLength;
  double ZLength;
  double Center[3];
  int OutputPointsPrecision;
private:
  vtkMDCubeSource(const vtkMDCubeSource&) = delete;
  void operator=(const vtkMDCubeSource&) = delete;
};

#endif
