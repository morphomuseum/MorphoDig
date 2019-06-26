/*=========================================================================

   Program: MorphoDig
   Module:    mqVolumeOptionsDialogReaction.cxx


========================================================================*/
#include "mqVolumeOptionsDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqVolumeOptionsDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqVolumeOptionsDialogReaction::mqVolumeOptionsDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	

	
}

void mqVolumeOptionsDialogReaction::onTriggered()
{
	mqVolumeOptionsDialogReaction::showVolumeOptionsDialog();

}
//-----------------------------------------------------------------------------
void mqVolumeOptionsDialogReaction::showVolumeOptionsDialog()
{
	
	mqVolumeOptionsDialog volumeOptionsDialog(mqCoreUtilities::mainWidget());
	volumeOptionsDialog.exec();
  
}
