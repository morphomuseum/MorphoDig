/*=========================================================================

   Program: MorphoDig
   Module:    mqICPDialogReaction.h


========================================================================*/
#ifndef mqICPDialogReaction_h
#define mqICPDialogReaction_h

#include "mqReaction.h"
#include "mqICPDialog.h"
/**
* @ingroup Reactions
* mqICPDialogReaction 
* application.
*/
class  mqICPDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqICPDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  //static void showICPDialog(mqICPDialog *FLG_dialog);
  static void showICPDialog();

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered() { mqICPDialogReaction::showICPDialog(); }
  
	//For an non modal dialog
	//virtual void onTriggered() { mqICPDialogReaction::showICPDialog(this->ThicknessBetween_dialog); }

private:
	Q_DISABLE_COPY(mqICPDialogReaction)
		//mqICPDialog *ThicknessBetween_dialog;
};

#endif
