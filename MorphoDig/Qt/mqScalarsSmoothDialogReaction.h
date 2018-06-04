/*=========================================================================

   Program: MorphoDig
   Module:    mqScalarsSmoothDialogReaction.h


========================================================================*/
#ifndef mqScalarsSmoothDialogReaction_h
#define mqScalarsSmoothDialogReaction_h

#include "mqReaction.h"
#include "mqScalarsSmoothDialog.h"
/**
* @ingroup Reactions
* mqScalarsSmoothDialogReaction used to show the standard scalar smoothing dialog for the
* application.
*/
class  mqScalarsSmoothDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqScalarsSmoothDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  //static void showScalarsSmoothDialog(mqScalarsSmoothDialog *FLG_dialog);
  static void showScalarsSmoothDialog();

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered() { mqScalarsSmoothDialogReaction::showScalarsSmoothDialog(); }
  
	//For an non modal dialog
	//virtual void onTriggered() { mqScalarsSmoothDialogReaction::showScalarsSmoothDialog(this->Smooth_dialog); }

private:
	Q_DISABLE_COPY(mqScalarsSmoothDialogReaction)
		//mqScalarsSmoothDialog *Smooth_dialog;
};

#endif
