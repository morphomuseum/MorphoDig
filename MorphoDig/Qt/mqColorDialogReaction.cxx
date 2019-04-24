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
	this->Color_dialog = new mqColorDialog(mqCoreUtilities::mainWidget());
}

//-----------------------------------------------------------------------------
void mqColorDialogReaction::showColorDialog(mqColorDialog *Color_dialog)
{
	// fonction statique. C'est à dire que lorsqu'on clique sur "Color", on crée un nouvel objet.
	// la réaction, elle, est bien instanciée à la création du menu, mais pas la fenêtre. 

  /*mqColorDialog color_dialog(mqCoreUtilities::mainWidget());
  color_dialog.exec();*/
	//Color_dialog->RefreshDialog();
	Color_dialog->show();
}
