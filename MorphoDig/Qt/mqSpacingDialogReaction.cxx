/*=========================================================================

   Program: MorphoDig
   Module:    mqSpacingDialogReaction.cxx


========================================================================*/
#include "mqSpacingDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqSpacingDialog.h"
#include "mqReaction.h"

//-----------------------------------------------------------------------------
mqSpacingDialogReaction::mqSpacingDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	/* mqAboutDialog about_dialog(mqCoreUtilities::mainWidget());
  about_dialog.exec();*/
	//this->Spacing_dialog = new mqSpacingDialog(mqCoreUtilities::mainWidget());
	
	
}


//-----------------------------------------------------------------------------
void mqSpacingDialogReaction::showSpacingDialog(vtkMDVolume *vol)
{
	mqSpacingDialog Spacing_dialog(mqCoreUtilities::mainWidget());
	Spacing_dialog.setVolume(vol);
 Spacing_dialog.exec();
}
