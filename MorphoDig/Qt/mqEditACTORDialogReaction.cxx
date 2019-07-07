/*=========================================================================

   Program: MorphoDig
   Module:    mqEditACTORDialogReaction.cxx


========================================================================*/
#include "mqEditACTORDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqEditACTORDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqEditACTORDialogReaction::mqEditACTORDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	this->ACTOR_dialog = new mqEditACTORDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqEditACTORDialogReaction::showEditACTORDialog(mqEditACTORDialog *ACTOR_dialog)
{
	
	ACTOR_dialog->ShowAndRefresh();
  ACTOR_dialog->show();
}
