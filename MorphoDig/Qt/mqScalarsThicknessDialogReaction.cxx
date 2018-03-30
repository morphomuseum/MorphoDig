/*=========================================================================

   Program: MorphoDig
   Module:    mqScalarsThicknessDialogReaction.cxx


========================================================================*/
#include "mqScalarsThicknessDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqScalarsThicknessDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqScalarsThicknessDialogReaction::mqScalarsThicknessDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	//this->Thickness_dialog = new mqScalarsThicknessDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
//void mqScalarsThicknessDialogReaction::showScalarsThicknessDialog(mqScalarsThicknessDialog *Thickness_dialog)
void mqScalarsThicknessDialogReaction::showScalarsThicknessDialog()
{
	mqScalarsThicknessDialog mqThickness(mqCoreUtilities::mainWidget());
	mqThickness.exec();
  
  //Thickness_dialog->show();
}
