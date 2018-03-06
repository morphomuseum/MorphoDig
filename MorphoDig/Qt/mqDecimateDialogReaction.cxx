/*=========================================================================

   Program: MorphoDig
   Module:    mqDecimateDialogReaction.cxx


========================================================================*/
#include "mqDecimateDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqDecimateDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqDecimateDialogReaction::mqDecimateDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	this->Decimate_dialog = new mqDecimateDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqDecimateDialogReaction::showDecimateDialog(mqDecimateDialog *Decimate_dialog)
{
	
  
 Decimate_dialog->show();
}
