/*=========================================================================

   Program: MorphoDig
   Module:    mqCameraDialogReaction.h


========================================================================*/
#ifndef mqCameraDialogReaction_h
#define mqCameraDialogReaction_h

#include "mqReaction.h"
#include "mqCameraDialog.h"

/**
* @ingroup Reactions
* mqCameraDialogReaction used to show the standard Camera dialog for the
* application.
*/
class  mqCameraDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqCameraDialogReaction(QAction* parent);

  /**
  * Shows the Camera dialog for the application.
  */
  static void showCameraDialog(mqCameraDialog *cam_dialog);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { 
	  mqCameraDialogReaction::showCameraDialog(this->cam_dialog);
  }

private:
  Q_DISABLE_COPY(mqCameraDialogReaction)
	  mqCameraDialog* cam_dialog;
};

#endif
