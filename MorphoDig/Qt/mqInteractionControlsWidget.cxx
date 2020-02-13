/*=========================================================================

   Program: MorphoDig
   

========================================================================*/
#include "mqInteractionControlsWidget.h"
#include "ui_mqInteractionControlsWidget.h"

// For later!

#include "mqMorphoDigCore.h"



#include <QToolButton>


//-----------------------------------------------------------------------------
void mqInteractionControlsWidget::constructor()
{
 // Ui::mqInteractionControlsWidget ui;
 // ui.setupUi(this);
  this->ui = new Ui_mqInteractionControlsWidget;
  this->ui->setupUi(this);
  this->ui->MoveCamera->setCheckable(true);
  this->ui->MoveObjects->setCheckable(true);
  this->ui->MoveOnlyLandmarks->setCheckable(true);
  
 
  

  if (mqMorphoDigCore::instance()->Getmui_MoveMode() == 1)
  {

	  this->ui->MoveCamera->setChecked(true);
  }
  else  if (mqMorphoDigCore::instance()->Getmui_MoveMode() == 0)
  {
	  this->ui->MoveOnlyLandmarks->setChecked(true);
  }
  else
  {
	  this->ui->MoveObjects->setChecked(true);

  }


  int landmark_mode = mqMorphoDigCore::instance()->Getmui_LandmarkMode();
  if (landmark_mode == 0) { this->ui->LandmarksModeNormal->setChecked(true); }
  else if (landmark_mode == 1) { this->ui->LandmarksModeTarget->setChecked(true); }
  else if (landmark_mode == 2) { this->ui->LandmarksModeNode->setChecked(true); }
  else if (landmark_mode == 3) { this->ui->LandmarksModeHandle->setChecked(true); }
  else { this->ui->LandmarksModeFlag->setChecked(true); }//4

  connect(this->ui->LandmarksModeNormal, SIGNAL(pressed()), this, SLOT(slotLandmarkNormalMode()));
  connect(this->ui->LandmarksModeTarget, SIGNAL(pressed()), this, SLOT(slotLandmarkTargetMode()));
  connect(this->ui->LandmarksModeHandle, SIGNAL(pressed()), this, SLOT(slotLandmarkHandleMode()));
  connect(this->ui->LandmarksModeNode, SIGNAL(pressed()), this, SLOT(slotLandmarkNodeMode()));
  connect(this->ui->LandmarksModeFlag, SIGNAL(pressed()), this, SLOT(slotFlagMode()));
  connect(this->ui->MoveCamera, SIGNAL(pressed()), this, SLOT(slotMoveCamera()));
  connect(this->ui->MoveObjects, SIGNAL(pressed()), this, SLOT(slotMoveObjects()));
  connect(this->ui->MoveOnlyLandmarks, SIGNAL(pressed()), this, SLOT(slotMoveOnlyLandmarks()));
  connect(mqMorphoDigCore::instance(), SIGNAL(moveModeChanged()), this, SLOT(slotMoveModeChanged()));
  connect(mqMorphoDigCore::instance(), SIGNAL(landmarkModeChanged()), this, SLOT(slotLandmarkModeChanged()));
  connect(mqMorphoDigCore::instance(), SIGNAL(curveModeChanged()), this, SLOT(slotCurveModeChanged()));
  
}



void mqInteractionControlsWidget::slotMoveModeChanged()
{
	cout << "Slot move mode changed" << endl;
	// move mode was changed externally (ex: "Escape" pressed)
	if (mqMorphoDigCore::instance()->Getmui_MoveMode() == 0 || mqMorphoDigCore::instance()->Getmui_MoveMode() == 1) //0 lmk movemode //1 camera //2 move objects
	{
		this->ui->MoveCamera->setChecked(true);
		this->ui->MoveOnlyLandmarks->setChecked(false);
		this->ui->MoveObjects->setChecked(false);
	}
	else
	{
		this->ui->MoveObjects->setChecked(true);
		this->ui->MoveOnlyLandmarks->setChecked(false);
		this->ui->MoveCamera->setChecked(false);
	}
}

void mqInteractionControlsWidget::slotLandmarkModeChanged()
{
	cout << "Slot move mode changed" << endl;
	// move mode was changed externally (ex: "Escape" pressed)
	if (mqMorphoDigCore::instance()->Getmui_LandmarkMode() == 0 ) 
	{
		this->ui->LandmarksModeNormal->setChecked(true);
		this->ui->LandmarksModeTarget->setChecked(false);
		this->ui->LandmarksModeNode->setChecked(false);
		this->ui->LandmarksModeHandle->setChecked(false);
		this->ui->LandmarksModeFlag->setChecked(false);
		
	}
	else
	{
		
			this->ui->LandmarksModeNormal->setChecked(false);
			this->ui->LandmarksModeTarget->setChecked(true);
			this->ui->LandmarksModeNode->setChecked(false);
			this->ui->LandmarksModeHandle->setChecked(false);
			this->ui->LandmarksModeFlag->setChecked(false);

		
	}
}
void mqInteractionControlsWidget::slotCurveModeChanged()
{
	cout << "Slot move mode changed" << endl;
	// move mode was changed externally (ex: "Escape" pressed)
	if (mqMorphoDigCore::instance()->Getmui_LandmarkMode() == 2)
	{
		this->ui->LandmarksModeNormal->setChecked(false);
		this->ui->LandmarksModeTarget->setChecked(false);
		this->ui->LandmarksModeNode->setChecked(true);
		this->ui->LandmarksModeHandle->setChecked(false);
		this->ui->LandmarksModeFlag->setChecked(false);

	}
	else
	{
		
			this->ui->LandmarksModeNormal->setChecked(false);
			this->ui->LandmarksModeTarget->setChecked(false);
			this->ui->LandmarksModeNode->setChecked(false);
			this->ui->LandmarksModeHandle->setChecked(true);
			this->ui->LandmarksModeFlag->setChecked(false);

		
	}
}

void mqInteractionControlsWidget::slotMoveCamera()
{
	cout << "Slot move camera" << endl;
	
	//	this->ui->MoveCamera->setChecked(true);
	if (this->ui->MoveCamera->isChecked())
	{
		this->ui->MoveCamera->setChecked(false);
	}

		this->ui->MoveOnlyLandmarks->setChecked(false);
		this->ui->MoveObjects->setChecked(false);
	
	mqMorphoDigCore::instance()->Setmui_MoveMode(1);
	mqMorphoDigCore::instance()->setCurrentCursor(0);

}

void mqInteractionControlsWidget::slotMoveObjects()
{
	cout << "Slot move objects" << endl;
	if (this->ui->MoveObjects->isChecked())
	{
		this->ui->MoveObjects->setChecked(false);
	}
	/*
	*/
	this->ui->MoveOnlyLandmarks->setChecked(false);
	this->ui->MoveCamera->setChecked(false);
	mqMorphoDigCore::instance()->Setmui_MoveMode(2);
	mqMorphoDigCore::instance()->setCurrentCursor(1);

}

void mqInteractionControlsWidget::slotMoveOnlyLandmarks()
{
	
	cout << "Slot move only landmarks" << endl;
	if (this->ui->MoveOnlyLandmarks->isChecked())
	{
		this->ui->MoveOnlyLandmarks->setChecked(false);
	}
		//this->ui->MoveOnlyLandmarks->setChecked(true);
		this->ui->MoveCamera->setChecked(false);		
		this->ui->MoveObjects->setChecked(false);
		mqMorphoDigCore::instance()->setCurrentCursor(0);
	

	mqMorphoDigCore::instance()->Setmui_MoveMode(0);
	mqMorphoDigCore::instance()->Render();

}

void mqInteractionControlsWidget::slotLandmarkNormalMode()
{
	cout << "Landmark setting mode: 0" << endl;
	if (this->ui->LandmarksModeNormal->isChecked())
	{
		this->ui->LandmarksModeNormal->setChecked(false);
	}
	this->ui->LandmarksModeTarget->setChecked(false);
	this->ui->LandmarksModeNode->setChecked(false);
	this->ui->LandmarksModeHandle->setChecked(false);
	this->ui->LandmarksModeFlag->setChecked(false);
	mqMorphoDigCore::instance()->Setmui_LandmarkMode(0);
	
}
void mqInteractionControlsWidget::slotLandmarkTargetMode()
{
	cout << "Landmark setting mode: 1" << endl;
	if (this->ui->LandmarksModeTarget->isChecked())
	{
		this->ui->LandmarksModeTarget->setChecked(false);
	}
	this->ui->LandmarksModeNormal->setChecked(false);
	this->ui->LandmarksModeNode->setChecked(false);
	this->ui->LandmarksModeHandle->setChecked(false);
	this->ui->LandmarksModeFlag->setChecked(false);
	mqMorphoDigCore::instance()->Setmui_LandmarkMode(1);

}
void mqInteractionControlsWidget::slotLandmarkNodeMode()
{
	if (this->ui->LandmarksModeNode->isChecked())
	{
		this->ui->LandmarksModeNode->setChecked(false);
	}
	cout << "Landmark setting mode: 2" << endl;
	this->ui->LandmarksModeNormal->setChecked(false);
	this->ui->LandmarksModeTarget->setChecked(false);
	this->ui->LandmarksModeHandle->setChecked(false);
	this->ui->LandmarksModeFlag->setChecked(false);
	mqMorphoDigCore::instance()->Setmui_LandmarkMode(2);
}
void mqInteractionControlsWidget::slotLandmarkHandleMode()
{
	if (this->ui->LandmarksModeHandle->isChecked())
	{
		this->ui->LandmarksModeHandle->setChecked(false);
	}
	cout << "Landmark setting mode: 3" << endl;
	this->ui->LandmarksModeNormal->setChecked(false);
	this->ui->LandmarksModeTarget->setChecked(false);
	this->ui->LandmarksModeNode->setChecked(false);
	this->ui->LandmarksModeFlag->setChecked(false);
	mqMorphoDigCore::instance()->Setmui_LandmarkMode(3);
	

}
void mqInteractionControlsWidget::slotFlagMode()
{
	if (this->ui->LandmarksModeFlag->isChecked())
	{
		this->ui->LandmarksModeFlag->setChecked(false);
	}
	cout << "Landmark setting mode: 4=flags" << endl;
	this->ui->LandmarksModeNormal->setChecked(false);
	this->ui->LandmarksModeTarget->setChecked(false);
	this->ui->LandmarksModeNode->setChecked(false);
	this->ui->LandmarksModeHandle->setChecked(false);
	
	mqMorphoDigCore::instance()->Setmui_LandmarkMode(4);


}
