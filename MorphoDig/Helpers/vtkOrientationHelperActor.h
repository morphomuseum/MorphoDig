/*=========================================================================

Program:   MorphoDig
Module:    vtkOrientationHelperActor.h
=========================================================================*/
// .NAME vtkOrientationHelperActor - a 3D orientation helper based on vtkAxesActor
// vtkOrientationHelperActor is a hybrid 2D/3D actor used to represent 3D orientation helper in a scene.
// The user can define the geometry to use for the shaft or the tip, and the
// user can set the text for the six axes (-x, x, -y, y, -z, z). 

#ifndef vtkOrientationHelperActor_h
#define vtkOrientationHelperActor_h

//#include "vtkRenderingAnnotationModule.h" // For export macro
#include <vtkAxesActor.h>

class vtkActor;
class vtkCaptionActor2D;
class vtkConeSource;
class vtkCylinderSource;
class vtkLineSource;
class vtkPolyData;
class vtkPropCollection;
class vtkProperty;
class vtkRenderer;
class vtkSphereSource;

//class VTKRENDERINGANNOTATION_EXPORT vtkOrientationHelperActor : public vtkProp3D
class  vtkOrientationHelperActor : public vtkAxesActor
{
public:
	static vtkOrientationHelperActor *New();
	vtkTypeMacro(vtkOrientationHelperActor, vtkAxesActor);
	void PrintSelf(ostream& os, vtkIndent indent);

	// Description:
	// For some exporters and other other operations we must be
	// able to collect all the actors or volumes. These methods
	// are used in that process.
	virtual void GetActors(vtkPropCollection *);

	// Description:
	// Support the standard render methods.
	virtual int RenderOpaqueGeometry(vtkViewport *viewport);
	virtual int RenderTranslucentPolygonalGeometry(vtkViewport *viewport);
	virtual int RenderOverlay(vtkViewport *viewport);

	// Description:
	// Does this prop have some translucent polygonal geometry?
	virtual int HasTranslucentPolygonalGeometry();

	// Description:
	// Shallow copy of an axes actor. Overloads the virtual vtkProp method.
	void ShallowCopy(vtkProp *prop);

	// Description:
	// Release any graphics resources that are being consumed by this actor.
	// The parameter window could be used to determine which graphic
	// resources to release.
	void ReleaseGraphicsResources(vtkWindow *);

	

	vtkProperty *GetX2AxisTipProperty();
	vtkProperty *GetY2AxisTipProperty();
	vtkProperty *GetZ2AxisTipProperty();


	vtkProperty *GetX2AxisShaftProperty();
	vtkProperty *GetY2AxisShaftProperty();
	vtkProperty *GetZ2AxisShaftProperty();


	vtkCaptionActor2D *GetX2AxisCaptionActor2D()
	{
		return this->X2AxisLabel;
	}

	vtkCaptionActor2D *GetY2AxisCaptionActor2D()
	{
		return this->Y2AxisLabel;
	}
	
	vtkCaptionActor2D *GetZ2AxisCaptionActor2D()
	{
		return this->Z2AxisLabel;
	}
	// Description:
	
	vtkSetStringMacro(X2AxisLabelText);
	vtkGetStringMacro(X2AxisLabelText);
	vtkSetStringMacro(Y2AxisLabelText);
	vtkGetStringMacro(Y2AxisLabelText);
	vtkSetStringMacro(Z2AxisLabelText);
	vtkGetStringMacro(Z2AxisLabelText)

	// Description:
	// Enable/disable drawing the X, Y, and Z axis labels.
	vtkSetMacro(XAxisLabels, int);
	vtkGetMacro(XAxisLabels, int);
	vtkBooleanMacro(XAxisLabels, int);
	vtkSetMacro(YAxisLabels, int);
	vtkGetMacro(YAxisLabels, int);
	vtkBooleanMacro(YAxisLabels, int);

	vtkSetMacro(ZAxisLabels, int);
	vtkGetMacro(ZAxisLabels, int);
	vtkBooleanMacro(ZAxisLabels, int);

protected:
	vtkOrientationHelperActor();
	~vtkOrientationHelperActor();


	vtkActor          *X2AxisShaft;
	vtkActor          *Y2AxisShaft;
	vtkActor          *Z2AxisShaft;



	vtkActor          *X2AxisTip;
	vtkActor          *Y2AxisTip;
	vtkActor          *Z2AxisTip;


	void               UpdateProps();

	

	char              *X2AxisLabelText;
	char              *Y2AxisLabelText;
	char              *Z2AxisLabelText;


	vtkCaptionActor2D *X2AxisLabel;
	vtkCaptionActor2D *Y2AxisLabel;
	vtkCaptionActor2D *Z2AxisLabel;

	int                XAxisLabels;
	int                YAxisLabels;
	int                ZAxisLabels;



private:
	vtkOrientationHelperActor(const vtkOrientationHelperActor&);  // Not implemented.
	void operator=(const vtkOrientationHelperActor&);  // Not implemented.
};

#endif

