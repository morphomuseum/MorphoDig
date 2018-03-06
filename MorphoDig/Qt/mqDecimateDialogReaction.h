/*=========================================================================

   Program: MorphoDig
   Module:    mqDecimateDialogReaction.h


========================================================================*/
#ifndef mqDecimateDialogReaction_h
#define mqDecimateDialogReaction_h

#include "mqReaction.h"
#include "mqDecimateDialog.h"
/**
* @ingroup Reactions
* mqDecimateDialogReaction used to Decimate selected surfaces
*/
class  mqDecimateDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqDecimateDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  static void showDecimateDialog(mqDecimateDialog *Decimate_dialog);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { mqDecimateDialogReaction::showDecimateDialog(this->Decimate_dialog); }

private:
	Q_DISABLE_COPY(mqDecimateDialogReaction)
		mqDecimateDialog *Decimate_dialog;
};

#endif
