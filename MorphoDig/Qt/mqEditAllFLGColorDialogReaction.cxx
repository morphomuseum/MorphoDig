/*=========================================================================

   Program: MorphoDig
   Module:    mqEditAllFLGColorDialogReaction.cxx


========================================================================*/
#include "mqEditAllFLGColorDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqEditAllFLGColorDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqEditAllFLGColorDialogReaction::mqEditAllFLGColorDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	this->FLG_dialog = new mqEditAllFLGColorDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqEditAllFLGColorDialogReaction::showEditAllFLGDialog(mqEditAllFLGColorDialog *FLG_dialog)
{
	
  
  FLG_dialog->show();
}
