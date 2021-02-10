/*=========================================================================

   Program: MorphoDig
   Module:    mqScalarsVolumeDialogReaction.cxx


========================================================================*/
#include "mqScalarsVolumeDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqMorphoDigCore.h"
#include "mqScalarsVolumeDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqScalarsVolumeDialogReaction::mqScalarsVolumeDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	//this->Volume_dialog = new mqScalarsVolumeDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
//void mqScalarsVolumeDialogReaction::showScalarsVolumeDialog(mqScalarsVolumeDialog *Volume_dialog)
void mqScalarsVolumeDialogReaction::showScalarsVolumeDialog()
{
	vtkIdType num_surf = 0;
	num_surf = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
	if (num_surf == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}
	mqScalarsVolumeDialog mqVolume(mqCoreUtilities::mainWidget());
	mqVolume.exec();
  
  //Volume_dialog->show();
}
