/*=========================================================================

   Program: MorphoDig
   Module:    mqScalarsDistanceDialogReaction.cxx


========================================================================*/
#include "mqScalarsDistanceDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqScalarsDistanceDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqScalarsDistanceDialogReaction::mqScalarsDistanceDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	//this->ScalarsDistance_dialog = new mqScalarsDistanceDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------

void mqScalarsDistanceDialogReaction::showScalarsDistanceDialog()
{
	mqScalarsDistanceDialog mqScalarsDistance(mqCoreUtilities::mainWidget());
	mqScalarsDistance.exec();
  
  //ScalarsDistance_dialog->show();
}
