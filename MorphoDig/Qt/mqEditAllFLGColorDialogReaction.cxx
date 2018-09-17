/*=========================================================================

   Program: MorphoDig
   Module:    mqEditAllFLGColorDialogReaction.cxx


========================================================================*/
#include "mqEditAllFLGColorDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqMorphoDigCore.h"
#include "mqEditAllFLGColorDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqEditAllFLGColorDialogReaction::mqEditAllFLGColorDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	this->FLG_dialog = new mqEditAllFLGColorDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqEditAllFLGColorDialogReaction::showEditAllFLGDialog(mqEditAllFLGColorDialog *FLG_dialog)
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
