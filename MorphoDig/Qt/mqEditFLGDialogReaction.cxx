/*=========================================================================

   Program: MorphoDig
   Module:    mqEditFLGDialogReaction.cxx


========================================================================*/
#include "mqEditFLGDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqEditFLGDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqEditFLGDialogReaction::mqEditFLGDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	this->FLG_dialog = new mqEditFLGDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqEditFLGDialogReaction::showEditFLGDialog(mqEditFLGDialog *FLG_dialog)
{
	
  FLG_dialog->RefreshDialog();
  FLG_dialog->show();
}
