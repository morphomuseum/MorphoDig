/*=========================================================================

   Program: MorphoDig
   Module:    mqScalarsInfosDialogReaction.cxx


========================================================================*/
#include "mqScalarsInfosDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqMorphoDigCore.h"
#include "mqScalarsInfosDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqScalarsInfosDialogReaction::mqScalarsInfosDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	//this->Infos_dialog = new mqScalarsInfosDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
//void mqScalarsInfosDialogReaction::showScalarsInfosDialog(mqScalarsInfosDialog *Infos_dialog)
void mqScalarsInfosDialogReaction::showScalarsInfosDialog()
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
		mqScalarsInfosDialog mqInfos(mqCoreUtilities::mainWidget());
		mqInfos.exec();
	}
	else
	{
		QMessageBox msgBox;
		msgBox.setText("Active array must be a scalar array.");
		msgBox.exec();
		return;
	}

	
  
  //Infos_dialog->show();
}
