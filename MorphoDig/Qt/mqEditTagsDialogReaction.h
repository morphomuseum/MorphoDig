/*=========================================================================

   Program: MorphoDig
   Module:    mqEditTagsDialogReaction.h


========================================================================*/
#ifndef mqEditTagsDialogReaction_h
#define mqEditTagsDialogReaction_h

#include "mqReaction.h"
#include "mqEditTagsDialog.h"
/**
* @ingroup Reactions
* mqEditTagsDialogReaction used to show the standard Tags dialog for the
* application.
*/
class  mqEditTagsDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqEditTagsDialogReaction(QAction* parent);

  /**
  * Shows the Tags dialog for the application.
  */
  static void showEditTagsDialog(mqEditTagsDialog *Tags_dialog);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { mqEditTagsDialogReaction::showEditTagsDialog(this->Tags_dialog); }

private:
	Q_DISABLE_COPY(mqEditTagsDialogReaction)
		mqEditTagsDialog *Tags_dialog;
};

#endif
