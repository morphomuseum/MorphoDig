/*=========================================================================

   Program: MorphoDig
   Module:    mqResampleDialogReaction.cxx


========================================================================*/
#include "mqResampleDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqResampleDialog.h"
#include "mqReaction.h"

//-----------------------------------------------------------------------------
mqResampleDialogReaction::mqResampleDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	/* mqAboutDialog about_dialog(mqCoreUtilities::mainWidget());
  about_dialog.exec();*/
	//this->Resample_dialog = new mqResampleDialog(mqCoreUtilities::mainWidget());
	
	
}


//-----------------------------------------------------------------------------
void mqResampleDialogReaction::showResampleDialog(vtkMDVolume *vol)
{
	mqResampleDialog Resample_dialog(mqCoreUtilities::mainWidget());
	Resample_dialog.setVolume(vol);
 Resample_dialog.exec();
}
