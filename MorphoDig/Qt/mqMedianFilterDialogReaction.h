/*=========================================================================

   Program: MorphoDig
   Module:    mqMedianFilterDialogReaction.h


========================================================================*/
#ifndef mqMedianFilterDialogReaction_h
#define mqMedianFilterDialogReaction_h

#include "mqReaction.h"
#include "mqMedianFilterDialog.h"
#include "mqMorphoDigCore.h"
#include "vtkMDVolume.h"
#include <QMessageBox>

/**
* @ingroup Reactions
* mqMedianFilterDialogReaction used to create an MedianFilter from the 1st selected volume 
*/
class  mqMedianFilterDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqMedianFilterDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  static void showMedianFilterDialog(vtkMDVolume *vol);

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
	  mqMedianFilterDialogReaction::showMedianFilterDialog(vol); 
  
  }

private:
	Q_DISABLE_COPY(mqMedianFilterDialogReaction)
		//mqMedianFilterDialog *MedianFilter_dialog;
	
};

#endif
