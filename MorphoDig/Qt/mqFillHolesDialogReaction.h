/*=========================================================================

   Program: MorphoDig
   Module:    mqFillHolesDialogReaction.h


========================================================================*/
#ifndef mqFillHolesDialogReaction_h
#define mqFillHolesDialogReaction_h

#include "mqReaction.h"
#include "mqFillHolesDialog.h"
/**
* @ingroup Reactions
* mqFillHolesDialogReaction used to Fill Holes of selected surfaces
*/
class  mqFillHolesDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqFillHolesDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  static void showFillHolesDialog(mqFillHolesDialog *FillHoles_dialog);

protected:
  /**
  * Called when the action is triggered.
  */
  virtual void onTriggered() { mqFillHolesDialogReaction::showFillHolesDialog(this->FillHoles_dialog); }

private:
	Q_DISABLE_COPY(mqFillHolesDialogReaction)
		mqFillHolesDialog *FillHoles_dialog;
};

#endif
