/*=========================================================================

   Program: MorphoDig
   Module:    mqScalarsDistanceDialogReaction.h


========================================================================*/
#ifndef mqScalarsDistanceDialogReaction_h
#define mqScalarsDistanceDialogReaction_h

#include "mqReaction.h"
#include "mqScalarsDistanceDialog.h"
/**
* @ingroup Reactions
* mqScalarsDistanceDialogReaction 
* application.
*/
class  mqScalarsDistanceDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqScalarsDistanceDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  //static void showScalarsDistanceDialog(mqScalarsDistanceDialog *FLG_dialog);
  static void showScalarsDistanceDialog();

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered() { mqScalarsDistanceDialogReaction::showScalarsDistanceDialog(); }
  
	//For an non modal dialog
	//virtual void onTriggered() { mqScalarsDistanceDialogReaction::showScalarsDistanceDialog(this->ThicknessBetween_dialog); }

private:
	Q_DISABLE_COPY(mqScalarsDistanceDialogReaction)
		//mqScalarsDistanceDialog *ThicknessBetween_dialog;
};

#endif
