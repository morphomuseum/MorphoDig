/*=========================================================================

   Program: MorphoDig
   Module:    mqEditAllSurfacesColorDialogReaction.cxx


========================================================================*/
#include "mqEditAllSurfacesColorDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqMorphoDigCore.h"
#include "mqEditAllSurfacesColorDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqEditAllSurfacesColorDialogReaction::mqEditAllSurfacesColorDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	this->Surfaces_dialog = new mqEditAllSurfacesColorDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqEditAllSurfacesColorDialogReaction::showEditAllSurfacesDialog(mqEditAllSurfacesColorDialog *Surfaces_dialog)
{
	
	vtkIdType num_Surfaces = 0;
	num_Surfaces = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
	if (num_Surfaces == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surfaces selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}
  Surfaces_dialog->show();
}
