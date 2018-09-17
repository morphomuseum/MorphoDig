/*=========================================================================

   Program: MorphoDig
   Module:    mqScalarsThicknessDialogReaction.cxx


========================================================================*/
#include "mqScalarsThicknessDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqMorphoDigCore.h"
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
	vtkIdType num_surf = 0;
	num_surf = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
	if (num_surf == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}
	mqScalarsThicknessDialog mqThickness(mqCoreUtilities::mainWidget());
	mqThickness.exec();
  
  //Thickness_dialog->show();
}
