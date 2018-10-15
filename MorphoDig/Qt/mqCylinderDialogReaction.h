/*=========================================================================

   Program: MorphoDig
   Module:    mqCylinderDialogReaction.h


========================================================================*/
#ifndef mqCylinderDialogReaction_h
#define mqCylinderDialogReaction_h

#include "mqReaction.h"
#include "mqCylinderDialog.h"
/**
* @ingroup Reactions
* mqCylinderDialogReaction used to show the cylinder 3D support material.
* application.
*/
class  mqCylinderDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqCylinderDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  //static void showCylinderDialog(mqCylinderDialog *FLG_dialog);
  static void showCylinderDialog();

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered() { mqCylinderDialogReaction::showCylinderDialog(); }
  
	//For an non modal dialog
	//virtual void onTriggered() { mqCylinderDialogReaction::showCylinderDialog(this->Cylinder_dialog); }

private:
	Q_DISABLE_COPY(mqCylinderDialogReaction)
		//mqCylinderDialog *Cylinder_dialog;
};

#endif
