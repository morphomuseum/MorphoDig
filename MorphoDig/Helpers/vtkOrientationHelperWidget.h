/*=========================================================================

  Program:   MorphoDig
  Module:    vtkOrientationHelperWidget.h

=========================================================================*/


#ifndef vtkOrientationHelperWidget_h
#define vtkOrientationHelperWidget_h

//#include "vtkInteractionWidgetsModule.h" // For export macro
#include "vtkOrientationMarkerWidget.h"
#include "vtkOrientationHelperActor.h"

class vtkActor2D;
class vtkPolyData;
class vtkProp;
class vtkOrientationHelperWidgetObserver;
class vtkRenderer;

class  vtkOrientationHelperWidget : public vtkOrientationMarkerWidget
{
public:
	static vtkOrientationHelperWidget* New();
	vtkTypeMacro(vtkOrientationHelperWidget, vtkOrientationMarkerWidget);
	
	virtual void SetEnabled(int);
  void ExecuteCameraUpdateEventHelper(vtkObject *o, unsigned long event, void *calldata);

protected:
	vtkOrientationHelperWidget();
	~vtkOrientationHelperWidget();
	// observer to update the renderer's camera
	vtkOrientationHelperWidgetObserver *Observer;
private:
	
};

#endif
