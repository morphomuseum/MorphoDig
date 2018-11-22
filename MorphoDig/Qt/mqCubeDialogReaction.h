/*=========================================================================

   Program: MorphoDig
   Module:    mqCubeDialogReaction.h


========================================================================*/
#ifndef mqCubeDialogReaction_h
#define mqCubeDialogReaction_h

#include "mqReaction.h"
#include "mqCubeDialog.h"
/**
* @ingroup Reactions
* mqCubeDialogReaction used to show the cubic/box-shaped 3D connective material.
* application.
*/
class  mqCubeDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqCubeDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  //static void showCubeDialog(mqCubeDialog *CUB_dialog);
  static void showCubeDialog();

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered() { mqCubeDialogReaction::showCubeDialog(); }
  
	//For an non modal dialog
	//virtual void onTriggered() { mqCubeDialogReaction::showCubeDialog(this->Cube_dialog); }

private:
	Q_DISABLE_COPY(mqCubeDialogReaction)
		//mqCubeDialog *Cube_dialog;
};

#endif
