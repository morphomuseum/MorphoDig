/*=========================================================================

   Program: MorphoDig
   Module:    mqIcosahedronDialogReaction.h


========================================================================*/
#ifndef mqIcosahedronDialogReaction_h
#define mqIcosahedronDialogReaction_h

#include "mqReaction.h"
#include "mqIcosahedronDialog.h"
/**
* @ingroup Reactions
* mqIcosahedronDialogReaction used to show the cubic/box-shaped 3D connective material.
* application.
*/
class  mqIcosahedronDialogReaction : public mqReaction
{
  Q_OBJECT
  typedef mqReaction Superclass;

public:
  mqIcosahedronDialogReaction(QAction* parent);

  /**
  * Shows the FLG dialog for the application.
  */
  //static void showIcosahedronDialog(mqIcosahedronDialog *CUB_dialog);
  static void showIcosahedronDialog();

protected:
  /**
  * Called when the action is triggered.
  */
	virtual void onTriggered() { mqIcosahedronDialogReaction::showIcosahedronDialog(); }
  
	//For an non modal dialog
	//virtual void onTriggered() { mqIcosahedronDialogReaction::showIcosahedronDialog(this->Icosahedron_dialog); }

private:
	Q_DISABLE_COPY(mqIcosahedronDialogReaction)
		//mqIcosahedronDialog *Icosahedron_dialog;
};

#endif
