/*=========================================================================

   Program: MorphoDig
   Module:    mqEditTagsDialogReaction.cxx


========================================================================*/
#include "mqEditTagsDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqEditTagsDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqEditTagsDialogReaction::mqEditTagsDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	this->Tags_dialog = new mqEditTagsDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqEditTagsDialogReaction::showEditTagsDialog(mqEditTagsDialog *Tags_dialog)
{
	
  Tags_dialog->UpdateUI();
  Tags_dialog->show();
}
