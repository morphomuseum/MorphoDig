/*=========================================================================

   Program: MorphoDig
   Module:    mqScalarsThicknessBetweenDialogReaction.cxx


========================================================================*/
#include "mqScalarsThicknessBetweenDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqScalarsThicknessBetweenDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqScalarsThicknessBetweenDialogReaction::mqScalarsThicknessBetweenDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	//this->ThicknessBetween_dialog = new mqScalarsThicknessBetweenDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
//void mqScalarsThicknessBetweenDialogReaction::showScalarsThicknessBetweenDialog(mqScalarsThicknessBetweenDialog *ThicknessBetween_dialog)
void mqScalarsThicknessBetweenDialogReaction::showScalarsThicknessBetweenDialog()
{
	mqScalarsThicknessBetweenDialog mqThicknessBetween(mqCoreUtilities::mainWidget());
	mqThicknessBetween.exec();
  
  //ThicknessBetween_dialog->show();
}
