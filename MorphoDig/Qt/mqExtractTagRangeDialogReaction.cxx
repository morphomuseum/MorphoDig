/*=========================================================================

   Program: MorphoDig
   Module:    mqExtractTagRangeDialogReaction.cxx


========================================================================*/
#include "mqExtractTagRangeDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqExtractTagRangeDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqExtractTagRangeDialogReaction::mqExtractTagRangeDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	
}


//-----------------------------------------------------------------------------
void mqExtractTagRangeDialogReaction::showExtractTagRangeDialog()
{
	mqExtractTagRangeDialog etr_dialog(mqCoreUtilities::mainWidget());

	etr_dialog.exec();
}
