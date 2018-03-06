/*=========================================================================

   Program: MorphoDig
   Module:    mqColorDialogReaction.cxx


========================================================================*/
#include "mqLandmarkDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqLandmarkDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqLandmarkDialogReaction::mqLandmarkDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
}

//-----------------------------------------------------------------------------
void mqLandmarkDialogReaction::showLandmarkDialog()
{
	// fonction statique. C'est à dire que lorsqu'on clique sur "Landmark", on crée un nouvel objet.
	// la réaction, elle, est bien instanciée à la création du menu, mais pas la fenêtre. 

  mqLandmarkDialog landmark_dialog(mqCoreUtilities::mainWidget());
  landmark_dialog.exec();
}
