/*=========================================================================

   Program: MorphoDig
   Module:    mqAboutDialogReaction.h


========================================================================*/
#ifndef mqAboutDialogReaction_h
#define mqAboutDialogReaction_h

#include "mqReaction.h"

/**
* @ingroup Reactions
* mqAboutDialogReaction used to show the standard about dialog for the
* application.
*/
class  mqAboutDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqAboutDialogReaction(QAction* parent);

  /**
  * Shows the about dialog for the application.
  */
  static void showAboutDialog();

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { mqAboutDialogReaction::showAboutDialog(); }

private:
  Q_DISABLE_COPY(mqAboutDialogReaction)
};

#endif
