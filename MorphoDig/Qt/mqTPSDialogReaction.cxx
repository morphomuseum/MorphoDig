/*=========================================================================

   Program: MorphoDig
   Module:    mqTPSDialogReaction.cxx


========================================================================*/
#include "mqTPSDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqTPSDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqTPSDialogReaction::mqTPSDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	this->TPS_dialog = new mqTPSDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqTPSDialogReaction::showTPSDialog(mqTPSDialog *TPS_dialog)
{
	
  
 TPS_dialog->show();
}
