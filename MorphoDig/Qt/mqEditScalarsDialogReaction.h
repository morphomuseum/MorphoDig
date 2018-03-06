/*=========================================================================

   Program: MorphoDig
   Module:    mqEditScalarsDialogReaction.h


========================================================================*/
#ifndef mqEditScalarsDialogReaction_h
#define mqEditScalarsDialogReaction_h

#include "mqReaction.h"
#include "mqEditScalarsDialog.h"
/**
* @ingroup Reactions
* mqEditScalarsDialogReaction used to show the standard Scalars dialog for the
* application.
*/
class  mqEditScalarsDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqEditScalarsDialogReaction(QAction* parent);

  /**
  * Shows the Scalars dialog for the application.
  */
  static void showEditScalarsDialog(mqEditScalarsDialog *Scalars_dialog);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { mqEditScalarsDialogReaction::showEditScalarsDialog(this->Scalars_dialog); }

private:
	Q_DISABLE_COPY(mqEditScalarsDialogReaction)
		mqEditScalarsDialog *Scalars_dialog;
};

#endif
