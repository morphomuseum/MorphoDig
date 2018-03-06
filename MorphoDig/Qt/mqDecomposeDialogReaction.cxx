/*=========================================================================

   Program: MorphoDig
   Module:    mqDecomposeDialogReaction.cxx


========================================================================*/
#include "mqDecomposeDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqDecomposeDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqDecomposeDialogReaction::mqDecomposeDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	this->Decompose_dialog = new mqDecomposeDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqDecomposeDialogReaction::showDecomposeDialog(mqDecomposeDialog *Decompose_dialog)
{
	
  
 Decompose_dialog->show();
}
