/*=========================================================================

  Program:   MorphoDig
  Module:    vtkOrientationHelperWidget.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkOrientationHelperWidget.h"

#include <vtkActor2D.h>
#include <vtkCallbackCommand.h>
#include <vtkCamera.h>
#include <vtkCoordinate.h>
#include <vtkObjectFactory.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper2D.h>
#include <vtkProperty2D.h>
#include <vtkProp.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

vtkStandardNewMacro(vtkOrientationHelperWidget);

class vtkOrientationHelperWidgetObserver : public vtkCommand
{
public:
	static vtkOrientationHelperWidgetObserver *New()
	{
		//cout << "Helper Objecter created?" << endl;
		return new vtkOrientationHelperWidgetObserver;
	};

	vtkOrientationHelperWidgetObserver()
	{
		this->OrientationHelperWidget = 0;
	}

	void Execute(vtkObject* wdg, unsigned long event, void *calldata) override
	{
		if (this->OrientationHelperWidget)
		{
			//cout << "execute!!!" << endl;
			this->OrientationHelperWidget->ExecuteCameraUpdateEventHelper(wdg, event, calldata);
		}
	}

	vtkOrientationHelperWidget *OrientationHelperWidget;
};
vtkOrientationHelperWidget::vtkOrientationHelperWidget()
{
	this->StartEventObserverId = 0;
	this->EventCallbackCommand->SetCallback(vtkOrientationHelperWidget::ProcessEvents);

	this->Observer = vtkOrientationHelperWidgetObserver::New();
	//cout << "I should have created a Helper oberver..." << endl;
	this->Observer->OrientationHelperWidget = this;

	this->Tolerance = 7;
	this->Moving = 0;

	
	
	this->Renderer = vtkRenderer::New();
	// VTK 7.0
	this->Renderer->SetViewport(0.0, 0.0, 0.2, 0.2);
	// END VTK 7.0
	this->Renderer->SetLayer(1);
	this->Renderer->InteractiveOff();

	//VTK 7.1!
	/*this->Viewport[0] = 0.0;
	this->Viewport[1] = 0.0;
	this->Viewport[2] = 0.2;
	this->Viewport[3] = 0.2;*/
	// END VTK 7.1

	this->Priority = 0.55;
	this->OrientationMarker = NULL;
	this->State = vtkOrientationHelperWidget::Outside;
	this->Interactive = 1;

	this->Outline = vtkPolyData::New();
	this->Outline->Allocate();
	vtkPoints *points = vtkPoints::New();
	vtkIdType ptIds[5];
	ptIds[4] = ptIds[0] = points->InsertNextPoint(1, 1, 0);
	ptIds[1] = points->InsertNextPoint(2, 1, 0);
	ptIds[2] = points->InsertNextPoint(2, 2, 0);
	ptIds[3] = points->InsertNextPoint(1, 2, 0);

	this->Outline->SetPoints(points);
	this->Outline->InsertNextCell(VTK_POLY_LINE, 5, ptIds);

	vtkCoordinate *tcoord = vtkCoordinate::New();
	tcoord->SetCoordinateSystemToDisplay();

	vtkPolyDataMapper2D *mapper = vtkPolyDataMapper2D::New();
	mapper->SetInputData(this->Outline);
	mapper->SetTransformCoordinate(tcoord);

	this->OutlineActor = vtkActor2D::New();
	this->OutlineActor->SetMapper(mapper);
	this->OutlineActor->SetPosition(0, 0);
	this->OutlineActor->SetPosition2(1, 1);

	points->Delete();
	mapper->Delete();
	tcoord->Delete();
}

//-------------------------------------------------------------------------
vtkOrientationHelperWidget::~vtkOrientationHelperWidget()
{
	this->Observer->Delete();
	this->Renderer->Delete();
	this->SetOrientationMarker(NULL);
	this->OutlineActor->Delete();
	this->Outline->Delete();
}
// Only there to avoid that this->Observer being a vtk...Marker instead of vtk...Helper... (but a copy)
void vtkOrientationHelperWidget::SetEnabled(int enabling)
{
	if (!this->Interactor)
	{
		vtkErrorMacro("The interactor must be set prior to enabling/disabling widget");
	}

	if (enabling)
	{
		if (this->Enabled)
		{
			return;
		}

		if (!this->OrientationMarker)
		{
			vtkErrorMacro("An orientation marker must be set prior to enabling/disabling widget");
			return;
		}

		if (!this->CurrentRenderer)
		{
			this->SetCurrentRenderer(this->Interactor->FindPokedRenderer(
				this->Interactor->GetLastEventPosition()[0],
				this->Interactor->GetLastEventPosition()[1]));

			if (this->CurrentRenderer == NULL)
			{
				return;
			}
		}

		this->Enabled = 1;

		// Compute the viewport for the widget w.r.t. to the current renderer
		// VTK 7.1!!!!
		/*double currentViewport[4];
		this->CurrentRenderer->GetViewport(currentViewport);
		double vp[4], currentViewportRange[2];
		for (int i = 0; i < 2; ++i)
		{
			currentViewportRange[i] = currentViewport[i + 2] - currentViewport[i];
			vp[i] = this->Viewport[i] * currentViewportRange[i] +
				currentViewport[i];
			vp[i + 2] = this->Viewport[i + 2] * currentViewportRange[i] +
				currentViewport[i];
		}
		this->Renderer->SetViewport(vp);*/
		// END VTK 7.1

		vtkRenderWindow* renwin = this->CurrentRenderer->GetRenderWindow();
		renwin->AddRenderer(this->Renderer);
		if (renwin->GetNumberOfLayers() < 2)
		{
			renwin->SetNumberOfLayers(2);
		}

		this->CurrentRenderer->AddViewProp(this->OutlineActor);
		this->OutlineActor->VisibilityOff();
		this->Renderer->AddViewProp(this->OrientationMarker);
		this->OrientationMarker->VisibilityOn();

		if (this->Interactive)
		{
			vtkRenderWindowInteractor *i = this->Interactor;
			if (this->EventCallbackCommand)
			{
				i->AddObserver(vtkCommand::MouseMoveEvent,
					this->EventCallbackCommand, this->Priority);
				i->AddObserver(vtkCommand::LeftButtonPressEvent,
					this->EventCallbackCommand, this->Priority);
				i->AddObserver(vtkCommand::LeftButtonReleaseEvent,
					this->EventCallbackCommand, this->Priority);
			}
		}

		vtkCamera* pcam = this->CurrentRenderer->GetActiveCamera();
		vtkCamera* cam = this->Renderer->GetActiveCamera();
		if (pcam && cam)
		{
			cam->SetParallelProjection(pcam->GetParallelProjection());
		}

		// We need to copy the camera before the compositing observer is called.
		// Compositing temporarily changes the camera to display an image.
		this->StartEventObserverId = this->CurrentRenderer->AddObserver(
			vtkCommand::StartEvent, this->Observer, 1);
		this->InvokeEvent(vtkCommand::EnableEvent, NULL);
	}
	else
	{
		if (!this->Enabled)
		{
			return;
		}

		this->Enabled = 0;
		this->Interactor->RemoveObserver(this->EventCallbackCommand);

		this->OrientationMarker->VisibilityOff();
		this->Renderer->RemoveViewProp(this->OrientationMarker);
		this->OutlineActor->VisibilityOff();
		this->CurrentRenderer->RemoveViewProp(this->OutlineActor);

		// if the render window is still around, remove our renderer from it
		if (this->CurrentRenderer->GetRenderWindow())
		{
			this->CurrentRenderer->GetRenderWindow()->
				RemoveRenderer(this->Renderer);
		}
		if (this->StartEventObserverId != 0)
		{
			this->CurrentRenderer->RemoveObserver(this->StartEventObserverId);
		}

		this->InvokeEvent(vtkCommand::DisableEvent, NULL);
		this->SetCurrentRenderer(NULL);
	}
}
//-------------------------------------------------------------------------
void vtkOrientationHelperWidget::ExecuteCameraUpdateEventHelper(vtkObject *vtkNotUsed(o),
                                   unsigned long vtkNotUsed(event),
                                   void *vtkNotUsed(calldata))
{
	//cout << "Try X labels off" << endl;
  if (!this->CurrentRenderer)
  {
    return;
  }
 
  
 
  
  vtkOrientationHelperActor *myAct = (vtkOrientationHelperActor*)this->OrientationMarker;
  
 // cout << "X labels off" << endl;
  vtkCamera *cam = this->CurrentRenderer->GetActiveCamera();
  double pos[3], fp[3], viewup[3];
  cam->GetPosition( pos );
  cam->GetFocalPoint( fp );
  cam->GetViewUp( viewup );
  double viewing_vector[3];
  vtkMath::Subtract(pos, fp, viewing_vector);
  vtkMath::Normalize(viewing_vector);
  myAct->XAxisLabelsOn();
  myAct->YAxisLabelsOn();
  myAct->ZAxisLabelsOn();
  //cout << "viewing vector: " << viewing_vector[0] << ", " << viewing_vector[1] << ", " << viewing_vector[2] << endl;
  if (abs(viewing_vector[0]) > 0.8) {
	  myAct->XAxisLabelsOff();
  }
  if (abs(viewing_vector[1]) > 0.8) {
	  myAct->YAxisLabelsOff();
  }

  if (abs(viewing_vector[2]) > 0.8) {
	  myAct->ZAxisLabelsOff();
  }

  cam = this->Renderer->GetActiveCamera();
  cam->SetPosition( pos );
  cam->SetFocalPoint( fp );
  cam->SetViewUp( viewup );
  this->Renderer->ResetCamera();

  this->UpdateOutline();
}
