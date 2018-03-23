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
#include "mqDisplayControlsToolbar.h"
#include "ui_mqDisplayControlsToolbar.h"

// For later!

#include "mqMorphoDigCore.h"

#include "mqDisplayReaction.h"
#include <vtkRenderer.h>
#include <QToolButton>


//-----------------------------------------------------------------------------
void mqDisplayControlsToolbar::constructor()
{
 // Ui::mqDisplayControlsToolbar ui;
 // ui.setupUi(this);
  this->ui = new Ui_mqDisplayControlsToolbar;
  this->ui->setupUi(this);
  
  //new mqSaveDataReaction(this->ui->actionSaveData);
 

  

  if (mqMorphoDigCore::instance()->Getmui_Anaglyph() == 1)
  {

	  this->ui->actionRendererAnaglyphToggle->setChecked(true);
  }

  if (mqMorphoDigCore::instance()->Getmui_ShowGrid() == 1)
  {

	  this->ui->actionGridToggle->setChecked(true);
  }
  if (mqMorphoDigCore::instance()->Getmui_ShowOrientationHelper() == 1)
  {

	  this->ui->actionOrientationHelperToggle->setChecked(true);
  }



  new mqDisplayReaction(this->ui->actionGridToggle, 0); //0 = display Grid Toggle
  new mqDisplayReaction(this->ui->actionOrientationHelperToggle, 1); //1 = display Orientation Helper Toggle
  new mqDisplayReaction(this->ui->actionRendererAnaglyphToggle, 2); //2 = display Anaglyph mode Toggle

  connect(this->ui->actionBackfaceCullingOnOff, SIGNAL(triggered()), this, SLOT(slotBackfaceCullingOnOff()));
  connect(this->ui->actionClippingPlaneOnOff, SIGNAL(triggered()), this, SLOT(slotClippingPlaneOnOff()));

  
  
}

void mqDisplayControlsToolbar::slotClippingPlaneOnOff()
{
	mqMorphoDigCore::instance()->getRenderer()->ResetCameraClippingRange();
	mqMorphoDigCore::instance()->ChangeClippingPlane();
	mqMorphoDigCore::instance()->Render();
	//mqMorphoDigCore::instance()->Render();
}

void mqDisplayControlsToolbar::slotBackfaceCullingOnOff()
{

	mqMorphoDigCore::instance()->ChangeBackfaceCulling();
	mqMorphoDigCore::instance()->Render();
	//mqMorphoDigCore::instance()->Render();
}

