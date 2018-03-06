/*=========================================================================

   Program: MorphoDig
   Module:    mqAboutDialogReaction.cxx


========================================================================*/
#include "mqAboutDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqAboutDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqAboutDialogReaction::mqAboutDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
}

//-----------------------------------------------------------------------------
void mqAboutDialogReaction::showAboutDialog()
{
	// fonction statique. C'est à dire que lorsqu'on clique sur "about", on crée un nouvel objet.
	// la réaction, elle, est bien instanciée à la création du menu, mais pas la fenêtre. 

  mqAboutDialog about_dialog(mqCoreUtilities::mainWidget());
  about_dialog.exec();
}
