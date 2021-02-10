/*=========================================================================

   Program: MorphoDig
   Module:    mqScalarsVolumeDialogReaction.h


========================================================================*/
#ifndef mqScalarsVolumeDialogReaction_h
#define mqScalarsVolumeDialogReaction_h

#include "mqReaction.h"
#include "mqScalarsVolumeDialog.h"
/**
* @ingroup Reactions
* mqScalarsVolumeDialogReaction used to compute Volume scalars
* application.
*/
class  mqScalarsVolumeDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqScalarsVolumeDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  //static void showScalarsVolumeDialog(mqScalarsVolumeDialog *FLG_dialog);
  static void showScalarsVolumeDialog();

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered() { mqScalarsVolumeDialogReaction::showScalarsVolumeDialog(); }
  
	//For an non modal dialog
	//virtual void onTriggered() { mqScalarsVolumeDialogReaction::showScalarsVolumeDialog(this->Volume_dialog); }

private:
	Q_DISABLE_COPY(mqScalarsVolumeDialogReaction)
		//mqScalarsVolumeDialog *Volume_dialog;
};

#endif
