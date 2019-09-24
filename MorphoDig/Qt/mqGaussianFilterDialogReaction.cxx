/*=========================================================================

   Program: MorphoDig
   Module:    mqGaussianFilterDialogReaction.cxx


========================================================================*/
#include "mqGaussianFilterDialogReaction.h"
#include "mqCoreUtilities.h"
#include "mqGaussianFilterDialog.h"
#include "mqReaction.h"

//-----------------------------------------------------------------------------
mqGaussianFilterDialogReaction::mqGaussianFilterDialogReaction(QAction* parentObject)
  : Superclass(parentObject)
{
	/* mqAboutDialog about_dialog(mqCoreUtilities::mainWidget());
  about_dialog.exec();*/
	//this->GaussianFilter_dialog = new mqGaussianFilterDialog(mqCoreUtilities::mainWidget());
	
	
}


//-----------------------------------------------------------------------------
void mqGaussianFilterDialogReaction::showGaussianFilterDialog(vtkMDVolume *vol)
{
	mqGaussianFilterDialog GaussianFilter_dialog(mqCoreUtilities::mainWidget());
	GaussianFilter_dialog.setVolume(vol);
 GaussianFilter_dialog.exec();
}
