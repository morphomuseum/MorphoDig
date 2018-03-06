/*=========================================================================

   Program: MorphoDig
   Module:    mqOrientationLabelsDialogReaction.cxx


========================================================================*/
#include "mqOrientationLabelsDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqOrientationLabelsDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqOrientationLabelsDialogReaction::mqOrientationLabelsDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	this->OrientationLabels_dialog = new mqOrientationLabelsDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqOrientationLabelsDialogReaction::showOrientationLabelsDialog(mqOrientationLabelsDialog *OrientationLabels_dialog)
{
	
  
  OrientationLabels_dialog->show();
}
