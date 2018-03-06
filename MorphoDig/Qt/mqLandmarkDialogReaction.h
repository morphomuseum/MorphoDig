/*=========================================================================

   Program: MorphoDig
   Module:    mqLandmarkDialogReaction.h


========================================================================*/
#ifndef mqLandmarkDialogReaction_h
#define mqLandmarkDialogReaction_h

#include "mqReaction.h"

/**
* @ingroup Reactions
* mqLandmarkDialogReaction used to show the standard Landmark dialog for the
* application.
*/
class  mqLandmarkDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqLandmarkDialogReaction(QAction* parent);

  /**
  * Shows the Landmark dialog for the application.
  */
  static void showLandmarkDialog();

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { mqLandmarkDialogReaction::showLandmarkDialog(); }

private:
  Q_DISABLE_COPY(mqLandmarkDialogReaction)
};

#endif
