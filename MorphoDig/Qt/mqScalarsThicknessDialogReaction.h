/*=========================================================================

   Program: MorphoDig
   Module:    mqScalarsThicknessDialogReaction.h


========================================================================*/
#ifndef mqScalarsThicknessDialogReaction_h
#define mqScalarsThicknessDialogReaction_h

#include "mqReaction.h"
#include "mqScalarsThicknessDialog.h"
/**
* @ingroup Reactions
* mqScalarsThicknessDialogReaction used to show the standard Alpha change dialog for the
* application.
*/
class  mqScalarsThicknessDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqScalarsThicknessDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  //static void showScalarsThicknessDialog(mqScalarsThicknessDialog *FLG_dialog);
  static void showScalarsThicknessDialog();

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered() { mqScalarsThicknessDialogReaction::showScalarsThicknessDialog(); }
  
	//For an non modal dialog
	//virtual void onTriggered() { mqScalarsThicknessDialogReaction::showScalarsThicknessDialog(this->Thickness_dialog); }

private:
	Q_DISABLE_COPY(mqScalarsThicknessDialogReaction)
		//mqScalarsThicknessDialog *Thickness_dialog;
};

#endif
