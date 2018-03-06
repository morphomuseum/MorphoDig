/*=========================================================================

   Program: MorphoDig
   Module:    mqEditAllFLGLengthDialogReaction.h


========================================================================*/
#ifndef mqEditAllFLGLengthDialogReaction_h
#define mqEditAllFLGLengthDialogReaction_h

#include "mqReaction.h"
#include "mqEditAllFLGLengthDialog.h"
/**
* @ingroup Reactions
* mqEditAllFLGLengthDialogReaction used to show the standard FLG dialog for the
* application.
*/
class  mqEditAllFLGLengthDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqEditAllFLGLengthDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  static void showEditAllFLGDialog(mqEditAllFLGLengthDialog *FLG_dialog);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { mqEditAllFLGLengthDialogReaction::showEditAllFLGDialog(this->FLG_dialog); }

private:
	Q_DISABLE_COPY(mqEditAllFLGLengthDialogReaction)
		mqEditAllFLGLengthDialog *FLG_dialog;
};

#endif
