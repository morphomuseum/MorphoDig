/*=========================================================================

   Program: MorphoDig
   Module:    mqScalarsSmoothDialogReaction.cxx


========================================================================*/
#include "mqScalarsSmoothDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqMorphoDigCore.h"
#include "mqScalarsSmoothDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqScalarsSmoothDialogReaction::mqScalarsSmoothDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	//this->Smooth_dialog = new mqScalarsSmoothDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
//void mqScalarsSmoothDialogReaction::showScalarsSmoothDialog(mqScalarsSmoothDialog *Smooth_dialog)
void mqScalarsSmoothDialogReaction::showScalarsSmoothDialog()
{
	vtkIdType num_surf = 0;
	num_surf = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
	if (num_surf == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}
	if ((mqMorphoDigCore::instance()->Getmui_ActiveArray()->DataType == VTK_FLOAT || mqMorphoDigCore::instance()->Getmui_ActiveArray()->DataType == VTK_DOUBLE) && mqMorphoDigCore::instance()->Getmui_ActiveArray()->NumComp == 1)
	{
		mqScalarsSmoothDialog mqSmooth(mqCoreUtilities::mainWidget());
		mqSmooth.exec();
	}
	else
	{
		QMessageBox msgBox;
		msgBox.setText("Active array must be a scalar array.");
		msgBox.exec();
		return;
	}
	
  //Smooth_dialog->show();
}
