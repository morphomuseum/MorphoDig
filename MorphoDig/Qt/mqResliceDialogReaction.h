/*=========================================================================

   Program: MorphoDig
   Module:    mqResampleDialogReaction.h


========================================================================*/
#ifndef mqResampleDialogReaction_h
#define mqResampleDialogReaction_h

#include "mqReaction.h"
#include "mqResampleDialog.h"
#include "mqMorphoDigCore.h"
#include "vtkMDVolume.h"
#include <QMessageBox>

/**
* @ingroup Reactions
* mqResampleDialogReaction used to resample the 1st selected volume 
*/
class  mqResampleDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqResampleDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  static void showResampleDialog(vtkMDVolume *vol);

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
	  mqResampleDialogReaction::showResampleDialog(vol); 
  
  }

private:
	Q_DISABLE_COPY(mqResampleDialogReaction)
		//mqResampleDialog *Resample_dialog;
	
};

#endif
