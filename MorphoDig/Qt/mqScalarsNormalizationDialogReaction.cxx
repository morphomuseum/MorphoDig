/*=========================================================================

   Program: MorphoDig
   Module:    mqScalarsNormalizationDialogReaction.cxx


========================================================================*/
#include "mqScalarsNormalizationDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqMorphoDigCore.h"
#include "mqScalarsNormalizationDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqScalarsNormalizationDialogReaction::mqScalarsNormalizationDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	//this->Normalization_dialog = new mqScalarsNormalizationDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
//void mqScalarsNormalizationDialogReaction::showScalarsNormalizationDialog(mqScalarsNormalizationDialog *Normalization_dialog)
void mqScalarsNormalizationDialogReaction::showScalarsNormalizationDialog()
{
	vtkIdType num_surf = 0;
	num_surf = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
	if (num_surf == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}
	mqScalarsNormalizationDialog mqNormalization(mqCoreUtilities::mainWidget());
	mqNormalization.exec();
  
  //Normalization_dialog->show();
}
