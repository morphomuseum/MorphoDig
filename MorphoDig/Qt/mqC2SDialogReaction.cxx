/*=========================================================================

   Program: MorphoDig
   Module:    mqC2SDialogReaction.cxx


========================================================================*/
#include "mqC2SDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqC2SDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqC2SDialogReaction::mqC2SDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	
	
}


//-----------------------------------------------------------------------------

void mqC2SDialogReaction::showC2SDialog()
{
	mqC2SDialog mqC2S(mqCoreUtilities::mainWidget());
	mqC2S.exec();
  
  //ThicknessBetween_dialog->show();
}
