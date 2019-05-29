/*=========================================================================

   Program: MorphoDig
   Module:    mqIsosurfaceDialogReaction.h


========================================================================*/
#ifndef mqIsosurfaceDialogReaction_h
#define mqIsosurfaceDialogReaction_h

#include "mqReaction.h"
#include "mqIsosurfaceDialog.h"
#include "mqMorphoDigCore.h"
#include <QMessageBox>

/**
* @ingroup Reactions
* mqIsosurfaceDialogReaction used to create an Isosurface from the 1st selected volume 
*/
class  mqIsosurfaceDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqIsosurfaceDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  static void showIsosurfaceDialog(mqIsosurfaceDialog *Isosurface_dialog);

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
	  mqIsosurfaceDialogReaction::showIsosurfaceDialog(this->Isosurface_dialog); 
  
  }

private:
	Q_DISABLE_COPY(mqIsosurfaceDialogReaction)
		mqIsosurfaceDialog *Isosurface_dialog;
};

#endif
