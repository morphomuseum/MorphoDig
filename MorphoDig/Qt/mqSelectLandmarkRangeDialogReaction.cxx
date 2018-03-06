/*=========================================================================

   Program: MorphoDig
   Module:    mqSelectLandmarkRange.cxx


========================================================================*/
#include "mqSelectLandmarkRangeDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqSelectLandmarkRangeDialog.h"
#include "mqReaction.h"
#include "mqMorphoDigCore.h"
#include <vtkType.h>
#include <QFileDialog>
#include <QMessageBox>

//-----------------------------------------------------------------------------
mqSelectLandmarkRangeDialogReaction::mqSelectLandmarkRangeDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	
}

void mqSelectLandmarkRangeDialogReaction::onTriggered()
{
	
	

		mqSelectLandmarkRangeDialogReaction::showSelectLandmarkRangeDialog();
		
		
		
}

//-----------------------------------------------------------------------------
void mqSelectLandmarkRangeDialogReaction::showSelectLandmarkRangeDialog()
{
	// fonction statique. C'est à dire que lorsqu'on clique sur SAVE Landmark, on crée un nouvel objet.
	// la réaction, elle, est bien instanciée à la création du menu, mais pas la fenêtre. 

  mqSelectLandmarkRangeDialog SaveLandmark_dialog(mqCoreUtilities::mainWidget());
  SaveLandmark_dialog.exec();
}
