/*=========================================================================

   Program: MorphoDig
   Module:    mqFillHolesDialogReaction.cxx


========================================================================*/
#include "mqFillHolesDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqFillHolesDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqFillHolesDialogReaction::mqFillHolesDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	this->FillHoles_dialog = new mqFillHolesDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------
void mqFillHolesDialogReaction::showFillHolesDialog(mqFillHolesDialog *FillHoles_dialog)
{
	
  
 FillHoles_dialog->show();
}
