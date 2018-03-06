/*=========================================================================

   Program: MorphoDig
   Module:    mqEditAlphaDialogReaction.cxx


========================================================================*/
#include "mqEditAlphaDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqEditAlphaDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqEditAlphaDialogReaction::mqEditAlphaDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	this->Alpha_dialog = new mqEditAlphaDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqEditAlphaDialogReaction::showEditAlphaDialog(mqEditAlphaDialog *Alpha_dialog)
{
	
  
  Alpha_dialog->show();
}
