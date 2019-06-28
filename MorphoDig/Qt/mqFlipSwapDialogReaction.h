/*=========================================================================

   Program: MorphoDig
   Module:    mqFlipSwapDialogReaction.h


========================================================================*/
#ifndef mqFlipSwapDialogReaction_h
#define mqFlipSwapDialogReaction_h

#include "mqReaction.h"
#include "mqFlipSwapDialog.h"
#include "mqMorphoDigCore.h"
#include "vtkMDVolume.h"
#include <QMessageBox>

/**
* @ingroup Reactions
* mqFlipSwapDialogReaction used to flip or swap the 1st selected volume 
*/
class  mqFlipSwapDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqFlipSwapDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  static void showFlipSwapDialog(vtkMDVolume *vol);

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
	  mqFlipSwapDialogReaction::showFlipSwapDialog(vol); 
  
  }

private:
	Q_DISABLE_COPY(mqFlipSwapDialogReaction)
		//mqFlipSwapDialog *FlipSwap_dialog;
	
};

#endif
