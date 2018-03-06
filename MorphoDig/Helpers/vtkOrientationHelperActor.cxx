/*=========================================================================

Program:   MorphoDig
Module:    vtkOrientationHelperActor.cxx


=========================================================================*/
#include "vtkOrientationHelperActor.h"

#include <vtkActor.h>
#include <vtkCaptionActor2D.h>
#include <vtkTextProperty.h>
#include <vtkConeSource.h>
#include <vtkCylinderSource.h>
#include <vtkLineSource.h>
#include <vtkObject.h>
#include <vtkObjectFactory.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPropCollection.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSphereSource.h>
#include <vtkTextProperty.h>
#include <vtkTransform.h>

vtkStandardNewMacro(vtkOrientationHelperActor);


//----------------------------------------------------------------------------
vtkOrientationHelperActor::vtkOrientationHelperActor()//:vtkAxesActor()
{
	
	this->AxisLabels = 1;
	this->XAxisLabels = 1;
	this->YAxisLabels = 1;
	this->ZAxisLabels = 1;

	this->XAxisLabelText = NULL;
	this->YAxisLabelText = NULL;
	this->ZAxisLabelText = NULL;

	this->X2AxisLabelText = NULL;
	this->Y2AxisLabelText = NULL;
	this->Z2AxisLabelText = NULL;

	this->SetXAxisLabelText("X");
	this->SetYAxisLabelText("Y");
	this->SetZAxisLabelText("Z");

	this->SetX2AxisLabelText("-X");
	this->SetY2AxisLabelText("-Y");
	this->SetZ2AxisLabelText("-Z");

	this->XAxisShaft = vtkActor::New();
	this->XAxisShaft->GetProperty()->SetColor(1, 0, 0);

	this->YAxisShaft = vtkActor::New();
	this->YAxisShaft->GetProperty()->SetColor(0, 1, 0);
	
	this->ZAxisShaft = vtkActor::New();
	this->ZAxisShaft->GetProperty()->SetColor(0, 0, 1);


	this->X2AxisShaft = vtkActor::New();
	this->X2AxisShaft->GetProperty()->SetColor(1, 1, 0);

	this->Y2AxisShaft = vtkActor::New();
	this->Y2AxisShaft->GetProperty()->SetColor(0, 1 ,1);

	this->Z2AxisShaft = vtkActor::New();
	this->Z2AxisShaft->GetProperty()->SetColor(1, 0, 1);
	
	this->XAxisTip = vtkActor::New();
	this->XAxisTip->GetProperty()->SetColor(1, 0, 0);
	this->YAxisTip = vtkActor::New();
	this->YAxisTip->GetProperty()->SetColor(0, 1, 0);
	this->ZAxisTip = vtkActor::New();
	this->ZAxisTip->GetProperty()->SetColor(0, 0, 1);


	this->X2AxisTip = vtkActor::New();
	this->X2AxisTip->GetProperty()->SetColor(1, 1, 0);
	this->Y2AxisTip = vtkActor::New();
	this->Y2AxisTip->GetProperty()->SetColor(0, 1, 1);
	this->Z2AxisTip = vtkActor::New();
	this->Z2AxisTip->GetProperty()->SetColor(1, 0, 1);

	this->CylinderSource = vtkCylinderSource::New();
	this->CylinderSource->SetHeight(1.0);

	this->LineSource = vtkLineSource::New();
	this->LineSource->SetPoint1(0.0, 0.0, 0.0);
	this->LineSource->SetPoint2(0.0, 1.0, 0.0);

	this->ConeSource = vtkConeSource::New();
	this->ConeSource->SetDirection(0, 1, 0);
	this->ConeSource->SetHeight(1.0);

	this->SphereSource = vtkSphereSource::New();
	
	vtkPolyDataMapper *shaftMapper2 = vtkPolyDataMapper::New();

	this->XAxisShaft->SetMapper(shaftMapper2);
	this->YAxisShaft->SetMapper(shaftMapper2);
	this->ZAxisShaft->SetMapper(shaftMapper2);
	this->X2AxisShaft->SetMapper(shaftMapper2);
	this->Y2AxisShaft->SetMapper(shaftMapper2);
	this->Z2AxisShaft->SetMapper(shaftMapper2);
	shaftMapper2->Delete();

	vtkPolyDataMapper *tipMapper2 = vtkPolyDataMapper::New();

	this->XAxisTip->SetMapper(tipMapper2);
	this->YAxisTip->SetMapper(tipMapper2);
	this->ZAxisTip->SetMapper(tipMapper2);
	this->X2AxisTip->SetMapper(tipMapper2);
	this->Y2AxisTip->SetMapper(tipMapper2);
	this->Z2AxisTip->SetMapper(tipMapper2);

	tipMapper2->Delete();

	this->TotalLength[0] = 1.0;
	this->TotalLength[1] = 1.0;
	this->TotalLength[2] = 1.0;
	
	

	this->NormalizedShaftLength[0] = 0.8;
	this->NormalizedShaftLength[1] = 0.8;
	this->NormalizedShaftLength[2] = 0.8;

	
	this->NormalizedTipLength[0] = 0.2;
	this->NormalizedTipLength[1] = 0.2;
	this->NormalizedTipLength[2] = 0.2;
	
	

	this->NormalizedLabelPosition[0] = 1.0;
	this->NormalizedLabelPosition[1] = 1.0;
	this->NormalizedLabelPosition[2] = 1.0;

	this->ConeResolution = 16;
	this->SphereResolution = 16;
	this->CylinderResolution = 16;

	this->ConeRadius = 0.4;
	this->SphereRadius = 0.5;
	this->CylinderRadius = 0.05;

	this->ShaftType = vtkOrientationHelperActor::LINE_SHAFT;
	this->TipType = vtkOrientationHelperActor::CONE_TIP;

	this->UserDefinedTip = NULL;
	this->UserDefinedShaft = NULL;

	this->XAxisLabel = vtkCaptionActor2D::New();
	this->YAxisLabel = vtkCaptionActor2D::New();
	this->ZAxisLabel = vtkCaptionActor2D::New();

	this->X2AxisLabel = vtkCaptionActor2D::New();
	this->Y2AxisLabel = vtkCaptionActor2D::New();
	this->Z2AxisLabel = vtkCaptionActor2D::New();

	double red[3] = { 1, 0, 0 };
	double green[3] = { 0, 1, 0 };
	double blue[3] = { 0, 0, 1 };
	double yellow[3] = { 1, 1, 0 };
	double cyan[3] = { 0, 1, 1 };
	double fuschia[3] = { 1, 0, 1 };

	vtkTextProperty* Xproperty = vtkTextProperty::New();
	Xproperty->SetColor(red);
	Xproperty->SetFontFamilyToArial();
	this->XAxisLabel->SetCaptionTextProperty(Xproperty);
	this->XAxisLabel->ThreeDimensionalLeaderOff();
	this->XAxisLabel->LeaderOff();
	this->XAxisLabel->BorderOff();
	this->XAxisLabel->SetPosition(0, 0);

	vtkTextProperty* X2property = vtkTextProperty::New();
	X2property->SetColor(yellow);
	X2property->SetFontFamilyToArial();
	this->X2AxisLabel->SetCaptionTextProperty(X2property);
	this->X2AxisLabel->ThreeDimensionalLeaderOff();
	this->X2AxisLabel->LeaderOff();
	this->X2AxisLabel->BorderOff();
	this->X2AxisLabel->SetPosition(0, 0);


	vtkTextProperty* Yproperty = vtkTextProperty::New();
	Yproperty->SetColor(green);
	Yproperty->SetFontFamilyToArial();
	this->YAxisLabel->SetCaptionTextProperty(Yproperty);
	this->YAxisLabel->ThreeDimensionalLeaderOff();
	this->YAxisLabel->LeaderOff();
	this->YAxisLabel->BorderOff();
	this->YAxisLabel->SetPosition(0, 0);

	vtkTextProperty* Y2property = vtkTextProperty::New();
	Y2property->SetColor(cyan);
	Y2property->SetFontFamilyToArial();
	this->Y2AxisLabel->SetCaptionTextProperty(Y2property);
	this->Y2AxisLabel->ThreeDimensionalLeaderOff();
	this->Y2AxisLabel->LeaderOff();
	this->Y2AxisLabel->BorderOff();
	this->Y2AxisLabel->SetPosition(0, 0);

	vtkTextProperty* Zproperty = vtkTextProperty::New();
	Zproperty->SetColor(blue);
	Zproperty->SetFontFamilyToArial();
	this->ZAxisLabel->SetCaptionTextProperty(Zproperty);

	this->ZAxisLabel->ThreeDimensionalLeaderOff();
	this->ZAxisLabel->LeaderOff();
	this->ZAxisLabel->BorderOff();
	this->ZAxisLabel->SetPosition(0, 0);

	vtkTextProperty* Z2property = vtkTextProperty::New();
	Z2property->SetColor(fuschia);
	Z2property->SetFontFamilyToArial();
	this->Z2AxisLabel->SetCaptionTextProperty(Z2property);
	this->Z2AxisLabel->ThreeDimensionalLeaderOff();
	this->Z2AxisLabel->LeaderOff();
	this->Z2AxisLabel->BorderOff();
	this->Z2AxisLabel->SetPosition(0, 0);

	this->UpdateProps();
}

//----------------------------------------------------------------------------
vtkOrientationHelperActor::~vtkOrientationHelperActor()
{

	this->CylinderSource->Delete();
	this->LineSource->Delete();
	this->ConeSource->Delete();
	this->SphereSource->Delete();

	this->XAxisShaft->Delete();
	this->YAxisShaft->Delete();
	this->ZAxisShaft->Delete();

	this->X2AxisShaft->Delete();
	this->Y2AxisShaft->Delete();
	this->Z2AxisShaft->Delete();


	this->XAxisTip->Delete();
	this->YAxisTip->Delete();
	this->ZAxisTip->Delete();
	this->X2AxisTip->Delete();
	this->Y2AxisTip->Delete();
	this->Z2AxisTip->Delete();


	this->SetUserDefinedTip(NULL);
	this->SetUserDefinedShaft(NULL);

	this->SetXAxisLabelText(NULL);
	this->SetYAxisLabelText(NULL);
	this->SetZAxisLabelText(NULL);
	this->SetX2AxisLabelText(NULL);
	this->SetY2AxisLabelText(NULL);
	this->SetZ2AxisLabelText(NULL);

	this->XAxisLabel->Delete();
	this->YAxisLabel->Delete();
	this->ZAxisLabel->Delete();

	this->X2AxisLabel->Delete();
	this->Y2AxisLabel->Delete();
	this->Z2AxisLabel->Delete();
}

//----------------------------------------------------------------------------
// Shallow copy of an actor.
void vtkOrientationHelperActor::ShallowCopy(vtkProp *prop)
{
	vtkOrientationHelperActor *a = vtkOrientationHelperActor::SafeDownCast(prop);
	if (a != NULL)
	{
		this->SetAxisLabels(a->GetAxisLabels());
		this->SetXAxisLabels(a->GetXAxisLabels());
		this->SetYAxisLabels(a->GetYAxisLabels());
		this->SetZAxisLabels(a->GetZAxisLabels());
		this->SetXAxisLabelText(a->GetXAxisLabelText());
		this->SetYAxisLabelText(a->GetYAxisLabelText());
		this->SetZAxisLabelText(a->GetZAxisLabelText());
		this->SetX2AxisLabelText(a->GetX2AxisLabelText());
		this->SetY2AxisLabelText(a->GetY2AxisLabelText());
		this->SetZ2AxisLabelText(a->GetZ2AxisLabelText());

		this->SetTotalLength(a->GetTotalLength());
		this->SetNormalizedShaftLength(a->GetNormalizedShaftLength());
		this->SetNormalizedTipLength(a->GetNormalizedTipLength());
		this->SetNormalizedLabelPosition(a->GetNormalizedLabelPosition());
		this->SetConeResolution(a->GetConeResolution());
		this->SetSphereResolution(a->GetSphereResolution());
		this->SetCylinderResolution(a->GetCylinderResolution());
		this->SetConeRadius(a->GetConeRadius());
		this->SetSphereRadius(a->GetSphereRadius());
		this->SetCylinderRadius(a->GetCylinderRadius());
		this->SetTipType(a->GetTipType());
		this->SetShaftType(a->GetShaftType());
		this->SetUserDefinedTip(a->GetUserDefinedTip());
		this->SetUserDefinedShaft(a->GetUserDefinedShaft());
	}

	// Now do superclass
	this->vtkProp3D::ShallowCopy(prop);
}

//----------------------------------------------------------------------------
void vtkOrientationHelperActor::GetActors(vtkPropCollection *ac)
{
	this->Superclass::GetActors(ac);
	ac->AddItem(this->X2AxisShaft);
	ac->AddItem(this->Y2AxisShaft);
	ac->AddItem(this->Z2AxisShaft);

	ac->AddItem(this->X2AxisTip);
	ac->AddItem(this->Y2AxisTip);
	ac->AddItem(this->Z2AxisTip);
}

//----------------------------------------------------------------------------
int vtkOrientationHelperActor::RenderOpaqueGeometry(vtkViewport *vp)
{
	//int renderedSomething = this->Superclass::RenderOpaqueGeometry(vp);
	int renderedSomething = 0;

	this->UpdateProps();

	renderedSomething += this->XAxisShaft->RenderOpaqueGeometry(vp);
	renderedSomething += this->YAxisShaft->RenderOpaqueGeometry(vp);
	renderedSomething += this->ZAxisShaft->RenderOpaqueGeometry(vp);

	renderedSomething += this->XAxisTip->RenderOpaqueGeometry(vp);
	renderedSomething += this->YAxisTip->RenderOpaqueGeometry(vp);
	renderedSomething += this->ZAxisTip->RenderOpaqueGeometry(vp);

		
	renderedSomething += this->X2AxisShaft->RenderOpaqueGeometry(vp);
	renderedSomething += this->Y2AxisShaft->RenderOpaqueGeometry(vp);
	renderedSomething += this->Z2AxisShaft->RenderOpaqueGeometry(vp);

	renderedSomething += this->X2AxisTip->RenderOpaqueGeometry(vp);
	renderedSomething += this->Y2AxisTip->RenderOpaqueGeometry(vp);
	renderedSomething += this->Z2AxisTip->RenderOpaqueGeometry(vp);

	if (this->AxisLabels)
	{
		
		if (this->XAxisLabels)
		{
			renderedSomething += this->XAxisLabel->RenderOpaqueGeometry(vp);
			renderedSomething += this->X2AxisLabel->RenderOpaqueGeometry(vp);
		}
		if (this->YAxisLabels)
		{
			renderedSomething += this->YAxisLabel->RenderOpaqueGeometry(vp);
			renderedSomething += this->Y2AxisLabel->RenderOpaqueGeometry(vp);
		}
		if (this->ZAxisLabels)
		{
			renderedSomething += this->ZAxisLabel->RenderOpaqueGeometry(vp);
			renderedSomething += this->Z2AxisLabel->RenderOpaqueGeometry(vp);
		}
	}

	renderedSomething = (renderedSomething > 0) ? (1) : (0);
	return renderedSomething;
}

//-----------------------------------------------------------------------------
int vtkOrientationHelperActor::RenderTranslucentPolygonalGeometry(vtkViewport *vp)
{
//	int renderedSomething = this->Superclass::RenderTranslucentPolygonalGeometry(vp);
	int renderedSomething = 0;

	this->UpdateProps();

	renderedSomething += this->XAxisShaft->RenderTranslucentPolygonalGeometry(vp);
	renderedSomething += this->YAxisShaft->RenderTranslucentPolygonalGeometry(vp);
	renderedSomething += this->ZAxisShaft->RenderTranslucentPolygonalGeometry(vp);

	renderedSomething += this->XAxisTip->RenderTranslucentPolygonalGeometry(vp);
	renderedSomething += this->YAxisTip->RenderTranslucentPolygonalGeometry(vp);
	renderedSomething += this->ZAxisTip->RenderTranslucentPolygonalGeometry(vp);

		
	renderedSomething += this->X2AxisShaft->RenderTranslucentPolygonalGeometry(vp);
	renderedSomething += this->Y2AxisShaft->RenderTranslucentPolygonalGeometry(vp);
	renderedSomething += this->Z2AxisShaft->RenderTranslucentPolygonalGeometry(vp);

	renderedSomething += this->X2AxisTip->RenderTranslucentPolygonalGeometry(vp);
	renderedSomething += this->Y2AxisTip->RenderTranslucentPolygonalGeometry(vp);
	renderedSomething += this->Z2AxisTip->RenderTranslucentPolygonalGeometry(vp);

	if (this->AxisLabels)
	{		
		if (this->XAxisLabels)
		{
			renderedSomething += this->XAxisLabel->RenderTranslucentPolygonalGeometry(vp);
			renderedSomething += this->X2AxisLabel->RenderTranslucentPolygonalGeometry(vp);
		}
		if (this->YAxisLabels)
		{
			renderedSomething += this->YAxisLabel->RenderTranslucentPolygonalGeometry(vp);
			renderedSomething += this->Y2AxisLabel->RenderTranslucentPolygonalGeometry(vp);
		}
		if (this->ZAxisLabels)
		{
			renderedSomething += this->ZAxisLabel->RenderTranslucentPolygonalGeometry(vp);
			renderedSomething += this->Z2AxisLabel->RenderTranslucentPolygonalGeometry(vp);
		}
	}
	renderedSomething = (renderedSomething > 0) ? (1) : (0);
	return renderedSomething;
}

//-----------------------------------------------------------------------------
// Description:
// Does this prop have some translucent polygonal geometry?
int vtkOrientationHelperActor::HasTranslucentPolygonalGeometry()
{
	//int result = this->Superclass::HasTranslucentPolygonalGeometry();
	int result = 0;

	this->UpdateProps();

	result |= this->XAxisShaft->HasTranslucentPolygonalGeometry();
	result |= this->YAxisShaft->HasTranslucentPolygonalGeometry();
	result |= this->ZAxisShaft->HasTranslucentPolygonalGeometry();

	result |= this->XAxisTip->HasTranslucentPolygonalGeometry();
	result |= this->YAxisTip->HasTranslucentPolygonalGeometry();
	result |= this->ZAxisTip->HasTranslucentPolygonalGeometry();
		
	result |= this->X2AxisShaft->HasTranslucentPolygonalGeometry();
	result |= this->Y2AxisShaft->HasTranslucentPolygonalGeometry();
	result |= this->Z2AxisShaft->HasTranslucentPolygonalGeometry();

	result |= this->X2AxisTip->HasTranslucentPolygonalGeometry();
	result |= this->Y2AxisTip->HasTranslucentPolygonalGeometry();
	result |= this->Z2AxisTip->HasTranslucentPolygonalGeometry();

	if (this->AxisLabels)
	{		
		if (this->XAxisLabels)
		{
			result |= this->XAxisLabel->HasTranslucentPolygonalGeometry();
			result |= this->X2AxisLabel->HasTranslucentPolygonalGeometry();
		}
		if (this->YAxisLabels)
		{
			result |= this->YAxisLabel->HasTranslucentPolygonalGeometry();
			result |= this->Y2AxisLabel->HasTranslucentPolygonalGeometry();
		}
		if (this->ZAxisLabels)
		{
			result |= this->ZAxisLabel->HasTranslucentPolygonalGeometry();
			result |= this->Z2AxisLabel->HasTranslucentPolygonalGeometry();
		}
	}
	return result;
}

//-----------------------------------------------------------------------------
int vtkOrientationHelperActor::RenderOverlay(vtkViewport *vp)
{
	//int renderedSomething = this->Superclass::RenderOverlay(vp);
	int renderedSomething = 0;

	if (!this->AxisLabels)
	{
		return renderedSomething;
	}

	this->UpdateProps();
	if (this->XAxisLabels)
	{
		renderedSomething += this->XAxisLabel->RenderOverlay(vp);
		renderedSomething += this->X2AxisLabel->RenderOverlay(vp);
	}
	if (this->YAxisLabels)
	{
		renderedSomething += this->YAxisLabel->RenderOverlay(vp);
		renderedSomething += this->Y2AxisLabel->RenderOverlay(vp);
	}
	if (this->ZAxisLabels)
	{
		renderedSomething += this->ZAxisLabel->RenderOverlay(vp);
		renderedSomething += this->Z2AxisLabel->RenderOverlay(vp);
	}

	renderedSomething = (renderedSomething > 0) ? (1) : (0);
	return renderedSomething;
}

//----------------------------------------------------------------------------
void vtkOrientationHelperActor::ReleaseGraphicsResources(vtkWindow *win)
{
	this->Superclass::ReleaseGraphicsResources(win);


	this->X2AxisShaft->ReleaseGraphicsResources(win);
	this->Y2AxisShaft->ReleaseGraphicsResources(win);
	this->Z2AxisShaft->ReleaseGraphicsResources(win);


	this->X2AxisTip->ReleaseGraphicsResources(win);
	this->Y2AxisTip->ReleaseGraphicsResources(win);
	this->Z2AxisTip->ReleaseGraphicsResources(win);


	this->X2AxisLabel->ReleaseGraphicsResources(win);
	this->Y2AxisLabel->ReleaseGraphicsResources(win);
	this->Z2AxisLabel->ReleaseGraphicsResources(win);
}

//----------------------------------------------------------------------------









//----------------------------------------------------------------------------
void vtkOrientationHelperActor::UpdateProps()
{
	this->Superclass::UpdateProps();
	

	if (this->GetUserTransform())
	{		

		this->X2AxisShaft->SetUserTransform(NULL);
		this->Y2AxisShaft->SetUserTransform(NULL);
		this->Z2AxisShaft->SetUserTransform(NULL);
		this->X2AxisTip->SetUserTransform(NULL);
		this->Y2AxisTip->SetUserTransform(NULL);
		this->Z2AxisTip->SetUserTransform(NULL);

	}

	double scale[3];
	double bounds[6];

	(vtkPolyDataMapper::SafeDownCast(this->XAxisShaft->GetMapper()))->
		GetInput()->GetBounds(bounds);

	// The shaft and tip geometry are both initially along direction 0 1 0
	// in the case of cylinder, line, and cone.  Build up the axis from
	// constituent elements defined in their default positions.

	int i;
	for (i = 0; i < 3; ++i)
	{
		scale[i] =
			this->NormalizedShaftLength[i] * this->TotalLength[i] /
			(bounds[3] - bounds[2]);
	}

	

	vtkTransform *x2Transform = vtkTransform::New();
	vtkTransform *y2Transform = vtkTransform::New();
	vtkTransform *z2Transform = vtkTransform::New();


	x2Transform->RotateZ(90);
	y2Transform->RotateZ(180);
	z2Transform->RotateX(-90);

	x2Transform->Scale(scale[0], scale[0], scale[0]);
	y2Transform->Scale(scale[1], scale[1], scale[1]);
	z2Transform->Scale(scale[2], scale[2], scale[2]);

	x2Transform->Translate(-(bounds[0] + bounds[1]) / 2,
		-bounds[2],
		-(bounds[4] + bounds[5]) / 2);
	y2Transform->Translate(-(bounds[0] + bounds[1]) / 2,
		-bounds[2],
		-(bounds[4] + bounds[5]) / 2);
	z2Transform->Translate(-(bounds[0] + bounds[1]) / 2,
		-bounds[2],
		-(bounds[4] + bounds[5]) / 2);

	
	this->X2AxisShaft->SetScale(x2Transform->GetScale());
	this->X2AxisShaft->SetPosition(x2Transform->GetPosition());
	this->X2AxisShaft->SetOrientation(x2Transform->GetOrientation());

	this->Y2AxisShaft->SetScale(y2Transform->GetScale());
	this->Y2AxisShaft->SetPosition(y2Transform->GetPosition());
	this->Y2AxisShaft->SetOrientation(y2Transform->GetOrientation());

	
	this->Z2AxisShaft->SetScale(z2Transform->GetScale());
	this->Z2AxisShaft->SetPosition(z2Transform->GetPosition());
	this->Z2AxisShaft->SetOrientation(z2Transform->GetOrientation());

	(vtkPolyDataMapper::SafeDownCast(this->XAxisTip->GetMapper()))->
		GetInput()->GetBounds(bounds);

	x2Transform->Identity();
	y2Transform->Identity();
	z2Transform->Identity();

	

	x2Transform->RotateZ(90);
	y2Transform->RotateZ(180);
	z2Transform->RotateX(-90);

	x2Transform->Scale(this->TotalLength[0], this->TotalLength[0], this->TotalLength[0]);
	y2Transform->Scale(this->TotalLength[1], this->TotalLength[1], this->TotalLength[1]);
	z2Transform->Scale(this->TotalLength[2], this->TotalLength[2], this->TotalLength[2]);


	x2Transform->Translate(0, (1.0 - this->NormalizedTipLength[0]), 0);
	y2Transform->Translate(0, (1.0 - this->NormalizedTipLength[1]), 0);
	z2Transform->Translate(0, (1.0 - this->NormalizedTipLength[2]), 0);

	x2Transform->Scale(this->NormalizedTipLength[0],
		this->NormalizedTipLength[0],
		this->NormalizedTipLength[0]);

	y2Transform->Scale(this->NormalizedTipLength[1],
		this->NormalizedTipLength[1],
		this->NormalizedTipLength[1]);

	z2Transform->Scale(this->NormalizedTipLength[2],
		this->NormalizedTipLength[2],
		this->NormalizedTipLength[2]);

	x2Transform->Translate(-(bounds[0] + bounds[1]) / 2,
		-bounds[2],
		-(bounds[4] + bounds[5]) / 2);
	y2Transform->Translate(-(bounds[0] + bounds[1]) / 2,
		-bounds[2],
		-(bounds[4] + bounds[5]) / 2);
	z2Transform->Translate(-(bounds[0] + bounds[1]) / 2,
		-bounds[2],
		-(bounds[4] + bounds[5]) / 2);

	

	this->X2AxisTip->SetScale(x2Transform->GetScale());
	this->X2AxisTip->SetPosition(x2Transform->GetPosition());
	this->X2AxisTip->SetOrientation(x2Transform->GetOrientation());

	this->Y2AxisTip->SetScale(y2Transform->GetScale());
	this->Y2AxisTip->SetPosition(y2Transform->GetPosition());
	this->Y2AxisTip->SetOrientation(y2Transform->GetOrientation());

	this->Z2AxisTip->SetScale(z2Transform->GetScale());
	this->Z2AxisTip->SetPosition(z2Transform->GetPosition());
	this->Z2AxisTip->SetOrientation(z2Transform->GetOrientation());

	x2Transform->Delete();
	y2Transform->Delete();
	z2Transform->Delete();

	
	this->X2AxisLabel->SetCaption(this->X2AxisLabelText);
	this->Y2AxisLabel->SetCaption(this->Y2AxisLabelText);
	this->Z2AxisLabel->SetCaption(this->Z2AxisLabelText);
	/*
	
	this->X2AxisShaft->GetBounds(bounds);
	double offset = -1* this->NormalizedLabelPosition[0] * (bounds[1] - bounds[0]);
	
	this->X2AxisLabel->SetAttachmentPoint(bounds[0] + offset,
		bounds[2] - (bounds[3] - bounds[2])*2.0,
		bounds[5] + (bounds[5] - bounds[4]) / 2.0);

	
	this->Y2AxisShaft->GetBounds(bounds);
	offset = -1 * this->NormalizedLabelPosition[1] * (bounds[3] - bounds[2]);
	this->Y2AxisLabel->SetAttachmentPoint((bounds[0] + bounds[1]) / 2,
		bounds[2] + offset,
		bounds[5] + (bounds[5] - bounds[4]) / 2.0);

	this->Z2AxisShaft->GetBounds(bounds);
	offset = -1 * this->NormalizedLabelPosition[2] * (bounds[5] - bounds[4]);
	this->Z2AxisLabel->SetAttachmentPoint(bounds[0],
		bounds[2] - (bounds[3] - bounds[2])*2.0,
		bounds[4] + offset);
	*/
	/*hack */
	this->XAxisLabel->SetAttachmentPoint(1,
		0,
		0);
	this->X2AxisLabel->SetAttachmentPoint(-1,
		0,
		0);
	this->YAxisLabel->SetAttachmentPoint(0,
		1,
		0);
	this->Y2AxisLabel->SetAttachmentPoint(0,-1,0);
	this->ZAxisLabel->SetAttachmentPoint(0,
		0,
		1);
	this->Z2AxisLabel->SetAttachmentPoint(0,
		0,
		-1);

	vtkLinearTransform* transform = this->GetUserTransform();
	if (transform)
	{
		
		this->X2AxisShaft->SetUserTransform(transform);
		this->Y2AxisShaft->SetUserTransform(transform);
		this->Z2AxisShaft->SetUserTransform(transform);

		this->X2AxisTip->SetUserTransform(transform);
		this->Y2AxisTip->SetUserTransform(transform);
		this->Z2AxisTip->SetUserTransform(transform);

		double newpos[3];
		double* pos = this->X2AxisLabel->GetAttachmentPoint();
		transform->TransformPoint(pos, newpos);
		this->X2AxisLabel->SetAttachmentPoint(newpos);

		pos = this->Y2AxisLabel->GetAttachmentPoint();
		transform->TransformPoint(pos, newpos);
		this->Y2AxisLabel->SetAttachmentPoint(newpos);

		pos = this->Z2AxisLabel->GetAttachmentPoint();
		transform->TransformPoint(pos, newpos);
		this->Z2AxisLabel->SetAttachmentPoint(newpos);
	}
}


vtkProperty *vtkOrientationHelperActor::GetX2AxisTipProperty()
{
	return this->X2AxisTip->GetProperty();
}

vtkProperty *vtkOrientationHelperActor::GetY2AxisTipProperty()
{
	return this->Y2AxisTip->GetProperty();
}

vtkProperty *vtkOrientationHelperActor::GetZ2AxisTipProperty()
{
	return this->Z2AxisTip->GetProperty();
}


vtkProperty *vtkOrientationHelperActor::GetX2AxisShaftProperty()
{
	return this->X2AxisShaft->GetProperty();
}



//----------------------------------------------------------------------------
vtkProperty *vtkOrientationHelperActor::GetY2AxisShaftProperty()
{
	return this->Y2AxisShaft->GetProperty();
}

//----------------------------------------------------------------------------
vtkProperty *vtkOrientationHelperActor::GetZ2AxisShaftProperty()
{
	return this->Z2AxisShaft->GetProperty();
}
//----------------------------------------------------------------------------
void vtkOrientationHelperActor::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);

	

	os << indent << "X2AxisLabelText: " << (this->X2AxisLabelText ?
		this->X2AxisLabelText : "(none)")
		<< endl;
	os << indent << "Y2AxisLabelText: " << (this->Y2AxisLabelText ?
		this->Y2AxisLabelText : "(none)")
		<< endl;
	os << indent << "Z2AxisLabelText: " << (this->Z2AxisLabelText ?
		this->Z2AxisLabelText : "(none)")
		<< endl;

	os << indent << "XAxisLabels: " << (this->XAxisLabels ? "On\n" : "Off\n");
	os << indent << "YAxisLabels: " << (this->YAxisLabels ? "On\n" : "Off\n");
	os << indent << "ZAxisLabels: " << (this->ZAxisLabels ? "On\n" : "Off\n");

}
