/*=========================================================================

   Program: MorphoDig
   Module:    mqScalarsInfosAlongDirectionDialogReaction.cxx


========================================================================*/
#include "mqScalarsInfosAlongDirectionDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqMorphoDigCore.h"
#include "mqScalarsInfosAlongDirectionDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqScalarsInfosAlongDirectionDialogReaction::mqScalarsInfosAlongDirectionDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	//this->Infos_dialog = new mqScalarsInfosAlongDirectionDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
//void mqScalarsInfosDialogReaction::showScalarsInfosDialog(mqScalarsInfosDialog *Infos_dialog)
void mqScalarsInfosAlongDirectionDialogReaction::showScalarsInfosAlongDirectionDialog()
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
		mqScalarsInfosAlongDirectionDialog mqInfosAlongDirection(mqCoreUtilities::mainWidget());
		mqInfosAlongDirection.exec();
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
