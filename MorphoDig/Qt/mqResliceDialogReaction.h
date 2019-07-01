/*=========================================================================

   Program: MorphoDig
   Module:    mqResliceDialogReaction.h


========================================================================*/
#ifndef mqResliceDialogReaction_h
#define mqResliceDialogReaction_h

#include "mqReaction.h"
#include "mqResliceDialog.h"
#include "mqMorphoDigCore.h"
#include "vtkMDVolume.h"
#include <QMessageBox>

/**
* @ingroup Reactions
* mqResliceDialogReaction used to Reslice the 1st selected volume 
*/
class  mqResliceDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqResliceDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  static void showResliceDialog(vtkMDVolume *vol);

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
	  mqResliceDialogReaction::showResliceDialog(vol); 
  
  }

private:
	Q_DISABLE_COPY(mqResliceDialogReaction)
		//mqResliceDialog *Reslice_dialog;
	
};

#endif
