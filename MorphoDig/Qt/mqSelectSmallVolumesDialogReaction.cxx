/*=========================================================================

   Program: MorphoDig
   Module:    mqSelectSmallVolumesDialogReaction.cxx


========================================================================*/
#include "mqSelectSmallVolumesDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqSelectSmallVolumesDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqSelectSmallVolumesDialogReaction::mqSelectSmallVolumesDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	this->SelectSmallVolumes_dialog = new mqSelectSmallVolumesDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqSelectSmallVolumesDialogReaction::showSelectSmallVolumesDialog(mqSelectSmallVolumesDialog *SelectSmallVolumes_dialog)
{
	
  
  SelectSmallVolumes_dialog->show();
}
