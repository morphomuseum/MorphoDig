/*=========================================================================

   Program: MorphoDig
   Module:    mqDensifyDialogReaction.h


========================================================================*/
#ifndef mqDensifyDialogReaction_h
#define mqDensifyDialogReaction_h

#include "mqReaction.h"
#include "mqDensifyDialog.h"
/**
* @ingroup Reactions
* mqDensifyDialogReaction used to Densify selected surfaces
*/
class  mqDensifyDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqDensifyDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  static void showDensifyDialog(mqDensifyDialog *Densify_dialog);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { mqDensifyDialogReaction::showDensifyDialog(this->Densify_dialog); }

private:
	Q_DISABLE_COPY(mqDensifyDialogReaction)
		mqDensifyDialog *Densify_dialog;
};

#endif
