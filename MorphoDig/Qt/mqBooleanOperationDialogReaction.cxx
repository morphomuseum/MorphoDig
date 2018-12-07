/*=========================================================================

   Program: MorphoDig
   Module:    mqBooleanOperationDialogReaction.cxx


========================================================================*/
#include "mqBooleanOperationDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqMorphoDigCore.h"
#include "mqBooleanOperationDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqBooleanOperationDialogReaction::mqBooleanOperationDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	//this->BooleanOperation_dialog = new mqBooleanOperationDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
//void mqBooleanOperationDialogReaction::showBooleanOperationDialog(mqBooleanOperationDialog *BooleanOperation_dialog)
void mqBooleanOperationDialogReaction::showBooleanOperationDialog()
{
	vtkIdType num_surf = 0;
	num_surf = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfItems();
	if (num_surf < 2) {
		QMessageBox msgBox;
		msgBox.setText("At least 2 opened surfaces are needed to use this option.");
		msgBox.exec();
		return;
	}
	mqBooleanOperationDialog mqBooleanOperation(mqCoreUtilities::mainWidget());
	mqBooleanOperation.exec();
  
  //BooleanOperation_dialog->show();
}
