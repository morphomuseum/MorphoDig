/*=========================================================================

   Program: MorphoDig
   Module:    mqScalarsThicknessBetweenDialogReaction.cxx


========================================================================*/
#include "mqScalarsThicknessBetweenDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqMorphoDigCore.h"
#include "mqScalarsThicknessBetweenDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqScalarsThicknessBetweenDialogReaction::mqScalarsThicknessBetweenDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	//this->ThicknessBetween_dialog = new mqScalarsThicknessBetweenDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
//void mqScalarsThicknessBetweenDialogReaction::showScalarsThicknessBetweenDialog(mqScalarsThicknessBetweenDialog *ThicknessBetween_dialog)
void mqScalarsThicknessBetweenDialogReaction::showScalarsThicknessBetweenDialog()
{
	vtkIdType num_surf = 0;
	num_surf = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfItems();
	if (num_surf < 2) {
		QMessageBox msgBox;
		msgBox.setText("At least 2 opened surfaces are needed to use this option.");
		msgBox.exec();
		return;
	}
	mqScalarsThicknessBetweenDialog mqThicknessBetween(mqCoreUtilities::mainWidget());
	mqThicknessBetween.exec();
  
  //ThicknessBetween_dialog->show();
}
