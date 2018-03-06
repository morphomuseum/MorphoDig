/*=========================================================================

   Program: MorphoDig
   Module:    mqOrientationLabelsDialogReaction.h


========================================================================*/
#ifndef mqOrientationLabelsDialogReaction_h
#define mqOrientationLabelsDialogReaction_h

#include "mqReaction.h"
#include "mqOrientationLabelsDialog.h"
/**
* @ingroup Reactions
* mqOrientationLabelsDialogReaction used to show the standard Alpha change dialog for the
* application.
*/
class  mqOrientationLabelsDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqOrientationLabelsDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  static void showOrientationLabelsDialog(mqOrientationLabelsDialog *OrientationLabels_dialog);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { mqOrientationLabelsDialogReaction::showOrientationLabelsDialog(this->OrientationLabels_dialog); }

private:
	Q_DISABLE_COPY(mqOrientationLabelsDialogReaction)
		mqOrientationLabelsDialog *OrientationLabels_dialog;
};

#endif
