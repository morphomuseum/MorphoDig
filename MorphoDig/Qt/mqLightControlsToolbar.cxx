/*=========================================================================

   Program: MorphoDig
   Module:    Copied from Paraview pqMainControlsToolbar.cxx

   Copyright (c) 2005,2006 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaView is a free software; you can redistribute it and/or modify it
   under the terms of the ParaView license version 1.2.

   See License_v1.2.txt for the full ParaView license.
   A copy of this license can be obtained by contacting
   Kitware Inc.
   28 Corporate Drive
   Clifton Park, NY 12065
   USA

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

========================================================================*/
#include "mqLightControlsToolbar.h"
#include "ui_mqLightControlsToolbar.h"

// For later!
#include "mqSaveNTWDialogReaction.h"
#include "mqUndoRedoReaction.h"
#include "mqEditLMKDialogReaction.h"
#include "mqCreateLMKDialogReaction.h"
#include "mqEditFLGDialogReaction.h"
#include "mqEditACTORDialogReaction.h"
#include "mqMorphoDigCore.h"
#include "mqOpenDataReaction.h"
#include "mqCameraReaction.h"
#include <vtkLight.h>
#include <vtkRenderer.h>

#include <QToolButton>


//-----------------------------------------------------------------------------
void mqLightControlsToolbar::constructor()
{
 // Ui::mqLightControlsToolbar ui;
 // ui.setupUi(this);
  this->ui = new Ui_mqLightControlsToolbar;
  this->ui->setupUi(this);
 
  connect(this->ui->actionFrontLight, SIGNAL(triggered()), this, SLOT(slotFrontLight()));
  connect(this->ui->actionBackLight, SIGNAL(triggered()), this, SLOT(slotBackLight()));
  connect(this->ui->actionAboveLight, SIGNAL(triggered()), this, SLOT(slotAboveLight()));
  connect(this->ui->actionBelowLight, SIGNAL(triggered()), this, SLOT(slotBelowLight()));
  connect(this->ui->actionLeftLight, SIGNAL(triggered()), this, SLOT(slotLeftLight()));
  connect(this->ui->actionRightLight, SIGNAL(triggered()), this, SLOT(slotRightLight()));
 
  
}


void mqLightControlsToolbar::slotFrontLight()
{
	vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
	cout << "front light!" << endl;
	light->SetLightTypeToCameraLight();
	light->SetPosition(0, 0, 1);
	mqMorphoDigCore::instance()->getRenderer()->RemoveAllLights();
	mqMorphoDigCore::instance()->getRenderer()->AddLight(light);
	mqMorphoDigCore::instance()->Render();


	
	
	
}
void mqLightControlsToolbar::slotBackLight()
{
	
	vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
	cout << "back light!" << endl;
	light->SetLightTypeToCameraLight();
	light->SetPosition(-1, -1, -1);
	
	

	vtkSmartPointer<vtkLight> light2 = vtkSmartPointer<vtkLight>::New();
	light2->SetLightTypeToCameraLight();
	light2->SetPosition(1, 1, -1);
	
	vtkSmartPointer<vtkLight> light3= vtkSmartPointer<vtkLight>::New();
	light3->SetLightTypeToCameraLight();
	light3->SetPosition(0, 1, -1);
	
	vtkSmartPointer<vtkLight> light4 = vtkSmartPointer<vtkLight>::New();
	light4->SetLightTypeToCameraLight();
	light4->SetPosition(0, -1, -1);

	mqMorphoDigCore::instance()->getRenderer()->RemoveAllLights();
	mqMorphoDigCore::instance()->getRenderer()->AddLight(light);
	//mqMorphoDigCore::instance()->getRenderer()->AddLight(light2);
	//mqMorphoDigCore::instance()->getRenderer()->AddLight(light3);
	//mqMorphoDigCore::instance()->getRenderer()->AddLight(light4);
	mqMorphoDigCore::instance()->Render();

	

}
void mqLightControlsToolbar::slotAboveLight()
{

	vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
	light->SetLightTypeToCameraLight();
	light->SetPosition(0, 1, 1);
	

	mqMorphoDigCore::instance()->getRenderer()->RemoveAllLights();
	mqMorphoDigCore::instance()->getRenderer()->AddLight(light);
	mqMorphoDigCore::instance()->Render();


	

}
void mqLightControlsToolbar::slotBelowLight()
{
	vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
	light->SetLightTypeToCameraLight();
	light->SetPosition(0, -1, 1);
	
	mqMorphoDigCore::instance()->getRenderer()->RemoveAllLights();
	mqMorphoDigCore::instance()->getRenderer()->AddLight(light);
	mqMorphoDigCore::instance()->Render();

	

}

void mqLightControlsToolbar::slotLeftLight()
{

	cout << "left light!" << endl;
	vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
	light->SetLightTypeToCameraLight();
	light->SetPosition(-1, 0, 1);
	
	mqMorphoDigCore::instance()->getRenderer()->RemoveAllLights();
	mqMorphoDigCore::instance()->getRenderer()->AddLight(light);
	mqMorphoDigCore::instance()->Render();


}
void mqLightControlsToolbar::slotRightLight()
{

	cout << "right light!" << endl;
	vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
	light->SetLightTypeToCameraLight();
	light->SetPosition(1, 0, 1);
	
	mqMorphoDigCore::instance()->getRenderer()->RemoveAllLights();
	mqMorphoDigCore::instance()->getRenderer()->AddLight(light);
	mqMorphoDigCore::instance()->Render();

}