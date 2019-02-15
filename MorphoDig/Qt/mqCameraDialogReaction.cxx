/*=========================================================================

   Program: MorphoDig
   Module:    mqColorDialogReaction.cxx


========================================================================*/
#include "mqCameraDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqCameraDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqCameraDialogReaction::mqCameraDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	this->cam_dialog = new mqCameraDialog(mqCoreUtilities::mainWidget());
}

//-----------------------------------------------------------------------------
void mqCameraDialogReaction::showCameraDialog(mqCameraDialog *cam_dialog)
{
	// fonction statique. C'est à dire que lorsqu'on clique sur "Camera", on crée un nouvel objet.
	// la réaction, elle, est bien instanciée à la création du menu, mais pas la fenêtre. 

  //mqCameraDialog Camera_dialog(mqCoreUtilities::mainWidget());
  ///Camera_dialog.exec();
	cam_dialog->RefreshDialog();
	cam_dialog->show();

}
