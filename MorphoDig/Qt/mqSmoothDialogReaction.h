/*=========================================================================

   Program: MorphoDig
   Module:    mqSmoothDialogReaction.h


========================================================================*/
#ifndef mqSmoothDialogReaction_h
#define mqSmoothDialogReaction_h

#include "mqReaction.h"
#include "mqSmoothDialog.h"
/**
* @ingroup Reactions
* mqSmoothDialogReaction used to smooth selected surfaces
*/
class  mqSmoothDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqSmoothDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  static void showSmoothDialog(mqSmoothDialog *SMOOTH_dialog);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { mqSmoothDialogReaction::showSmoothDialog(this->SMOOTH_dialog); }

private:
	Q_DISABLE_COPY(mqSmoothDialogReaction)
		mqSmoothDialog *SMOOTH_dialog;
};

#endif
