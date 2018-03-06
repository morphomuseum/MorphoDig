/*=========================================================================

   Program: MorphoDig
   Module:    mqEditLMKDialogReaction.h


========================================================================*/
#ifndef mqEditLMKDialogReaction_h
#define mqEditLMKDialogReaction_h

#include "mqReaction.h"
#include "mqEditLMKDialog.h"
/**
* @ingroup Reactions
* mqEditLMKDialogReaction used to show the standard lmk dialog for the
* application.
*/
class  mqEditLMKDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqEditLMKDialogReaction(QAction* parent);

  /**
  * Shows the lmk dialog for the application.
  */
  static void showEditLMKDialog(mqEditLMKDialog *lmk_dialog);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { mqEditLMKDialogReaction::showEditLMKDialog(this->lmk_dialog); }

private:
	Q_DISABLE_COPY(mqEditLMKDialogReaction)
		mqEditLMKDialog *lmk_dialog;
};

#endif
