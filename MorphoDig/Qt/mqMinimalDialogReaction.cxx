/*=========================================================================

   Program: MorphoDig
   Module:    mqMinimalDialogReaction.cxx


========================================================================*/
#include "mqMinimalDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqMinimalDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqMinimalDialogReaction::mqMinimalDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	this->Minimal_dialog = new mqMinimalDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqMinimalDialogReaction::showMinimalDialog(mqMinimalDialog *Minimal_dialog)
{
	
  
 Minimal_dialog->show();
}
