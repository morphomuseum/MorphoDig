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
	this->Isosurface_dialog = new mqIsosurfaceDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqIsosurfaceDialogReaction::showIsosurfaceDialog(mqIsosurfaceDialog *Isosurface_dialog)
{
	
  
 Isosurface_dialog->show();
}
