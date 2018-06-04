/*=========================================================================

   Program: MorphoDig
   Module:    mqScalarsComplexityDialogReaction.h


========================================================================*/
#ifndef mqScalarsComplexityDialogReaction_h
#define mqScalarsComplexityDialogReaction_h

#include "mqReaction.h"
#include "mqScalarsComplexityDialog.h"
/**
* @ingroup Reactions
* mqScalarsComplexityDialogReaction used to show the standard Alpha change dialog for the
* application.
*/
class  mqScalarsComplexityDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqScalarsComplexityDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  //static void showScalarsComplexityDialog(mqScalarsComplexityDialog *FLG_dialog);
  static void showScalarsComplexityDialog();

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered() { mqScalarsComplexityDialogReaction::showScalarsComplexityDialog(); }
  
	//For an non modal dialog
	//virtual void onTriggered() { mqScalarsComplexityDialogReaction::showScalarsComplexityDialog(this->Complexity_dialog); }

private:
	Q_DISABLE_COPY(mqScalarsComplexityDialogReaction)
		//mqScalarsComplexityDialog *Complexity_dialog;
};

#endif
