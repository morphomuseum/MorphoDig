/*=========================================================================

   Program: MorphoDig
   Module:    mqEditAllFLGLengthDialogReaction.cxx


========================================================================*/
#include "mqEditAllFLGLengthDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqMorphoDigCore.h"
#include "mqEditAllFLGLengthDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqEditAllFLGLengthDialogReaction::mqEditAllFLGLengthDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	this->FLG_dialog = new mqEditAllFLGLengthDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqEditAllFLGLengthDialogReaction::showEditAllFLGDialog(mqEditAllFLGLengthDialog *FLG_dialog)
{
	
	vtkIdType num_flg = 0;
	num_flg = mqMorphoDigCore::instance()->getFlagLandmarkCollection()->GetNumberOfSelectedActors();
	if (num_flg == 0) {
		QMessageBox msgBox;
		msgBox.setText("No flag landmark selected. Please select at least one flag to use this option.");
		msgBox.exec();
		return;
	}
  FLG_dialog->show();
}
