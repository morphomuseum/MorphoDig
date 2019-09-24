/*=========================================================================

   Program: MorphoDig
   Module:    mqMedianFilterDialogReaction.cxx


========================================================================*/
#include "mqMedianFilterDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqMedianFilterDialog.h"
#include "mqReaction.h"

//-----------------------------------------------------------------------------
mqMedianFilterDialogReaction::mqMedianFilterDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	/* mqAboutDialog about_dialog(mqCoreUtilities::mainWidget());
  about_dialog.exec();*/
	//this->MedianFilter_dialog = new mqMedianFilterDialog(mqCoreUtilities::mainWidget());
	
	
}


//-----------------------------------------------------------------------------
void mqMedianFilterDialogReaction::showMedianFilterDialog(vtkMDVolume *vol)
{
	mqMedianFilterDialog MedianFilter_dialog(mqCoreUtilities::mainWidget());
	MedianFilter_dialog.setVolume(vol);
 MedianFilter_dialog.exec();
}
