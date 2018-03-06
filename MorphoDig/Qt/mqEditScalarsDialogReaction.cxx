/*=========================================================================

   Program: MorphoDig
   Module:    mqEditScalarsDialogReaction.cxx


========================================================================*/
#include "mqEditScalarsDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqEditScalarsDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqEditScalarsDialogReaction::mqEditScalarsDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	this->Scalars_dialog = new mqEditScalarsDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqEditScalarsDialogReaction::showEditScalarsDialog(mqEditScalarsDialog *Scalars_dialog)
{
	
  Scalars_dialog->UpdateUI();
  Scalars_dialog->show();
}
