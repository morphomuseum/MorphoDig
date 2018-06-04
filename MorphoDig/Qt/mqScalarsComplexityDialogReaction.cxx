/*=========================================================================

   Program: MorphoDig
   Module:    mqScalarsComplexityDialogReaction.cxx


========================================================================*/
#include "mqScalarsComplexityDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqScalarsComplexityDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqScalarsComplexityDialogReaction::mqScalarsComplexityDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	//this->Complexity_dialog = new mqScalarsComplexityDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
//void mqScalarsComplexityDialogReaction::showScalarsComplexityDialog(mqScalarsComplexityDialog *Complexity_dialog)
void mqScalarsComplexityDialogReaction::showScalarsComplexityDialog()
{
	mqScalarsComplexityDialog mqComplexity(mqCoreUtilities::mainWidget());
	mqComplexity.exec();
  
  //Complexity_dialog->show();
}
