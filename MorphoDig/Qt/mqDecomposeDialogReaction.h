/*=========================================================================

   Program: MorphoDig
   Module:    mqDecomposeDialogReaction.h


========================================================================*/
#ifndef mqDecomposeDialogReaction_h
#define mqDecomposeDialogReaction_h

#include "mqReaction.h"
#include "mqDecomposeDialog.h"
/**
* @ingroup Reactions
* mqDecomposeDialogReaction used to Decompose selected surfaces
*/
class  mqDecomposeDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqDecomposeDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  static void showDecomposeDialog(mqDecomposeDialog *Decompose_dialog);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { mqDecomposeDialogReaction::showDecomposeDialog(this->Decompose_dialog); }

private:
	Q_DISABLE_COPY(mqDecomposeDialogReaction)
		mqDecomposeDialog *Decompose_dialog;
};

#endif
