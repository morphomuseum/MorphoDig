/*=========================================================================

   Program: MorphoDig
   Module:    mqSelectSmallObjectsDialogReaction.cxx


========================================================================*/
#include "mqSelectSmallObjectsDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqSelectSmallObjectsDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqSelectSmallObjectsDialogReaction::mqSelectSmallObjectsDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	this->SelectSmallObjects_dialog = new mqSelectSmallObjectsDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqSelectSmallObjectsDialogReaction::showSelectSmallObjectsDialog(mqSelectSmallObjectsDialog *SelectSmallObjects_dialog)
{
	
  
  SelectSmallObjects_dialog->show();
}
