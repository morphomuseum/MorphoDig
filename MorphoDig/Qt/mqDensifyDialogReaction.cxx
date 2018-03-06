/*=========================================================================

   Program: MorphoDig
   Module:    mqDensifyDialogReaction.cxx


========================================================================*/
#include "mqDensifyDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqDensifyDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqDensifyDialogReaction::mqDensifyDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	this->Densify_dialog = new mqDensifyDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqDensifyDialogReaction::showDensifyDialog(mqDensifyDialog *Densify_dialog)
{
	
  
 Densify_dialog->show();
}
