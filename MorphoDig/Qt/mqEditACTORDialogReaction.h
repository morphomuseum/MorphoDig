/*=========================================================================

   Program: MorphoDig
   Module:    mqEditACTORDialogReaction.h


========================================================================*/
#ifndef mqEditACTORDialogReaction_h
#define mqEditACTORDialogReaction_h

#include "mqReaction.h"
#include "mqEditACTORDialog.h"
/**
* @ingroup Reactions
* mqEditACTORDialogReaction used to show the standard ACTOR dialog for the
* application.
*/
class  mqEditACTORDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqEditACTORDialogReaction(QAction* parent);

  /**
  * Shows the ACTOR dialog for the application.
  */
  static void showEditACTORDialog(mqEditACTORDialog *ACTOR_dialog);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { mqEditACTORDialogReaction::showEditACTORDialog(this->ACTOR_dialog); }

private:
	Q_DISABLE_COPY(mqEditACTORDialogReaction)
		mqEditACTORDialog *ACTOR_dialog;
};

#endif
