/*=========================================================================

   Program: MorphoDig
   Module:    mqEditVolumeDialogReaction.h


========================================================================*/
#ifndef mqEditVolumeDialogReaction_h
#define mqEditVolumeDialogReaction_h

#include "mqReaction.h"
#include "mqEditVolumeDialog.h"
/**
* @ingroup Reactions
* mqEditVolumeDialogReaction used to show the standard Volume dialog for the
* application.
*/
class  mqEditVolumeDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqEditVolumeDialogReaction(QAction* parent);

  /**
  * Shows the Volume dialog for the application.
  */
  static void showEditVolumeDialog(mqEditVolumeDialog *Volume_dialog);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { mqEditVolumeDialogReaction::showEditVolumeDialog(this->Volume_dialog); }

private:
	Q_DISABLE_COPY(mqEditVolumeDialogReaction)
		mqEditVolumeDialog *Volume_dialog;
};

#endif
