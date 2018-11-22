/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkMDCylinderSource.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkMDCylinderSource
 * @brief   generate a cylinder centered at origin
 *
 * vtkMDCylinderSource creates a polygonal cylinder centered at Center;
 * The axis of the cylinder is aligned along the global y-axis.
 * The height and radius of the cylinder can be specified, as well as the
 * number of sides. It is also possible to control whether the cylinder is
 * open-ended or capped. If you have the end points of the cylinder, you
 * should use a vtkLineSource followed by a vtkTubeFilter instead of the
 * vtkMDCylinderSource.
*/

#ifndef vtkMDCylinderSource_h
#define vtkMDCylinderSource_h

#include "vtkFiltersSourcesModule.h" // For export macro
#include "vtkPolyDataAlgorithm.h"

#include "vtkCell.h" // Needed for VTK_CELL_SIZE

class vtkMDCylinderSource : public vtkPolyDataAlgorithm
{
public:
  static vtkMDCylinderSource *New();
  vtkTypeMacro(vtkMDCylinderSource,vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //@{
  /**
   * Set the height of the cylinder. Initial value is 1.
   */
  vtkSetClampMacro(Height,double,0.0,VTK_DOUBLE_MAX)
  vtkGetMacro(Height,double);
  //@}

  //@{
  /**
   * Set the radius of the cylinder. Initial value is 0.5
   */
  vtkSetClampMacro(Radius,double,0.0,VTK_DOUBLE_MAX)
  vtkGetMacro(Radius,double);
  //@}

  /**
  * Set the radius of the ellipse. Initial value is 0.5
  */
  vtkSetClampMacro(Radius2, double, 0.0, VTK_DOUBLE_MAX)
	  vtkGetMacro(Radius2, double);
  //@}

  //@{
  /**
   * Set/Get cylinder center. Initial value is (0.0,0.0,0.0)
   */
  vtkSetVector3Macro(Center,double);
  vtkGetVectorMacro(Center,double,3);
  //@}

  //@{
  /**
   * Set the number of facets used to define cylinder. Initial value is 6.
   */
  vtkSetClampMacro(Resolution,int,2,VTK_CELL_SIZE)
  vtkGetMacro(Resolution,int);

  //@}
  vtkSetClampMacro(ConeHeight, int, 0, 100)
	  vtkGetMacro(ConeHeight, int);
  

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
  vtkMDCylinderSource(int res=6);
  ~vtkMDCylinderSource() override {}

  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
  double Height;
  double Radius;
  double Radius2;
  double Center[3];
  int Resolution;
  int ConeHeight;//in % of shaft length
  int OutputPointsPrecision;

private:
  vtkMDCylinderSource(const vtkMDCylinderSource&) = delete;
  void operator=(const vtkMDCylinderSource&) = delete;
};

#endif
