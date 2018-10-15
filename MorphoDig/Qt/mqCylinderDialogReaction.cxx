/*=========================================================================

   Program: MorphoDig
   Module:    mqCylinderDialogReaction.cxx


========================================================================*/
#include "mqCylinderDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqMorphoDigCore.h"
#include "mqCylinderDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqCylinderDialogReaction::mqCylinderDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	//this->Cylinder_dialog = new mqCylinderDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
//void mqCylinderDialogReaction::showCylinderDialog(mqCylinderDialog *Cylinder_dialog)
void mqCylinderDialogReaction::showCylinderDialog()
{
	
		mqCylinderDialog mqCylinder(mqCoreUtilities::mainWidget());
		mqCylinder.exec();
	
	
  //Cylinder_dialog->show();
}
