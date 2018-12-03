/*=========================================================================

   Program: MorphoDig
   Module:    mqExtractScalarRangeDialogReaction.cxx


========================================================================*/
#include "mqExtractScalarRangeDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqExtractScalarRangeDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqExtractScalarRangeDialogReaction::mqExtractScalarRangeDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	//this->esr_dialog = new mqExtractScalarRangeDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqExtractScalarRangeDialogReaction::showExtractScalarRangeDialog()
{
	mqExtractScalarRangeDialog esr_dialog(mqCoreUtilities::mainWidget());
	
	esr_dialog.exec();
	
}
