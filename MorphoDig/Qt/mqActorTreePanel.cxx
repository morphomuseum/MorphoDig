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
 
  connect(mqMorphoDigCore::instance(), SIGNAL(actorsMightHaveChanged()), this, SLOT(slotRefresh()));
  
 
  
}


void mqActorTreePanel::slotRefresh()
{
	cout << "Refresh actor panel!!!" << endl;
	//mqMorphoDigCore::instance()->Render();	
	
}
