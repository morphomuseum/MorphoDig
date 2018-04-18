/*=========================================================================

   Program: MorphoDig
   Module:    mqScalarsThicknessBetweenDialogReaction.h


========================================================================*/
#ifndef mqScalarsThicknessBetweenDialogReaction_h
#define mqScalarsThicknessBetweenDialogReaction_h

#include "mqReaction.h"
#include "mqScalarsThicknessBetweenDialog.h"
/**
* @ingroup Reactions
* mqScalarsThicknessBetweenDialogReaction used to show the standard Alpha change dialog for the
* application.
*/
class  mqScalarsThicknessBetweenDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqScalarsThicknessBetweenDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  //static void showScalarsThicknessBetweenDialog(mqScalarsThicknessBetweenDialog *FLG_dialog);
  static void showScalarsThicknessBetweenDialog();

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered() { mqScalarsThicknessBetweenDialogReaction::showScalarsThicknessBetweenDialog(); }
  
	//For an non modal dialog
	//virtual void onTriggered() { mqScalarsThicknessBetweenDialogReaction::showScalarsThicknessBetweenDialog(this->ThicknessBetween_dialog); }

private:
	Q_DISABLE_COPY(mqScalarsThicknessBetweenDialogReaction)
		//mqScalarsThicknessBetweenDialog *ThicknessBetween_dialog;
};

#endif
