/*=========================================================================

   Program: MorphoDig
   Module:    mqRendererOptionsDialogReaction.cxx


========================================================================*/
#include "mqRendererOptionsDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqRendererOptionsDialog.h"
#include "mqReaction.h"


//-----------------------------------------------------------------------------
mqRendererOptionsDialogReaction::mqRendererOptionsDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	

	
}

void mqRendererOptionsDialogReaction::onTriggered()
{
	mqRendererOptionsDialogReaction::showRendererOptionsDialog();

}
//-----------------------------------------------------------------------------
void mqRendererOptionsDialogReaction::showRendererOptionsDialog()
{
	
	mqRendererOptionsDialog RendererOptionsDialog(mqCoreUtilities::mainWidget());
	RendererOptionsDialog.exec();
  
}
