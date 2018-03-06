/*=========================================================================

   Program: MorphoDig
   Module:    mqEditLMKDialogReaction.cxx


========================================================================*/
#include "mqEditLMKDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqEditLMKDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqEditLMKDialogReaction::mqEditLMKDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	this->lmk_dialog = new mqEditLMKDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqEditLMKDialogReaction::showEditLMKDialog(mqEditLMKDialog *lmk_dialog)
{
	
  lmk_dialog->RefreshDialog();
  lmk_dialog->show();
}
