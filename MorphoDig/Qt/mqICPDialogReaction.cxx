/*=========================================================================

   Program: MorphoDig
   Module:    mqICPDialogReaction.cxx


========================================================================*/
#include "mqICPDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqICPDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqICPDialogReaction::mqICPDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	//this->ThicknessBetween_dialog = new mqICPDialog(mqCoreUtilities::mainWidget());

	
}


//-----------------------------------------------------------------------------

void mqICPDialogReaction::showICPDialog()
{
	mqICPDialog mqICP(mqCoreUtilities::mainWidget());
	mqICP.exec();
  
  //ThicknessBetween_dialog->show();
}
