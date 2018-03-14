/*=========================================================================

   Program: MorphoDig
  
========================================================================*/
#include "mqActorTreePanel.h"
#include "ui_mqActorTreePanel.h"

// For later!

#include "mqMorphoDigCore.h"
#include <vtkLight.h>
#include <vtkRenderer.h>
#include <QToolButton>


//-----------------------------------------------------------------------------
void mqActorTreePanel::constructor()
{
 // Ui::mqLightControlsToolbar ui;
 // ui.setupUi(this);
  this->ui = new Ui_mqActorTreePanel;
  this->ui->setupUi(this);
 
  //connect(this->ui->actionFrontLight, SIGNAL(triggered()), this, SLOT(slotFrontLight()));
  
 
  
}


void mqActorTreePanel::slotSomething()
{
	
	//mqMorphoDigCore::instance()->Render();	
	
}
