/*=========================================================================

Program:   MorphoDig
Module:    vtkGridActor.h



=========================================================================*/


#ifndef vtkGridActor_h
#define vtkGridActor_h


#include "vtkProp3D.h"

class vtkActor;
//class vtkCaptionActor2D;
class vtkBillboardTextActor3D;
class vtkConeSource;
class vtkCylinderSource;
class vtkLineSource;
class vtkPolyData;
class vtkPropCollection;
class vtkProperty;
class vtkRenderer;


class  vtkGridActor : public vtkProp3D
{
public:
	static vtkGridActor *New();
	vtkTypeMacro(vtkGridActor, vtkProp3D);
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

	// Description:
	// Get the bounds for this Actor as (Xmin,Xmax,Ymin,Ymax,Zmin,Zmax). (The
	// method GetBounds(double bounds[6]) is available from the superclass.)
	void GetBounds(double bounds[6]);
	double *GetBounds();

	void SetGridOrigin(double origin[3]);
	void GetGridOrigin(double origin[3]);
	double * GetGridOrigin();


	// Description:
	// Get the actors mtime plus consider its properties and texture if set.

#if VTK_MAJOR_VERSION >= 8 || VTK_MINOR_VERSION >= 1

	/**
	* Get the actors mtime plus consider its properties and texture if set.
	*/
	vtkMTimeType GetMTime();

	/**
	* Return the mtime of anything that would cause the rendered image to
	* appear differently. Usually this involves checking the mtime of the
	* prop plus anything else it depends on such as properties, textures
	* etc.
	*/
	virtual vtkMTimeType GetRedrawMTime();

#else
	unsigned long int GetMTime();

	// Description:
	// Return the mtime of anything that would cause the rendered image to
	// appear differently. Usually this involves checking the mtime of the
	// prop plus anything else it depends on such as properties, textures
	// etc.

	virtual unsigned long GetRedrawMTime();
#endif







	// Description:
	// Retrieve handles to the X, Y and Z axis (so that you can set their text
	// properties for example)
	vtkBillboardTextActor3D *GetFirstAxisCaptionActor2D()
	{
		return this->FirstAxisLabel;
	}
	vtkBillboardTextActor3D *GetSecondAxisCaptionActor2D()
	{
		return this->SecondAxisLabel;
	}
	vtkBillboardTextActor3D *GetFirstAxis2CaptionActor2D()
	{
		return this->FirstAxisLabel2;
	}
	vtkBillboardTextActor3D *GetSecondAxis2CaptionActor2D()
	{
		return this->SecondAxisLabel2;
	}

	// Description:
	// Set/get the label text.
	vtkSetStringMacro(FirstAxisLabelText);
	vtkGetStringMacro(FirstAxisLabelText);
	vtkSetStringMacro(SecondAxisLabelText);
	vtkGetStringMacro(SecondAxisLabelText);
	vtkSetStringMacro(FirstAxisLabelText2);
	vtkGetStringMacro(FirstAxisLabelText2);
	vtkSetStringMacro(SecondAxisLabelText2);
	vtkGetStringMacro(SecondAxisLabelText2);


	// Description:

	vtkSetMacro(OutlineMode, int);
	vtkGetMacro(OutlineMode, int);

	// Enable/disable drawing the axis labels.
	vtkSetMacro(AxisLabels, int);
	vtkGetMacro(AxisLabels, int);
	vtkBooleanMacro(AxisLabels, int);



	vtkGetMacro(GridType, int);
	void SetGridType(int type);

	// no "Set" Macro as it needs an updage of the properties! => set function defined in the CXX file
	vtkGetMacro(GridSize, double);
	void SetGridSize(double size);
	// no "Set" Macro as it needs an updage of the properties! => set function defined in the CXX file

	vtkGetMacro(GridSpacing, double);
	void SetGridSpacing(double spacing);
	// no "Set" Macro as it needs an updage of the properties! => set function defined in the CXX file


protected:
	vtkGridActor();
	~vtkGridActor();

	
	vtkActor          *GridLines;
	vtkActor          *GridOutline;


	void               UpdateProps();





	char              *FirstAxisLabelText;
	char              *FirstAxisLabelText2;
	char              *SecondAxisLabelText;
	char              *SecondAxisLabelText2;

	
	vtkBillboardTextActor3D *FirstAxisLabel;
	vtkBillboardTextActor3D *FirstAxisLabel2;
	vtkBillboardTextActor3D *SecondAxisLabel;
	vtkBillboardTextActor3D *SecondAxisLabel2;


	int OutlineMode; // 0; outline orange // 1 outline cyan
	int                AxisLabels;
	int                GridType; // 0: xy, z=0; 1: xz, y=0; 2: yz, x=0
	double		     GridSize; // number of squares in each quadrant
	double		     GridSpacing; // spacing in "SizeUnit"(mm) between each grid line
	double			GridOrigin[3];



private:
	vtkGridActor(const vtkGridActor&);  // Not implemented.
	void operator=(const vtkGridActor&);  // Not implemented.
	void CreateGridLines(); //instantiates GridOutline 
	void CreateGridOutline(); //instantiates GridLines
	void CreateGridLabelText();//instantiates the 4 label actors

							   // depending this->GridType

};

#endif

