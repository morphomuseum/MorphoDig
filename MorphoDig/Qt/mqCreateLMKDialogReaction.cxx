/*=========================================================================

   Program: MorphoDig
   Module:    mqCreateLMKDialogReaction.cxx


========================================================================*/
#include "mqCreateLMKDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqCreateLMKDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqCreateLMKDialogReaction::mqCreateLMKDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	this->lmk_dialog = new mqCreateLMKDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqCreateLMKDialogReaction::showCreateLMKDialog(mqCreateLMKDialog *lmk_dialog)
{
	
	lmk_dialog->show();
}
