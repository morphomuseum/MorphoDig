/*=========================================================================

   Program: MorphoDig
   Module:    mqGridSizeDialogReaction.cxx


========================================================================*/
#include "mqGridSizeDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqGridSizeDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqGridSizeDialogReaction::mqGridSizeDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	this->GridSize_dialog = new mqGridSizeDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqGridSizeDialogReaction::showGridSizeDialog(mqGridSizeDialog *GridSize_dialog)
{
	
	GridSize_dialog->Refresh();
  GridSize_dialog->show();
}
