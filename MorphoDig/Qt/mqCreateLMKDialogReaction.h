/*=========================================================================

   Program: MorphoDig
   Module:    mqCreateLMKDialogReaction.h


========================================================================*/
#ifndef mqCreateLMKDialogReaction_h
#define mqCreateLMKDialogReaction_h

#include "mqReaction.h"
#include "mqCreateLMKDialog.h"
/**
* @ingroup Reactions
* mqCreateLMKDialogReaction used to show the standard Color dialog for the
* application.
*/
class  mqCreateLMKDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqCreateLMKDialogReaction(QAction* parent);

  /**
  * Shows the Color dialog for the application.
  */
  static void showCreateLMKDialog(mqCreateLMKDialog *lmk_dialog);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { mqCreateLMKDialogReaction::showCreateLMKDialog(this->lmk_dialog); }

private:
	Q_DISABLE_COPY(mqCreateLMKDialogReaction)
		mqCreateLMKDialog *lmk_dialog;
};

#endif
