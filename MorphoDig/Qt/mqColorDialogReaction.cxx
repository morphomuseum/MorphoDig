/*=========================================================================

   Program: MorphoDig
   Module:    mqColorDialogReaction.cxx


========================================================================*/
#include "mqColorDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqColorDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqColorDialogReaction::mqColorDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
}

//-----------------------------------------------------------------------------
void mqColorDialogReaction::showColorDialog()
{
	// fonction statique. C'est à dire que lorsqu'on clique sur "Color", on crée un nouvel objet.
	// la réaction, elle, est bien instanciée à la création du menu, mais pas la fenêtre. 

  mqColorDialog color_dialog(mqCoreUtilities::mainWidget());
  color_dialog.exec();
}
