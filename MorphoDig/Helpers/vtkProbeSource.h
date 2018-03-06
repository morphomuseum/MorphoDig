/*=========================================================================

  Program:  MorphoDig
  Module:    Modified from VTK vtkProbeSource.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkProbeSource - Appends a cylinder to a cone to form an arrow.
// .SECTION Description
// vtkProbeSource was intended to be used as the source for a glyph.
// The shaft base is always at (0,0,0). The arrow tip is always at (size,0,0). If
// "Invert" is true, then the ends are flipped i.e. tip is at (0,0,0) while
// base is at (size, 0, 0).
// The resolution of the cone and shaft can be set and default to 6.
// The radius of the cone and shaft can be set and default to 0.03 and 0.1.
// The length of the tip can also be set, and defaults to 0.35.


#ifndef vtkProbeSource_h
#define vtkProbeSource_h


#include "vtkPolyDataAlgorithm.h"

class  vtkProbeSource : public vtkPolyDataAlgorithm
{
public:
  // Description
  // Construct cone with angle of 45 degrees.
  static vtkProbeSource *New();

  vtkTypeMacro(vtkProbeSource,vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set the length, and radius of the tip.  They default to 0.35 and 0.1
  vtkSetClampMacro(TipLength,double,0.0,1.0);
  vtkGetMacro(TipLength,double);
  vtkSetClampMacro(TipRadius,double,0.0,10.0);
  vtkGetMacro(TipRadius,double);

  
  vtkSetClampMacro(ArrowLength, double, 0.0, VTK_DOUBLE_MAX);
  vtkGetMacro(ArrowLength, double);

  // Description:
  // Set the resolution of the tip.  The tip behaves the same as a cone.
  // Resoultion 1 gives a single triangle, 2 gives two crossed triangles.
  vtkSetClampMacro(TipResolution,int,1,128);
  vtkGetMacro(TipResolution,int);

  // Description:
  // Set the radius of the shaft.  Defaults to 0.03.
  vtkSetClampMacro(ShaftRadius,double,0.0,5.0);
  vtkGetMacro(ShaftRadius,double);

  // Description:
  // Set the resolution of the shaft.  2 gives a rectangle.
  // I would like to extend the cone to produce a line,
  // but this is not an option now.
  vtkSetClampMacro(ShaftResolution,int,0,128);
  vtkGetMacro(ShaftResolution,int);

  // Description:
  // Inverts the Probe direction. When set to true, base is at (1, 0, 0) while the
  // tip is at (0, 0, 0). The default is false, i.e. base at (0, 0, 0) and the tip
  // at (1, 0, 0).
  vtkBooleanMacro(Invert, bool);
  vtkSetMacro(Invert, bool);
  vtkGetMacro(Invert, bool);

protected:
  vtkProbeSource();
  ~vtkProbeSource() {}

  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

  int TipResolution;
  int TipType;
  double TipLength;
  double TipRadius;
  double ArrowLength;
  int ShaftResolution;
  double ShaftRadius;
  bool Invert;


private:
  vtkProbeSource(const vtkProbeSource&); // Not implemented.
  void operator=(const vtkProbeSource&); // Not implemented.
};

#endif


