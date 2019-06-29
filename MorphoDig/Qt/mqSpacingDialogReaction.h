/*=========================================================================

   Program: MorphoDig
   Module:    mqSpacingDialogReaction.h


========================================================================*/
#ifndef mqSpacingDialogReaction_h
#define mqSpacingDialogReaction_h

#include "mqReaction.h"
#include "mqSpacingDialog.h"
#include "mqMorphoDigCore.h"
#include "vtkMDVolume.h"
#include <QMessageBox>

/**
* @ingroup Reactions
* mqSpacingDialogReaction used to change the voxel size of the 1st selected volume 
*/
class  mqSpacingDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqSpacingDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  static void showSpacingDialog(vtkMDVolume *vol);

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
	  mqSpacingDialogReaction::showSpacingDialog(vol); 
  
  }

private:
	Q_DISABLE_COPY(mqSpacingDialogReaction)
		//mqSpacingDialog *Spacing_dialog;
	
};

#endif
