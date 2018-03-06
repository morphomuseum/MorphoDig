/*=========================================================================

   Program: MorphoDig
   Module:    mqEditAlphaDialogReaction.h


========================================================================*/
#ifndef mqEditAlphaDialogReaction_h
#define mqEditAlphaDialogReaction_h

#include "mqReaction.h"
#include "mqEditAlphaDialog.h"
/**
* @ingroup Reactions
* mqEditAlphaDialogReaction used to show the standard Alpha change dialog for the
* application.
*/
class  mqEditAlphaDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqEditAlphaDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  static void showEditAlphaDialog(mqEditAlphaDialog *FLG_dialog);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { mqEditAlphaDialogReaction::showEditAlphaDialog(this->Alpha_dialog); }

private:
	Q_DISABLE_COPY(mqEditAlphaDialogReaction)
		mqEditAlphaDialog *Alpha_dialog;
};

#endif
