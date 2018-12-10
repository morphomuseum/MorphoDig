/*=========================================================================

   Program: MorphoDig
   Module:    mqShrinkWrapIterativeDialogReaction.h


========================================================================*/
#ifndef mqShrinkWrapIterativeDialogReaction_h
#define mqShrinkWrapIterativeDialogReaction_h

#include "mqReaction.h"
#include "mqShrinkWrapIterativeDialog.h"
/**
* @ingroup Reactions
* mqShrinkWrapIterativeDialogReaction used to show the iterative shrink wrap dialog.
*/
class  mqShrinkWrapIterativeDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqShrinkWrapIterativeDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  //static void showShrinkWrapIterativeDialog(mqShrinkWrapIterativeDialog *FLG_dialog);
  static void showShrinkWrapIterativeDialog();

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered() { mqShrinkWrapIterativeDialogReaction::showShrinkWrapIterativeDialog(); }
  
	//For an non modal dialog
	//virtual void onTriggered() { mqShrinkWrapIterativeDialogReaction::showShrinkWrapIterativeDialog(this->ThicknessBetween_dialog); }

private:
	Q_DISABLE_COPY(mqShrinkWrapIterativeDialogReaction)
		//mqShrinkWrapIterativeDialog *ThicknessBetween_dialog;
};

#endif
