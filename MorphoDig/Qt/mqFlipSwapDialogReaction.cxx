/*=========================================================================

   Program: MorphoDig
   Module:    mqFlipSwapDialogReaction.cxx


========================================================================*/
#include "mqFlipSwapDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqFlipSwapDialog.h"
#include "mqReaction.h"

//-----------------------------------------------------------------------------
mqFlipSwapDialogReaction::mqFlipSwapDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	/* mqAboutDialog about_dialog(mqCoreUtilities::mainWidget());
  about_dialog.exec();*/
	//this->FlipSwap_dialog = new mqFlipSwapDialog(mqCoreUtilities::mainWidget());
	
	
}


//-----------------------------------------------------------------------------
void mqFlipSwapDialogReaction::showFlipSwapDialog(vtkMDVolume *vol)
{
	mqFlipSwapDialog FlipSwap_dialog(mqCoreUtilities::mainWidget());
	FlipSwap_dialog.setVolume(vol);
 FlipSwap_dialog.exec();
}
