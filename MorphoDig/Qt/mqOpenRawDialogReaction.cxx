/*=========================================================================

   Program: MorphoDig
   Module:    mqIsosurfaceDialogReaction.cxx


========================================================================*/
#include "mqIsosurfaceDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqIsosurfaceDialog.h"
#include "mqReaction.h"

//-----------------------------------------------------------------------------
mqIsosurfaceDialogReaction::mqIsosurfaceDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	/* mqAboutDialog about_dialog(mqCoreUtilities::mainWidget());
  about_dialog.exec();*/
	//this->Isosurface_dialog = new mqIsosurfaceDialog(mqCoreUtilities::mainWidget());
	
	
}


//-----------------------------------------------------------------------------
void mqIsosurfaceDialogReaction::showIsosurfaceDialog(vtkMDVolume *vol)
{
	mqIsosurfaceDialog Isosurface_dialog(mqCoreUtilities::mainWidget());
	Isosurface_dialog.setVolume(vol);
 Isosurface_dialog.exec();
}
