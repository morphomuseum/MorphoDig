/*=========================================================================

   Program: MorphoDig
   Module:    mqTPSDialogReaction.h


========================================================================*/
#ifndef mqTPSDialogReaction_h
#define mqTPSDialogReaction_h

#include "mqReaction.h"
#include "mqTPSDialog.h"
/**
* @ingroup Reactions
* mqTPSDialogReaction used to TPS selected surfaces
*/
class  mqTPSDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqTPSDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  static void showTPSDialog(mqTPSDialog *TPS_dialog);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { mqTPSDialogReaction::showTPSDialog(this->TPS_dialog); }

private:
	Q_DISABLE_COPY(mqTPSDialogReaction)
		mqTPSDialog *TPS_dialog;
};

#endif
