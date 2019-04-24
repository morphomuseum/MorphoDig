/*=========================================================================

   Program: MorphoDig
   Module:    mqColorDialogReaction.h


========================================================================*/
#ifndef mqColorDialogReaction_h
#define mqColorDialogReaction_h

#include "mqReaction.h"
#include "mqColorDialog.h"

/**
* @ingroup Reactions
* mqColorDialogReaction used to show the standard Color dialog for the
* application.
*/
class  mqColorDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqColorDialogReaction(QAction* parent);

  /**
  * Shows the Color dialog for the application.
  */
  static void showColorDialog(mqColorDialog *Color_dialog);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { mqColorDialogReaction::showColorDialog(this->Color_dialog); }

private:
  Q_DISABLE_COPY(mqColorDialogReaction)
	  mqColorDialog *Color_dialog;
};

#endif
