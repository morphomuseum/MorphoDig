/*=========================================================================

   Program: MorphoDig
   Module:    mqOpenRawDialogReaction.cxx


========================================================================*/
#include "mqOpenRawDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqOpenRawDialog.h"
#include "mqReaction.h"

//-----------------------------------------------------------------------------
mqOpenRawDialogReaction::mqOpenRawDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	/* mqOpenRawDialog OpenRaw_dialog(mqCoreUtilities::mainWidget());
  OpenRaw_dialog.exec();*/
	//this->OpenRaw_dialog = new mqOpenRawDialog(mqCoreUtilities::mainWidget());
	
	
}


//-----------------------------------------------------------------------------
void mqOpenRawDialogReaction::showOpenRawDialog(QString fileName)
{
	mqOpenRawDialog OpenRaw_dialog(mqCoreUtilities::mainWidget());
	OpenRaw_dialog.setFileName(fileName);
 OpenRaw_dialog.exec();
}
