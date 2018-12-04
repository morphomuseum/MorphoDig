/*=========================================================================

   Program: MorphoDig
   Module:    mqCubeDialogReaction.cxx


========================================================================*/
#include "mqCubeDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqMorphoDigCore.h"
#include "mqCubeDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqCubeDialogReaction::mqCubeDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	//this->Cube_dialog = new mqCubeDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
//void mqCubeDialogReaction::showCubeDialog(mqCubeDialog *Cube_dialog)
void mqCubeDialogReaction::showCubeDialog()
{
	
		mqCubeDialog mqCube(mqCoreUtilities::mainWidget());
		mqCube.exec();
	
	
  //Cube_dialog->show();
}
