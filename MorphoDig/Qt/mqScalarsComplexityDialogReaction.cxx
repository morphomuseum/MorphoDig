/*=========================================================================

   Program: MorphoDig
   Module:    mqScalarsComplexityDialogReaction.cxx


========================================================================*/
#include "mqScalarsComplexityDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqMorphoDigCore.h"
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
	vtkIdType num_surf = 0;
	num_surf = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
	if (num_surf == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}
	mqScalarsComplexityDialog mqComplexity(mqCoreUtilities::mainWidget());
	mqComplexity.exec();
  
  //Complexity_dialog->show();
}
