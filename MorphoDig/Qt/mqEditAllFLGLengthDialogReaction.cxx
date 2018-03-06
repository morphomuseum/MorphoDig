/*=========================================================================

   Program: MorphoDig
   Module:    mqEditAllFLGLengthDialogReaction.cxx


========================================================================*/
#include "mqEditAllFLGLengthDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqEditAllFLGLengthDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqEditAllFLGLengthDialogReaction::mqEditAllFLGLengthDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	this->FLG_dialog = new mqEditAllFLGLengthDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqEditAllFLGLengthDialogReaction::showEditAllFLGDialog(mqEditAllFLGLengthDialog *FLG_dialog)
{
	
  
  FLG_dialog->show();
}
