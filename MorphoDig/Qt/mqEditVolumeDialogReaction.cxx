/*=========================================================================

   Program: MorphoDig
   Module:    mqEditVolumeDialogReaction.cxx


========================================================================*/
#include "mqEditVolumeDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqEditVolumeDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqEditVolumeDialogReaction::mqEditVolumeDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	this->Volume_dialog = new mqEditVolumeDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqEditVolumeDialogReaction::showEditVolumeDialog(mqEditVolumeDialog *Volume_dialog)
{
	
	Volume_dialog->ShowAndRefresh();
  Volume_dialog->show();
}
