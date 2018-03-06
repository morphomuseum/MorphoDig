/*=========================================================================

   Program: MorphoDig
   Module:    mqEditAllFLGColorDialogReaction.h


========================================================================*/
#ifndef mqEditAllFLGColorDialogReaction_h
#define mqEditAllFLGColorDialogReaction_h

#include "mqReaction.h"
#include "mqEditAllFLGColorDialog.h"
/**
* @ingroup Reactions
* mqEditAllFLGColorDialogReaction used to show the standard FLG dialog for the
* application.
*/
class  mqEditAllFLGColorDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqEditAllFLGColorDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  static void showEditAllFLGDialog(mqEditAllFLGColorDialog *FLG_dialog);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { mqEditAllFLGColorDialogReaction::showEditAllFLGDialog(this->FLG_dialog); }

private:
	Q_DISABLE_COPY(mqEditAllFLGColorDialogReaction)
		mqEditAllFLGColorDialog *FLG_dialog;
};

#endif
