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
#include "mqInteractionControlsToolbar.h"
#include "ui_mqInteractionControlsToolbar.h"

// For later!

#include "mqMorphoDigCore.h"


#include "mqDisplayReaction.h"

#include <QToolButton>


//-----------------------------------------------------------------------------
void mqInteractionControlsToolbar::constructor()
{
 // Ui::mqInteractionControlsToolbar ui;
 // ui.setupUi(this);
  this->ui = new Ui_mqInteractionControlsToolbar;
  this->ui->setupUi(this);
  
  
 
  

  if (mqMorphoDigCore::instance()->Getmui_MoveAll() == 1)
  {

	  this->ui->actionMoveAll->setChecked(true);
  }
  else
  {
	  this->ui->actionMoveOnlyLandmarks->setChecked(true);
  }


  int landmark_mode = mqMorphoDigCore::instance()->Getmui_LandmarkMode();
  if (landmark_mode == 0) { this->ui->actionLandmarksModeNormal->setChecked(true); }
  else if (landmark_mode == 1) { this->ui->actionLandmarksModeTarget->setChecked(true); }
  else if (landmark_mode == 2) { this->ui->actionLandmarksModeNode->setChecked(true); }
  else if (landmark_mode == 3) { this->ui->actionLandmarksModeHandle->setChecked(true); }
  else { this->ui->actionLandmarksModeFlag->setChecked(true); }//4

  connect(this->ui->actionLandmarksModeNormal, SIGNAL(triggered()), this, SLOT(slotLandmarkNormalMode()));
  connect(this->ui->actionLandmarksModeTarget, SIGNAL(triggered()), this, SLOT(slotLandmarkTargetMode()));
  connect(this->ui->actionLandmarksModeHandle, SIGNAL(triggered()), this, SLOT(slotLandmarkHandleMode()));
  connect(this->ui->actionLandmarksModeNode, SIGNAL(triggered()), this, SLOT(slotLandmarkNodeMode()));
  connect(this->ui->actionLandmarksModeFlag, SIGNAL(triggered()), this, SLOT(slotFlagMode()));
  connect(this->ui->actionMoveAll, SIGNAL(triggered()), this, SLOT(slotMoveAll()));
  connect(this->ui->actionMoveOnlyLandmarks, SIGNAL(triggered()), this, SLOT(slotMoveOnlyLandmarks()));
  
  
}




void mqInteractionControlsToolbar::slotMoveAll()
{
	
		this->ui->actionMoveAll->setChecked(true);
		this->ui->actionMoveOnlyLandmarks->setChecked(false);

	
	mqMorphoDigCore::instance()->Setmui_MoveAll(1);

}

void mqInteractionControlsToolbar::slotMoveOnlyLandmarks()
{
	

		this->ui->actionMoveAll->setChecked(false);
		this->ui->actionMoveOnlyLandmarks->setChecked(true);


	

	mqMorphoDigCore::instance()->Setmui_MoveAll(0);
	mqMorphoDigCore::instance()->Render();

}

void mqInteractionControlsToolbar::slotLandmarkNormalMode()
{
	cout << "Landmark setting mode: 0" << endl;
	
	this->ui->actionLandmarksModeTarget->setChecked(false);
	this->ui->actionLandmarksModeNode->setChecked(false);
	this->ui->actionLandmarksModeHandle->setChecked(false);
	this->ui->actionLandmarksModeFlag->setChecked(false);
	mqMorphoDigCore::instance()->Setmui_LandmarkMode(0);
	
}
void mqInteractionControlsToolbar::slotLandmarkTargetMode()
{
	cout << "Landmark setting mode: 1" << endl;
	this->ui->actionLandmarksModeNormal->setChecked(false);
	this->ui->actionLandmarksModeNode->setChecked(false);
	this->ui->actionLandmarksModeHandle->setChecked(false);
	this->ui->actionLandmarksModeFlag->setChecked(false);
	mqMorphoDigCore::instance()->Setmui_LandmarkMode(1);

}
void mqInteractionControlsToolbar::slotLandmarkNodeMode()
{
	cout << "Landmark setting mode: 2" << endl;
	this->ui->actionLandmarksModeNormal->setChecked(false);
	this->ui->actionLandmarksModeTarget->setChecked(false);
	this->ui->actionLandmarksModeHandle->setChecked(false);
	this->ui->actionLandmarksModeFlag->setChecked(false);
	mqMorphoDigCore::instance()->Setmui_LandmarkMode(2);
}
void mqInteractionControlsToolbar::slotLandmarkHandleMode()
{
	cout << "Landmark setting mode: 3" << endl;
	this->ui->actionLandmarksModeNormal->setChecked(false);
	this->ui->actionLandmarksModeTarget->setChecked(false);
	this->ui->actionLandmarksModeNode->setChecked(false);
	this->ui->actionLandmarksModeFlag->setChecked(false);
	mqMorphoDigCore::instance()->Setmui_LandmarkMode(3);
	

}
void mqInteractionControlsToolbar::slotFlagMode()
{
	cout << "Landmark setting mode: 4=flags" << endl;
	this->ui->actionLandmarksModeNormal->setChecked(false);
	this->ui->actionLandmarksModeTarget->setChecked(false);
	this->ui->actionLandmarksModeNode->setChecked(false);
	this->ui->actionLandmarksModeHandle->setChecked(false);
	
	mqMorphoDigCore::instance()->Setmui_LandmarkMode(4);


}
