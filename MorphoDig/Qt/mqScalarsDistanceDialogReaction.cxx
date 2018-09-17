/*=========================================================================

   Program: MorphoDig
   Module:    mqScalarsDistanceDialogReaction.cxx


========================================================================*/
#include "mqScalarsDistanceDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqMorphoDigCore.h"
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
	vtkIdType num_surf = 0;
	num_surf = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfItems();
	if (num_surf < 2) {
		QMessageBox msgBox;
		msgBox.setText("At least 2 opened surfaces are needed to use this option.");
		msgBox.exec();
		return;
	}
	mqScalarsDistanceDialog mqScalarsDistance(mqCoreUtilities::mainWidget());
	mqScalarsDistance.exec();
  
  //ScalarsDistance_dialog->show();
}
