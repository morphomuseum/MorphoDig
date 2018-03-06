/*=========================================================================

   Program: MorphoDig
   Module:    mqSmoothDialogReaction.cxx


========================================================================*/
#include "mqSmoothDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqSmoothDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqSmoothDialogReaction::mqSmoothDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	this->SMOOTH_dialog = new mqSmoothDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqSmoothDialogReaction::showSmoothDialog(mqSmoothDialog *SMOOTH_dialog)
{
	
  
 SMOOTH_dialog->show();
}
