/*=========================================================================

   Program: MorphoDig
   Module:    mqShrinkWrapDialogReaction.cxx


========================================================================*/
#include "mqShrinkWrapDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqMorphoDigCore.h"
#include "mqShrinkWrapDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqShrinkWrapDialogReaction::mqShrinkWrapDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	//this->ShrinkWrap_dialog = new mqShrinkWrapDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
//void mqShrinkWrapDialogReaction::showShrinkWrapDialog(mqShrinkWrapDialog *ShrinkWrap_dialog)
void mqShrinkWrapDialogReaction::showShrinkWrapDialog()
{
	vtkIdType num_surf = 0;
	num_surf = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfItems();
	if (num_surf < 2) {
		QMessageBox msgBox;
		msgBox.setText("At least 2 opened surfaces are needed to use this option.");
		msgBox.exec();
		return;
	}
	mqShrinkWrapDialog mqShrinkWrap(mqCoreUtilities::mainWidget());
	mqShrinkWrap.exec();
  
  //ShrinkWrap_dialog->show();
}
