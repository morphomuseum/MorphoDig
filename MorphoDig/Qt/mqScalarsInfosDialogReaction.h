/*=========================================================================

   Program: MorphoDig
   Module:    mqScalarsInfosDialogReaction.h


========================================================================*/
#ifndef mqScalarsInfosDialogReaction_h
#define mqScalarsInfosDialogReaction_h

#include "mqReaction.h"
#include "mqScalarsInfosDialog.h"
/**
* @ingroup Reactions
* mqScalarsInfosDialogReaction used to compute Infos (mean variance etc.) about scalars
* application.
*/
class  mqScalarsInfosDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqScalarsInfosDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  //static void showScalarsInfosDialog(mqScalarsInfosDialog *FLG_dialog);
  static void showScalarsInfosDialog();

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered() { mqScalarsInfosDialogReaction::showScalarsInfosDialog(); }
  
	//For an non modal dialog
	//virtual void onTriggered() { mqScalarsInfosDialogReaction::showScalarsInfosDialog(this->Infos_dialog); }

private:
	Q_DISABLE_COPY(mqScalarsInfosDialogReaction)
		//mqScalarsInfosDialog *Infos_dialog;
};

#endif
