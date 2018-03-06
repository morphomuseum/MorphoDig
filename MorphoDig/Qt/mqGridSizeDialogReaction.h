/*=========================================================================

   Program: MorphoDig
   Module:    mqGridSizeDialogReaction.h


========================================================================*/
#ifndef mqGridSizeDialogReaction_h
#define mqGridSizeDialogReaction_h

#include "mqReaction.h"
#include "mqGridSizeDialog.h"
/**
* @ingroup Reactions
* mqGridSizeDialogReaction used to show the standard Alpha change dialog for the
* application.
*/
class  mqGridSizeDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqGridSizeDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  static void showGridSizeDialog(mqGridSizeDialog *GridSize_dialog);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { mqGridSizeDialogReaction::showGridSizeDialog(this->GridSize_dialog); }

private:
	Q_DISABLE_COPY(mqGridSizeDialogReaction)
		mqGridSizeDialog *GridSize_dialog;
};

#endif
