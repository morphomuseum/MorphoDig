/*=========================================================================

   Program: MorphoDig
   Module:    mqScalarsSmoothDialogReaction.cxx


========================================================================*/
#include "mqScalarsSmoothDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqScalarsSmoothDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqScalarsSmoothDialogReaction::mqScalarsSmoothDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	//this->Smooth_dialog = new mqScalarsSmoothDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
//void mqScalarsSmoothDialogReaction::showScalarsSmoothDialog(mqScalarsSmoothDialog *Smooth_dialog)
void mqScalarsSmoothDialogReaction::showScalarsSmoothDialog()
{
	mqScalarsSmoothDialog mqSmooth(mqCoreUtilities::mainWidget());
	mqSmooth.exec();
  
  //Smooth_dialog->show();
}
