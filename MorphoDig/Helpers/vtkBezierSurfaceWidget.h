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
    Brian Gee Chacko (brian.chacko@vcreatelogic.com)

Contributors:
    Rafael Palomar (rafaelpalomaravalos@gmail.com)

*/

#ifndef VTK_BEZIER_SURFACE_WIDGET_H
#define VTK_BEZIER_SURFACE_WIDGET_H

#include "vtk3DWidget.h"
#include "vtkObjectFactory.h"
#include "vtkSmartPointer.h"
#include <vector>

class vtkActor;
class vtkProp3D;
class vtkDataSet;
class vtkProperty;
class vtkPropPicker;
class vtkPolyDataMapper;
class vtkBezierSurfaceSource;
struct HandleInfo;

class vtkBezierSurfaceWidget : public vtk3DWidget
{
public:
  static vtkBezierSurfaceWidget *New();

  vtkTypeMacro(vtkBezierSurfaceWidget, vtk3DWidget);
  void PrintSelf(ostream& os, vtkIndent indent);

  void SetBezierSource(vtkBezierSurfaceSource* bezierSource);
  vtkBezierSurfaceSource* GetBezierSource();

  void SetProperty(vtkProperty* property);
  vtkProperty* GetProperty();

  // Implementing the vtk3DWidget interface.
  void SetInteractor(vtkRenderWindowInteractor* iren);
  void SetProp3D(vtkProp3D* prop);
  void SetInput(vtkDataSet* dataSet);
  void SetEnabled(int val);
  void PlaceWidget(double bounds[6]); // This is not implemented, but required by the vtk3DWidget interface
  void SetPlaceFactor(double val);
  void SetHandleSize(double size);
  double GetHandleSize() { return this->HandleSize; }

protected:
  vtkBezierSurfaceWidget();
  ~vtkBezierSurfaceWidget();

  vtkRenderer* GetRenderer();
  void DestroyHandles();
  void SizeHandles();
  void ConstructHandles();
  void SelectHandle(int index);
  void UnSelectCurrentHandle();

  static void ProcessEvents(vtkObject* object, unsigned long event, void* clientdata, void* calldata);

  // ProcessEvents() dispatches to these methods.
  virtual void OnLeftButtonDown();
  virtual void OnMouseMove();
  virtual void OnLeftButtonUp();

private:
  vtkSmartPointer<vtkBezierSurfaceSource> BezierSource;
  std::vector<HandleInfo*> HandleInfoList;
  int CurrHandleIndex;
  vtkSmartPointer<vtkProperty> Property;
  vtkSmartPointer<vtkPropPicker> Picker;
  vtkSmartPointer<vtkPolyDataMapper> CPGridMapper;
  vtkSmartPointer<vtkActor> CPGridActor;
  double HandleSize;

private:
  vtkBezierSurfaceWidget(const vtkBezierSurfaceWidget&);  //Not implemented
  void operator=(const vtkBezierSurfaceWidget&);  //Not implemented
};

#endif
