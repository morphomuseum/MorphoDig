/*=========================================================================

   Program: MorphoDig
   Module:    mqScalarsCurvatureDialogReaction.cxx


========================================================================*/
#include "mqScalarsCurvatureDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqMorphoDigCore.h"
#include "mqScalarsCurvatureDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqScalarsCurvatureDialogReaction::mqScalarsCurvatureDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	//this->Curvature_dialog = new mqScalarsCurvatureDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
//void mqScalarsCurvatureDialogReaction::showScalarsCurvatureDialog(mqScalarsCurvatureDialog *Curvature_dialog)
void mqScalarsCurvatureDialogReaction::showScalarsCurvatureDialog()
{
	vtkIdType num_surf = 0;
	num_surf = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
	if (num_surf == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}
	mqScalarsCurvatureDialog mqCurvature(mqCoreUtilities::mainWidget());
	mqCurvature.exec();
  
  //Curvature_dialog->show();
}
