/*=========================================================================

   Program: MorphoDig
   Module:    mqGaussianFilterDialogReaction.h


========================================================================*/
#ifndef mqGaussianFilterDialogReaction_h
#define mqGaussianFilterDialogReaction_h

#include "mqReaction.h"
#include "mqGaussianFilterDialog.h"
#include "mqMorphoDigCore.h"
#include "vtkMDVolume.h"
#include <QMessageBox>

/**
* @ingroup Reactions
* mqGaussianFilterDialogReaction used to create an GaussianFilter from the 1st selected volume 
*/
class  mqGaussianFilterDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqGaussianFilterDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  static void showGaussianFilterDialog(vtkMDVolume *vol);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() {
	  vtkIdType num_selected_volumes = mqMorphoDigCore::instance()->getVolumeCollection()->GetNumberOfSelectedVolumes();
	  if (num_selected_volumes != 1) {
		  
		  QMessageBox msgBox;
		  msgBox.setText("Please select a single volume to use this option.");
		  msgBox.exec();
		  return;
	  }
	  vtkMDVolume *vol = mqMorphoDigCore::instance()->GetFirstSelectedVolume();
	  mqGaussianFilterDialogReaction::showGaussianFilterDialog(vol); 
  
  }

private:
	Q_DISABLE_COPY(mqGaussianFilterDialogReaction)
		//mqGaussianFilterDialog *GaussianFilter_dialog;
	
};

#endif
