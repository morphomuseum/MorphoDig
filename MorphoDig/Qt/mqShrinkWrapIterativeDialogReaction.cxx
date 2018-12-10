/*=========================================================================

   Program: MorphoDig
   Module:    mqShrinkWrapIterativeDialogReaction.cxx


========================================================================*/
#include "mqShrinkWrapIterativeDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqMorphoDigCore.h"
#include "mqShrinkWrapIterativeDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqShrinkWrapIterativeDialogReaction::mqShrinkWrapIterativeDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	//this->IterativeShrinkWrap_dialog = new mqShrinkWrapIterativeDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
//void mqShrinkWrapIterativeDialogReaction::showShrinkWrapIterativeDialog(mqShrinkWrapIterativeDialog *IterativeShrinkWrap_dialog)
void mqShrinkWrapIterativeDialogReaction::showShrinkWrapIterativeDialog()
{
	vtkIdType num_surf = 0;
	num_surf = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfItems();
	if (num_surf < 2) {
		QMessageBox msgBox;
		msgBox.setText("At least 2 opened surfaces are needed to use this option.");
		msgBox.exec();
		return;
	}
	mqShrinkWrapIterativeDialog mqIterativeShrinkWrap(mqCoreUtilities::mainWidget());
	mqIterativeShrinkWrap.exec();
  
  //IterativeShrinkWrap_dialog->show();
}
