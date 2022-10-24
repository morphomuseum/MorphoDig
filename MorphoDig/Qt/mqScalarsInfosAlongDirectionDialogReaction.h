/*=========================================================================

   Program: MorphoDig
   Module:    mqScalarsInfosAlongDirectionDialogReaction.h


========================================================================*/
#ifndef mqScalarsInfosAlongDirectionDialogReaction_h
#define mqScalarsInfosAlongDirectionDialogReaction_h

#include "mqReaction.h"
#include "mqScalarsInfosAlongDirectionDialog.h"
/**
* @ingroup Reactions
* mqScalarsInfosAlongDirectionDialogReaction used to compute Infos (mean variance etc.) about scalars
* application.
*/
class  mqScalarsInfosAlongDirectionDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqScalarsInfosAlongDirectionDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  //static void showScalarsInfosAlongDirectionDialog(mqScalarsInfosDialog *FLG_dialog);
  static void showScalarsInfosAlongDirectionDialog();

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered() { mqScalarsInfosAlongDirectionDialogReaction::showScalarsInfosAlongDirectionDialog(); }
  
	//For an non modal dialog
	//virtual void onTriggered() { mqScalarsInfosDialogReaction::showScalarsInfosAlongDirectionDialog(this->Infos_dialog); }

private:
	Q_DISABLE_COPY(mqScalarsInfosAlongDirectionDialogReaction)
		//mqScalarsInfosDialog *Infos_dialog;
};

#endif
