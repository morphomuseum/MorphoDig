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
	
	
	vtkIdType num_lmk = 0;
	num_lmk = mqMorphoDigCore::instance()->getNormalLandmarkCollection()->GetNumberOfItems();
	num_lmk+= mqMorphoDigCore::instance()->getTargetLandmarkCollection()->GetNumberOfItems();
	num_lmk += mqMorphoDigCore::instance()->getNodeLandmarkCollection()->GetNumberOfItems();
	num_lmk += mqMorphoDigCore::instance()->getHandleLandmarkCollection()->GetNumberOfItems();
	if (num_lmk == 0) {
		QMessageBox msgBox;
		msgBox.setText("Please create or load landmarks in order to be able to use this option.");
		msgBox.exec();
		return;
	}
		mqSelectLandmarkRangeDialogReaction::showSelectLandmarkRangeDialog();
		
		
		
}

//-----------------------------------------------------------------------------
void mqSelectLandmarkRangeDialogReaction::showSelectLandmarkRangeDialog()
{
	// fonction statique. C'est � dire que lorsqu'on clique sur SAVE Landmark, on cr�e un nouvel objet.
	// la r�action, elle, est bien instanci�e � la cr�ation du menu, mais pas la fen�tre. 

  mqSelectLandmarkRangeDialog SaveLandmark_dialog(mqCoreUtilities::mainWidget());
  SaveLandmark_dialog.exec();
}
