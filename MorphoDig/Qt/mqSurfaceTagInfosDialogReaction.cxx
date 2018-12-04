/*=========================================================================

   Program: MorphoDig
   Module:    mqSurfaceTagInfosDialogReaction.cxx


========================================================================*/
#include "mqSurfaceTagInfosDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqMorphoDigCore.h"
#include "mqSurfaceTagInfosDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqSurfaceTagInfosDialogReaction::mqSurfaceTagInfosDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	//this->Infos_dialog = new mqSurfaceTagInfosDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
//void mqSurfaceTagInfosDialogReaction::showSurfaceTagInfosDialog(mqSurfaceTagInfosDialog *Infos_dialog)
void mqSurfaceTagInfosDialogReaction::showSurfaceTagInfosDialog()
{
	vtkIdType num_surf = 0;
	num_surf = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
	if (num_surf == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}
	
	
  
  //Infos_dialog->show();
}
