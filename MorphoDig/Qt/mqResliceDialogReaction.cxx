/*=========================================================================

   Program: MorphoDig
   Module:    mqResliceDialogReaction.cxx


========================================================================*/
#include "mqResliceDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqResliceDialog.h"
#include "mqReaction.h"

//-----------------------------------------------------------------------------
mqResliceDialogReaction::mqResliceDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	/* mqAboutDialog about_dialog(mqCoreUtilities::mainWidget());
  about_dialog.exec();*/
	//this->Reslice_dialog = new mqResliceDialog(mqCoreUtilities::mainWidget());
	
	
}


//-----------------------------------------------------------------------------
void mqResliceDialogReaction::showResliceDialog(vtkMDVolume *vol)
{
	mqResliceDialog Reslice_dialog(mqCoreUtilities::mainWidget());
	Reslice_dialog.setVolume(vol);
 Reslice_dialog.exec();
}
