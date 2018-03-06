/*=========================================================================

   Program: MorphoDig
   Module:    mqEditFLGDialogReaction.h


========================================================================*/
#ifndef mqEditFLGDialogReaction_h
#define mqEditFLGDialogReaction_h

#include "mqReaction.h"
#include "mqEditFLGDialog.h"
/**
* @ingroup Reactions
* mqEditFLGDialogReaction used to show the standard FLG dialog for the
* application.
*/
class  mqEditFLGDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqEditFLGDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  static void showEditFLGDialog(mqEditFLGDialog *FLG_dialog);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { mqEditFLGDialogReaction::showEditFLGDialog(this->FLG_dialog); }

private:
	Q_DISABLE_COPY(mqEditFLGDialogReaction)
		mqEditFLGDialog *FLG_dialog;
};

#endif
