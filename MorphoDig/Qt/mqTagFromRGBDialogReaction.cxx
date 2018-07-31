/*=========================================================================

   Program: MorphoDig
   Module:    mqTagFromRGBDialogReaction.cxx


========================================================================*/
#include "mqTagFromRGBDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqTagFromRGBDialog.h"
#include "mqReaction.h"

//-----------------------------------------------------------------------------
mqTagFromRGBDialogReaction::mqTagFromRGBDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	this->TagFromRGB_dialog = new mqTagFromRGBDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqTagFromRGBDialogReaction::showTagFromRGBDialog(mqTagFromRGBDialog *TagFromRGB_dialog)
{
	
  
 TagFromRGB_dialog->show();
}
