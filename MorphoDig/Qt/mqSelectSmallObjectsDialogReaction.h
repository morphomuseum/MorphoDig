/*=========================================================================

   Program: MorphoDig
   Module:    mqSelectSmallObjectsDialogReaction.h


========================================================================*/
#ifndef mqSelectSmallObjectsDialogReaction_h
#define mqSelectSmallObjectsDialogReaction_h

#include "mqReaction.h"
#include "mqSelectSmallObjectsDialog.h"
/**
* @ingroup Reactions
* mqSelectSmallObjectsDialogReaction used to show the standard Alpha change dialog for the
* application.
*/
class  mqSelectSmallObjectsDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqSelectSmallObjectsDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  static void showSelectSmallObjectsDialog(mqSelectSmallObjectsDialog *FLG_dialog);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { mqSelectSmallObjectsDialogReaction::showSelectSmallObjectsDialog(this->SelectSmallObjects_dialog); }

private:
	Q_DISABLE_COPY(mqSelectSmallObjectsDialogReaction)
		mqSelectSmallObjectsDialog *SelectSmallObjects_dialog;
};

#endif
