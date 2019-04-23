/*=========================================================================

   Program: MorphoDig
   Module:    Copied from Paraview pqMainControlsWidget.cxx

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
#include "mqLightControlsWidget.h"
#include "ui_mqLightControlsWidget.h"

// For later!
#include <vtkLightCollection.h>
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
void mqLightControlsWidget::constructor()
{
 // Ui::mqLightControlsWidget ui;
 // ui.setupUi(this);
  this->ui = new Ui_mqLightControlsWidget;
  this->ui->setupUi(this);
 
  connect(this->ui->FrontLight, SIGNAL(pressed()), this, SLOT(slotFrontLight()));
  connect(this->ui->BackLight, SIGNAL(pressed()), this, SLOT(slotBackLight()));
  connect(this->ui->AboveLight, SIGNAL(pressed()), this, SLOT(slotAboveLight()));
  connect(this->ui->BelowLight, SIGNAL(pressed()), this, SLOT(slotBelowLight()));
  connect(this->ui->LeftLight, SIGNAL(pressed()), this, SLOT(slotLeftLight()));
  connect(this->ui->RightLight, SIGNAL(pressed()), this, SLOT(slotRightLight()));
 
  
}


void mqLightControlsWidget::slotFrontLight()
{

	
	vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
	light->SetLightTypeToHeadlight();
	light->SetAmbientColor(0, 0, 0);
	
	vtkSmartPointer<vtkLight> light2 = vtkSmartPointer<vtkLight>::New();

	light2->SetLightTypeToCameraLight();
	light2->SetPosition(0, 0, 1);
	//light2->SetAmbientColor(1, 1, 1);

	mqMorphoDigCore::instance()->getRenderer()->RemoveAllLights();
	//mqMorphoDigCore::instance()->getRenderer()->AddLight(light);
	mqMorphoDigCore::instance()->getRenderer()->AddLight(light2);
	mqMorphoDigCore::instance()->Render();


	
	
	
}
void mqLightControlsWidget::slotBackLight()
{
	
	vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
	cout << "back light!" << endl;
	light->SetLightTypeToHeadlight();
	light->SetAmbientColor(0, 0, 0);
	
	vtkSmartPointer<vtkLight> light2 = vtkSmartPointer<vtkLight>::New();

	light2->SetLightTypeToSceneLight();
	light2->SetPosition(-1, -1, -1);
	light2->SetAmbientColor(1, 1, 1);

	
	
	vtkSmartPointer<vtkLight> light3= vtkSmartPointer<vtkLight>::New();
	light3->SetLightTypeToSceneLight();
	light3->SetPosition(0, 1, -1);
	
	vtkSmartPointer<vtkLight> light4 = vtkSmartPointer<vtkLight>::New();
	light4->SetLightTypeToSceneLight();
	light4->SetPosition(0, -1, -1);

	mqMorphoDigCore::instance()->getRenderer()->RemoveAllLights();
	mqMorphoDigCore::instance()->getRenderer()->AddLight(light);
	mqMorphoDigCore::instance()->getRenderer()->AddLight(light2);
	//mqMorphoDigCore::instance()->getRenderer()->AddLight(light3);
	//mqMorphoDigCore::instance()->getRenderer()->AddLight(light4);
	mqMorphoDigCore::instance()->Render();

	

}
void mqLightControlsWidget::slotAboveLight()
{
	vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
	cout << "above light!" << endl;
	light->SetLightTypeToHeadlight();
	light->SetAmbientColor(0, 0, 0);

	vtkSmartPointer<vtkLight> light2 = vtkSmartPointer<vtkLight>::New();

	light2->SetLightTypeToSceneLight();
	light2->SetPosition(0, 1, 1);
	light2->SetAmbientColor(1, 1, 1);
	

	mqMorphoDigCore::instance()->getRenderer()->RemoveAllLights();
	mqMorphoDigCore::instance()->getRenderer()->AddLight(light);
	mqMorphoDigCore::instance()->getRenderer()->AddLight(light2);

	mqMorphoDigCore::instance()->Render();


	

}
void mqLightControlsWidget::slotBelowLight()
{
	vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
	cout << "below light!" << endl;
	light->SetLightTypeToHeadlight();
	light->SetAmbientColor(0, 0, 0);

	vtkSmartPointer<vtkLight> light2 = vtkSmartPointer<vtkLight>::New();

	light2->SetLightTypeToSceneLight();
	light2->SetAmbientColor(1, 1, 1);
	light2->SetPosition(0, -1, 1);

	
	
	mqMorphoDigCore::instance()->getRenderer()->RemoveAllLights();
	mqMorphoDigCore::instance()->getRenderer()->AddLight(light);
	mqMorphoDigCore::instance()->getRenderer()->AddLight(light2);
	mqMorphoDigCore::instance()->Render();

	

}

void mqLightControlsWidget::slotLeftLight()
{
	vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
	cout << "left!" << endl;
	light->SetLightTypeToHeadlight();
	light->SetAmbientColor(0, 0, 0);

	vtkSmartPointer<vtkLight> light2 = vtkSmartPointer<vtkLight>::New();

	light2->SetLightTypeToSceneLight();
	light2->SetPosition(-1, 0, 1);
	light2->SetAmbientColor(1, 1, 1);


	
	
	mqMorphoDigCore::instance()->getRenderer()->RemoveAllLights();
	mqMorphoDigCore::instance()->getRenderer()->AddLight(light);
	mqMorphoDigCore::instance()->getRenderer()->AddLight(light2);
	mqMorphoDigCore::instance()->Render();


}
void mqLightControlsWidget::slotRightLight()
{
	vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
	cout << "right!" << endl;
	light->SetLightTypeToHeadlight();
	light->SetAmbientColor(0, 0, 0);

	vtkSmartPointer<vtkLight> light2 = vtkSmartPointer<vtkLight>::New();

	light2->SetLightTypeToSceneLight();
	light2->SetAmbientColor(1, 1, 1);
	light2->SetPosition(1, 0, 1);

	
	
	mqMorphoDigCore::instance()->getRenderer()->RemoveAllLights();
	mqMorphoDigCore::instance()->getRenderer()->AddLight(light);
	mqMorphoDigCore::instance()->getRenderer()->AddLight(light2);
	mqMorphoDigCore::instance()->Render();

}